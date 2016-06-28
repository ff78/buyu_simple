//
//  CSProtocol.h
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#ifndef CSProtocol_h
#define CSProtocol_h

enum C2SProtocol
{
    c2s_recharge,                   //  充值请求
    c2s_cash_benefit_code,          //  救济金兑奖
    c2s_link_to_netgame,            //  跳转网页
    c2s_share,                      //  分享
    c2s_max
};

enum S2CProtocol
{
    s2c_recharge,                   //  充值结果
    s2c_cash_benefit_code,          //  兑奖结果
    s2c_share,                      //  分享结果
    s2c_max
};

typedef struct __C2S_RECHARGE
{
    C2SProtocol             eProtocol;
    int                     rechargeId;
}C2S_RECHARGE;

typedef struct __S2C_RECHARGE
{
    S2CProtocol             eProtocol;
    int                     rechargeId;
    int                     errNo;
}S2C_RECHARGE;

typedef struct __C2S_CASH_BENEFIT_CODE
{
    C2SProtocol             eProtocol;
    std::string             code;
}C2S_CASH_BENEFIT_CODE;

typedef struct __S2C_CASH_BENEFIT_CODE {
    S2CProtocol             eProtocol;
    int                     result;
    int                     errNo;
}S2C_CASH_BENEFIT_CODE;

typedef struct __C2S_LINK_TO_NETGAME
{
    C2SProtocol             eProtocol;
    int                     type;
}C2S_LINK_TO_NETGAME;

typedef struct __C2S_SHARE
{
    C2SProtocol             eProtocol;
    int                     channalId;
}C2S_SHARE;

typedef struct __S2C_SHARE
{
    S2CProtocol             eProtocol;
    int                     channalId;
    int                     errNo;
}S2C_SHARE;
#endif /* CSProtocol_h */
