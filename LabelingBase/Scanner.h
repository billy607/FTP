#ifndef SCANNER_H
#define SCANNER_H

#include<QStringList>
#include"labelingbase_global.h"
namespace labelingbase {
/**
 * @brief The ImageScanner class
 * 扫描目录下所有的图片
 */
class LABELINGBASESHARED_EXPORT ImageScanner
{
public:
    ImageScanner(QString path){
        this->path = path;
    }

    QStringList getImages();

private:
    QString path;
};

/**
 * @brief The ImageDirScanner class
 * 扫描具有多个图片的文件夹，
 * 如果一个文件夹下有图片和文件夹（这个下面还有图片），两个文件夹都会返回，也许应该搞一个imagedirdirscanner，太特殊，不考虑
 */
class LABELINGBASESHARED_EXPORT ImageDirScanner{
public:
    ImageDirScanner(QString path){
        this->path = path;
    }

    QStringList getDirs();

private:
    QString path;

};

QStringList LABELINGBASESHARED_EXPORT getImageSuffix();

bool LABELINGBASESHARED_EXPORT isImageFile(QString fileName);


}


#endif // SCANNER_H
