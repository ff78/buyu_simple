//
//  StageCompleteLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/4/5.
//
//
#include "utils/GameUtils.h"
#include "StageCompleteLayer.h"
#include "logic/ClientLogic.h"
#include "../node/GoldFallen.h"
#include "sound/SoundManager.h"
#include "GameDef.h"

USING_NS_CC;
using namespace ui;

bool StageCompleteLayer::init()
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
    
    dialogNode = CSLoader::createNode(STAGE_COMP_TIP_UI);
    dialogNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    dialogNode->setPosition(Vec2(GameUtils::winSize.width/2,GameUtils::winSize.height/2));
    addChild(dialogNode);
    return true;
}

void StageCompleteLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_COMPLETE_BOSS_TASK info = *static_cast<L2E_COMPLETE_BOSS_TASK*>(event->getUserData());
    auto goldLabel = (Text*)dialogNode->getChildByName("bound_gold");
    goldLabel->setString(Convert2String(info.boundGold));
    auto diamondLabel = (Text*)dialogNode->getChildByName("bound_diamond");
    diamondLabel->setString(Convert2String(info.boundDiamond));
    
    auto okBtn = (Button*)dialogNode->getChildByName("collect_btn");
    okBtn->addClickEventListener(CC_CALLBACK_1(StageCompleteLayer::clickOk, this));
}

void StageCompleteLayer::clickOk(cocos2d::Ref *ref)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_TAKE_BOSS_BOUND info;
    info.eProtocol = e2l_boss_task_bounds;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    GOLD_FALL_ANIMATION();
    removeFromParent();
}
