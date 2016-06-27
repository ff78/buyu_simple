//
//  BulletManager.cpp
//  buyu_simple
//
//  Created by ff on 16/1/20.
//
//

#include "BulletManager.h"
#include "ELProtocol.h"
#include "EventDef.h"
#include "logic/ClientLogic.h"

USING_NS_CC;

BulletManager::BulletManager()
:readyFire(true),
rushFire(false)
{
    clear();
    bulletHitListener = EventListenerCustom::create(BULLET_HIT, CC_CALLBACK_1(BulletManager::bulletHit, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(bulletHitListener, -1);
    startRushListener = EventListenerCustom::create(START_RUSH_FIRE, std::bind(&BulletManager::setRushFire, this, true));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startRushListener, -1);
    endRushListener = EventListenerCustom::create(END_RUSH_FIRE, std::bind(&BulletManager::setRushFire, this, false));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(endRushListener, -1);
}

BulletManager::~BulletManager()
{
    clear();
    
    Director::getInstance()->getEventDispatcher()->removeEventListener(bulletHitListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startRushListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(endRushListener);
}

void BulletManager::clear()
{
    incrementId = 0;
    bulletMap.clear();
    state = BULLET_MANAGER_NORMAL;
}

void BulletManager::logic(float dt)
{
    auto itr = bulletMap.begin();
    while(itr != bulletMap.end())
    {
        if (itr->second.getState() == BULLET_DIE) {
            itr = bulletMap.erase(itr);
        }else{
            itr++;
        }
    }
    
    for (auto it : bulletMap) {
        it.second.OnUpdate(dt);
    }
    

}

bool BulletManager::createBullet(CREATE_BULLET_CMD &cmd)
{
    if (!readyFire) {
        return false;
    }
    if (state == BULLET_MANAGER_RAY || state == BULLET_MANAGER_PLANE) {
        return false;
    }
    
    BlackBullet bullet;
    bullet.create(cmd);
    if (state == BULLET_MANAGER_READY_RAY) {
        bullet.setBulletType(2);
        state = BULLET_MANAGER_RAY;
    }else if (state == BULLET_MANAGER_READY_PLANE) {
        bullet.setBulletType(3);
        state = BULLET_MANAGER_PLANE;
    }
    incrementId++;
    bulletMap[incrementId] = bullet;
    
    
    L2E_CANNON_FIRE info;
    info.eProtocol = l2e_cannon_fire;
    info.bulletId = incrementId;
    

    ClientLogic::instance()->pass2Engine(&info);
    
    if (state == BULLET_MANAGER_NORMAL) {
        if (rushFire) {
            CREATE_BULLET_CMD cmd1;
            memcpy(&cmd1, &cmd, sizeof(CREATE_BULLET_CMD));
            cmd1.angle += 25;
            BlackBullet bullet1;
            bullet1.create(cmd1);
            incrementId++;
            bulletMap[incrementId] = bullet1;
            
            
            L2E_CANNON_FIRE info1;
            info1.eProtocol = l2e_cannon_fire;
            info1.bulletId = incrementId;
            
            ClientLogic::instance()->pass2Engine(&info1);
            
            CREATE_BULLET_CMD cmd2;
            memcpy(&cmd2, &cmd, sizeof(CREATE_BULLET_CMD));
            cmd2.angle -= 25;
            BlackBullet bullet2;
            bullet2.create(cmd2);
            incrementId++;
            bulletMap[incrementId] = bullet2;
            
            
            L2E_CANNON_FIRE info2;
            info2.eProtocol = l2e_cannon_fire;
            info2.bulletId = incrementId;
            
            ClientLogic::instance()->pass2Engine(&info2);
            
        }
    }    
    return true;
}

void BulletManager::bulletHit(EventCustom *event)
{
    std::vector<int> info = *static_cast<std::vector<int>*>(event->getUserData());
    for (auto bulletId : info) {
        BlackBullet bullet = bulletMap[bulletId];
        if (bullet.getBulletType() != 1) {
            continue;
        }
        bulletMap[bulletId].setState(BULLET_CASTING_NET);
        L2E_CAST_NET info;
        info.eProtocol = l2e_cast_net;
        info.bulletId = bulletId;
        info.posX = bullet.getPosition().x;
        info.posY = bullet.getPosition().y;
        info.cannonType = bullet.getCannonType();
        info.bulletCount = bullet.getBulletCount();
        ClientLogic::instance()->pass2Engine(&info);
    }
}

void BulletManager::readyRay()
{
    state = BULLET_MANAGER_READY_RAY;
}

void BulletManager::endRay()
{
    state = BULLET_MANAGER_NORMAL;
}

void BulletManager::readyPlane()
{
    state = BULLET_MANAGER_READY_PLANE;
}

void BulletManager::endPlane()
{
    state = BULLET_MANAGER_NORMAL;
}
