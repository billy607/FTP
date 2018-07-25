#ifndef CONFIGURELOADER_H
#define CONFIGURELOADER_H

#include<QString>
class ConfigureReaderAndWriter
{
public:
    ConfigureReaderAndWriter(QString filePath){
        this->filePath = filePath;
    }

    void load();

    void save();

private:
    QString filePath;
};

#endif // CONFIGURELOADER_H
