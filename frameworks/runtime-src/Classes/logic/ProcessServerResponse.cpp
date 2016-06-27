//
//  ProcessServerResponse.cpp
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#include "ClientLogic.h"
#include "GameCore.h"

void ClientLogic::ProcessServerResponse(void *pMsg)
{
    if (NULL == pMsg)
        return;
    
    S2CProtocol	eProtocol	= *((S2CProtocol*)pMsg);
    if (eProtocol >= s2c_max)
        return;
    //分发到各个协议的处理函数
    if (this->m_Server2ClientFunc[eProtocol] != NULL)
    {
        (this->*m_Server2ClientFunc[eProtocol])(pMsg);
    }
}

void ClientLogic::ResRecharge(void *pMsg)
{
    S2C_RECHARGE *pMsgInfo = (S2C_RECHARGE*)pMsg;
    GameCore::instance()->responseRecharge(*pMsgInfo);
}

void ClientLogic::ResCashBenefitCode(void *pMsg)
{
    S2C_CASH_BENEFIT_CODE *pMsgInfo = (S2C_CASH_BENEFIT_CODE*)pMsg;
    GameCore::instance()->responseCashBenefitCode(*pMsgInfo);
}

void ClientLogic::ResShare(void *pMsg)
{
    S2C_SHARE *pMsgInfo = (S2C_SHARE*)pMsg;
    GameCore::instance()->responseShare(*pMsgInfo);
}
