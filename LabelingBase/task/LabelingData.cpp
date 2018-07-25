#include "LabelingData.h"
#include"JobConfiguration.h"
#include"TaskManager.h"
#include"Dispatch.h"
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QDir>
#include"LogUtil.h"
#include"crypto/Crypto.h"
#include"context/ToolContext.h"
#include"Scanner.h"
namespace labelingbase {
QMap<QString , QString>  LabelingImage::imageMD5Maps;


LabelingImage::LabelingImage(QString path){
    this->path = path;
}


QPixmap LabelingImage::getImage(){

    QString fileName = getPath();
    QFile imageFile(ToolContext::getInstance()->getRootPath() +
                    "/" + ToolContext::getInstance()->getDataPath() + "/" + path);
    if(!imageFile.open(QIODevice::ReadOnly)){
        Q_ASSERT(false);
        return QPixmap();
    }
    QByteArray fileData =  imageFile.readAll();
    Crypto::CrypedData uncrypedData = Crypto::getInstance()->decryptData(fileData) ;
    QImage image = QImage::fromData((unsigned char *)uncrypedData.getData(),uncrypedData.getLength());


    if(imageMD5Maps.contains(fileName)){
        QByteArray md5Data= QCryptographicHash::hash(
                    QByteArray::fromRawData(uncrypedData.getData(),uncrypedData.getLength())
                    ,QCryptographicHash::Md5);

        imageMD5Maps.insert(fileName, md5Data.toHex());
    }

    if(image.isNull()){
        Q_ASSERT(!image.isNull());
        return QPixmap();
    }

    return QPixmap::fromImage(image);
}

QString LabelingImage::getImageMD5(QString fileName){
    if(!imageMD5Maps.contains(fileName)){

        QFile imageFile(ToolContext::getInstance()->getRootPath() +
                        "/" + ToolContext::getInstance()->getDataPath() + "/" + fileName);

        if(!imageFile.open(QIODevice::ReadOnly)){
            return "";
        }
        QByteArray fileData =  imageFile.readAll();
        Crypto::CrypedData uncrypedData = Crypto::getInstance()->decryptData(fileData) ;
        QByteArray md5Data= QCryptographicHash::hash(
                    QByteArray::fromRawData(uncrypedData.getData(),uncrypedData.getLength())
                    ,QCryptographicHash::Md5);

        imageMD5Maps.insert(fileName, md5Data.toHex());
        imageFile.close();
    }

    return imageMD5Maps.value(fileName) ;
}



QVector<pLabelingData> LabelingImage::getShapes(QString shapeName, QString field){
    QVector<pLabelingData> items;

    if(shapeName == ATTRIBUTE_POLYGON_NAME){
        return getPolygons(field) ;
    }else if(shapeName == ATTRIBUTE_RECT_NAME){
        return getRects(field) ;
    }else if(shapeName == ATTRIBUTE_LINE_NAME){
        return getLines(field) ;
    }else if(shapeName == ATTRIBUTE_MULTIPOINT_NAME){
        return getPoints(field) ;
    }else{
        LogUtil::critical(QString("unsupported labeling shape must be [%1,%2,%3,%4]")
                          .arg(ATTRIBUTE_POLYGON_NAME)
                          .arg(ATTRIBUTE_RECT_NAME)
                          .arg(ATTRIBUTE_LINE_NAME)
                          .arg(ATTRIBUTE_MULTIPOINT_NAME));
        return items;
    }

    return items;
}

bool dirNameLessThan(const QFileInfo &s1, const QFileInfo &s2)
{
    return s1.fileName() < s2.fileName();
}

LabelingImageDir::LabelingImageDir(QString path){

    this->path = path;
    setPath(path);
}

void LabelingImageDir::setPath(QString path){
//    LabelingDataArray * array = new LabelingDataArray();
//    images =  pLabelingData(array);

    QDir dir(path);
    QStringList imageSuffixs = getImageSuffix();
    QStringList filters ;
    for(QString suffix : imageSuffixs){
        filters.append(QString("*.%1").arg(suffix));
    }
    QFileInfoList files = dir.entryInfoList(filters,QDir::Files);
    qSort(files.begin(),files.end(),dirNameLessThan);


    for(QFileInfo file : files){
        QDir dataDir(ToolContext::getInstance()->getRootPath()+"/"+ToolContext::getInstance()->getDataPath()+"/");
        QString relativePath = dataDir.relativeFilePath(file.absoluteFilePath());
        images.append(pLabelingImage(new LabelingImage( relativePath) ));
    }
}




}

