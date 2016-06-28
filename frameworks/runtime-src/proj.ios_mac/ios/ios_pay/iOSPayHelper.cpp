//
//  iOSHelper.cpp
//  diamond
//
//  Created by ff on 15/9/21.
//
//

#include "iOSHelper.h"
#include "IOSIAP_Bridge.h"
#include "JZVerManager.h"
//#include "plazz/data/GlobalUserInfo.h"
//#include "event/MTNotification.h"
//#include "PublicSendLuaData.h"
#include "CSProtocol.h"
#include "logic/ClientLogic.h"


std::string iOSHelper::uuidStr = "";
bool iOSHelper::bOnline = false;
//bool iOSHelper::bQQInstalled = false;


iOSHelper *iOSHelper::instance = nullptr;

iOSHelper *iOSHelper::getInstance()
{
    if (instance == nullptr) {
        instance = new iOSHelper;
    }
    
    return instance;
}


//bool iOSHelper::isOnLine()
//{
//    return bOnline;
//}
//bool iOSHelper::isQQInstalled()
//{
//    return bQQInstalled;
//}
//
//std::string iOSHelper::getDeviceId()
//{
//    return uuidStr;
//    //    return "MDIEH7788991450";
//}
#if iOS_PAY
void iOSHelper::pay(int idx)
{

    auto iap = new IOSiAP_Bridge;
    iap->requestProducts(idx-1);
}

void iOSHelper::paySuccess(int idx)
{
//    cocos2d::ValueMap params;
//    params["data"] = result;
//    PublicSendLuaData::getInstance()->callLuaFuncPar("out/app/layers/PaymentLayer.luac", "onIAPPayResult", params);
//    if (result.status == 0) {
//        CGlobalUserInfo   * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
//        pGlobalUserInfo->GetUserInsureInfo()->lUserScore += result.money;
//        pGlobalUserInfo->GetGlobalUserData()->iTelephoneBill += result.phoneFee;
//        pGlobalUserInfo->GetGlobalUserData()->iTreasureMapNum += result.Treature;
//        pGlobalUserInfo->GetGlobalUserData()->iVip = result.vip;
//
//        char str[255] = {0};
//
//        sprintf(str, "%lld", pGlobalUserInfo->GetUserInsureInfo()->lUserScore);
//        G_NOTIFY(UPDATE_SCORE_NOTIFY, MTData::create(0, 0, 0, str));
//
//        sprintf(str, "%.1f", pGlobalUserInfo->GetGlobalUserData()->iTelephoneBill);
//        G_NOTIFY(UPDATE_PHONE_FEE_NOTIFY, MTData::create(0, 0, 0, str));
//
//        G_NOTIFY(UPDATE_VIP_NOTIFY, MTData::create(pGlobalUserInfo->GetGlobalUserData()->iVip));
//        
//        G_NOTIFY(ON_RECHARGE_UPDATE_RMBCOST, MTData::create(result.RechargeAll));
//        
//        if (result.Treature) {
//            G_NOTIFY(ON_VIP_GIFT_CHARGE_COMPLETE, 0);
//        }
//        
//        if (result.phoneFee) {
//            G_NOTIFY(ON_FIRST_RECHARGE_COMPLETE, 0);
//        }
        
//        G_NOTIFY(UPDATE_SCORE_IN_GAME, MTData::create(result.money));
//    }
//    else {
//        //notify hide shield
//        G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
//    }
    S2C_RECHARGE info;
    info.eProtocol = s2c_recharge;
    info.rechargeId = idx+1;
    info.errNo = 0;
    ClientLogic::instance()->ProcessServerResponse(&info);
}
#endif

void iOSHelper::openCommentUrl(int type)
{
    if (type == 1) {
        GameIOS::getInstance()->openCommentUrl();
    }else {
        GameIOS::getInstance()->openCommentUrl2();
    }
    

}
//
//void iOSHelper::onGameExit()
//{
//    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
//#else
//    cocos2d::CCDirector::getInstance()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//#endif
//}
