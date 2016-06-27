//
//  BulletLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/1/18.
//
//

#include "BulletLayer.h"
#include "../node/BulletNode.h"

USING_NS_CC;

BulletLayer::BulletLayer()
{
    createListener = EventListenerCustom::create(HERO_FIRE, CC_CALLBACK_1(BulletLayer::createBullet, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(createListener, -1);
}

BulletLayer::~BulletLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(createListener);
}

bool BulletLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

void BulletLayer::update(float dt, int traceId)
{
    auto bulletVec = getChildren();
    for (auto it : bulletVec) {
        ((BulletNode*)it)->update(dt, traceId);
    }
    
    for (int i = (int)bulletVec.size() - 1; i >= 0; i--) {
        if (!bulletVec.at(i)->isVisible()) {
            removeChild(bulletVec.at(i));
        }
    }
}

void BulletLayer::updateTrace(cocos2d::Vec2 pos)
{
    auto bulletVec = getChildren();
    for (auto it : bulletVec) {
        ((BulletNode*)it)->traceFish(pos);
    }
}

void BulletLayer::createBullet(cocos2d::EventCustom *event)
{
    L2E_CANNON_FIRE info = *static_cast<L2E_CANNON_FIRE*>(event->getUserData());
    auto bullet = BulletNode::create();
    bullet->initWithRes(info);
    addChild(bullet);
}

void BulletLayer::pauseAll()
{
    pause();
    auto bulletVec = getChildren();
    for (auto it : bulletVec) {
        ((BulletNode*)it)->pause();
    }
}

void BulletLayer::resumeAll()
{
    resume();
    auto bulletVec = getChildren();
    for (auto it : bulletVec) {
        ((BulletNode*)it)->resume();
    }
}