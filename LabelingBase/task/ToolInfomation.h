#ifndef TOOLINFOMATION_H
#define TOOLINFOMATION_H
#include<QString>
#include<QDateTime>
#include<QMap>
#include<QVector>
#include<QJsonObject>
#include<QDebug>
class ToolInfomation
{
public:
    ToolInfomation(){

    }

    ToolInfomation(QJsonObject object){

        Q_ASSERT(object.contains("md5"));
        this->id =QString("%1").arg(object.value("id").toInt());
        this->desc = object.value("version_desc").toString();
        this->releasetime = QDateTime::fromString(object.value("time").toString(),"yyyy.MM.dd hh:mm:ss");
        this->md5 = object.value("md5").toString();
        this->majorVersion = QString("%1").arg(object.value("major_version").toInt());
        this->minorVersion = QString("%1").arg(object.value("minor_version").toInt());
        this->microVersion = QString("%1").arg(object.value("micro_version").toInt());
    }


    QString getId() {
        return id;
    }
    void setId(QString id) {
        this->id = id;
    }
    QString getDesc() {
        return desc;
    }
    void setDesc(QString desc) {
        this->desc = desc;
    }
    QDateTime getReleasetime() {
        return releasetime;
    }
    void setReleasetime(QDateTime releasetime) {
        this->releasetime = releasetime;
    }
    QString getMd5() {
        return md5;
    }
    void setMd5(QString md5) {
        this->md5 = md5;
    }
    QString getMajorVersion() {
        return majorVersion;
    }
    void setMajorVersion(QString majorVersion) {
        this->majorVersion = majorVersion;
    }
    QString getMinorVersion() {
        return minorVersion;
    }
    void setMinorVersion(QString minorVersion) {
        this->minorVersion = minorVersion;
    }
    QString getMicroVersion() {
        return microVersion;
    }
    void setMicroVersion(QString microVersion) {
        this->microVersion = microVersion;
    }
private:
    QString id;//此id看起来一点用都没有，已经有了版本标识，xi下一个版本应该废弃
    QString desc;
    QDateTime releasetime;
    QString md5;
    QString majorVersion;
    QString minorVersion;
    QString microVersion;

};


class ToolInfomationManager{
public:
    ToolInfomationManager();
    void addToolInfomation(QString toolId ,  QVector<ToolInfomation> versions){
        Q_ASSERT(!versions.isEmpty());
        infomations.insert(toolId,versions);
    }
    bool containsTool(QString toolId)const {
        return infomations.contains(toolId);
    }
    ToolInfomation getLastVersionInfomationById(QString toolId)const {
        Q_ASSERT(infomations.contains(toolId));
        return infomations.value(toolId).last();
    }
    QString getToolName(QString name) const{
        return toolNameMaps.value(name);
    }
    QString getToolChineseName(QString name) const{
        return toolChineseMaps.value(name);
    }

    bool initialize(QString filePath);
private:
    QMap <QString , QVector< ToolInfomation > > infomations;
    QMap<QString,QString> toolNameMaps;
    QMap<QString,QString> toolChineseMaps;
};

#endif // TOOLINFOMATION_H
