#include "ConfigureReaderAndWriter.h"
#include<QFile>
#include<QXmlStreamReader>
#include<QXmlStreamWriter>
#include"context/Context.h"
#include"ConfigureManager.h"

void readLoginInformation(QXmlStreamReader & reader){
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token =  reader.readNext();
        QStringRef name = reader.name();
        if(token == QXmlStreamReader::StartElement){
            if(name == "userName"){
                QString text = reader.readElementText();
                Context::getInstance()->getUserInfo().setUserName(text);
            }else if(name  == "password"){
                QString text = reader.readElementText();
                Context::getInstance()->getUserInfo().setPassword(text);
            }
        }else if(token == QXmlStreamReader::EndElement && name == "Login"){
            break;
        }
    }
}

void ConfigureReaderAndWriter::load(){

    QFile file(filePath) ;
    if(!file.open(QIODevice::ReadOnly)){
        return ;
    }
    QXmlStreamReader reader(&file);
    while(!reader.atEnd()){
        QXmlStreamReader::TokenType token =  reader.readNext();
        if(token == QXmlStreamReader::StartElement){
            QStringRef name = reader.name();

            if(name == "host"){
                QString url = reader.attributes().value("url").toString();
                Context::getInstance()->setHostName(url);
            }else if(name  == "Login"){
                bool autoLogin = reader.attributes().value("auto")=="true"?true:false;
                ConfigureManager::getInstance()->setAutoLgoin(autoLogin);
                readLoginInformation(reader);
            }

        }
    }
}

void ConfigureReaderAndWriter::save(){
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly)){
        QXmlStreamWriter writer(&file);
        writer.writeStartDocument();

        writer.writeStartElement("OnlineMonitor");

        writer.writeStartElement("host");
        writer.writeAttribute("url",Context::getInstance()->getHostName());
        writer.writeEndElement();

        writer.writeStartElement("Login");
        writer.writeAttribute("auto", ConfigureManager::getInstance()->getAutoLogin()?"true":"false");
        writer.writeTextElement("userName",Context::getInstance()->getUserInfo().getUserName());
        writer.writeTextElement("password",Context::getInstance()->getUserInfo().getPassword());
        writer.writeEndElement();

        writer.writeEndElement();

        writer.writeEndDocument();
    }
}
