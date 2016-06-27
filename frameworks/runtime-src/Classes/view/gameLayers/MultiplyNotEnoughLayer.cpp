//
//  MultiplyNotEnoughLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/24.
//
//

#include "MultiplyNotEnoughLayer.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "GameDef.h"
#include "utils/StringData.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool MultiplyNotEnoughLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(10, 10, 10, 128))) {
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
    
    root = CSLoader::createNode(NEXT_STAGE_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    return true;
}

void MultiplyNotEnoughLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_CANNON_MULTI_NOT_ENOUGH info = *static_cast<L2E_CANNON_MULTI_NOT_ENOUGH*>(event->getUserData());
    std::string tipStr(SSTRING("room_need_multi_1"));
    tipStr += Convert2String(info.requireMultiply);
    tipStr += SSTRING("room_need_multi_2");
    
    auto tipLabel = (Text*)root->getChildByName("Text_1");
    tipLabel->setString(tipStr);
    
    auto cancelBtn = (Button*)root->getChildByName("cancel_btn");
    cancelBtn->addClickEventListener(CC_CALLBACK_0(MultiplyNotEnoughLayer::clickClose, this));
    
    auto okBtn = (Button*)root->getChildByName("ok_btn");
    okBtn->addClickEventListener(CC_CALLBACK_0(MultiplyNotEnoughLayer::clickOk, this));
}

void MultiplyNotEnoughLayer::clickOk()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_UNLOCK_TO_REQUIRE info;
    info.eProtocol = e2l_unlock_to_require;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void MultiplyNotEnoughLayer::clickClose()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_HIDE_MULTI_NOT_ENOUGH info;
    info.eProtocol = e2l_hide_multi_not_enough;
    ClientLogic::instance()->ProcessUIRequest(&info);
}