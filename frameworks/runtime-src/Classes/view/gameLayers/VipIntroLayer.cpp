//
//  VipIntroLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#include "VipIntroLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "EventDef.h"
#include "GameDef.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "utils/GameUtils.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;

VipIntroLayer::~VipIntroLayer()
{
//    Director::getInstance()->getEventDispatcher()->removeEventListener(updateViewListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(closeListener);
}

bool VipIntroLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(10, 10, 10, 128)))
    {
        return false;
    }
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    
    root = CSLoader::createNode(HALL_VIP_INTRO_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    auto closeBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_0(VipIntroLayer::clickClose, this));
    
    auto goldBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("gold_button");
    auto vipGiftBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("vip_package");
    goldBtn->addClickEventListener(CC_CALLBACK_0(VipIntroLayer::clickGold, this));
    vipGiftBtn->addClickEventListener(CC_CALLBACK_0(VipIntroLayer::clickGift, this));
    
//    updateViewListener = EventListenerCustom::create(UI_RECHARGE_OK, CC_CALLBACK_1(VipIntroLayer::updateView, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateViewListener, -1);
    closeListener = EventListenerCustom::create(UI_CLOSE_RECHARGE, CC_CALLBACK_0(VipIntroLayer::removeOff, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(closeListener, -1);
    return true;
}

void VipIntroLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_VIP_INTRO info = *static_cast<L2E_SHOW_VIP_INTRO*>(event->getUserData());
    auto nextNeedText = (TextAtlas*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("next_vip_money");
    nextNeedText->setString(Convert2String(info.nextNeedMoney));
    auto nextBar = (LoadingBar*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("recharge_bar");
    nextBar->setPercent(info.nextPercent);
    
    auto vip0Spt = (Sprite*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("vip_0_spt");
    auto currVipSpt = (Sprite*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("curr_vip_spt");
    vip0Spt->setVisible(info.vip == 0);
    currVipSpt->setVisible(info.vip != 0);
    auto currVipText = (TextAtlas*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("curr_vip_level");
    if (info.vip == 0) {
        currVipText->setString("/");
    }else{
        currVipText->setString(Convert2String(info.vip));
    }
    
    auto nextVipText = (TextAtlas*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("next_vip_level");
    if (info.vip == 10) {
        nextVipText->setString(Convert2String(10));
    }else{
        nextVipText->setString(Convert2String(info.vip+1));
    }
    
    auto giftBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("vip_package");
    giftBtn->setVisible(info.vip < 1);

}

void VipIntroLayer::updateView(cocos2d::EventCustom *event)
{
    L2E_RECHARGE_OK info = *static_cast<L2E_RECHARGE_OK*>(event->getUserData());
    auto nextNeedText = (TextAtlas*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("next_vip_money");
    nextNeedText->setString(Convert2String(info.nextNeedMoney));
    auto nextBar = (LoadingBar*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("recharge_bar");
    nextBar->setPercent(info.nextPercent);
    
    auto vip0Spt = (Sprite*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("vip_0_spt");
    auto currVipSpt = (Sprite*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("curr_vip_spt");
    vip0Spt->setVisible(info.vip == 0);
    currVipSpt->setVisible(info.vip != 0);
    auto currVipText = (TextAtlas*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("curr_vip_level");
    if (info.vip == 0) {
        currVipText->setString("/");
    }else{
        currVipText->setString(Convert2String(info.vip));
    }
    
    auto nextVipText = (TextAtlas*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("next_vip_level");
    if (info.vip == 10) {
        nextVipText->setString(Convert2String(10));
    }else{
        nextVipText->setString(Convert2String(info.vip+1));
    }
    
    auto giftBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("vip_package");
    giftBtn->setVisible(info.vip < 1);
}

void VipIntroLayer::removeOff()
{
    removeFromParent();
}

void VipIntroLayer::clickGold()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_HALL_RECHARGE info;
    info.eProtocol = e2l_show_hall_recharge;
    info.type = 1;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void VipIntroLayer::clickGift()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_VIP_GIFT info;
    info.eProtocol = e2l_show_vip_gift;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void VipIntroLayer::clickClose()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CLOSE_RECHARGE info;
    info.eProtocol = e2l_close_recharge;
    ClientLogic::instance()->ProcessUIRequest(&info);
}