#ifndef CONTEXT_H
#define CONTEXT_H
#include<QString>
#include"UserAuthorize.h"
class Context
{
public:
    static Context *getInstance();

    QString getRootPath() const{
        return rootPath;
    }

    void setRootPath(QString path){
        this->rootPath = path;
    }

    QString getHostName() const {
        return hostName;
    }
    void setHostName(QString hostName ) {
        this->hostName = hostName;
    }
    UserAuthorize & getUserInfo(){
        return userAuthorize;
    }    

private:
    static Context * instance;

    Context();
private:
    QString rootPath;
    QString hostName;
    UserAuthorize userAuthorize;

};





#endif // CONTEXT_H
