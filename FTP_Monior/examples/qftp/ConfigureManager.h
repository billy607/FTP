#ifndef CONFIGUREMANAGER_H
#define CONFIGUREMANAGER_H

#include<QString>
class ConfigureManager
{
public:
    ConfigureManager();

    static ConfigureManager* getInstance() ;

    void init(QString configurePath);

    bool getAutoLogin() const{
        return autoLogin;
    }
    void setAutoLgoin(bool autoLogin){
        this->autoLogin = autoLogin;
    }

private :
    static ConfigureManager * instance;

    bool autoLogin;
};

#endif // CONFIGUREMANAGER_H
