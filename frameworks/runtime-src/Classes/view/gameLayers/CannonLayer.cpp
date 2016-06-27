//
//  CannonLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/1/18.
//
//

#include "CannonLayer.h"

USING_NS_CC;

CannonLayer::~CannonLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(fireListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(createListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(changeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(mulListener);
}

bool CannonLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
//    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
//    cache->addSpriteFramesWithFile("game_scene/cannon/gun/carriage.plist");
//    cache->addSpriteFramesWithFile("game_scene/cannon/gun/foudation.plist");

    Size winSize = Director::getInstance()->getWinSize();

    heroCannon = CannonNode::create();
    heroCannon->setIsHero(true);
    heroCannon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    heroCannon->setPosition(Vec2(winSize.width/2, 45));
    addChild(heroCannon);
    
    fireListener = EventListenerCustom::create(HERO_FIRE, CC_CALLBACK_1(CannonLayer::heroFire, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(fireListener, -1);
    createListener = EventListenerCustom::create(CREATE_CANNON, CC_CALLBACK_1(CannonLayer::createCannon, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(createListener, -1);
    changeListener = EventListenerCustom::create(CHANGE_CANNON, CC_CALLBACK_1(CannonLayer::changeCannon, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(changeListener, -1);
    mulListener = EventListenerCustom::create(CHANGE_CANNON_MULTIPLY, CC_CALLBACK_1(CannonLayer::changeCannonMultiply, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mulListener, -1);
    return true;
}

void CannonLayer::heroTowardPoint(cocos2d::Vec2 &fingle)
{
    Size winSize = Director::getInstance()->getWinSize();
    fingle.y = winSize.height - fingle.y;
    auto cannonPos = fingle;
    cannonPos -= heroCannon->getPosition();
    float fingleAngle = cannonPos.getAngle();
    fingleAngle = CC_RADIANS_TO_DEGREES(fingleAngle);
    heroCannon->toward(fingleAngle);
}

void CannonLayer::questHeroFire()
{
    heroCannon->questFire(-1);
}

void CannonLayer::questHeroTraceFire(int fishId)
{
    heroCannon->questFire(fishId);
}

void CannonLayer::questHeroBombFire(float bulletX, float bulletY)
{
    heroCannon->questFire(-1, bulletX, bulletY);
}

void CannonLayer::heroFire(EventCustom* event)
{
    L2E_CANNON_FIRE info = *static_cast<L2E_CANNON_FIRE*>(event->getUserData());
//    if (info.isHero) {
    heroCannon->fire();
//    }else{
//        log("其他发炮");
//    }
    
}

void CannonLayer::createCannon(EventCustom* event)
{
    Size winSize = Director::getInstance()->getWinSize();
    L2E_CREATE_CANNON info = *static_cast<L2E_CREATE_CANNON*>(event->getUserData());
    
    removeChildByTag(info.chairId);
    if (info.isHero) {
        heroCannon = CannonNode::create();
        heroCannon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        heroCannon->setPosition(Vec2(winSize.width/2, 40));
        heroCannon->setupCannon(event);
        heroCannon->setTag(info.chairId);
        addChild(heroCannon);

    }else{
        auto cannon = CannonNode::create();
        cannon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        cannon->setPosition(Vec2(winSize.width/2+200-400*info.chairId%2, info.chairId>2?(winSize.height-40):40));
        cannon->setupCannon(event);
        cannon->setTag(info.chairId);
        addChild(cannon);
    }
}

void CannonLayer::changeCannon(EventCustom* event)
{
    L2E_CHANGE_CANNON info = *static_cast<L2E_CHANGE_CANNON*>(event->getUserData());
    heroCannon->changeCannon(info.cannonType);
}

void CannonLayer::changeCannonMultiply(cocos2d::EventCustom *event)
{
    L2E_CHANGE_CANNON_MULTIPLY info = *static_cast<L2E_CHANGE_CANNON_MULTIPLY*>(event->getUserData());
    heroCannon->changeMultiply(info.multiply);
    heroCannon->changeBulletCount(info.bulletCount);
    
}
