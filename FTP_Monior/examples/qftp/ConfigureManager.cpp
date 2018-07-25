#include "ConfigureManager.h"
#include"ConfigureReaderAndWriter.h"
#include<QFile>
ConfigureManager * ConfigureManager::instance = NULL;
ConfigureManager::ConfigureManager()
{
    autoLogin = false;
}

ConfigureManager * ConfigureManager::getInstance(){
    if(!instance){
        instance = new ConfigureManager();
    }
    return instance;
}
void ConfigureManager::init(QString configurePath){

    if(!QFile(configurePath).exists()){
        ConfigureReaderAndWriter writer(configurePath);
        writer.save();
    }else{
        ConfigureReaderAndWriter reader(configurePath);
        reader.load();
    }

}
