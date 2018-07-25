#include "HttpUtil.h"
#include<QCryptographicHash>
#include<QDebug>
#include<QJsonDocument>
#include<QJsonObject>
#include<QTextStream>
#include<QByteArray>
#include<QJsonArray>
#include<QFileInfo>
#include<QDir>

#include"task/OnlineTask.h"
#include"LogUtil.h"
#include"LogEvent.h"

#include"CURLUtil.h"

#include"context/ManagerContext.h"
#include"context/ToolContext.h"
namespace labelingbase {

#define ADDRESS_ONLINE_PACKAGES "/v1/get-tasks"


HttpUtil::HttpUtil()
{

}

HttpUtil::~HttpUtil()
{

}

template<class T>   bool login(Context<T> * context){
    QString hostName = context->getHostName()+ "/v1/login";

    std::string strPassword = context->getUserInfo().getPassword().toStdString();
    QByteArray password= QCryptographicHash::hash(
                QByteArray(strPassword.c_str(),strPassword.size())
                ,QCryptographicHash::Md5);

    QString userInfo =  QString("username=%1&pwd=%2")
            .arg(context->getUserInfo().getUserName())
            .arg(QString(password.toHex()));


    CURLUtil curl(hostName,userInfo);
    CRULResult result = curl.sendRequest();

    if(result.isSuccess()){
        QJsonDocument document = QJsonDocument::fromJson(QByteArray(result.getData()->getBuffer(),result.getData()->getSize()));
        QJsonObject object = document.object();
        if(document.isNull() || document.isEmpty() || !object.contains("token")
                || !object.contains("userId") ){

            LogUtil::critical(QString("%1 \n %2").arg("can not find userId field or token ")
                              .arg(QString(result.getData()->getBuffer()))) ;
            return false;
        }else{
            context->getUserInfo().setToken(object.value("token").toString());
            context->getUserInfo().setUserId(QString("%1").arg(object.value("userId").toInt()));
        }

    }else{
        LogUtil::debug(result.getErrorMessage()) ;
        return false;
    }

    return true;
}

bool  HttpUtil::login(ManagerContext *context){
    return labelingbase::login(context);
}
bool HttpUtil::login(ToolContext *context){
    return labelingbase::login(context);
}

template<class T>
static bool getOnlineTasks(Context<T> * context ,QVector<OnlineTask> & tasks){

    QString hostName = context->getHostName()+"/v1/get-tasks";

    QString postData = QString("userId=%1").arg(context->getUserInfo().getUserId());

    CURLUtil curl(hostName,postData,"GET");
    CRULResult result = curl.sendRequest();
    if(result.isSuccess()){
        QJsonDocument document = QJsonDocument::fromJson(QByteArray(result.getData()->getBuffer(),result.getData()->getSize()));
        QJsonArray array = document.array();
        for(int index = 0 ; index < array.size(); index ++ ){
            QJsonObject object = array.at(index).toObject();
            OnlineTask task;
            if(!object.contains("taskId") || !object.contains("taskName")||! object.contains("toolId")||!object.contains("toolName")
                    || !object.contains("packages")){
                LogUtil::critical("can not load getOnlinePackets : online task format error");
                return false;
            }
            task.setTaskId(QString("%1").arg(object.value("taskId").toInt()));
            task.setTaskName(object.value("taskName").toString());
            task.setSource(object.value("source").toString());
            task.setToolId(object.value("toolId").toString());
            task.setToolVersion(object.value("toolVersion").toString());
            task.setToolName(object.value("toolName").toString());
            task.setToolObject(object.value("toolObject").toString());

            QJsonArray packages = object.value("packages").toArray();
            for(int jndex = 0 ; jndex < packages.size(); jndex ++){
                QJsonObject packageObject = packages.at(jndex).toObject();

                if(!packageObject.contains("packageId") || !packageObject.contains("packageName")
                        || !packageObject.contains("actionId") ||  !packageObject.contains("type")){
                    LogUtil::critical("can not load getOnlinePackets : package format error ") ;
                    return false;
                }
                Package package;
                package.setPackageId(QString("%1").arg(packageObject.value("packageId").toInt()));
                package.setPackageName(packageObject.value("packageName").toString());
                package.setPackageUrl(packageObject.value("packageUrl").toString());
                package.setActionId(QString("%1").arg(packageObject.value("actionId").toInt()));
                package.setType(packageObject.value("type").toString());

                QJsonArray array  = packageObject.value("packagePatchesUrl").toArray();
                QStringList patchageURLs ;
                for(int index = 0 ; index < array.size() ; index ++ ){
                    patchageURLs.append(array.at(index).toString());
                }

                package.setPackagePatchesUrl(patchageURLs);
                task.addPackage(package);
            }
            tasks.append(task);
        }
    }else{

        if(result.isTookenExpire()){
            if(! labelingbase::login(context)){
                LogUtil::info("uploadLogEvent can not refresh token , login failed ");
                return false;
            }
            return getOnlineTasks( context, tasks);
        }else{
            LogUtil::warning(QString(QString("getOnlineTasks : %1").arg(result.getErrorMessage())));
            return false;
        }
    }
    return true;
}

bool HttpUtil::initTaskInfomation(ToolContext *context, QString taskId, QString packageId){
    QVector<OnlineTask> tasks ;
    if(!getOnlineTasks(context, tasks)){
        return false;
    }

    for(OnlineTask task : tasks ){
        if(task.getTaskId() == taskId){
            QVector<Package> packages = task.getPackages() ;
            auto iterator = packages.begin();//删除所有其他的package
            while(iterator != packages.end()){
                if(iterator->getPackageId() != packageId ){
                    iterator = packages.erase(iterator);
                }else{
                    iterator ++ ;
                }
            }
            if(packages.isEmpty()){
                return false;
            }
            task.setPackage(packages);
            context->setTask(task);
            return  true;
        }
    }
    return false;

}

bool TaskIdLessThan(const OnlineTask &task1, const OnlineTask &task2)
{
    return task1.getTaskId() < task2.getTaskId();
}


bool HttpUtil::initOnlinePackets(ManagerContext *context ){

    QVector<OnlineTask> tasks ;
    if(!getOnlineTasks(context, tasks)){
        return false;
    }

    qSort(tasks.begin(), tasks.end(), TaskIdLessThan);
    context->setTasks(tasks);

    return true;
}


//bool downloadSwiftPatchPackage(ContextManager &context ,Package &package,QString filePath ){

//    Q_ASSERT(!package.getPackagePatchesUrl().isEmpty());
//    QString lastPatch = package.getPackagePatchesUrl().last();
//    QString url  = context.getHostName() + lastPatch + "?token=" + context.getUserInfo().getToken();

//    auto fuction = [](void * , double value1 ,double value2 , double value3 ,double value4){
//        LogUtil::debug(QString("%1 \t %2 \t \%3 \t %4").arg(value1,0,'f',2).arg(value2,0,'f',2)
//                       .arg(value3,0,'f',2).arg(value4,0,'f',2));
//                       };

//    sendResult result = sendRequest(url,"GET","",&fuction);

//    if(result.isSuccess()){
//        QFile file(filePath) ;
//        if(!file.open(QIODevice::WriteOnly)){
//            LogUtil::critical(QString("can not create file %1").arg(filePath));
//            return false;
//        }
//        file.write(result.getData()->getBuffer(),result.getData()->getSize());
//        file.close();
//    }else{
//        LogUtil::warning(QString(QString("downloadSwiftPatchPackage : %1").arg(result.getErrorMessage())));
//        return false;
//    }

//    return true;
//}

bool downloadFile(ManagerContext *context , QString url , QString methodName, QString postFiled ,
                  loadProgress progress , void * param
                  ,QString filePath){
    DownloadMethod method(progress, param,filePath);
    CURLUtil util(url,postFiled,methodName);
    CRULResult result = util.downloadFile(method);
    if(result.isSuccess()){
        return true;
    }else{

        if(result.isTookenExpire()){
            HttpUtil util ;
            if(! util.login(context)){
                LogUtil::info("downloadSwiftPackage can not refresh token , login failed ");
                return false;
            }
            return downloadFile( context , url , methodName , postFiled , progress , param , filePath );
        }else{
            LogUtil::warning(QString(QString("downloadSwiftPatchPackage : %1").arg(result.getErrorMessage())));
            return false;
        }

    }

    return true;

}


bool downloadSwiftPackage(ManagerContext *context ,Package &package,QString filePath , loadProgress progress , void * param){
    QString url  = package.getPackageUrl();

    return downloadFile(context,url,"GET","",progress,param,filePath);


}

bool downloadDBPilePackage(ManagerContext *context,Package &package,QString filePath , loadProgress progress ,void * param){
    QString url = context->getHostName()+"/v1/download-package/"+package.getPackageId()+"/"+context->getUserInfo().getToken();

    return downloadFile(context,url,"GET","",progress,param,filePath);


}

bool HttpUtil::downloadPackage(ManagerContext *context , QString taskId, QString packageId , loadProgress progress
                               , void * param){

    Q_ASSERT(context->containsTask(taskId));
    OnlineTask & task  = context->getTaskByTaskName(taskId);
    Q_ASSERT(task.containsPackage(packageId));
    Package &package = task.getPackageById(packageId);
    Q_ASSERT(task.isValid() && package.isValid());
    QString packagePath =  "jobs/" + taskId+"/"+packageId;

    QDir root(context->getRootPath()) ;
    if( ! root.mkpath(packagePath) ){
        LogUtil::critical(QString("downLoadToolPackage can not create tool dir : %1").arg(context->getRootPath() + "/"+packagePath));
        return false;
    }


    QString fileDownload = context->getRootPath() + "/" +  packagePath + "/" + "data.zip";

    if(task.getSource() == "Swift"){
        return downloadSwiftPackage(context,package,fileDownload,progress,param);
    }else{
        return downloadDBPilePackage(context, package, fileDownload , progress,param);
    }

    return false ;
}

bool HttpUtil::downloadPatchPackage(ManagerContext *context, QString taskId, QString packageId){
    Q_ASSERT(context->containsTask(taskId));
    OnlineTask & task  = context->getTaskByTaskName(taskId);
    Q_ASSERT(task.containsPackage(packageId));
    Package &package = task.getPackageById(packageId);
    Q_ASSERT(task.isValid() && package.isValid());
    QString packagePath =  "jobs/" + taskId+"/"+packageId;

    QDir root(context->getRootPath()) ;
    if( ! root.mkpath(packagePath) ){
        LogUtil::critical(QString("downLoadToolPackage can not create tool dir : %1").arg(context->getRootPath() + "/"+packagePath));
        return false;
    }
    QStringList patchURLs = package.getPackagePatchesUrl();

    if(patchURLs.isEmpty())
        return false;

    QString url =context->getHostName() +  patchURLs.last();
    url += QString("?token=%1").arg(context->getUserInfo().getToken());

    QString fileDownload = context->getRootPath() + "/" +  packagePath + "/" + "result.zip";



    return downloadFile(context,url,"GET","",nullptr,nullptr,fileDownload );
}


bool HttpUtil::downLoadToolPackage(ManagerContext *context, QString taskId , loadProgress progress , void * param ){

    Q_ASSERT(context->containsTask(taskId)) ;
    if(!context->containsTask(taskId)){
        return false;
    }
    OnlineTask & task = context->getTaskByTaskName(taskId);
    QString toolId  = task.getToolId();
    if(!context->containsTool(toolId))
        return false;

    ToolInfomation infomation = context->getToolLastVersionInformation(toolId);

    QString url = context->getHostName() + "/tool/download-object" +
            "?md5=" + infomation.getMd5()+
            "&token="+context->getUserInfo().getToken();


    QString toolPath = context->getToolPath(taskId);
    QDir dir(context->getRootPath());
    if( ! dir.mkpath(toolPath) ){
        LogUtil::critical(QString("downLoadToolPackage can not create tool dir : %1").arg(context->getRootPath() + "/"+toolPath));
        return false;
    }



    return downloadFile(context,url,"GET","",progress,param,context->getRootPath()+"/" + toolPath + "/" + "tool.zip" );

}

bool HttpUtil::downLoadToolPackageByToolId(ManagerContext *context, QString toolId, loadProgress progress, void *param){

    if(!context->containsTool(toolId))
        return false;

    ToolInfomation infomation = context->getToolLastVersionInformation(toolId);

    QString url = context->getHostName() + "/tool/download-object" +
            "?md5=" + infomation.getMd5()+
            "&token="+context->getUserInfo().getToken();


    QString toolPath = context->getRootPath() + "/tools/" + QString("%1").arg(toolId);
    QDir dir(context->getRootPath());
    if( ! dir.mkpath(toolPath) ){
        LogUtil::critical(QString("downLoadToolPackage can not create tool dir : %1").arg(context->getRootPath() + "/"+toolPath));
        return false;
    }



    return downloadFile(context,url,"GET","",progress,param,toolPath +  "/" + "tool.zip" );

}



bool HttpUtil::downloadConfigureFile(ManagerContext *context, QString taskId){
    Q_ASSERT(context->containsTask(taskId)) ;
    if(!context->containsTask(taskId)){
        return false;
    }

    QString url = context->getHostName() +  "/v3/static-file/" +taskId +
            + "/config/" + context->getUserInfo().getToken();

    return downloadFile(context,url,"GET","",nullptr,nullptr,
                        context->getRootPath()+"/jobs/" + QString("%1").arg(taskId)+"/"+"config.zip" );

}

bool HttpUtil::getTooInfomation(ManagerContext *context, QString toolId){
    QString url = context->getHostName() + "/tool/all-version";
    QString postFileds = QString("tool_id=%1&token=%2").arg(toolId).arg(context->getUserInfo().getToken());

    CURLUtil util(url,postFileds,"GET");
    CRULResult result = util.sendRequest();
    if(result.isSuccess()){

        if(result.isTookenExpire()){
            HttpUtil util ;
            if(! util.login(context)){
                LogUtil::info("downloadSwiftPackage can not refresh token , login failed ");
                return false;
            }
            getTooInfomation(context,toolId);
        }else{
            QJsonDocument document = QJsonDocument::fromJson(QByteArray(result.getData()->getBuffer(),result.getData()->getSize()));
            QVector<ToolInfomation> infomations;
            QJsonArray array = document.array();
            for(int index = 0 ; index < array.size(); index ++ ){
                QJsonObject object = array.at(index).toObject();
                infomations.append(ToolInfomation(object));
            }

            if(infomations.isEmpty())
                return false;
            context->addToolInfomations(toolId,infomations);
            return true;
        }
    }else{

        LogUtil::warning(QString(QString("downloadSwiftPatchPackage : %1").arg(result.getErrorMessage())));
        return false;
    }

    return false;
}

bool HttpUtil::uploadLabelingResult(ManagerContext *context, QString taskId, QString packageId ,bool success){

    Q_ASSERT(context->containsTask(taskId));
    OnlineTask & task  = context->getTaskByTaskName(taskId);
    Q_ASSERT(task.containsPackage(packageId));
    Package &package = task.getPackageById(packageId);
    Q_ASSERT(task.isValid() && package.isValid());

    bool checkTask = false ;
    if(package.getActionId().toInt() %2 == 1 ){
        checkTask = true;
    }


    QString url;
    if(checkTask){
        url = context->getHostName() + "/v3/annotation-check";
    }else{
         url = context->getHostName() + "/v3/annotation-result";
    }


    QMap<QString , QString > files , fields ;
    QString resultFilePath = context->getRootPath()+ "/" + context->getDataPath( taskId, packageId )+ "/" + "result.zip";
    QFileInfo info(resultFilePath);
    if(!info.exists()){
        return false;
    }

    resultFilePath = info.absoluteFilePath();
    files.insert("content",resultFilePath);

    fields.insert("userId",context->getUserInfo().getUserId());
    fields.insert("taskId",taskId);
    fields.insert("packageId",packageId);
    fields.insert("actionId",QString("%1").arg(package.getActionId()));
    fields.insert("token",context->getUserInfo().getToken());

    if(checkTask){
        fields.insert("result",success ? "ok" : "fail");
    }


    CURLUtil util(url,"");
    CRULResult result = util.uploadForm(url,files,fields);

    if(result.isSuccess()){
        return true;
    }else{
        if(result.isTookenExpire()){
            HttpUtil util ;
            if(! util.login(context)){
                LogUtil::info("downloadSwiftPackage can not refresh token , login failed ");
                return false;
            }
            return uploadLabelingResult(context,taskId, packageId);
        }else{
            LogUtil::warning(QString(QString("downloadSwiftPatchPackage : %1").arg(result.getErrorMessage())));
            return false;
        }

    }

    return false;

}


template<class T>  bool uploadLogEvent(Context<T> *context, OnlineTask & task, Package &package ,LogEvent &event){

    QString url = event.getURL();

    QString postFields = event.getPostFields(context,task,package);

    struct curl_slist *headers = NULL;
    if(event.getType() != LogEvent::Type_StartLabeling){
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
    }

    CURLUtil util(url,postFields,"",headers);
    CRULResult result = util.sendRequest();
    if(result.isSuccess()){
       return true;
    }else{
        if(result.isTookenExpire()){
            if(! labelingbase::login(context)){
                LogUtil::info("uploadLogEvent can not refresh token , login failed ");
                return false;
            }
            return uploadLogEvent( context , task ,package , event );
        }else{
            LogUtil::warning(QString(QString("downloadSwiftPatchPackage : %1").arg(result.getErrorMessage())));
            return false;
        }


    }
    return true;
}

bool HttpUtil::uploadLogEvent(ManagerContext * context, QString taskId, QString packageId ,LogEvent &event){
    Q_ASSERT(context->containsTask(taskId));
    OnlineTask & task  = context->getTaskByTaskName(taskId);
    Q_ASSERT(task.containsPackage(packageId));
    Package &package = task.getPackageById(packageId);
    Q_ASSERT(task.isValid() && package.isValid());

    return labelingbase::uploadLogEvent(context,task,package,event);

}
bool HttpUtil::uploadLogEvent(ToolContext * context, LogEvent &event){

    OnlineTask  task = context->getTask() ;
    Package package = task.getPackages().at(0);


    return labelingbase::uploadLogEvent(context,task,package,event);
}





}

