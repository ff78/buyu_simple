//
//  iOSHelper.h
//  diamond
//
//  Created by ff on 15/9/21.
//
//

#ifndef __diamond__iOSHelper__
#define __diamond__iOSHelper__

#include "cocos2d.h"
//#include "MakeJson.h"

class iOSHelper
{
public:
#if iOS_PAY
    void pay(int idx);
    
    void paySuccess(int idx);
    
#endif
    
    void openCommentUrl(int type);
//
//    static void onGameExit();
//    
//    static bool isOnLine();
//    static std::string getDeviceId();
    static iOSHelper *getInstance();
//
//    static bool isQQInstalled();
    static bool bOnline;
    static std::string uuidStr;
    static std::string todayStr;
//    static bool bQQInstalled;
private:
    static iOSHelper *instance;
};

#endif /* defined(__diamond__iOSHelper__) */
