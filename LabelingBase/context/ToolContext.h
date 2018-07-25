#ifndef TOOLCONTEXT_H
#define TOOLCONTEXT_H

#include"task/OnlineTask.h"
#include"context/Context.h"
#include"LogUtil.h"
#include"http/HttpUtil.h"
#include"labelingbase_global.h"
#include"task/JobConfiguration.h"
#include"LabelingFieldNames.h"
namespace labelingbase{
class LABELINGBASESHARED_EXPORT ToolContext :public Context<ToolContext>
{
public:
    ToolContext();


    OnlineTask & getTask(){
        return currentTask;
    }

    void setTask(OnlineTask task){
        this->currentTask = task;
    }
    QString getTarget() const{
        return target;
    }
    void setTarget(QString target){
        this->target = target;
    }
    QString getResultPath() const{
        return "";
    }
    bool containsOption(QString name) const{
        for(JobStepOption option : options){
            if(name.trimmed() == option.getFiled().trimmed())
                return true;
        }
        return false;
    }
    QString getOption(QString name) const{
        for(JobStepOption option : options){
            if(name.trimmed() == option.getFiled().trimmed())
                return option.getValue();
        }
        return "";
    }

    QStringList getHotKeys()const{
        return key_valies.keys();
    }
    bool containsHotKey(QString keyName) const{
        return key_valies.contains(keyName);
    }
    QString getHotkeyValue(QString keyName)const{
        return key_valies.value(keyName);
    }
    QString getField()const{
        return this->field;
    }

    QStringList getValues() const{
       return  value_labels.keys();
    }
    bool containsValue(QString value)const{
        return value_labels.contains(value);
    }
    QString getValueLabel(QString value){
        return value_labels.value(value);
    }

    QString getDataPath() {
        return  "jobs/" + currentTask.getTaskId() + "/" + currentTask.getPackages().at(0).getPackageId();
    }
    QString getConfigurePath(){
        return "jobs/" + currentTask.getTaskId() + "/"  + "config"  ;
    }


    bool initialize(char ** argv , int argc){

        QString targetName = QString("-%1").arg( STEP_TARGET_NAME );
        QString fieldName = QString("-%1").arg(STEP_FEILD_NAME);
        QString key_value_maps = QString("%1:").arg(KEY_VALUE_NAME);
        QString value_label_maps = QString("%1:").arg(VALUE_LABEL_NAME);
        QString taskId;
        QString packageId;
        for(int index = 0 ; index < argc ; index ++ ){
            QString command =  QString::fromLocal8Bit(argv[index]).trimmed();
            QStringList args = command.split("=");
            if(args.size()!=2)
                continue;
            if( command.toLower().startsWith("-username")){
                getUserInfo().setUserName(args[1].trimmed());
            }else if(command.toLower().startsWith("-password")){
                getUserInfo().setPassword(args[1].trimmed());
            }else if(command.toLower().startsWith("-rootpath")){
                setRootPath(args[1].trimmed());
            }else if(command.toLower().startsWith("-taskid")){
                taskId = args[1].trimmed();
            }else if(command.toLower().startsWith("-packageid")){
                packageId = args[1].trimmed();
            }else if(command.toLower().startsWith( targetName )){
                target = args[1].trimmed();
            }else if(command.toLower().startsWith( fieldName )){
                this->field = args[1].trimmed();
            }else if(command.toLower().startsWith(key_value_maps)){
                QString key = args.at(0).mid(key_value_maps.length());
                QString value = args.at(1);
                key_valies.insert(key,value);
            }else if(command.toLower().startsWith(value_label_maps)){
                QString key = args.at(0).mid(value_label_maps.length());
                QString value = args.at(1);
                value_labels.insert(key,value);
            }else{
                QString name = args[0].trimmed();
                QString value = args[1].trimmed();
                options.append(JobStepOption(name,value));
            }
        }
        if(taskId.isEmpty() || packageId.isEmpty()){
            LogUtil::debug("you have not assign a taskId or package Id ");
            Q_ASSERT(false);
            return false;
        }

        {
            HttpUtil util;
            if(!util.login(this)){
                return false;
            }
            if(!util.initTaskInfomation(this,taskId,packageId)){
                return false;
            }

        }


        return true;

    }

private:
    OnlineTask currentTask;
    QString target;
    QString field;
    QVector<JobStepOption> options;
    QMap<QString,QString> key_valies;
    QMap<QString,QString> value_labels;

};

}


#endif // TOOLCONTEXT_H
