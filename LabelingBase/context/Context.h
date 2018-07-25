#ifndef CONTEXT_H
#define CONTEXT_H
#include<QString>
#include"UserAuthorize.h"
#include"labelingbase_global.h"
namespace labelingbase {
#define JOB_CONFIG_FILE_NAME "job.xml"
template<typename T> class LABELINGBASESHARED_EXPORT Context
{
public:
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


    QString getDataPath(QString taskId, QString packageId) {
        return  "jobs/" + taskId + "/" + packageId;
    }

public:
    static T * getInstance() {
        if( !instance ){
            instance = new T();
        }
        return instance;
    }

protected:
    Context(){
        rootPath = ".";
        hostName = "https://label.bj.sensetime.com";
    }
private:
    static T * instance;




private:
    QString rootPath;
    QString hostName;
    UserAuthorize userAuthorize;

};

}


#endif // CONTEXT_H
