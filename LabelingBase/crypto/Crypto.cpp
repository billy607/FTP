#include "Crypto.h"
#include"CryptoImpl.h"
#include"LogUtil.h"

namespace labelingbase {


Crypto * Crypto::pInstance = nullptr;
Crypto * Crypto::getInstance(){
    if(!pInstance){
        pInstance = new Crypto();
    }

    return pInstance;
}

Crypto::Crypto()
{
    cryptoImpl = new CryptoImpl();

}
Crypto::~Crypto(){
    delete cryptoImpl;
}

Crypto::CrypedData Crypto::encryptData( const QByteArray &toEncrypt){

    const char * srcData = toEncrypt.constData();

    char * encryptedData ;
    int length = cryptoImpl->aesEncrypt((const unsigned char*)srcData, toEncrypt.length(), (unsigned char **)&encryptedData);

    if( length < 0 ){
        LogUtil::critical("encryptData failed");
        return Crypto::CrypedData();
    }

    return Crypto::CrypedData(encryptedData,length);


}

Crypto::CrypedData Crypto::decryptData(const QByteArray & toDecrypt){
    const char * srcData = toDecrypt.constData();

    char * decryptedData ;
    int length = cryptoImpl->aesDecrypt((const unsigned char*)srcData,toDecrypt.length(),(unsigned char **) &decryptedData);

    if( length < 0 ){
        LogUtil::critical("unencryptData failed");
        return Crypto::CrypedData();
    }

    return Crypto::CrypedData(decryptedData,length);

}

}
