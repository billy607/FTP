#ifndef CURLUTIL_H
#define CURLUTIL_H
#include<memory>
#include<QString>
#include<QTextStream>
#include"curl.h"
//#include"HttpUtil.h"
#include"labelingbase_global.h"
namespace labelingbase {




class DownloadMethod{
public:
    DownloadMethod(loadProgress progress, void * param , QString fileName):progress(progress),param(param),fileName(fileName){}
    int operator()(double totalDownload , double nowDownload){

        if(progress)
            progress(param,totalDownload,nowDownload);
        return 0;
    }

    QString getFileName(){
        return fileName;
    }

private:
    loadProgress progress;
    void* param;
    QString fileName;
};

class UploadMethod{
public:
    UploadMethod(loadProgress progress, void * param):progress(progress),param(param){}
    int operator()(double totalUpload , double nowUpload){
        if(progress)
            progress(param,totalUpload,nowUpload);
        return 0;
    }

private:
    loadProgress progress;
    void* param;
};


struct crul_data {

    crul_data(){
        len = 0 ;
        ptr =  (char*)malloc(len);
        if (ptr == nullptr) {
            exit(EXIT_FAILURE);
        }
        ptr[0] = '\0';
    }
    ~crul_data(){
        free(ptr) ;
    }
    void appendData(void * data, int size ){

        size_t new_len = len + size;
        ptr = (char*)realloc(ptr, new_len + 1);
        if (ptr == NULL) {
            fprintf(stderr, "realloc() failed\n");
            exit(EXIT_FAILURE);
        }
        memcpy(ptr + len, data, size);
        ptr[new_len] = '\0';
        len = new_len;
    }
    int getSize() const {
        return len;
    }
    char * getBuffer() const{
        return ptr;
    }

private:
    char* ptr;
    size_t len;
};


struct CRULResult{
    CRULResult(){
        sucess = false;
        DATA = std::shared_ptr<crul_data>(new crul_data);
        HEADER = std::shared_ptr<crul_data>(new crul_data);
    }

    std::shared_ptr<crul_data> getData(){
        return DATA;
    }
    std::shared_ptr<crul_data> getHeader(){
        return HEADER;
    }

    bool isSuccess() const{
        return sucess;
    }
    void setSuccess( bool success){
        this->sucess = success;
    }
    QString getErrorMessage()const{
        return errorMessage ;
    }
    void setErrorMessage(QString message){
        this->errorMessage = message;
    }
    bool isTookenExpire(){
        QString content(HEADER->getBuffer());
        QTextStream stream(&content);
        for (int i = 0; i < 3; i++)
        {
            QString token = stream.readLine();
            if(token.contains("401 Unauthorized")){
                return 1 ;
            }
        }
        return 0;
    }
    int getStatus(){
        QString content(HEADER->getBuffer());
        QTextStream stream(&content) ;
        QString token = stream.readLine();
        if(token.contains("200")|| token.contains("100 Continue")){
            return 0;
        }
        return 1;
    }

private:
    std::shared_ptr<crul_data> DATA;
    std::shared_ptr<crul_data> HEADER;
    bool sucess;
    QString errorMessage;
};
class CURLUtil
{
public:
    CURLUtil(QString address , QString postFields , QString method = "",curl_slist *headers = nullptr);
    ~CURLUtil();

    CRULResult sendRequest( );

    CRULResult downloadFile(DownloadMethod method );

    CRULResult uploadForm(QString url ,QMap<QString , QString > files , QMap<QString , QString > fileds );

private:
    void *curl ;




    QString address;
    QString postFields;
    QString method;
    curl_slist *headers;

};

}



#endif // CURLUTIL_H
