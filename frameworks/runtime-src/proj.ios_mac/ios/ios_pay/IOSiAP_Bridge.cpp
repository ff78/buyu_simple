//
//  IOSiAP_Bridge.cpp
//  LongChengDaRen
//
//  Created by 白白 on 14-11-11.
//
//

#include "IOSiAP_Bridge.h"
#include "cocos2d.h"
//#include "MakeJson.h"
//#include "MTNotification.h"
#include "iOSHelper.h"

USING_NS_CC;
using namespace std;

//static const int price[]={1,6,30,68,168,328,648};
static const std::string idenStr[]={
//#if COCOS2D_DEBUG
//    "com.chenpenghui.buyu.gold3w1",
//    "com.chenpenghui.buyu.gold6w1",
//    "com.chenpenghui.buyu.gold30w1",
//    "com.chenpenghui.buyu.gold72w1",
//    "com.chenpenghui.buyu.gold185w1",
//    "com.chenpenghui.buyu.gold360w1",
//    "com.chenpenghui.buyu.gold750w1",
//    "com.chenpenghui.buyu.vipgift30"
//#else
    "com.jzgame.standalonebuyu.gold6w",
    "com.jzgame.standalonebuyu.gold30w",
    "com.jzgame.standalonebuyu.gold72w",
    "com.jzgame.standalonebuyu.gold185w",
    "com.jzgame.standalonebuyu.gold360w",
    "com.jzgame.standalonebuyu.gold720w",
    "com.jzgame.standalonebuyu.diamond60",
    "com.jzgame.standalonebuyu.diamond300",
    "com.jzgame.standalonebuyu.diamond680",
    "com.jzgame.standalonebuyu.diamond1680",
    "com.jzgame.standalonebuyu.diamond3280",
    "com.jzgame.standalonebuyu.diamond6480"
//#endif
};

IOSiAP_Bridge::IOSiAP_Bridge()
{
    iap = IOSiAP::getInstance();
    iap->delegate = this;
}

IOSiAP_Bridge::~IOSiAP_Bridge()
{
    delete iap;
}

void IOSiAP_Bridge:: requestProducts(int id)
{
    Director::getInstance()->getRunningScene()->removeChildByTag(10000);
    //加屏蔽层，屏蔽操作
//    if (maskLayer == nullptr) {
        auto maskLayer = LayerColor::create(Color4B(0,0,0,88));
        auto s = Director::getInstance()->getWinSize();
        auto load = Sprite::create();
        auto animation = Animation::create();
        for (int i=1; i<8; i++) {
            std::string file = StringUtils::format("iOS_Pay_Res/loading_0%d.png", i);
            animation->addSpriteFrameWithFile(file);
        }
        animation->setDelayPerUnit(0.5f/7.0f);
        animation->setRestoreOriginalFrame(true);
        auto action = Animate::create(animation);
        load->setPosition(Vec2(s.width/2, s.height/2));
        load->runAction(RepeatForever::create(action));
        maskLayer->addChild(load);
        
        auto callback = [](Touch * ,Event *)
        {
            return true;
        };

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = callback;
        listener->setSwallowTouches(true);
        maskLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,maskLayer);
        maskLayer->setVisible(true);
        Director::getInstance()->getRunningScene()->addChild(maskLayer, 10000, 10000);
//    }
    
    productID = id;
    iap->productId = productID;
//    iap->userId = userid;
//    iap->orderId = orderid;
    //把需要付费的道具的所有product id都放到容器里面传进去
    vector<std::string> product;
    for (auto str : idenStr) {
        product.push_back(str);
    }
	//传递到Apple服务器，等待Apple对productID的验证
    iap->requestProducts(product);
}


void IOSiAP_Bridge::onRequestProductsFinish(void)
{
    std::string identifier = "";
    
    if (productID >= idenStr->length()) {
        onRequestProductsError(0);
        return;
    }
    identifier = idenStr[productID];

    //必须在onRequestProductsFinish后才能去请求iAP产品数据。
    IOSProduct *product = iap->iOSProductByIdentifier(identifier);
    if (product == nullptr) {
        onRequestProductsError(0);
        return;
    }
    
    //发起付费前，先向游戏服务器请求订单号
//    if (!iap->requestOrderId(product)) {
//        MessageBox("订单认证失败，请稍候重试", "认证结果");
//        Director::getInstance()->getRunningScene()->removeChildByTag(10000);
//        G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
//        return;
//    }
    
    // 然后可以发起付款请求。,第一个参数是由iOSProductByIdentifier获取的IOSProduct实例，第二个参数是购买数量
    iap->paymentWithProduct(product, 1);
}

void IOSiAP_Bridge::onRequestProductsError(int code)
{
    //这里requestProducts出错了，不能进行后面的所有操作。
    log("付款失败");
    MessageBox("付款失败,请稍候重试", "购买结果");
    Director::getInstance()->getRunningScene()->removeChildByTag(10000);
//    G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
}

void IOSiAP_Bridge::onValidSuccess(int productId)
{
    iOSHelper::getInstance()->paySuccess(productId);
    Director::getInstance()->getRunningScene()->removeChildByTag(10000);
}
void IOSiAP_Bridge::onValidFail()
{
    MessageBox("认证失败，请稍候重试", "认证结果");
    Director::getInstance()->getRunningScene()->removeChildByTag(10000);
//    G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
}

void IOSiAP_Bridge::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity)
{
    
    if(event == IOSIAP_PAYMENT_FAILED){
        log("购买失败");
        MessageBox("购买失败,请稍候重试", "购买结果");
        Director::getInstance()->getRunningScene()->removeChildByTag(10000);
//        G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
    }else if(event == IOSIAP_PAYMENT_CANCEL){
        log("用户取消交易");
        MessageBox("购买失败,用户取消交易", "购买结果");
        Director::getInstance()->getRunningScene()->removeChildByTag(10000);
//        G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
    }
    //其他状态依情况处理掉。
}
