#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDialog>
#include <QtNetwork/QFtp>

namespace Ui {
class LoginPage;
}

class LoginPage : public QDialog
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = 0);
    ~LoginPage();

public slots:
    void loginClicked();
    void loginCommand(int commandID, bool error);

private:
    Ui::LoginPage *ui;
    int retryTime;
    bool loginSuccess=false;
};

#endif // LOGINPAGE_H
