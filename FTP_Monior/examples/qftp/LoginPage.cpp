#include "LoginPage.h"
#include "ui_LoginPage.h"
#include"context/Context.h"
#include"ConfigureManager.h"
#include"ConfigureReaderAndWriter.h"

#include<QMessageBox>
#include <QUrl>

#include <QtNetwork/QFtp>
#define MAX_RETRY_LOGIN 3

LoginPage::LoginPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    setWindowTitle("欢迎登录进度监控工具");
    ui->edPassword->setEchoMode(QLineEdit::Password);

    if(ConfigureManager::getInstance()->getAutoLogin()){
        ui->rdRemeberMe->setChecked(true);
        ui->edUserName->setText(Context::getInstance()->getUserInfo().getUserName());
        ui->edPassword->setText(Context::getInstance()->getUserInfo().getPassword());
    }

    retryTime = 0 ;

    connect(ui->btnLogin,SIGNAL(clicked(bool)),this,SLOT(loginClicked()));

}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::loginClicked(){
    if( retryTime >= MAX_RETRY_LOGIN )
        reject();
    QString userName = ui->edUserName->text();
    QString password = ui->edPassword->text();

    Context::getInstance()->getUserInfo().setUserName(userName);
    Context::getInstance()->getUserInfo().setPassword(password);

    QFtp *ftp = new QFtp(this);

    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(loginCommand(int,bool)));

    ftp->connectToHost(Context::getInstance()->getHostName(),21);

    ftp->login(Context::getInstance()->getUserInfo().getUserName(),
               Context::getInstance()->getUserInfo().getPassword());
}

void LoginPage::loginCommand(int commandID, bool error)
{
    if(commandID == 2){
        if(error){
            loginSuccess = false;
            retryTime ++;
            QMessageBox::information(NULL,"用户/密码错误","您输入的账号或者密码错误，请重新输入");
        }else{
            loginSuccess = true;
        }

        if(loginSuccess){
            if(ui->rdRemeberMe->isChecked()){
                ConfigureManager::getInstance()->setAutoLgoin(true);
            }else{
                ConfigureManager::getInstance()->setAutoLgoin(false);
            }

            ConfigureReaderAndWriter writer("configure.xml");
            writer.save();
            accept();
        }
    }

}
