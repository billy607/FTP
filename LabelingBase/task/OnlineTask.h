#ifndef TASKINFOMATION_H
#define TASKINFOMATION_H

#include<QString>
#include<QVector>
#include "labelingbase_global.h"


namespace labelingbase {

class LABELINGBASESHARED_EXPORT Package{

public :
    Package(){

    }
    bool isValid() const{
        return !packageId.isEmpty();
    }

    Package(QString packageId){
        this->packageId = packageId;
    }

    QString getPackageName() {
        return packageName;
    }
    void setPackageName(QString packageName) {
        this->packageName = packageName;
    }
    QString getPackageId() const {
        return packageId;
    }
    void setPackageId(QString packageId) {
        this->packageId = packageId;
    }
    QString getPackageUrl() {
        return packageUrl;
    }
    void setPackageUrl(QString packageUrl) {
        this->packageUrl = packageUrl;
    }
    QStringList getPackagePatchesUrl()const {
        return packagePatchesUrl;
    }
    bool containsPachPackage()const{
        return !packagePatchesUrl.isEmpty();
    }
    void setPackagePatchesUrl(QStringList packagePatchesUrl) {
        this->packagePatchesUrl = packagePatchesUrl;
    }
    QString getActionId() {
        return actionId;
    }
    void setActionId(QString actionId) {
        this->actionId = actionId;
    }
    QString getType() {
        return type;
    }
    void setType(QString type) {
        this->type = type;
    }
private:
    QString packageName;
    QString packageId;
    QString packageUrl;
    QStringList packagePatchesUrl;
    QString actionId;
    QString type;
};

class LABELINGBASESHARED_EXPORT OnlineTask{
public:

    OnlineTask(){

    }
    OnlineTask(QString taskId, QString packageId){
        this->taskId = taskId;
        packages.append(Package(packageId));
    }

    bool isValid() const{
        return !taskId.isEmpty() && !taskName.isEmpty();
    }
    QString getTaskId() const {
            return taskId;
        }
        void setTaskId(QString taskId) {
            this->taskId = taskId;
        }
        QString getTaskName() {
            return taskName;
        }
        void setTaskName(QString taskName) {
            this->taskName = taskName;
        }
        QString getSource() {
            return source;
        }
        void setSource(QString source) {
            this->source = source;
        }
        QString getToolId()const {
            return toolId;
        }
        void setToolId(QString toolId) {
            this->toolId = toolId;
        }
        QString getToolVersion() {
            return toolVersion;
        }
        void setToolVersion(QString toolVersion) {
            this->toolVersion = toolVersion;
        }
        QString getToolName() {
            return toolName;
        }
        void setToolName(QString toolName) {
            this->toolName = toolName;
        }
        QString getToolObject() {
            return toolObject;
        }
        void setToolObject(QString toolObject) {
            this->toolObject = toolObject;
        }

        void addPackage(Package package){
            this->packages.append(package);
        }
        void removePackageById( QString packageId ){
            for(int index = 0 ; index < packages.size() ; index ++ ){
                if(packages[index].getPackageId() == packageId){
                    packages.removeAt(index);
                    break;
                }
            }
        }

        QVector<Package> getPackages() const{
            return packages;
        }
        void setPackage( QVector<Package> packages  ){
            this->packages = packages;
        }
        bool containsPackage(QString packageId){
            for(Package & package : packages ){
                if(package.getPackageId() == packageId)
                    return true;
            }
            return false;
        }

        Package & getPackageById(QString packageId){
            for(Package & package : packages ){
                if(package.getPackageId() == packageId)
                    return package;
            }
            Q_ASSERT(false);
        }

private:
    QString taskId;
    QString taskName;
    QString source;
    QString toolId;
    QString toolVersion;
    QString toolName;
    QString toolObject;
    QVector<Package> packages;

};


}



#endif // TASKINFOMATION_H
