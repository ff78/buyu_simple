//
//  NetLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/1/25.
//
//

#include "NetLayer.h"
#include "../Share/Header/EventDef.h"
#include "../Share/Header/ELProtocol.h"
#include "../node/NetNode.h"

USING_NS_CC;

NetLayer::NetLayer()
{
    castListener = EventListenerCustom::create(CAST_NET, CC_CALLBACK_1(NetLayer::castNet, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(castListener, -1);
//    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_scene/cannon/net/net.plist");
//    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_scene/cannon/net/spray.plist");
}

NetLayer::~NetLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(castListener);
//    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_scene/cannon/net/net.plist");
//    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_scene/cannon/net/spray.plist");
}

bool NetLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_scene/cannon/net/net.plist");
    
    return true;
}

void NetLayer::castNet(cocos2d::EventCustom *event)
{
    L2E_CAST_NET info = *static_cast<L2E_CAST_NET*>(event->getUserData());
    auto net = NetNode::create();
    net->setCannonType(info.cannonType);
    net->setbulletCount(info.bulletCount);
    net->setPosition(Vec2(info.posX, info.posY));
    net->setupView();
    addChild(net);
}


