#include "ZipUtil.h"
#include<QFileInfo>
#include<QSet>
#include<QCryptographicHash>
#include"JlCompress.h"
#include"Scanner.h"
#include"crypto/Crypto.h"
namespace labelingbase {

ZipUtil::ZipUtil()
{

}

ZipUtil::~ZipUtil()
{

}

bool  ZipUtil::zip(QString src_file, QString dest_file){
    QFileInfo info(src_file);

    if( ! info.exists() ){
        return false;
    }

    if( info.isDir() ){
        return JlCompress::compressDir(dest_file,src_file,true,QDir::Hidden) ;
    }else if(info.isFile()){
        return JlCompress::compressFile(dest_file, src_file);
    }

    return false;

}

bool ZipUtil::unzip(QString src_file, QString dir){

    QStringList files =  JlCompress::extractDir(src_file,dir);

    return !files.isEmpty();

}

bool ZipUtil::unzipAndCrypto(QString src_file, QString dest_path,loadProgress progress, void * param ){
    QSet<QString> md5Names;
    QuaZip zip(src_file);
    zip.open(QuaZip::mdUnzip);

    int allCount = zip.getFileNameList().size();
    int current = 0 ;
    QDir dir(".");
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile()) {
        QuaZipFile file(&zip);
        if(file.open(QIODevice::ReadOnly)){
            QString fileName = file.getActualFileName();

            if(fileName.endsWith("/") || fileName.endsWith("\\")){//文件夹
                dir.mkpath(dest_path + "/" + fileName);
            }else{
                int lastIndex = qMax(fileName.lastIndexOf("/"), fileName.lastIndexOf("\\"));

                QString path , name;
                if(lastIndex > 0 ){
                    path = dest_path + "/" + fileName.left(lastIndex);
                    name = fileName.mid(lastIndex+1);
                }else{
                    path = dest_path;
                    name = fileName;
                }
                dir.mkpath(path);

                QByteArray array = file.readAll();
                if(!array.isEmpty()){

                    QByteArray md5Data= QCryptographicHash::hash(
                                array ,QCryptographicHash::Md5);

                    QString md5Name = md5Data.toHex() ;
                    if(! md5Names.contains(md5Name)){
                        if(isImageFile(path + "/" + name)){
                            Crypto::CrypedData data = Crypto::getInstance()->encryptData(array);

                            QFile destFile(path + "/" + name);
                            destFile.open(QIODevice::WriteOnly);
                            destFile.write(data.getData(),data.getLength());
                            destFile.close();

                        }else{
                            QFile destFile(path + "/" + name);
                            destFile.open(QIODevice::WriteOnly);
                            destFile.write(array);
                            destFile.close();
                        }
                    }

                }


            }

            current ++;

            if(progress){
                progress(param,allCount,current);
            }
            file.close(); // do not forget to close!
        }


    }

    return true;


}


}

