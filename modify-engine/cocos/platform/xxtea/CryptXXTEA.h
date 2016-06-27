//
//  CryptXXTEA.hpp
//  encodeResource
//
//  Created by ff on 16/6/3.
//
//

#ifndef CryptXXTEA_hpp
#define CryptXXTEA_hpp

#include "cocos2d.h"


class CryptXXTEA
{
public:
    CryptXXTEA();
    ~CryptXXTEA();
    static CryptXXTEA* getInstance();
    void setXXTEAKeyAndSign(const char *key, int keyLen, const char *sign, int signLen);
    size_t decryptData(unsigned char* chunk, size_t chunkSize);
    bool decryptData(cocos2d::Data &data);
private:
    void cleanupXXTEAKeyAndSign();
    
    static CryptXXTEA* instance;
private:
    CC_SYNTHESIZE_READONLY(bool, _xxteaEnabled, XxteaEnabled);
    CC_SYNTHESIZE_READONLY(char*, _xxteaKey, XxteaKey);
    CC_SYNTHESIZE_READONLY(int, _xxteaKeyLen, XxteaKeyLen);
    CC_SYNTHESIZE_READONLY(char*, _xxteaSign, XxteaSign);
    CC_SYNTHESIZE_READONLY(int, _xxteaSignLen, XxteaSignLen);
};


#endif /* CryptXXTEA_hpp */
