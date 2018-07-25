#ifndef JOB_CONFIGURATION_H
#define JOB_CONFIGURATION_H

#include<QString>
#include<QMap>
#include<QVector>
#include<QPair>
#include"LabelingData.h"
#include"labelingbase_global.h"
namespace labelingbase{


class LabelingSet ;
class LabelingData;

class LABELINGBASESHARED_EXPORT JobStepOption : private QPair<QString,QString>{

public:
    JobStepOption(QString field , QString value):QPair<QString,QString>(field,value) {

    }

    JobStepOption(){}

    QString getFiled()const{
        return first;
    }
    QString getValue()const{
        return second;
    }

};

class LABELINGBASESHARED_EXPORT JobStep{
public:

    JobStep(QString toolId, QString toolName, QString target){
        this->toolId = toolId;
        this->toolName = toolName;
        this->target = target;
    }

    JobStep(){}

    QString getToolName() const{
        return toolName;
    }
    void setToolName(QString toolName){
        this->toolName = toolName;
    }
    QString getTargetName() const{
        return target;
    }
    void setTarget(QString target){
        this->target = target;
    }
    QString getToolId()const{
        return toolId;
    }
    void setToolId(QString toolId){
        this->toolId = toolId;
    }
    QString getField() const{
        return field;
    }
    void setField(QString field){
        this->field = field;
    }

    void addOption(QString field , QString value){
        options.append(JobStepOption(field,value));
    }
    bool containsOption(QString field ){
        for(JobStepOption option : options){
            if(option.getFiled() == field){
                return true;
            }
        }
        return false;
    }

    JobStepOption getOption(QString field){
        Q_ASSERT(containsOption(field));
        for(JobStepOption option : options){
            if(option.getFiled() == field){
                return option;
            }
        }
        return JobStepOption();
    }
    void setOptions(QVector<JobStepOption> options){
        this->options = options;
    }

    QVector<JobStepOption> getOptions()const{
        return options;
    }

    void setKeyValues(QMap<QString,QString> key_values){
        this->key_values = key_values;
    }

    QMap<QString,QString> getKeyValues()const{
        return this->key_values;
    }

    void setValueLabels(QMap<QString,QString> value_labels){
        this->value_labels = value_labels;
    }
    QMap<QString,QString> getValueLabels()const{
        return this->value_labels;
    }


private:
    QString target;
    QString toolName;
    QString toolId;
    QString field;
    QVector<JobStepOption> options;
    QMap<QString,QString> key_values ;
    QMap<QString,QString> value_labels;
};



class LABELINGBASESHARED_EXPORT JobConfiguration
{
public:

    JobConfiguration(){
    }
    ~JobConfiguration();
    bool init(QString xmlFilePath);

    void setSteps(QVector<JobStep> steps){
        this->steps = steps;
    }
    QVector<JobStep> getSteps() const{
        return steps;
    }
//    void addDefinitions(QString name , LabelingEnumDef definition){
//        definitions.insert(name,definition);
//    }
//    bool containsDefnition(QString name){
//        auto ite = definitions.begin();
//        while(ite != definitions.end()){
//            if(ite.key() == name)
//                return true;
//            ite ++ ;
//        }
//        return false;
//    }

//    LabelingEnumDef getDefinition(QString name) {
//        auto ite = definitions.begin();
//        while(ite != definitions.end()){
//            if(ite.key() == name)
//                return ite.value();
//            ite ++ ;
//        }
//        return LabelingEnumDef();
//    }
//    void setDefinitons(QMap< QString , LabelingEnumDef > definitions){
//        this->definitions = definitions;
//    }



private:
    QVector<JobStep> steps;
    //QMap< QString , LabelingEnumDef > definitions;

};








}


#endif // RESULTCHECKER_H
