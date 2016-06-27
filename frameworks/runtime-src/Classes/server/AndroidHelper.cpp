//
//  AndroidHelper.cpp
//  buyu_simple
//
//  Created by ff on 16/6/22.
//
//

#include "AndroidHelper.h"
#include "CSProtocol.h"
#include "logic/ClientLogic.h"

USING_NS_CC;

extern "C"
{
    bool getJniMethod(jobject& jobj, JniMethodInfo &minfo, const char* sFunction, const char* sSignature)
    {
        if (!JniHelper::getStaticMethodInfo(minfo,JNI_CLASS_NAME,"getInstance","()Ljava/lang/Object;"))
            return false;
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        return JniHelper::getMethodInfo(minfo, JNI_CLASS_NAME, sFunction, sSignature);
    }


}
/// 得到设备的唯一标示
std::string AndroidHelper::getDeviceID()
{
    JniMethodInfo minfo;
    jobject jobj;
    
    if(getJniMethod(jobj,minfo,"getDeviceID","()Ljava/lang/String;"))
    {
        jstring result = (jstring)minfo.env->CallObjectMethod(jobj,minfo.methodID);
        m_deviceID = minfo.env->GetStringUTFChars(result,0);
//        JZCore::getInstance()->setDeviceID(str);
        minfo.env->DeleteLocalRef(result);
        return m_deviceID;
    }
    return "";
}

void AndroidHelper::openURL(std::string url)
{
    JniMethodInfo minfo;
    jobject jobj;
    
    if(getJniMethod(jobj,minfo,"openURL","(Ljava/lang/String;)V"))
    {
        jstring url_jstr = minfo.env->NewStringUTF(appuserid.c_str());
        minfo.env->CallVoidMethod(jobj, minfo.methodID, url_jstr);
//        m_deviceID = minfo.env->GetStringUTFChars(result,0);
//        JZCore::getInstance()->setDeviceID(str);
        minfo.env->DeleteLocalRef(url_jstr);
    }
    
}

/// 手机平台显示消息
void AndroidHelper::Toast(const std::string& message)
{
    MessageBox(message.c_str(), "");
}

void AndroidHelper::pay(int index)
{
    JniMethodInfo minfo;
    jobject jobj;
    
    if(getJniMethod(jobj,minfo,"pay","(I)V;"))
    {
        minfo.env->CallVoidMethod(jobj, minfo.methodID, index);
    }
    
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lua_AndroidHelper_OnPayProcessFailed()
{
    S2C_RECHARGE info;
    info.eProtocol = s2c_recharge;
    info.rechargeId = pMsgInfo.rechargeId;
    info.errNo = 1;
    ClientLogic::instance()->ProcessServerResponse(&info);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_lua_AndroidHelper_OnPayProcessSuccess()
{
    S2C_RECHARGE info;
    info.eProtocol = s2c_recharge;
    info.rechargeId = pMsgInfo.rechargeId;
    info.errNo = 0;
    ClientLogic::instance()->ProcessServerResponse(&info);
}

