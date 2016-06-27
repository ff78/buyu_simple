//
//  LightDot.cpp
//  buyu_simple
//
//  Created by ff on 16/4/18.
//
//

#include "LightDot.h"
#include "GameDef.h"

USING_NS_CC;
using namespace cocostudio;
using namespace timeline;

bool LightDot::init()
{
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

void LightDot::setupView()
{
    animateNode = CSLoader::createNode(LIGHT_DOT_ANIM);
    animateNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    animateNode->setPosition(Vec2::ZERO);
    addChild(animateNode);
    
    ActionTimeline *action = CSLoader::createTimeline(LIGHT_DOT_ANIM);
    action->play("play", false);
    action->setLastFrameCallFunc(CC_CALLBACK_0(LightDot::onRemove, this));
    animateNode->runAction(action);
}

void LightDot::onRemove()
{
    removeFromParent();
}
