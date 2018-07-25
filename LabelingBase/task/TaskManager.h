#ifndef LABELINGBASEMANAGER_H
#define LABELINGBASEMANAGER_H

#include<QVector>
#include"LabelingSet.h"
#include"labelingbase_global.h"
#include"Log4Labeling.h"
namespace labelingbase {

class LabelingSet;
class Dispatcher ;

class LABELINGBASESHARED_EXPORT TaskManager
{
public:

    static TaskManager* getInstance();

    ~TaskManager();

    bool init(Dispatcher * dispatch);

    Dispatcher * getDispatcher() const{
        return dispatch;
    }

    int getCurrentIndex() const{
        return currentIndex ;
    }
    void setCurrentIndex(int currentIndex){
        this->currentIndex = currentIndex;
    }
    void reset(){
        currentIndex = -1 ;
    }

    int getLabelingsetCount()const{
        return labelingsets.size();
    }

    bool hasNextLabelingset() const ;
    void nextLabelingset();


    bool hasPreLabelingset() const ;
    void preLabelingset();


    void jumpLabelingset(int index);

    LabelingSet * getLabelinget(int index);

    void savaLableingsetResult(LabelingSet * set);


    LabelingSet * getCurrentLabelingset();

    QVector<LabelingSet * > getLabelingsets() const{
        return labelingsets;
    }





private:

    static TaskManager * instance;

    TaskManager();
private:
    QVector<LabelingSet * > labelingsets;
    int currentIndex;
    Dispatcher * dispatch;
};


}


#endif // LABELINGBASEMANAGER_H
