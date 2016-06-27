//
//  CryptXXTEA.cpp
//  encodeResource
//
//  Created by ff on 16/6/3.
//
//

#include "CryptXXTEA.h"
#include "xxtea.h"

CryptXXTEA* CryptXXTEA::instance = nullptr;

CryptXXTEA::CryptXXTEA():
_xxteaEnabled(false),
_xxteaKey(nullptr),
_xxteaKeyLen(0),
_xxteaSign(nullptr),
_xxteaSignLen(0)
{
    
}

CryptXXTEA::~CryptXXTEA()
{
    
}

CryptXXTEA* CryptXXTEA::getInstance()
{
    if (instance == nullptr) {
        instance = new CryptXXTEA;
    }
    return instance;
}

void CryptXXTEA::setXXTEAKeyAndSign(const char *key, int keyLen, const char *sign, int signLen)
{
    cleanupXXTEAKeyAndSign();
    
    if (key && keyLen && sign && signLen)
    {
        _xxteaKey = (char*)malloc(keyLen);
        memcpy(_xxteaKey, key, keyLen);
        _xxteaKeyLen = keyLen;
        
        _xxteaSign = (char*)malloc(signLen);
        memcpy(_xxteaSign, sign, signLen);
        _xxteaSignLen = signLen;
        
        _xxteaEnabled = true;
    }
    else
    {
        _xxteaEnabled = false;
    }
}

void CryptXXTEA::cleanupXXTEAKeyAndSign()
{
    if (_xxteaKey)
    {
//        free(_xxteaKey);
        _xxteaKey = nullptr;
        _xxteaKeyLen = 0;
    }
    if (_xxteaSign)
    {
//        free(_xxteaSign);
        _xxteaSign = nullptr;
        _xxteaSignLen = 0;
    }
}

size_t CryptXXTEA::decryptData(unsigned char* chunk, size_t chunkSize)
{
//    if (data.isNull())
//        return false;
//    const char* chunk = (const char*)data.getBytes();
//    int chunkSize = (int)data.getSize();
    if (chunkSize == 0) {
        return 0;
    }
    
//    cocos2d::log("<<<<<<<<<<<<<<<decrypt>>>>>>>>>>>>>>>>");
//    cocos2d::log("origin: %s",chunk);
//    cocos2d::log("xxtea Sign: %s", _xxteaSign);
//    cocos2d::log("enable: %d", _xxteaEnabled);
    if (_xxteaEnabled && strncmp((const char*)chunk, _xxteaSign, _xxteaSignLen) == 0)
    {
        // decrypt XXTEA
        xxtea_long len = 0;
        unsigned char *trans = xxtea_decrypt((unsigned char*)chunk + _xxteaSignLen,
                                             (xxtea_long)chunkSize - _xxteaSignLen,
                                             (unsigned char*)_xxteaKey,
                                             (xxtea_long)_xxteaKeyLen,
                                             &len);
//        data.fastSet(trans, chunkSize - _xxteaSignLen);
        memcpy(chunk, trans, chunkSize - _xxteaSignLen);
        
        delete trans;
        trans = nullptr;
        return chunkSize - _xxteaSignLen;
    }
    return chunkSize;
}

bool CryptXXTEA::decryptData(cocos2d::Data &data)
{
        if (data.isNull())
            return false;
        const char* chunk = (const char*)data.getBytes();
        int chunkSize = (int)data.getSize();

    
    if (_xxteaEnabled && strncmp((const char*)chunk, _xxteaSign, _xxteaSignLen) == 0)
    {
        // decrypt XXTEA
        xxtea_long len = 0;
        unsigned char *trans = xxtea_decrypt((unsigned char*)chunk + _xxteaSignLen,
                                             (xxtea_long)chunkSize - _xxteaSignLen,
                                             (unsigned char*)_xxteaKey,
                                             (xxtea_long)_xxteaKeyLen,
                                             &len);
        data.fastSet(trans, chunkSize - _xxteaSignLen);
//        memcpy(chunk, trans, chunkSize - _xxteaSignLen);
        return true;
    }
    return false;
}
