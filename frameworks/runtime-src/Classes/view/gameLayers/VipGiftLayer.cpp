//
//  VipGiftLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#include "VipGiftLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "ELProtocol.h"
#include "EventDef.h"
#include "sound/SoundManager.h"
#include "GameDef.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

VipGiftLayer::~VipGiftLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(rechargeOkListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(closeListener);
}

bool VipGiftLayer::init()
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
    
    
    root = CSLoader::createNode(HALL_VIP_GIFT_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    auto closeBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_0(VipGiftLayer::clickClose, this));
    
    auto rechargeBtn = (Button*)root->getChildByName("ms_bg")->getChildByName("first_recharge_button");
    rechargeBtn->addClickEventListener(CC_CALLBACK_0(VipGiftLayer::clickRecharge, this));
    
    rechargeOkListener = EventListenerCustom::create(UI_RECHARGE_OK, CC_CALLBACK_0(VipGiftLayer::clickClose, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(rechargeOkListener, -1);
    closeListener = EventListenerCustom::create(UI_CLOSE_RECHARGE, CC_CALLBACK_0(VipGiftLayer::removeOff, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(closeListener, -1);
    return true;
}

void VipGiftLayer::removeOff()
{
    removeFromParent();
}

void VipGiftLayer::clickRecharge()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_RECHARGE info;
    info.eProtocol = e2l_recharge;
    info.rechargeId = 2;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void VipGiftLayer::clickClose()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CLOSE_RECHARGE info;
    info.eProtocol = e2l_close_recharge;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
