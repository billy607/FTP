#ifndef CRYPTO_H
#define CRYPTO_H



#include<QByteArray>
#include<memory>
#include"labelingbase_global.h"

namespace labelingbase {


class CryptoImpl;
class LABELINGBASESHARED_EXPORT Crypto
{

public:

    class CrypedData{
    public:
        CrypedData(){
            length = 0 ;
        }

        CrypedData(char * crypedData, int length){
            this->data = crypedData;
            this->length = length;
        }
        int getLength() const{
            return length;
        }
        char*  getData(){
            return data;
        }

    private:
        int length;
        char* data;
    };

    static Crypto * getInstance();


    ~Crypto();

    CrypedData encryptData( const QByteArray& data );
    CrypedData decryptData( const QByteArray &data );

private:
    static Crypto * pInstance;
private:
    Crypto();
    CryptoImpl * cryptoImpl;
};

}


#endif // CRYPTO_H
