#ifndef LABELINGDISPATCH_H
#define LABELINGDISPATCH_H

#include"labelingbase_global.h"
#include"LabelingData.h"
namespace labelingbase {

class LabelingImage;
class LabelingSet ;
class LabelingData;
class LABELINGBASESHARED_EXPORT Dispatcher
{
public:
    Dispatcher(){}
    ~Dispatcher(){}

    /**
     *已经实现了图片扫面，和图片文件夹扫描，具体见scanner.h，
     * 扫描结果为一个文件名列表，可以对其进行二次包装，返回列表即可
     */
    virtual QVector<LabelingSet*> OnInitialize() = 0 ;

    virtual void OnStartup() = 0 ;

    virtual void OnNewLabelingSetStart(LabelingSet * newLabelingSet) = 0 ;

    virtual void onFinished() = 0 ;

    virtual void onLoadAttachment(pLabelingData labelingData,QString filename) = 0 ;

};


}


#endif // LABELINGDISPATCH_H
