#include "TaskManager.h"
#include"Dispatch.h"
#include"LabelingSet.h"
#include"context/ToolContext.h"
#include"JobConfiguration.h"
#include<QMessageBox>
#include<QFile>
#include<QDir>
#include<QMap>
#include<QJsonDocument>
#include<QJsonObject>
#include"LogUtil.h"
#include<Log4Labeling.h>

namespace labelingbase {
TaskManager * TaskManager::instance = nullptr;


bool loadAttachmentToLabelingset(LabelingSet *  labelingset ){

    ImageLabelingset * imageset = dynamic_cast<ImageLabelingset *> (labelingset) ;
    ImageDirLabelingset * imageDirSet = dynamic_cast<ImageDirLabelingset *> (labelingset) ;
    if(imageset){
        LabelingImage * image = dynamic_cast< LabelingImage *>  (imageset->getImage().data());
        if(image){
            QString md5FileName = LabelingImage::getImageMD5(image->getPath());
            QString attachmentFilePath = ToolContext::getInstance()->getRootPath() + "/"
                    + ToolContext::getInstance()->getDataPath() + "/"
                    + "Label/" + md5FileName + ".json";
            TaskManager::getInstance()->getDispatcher()->onLoadAttachment(imageset->getImage(),attachmentFilePath);
        }
    }
    if(imageDirSet){
        LabelingImageDir * imageDir = dynamic_cast<LabelingImageDir *> (imageDirSet->getImageDir().data());
        if(imageDir != nullptr){
            QVector<pLabelingData> images = imageDir->getImages();
            for(int jndex = 0 ; jndex < images.size() ; jndex ++ ){
                pLabelingData data = images[jndex] ;
                LabelingImage * image = dynamic_cast< LabelingImage *>  (data.data());
                if(image){
                    QString md5FileName = LabelingImage::getImageMD5(image->getPath());
                    QString attachmentFilePath = ToolContext::getInstance()->getRootPath() + "/"
                            + ToolContext::getInstance()->getDataPath() + "/"
                            + "Label/" + md5FileName + ".json";
                    TaskManager::getInstance()->getDispatcher()->onLoadAttachment(data,attachmentFilePath);
                }
            }
        }
    }
    return true;

}

bool loadFromAttachment(LabelingSet * labelingset){

    QDir root(ToolContext::getInstance()->getRootPath()+"/"+ToolContext::getInstance()->getDataPath());
    QString attachmentPath = QString("Label/") + labelingset->getPath();
    QFile attachmentFile(root.absolutePath() + "/"  + attachmentPath +  "/" + labelingset->getName()+".json");


    if(attachmentFile.open(QIODevice::ReadOnly)){
        QByteArray array  = attachmentFile.readAll();
        labelingset->unserialize(QString(array));
        attachmentFile.close();
        return true;
    }
    return false;


}


bool loadFromSerializeData(LabelingSet * labelingset){
    QDir root(ToolContext::getInstance()->getRootPath()+"/"+ToolContext::getInstance()->getDataPath());
    QString resultPath = QString("Label/") + ToolContext::getInstance()->getResultPath() + "/" + labelingset->getPath();
    QFile resultFile ( root.absolutePath() + "/"  + resultPath+ "/" + labelingset->getName()+".json");

    bool tryUnserilize = false;
    if(resultFile.open(QIODevice::ReadOnly)){
        QByteArray array  = resultFile.readAll();
        tryUnserilize = labelingset->unserialize(QString(array));
        resultFile.close();
    }

    return tryUnserilize;
}

void unserilizeLabelingset(LabelingSet * labelingset){

    QDir root(ToolContext::getInstance()->getRootPath()+"/"+ToolContext::getInstance()->getDataPath());
    QString attachmentPath = QString("Label/")  + labelingset->getPath();
    QString resultPath =  QString("Label/") + ToolContext::getInstance()->getResultPath() + "/" + labelingset->getPath();

    QFile resultFile ( root.absolutePath() + "/"  + resultPath+ "/" + labelingset->getName()+".json");
    QFile attachmentFile(root.absolutePath() + "/"  + attachmentPath +  "/" + labelingset->getName()+".json");
    if(resultFile.open(QIODevice::ReadOnly)){
        QByteArray array  = resultFile.readAll();
        labelingset->unserialize(QString(array));
        resultFile.close();
    }else if(attachmentFile.open(QIODevice::ReadOnly)){
        QByteArray array  = attachmentFile.readAll();
        labelingset->unserialize(QString(array));
        attachmentFile.close();
    }

}




TaskManager * TaskManager::getInstance(){
    if(!instance){
        instance = new TaskManager();
    }
    return instance;
}

TaskManager::TaskManager()
{
    currentIndex = -1 ;
    dispatch = nullptr;
}



bool TaskManager::init(Dispatcher *dispatch){


    this->dispatch = dispatch;

    labelingsets = dispatch->OnInitialize();

    int index = 0 ;
    for(LabelingSet *labelingset : labelingsets){
        labelingset->setIndex(index ++ );
    }

    dispatch->OnStartup();

    return true;
}


TaskManager::~TaskManager()
{

}

bool TaskManager::hasNextLabelingset()const {

    return (!labelingsets.isEmpty()) &&( currentIndex + 1 < labelingsets.size() ) ;
}


void TaskManager::savaLableingsetResult( LabelingSet * labelingset ){
    if(labelingset){
        QDir root(ToolContext::getInstance()->getRootPath()+"/"+ToolContext::getInstance()->getDataPath());
        QString tmpPath =  QString("Label/") +  ToolContext::getInstance()->getResultPath() + "/" + labelingset->getPath();
        bool success = false;
        if(root.mkpath(tmpPath)){
            QFile file(root.absolutePath() + "/" + tmpPath + "/" + labelingset->getName() + ".json");
            if(file.open(QIODevice::WriteOnly)){
                file.write(labelingset->serialize().toUtf8());
                file.close();
                success = true;
            }
        }

        if(!success){
            LogUtil::critical(QString("can not write %1").arg(tmpPath));
        }
    }
}



void initializeLableingSet(LabelingSet * newLableingset){

    if(!newLableingset->hasInitialized()){
        if(!loadFromSerializeData(newLableingset)){//先从结果反序列化结果
             loadAttachmentToLabelingset(newLableingset);
        }
    }

    newLableingset->setInitialized(true);
}

void TaskManager::nextLabelingset(){
    LabelingSet * labelingset = getCurrentLabelingset() ;

    if(labelingset){
        savaLableingsetResult(labelingset);
    }

    if(labelingsets.isEmpty()  || currentIndex + 1 >= labelingsets.size()){
        return ;
    }

    ++currentIndex ;

    LabelingSet * newLableingset = labelingsets[currentIndex];

    initializeLableingSet(newLableingset);


    dispatch->OnNewLabelingSetStart(labelingsets[currentIndex]);
}

bool TaskManager::hasPreLabelingset()const {
    return (!labelingsets.isEmpty()) && (currentIndex-1 >=0) ;
}

void TaskManager::preLabelingset(){
    LabelingSet * labelingset = getCurrentLabelingset() ;

    if(labelingset){
        savaLableingsetResult(labelingset);
    }

    if(labelingsets.isEmpty()  || currentIndex - 1 < 0){
        return ;
    }

    --currentIndex ;

    LabelingSet * newLableingset = labelingsets[currentIndex];

    initializeLableingSet(newLableingset);

    dispatch->OnNewLabelingSetStart(labelingsets[currentIndex]);
}

void TaskManager::jumpLabelingset(int index){

    LabelingSet * labelingset = getCurrentLabelingset() ;

    if(labelingset){
        savaLableingsetResult(labelingset);
    }

    if(labelingsets.isEmpty()  || index < 0 || index >= labelingsets.size()){
        return ;
    }

    currentIndex = index ;

    LabelingSet * newLableingset = labelingsets[currentIndex];

    initializeLableingSet(newLableingset);

    dispatch->OnNewLabelingSetStart(labelingsets[currentIndex = index]);

}
LabelingSet * TaskManager::getLabelinget(int index){
        if(labelingsets.isEmpty()  || index < 0 || index >= labelingsets.size()){
            return nullptr;
        }
        LabelingSet * newLableingset = labelingsets[index];

        initializeLableingSet(newLableingset);

        return newLableingset;
}

LabelingSet * TaskManager::getCurrentLabelingset(){

    if(currentIndex < 0 || currentIndex >= labelingsets.size())
        return NULL;

    return labelingsets[currentIndex];
}

}









