//
//  LightNode.cpp
//  buyu_simple
//
//  Created by ff on 16/3/22.
//
//

#include "LightNode.h"
#include "cocostudio/CocoStudio.h"
#include "GameDef.h"

USING_NS_CC;
using namespace cocostudio;
using namespace timeline;

bool LightNode::init()
{
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

void LightNode::setupView()
{
    animateNode = CSLoader::createNode(LIGHT_EFFECT);
    animateNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    animateNode->setPosition(Vec2::ZERO);
    addChild(animateNode);

    ActionTimeline *action = CSLoader::createTimeline(LIGHT_EFFECT);
    action->play("play", false);
    action->setLastFrameCallFunc(CC_CALLBACK_0(LightNode::onRemove, this));
    animateNode->runAction(action);
    
    animateNode->setRotation(angle);
    animateNode->setScaleX(distance/482);
    
}

void LightNode::onRemove()
{
    removeFromParent();
}
