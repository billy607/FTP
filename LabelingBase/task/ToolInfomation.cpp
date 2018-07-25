#include "ToolInfomation.h"
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QFile>
ToolInfomationManager::ToolInfomationManager(){
    QFile file("Version/tool_id_name_mapping.json");

}

bool ToolInfomationManager::initialize(QString filePath){
    QFile file(filePath);
    bool success = false;
    if(file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll() ;
        QJsonDocument document = QJsonDocument::fromJson(data);
        if(document.isObject()){
            QJsonObject idMaps = document.object();
            if(idMaps.contains("id_name_mapping") && idMaps.value("id_name_mapping").isArray()){
                success = true;
                QJsonArray array = idMaps.value("id_name_mapping").toArray();
                for(int index = 0 ; index <array.size();index ++ ){
                    QJsonObject item = array.at(index).toObject();
                    QString id = QString("%1").arg( item.value("tool_id").toInt() );
                    QString tool_name = item.value("tool_name").toString();
                    QString tool_chinese_name = item.value("tool_chinese_name").toString();
                    toolNameMaps.insert(id,tool_name);
                    toolChineseMaps.insert(id,tool_chinese_name);
                }
            }
        }
    }

    return success;
}
