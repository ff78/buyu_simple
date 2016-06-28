//
//  EvaluateLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/6/28.
//
//

#include "EvaluateLayer.h"
#include "GameDef.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "../node/GoldFallen.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool EvaluateLayer::init()
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
    
    auto rootNode = CSLoader::createNode(HALL_EVALUATE_UI);
    rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rootNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(rootNode);
    
    auto closeBtn = (Button*)rootNode->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_0(EvaluateLayer::removeOff, this));
    
    auto cancelBtn = (Button*)rootNode->getChildByName("cancel_button");
    cancelBtn->addClickEventListener(CC_CALLBACK_0(EvaluateLayer::removeOff, this));
    
    auto okBtn = (Button*)rootNode->getChildByName("ok_button");
    okBtn->addClickEventListener(CC_CALLBACK_0(EvaluateLayer::clickOk, this));
        
    return true;
}

void EvaluateLayer::clickOk()
{
    E2L_COMMENT info;
    info.eProtocol = e2l_commnet;
    
    ClientLogic::instance()->ProcessUIRequest(&info);
    GOLD_FALL_ANIMATION();
    removeFromParent();
    
}

void EvaluateLayer::removeOff()
{
    removeFromParent();
}
