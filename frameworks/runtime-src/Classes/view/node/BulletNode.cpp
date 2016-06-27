//
//  BulletNode.cpp
//  buyu_simple
//
//  Created by ff on 16/1/20.
//
//

#include "BulletNode.h"
#include "data/BulletManager.h"
#include "data/FishManager.h"
#include "sound/SoundManager.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;
using namespace timeline;

bool BulletNode::init()
{
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

void BulletNode::initWithRes(L2E_CANNON_FIRE &info)
{
    if (BulletManager::instance()->bulletMap.find(info.bulletId) == BulletManager::instance()->bulletMap.end()) {
        return;
    }

    rayLength = 1;
    auto data = BulletManager::instance()->bulletMap[info.bulletId];
    bulletType = data.getBulletType();
    if (data.getBulletType() == 1) {
        traceFishId = data.getTraceFishId();
        std::string resDir("game_scene/cannon/bullet/bullet_bullet");
//        resDir += std::to_string(data.getCannonType());
        resDir += Convert2String(data.getCannonType());
        resDir += "_";
//        resDir += std::to_string(data.getBulletCount());
        resDir += Convert2String(data.getBulletCount());
        resDir += ".png";
        resName = resDir;
        
        bullet = Sprite::create(resName);
        addChild(bullet);
        
        setPosition(data.getPosition());
        setRotation(data.getAngle());
        radius = data.getRadius();
        angle = 90 - data.getAngle();
        if (traceFishId == -1) {
            auto speedVec = Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
            auto move = MoveBy::create(1, speedVec*1000);
            auto repeateMove = RepeatForever::create(move);
            repeateMove->setTag(10005);
            runAction(repeateMove);
        }
        startDis = -1;
        halfDis = false;
        
        alive = true;
        bulletId = info.bulletId;
        setTag(bulletId);
        AUDIO_PLAY("gun", AUDIO_TYPE::EFFECT_TYPE);
    } else if(data.getBulletType() == 2){
        rayLength = Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height).length();

        traceFishId = data.getTraceFishId();
        resName = BULLET_LASER_ANIM;
        
        bulletAnimation = CSLoader::createNode(resName);
        bulletAction = CSLoader::createTimeline(resName);
        bulletAction->play("play", false);
        bulletAnimation->runAction(bulletAction);
        bulletAction->setLastFrameCallFunc(CC_CALLBACK_0(BulletNode::endRay, this));
        bulletAction->setFrameEventCallFunc(CC_CALLBACK_1(BulletNode::onRayFrameEvent, this));
        bulletAnimation->setScaleY(rayLength/115);
        addChild(bulletAnimation);
        
        setPosition(data.getPosition());
        setRotation(data.getAngle());
        radius = data.getRadius();
        angle = 90 - data.getAngle();
        startDis = -1;
        halfDis = false;
        
        alive = true;
        bulletId = info.bulletId;
        setTag(bulletId);
    } else {
        rayLength = -1;
        
        traceFishId = data.getTraceFishId();
        resName = BULLET_BOMB_ANIM;
        
        bulletAnimation = CSLoader::createNode(resName);
        bulletAction = CSLoader::createTimeline(resName);
        bulletAction->play("play", false);
        bulletAnimation->runAction(bulletAction);
        bulletAction->setLastFrameCallFunc(CC_CALLBACK_0(BulletNode::endPlane, this));
        bulletAction->setFrameEventCallFunc(CC_CALLBACK_1(BulletNode::onRayFrameEvent, this));

        addChild(bulletAnimation);
        
        setPosition(data.getPosition());
        radius = data.getRadius();
        startDis = -1;
        halfDis = false;
        
        alive = true;
        bulletId = info.bulletId;
        setTag(bulletId);
        AUDIO_PLAY("missile", AUDIO_TYPE::EFFECT_TYPE);
    }
}

