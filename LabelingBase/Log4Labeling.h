#ifndef LOG4LABELING_H
#define LOG4LABELING_H

#include"labelingbase_global.h"
#include<QFile>
#include<QDebug>
#include<QTextStream>
#include<QDateTime>
#include<context/ManagerContext.h>
#include<context/ToolContext.h>
using namespace labelingbase;
class LABELINGBASESHARED_EXPORT Log4Labeling
{
public:
    Log4Labeling(){};

    static Log4Labeling * getInstance() {
        if( !instance ){
            instance = new Log4Labeling();
            instance->startTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        }
        return instance;
    }

    void info(QString type, QString message){
        QString userName = ToolContext::getInstance()->getUserInfo().getUserName();
        QString logFilePath = ToolContext::getInstance()->getRootPath() + "/" +
                      ToolContext::getInstance()->getDataPath() + "/Label/" + QString("%1%2.dll").arg(userName).arg(startTime);

        QFile logFile(logFilePath);
        if(logFile.open(QIODevice::Text | QIODevice::Append | QIODevice::WriteOnly)){
            QTextStream out(&logFile);

            out << QDateTime::currentDateTime().toTime_t() << " "<< userName << " "<< type << " " << message << "\n";
            qDebug() << QDateTime::currentDateTime().toTime_t() << " "<< userName << " "<< type << " " << message;
            logFile.close();
        }

    }

private:
    static Log4Labeling * instance;
    QString startTime;
};

#endif // LOG4LABELING_H
