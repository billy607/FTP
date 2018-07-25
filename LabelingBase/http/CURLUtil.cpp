#include "CURLUtil.h"
#include"task/OnlineTask.h"
#include"curl.h"
#include"LogUtil.h"

#include<string>

namespace labelingbase{


static size_t writefunc(void *ptr, size_t size, size_t nmemb, crul_data *s)
{
    s->appendData(ptr,size*nmemb);

    return size*nmemb;
}

static size_t writeDataToFile(void *ptr, size_t size, size_t nmemb, QFile *file){
    return file->write( (const char*)ptr , size *nmemb );
}

static void add_SSL_support(CURL *curl)
{
    curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "rootCA.pem");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0); // cancel timeout, for long task
}



CURLUtil::CURLUtil(QString address , QString postFields , QString method,curl_slist *headers)
{
    this->address = address;
    this->postFields = postFields;
    this->method = method;
    this->headers = headers;
}


CURLUtil::~CURLUtil(){

    if(curl){
        curl_easy_cleanup(curl);
    }
    if(headers)
        curl_slist_free_all(headers);
    //原代码中有，工作正常，但是逻辑上说不通的
    curl_global_cleanup();
}

// 这里需要传入的std::string都应该存在直到发送过程结束，因此要求是引用
CURL * initCURL(std::string &address ,std::string &postFiedls , std::string &method, CRULResult & result , curl_slist *headers = nullptr
                ,curl_httppost *formpost = nullptr , QFile * file = nullptr ){
    CURL * curl;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());

        if(headers)
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        if( ! method.empty() ){
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        }


        if(!postFiedls.empty())
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFiedls.c_str());


        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, result.getHeader().get());
        if(file){
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataToFile);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        }else{
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, result.getData().get());
        }




        if(formpost != nullptr)
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        //没有加入到init函数中的原因是，不知道调用顺序是否会影响下载过程
    }

    return curl;
}




CRULResult CURLUtil::sendRequest(){

    CURLcode res = CURLE_OK;
    CRULResult result;
    //转一下的原因是，QString：：tostdsing调用完成后回去析构，该返回结果，因此缓存一下
    std::string theddress = this->address.toStdString(),thisPostfields= this->postFields.toStdString(),
            theMethod = this->method.toStdString();
    curl = initCURL(theddress,thisPostfields,theMethod,result,headers);

    if (curl)
    {
        add_SSL_support(curl);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            result.setErrorMessage(QString("%1").arg(curl_easy_strerror(res)));
            result.setSuccess(false);
        }
        else
        {
            if (result.getStatus())
            {
                result.setErrorMessage(QString("%1,%2").arg(result.getHeader()->getBuffer()).arg(result.getData()->getBuffer()));
                result.setSuccess(false);
            }
            else
            {
                result.setSuccess(true);

            }
        }
    }
    else
    {
        result.setErrorMessage("curl_easy_init error");
        result.setSuccess(false);
    }

    return result;
}


int dumpDownloadProgress(void * param , double total ,double now ,double ,double ){
    DownloadMethod * method = (DownloadMethod * )param;
    (*method)(total,now);
    return 0;
}

int dumpUploadProgress(void * param , double total ,double now ,double ,double ){
    UploadMethod * method = (UploadMethod * )param;
    (*method)(total,now);
    return 0;
}

CRULResult CURLUtil::downloadFile(DownloadMethod method ){
    CURLcode res = CURLE_OK;
    CRULResult result;
    //转一下的原因是，QString：：tostdsing调用完成后回去析构，该返回结果，因此缓存一下
    std::string theddress = this->address.toStdString(),thisPostfields= this->postFields.toStdString(),
            theMethod = this->method.toStdString();
    QFile file(method.getFileName());
    if(!file.open(QIODevice::WriteOnly)){
        result.setSuccess(false);
        result.setErrorMessage(QString("can not open/create file %1").arg(method.getFileName()));
        return result;
    }
    curl = initCURL(theddress,thisPostfields,theMethod,result,headers,nullptr,&file);
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
        // Install the callback function
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dumpDownloadProgress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &method);

        add_SSL_support(curl);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            result.setErrorMessage(QString("%1").arg(curl_easy_strerror(res)));
            result.setSuccess(false);
        }
        else
        {
            if (result.getStatus())
            {
                result.setErrorMessage(QString("%1,%2").arg(result.getData()->getBuffer()).arg(result.getData()->getBuffer()));
                result.setSuccess(false);
            }
            else
            {
                result.setSuccess(true);
            }
        }
    }
    else
    {
        result.setErrorMessage("curl_easy_init error");
        result.setSuccess(false);
    }


    file.close();
    return result;
}

CRULResult CURLUtil::uploadForm(QString url,QMap<QString , QString > files , QMap<QString , QString > fileds){
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    QVector<std::string> fileKeys;
    QVector<std::string> fileValues;


    auto fileIte = files.begin();
    while(fileIte != files.end()){
        QString name = fileIte.key();
        QString value = fileIte.value();
        fileKeys.append(name.toStdString());
        fileValues.append(value.toStdString());
        fileIte++;
    }
    for(int index = 0 ; index < fileKeys.size() ; index ++ ){

        curl_formadd(&formpost,
                        &lastptr,
                        CURLFORM_COPYNAME, fileKeys[index].c_str(),
                        CURLFORM_FILE, fileValues[index].c_str(),
                        CURLFORM_END);
    }

    QVector<std::string> fieldKeys;
    QVector<std::string> fieldValues;
    auto fieldIte = fileds.begin();
    while(fieldIte != fileds.end()){
        QString name = fieldIte.key();
        QString value = fieldIte.value();
        fieldKeys.append(name.toStdString());
        fieldValues.append(value.toStdString());
        fieldIte++;
    }

    for(int index = 0 ; index < fieldKeys.size() ; index ++ ){
        curl_formadd(&formpost,
                        &lastptr,
                        CURLFORM_COPYNAME, fieldKeys[index].c_str(),
                        CURLFORM_COPYCONTENTS, fieldValues[index].c_str(),
                        CURLFORM_END);
    }

    CRULResult result ;
    std::string address = url.toStdString();
    std::string postfield = this->postFields.toStdString();
    std::string methodfield = this->method.toStdString();
    curl = initCURL(address,postfield,methodfield,result,nullptr,formpost);

    CURLcode res = CURLE_OK;

    if (curl)
    {

        add_SSL_support(curl);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            result.setErrorMessage(QString("%1").arg(curl_easy_strerror(res)));
            result.setSuccess(false);
        }
        else
        {
            if (result.getStatus())
            {
                result.setErrorMessage(QString("%1,%2").arg(result.getData()->getBuffer()).arg(result.getData()->getBuffer()));
                result.setSuccess(false);
            }
            else
            {
                result.setSuccess(true);

            }
        }
    }
    else
    {
        result.setErrorMessage("curl_easy_init error");
        result.setSuccess(false);
    }

    curl_formfree(formpost);

    return result;

}

}

