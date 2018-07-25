#include"LabelingSet.h"
#include"JobConfiguration.h"
#include"LabelingData.h"
#include"QFileInfo"
#include"LogUtil.h"
#include"context/ToolContext.h"
#include<QDir>
namespace labelingbase {

QJsonObject serializePoint(QPointF position){
    QJsonObject point ;
    point.insert("x",QString("%1").arg(position.x(),0,'f',2));
    point.insert("y",QString("%1").arg(position.y(),0,'f',2));
    return point;
}

QPointF unserializePoint(QJsonObject object){

    QPointF point(object.value("x").toString().toDouble(), object.value("y").toString().toDouble());
    return point;
}

QJsonArray serializePoints(QVector<QPointF> positions){
    QJsonArray points;
    for(QPointF position : positions){
        QJsonObject point ;
        point.insert("x",QString("%1").arg(position.x(),0,'f',2));
        point.insert("y",QString("%1").arg(position.y(),0,'f',2));
        points.append(point);
    }
    return points;
}

QVector<QPointF> unserializePoints(QJsonArray value){
    QVector<QPointF>  points;
    for(int index = 0 ; index < value.size() ; index ++ ){
        QJsonObject point = value.at(index).toObject();
        points.append(QPointF(point.value("x").toString().toDouble(), point.value("y").toString().toDouble()));
    }
    return points;
}

QJsonObject serializeRect(QRectF rect){
    QJsonObject object;
    object.insert("left",QString("%1").arg(rect.left(),0,'f',2));
    object.insert("right",QString("%1").arg(rect.right(),0,'f',2));
    object.insert("top",QString("%1").arg(rect.top(),0,'f',2));
    object.insert("bottom",QString("%1").arg(rect.bottom(),0,'f',2));
    return object;
}
QRectF unserializeRect(QJsonObject object){

    double left = object.value("left").toString().toDouble();
    double right = object.value("right").toString().toDouble();
    double top = object.value("top").toString().toDouble();
    double bottom = object.value("bottom").toString().toDouble();

    return QRectF(left,top,right-left,bottom-top);
}

template<class LabelingKind>
QMap<QString , QVector<pLabelingData> > unserilaizeShapes(QJsonObject object) ;
template<class LabelingKind>
QJsonObject serilaizeShapes(QMap<QString , QVector<pLabelingData> > shapeAttributes);

template<class T> pLabelingData unserilaizeLabelingShape(QJsonObject ){
    T t;
    return t;
}

template<class T> QJsonObject serilaizeLabelingShape(pLabelingData ){
    return QJsonObject();
}

template<>
QJsonObject serilaizeLabelingShape<LabelingPoint>(pLabelingData data){

    LabelingPoint *multiPoints = dynamic_cast<LabelingPoint*>(data.data());
    if(!multiPoints){
        LogUtil::critical("serilaizeLabelingShape<LabelingMultiPoint> non LabelingMultiPoint ") ;
        return QJsonObject();
    }
    QJsonObject object ;

     QMap<QString , QString> attributes = multiPoints->getAttributes();
     auto attributeIte = attributes.begin();
     while(attributeIte != attributes.end()){
         object.insert(attributeIte.key(),attributeIte.value());
         attributeIte++;
     }

     QJsonObject array = serializePoint(multiPoints->getPostion());
     object.insert(ATTRIBUTE_SHAPE_POSITION_NAME,array);

     return object;
}
template<> pLabelingData unserilaizeLabelingShape<LabelingPoint>(QJsonObject object){

    LabelingPoint *multiPoints = new LabelingPoint();
    pLabelingData data = pLabelingData(multiPoints) ;

    if(object.contains(ATTRIBUTE_SHAPE_POSITION_NAME)){

        auto itAttribute = object.begin();
        while(itAttribute != object.end()){

            QString key = itAttribute.key();
            QJsonValue jsonValue = itAttribute.value();

            if(key  == ATTRIBUTE_SHAPE_POSITION_NAME){
                Q_ASSERT(jsonValue.isObject());
                if(jsonValue.isObject()){
                    QJsonObject value = jsonValue.toObject();
                    QPointF  point = unserializePoint(value);
                    multiPoints->setPostion(point);
                }
            }else {
                Q_ASSERT(jsonValue.isString());
                if(jsonValue.isString()){
                    multiPoints->setAttribute(key,jsonValue.toString());
                }
            }

            itAttribute ++ ;
        }


    }
    return data;
}
template<>
QJsonObject serilaizeLabelingShape<LabelingLine>(pLabelingData data){

    LabelingLine* line = dynamic_cast<LabelingLine*> (data.data());
    if(line == nullptr){
        LogUtil::critical("serilaizeLabelingShape<LabelingLine> non LabelingLine ") ;
        return QJsonObject();
    }
    QJsonObject object ;

     QMap<QString , QString> attributes = line->getAttributes();
     auto attributeIte = attributes.begin();
     while(attributeIte != attributes.end()){
         object.insert(attributeIte.key(),attributeIte.value());
         attributeIte++;
     }

     QJsonArray array = serializePoints(line->getPoints());
     object.insert(ATTRIBUTE_SHAPE_POSITION_NAME,array);

     return object;
}

template<> pLabelingData unserilaizeLabelingShape<LabelingLine>(QJsonObject object){
    LabelingLine * line = new LabelingLine();
    pLabelingData data = pLabelingData(line);

    if(object.contains(ATTRIBUTE_SHAPE_POSITION_NAME)){

        auto itAttribute = object.begin();
        while(itAttribute != object.end()){

            QString key = itAttribute.key();
            QJsonValue jsonValue = itAttribute.value();

            if(key  == ATTRIBUTE_SHAPE_POSITION_NAME){
                Q_ASSERT(jsonValue.isArray());
                if(jsonValue.isArray()){
                    QJsonArray value = jsonValue.toArray();
                    QVector<QPointF>  points = unserializePoints(value);
                    line->setPoints(points);
                }
            }else {
                Q_ASSERT(jsonValue.isString());
                if(jsonValue.isString()){
                    line->setAttribute(key,jsonValue.toString());
                }
            }

            itAttribute ++ ;
        }


    }
    return data;
}
template<>
QJsonObject serilaizeLabelingShape<LabelingPolygon>(pLabelingData data){

    LabelingPolygon * polygon = dynamic_cast<LabelingPolygon*>(data.data());

    if(!polygon){
        LogUtil::critical("serilaizeLabelingShape<pLabelingData> non pLabelingData ") ;
        return QJsonObject();
    }

    QJsonObject object ;

     QMap<QString , QString> attributes = polygon->getAttributes();
     auto attributeIte = attributes.begin();
     while(attributeIte != attributes.end()){
         object.insert(attributeIte.key(),attributeIte.value());
         attributeIte++;
     }

     QJsonArray array = serializePoints(polygon->getPoints());
     object.insert(ATTRIBUTE_SHAPE_POSITION_NAME,array);

     return object;
}

template<>
pLabelingData unserilaizeLabelingShape<LabelingPolygon>(QJsonObject object){
     LabelingPolygon* polygon =  new LabelingPolygon();
     pLabelingData data = pLabelingData(polygon) ;


    if(object.contains(ATTRIBUTE_SHAPE_POSITION_NAME)){

        auto itAttribute = object.begin();
        while(itAttribute != object.end()){

            QString key = itAttribute.key();
            QJsonValue jsonValue = itAttribute.value();

            if(key  == ATTRIBUTE_SHAPE_POSITION_NAME){
                Q_ASSERT(jsonValue.isArray());
                if(jsonValue.isArray()){
                    QJsonArray value = jsonValue.toArray();
                    polygon->setPoints(unserializePoints(value));
                }
            }else {
                Q_ASSERT(jsonValue.isString());
                if(jsonValue.isString()){
                    polygon->setAttribute(key,jsonValue.toString());
                }
            }


            itAttribute ++ ;
        }


    }
    return data;
}
template<>
QJsonObject serilaizeLabelingShape<LabelingRect>(pLabelingData data){
    LabelingRect * rect = dynamic_cast<LabelingRect * >(data.data());

    if(rect == nullptr){
        LogUtil::critical("serilaizeLabelingShape<LabelingRect> non rect ") ;
        return QJsonObject();
    }
    QJsonObject object ;

     QMap<QString , QString> attributes = rect->getAttributes();
     auto attributeIte = attributes.begin();
     while(attributeIte != attributes.end()){
         object.insert(attributeIte.key(),attributeIte.value());
         attributeIte++;
     }

     QJsonObject position = serializeRect(rect->getRect());
     object.insert(ATTRIBUTE_SHAPE_POSITION_NAME,position);

     if(!rect->getChildRects().isEmpty())
         object.insert(ATTRIBUTE_RECT_NAME,serilaizeShapes<LabelingRect>(rect->getChildRects())) ;

     return object;
}

template<>pLabelingData unserilaizeLabelingShape<LabelingRect>(QJsonObject object){
    LabelingRect * rect = new LabelingRect();
    pLabelingData data = pLabelingData(rect);

    if(object.contains(ATTRIBUTE_SHAPE_POSITION_NAME)){

        auto itAttribute = object.begin();
        while(itAttribute != object.end()){

            QString key = itAttribute.key();
            QJsonValue jsonValue = itAttribute.value();

            if(key  == ATTRIBUTE_SHAPE_POSITION_NAME){
                Q_ASSERT(jsonValue.isObject());
                if(jsonValue.isObject()){
                    QJsonObject value = jsonValue.toObject();
                    rect->setRect(unserializeRect(value));
                }
            }else  if(key == ATTRIBUTE_RECT_NAME){
                rect->setChildRects(unserilaizeShapes<LabelingRect>(jsonValue.toObject()));
            }else{
                Q_ASSERT(jsonValue.isString());
                if(jsonValue.isString()){
                    rect->setAttribute(key,jsonValue.toString());
                }
            }
            itAttribute ++ ;
        }

    }
    return data;
}

template<class LabelingKind>
QJsonArray serilaizeShapes(QVector<pLabelingData> shapes ){

    QJsonArray array;
    auto shapesIte = shapes.begin();
    while(shapesIte != shapes.end()){
        array.append(serilaizeLabelingShape<LabelingKind>(*shapesIte));
        shapesIte ++ ;
    }
    return array;
}


template<class LabelingKind>
QVector<pLabelingData> unserilaizeShapes(QJsonArray array){
    QVector<pLabelingData> shapes;
    for(int index = 0 ; index < array.size(); index ++ ){
        QJsonValue value = array.at(index);
        Q_ASSERT(value.isObject());
        if(value.isObject()){
            shapes.append(unserilaizeLabelingShape<LabelingKind>(value.toObject()));
        }
    }
    return shapes;
}


template<class LabelingKind>
QMap<QString , QVector<pLabelingData> > unserilaizeShapes(QJsonObject object){
    QMap<QString , QVector<pLabelingData> > result;
    auto itAttribute = object.begin();
    while(itAttribute != object.end()){
        QString key = itAttribute.key();
        QJsonValue value = itAttribute.value();
        Q_ASSERT(value.isArray());
        if(value.isArray()){
            result.insert(key , unserilaizeShapes<LabelingKind>(value.toArray()));
        }
        itAttribute ++ ;
    }

    return result;
}

template<class LabelingKind>
QJsonObject serilaizeShapes(QMap<QString , QVector<pLabelingData> > shapeAttributes){

    QJsonObject object;
    auto attributeIte = shapeAttributes.begin();
    while(attributeIte != shapeAttributes.end()){
        object.insert(attributeIte.key(),serilaizeShapes<LabelingKind>(attributeIte.value()));
        attributeIte++;
    }

    return object;
}


void unserilaizeImage(pLabelingData data , QJsonObject object){

    LabelingImage * image = dynamic_cast<LabelingImage*>(data.data());

    auto itAttribute = object.begin();
    while(itAttribute != object.end()){
        QString key = itAttribute.key();
        QJsonValue value = itAttribute.value();
        if(value.isString()){
            image->setAttribute(key,value.toString());
        }else if(value.isObject()){
            if(key == ATTRIBUTE_POLYGON_NAME){
                image->setPolygons(unserilaizeShapes<LabelingPolygon>(value.toObject()));
            }else if(key == ATTRIBUTE_RECT_NAME){
                image->setRects(unserilaizeShapes<LabelingRect>(value.toObject()));
            }else if(key == ATTRIBUTE_LINE_NAME){
                image->setLines(unserilaizeShapes<LabelingLine>(value.toObject()));
            }else if(key == ATTRIBUTE_MULTIPOINT_NAME){
                image->setPoints(unserilaizeShapes<LabelingPoint>(value.toObject()));
            }
        }
        itAttribute ++;
    }

}

QJsonObject serializeImage(pLabelingData data){


    LabelingImage * image = dynamic_cast<LabelingImage*>(data.data());
    QJsonObject object;
    if(image){

        //插入属性
        QMap<QString , QString> attributes = image->getAttributes();
        auto attributeIte = attributes.begin();
        while(attributeIte!= attributes.end()){
            object.insert(attributeIte.key(),attributeIte.value());
            attributeIte ++ ;
        }


        if(!image->getPolygons().isEmpty())
            object.insert(ATTRIBUTE_POLYGON_NAME,serilaizeShapes<LabelingPolygon>(image->getPolygons())) ;
        if(!image->getLines().isEmpty())
            object.insert(ATTRIBUTE_LINE_NAME,serilaizeShapes<LabelingLine>(image->getLines())) ;
        if(!image->getRects().isEmpty())
            object.insert(ATTRIBUTE_RECT_NAME,serilaizeShapes<LabelingRect>(image->getRects())) ;
        if(!image->getPoints().isEmpty())
            object.insert(ATTRIBUTE_MULTIPOINT_NAME,serilaizeShapes<LabelingPoint>(image->getPoints())) ;


    }


    return object;
}





LabelingSet::LabelingSet(){
    setAttribute(RESULT_VERSION_NAME,RESULT_VERSION_VALUE);
    initialized = false;
    this->index = -1 ;
}

ImageLabelingset::ImageLabelingset(QString imagePath){
    //insert("image", pLabelingData(new LabelingImage(imagePath))) ;

    image = pLabelingImage( new  LabelingImage(imagePath) )  ;
}


QString ImageLabelingset::getName() const{

//    QString dataPath = ToolContext::getInstance()->getRootPath()+"/"+ToolContext::getInstance()->getDataPath()+"/";
//    QFileInfo imagePath(dataPath + "/" + image->getPath());

    LabelingImage * labelingimage = dynamic_cast<LabelingImage *>(image.data());
    Q_ASSERT(labelingimage != nullptr) ;
    if(labelingimage != nullptr){
        return LabelingImage::getImageMD5(labelingimage->getPath());
    }
    return "";

}

QString ImageLabelingset::getPath() const{

//    QString dataPath = ToolContext::getInstance()->getRootPath()+"/"+ToolContext::getInstance()->getDataPath()+"/";
//    QDir dataDir(dataPath);
//    QFileInfo imageInfo = QFileInfo(dataPath + image->getPath());
//    QString relativePath = dataDir.relativeFilePath(imageInfo.absoluteDir().absolutePath());

    return ".";


}

bool ImageLabelingset::unserialize(QString content){
    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());

