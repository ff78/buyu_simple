//
//  FishLayer.cpp
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#include "FishLayer.h"
#include "../node/FishNode.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"

FishLayer::FishLayer()
{
    createListener = EventListenerCustom::create(CREATE_FISH, CC_CALLBACK_1(FishLayer::createFish, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(createListener, -1);
    clearLockListener = EventListenerCustom::create(CLEAR_LOCK_FISH, CC_CALLBACK_0(FishLayer::clearLockFish, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(clearLockListener, -1);
    lockFishId = -1;
    lockFish = nullptr;
    lightFishes.clear();
}

FishLayer::~FishLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(createListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(clearLockListener);
}

bool FishLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    

    return true;
}

bool FishLayer::createFish(EventCustom* event)
{
    L2E_CREATE_FISH info = *static_cast<L2E_CREATE_FISH *>(event->getUserData());

    auto fish = FishNode::create();
    fish->setFishId(info.fishId);
//    fish->setResName(resDir);
    fish->initWithRes();
    addChild(fish);
    
    return true;
}

void FishLayer::update(float dt)
{
    auto fishVec = getChildren();
    
    lightFishes.clear();
    for (auto itr : fishVec)
    {
        ((FishNode*)itr)->update(dt);
        if (((FishNode*)itr)->getClick()) {
            if (lockFish == nullptr) {
                EventCustom event(TRACE_LOCK_FISH);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
            lockFishId = ((FishNode*)itr)->getFishId();
            if (lockFish != nullptr) {
                lockFish->setClick(false);
                lockFish->setLock(false);
            }
            lockFish = ((FishNode*)itr);
            lockFish->setClick(false);
            lockFish->setLock(true);
        }
        if (((FishNode*)itr)->getLight())
        {
            lightFishes.pushBack((FishNode*)itr);
        }
    }

    for (int i = fishVec.size()-1; i>=0; i--) {
        auto fish = ((FishNode*)fishVec.at(i));
        if(!fish->getAlive())
        {
            if (fish == lockFish) {
                lockFish = nullptr;
            }
            fish->removeFromParent();
        }
    }
    if (fishVec.size() == 0) {
        E2L_FISH_ALL_REMOVE info;
        info.eProtocol = e2l_fish_all_remove;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
}

void FishLayer::clearLockFish()
{
    if (lockFish != nullptr) {
        lockFish->setLock(false);
        lockFishId = -1;
        lockFish = nullptr;
        EventCustom event(STOP_LOCK_FIRE);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}