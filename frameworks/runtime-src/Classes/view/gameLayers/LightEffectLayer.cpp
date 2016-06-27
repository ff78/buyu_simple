//
//  LightEffectLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/3/22.
//
//

#include "LightEffectLayer.h"
#include "../node/LightNode.h"
#include "../node/LightDot.h"

bool LightEffectLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    return true;
}

void LightEffectLayer::addLight(cocos2d::Vec2 pos, float distance, float angle)
{
    LightNode *node = LightNode::create();
    node->setPosition(pos);
    node->setAngle(angle);
    node->setDistance(distance);
    node->setupView();
    addChild(node);
}

void LightEffectLayer::addLightDot(cocos2d::Vec2 pos)
{
    LightDot *node = LightDot::create();
    node->setPosition(pos);
    node->setupView();
    addChild(node);
}