    bool tryUnserialize = false;
    if(document.isObject()){
        QJsonObject object = document.object();
        if(object.contains(RESULT_VERSION_NAME)
                && object.value(RESULT_VERSION_NAME).toString() == RESULT_VERSION_VALUE ){
            tryUnserialize = true;
            unserilaizeImage(this->image, object);
        }
    }
    return tryUnserialize;

}

QString  ImageLabelingset::serialize(){

    QJsonObject object = serializeImage(image);
    object.insert(RESULT_VERSION_NAME,getAttribute(RESULT_VERSION_NAME));
    QJsonDocument document(object);
    return document.toJson();
}


ImageDirLabelingset::ImageDirLabelingset(QString path){

    imageDir = pLabelingData(new LabelingImageDir(path));
}

QString ImageDirLabelingset::getName()const{

    LabelingImageDir * dir =  dynamic_cast<LabelingImageDir*>(imageDir.data());
    QFileInfo imagePath(dir->getPath());

    return imagePath.fileName() ;
}

QString ImageDirLabelingset::getPath() const{

    LabelingImageDir * dir =  dynamic_cast<LabelingImageDir*>(imageDir.data());

    QDir dataDir(ToolContext::getInstance()->getRootPath()+"/"+ToolContext::getInstance()->getDataPath()+"/");
    QFileInfo dirInfo = QFileInfo(dir->getPath());
    QString relativePath = dataDir.relativeFilePath(dirInfo.absoluteDir().absolutePath());

    return relativePath;
}

