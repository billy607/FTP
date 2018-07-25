#include "ManagerContext.h"
#include"http/HttpUtil.h"
#include"LogUtil.h"
#include"zip/ZipUtil.h"
#include<QFile>
#include<QDir>
namespace labelingbase {

template <> ManagerContext * Context<ManagerContext>::instance = nullptr;

ManagerContext::ManagerContext()
{

}

bool ManagerContext::initialize(){


    bool success = false;
    HttpUtil util;
    if(util.login(this) && util.initOnlinePackets(this) ){
        success =  true;
    }

    if(!toolInformations.initialize("Version/tool_id_name_mapping.json")){
        return false;
    }

    if(success){
        for(OnlineTask task : tasks){

            QDir taskDir(getRootPath() + "/jobs/" + QString("%1").arg(task.getTaskId())  ) ;

            if(!taskDir.exists()){
                QDir(getRootPath()).mkpath("jobs/" + QString("%1").arg(task.getTaskId()));
            }
            if(taskDir.exists("config"))
                continue;

            taskDir.mkdir("config") ;

            if(util.downloadConfigureFile(this,task.getTaskId())){
                QString configureZip = getRootPath() + "/jobs/" + QString("%1").arg(task.getTaskId()) +
                        "/config.zip";

                QFile configFile(configureZip) ;
                if(configFile.exists()){
                    ZipUtil zipUtil;
                    zipUtil.unzip(configureZip,getRootPath() + "/jobs/" + QString("%1").arg(task.getTaskId())+"/config");
                    configFile.remove();
                }
            }
        }
    }

    return success;
}

bool ManagerContext::refreshTaskList(){
    tasks.clear();
    HttpUtil util ;
    return util.initOnlinePackets(this);
}

QString ManagerContext::getToolPath(QString taskId){
    Q_ASSERT(containsTask(taskId));
    if(!containsTask(taskId)){
        LogUtil::warning(QString("unknown taskId %1").arg(taskId));
        return "";
    }
    OnlineTask & task  = getTaskByTaskName(taskId);

    return  "tools/" + task.getToolId() + "/" + task.getToolVersion();
}
QString ManagerContext::getDataPath(QString taskId, QString packageId){
    return "jobs/" + taskId + "/" + packageId ;
}
QString ManagerContext::getConfigPath(QString taskId){
    return "jobs/" + taskId + "/" + "config/"  ;
}

bool ManagerContext::containsTool(QString toolId){
    if(!toolInformations.containsTool(toolId)){
        HttpUtil util;
        if(!util.getTooInfomation(this,toolId)){
            return false;
        }
    }
    return true;
}

bool deleteDir(QString path){
    if (path.isEmpty()){
           return false;
       }
       QDir dir(path);
       if(!dir.exists()){
           return true;
       }
       dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
       QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
       foreach (QFileInfo file, fileList){ //遍历文件信息
           if (file.isFile()){ // 是文件，删除
               file.dir().remove(file.fileName());
           }else{ // 递归删除
               deleteDir(file.absoluteFilePath());
           }
       }
       return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

void ManagerContext::clearPackage(QString taskId, QString packageId){
    QString packagePath = getRootPath() + "/jobs/" +
            QString("%1").arg(taskId)+ "/" + QString("%2").arg(packageId);
    deleteDir(packagePath);
}






}

