#ifndef LABELINGDATA_H
#define LABELINGDATA_H

#include<QMap>
#include<QPixmap>
#include<QString>
#include<QSharedPointer>
#include<QPair>
#include<QVector>
#include<QFileInfo>
class EnumRequrement;

#include"labelingbase_global.h"
namespace labelingbase {

#define ATTRIBUTE_VALID  "valid"
#define ATTRIBUTE_ROTATE "rotate"


enum LabelingType{
    LabelingType_Point,
    LabelingType_Line,
    LabelingType_Rect,
    LabelingType_Polygon,
    LabelingType_MultiPoint,
    LabelingType_Enum,
    LabelingType_Text,
    LabelingType_Image,
    LabelingType_ImageDir,
    LabelingType_Array,
    LabelingType_Null,
    LabelingType_ImageDataset,
    LabelingType_Unknown
};
//class LabelingData;
//typedef QSharedPointer<LabelingData> pLabelingData;


#define ATTRIBUTE_POLYGON_NAME "polygons"
#define ATTRIBUTE_RECT_NAME "rects"
#define ATTRIBUTE_LINE_NAME "lines"
#define ATTRIBUTE_MULTIPOINT_NAME "multiPoints"
#define ATTRIBUTE_SHAPE_POSITION_NAME "position"

class LabelingData;
typedef QSharedPointer<LabelingData> pLabelingData ;
class LABELINGBASESHARED_EXPORT LabelingData
{
public:
    LabelingData(){
    }

    virtual ~LabelingData(){
    }

    void setAttribute(QString key ,QString value){
        attributes.insert(key,value);
    }
    QString getAttribute(QString key) const{
        return attributes.value(key);
    }
    QMap<QString , QString> getAttributes() const{
        return attributes;
    }
    bool containsAttribute(QString attribute) const{
        return attributes.contains(attribute);
    }


private:
    QMap<QString,QString> attributes;

};

typedef QVector<pLabelingData> LabelingDatas;

class LABELINGBASESHARED_EXPORT LabelingPolygon  : public LabelingData{
public:
    LabelingPolygon(){
        //this->type = LabelingType_Polygon;
    }
    LabelingPolygon(QVector<QPointF> polygon):
    points(polygon){
        //this->type = LabelingType_Polygon;
    }
    virtual bool isNull()const{
        return points.isEmpty();
    }
    QVector<QPointF> getPoints() const{
        return points;
    }
    void setPoints(QVector<QPointF> points){
        this->points = points;
    }

private:
    QVector<QPointF> points;


};

class LABELINGBASESHARED_EXPORT LabelingPoint : public LabelingData {
public:
    LabelingPoint(){
    }
    LabelingPoint( QPointF position):
    position(position){
    }
    virtual bool isNull()const{
        return false;
    }
    QPointF getPostion() const{
        return position;
    }
    void setPostion(QPointF position){
        this->position = position;
    }

private:
    QPointF position;

};

class LABELINGBASESHARED_EXPORT LabelingLine : public LabelingData {
public:
    LabelingLine(){
        //this->type = LabelingType_Line;
    }

    LabelingLine(QVector<QPointF> points) :points(points)
    {
        //this->type = LabelingType_Line;
    }

    virtual bool isNull()const{
        return points.isEmpty();
    }

    QVector<QPointF> getPoints() const{
        return points;
    }
    void setPoints(QVector<QPointF> points){
        this->points = points;
    }

private:
    QVector<QPointF> points;

};
LABELINGBASESHARED_EXPORT void addShape(QMap<QString, QVector<pLabelingData> >& typeMap ,QString key , pLabelingData item  );
LABELINGBASESHARED_EXPORT void removeShape(QMap<QString, QVector<pLabelingData> >& typeMap ,QString key , int index);

class LABELINGBASESHARED_EXPORT LabelingRect : public LabelingData {
public:
    LabelingRect(){
    }
    LabelingRect(const QPointF & topLeft, const QPointF & bottomRight):rect(topLeft,bottomRight){
    }

    LabelingRect(const QPointF & topLeft, const QSizeF & size):rect(topLeft,size){
    }

    LabelingRect(double x, double y, double width, double height):rect(x,y,width,height){
    }
    LabelingRect(QRectF rect):rect(rect){
    }

    virtual bool isNull()const{
        return ((QRectF*)this)->isNull();
    }
    QRectF getRect() const{
        return rect;
    }
    void setRect(QRectF rect){
        this->rect = rect;
    }

    //rects CRUD
    QMap<QString, LabelingDatas> getChildRects() const{
        return this->childRects;
    }
    LabelingDatas getChildRects(QString key) const{
        return childRects.value(key);
    }
    void setChildRects(QMap<QString, LabelingDatas> childRects){
        this->childRects = childRects;
    }
    void setChildRects(QString key,LabelingDatas rectArrays){
        this->childRects.insert( key ,  rectArrays );
    }
    void addChildRect(QString key, pLabelingData rect){
        addShape(childRects,key ,rect);
    }
    void removeChildRect(QString key, int index ){
        removeShape(childRects,key,index);
    }




private:
    QRectF rect;
    QMap<QString , LabelingDatas > childRects;
};




class LabelingImage;
typedef QSharedPointer<LabelingImage> pLabelingImage;
class LABELINGBASESHARED_EXPORT LabelingImage :public LabelingData{
public:
    LabelingImage(){
    }

    LabelingImage(QString path);
    QString getPath()const{
        return path;
    }
    void setPath(QString path){
        this->path = path;
    }
    bool isValid() const{
        return !path.isEmpty();
    }

    QPixmap getImage();

    virtual bool isNull()const{
        return !QFileInfo(path).exists();
    }

