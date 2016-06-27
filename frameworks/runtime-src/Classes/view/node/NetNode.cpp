//
//  NetNode.cpp
//  buyu_simple
//
//  Created by ff on 16/1/22.
//
//

#include "NetNode.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;
using namespace timeline;

bool NetNode::init()
{
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

void NetNode::setupView()
{
    
    std::string resName("game_scene/cannon/net/Net_1");
    resName += ".csb";
    
    animateNode = CSLoader::createNode(resName);
    animateNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    animateNode->setPosition(Vec2::ZERO);
    addChild(animateNode);
    std::string netResStr("game_scene/cannon/net/net_net");
    netResStr += Convert2String(cannonType);
    netResStr += ".png";
    ((Sprite*)animateNode->getChildByName("net_net0_1"))->initWithFile(netResStr);
    ((Sprite*)animateNode->getChildByName("net_net0_1_0"))->initWithFile(netResStr);
    ((Sprite*)animateNode->getChildByName("net_net0_1_1"))->initWithFile(netResStr);
    ((Sprite*)animateNode->getChildByName("net_net0_1_2"))->initWithFile(netResStr);
    
    ActionTimeline *action = CSLoader::createTimeline(resName);
    
    std::string actionName("cast_");
    actionName += Convert2String(bulletCount);
    
    action->play(actionName, false);
    action->setLastFrameCallFunc(CC_CALLBACK_0(NetNode::onRemove, this));
    animateNode->runAction(action);
}

void NetNode::onRemove()
{
    removeFromParent();
}