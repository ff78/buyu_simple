//
//  ShareHelper.cpp
//  buyu_simple
//
//  Created by ff on 16/6/1.
//
//

#include "ShareHelper.h"

#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "CSProtocol.h"
#include "logic/ClientLogic.h"

#include "utils/StringData.h"

#define SHARE_IMG 		"buyu_icon.png"
//    #define SHARE_IMG 		"icon-120.png"
USING_NS_UM_SOCIAL;

const char *umKey = "576cfd72e0f55af3600013d4";
const char *weixinid = "wxecb148df5ec0c2c2";
const char *weixinkey = "beacf4d7bfc1db200139a49e106a9da7";

USING_NS_CC;

enum ShareButtonTag
{
    TAG_SHARE_NULL = 0,
    TAG_SHARE_TO_WEIXIN,
    TAG_SHARE_TO_CICLE,
    TAG_SHARE_TO_QQZONE,
    TAG_BTN_CLOSE
};

ShareHelper* ShareHelper::instance = nullptr;

ShareHelper *ShareHelper::getInstance()
{
    if (instance == nullptr) {
        instance = new ShareHelper;
    }
    
    return instance;
}

/*
 * 分享回调, 该回调不是某个类的成员函数， 而是一个普通的函数, 具体使用参考HelloWorldScene的例子
 * @param platform 要分享到的目标平台
 * @param stCode 返回码, 200代表分享成功, 100代表开始分享
 * @param errorMsg 分享失败时的错误信息,android平台没有错误信息
 */
void shareCallback(int platform, int stCode, std::string& errorMsg)
{
    S2C_SHARE info;
    info.eProtocol = s2c_share;
    if ( stCode == 100 )
    {
        log("#### HelloWorld 开始分享a");
        return;
    }
    else if ( stCode == 200 )
    {
//        G_NOTIFY(ON_SHARE_RETURN, MTData::create());
        log("#### HelloWorld 分享成功");
        info.errNo = 0;
    }
    else if (stCode == 40002)
    {
        log("#### HelloWorld 未安装QQ或QZone客户端");
//        TIP_BOX_TOAST_0(SSTRING("qq_not_installed"));
        info.errNo = 2;
    }
    else
    {
        log("#### HelloWorld 分享出错");
        info.errNo = 1;
    }
    info.channalId = ShareHelper::getInstance()->getChannalId();
    log("platform num is : %d.", platform);
    ClientLogic::instance()->ProcessServerResponse(&info);
}

//返回码生产成功
void ShareHelper::share(int index)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto pSdk = CCUMSocialSDK::create(umKey);
    //链接路径
    pSdk->setTargetUrl("yuyu.91bisai.com");
    
    vector<int>* platforms = new vector<int>();
#endif
    channalId = index;
    switch (index)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            //微信
        case TAG_SHARE_TO_WEIXIN:
        {
            platforms->push_back(WEIXIN);
            pSdk->setPlatforms(platforms);
            pSdk->setWeiXinAppInfo(weixinid, weixinkey);
            pSdk->setPlatformShareContent(WEIXIN, SSTRING("share_content").c_str(), SHARE_IMG, "单机捕鱼", "yuyu.91bisai.com");
            pSdk->directShare(WEIXIN, SSTRING("share_content").c_str(), SHARE_IMG, share_selector(shareCallback));
        }
            break;
            //微信朋友圈
        case TAG_SHARE_TO_CICLE:
        {
            platforms->push_back(WEIXIN_CIRCLE);
            pSdk->setPlatforms(platforms);
            pSdk->setWeiXinAppInfo(weixinid, weixinkey);
            
            pSdk->setPlatformShareContent(WEIXIN_CIRCLE, SSTRING("share_content").c_str(), SHARE_IMG, "", "yuyu.91bisai.com");
            pSdk->directShare(WEIXIN_CIRCLE, SSTRING("share_content").c_str(), SHARE_IMG, share_selector(shareCallback));
        }
            
            break;
            //QQ空间
        case TAG_SHARE_TO_QQZONE:
        {
            platforms->push_back(QQ);
            platforms->push_back(QZONE);
            pSdk->setPlatforms(platforms);
            //QQ未配后台，APPKEY和APPSECURITY要换
            pSdk->setQQAppIdAndAppKey("1104815387", "broFIouekdJKZWpZ");
            pSdk->directShare(QZONE, "", SHARE_IMG, share_selector(shareCallback));
            
        }
            break;
#endif
            
        default:
            break;
    }
}