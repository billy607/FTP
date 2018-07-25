#ifndef MANAGERCONTEXT_H
#define MANAGERCONTEXT_H
#include"task/OnlineTask.h"
#include"labelingbase_global.h"
#include"task/ToolInfomation.h"
#include"context/Context.h"
#include<QThread>
namespace labelingbase{

class LABELINGBASESHARED_EXPORT ManagerContext : public Context<ManagerContext>
{
public:
    bool hasGetTasks(){
        return !tasks.isEmpty();
    }
    QVector<OnlineTask>  getTasks()const{
        return tasks;
    }
    void setTasks(QVector<OnlineTask> tasks){
        this->tasks = tasks;
    }

    bool refreshTaskList();

    bool containsTask(QString taskId){
        for(OnlineTask  &task : tasks){
            if(task.getTaskId() == taskId){
                return true;
            }
        }
        return false;
    }

    OnlineTask & getTaskByTaskName(QString taskId){
        for(OnlineTask  &task : tasks){
            if(task.getTaskId() == taskId){
                return task;
            }
        }
        Q_ASSERT(false);
    }


    void addToolInfomations(QString toolId, QVector<ToolInfomation> infomations){
        toolInformations.addToolInfomation(toolId,infomations);
    }
    QString getToolName(QString name) const{
        return toolInformations.getToolName(name);
    }
    QString getToolChineseName(QString name) const{
        return toolInformations.getToolChineseName(name);
    }


    bool containsTool(QString toolId) ;

    ToolInfomation getToolLastVersionInformation(QString toolId){
        return toolInformations.getLastVersionInfomationById(toolId);
    }

    bool initialize();

    QString getToolPath(QString taskId);

    QString getDataPath(QString taskId, QString packageId) ;

    QString getConfigPath(QString taskId);

    void clearPackage(QString taskId, QString packageId) ;



public:
    ManagerContext();

private:
    QVector<OnlineTask> tasks;
    ToolInfomationManager toolInformations;
};


}

#endif // MANAGERCONTEXT_H
