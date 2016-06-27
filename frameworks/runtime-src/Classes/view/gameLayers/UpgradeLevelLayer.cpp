//
//  UpgradeLevelLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/3/29.
//
//

#include "UpgradeLevelLayer.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"
#include "ui/CocosGUI.h"
#include "logic/ClientLogic.h"
#include "EventDef.h"
#include "GameDef.h"
#include "sound/SoundManager.h"
#include "utils/StringData.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

UpgradeLevelLayer::~UpgradeLevelLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(unlockSuccessListener);
}

bool UpgradeLevelLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(10, 10, 10, 128))) {
        return false;
    }
    auto root = CSLoader::createNode(UNLOCK_CANNON_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    bg = root->getChildByName("ms_bg");
    for (int i = 0; i < 5; i++) {
        card[i] = bg->getChildByName("FileNode"+Convert2String(i+1));
    }
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);


    auto unlockBtn = (Button*)card[0]->getChildByName("UnlockBtn");
    unlockBtn->addClickEventListener(CC_CALLBACK_1(UpgradeLevelLayer::clickUpgrade, this));
    
    auto closeBtn = (Button*)bg->getChildByName("close");
    closeBtn->addClickEventListener(CC_CALLBACK_1(UpgradeLevelLayer::clickClose, this));
    
    unlockSuccessListener = EventListenerCustom::create(UI_CHARGE_UNLOCK_LEVEL, CC_CALLBACK_1(UpgradeLevelLayer::unlockSuccess, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(unlockSuccessListener, -1);
    return true;
}

void UpgradeLevelLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_CANNON_SHOW_UNLOCK_DIALOG info = *static_cast<L2E_CANNON_SHOW_UNLOCK_DIALOG*>(event->getUserData());
    for (int i = 0; i<5; i++) {
        card[i]->setVisible(i<info.cardCount);
        if (i >= info.cardCount) {
            break;
        }
        
        auto bg = card[i]->getChildByName("ms_unlock_card");
        auto diamondLabel = (TextAtlas*)bg->getChildByName("NeedDiamond");
        diamondLabel->setString(Convert2String(info.needDiamond[i]));
        
        auto multiLabel = (TextAtlas*)card[i]->getChildByName("Multi");
        multiLabel->setString(Convert2String(info.nextMultiply[i]));
        
        auto boundLabel = (TextAtlas*)card[i]->getChildByName("Bounds");
        boundLabel->setString(Convert2String(info.boundGolds[i]));
    }

}

//bool UpgradeLevelLayer::onTouchCardBegan(cocos2d::Touch *touch, cocos2d::Event *event)
//{
//    Vec2 locationInNode = this->convertToWorldSpace(touch->getLocation());
//    auto iconPos = bg->getPosition()-Vec2(bg->getContentSize().width, bg->getContentSize().height/2);
//    auto origin = this->convertToWorldSpace(card[0]->getPosition());
//    origin += iconPos;
//    Size s = card[0]->getContentSize();
//    Rect rect = Rect(origin.x-s.width/2, origin.y-s.height/2, s.width, s.height);
//    //
//    if (rect.containsPoint(locationInNode)){
//        log("click card[0] end");
//        
//        return true;
//    }
//    return false;
//}
//
//void UpgradeLevelLayer::onTouchCard(cocos2d::Touch *touch, cocos2d::Event *event)
//{
//    
//}

void UpgradeLevelLayer::clickUpgrade(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CHARGE_UNLOCK_LEVEL info;
    info.eProtocol = e2l_charge_unlock_level;
//    info.chargeId = 1;
    ClientLogic::instance()->ProcessUIRequest(&info);
//    
//    log("click card[0] end");
//    tipNode = CSLoader::createNode("common/CommonTip.csb");
//    tipNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    tipNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
//    addChild(tipNode);
//    
//    auto text2 = (Text*)tipNode->getChildByName("Text_2");
//    auto boundText = (TextAtlas*)card[0]->getChildByName("Bounds");
//    std::string boundStr("获得奖励金币");
//    boundStr += boundText->getString();
//    text2->setString(boundStr);
//    
//    auto okBtn = (Button*)tipNode->getChildByName("ButtonOk");
//    okBtn->addClickEventListener(CC_CALLBACK_1(UpgradeLevelLayer::clickDialogOk, this));
}

void UpgradeLevelLayer::clickClose(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
//    removeFromParent();
    E2L_CANNON_HIDE_UNLOCK_LEVEL info;
    info.eProtocol = e2l_cannon_hide_unlock_level;
    ClientLogic::instance()->ProcessUIRequest(&info);
//    EventCustom event(UI_CHARGE_UNLOCK_LEVEL);
//    
//    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UpgradeLevelLayer::unlockSuccess(cocos2d::EventCustom *event)
{
    L2E_CHARGE_UNLOCK_LEVEL info = *static_cast<L2E_CHARGE_UNLOCK_LEVEL*>(event->getUserData());
    
    
    tipNode = CSLoader::createNode(COMMON_TIP_UI);
    tipNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tipNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tipNode);

    auto text2 = (Text*)tipNode->getChildByName("Text_2");
    std::string boundStr(SSTRING("get_score"));
    boundStr += Convert2String(info.bound);
    text2->setString(boundStr);

    auto okBtn = (Button*)tipNode->getChildByName("ButtonOk");
    okBtn->addClickEventListener(CC_CALLBACK_1(UpgradeLevelLayer::clickClose, this));
}

//void UpgradeLevelLayer::clickDialogOk(cocos2d::Ref *pSender)
//{
//    E2L_CHARGE_UNLOCK_LEVEL info;
//    info.eProtocol = e2l_charge_unlock_level;
////    info.chargeId = 1;
//    ClientLogic::instance()->ProcessUIRequest(&info);
//}
