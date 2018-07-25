#ifndef FTPWINDOW_H
#define FTPWINDOW_H

#include <QDialog>
#include <QHash>
#include <QNetworkConfigurationManager>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFile;
class QFtp;
class QLabel;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QProgressDialog;
class QPushButton;
class QUrlInfo;
class QNetworkSession;
QT_END_NAMESPACE

class FtpWindow : public QDialog
{
    Q_OBJECT

public:
    FtpWindow(QWidget *parent = 0);
    QSize sizeHint() const;


private slots:
    void downloadFile();
    void connectToFtp();
    void ftpCommandFinished(int commandId, bool error);
    void addToList(const QUrlInfo &urlInfo);
    void processItem(QTreeWidgetItem *item, int column);
    void cdToParent();
    void enableDownloadButton();


private:
    QLabel *statusLabel;
    QTreeWidget *fileList;
    QPushButton *cdToParentButton;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QHash<QString, bool> isDirectory;
    QString currentPath;
    QFtp *ftp;
    QFile *file;
};

#endif