    //获取图形列表，type = [ATTRIBUTE_POLYGON_NAME , ATTRIBUTE_RECT_NAME,ATTRIBUTE_LINE_NAME
    //                  ,ATTRIBUTE_MULTILINE_NAME]
    QVector<pLabelingData> getShapes(QString shapeName, QString field);

    //polygons CRUD
    QMap<QString, LabelingDatas> getPolygons() const{
        return polygons;
    }
    LabelingDatas getPolygons(QString key) const{
        return polygons.value(key);
    }
    void setPolygons(QMap<QString, LabelingDatas> polygons){
        this->polygons = polygons;
    }
    void setPolygons( QString key , LabelingDatas polygonsArray){
        this->polygons.insert( key , polygonsArray );
    }
    void addPolygon(QString key, pLabelingData polygon){
        addShape(polygons,key ,polygon);
    }
    void removePolygon(QString key, int index ){
        removeShape(polygons,key,index);
    }

    //rects CRUD
    QMap<QString, LabelingDatas> getRects() const{
        return this->rects;
    }
    LabelingDatas getRects(QString key) const{
        return rects.value(key);
    }
    void setRects(QMap<QString, LabelingDatas> rects){
        this->rects = rects;
    }
    void setRects(QString key,LabelingDatas rectArrays){
        this->rects.insert( key ,  rectArrays );
    }
    void addRect(QString key, pLabelingData rect){
        addShape(rects,key ,rect);
    }
    void removeRect(QString key, int index ){
        removeShape(rects,key,index);
    }

    //Lines CRUD
    QMap<QString, LabelingDatas> getLines() const{
        return this->lines;
    }
    LabelingDatas getLines(QString key) const{
        return lines.value(key);
    }
    void setLines(QMap<QString, LabelingDatas> lines){
        this->lines = lines;
    }
    void setLines(QString key,LabelingDatas lineArrays){
        this->lines.insert( key ,  lineArrays );
    }
    void addLine(QString key, pLabelingData line){
        addShape(lines,key ,line);
    }
    void removeLine(QString key, int index ){
        removeShape(lines,key,index);
    }


    //rects CRUD
    QMap<QString, LabelingDatas> getPoints()const{
        return points;
    }
    LabelingDatas getPoints(QString key) const{
        return points.value(key);
    }
    void setPoints(QMap<QString, LabelingDatas> points){
        this->points = points;
    }
    void setPoints(QString key,LabelingDatas pointsArray){
        this->points.insert( key ,  pointsArray );
    }
    void addPoints(QString key, pLabelingData point){
        addShape(points,key ,point);
    }
    void removePoint(QString key, int index ){
        removeShape(points,key,index);
    }

    static QString getImageMD5(QString fileName) ;
private:

    static QMap<QString , QString> imageMD5Maps;

    QString path;
    QMap<QString , LabelingDatas> polygons;
    QMap<QString , LabelingDatas > rects;
    QMap<QString , LabelingDatas> lines;
    QMap<QString , LabelingDatas > points;
};
class LabelingImageDir;
typedef QSharedPointer<LabelingImageDir> pLabelingImageDir;
class LABELINGBASESHARED_EXPORT LabelingImageDir : public LabelingData{
public:
    LabelingImageDir(){
    }
    LabelingImageDir(QString path) ;

    QVector<pLabelingData> getImages() {
        return images;
    }
    virtual bool isNull()const{
        return false;
    }
    void setPath(QString path);

    QString getPath() const{
        return path;
    }


private:
    QString path;
    QVector<pLabelingData> images;
};


class LABELINGBASESHARED_EXPORT LabelingText  {
public:
    LabelingText(){
    }
    LabelingText(QString text):text(text){
    }
    virtual bool isNull()const{
        return text.isEmpty();
    }

    QString getText() const{
        return text;
    }

    void setText(QString text){
        this->text = text;
    }

private:
    QString text;

};

class LabelEnumOption {
public:
    LabelEnumOption(){}

    LabelEnumOption(QString label ,QString value, QString key)
    {
        this->label = label;
        this->value = value;
        this->key = key;
    }
    QString getLabel() const{
        return label;
    }
    QString getValue() const{
        return value;
    }
    QString getKey() const{
        return key;
    }

private:
    QString label;
    QString value;
    QString key;
};

class LabelingEnumDef{
public:
    LabelingEnumDef(){
    }

    LabelingEnumDef(QVector<LabelEnumOption> options){
        this->options = options;
    }

    bool containsValue(QString value){
        for(LabelEnumOption option : options){
            if(option.getValue().trimmed() == value)
                return true;
        }

        return false;
    }

    QString getLabel(QString value) const{
        for(LabelEnumOption option : options){
            if(option.getValue() == value){
                return option.getLabel();
            }
        }
        return "";
    }
    QVector<LabelEnumOption> getOptions() const{
        return options;
    }
    void setOptions(QVector<LabelEnumOption> options){
        this->options = options;
    }

private:
    QVector<LabelEnumOption> options;

};

class LABELINGBASESHARED_EXPORT LabelingEnum {
public:
    LabelingEnum(){
    }

    LabelingEnum(QString definitionName) :definitionName(definitionName){
    }

    QString getValue() const{
        return value;
    }
    void setValue(QString value){
        this->value = value;
    }

//    LabelingEnumDef * getDefinition() const;
    QString getDefinitionName() const{
        return definitionName;
    }
    void setDefinitionName(QString definitionName){
        this->definitionName = definitionName;
    }


    virtual bool isNull()const{

        return value.isEmpty();
    }

private:
    QString value;
    QString definitionName;
};


}


#endif // LABELINGDATA_H
