#include "TaskStatistics.h"

#include <QFile>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QDebug>
#include "LogUtil.h"
#include "task/TaskManager.h"

namespace labelingbase{

TaskStatistics::TaskStatistics(QString Path)
{
    this->configPath = Path;

    QFile file(configPath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString val;
        val = file.readAll();
        file.close();

        QJsonParseError jsonParseError;
        QJsonDocument document = QJsonDocument::fromJson(val.toUtf8(), &jsonParseError);
        if(jsonParseError.error == QJsonParseError::NoError) {
            if(document.isObject()) {
                QJsonObject jsonObj = document.object();
                if (jsonObj.contains("necessary")) {
                    QJsonValue value = jsonObj.value("necessary");
                    if(value.isObject()){
                        QJsonObject necObj = value.toObject();
                        setNecessarys(necObj);
                    }
                }
                if(jsonObj.contains("detail")){
                    QJsonValue value = jsonObj.value("detail");
                    if(value.isObject()){
                        setDetails(value.toObject());
                    }
                }
            }
        } else {
            LogUtil::critical(QString("parseJsonStr: jsonParseError( %1 ) + %2").arg(jsonParseError.error).arg(jsonParseError.errorString()));
        }
    }

}

TaskStatistics::~TaskStatistics(){

}

void TaskStatistics::save(){
    //将Necessarys从utf8转码为Local encoding（没有此步的话写入到json中时中文会变为“????”）
    QJsonObject byteNecessarys;
    auto necessaryIter = necessarys.begin();
    while(necessaryIter!=necessarys.end()){
        QString byte=fromUTF8toLocal(necessaryIter.key());
        byteNecessarys.insert(byte,necessaryIter.value());
        necessaryIter++;
    }

    QJsonObject json;

    json.insert("necessary",QJsonValue(byteNecessarys));
    json.insert("detail",QJsonValue(details));

    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString json_str(byteArray);

    QFile staticsFile(configPath);
    staticsFile.open(QIODevice::WriteOnly);
    staticsFile.write(json_str.toLatin1(),json_str.length());
    staticsFile.close();
}

QMap<QString, QString> TaskStatistics::getNecessarys() const{
    QMap<QString,QString> result;
    auto necIter = necessarys.begin();
    while(necIter != necessarys.end()){
        result.insert(necIter.key(),necIter.value().toString());
        necIter++;
    }
    return result;
}

QMap<QString, QMap<QString, QString> > TaskStatistics::getDetails() const{
    QMap<QString,QMap<QString,QString>> result;
    auto detailsIter = details.begin();
    while(detailsIter != details.end()){
        QMap<QString,QString> detailResult;
        QJsonObject detail = detailsIter.value().toObject();
        auto Iter = detail.begin();
        while(Iter != detail.end()){
            detailResult.insert(Iter.key(),Iter.value().toString());
            Iter++;
        }
        result.insert(detailsIter.key(),detailResult);
        detailsIter++;
    }
    return result;
}

QMap<QString, QString> TaskStatistics::getDetail(QString imageName){
    QMap<QString,QString> result;
    QJsonObject detail = details.value(imageName).toObject();
    auto Iter = detail.begin();
    while(Iter != detail.end()){
        result.insert(Iter.key(),Iter.value().toString());
        Iter++;
    }
    return result;
}

void TaskStatistics::insertDetails(QString imageName, QString fieldName, QString value)
{
    if(details.contains(imageName)){
        QJsonObject newDetails = details.value(imageName).toObject();
        newDetails.insert(fieldName,value);
        newDetails.insert(DATA_INDEX,QString::number(TaskManager::getInstance()->getCurrentIndex()));
        details.insert(imageName,newDetails);
    }else{
        QJsonObject newDetails;
        newDetails.insert(fieldName,value);
        newDetails.insert(DATA_INDEX,QString::number(TaskManager::getInstance()->getCurrentIndex()));
        details.insert(imageName,newDetails);
    }
}

QString TaskStatistics::fromUTF8toLocal(QString value)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray byte = codec->fromUnicode(value);
    QString byteKey;
    for(int i=0;i<byte.size();i++){
        byteKey=byteKey+byte.at(i);
    }
    return byteKey;
}

QString TaskStatistics::fromLocaltoUTF8(QByteArray value)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString string = codec->toUnicode(value);
    return string;
}

QMap<QString,QString> TaskStatistics::getAllDetail(QString detailName)
{
    QMap<QString,QString> result;
    auto detailIter = details.begin();
    while(detailIter != details.end()){
        if(detailIter.value().isObject()){
            QJsonObject imageDetail = detailIter.value().toObject();
            if(imageDetail.contains(detailName)){
                result.insert(detailIter.key(),imageDetail.value(detailName).toString());
            }
        }
        detailIter ++;
    }
    return result;
}

}

