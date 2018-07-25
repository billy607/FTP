#include "LabelingSpeciication.h"
namespace labelingbase{

LabelingSpeciication * LabelingSpeciication::instance = nullptr;

LabelingSpeciication * LabelingSpeciication::getInstance(){
    if(instance == nullptr){
        instance = new LabelingSpeciication();
    }

    return instance;
}


}