QString ImageDirLabelingset::serialize(){
    QJsonObject object;
    object.insert(RESULT_VERSION_NAME,getAttribute(RESULT_VERSION_NAME));
    QMap<QString ,QString> attributes = imageDir->getAttributes();
    auto attributeIte = attributes.begin();
    while(attributeIte!= attributes.end()){
        object.insert(attributeIte.key(),attributeIte.value());
        attributeIte ++;
    }
    QJsonDocument document(object);

    return document.toJson();
}

bool ImageDirLabelingset::unserialize(QString content){

    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());

    bool tryUnserialize = false;
    Q_ASSERT(document.isObject());
    if(document.isObject()){
        QJsonObject object = document.object();
        if(object.contains(RESULT_VERSION_NAME)
                && object.value(RESULT_VERSION_NAME).toString() == RESULT_VERSION_VALUE ){
            auto itAttribute = object.begin();
            while(itAttribute != object.end()){
                QString key = itAttribute.key();
                if(itAttribute.value().isString()){
                    QString value = itAttribute.value().toString();
                    getImageDir()->setAttribute(key,value);
                }
                itAttribute ++ ;
            }
            tryUnserialize = true;
        }
    }
    return tryUnserialize;

}


void addShape(QMap<QString, QVector<pLabelingData> >& typeMap ,QString key , pLabelingData item  ){
    if(typeMap.contains(key)){
        typeMap[key].append(item);
    }else{
        QVector<pLabelingData> array ;
        array.append(item);
        typeMap.insert(key,array);
    }
}
void removeShape(QMap<QString, QVector<pLabelingData> >& typeMap ,QString key , int index){
    Q_ASSERT(typeMap.contains(key));
    if(typeMap.contains(key)){
        QVector<pLabelingData> & array = typeMap[key];
        Q_ASSERT(index >= 0 && index < array.size());
        array.removeAt(index);
    }
}


}