void BulletNode::update(float dt, int traceId)
{
    if (BulletManager::instance()->bulletMap.find(bulletId) == BulletManager::instance()->bulletMap.end())
    {
        return;
    }
    auto data = BulletManager::instance()->bulletMap[bulletId];
    if (data.getBulletType() != 1) {
        return;
    }
    if (data.getState() != BULLET_FLYING || (traceFishId != traceId && traceFishId != -1)) {
        BulletManager::instance()->bulletMap[bulletId].setState(BULLET_DIE);
        alive = false;
        setVisible(false);
        return;
    }
    
    if (traceFishId != -1) {
        return;
    }    
    auto winSize = Director::getInstance()->getWinSize();


    bool outScreen = false;
    float currRotation = getRotation();
    if (getPositionX() < 0) {
        setPositionX(0);
        currRotation *= -1;
        outScreen = true;
    }
    if (getPositionX() > winSize.width) {
        setPositionX(winSize.width);
        currRotation *= -1;
        outScreen = true;
    }
    
    if (getPositionY() < 0) {
        setPositionY(0);
        currRotation = 180-currRotation;
        outScreen = true;
    }
    if (getPositionY() > winSize.height) {
        setPositionY(winSize.height);
        currRotation = 180-currRotation;
        outScreen = true;
    }
    
    if (outScreen) {
        stopAllActions();
        angle = 90-currRotation;
        auto speedVec = Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
        setRotation(currRotation);
        auto move = MoveBy::create(1, speedVec*1000);
        auto repeateMove = RepeatForever::create(move);
        repeateMove->setTag(10005);
        runAction(repeateMove);
    }


}

void BulletNode::traceFish(cocos2d::Vec2 targetPos)
{
    if (traceFishId == -1) {
        return;
    }
    if (startDis == -1) {
        startDis = targetPos.distance(getPosition());
    }
    float ag = CC_RADIANS_TO_DEGREES((targetPos-getPosition()).getAngle());
    float delta = angle - ag;
    if (!halfDis) {
        halfDis = (targetPos.distance(getPosition()) < startDis/2);
        if (halfDis) {
            stopAllActions();
        }
    }
    if (!halfDis) {
        delta = MIN(delta, 5);
        delta = MAX(delta, -5);
        angle -= delta;
    }else{
        angle = ag;
    }
    
    
//    stopAllActions();
    auto speedVec = Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle));
    auto move = MoveBy::create(1, speedVec*500);
    auto repeateMove = RepeatForever::create(move);
    repeateMove->setTag(10005);
    runAction(repeateMove);
    setRotation(90-angle);
//    float angle = CC_RADIANS_TO_DEGREES(targetPos.getAngle(getPosition()));
    log("angle:%f", angle);
}

void BulletNode::syncCurrPos()
{
    if (BulletManager::instance()->bulletMap.find(bulletId) == BulletManager::instance()->bulletMap.end())
    {
        return;
    }
    

    BulletManager::instance()->bulletMap[bulletId].setPosition(getPosition());
}

void BulletNode::endRay()
{
    if (BulletManager::instance()->bulletMap.find(bulletId) == BulletManager::instance()->bulletMap.end())
    {
        return;
    }
    auto data = BulletManager::instance()->bulletMap[bulletId];
    BulletManager::instance()->bulletMap[bulletId].setState(BULLET_DIE);
    alive = false;
    setVisible(false);
    
    EventCustom event(UI_END_RAY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void BulletNode::endPlane()
{
    if (BulletManager::instance()->bulletMap.find(bulletId) == BulletManager::instance()->bulletMap.end())
    {
        return;
    }
    auto data = BulletManager::instance()->bulletMap[bulletId];
    BulletManager::instance()->bulletMap[bulletId].setState(BULLET_DIE);
    alive = false;
    setVisible(false);
    
    EventCustom event(UI_END_PLANE);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void BulletNode::onRayFrameEvent(cocostudio::timeline::Frame *frame)
{
    EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
    if(!evnt)
        return;
    
    std::string str = evnt->getEvent();
    
    if (str == "attack1")
    {
        EventCustom event(RAY_ATTACK);
        event.setUserData(this);
        Director::getInstance()->getInstance()->getEventDispatcher()->dispatchEvent(&event);
    } else if (str == "attack") {
        EventCustom event(PLANE_ATTACK);
        event.setUserData(this);
        Director::getInstance()->getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}
