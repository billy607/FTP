#ifndef LOGUTIL_H
#define LOGUTIL_H

#include<QDebug>
#include<QFile>
#include<QDateTime>
class LogUtil
{
public:

    static void debug(QString content ) {
#ifdef QT_DEBUG
        qDebug()<<content;
#else
        QFile file(QDateTime::currentDateTime().toString("yyyy-MM-dd")+"-debug.log");
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        std::string message= QString(content + "\n").toStdString();
        file.write(message.c_str());
        file.close();
#endif
    }

    static void verbose(QString content ) {
#ifdef QT_DEBUG
        qDebug()<<content;
#else
        QFile file(QDateTime::currentDateTime().toString("yyyy-MM-dd")+"-debug.log");
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        std::string message= QString(content + "\n").toStdString();
        file.write(message.c_str());
        file.close();
#endif
    }

    static void info(QString content ) {
#ifdef QT_DEBUG
        qDebug()<<content;
#else
        QFile file(QDateTime::currentDateTime().toString("yyyy-MM-dd")+"-debug.log");
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        std::string message= QString(content + "\n").toStdString();
        file.write(message.c_str());
        file.close();
#endif
    }

    static void warning(QString content ) {
#ifdef QT_DEBUG
        qDebug()<<content;
#else
        QFile file(QDateTime::currentDateTime().toString("yyyy-MM-dd")+"-debug.log");
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        std::string message= QString(content + "\n").toStdString();
        file.write(message.c_str());
        file.close();
#endif
    }

    static void critical(QString content  ) {
#ifdef QT_DEBUG
        qDebug()<<content;
#else
        QFile file(QDateTime::currentDateTime().toString("yyyy-MM-dd")+"-debug.log");
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        std::string message= QString(content + "\n").toStdString();
        file.write(message.c_str());
        file.close();
#endif
    }

private:
    LogUtil(){}
};

#endif // LOGUTIL_H
