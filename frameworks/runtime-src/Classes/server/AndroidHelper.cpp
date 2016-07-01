//
//  AndroidHelper.cpp
//  buyu_simple
//
//  Created by ff on 16/6/22.
//
//

#include "AndroidHelper.h"
#include "CSProtocol.h"
#include "EventDef.h"
#include "logic/ClientLogic.h"

USING_NS_CC;

extern "C"
{    
    void Java_org_cocos2dx_lua_AndroidHelper_OnPayProcessResult(JNIEnv *env, jobject thiz, int orderId, int errNo)
    {
//        log("back to c++ result!");
        S2C_RECHARGE info;
        info.eProtocol = s2c_recharge;
        info.rechargeId = orderId;
        info.errNo = errNo;
//        log("pass 2 sever");
//        ClientLogic::instance()->ProcessServerResponse(&info);
        EventCustom event(JNI_RECHARGE_OK);
        event.setUserData(&info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

/// 得到设备的唯一标示
std::string AndroidHelper::getDeviceID()
{
    JniMethodInfo minfo;
    jobject jobj;
    
    if(JniHelper::getStaticMethodInfo(minfo,JNI_CLASS_NAME,"getDeviceID","()Ljava/lang/String;"))
    {
        jstring result = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        return JniHelper::jstring2string(result);
    }
    return "";
}

void AndroidHelper::openURL(std::string url)
{
    JniMethodInfo minfo;
    jobject jobj;
    
    if(JniHelper::getStaticMethodInfo(minfo,JNI_CLASS_NAME,"openURL","(Ljava/lang/String;)V"))
    {
        jstring url_jstr = minfo.env->NewStringUTF(url.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, url_jstr);
        minfo.env->DeleteLocalRef(url_jstr);
    }
    
}

/// 手机平台显示消息
void AndroidHelper::Toast(const std::string& message)
{
    MessageBox(message.c_str(), "");
}

void AndroidHelper::pay(int index, float price)
{
    JniMethodInfo minfo;
    jobject jobj;
    
    if(JniHelper::getStaticMethodInfo(minfo,JNI_CLASS_NAME,"pay","(IF)V"))
    {
        jint jorderId = index;
        jfloat jprice = price;
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jorderId, jprice);
    }
    
}


