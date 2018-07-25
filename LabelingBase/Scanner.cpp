#include "Scanner.h"
#include<QDir>
#include<QFileInfo>
#include"context/ToolContext.h"

namespace labelingbase{

QStringList imageSuffixs = {"BMP","GIF","JPG","JPEG","PNG","PBM","PGM","XBM","XPM"};

QStringList getImageSuffix(){
    return imageSuffixs;
}

bool isImageFile(QString fileName){
    QFileInfo info(fileName);
    QString suffix = info.suffix();
    for(QString imageSuffix : imageSuffixs){
        if(suffix.compare(imageSuffix,Qt::CaseInsensitive) == 0){
            return true;
        }
    }
    return false;
}


QStringList getImages(QString path){
    QStringList result;
    QDir root(path);
    if(!root.exists()){
        return result;
    }
    QFileInfoList infos = root.entryInfoList();

    for(QFileInfo info : infos){
        if(info.isDir() && info.fileName()!="." && info.fileName()!=".."){
            result += getImages(info.absoluteFilePath());
        }else if(info.isFile() && isImageFile(info.absoluteFilePath())){
            QString dataPath = ToolContext::getInstance()->getRootPath() + "/" + ToolContext::getInstance()->getDataPath();
            QDir dir(dataPath);
            QString relativePath = dir.relativeFilePath(info.absoluteFilePath());
            result.append(relativePath);
        }
    }

    return result;
}

QStringList ImageScanner::getImages(){
    return labelingbase::getImages(path);
}


QStringList getImageDirs(QString path){
    QStringList result;
    QDir root(path);
    if(!root.exists()){
        return result;
    }

    QFileInfoList infos = root.entryInfoList();

    bool found  = false;
    for(QFileInfo info : infos){
        if(info.isDir() && info.fileName()!="." && info.fileName()!=".."){
            result += getImageDirs(info.absoluteFilePath());
        }else if(info.isFile() && isImageFile(info.absoluteFilePath())){
            found = true;
        }
    }

    if(found)
        result.append(path);

    return result;
}


QStringList ImageDirScanner::getDirs(){
    return labelingbase::getImageDirs(path);
}



}


