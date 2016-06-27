
//
//  NextStageLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/4/5.
//
//

#include "NextStageLayer.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "GameDef.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

bool NextStageLayer::init()
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
    
    auto root = CSLoader::createNode(NEXT_STAGE_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    auto okBtn = (Button*)root->getChildByName("ok_btn");
    okBtn->addClickEventListener(CC_CALLBACK_1(NextStageLayer::clickNext, this));
    auto cancelBtn = (Button*)root->getChildByName("cancel_btn");
    cancelBtn->addClickEventListener(CC_CALLBACK_1(NextStageLayer::clickCancel, this));
    
    return true;
}

void NextStageLayer::clickNext(Ref *ref)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_NEXT_STAGE info;
    info.eProtocol = e2l_next_stage;
    ClientLogic::instance()->ProcessUIRequest(&info);
//    removeFromParent();
}

void NextStageLayer::clickCancel(Ref *ref)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeFromParent();
}
