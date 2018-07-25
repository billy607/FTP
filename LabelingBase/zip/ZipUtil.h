#ifndef ZIPUTIL_H
#define ZIPUTIL_H
#include<QString>
#include"labelingbase_global.h"

namespace labelingbase {

class LABELINGBASESHARED_EXPORT ZipUtil
{
public:
    ZipUtil();
    ~ZipUtil();

    static bool zip(QString src_file, QString  dest_file);

    static bool unzip(QString src_file, QString dir);

    static bool unzipAndCrypto(QString src_file, QString dir,
                               loadProgress = nullptr, void * param = nullptr);
};


}




#endif // ZIPUTIL_H
