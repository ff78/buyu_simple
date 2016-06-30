//
//  ServerManager.cpp
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#include "ServerManager.h"
#include "logic/ClientLogic.h"
#include "ShareHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "server/AndroidHelper.h"
#endif

ServerManager::ServerManager()
{
    memset(m_Client2ServerFunc, 0, sizeof(m_Client2ServerFunc));
    
    m_Client2ServerFunc[c2s_recharge] = &ServerManager::processRecharge;
    m_Client2ServerFunc[c2s_cash_benefit_code] = &ServerManager::processCashBenefitCode;
    m_Client2ServerFunc[c2s_link_to_netgame] = &ServerManager::processLink2NetGame;
    m_Client2ServerFunc[c2s_share] = &ServerManager::processShare;
}

void ServerManager::processClientQuest(void *pMsg)
{
    if (NULL == pMsg)
        return;
    
    C2SProtocol	eProtocol	= *((C2SProtocol*)pMsg);
    if (eProtocol >= c2s_max)
        return;
    if (this->m_Client2ServerFunc[eProtocol] != NULL)
    {
        (this->*m_Client2ServerFunc[eProtocol])(pMsg);
    }
}

void ServerManager::processRecharge(void *pMsg)
{
    C2S_RECHARGE pMsgInfo =  *(C2S_RECHARGE*)(pMsg);
#if iOS_PAY
    iOSHelper::getInstance()->pay(pMsgInfo.rechargeId);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AndroidHelper::instance()->pay(pMsgInfo.rechargeId, pMsgInfo.price);
#else
    S2C_RECHARGE info;
    info.eProtocol = s2c_recharge;
    info.rechargeId = pMsgInfo.rechargeId;
    info.errNo = 0;
    ClientLogic::instance()->ProcessServerResponse(&info);
#endif
}

void ServerManager::processCashBenefitCode(void *pMsg)
{
    C2S_CASH_BENEFIT_CODE pMsgInfo = *(C2S_CASH_BENEFIT_CODE*)(pMsg);
    S2C_CASH_BENEFIT_CODE info;
    info.eProtocol = s2c_cash_benefit_code;
    info.errNo = 0;
    if (pMsgInfo.code == "203828") {
        info.result = 1;
    } else if (pMsgInfo.code == "200132") {
        info.result = 2;
    } else {
        info.result = -1;
        info.errNo = 1;
    }
    ClientLogic::instance()->ProcessServerResponse(&info);
}

void ServerManager::processLink2NetGame(void *pMsg)
{
    C2S_LINK_TO_NETGAME info = *static_cast<C2S_LINK_TO_NETGAME*>(pMsg);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    iOSHelper::getInstance()->openCommentUrl(info.type);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    AndoridHelper::instance()->openURL("www.buyubisai.com");
    std::string url("http://www.buyubisai.com");
    if (info.type == 2) {
        url = "http://yuyu.91bisai.com";
    }
    AndroidHelper::instance()->openURL(url);
#endif
}

void ServerManager::processShare(void *pMsg)
{
    C2S_SHARE shareInfo = *static_cast<C2S_SHARE*>(pMsg);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ShareHelper::getInstance()->share(shareInfo.channalId);
//#else
//    S2C_SHARE info;
//    info.eProtocol = s2c_share;
//    info.channalId = shareInfo.channalId;
//    info.errNo = 0;
//    ClientLogic::instance()->ProcessServerResponse(&info);
//#endif
}