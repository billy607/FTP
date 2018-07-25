#ifndef LOGEVENT_H
#define LOGEVENT_H

#include<QString>
#include<QJsonDocument>
#include"task/OnlineTask.h"
#include<QTimeZone>
#include"context/Context.h"
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include "labelingbase_global.h"
namespace labelingbase {

class LABELINGBASESHARED_EXPORT LogEvent
{
public:
    enum Type{Type_StartLabeling,Type_DataDownloading,Type_DataDownloded,Type_PackageLoaded,Type_Started,Type_ResultUpload,
             Type_PackageDeleted, Type_Operation};
    LogEvent(Type type){
        this->type = type;
    }

    Type getType() const{
        return type;
    }

    QString getURL() const {
        switch(type){
        case Type_StartLabeling:
            return "https://label.bj.sensetime.com/v3/start-label";
        case Type_DataDownloading:
        case Type_DataDownloded:
        case Type_PackageLoaded:
        case Type_ResultUpload:
        case Type_Started:
        case Type_PackageDeleted:
            return "https://label.bj.sensetime.com/v3/annotation-log";
        case Type_Operation:
            return "https://label.bj.sensetime.com/v3/annotation-log";

        default:
            Q_ASSERT(false);
            return "";
        }
        return "";
    }

    template<class T>
    QString getPostFields(Context<T> * context,OnlineTask & task , Package & package){
        if( type == Type_StartLabeling ){
            return QString("packageID=%1&token=%2").arg(package.getPackageId()).arg(context->getUserInfo().getToken());
        }else if(type > Type_StartLabeling && type < Type_Operation ){
            QJsonObject object;
            object.insert("taskName",task.getTaskName());
            object.insert("userName",context->getUserInfo().getUserName());
            object.insert("taskID",task.getTaskId().toInt());
            object.insert("packageID",package.getPackageId().toInt());
            object.insert("type","anno_event");
            object.insert("eventName",getEventName());
//            int actionId = package.getActionId().toInt();
//            QJsonValue value(actionId);
            object.insert("actionID",package.getActionId());
            object.insert("userId",context->getUserInfo().getUserId().toInt());
            QString ts = QDateTime::currentDateTime().toString("yyyy-M-dThh:mm:ss.z")+ "+08:00";
            object.insert("ts",ts);

            QJsonDocument document;
            document.setObject(object);
            return QString (document.toJson());
        }else if(type ==Type_Operation ){
            QJsonObject object;
            object.insert("taskName",task.getTaskName());
            object.insert("userName",context->getUserInfo().getUserName());
            object.insert("taskID",task.getTaskId().toInt());
            object.insert("packageID",package.getPackageId().toInt());
            object.insert("type","anno_op");
            object.insert("eventName",getEventName());
            object.insert("actionID",package.getActionId());
            object.insert("userId",context->getUserInfo().getUserId().toInt());
            object.insert("actionName",getActionName(package.getActionId().toInt()));
            QString ts = QDateTime::currentDateTime().toString("yyyy-M-dThh:mm:ss.z")+ "+08:00";
            object.insert("ts",ts);

            QJsonDocument document;
            document.setObject(object);
            return QString (document.toJson());
        }

        return "";
    }

    //follow before
    QString getEventName(){
        switch(type){
        case Type_DataDownloading:
            return "package_download";
        case Type_PackageLoaded:
            return "package_load";
        case Type_ResultUpload:
            return "result_upload";
        case Type_PackageDeleted:
            return "package_delete";
        case Type_Started:
            return "package_start";
        case Type_DataDownloded:
            return "package_download_finished";
        default:
            break;
        }

        Q_ASSERT(false);
        return "";
    }

    QString getActionName(int actionId){
        if(actionId  == 0 )
            return "label";
        else if(actionId %2 == 0)
            return "rework";
        else
            return "check";
    }

private:
    Type type;
};


}

#endif // LOGEVENT_H
