//
//  AndroidHelper.h
//  buyu_simple
//
//  Created by ff on 16/6/22.
//
//
#ifndef AndroidHelper_h
#define AndroidHelper_h

#include "GameUtils.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <Jni.h>

#define JNI_CLASS_NAME "org/cocos2dx/lua/AndroidHelper"

class AndroidHelper : public Singleton<AndroidHelper>
{
public:
    /// 得到设备的唯一标示
    std::string getDeviceID();
    
    /// 手机平台显示消息
    void Toast(const std::string& message);
    
    void openURL(std::string url);
    
    void pay(int index);
private:
    std::string m_deviceID;
};

#endif /* AndroidHelper_h */
