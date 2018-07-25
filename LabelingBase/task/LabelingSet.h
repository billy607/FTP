#ifndef LABELINGSET_H
#define LABELINGSET_H

#include<QString>

#include<QFile>
#include<QMap>
#include"LabelingData.h"
#include"labelingbase_global.h"



namespace labelingbase {

class LabelingData;

#define RESULT_VERSION_NAME "result_version"
#define RESULT_VERSION_VALUE "1.0"
class LABELINGBASESHARED_EXPORT LabelingSet : public LabelingData
{
public:

    enum  Status{Status_Unknown,Status_UnLabeled,Status_Labeled,Status_Invaid};

    LabelingSet();


    virtual QString getName() const = 0 ;

    virtual QString getPath() const = 0 ;

    virtual bool unserialize( QString )  = 0 ;

    virtual QString serialize() = 0 ;


    int getIndex() const{
        return index;
    }
    void setIndex(int index){
        this->index = index;
    }

    bool hasInitialized() const{
        return initialized;
    }
    void setInitialized(bool initialized){
        this->initialized = initialized;
    }

private:
     bool initialized;
     int index;

};

/**
 * @brief The ImageLabelingset class
 * LabelingImage 为ImageLabelingset的一个属性，属性的key为image
 * 请不要覆盖该属性
 */

//#define LABEL_IMAGE_RESULT_NAME_OPTION  "result-name-type"
//#define LABEL_IMAGE_RESULT_NAME_MD5  "md5"
//#define LABEL_IMAGE_RESULT_NAME_FILENAME  "filename"

//#define LABEL_RESULT_PATH_OPTION  "result-path-type"
//#define LABEL_RESULT_PATH_LABELING  "labeling"
//#define LABEL_RESULT_PATH_LABELING_ADN_PATH  "labeling-path"



class LABELINGBASESHARED_EXPORT ImageLabelingset : public LabelingSet{
public:
    ImageLabelingset(QString imagePath);


     QString getName() const override ;

     QString getPath() const override;

     pLabelingData getImage(){
         return image;
     }
     virtual bool unserialize( QString );

     virtual QString serialize();

private:
     pLabelingData image;



};

class LABELINGBASESHARED_EXPORT ImageDirLabelingset: public LabelingSet{

public:
    ImageDirLabelingset(){}
    ImageDirLabelingset(QString path);

    QString getName() const override ;

    QString getPath() const override;

    pLabelingData getImageDir() const{
        return imageDir ;
    }

    virtual bool unserialize( QString ) ;

    virtual QString serialize() ;

private:
   pLabelingData imageDir;


};

}



#endif // LABELINGSET_H
