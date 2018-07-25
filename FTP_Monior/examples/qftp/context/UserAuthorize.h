#ifndef USERAUTHORIZE_H
#define USERAUTHORIZE_H

#include<QString>

class UserAuthorize
{
public:
    UserAuthorize(){}


    QString getUserName() {
        return userName;
    }
    void setUserName(QString userName) {
        this->userName = userName;
    }
    QString getUserId() {
        return userId;
    }
    void setUserId(QString userId) {
        this->userId = userId;
    }
    QString getPassword() {
        return password;
    }
    void setPassword(QString password) {
        this->password = password;
    }

    QString getToken() {
        return token;
    }
    void setToken(QString token) {
        this->token = token;
    }

private:
    QString userName;
    QString userId;
    QString password;
    QString token;


};


#endif // USERAUTHORIZE_H
