#include "JobConfiguration.h"
#include"LabelingData.h"
#include<QXmlStreamReader>
#include<QFile>
#include"LabelingSet.h"
#include"LogUtil.h"
#include"LabelingSpeciication.h"
#include"context/ToolContext.h"
#include"JobConfiguration.h"
namespace labelingbase {

QVector<JobStepOption> readStepOptions(QXmlStreamReader & reader){
    QVector<JobStepOption> options;
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token = reader.readNext();
        QStringRef name = reader.name();
        if(token == QXmlStreamReader::StartElement){
            if(name == "option"){
                QString field = reader.attributes().value("field").toString().trimmed();
                QString value= reader.attributes().value("value").toString().trimmed();

                options.append(JobStepOption(field,value));
            }
        }else if(QXmlStreamReader::EndElement && name=="step"){
            break;
        }
    }

    return options;
}

QMap<QString,QString> readKeyValues(QXmlStreamReader & reader){
    QMap<QString,QString> values;
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token = reader.readNext();
        QStringRef name = reader.name();
        if(token == QXmlStreamReader::StartElement){
            if(name == "entry"){
                QString key = reader.attributes().value("key").toString();
                QString value = reader.attributes().value("value").toString();
                values.insert(key,value);
            }
        }else if(token == QXmlStreamReader::EndElement && name =="key-values" ){
            break;
        }
    }

    return values;
}

QMap<QString,QString> readValueLabel(QXmlStreamReader & reader){
    QMap<QString,QString> values;
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token = reader.readNext();
        QStringRef name = reader.name();
        if(token == QXmlStreamReader::StartElement){
            if(name == "entry"){
                QString value = reader.attributes().value("value").toString();
                QString label = reader.attributes().value("label").toString();
                values.insert(value,label);
            }
        }else if(token == QXmlStreamReader::EndElement && name =="value-label" ){
            break;
        }
    }
    return values;
}

JobStep readStep(QXmlStreamReader & reader){
    QString field = reader.attributes().value("target").toString().trimmed();
    QString toolId= reader.attributes().value("toolId").toString().trimmed();
    QString toolName = reader.attributes().value("toolName").toString().trimmed();
    QString fieldName = reader.attributes().value("field").toString().trimmed();
    JobStep step(toolId,toolName,field) ;
    step.setField(fieldName);

    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token = reader.readNext();
        QStringRef name = reader.name();
        if(token == QXmlStreamReader::StartElement){
            if(name == "key-values"){
                step.setKeyValues(readKeyValues(reader));
            }else if (name == "value-label"){
                step.setValueLabels(readValueLabel(reader));
            }else if( name == "option"){
                QString field = reader.attributes().value("field").toString().trimmed();
                QString value= reader.attributes().value("value").toString().trimmed();

                step.addOption(field,value);
            }
        }else if(token == QXmlStreamReader::EndElement && name =="step" ){
            break;
        }
    }
    return step;
}

QVector<JobStep> readSteps(QXmlStreamReader & reader){
    QVector<JobStep> steps;
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token = reader.readNext();
        QStringRef name = reader.name();
        if(token == QXmlStreamReader::StartElement){
            if(name == "step"){
                steps.append(readStep(reader));
            }
        }else if(QXmlStreamReader::EndElement && name=="steps"){
            break;
        }
    }

    return steps;
}

QVector<LabelEnumOption> readEnumDefOption(QXmlStreamReader & reader){
    QVector<LabelEnumOption> enumOptions;
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token = reader.readNext();
        QStringRef name = reader.name();
        if(token == QXmlStreamReader::StartElement){
            if(name == "param"){
                QString value = reader.attributes().value("value").toString().trimmed();
                QString label = reader.attributes().value("label").toString().trimmed();
                QString key = reader.attributes().value("key").toString().trimmed();
                LabelEnumOption option(label,value,key);
                enumOptions.append(option);
            }
        }else if(QXmlStreamReader::EndElement && name=="enum"){
            break;
        }
    }
    return enumOptions;
}

QMap<QString, LabelingEnumDef > readEnumDefinition( QXmlStreamReader & reader){
    QMap<QString, LabelingEnumDef > defintions;
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token = reader.readNext();
        QStringRef name = reader.name();
        if(token == QXmlStreamReader::StartElement){
            if(name == "enum"){
                QString key = reader.attributes().value("key").toString().trimmed();

                LabelingEnumDef defintion ;
                defintion.setOptions(readEnumDefOption(reader));

                defintions.insert(key,defintion);
            }
        }else if(QXmlStreamReader::EndElement && name=="enumDefinitions"){
            break;
        }
    }
    return defintions;
}

JobConfiguration::~JobConfiguration(){
}



bool JobConfiguration::init(QString xmlFilePath ){
    QString xmlFile = xmlFilePath;
    QFile file(xmlFile);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }
    bool success = true;
    QXmlStreamReader reader(&file);
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token = reader.readNext();
        if(token == QXmlStreamReader::StartElement){
            QStringRef name = reader.name();
                if(name == "steps"){
                QVector<JobStep> steps = readSteps(reader);
                setSteps(steps);
            }
//            else if(name == "enumDefinitions"){
//                QMap<QString, LabelingEnumDef > definitions = readEnumDefinition(reader);
//                setDefinitons(definitions);
//            }
        }
    }
    file.close();

    return success;
}




}
