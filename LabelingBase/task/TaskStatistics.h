#ifndef TASKSTATISTICS_H
#define TASKSTATISTICS_H

#include<QVector>
#include<QPair>
#include<QSettings>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qtextcodec.h>
#include"labelingbase_global.h"

namespace labelingbase {
#define NECESSARY_STATIS "necessarys"
#define STATISTICS_FILE_NAME "summary.json"
#define DATA_INDEX "labelingSetIndex"


class LABELINGBASESHARED_EXPORT TaskStatistics
{
public:
    explicit TaskStatistics(QString Path);

    ~TaskStatistics();

    void save() ;

    QMap<QString,QString> getNecessarys() const;

    void addNecessary(QString imageName , QString value){
        necessarys.insert(imageName,value);
    }

    void removeNecessary(QString imageName){
        necessarys.remove(imageName);
    }

    bool containsNecessary(QString imageName) const{
        return necessarys.contains(imageName);
    }

    QMap<QString,QMap<QString,QString>> getDetails() const;

    QMap<QString,QString> getDetail(QString imageName);

    QString getDetail(QString imageName,QString fieldName){
        return details.value(imageName).toObject().value(fieldName).toString();
    }

    void insertDetails(QString imageName, QString fieldName , QString value);

    void removeDetails(QString imageName){
        details.remove(imageName);
    }

    void removeDetail(QString imageName , QString fieldName){
        details.value(imageName).toObject().remove(fieldName);
    }

    bool containsDetail(QString imageName) const{
        return details.contains(imageName);
    }

    QMap<QString,QString> getAllDetail(QString detailName);       //返回名称为detailName的全部图片的该统计信息

private:
    void setNecessarys(const QJsonObject &value) {necessarys = value;}
    void setDetails(const QJsonObject &value) {details = value;}

    QString fromUTF8toLocal(QString value);
    QString fromLocaltoUTF8(QByteArray value);



private:
    QJsonObject necessarys;
    QJsonObject details;
    QString configPath;
};
}


#endif // TASKSTATISTICS_H
