#include <QtWidgets>
#include <QtNetwork>
#include <QFtp>

#include "ftpwindow.h"
#include "context/Context.h"
#include "zip/ZipUtil.h"

QString FromSpecialEncoding(const QString &InputStr)
{
    return  QString::fromLocal8Bit(InputStr.toLatin1());
}

QString ToSpecialEncoding(const QString &InputStr)
{
    return  QString::fromLatin1(InputStr.toLocal8Bit());
}

FtpWindow::FtpWindow(QWidget *parent)
    : QDialog(parent), ftp(0)
{
    statusLabel = new QLabel();

    fileList = new QTreeWidget;
    fileList->setEnabled(false);
    fileList->setRootIsDecorated(false);
    fileList->setHeaderLabels(QStringList() << tr("名称") << tr("大小") << tr("所有者") << tr("组") << tr("修改时间"));
    fileList->header()->setStretchLastSection(false);
    fileList->setColumnWidth(0,250);

    cdToParentButton = new QPushButton;
    cdToParentButton->setIcon(QPixmap(":/images/cdtoparent.png"));
    cdToParentButton->setEnabled(false);

    downloadButton = new QPushButton(tr("解析"));
    downloadButton->setEnabled(false);

    quitButton = new QPushButton(tr("退出"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cdToParentButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(fileList, SIGNAL(itemActivated(QTreeWidgetItem*,int)),
            this, SLOT(processItem(QTreeWidgetItem*,int)));
    connect(fileList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(enableDownloadButton()));

    connect(cdToParentButton, SIGNAL(clicked()), this, SLOT(cdToParent()));
    connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadFile()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fileList);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("FTP"));

    connectToFtp();
}

QSize FtpWindow::sizeHint() const
{
    return QSize(700, 300);
}

void FtpWindow::connectToFtp()
{
    ftp = new QFtp(this);
    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftp, SIGNAL(listInfo(QUrlInfo)),
            this, SLOT(addToList(QUrlInfo)));

    fileList->clear();
    currentPath.clear();
    isDirectory.clear();

    ftp->connectToHost(Context::getInstance()->getHostName(),21);

    ftp->login(Context::getInstance()->getUserInfo().getUserName(),
               Context::getInstance()->getUserInfo().getPassword());

    fileList->setEnabled(true);
}

void FtpWindow::downloadFile()
{
    QString fileName = fileList->currentItem()->text(0);

    if (QFile::exists(fileName)) {
        QMessageBox::information(this, tr("FTP"),
                                 tr("There already exists a file called %1 in "
                                    "the current directory.")
                                 .arg(fileName));
        return;
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("FTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        delete file;
        return;
    }

    ftp->get(fileList->currentItem()->text(0), file);

    downloadButton->setEnabled(false);

}

void FtpWindow::ftpCommandFinished(int, bool error)
{
#ifndef QT_NO_CURSOR
    setCursor(Qt::ArrowCursor);
#endif

    if (ftp->currentCommand() == QFtp::ConnectToHost) {
        if (error) {
            QMessageBox::information(this, tr("FTP"),
                                     tr("无法连接到FTP服务器，请寻找相关IT人员"));
            return;
        }
        statusLabel->setText(tr("Logged onto %1.")
                             .arg(Context::getInstance()->getHostName()));
        fileList->setFocus();
        downloadButton->setDefault(true);
        return;
    }

    if (ftp->currentCommand() == QFtp::Login)
        ftp->list();

    if (ftp->currentCommand() == QFtp::Get) {
        if (error) {
            QMessageBox::critical(this, tr("下载发生错误"),
                                     tr("下载%1时发生错误，已取消下载").arg(file->fileName()));
            file->close();
            file->remove();
        } else {
            statusLabel->setText(tr("已下载%1至根目录")
                                 .arg(file->fileName()));

            file->close();
        }
        delete file;
        enableDownloadButton();

    } else if (ftp->currentCommand() == QFtp::List) {
        if (isDirectory.isEmpty()) {
            fileList->addTopLevelItem(new QTreeWidgetItem(QStringList() << tr("<empty>")));
            fileList->setEnabled(false);
        }
    }
}

void FtpWindow::addToList(const QUrlInfo &urlInfo)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, FromSpecialEncoding(urlInfo.name()));
    item->setText(1, QString::number(urlInfo.size()));
    item->setText(2, urlInfo.owner());
    item->setText(3, urlInfo.group());
    item->setText(4, urlInfo.lastModified().toString("MMM dd yyyy"));

    QPixmap pixmap(urlInfo.isDir() ? ":/images/dir.png" : ":/images/file.png");
    item->setIcon(0, pixmap);

    isDirectory[urlInfo.name()] = urlInfo.isDir();
    fileList->addTopLevelItem(item);
    if (!fileList->currentItem()) {
        fileList->setCurrentItem(fileList->topLevelItem(0));
        fileList->setEnabled(true);
    }
}

void FtpWindow::processItem(QTreeWidgetItem *item, int /*column*/)
{
    QString name = ToSpecialEncoding(item->text(0));
    if (isDirectory.value(name)) {
        fileList->clear();
        isDirectory.clear();
        currentPath += '/';
        currentPath += name;
        ftp->cd(name);
        ftp->list();
        cdToParentButton->setEnabled(true);
#ifndef QT_NO_CURSOR
        setCursor(Qt::WaitCursor);
#endif
        return;
    }
}

void FtpWindow::cdToParent()
{
#ifndef QT_NO_CURSOR
    setCursor(Qt::WaitCursor);
#endif
    fileList->clear();
    isDirectory.clear();
    currentPath = currentPath.left(currentPath.lastIndexOf('/'));
    if (currentPath.isEmpty()) {
        cdToParentButton->setEnabled(false);
        ftp->cd("/");
    } else {
        ftp->cd(currentPath);
    }
    ftp->list();
}

void FtpWindow::enableDownloadButton()
{
    QTreeWidgetItem *current = fileList->currentItem();
    if (current) {
        QString currentFile = current->text(0);
        downloadButton->setEnabled(!isDirectory.value(ToSpecialEncoding(currentFile)));
    } else {
        downloadButton->setEnabled(false);
    }
}
//![14]



