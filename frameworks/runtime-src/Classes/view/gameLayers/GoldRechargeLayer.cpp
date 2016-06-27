//
//  GoldRechargeLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/26.
//
//

#include "GoldRechargeLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "GameDef.h"
#include "EventDef.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

GoldRechargeLayer::~GoldRechargeLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateViewListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(closeListener);
}

bool GoldRechargeLayer::init()
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

    
    root = CSLoader::createNode(GOLD_RECHARGE_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    auto closeBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_0(GoldRechargeLayer::clickClose, this));
    
    auto diamondBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("diamond_button");
    auto vipIntroBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("recharge_tips")->getChildByName("vip_intro");
    diamondBtn->addClickEventListener(CC_CALLBACK_0(GoldRechargeLayer::clickDiamond, this));
    vipIntroBtn->addClickEventListener(CC_CALLBACK_0(GoldRechargeLayer::clickVipIntro, this));
    
    for (auto node : root->getChildByName("ms_bg")->getChildByName("active_area")->getChildren()) {
        ((Button*)node)->addClickEventListener(CC_CALLBACK_1(GoldRechargeLayer::clickRechargeButton, this));
    }
    updateViewListener = EventListenerCustom::create(UI_RECHARGE_GOLD_OK, CC_CALLBACK_1(GoldRechargeLayer::updateView, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateViewListener, -1);
    closeListener = EventListenerCustom::create(UI_CLOSE_RECHARGE, CC_CALLBACK_0(GoldRechargeLayer::removeOff, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(closeListener, -1);
    return true;
}

void GoldRechargeLayer::setupView(EventCustom* event)
{
    L2E_SHOW_HALL_RECHARGE info = *static_cast<L2E_SHOW_HALL_RECHARGE*>(event->getUserData());
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
    
    for (int i = 0; i < 6; i++) {
        auto giftTipSpt = (Sprite*)root->getChildByName("ms_bg")->getChildByName("active_area")->getChildByName(("recharge_button_"+Convert2String(i+1)).c_str())->getChildByName("recharge_give");
        std::string giftStr("hall_scene/recharge/recharge_give_1");
        giftStr += Convert2String(i+1);
        giftStr += ".png";
        if (!info.firstRecharge[i]) {
            giftTipSpt->initWithFile(giftStr);
        }
        
    }
}

void GoldRechargeLayer::updateView(cocos2d::EventCustom *event)
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
    
    for (int i = 0; i < 6; i++) {
        auto giftTipSpt = (Sprite*)root->getChildByName("ms_bg")->getChildByName("active_area")->getChildByName(("recharge_button_"+Convert2String(i+1)).c_str())->getChildByName("recharge_give");
        std::string giftStr("hall_scene/recharge/recharge_give_1");
        giftStr += Convert2String(i+1);
        giftStr += ".png";
        if (!info.firstRecharge[i]) {
            giftTipSpt->initWithFile(giftStr);
        }
        
    }
}

void GoldRechargeLayer::removeOff()
{
    removeFromParent();
}

void GoldRechargeLayer::clickDiamond()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_HALL_RECHARGE info;
    info.eProtocol = e2l_show_hall_recharge;
    info.type = 0;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GoldRechargeLayer::clickVipIntro()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_VIP_INTRO info;
    info.eProtocol = e2l_show_vip_intro;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GoldRechargeLayer::clickRechargeButton(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    int tag = ((Button*)pSender)->getTag();
    E2L_RECHARGE info;
    info.eProtocol = e2l_recharge;
    info.rechargeId = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GoldRechargeLayer::clickClose()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CLOSE_RECHARGE info;
    info.eProtocol = e2l_close_recharge;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

