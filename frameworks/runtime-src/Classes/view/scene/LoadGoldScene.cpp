//
//  LoadGoldScene.cpp
//  buyu_simple
//
//  Created by ff on 15/12/22.
//
//

#include "LoadGoldScene.h"
#include "../UIManager.h"

USING_NS_CC;

LoadGoldScene::~LoadGoldScene()
{
    unscheduleUpdate();
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateListener);
}

bool LoadGoldScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    updateListener = EventListenerCustom::create(LOAD_GOLD_UPDATE_EVENT, CC_CALLBACK_1(LoadGoldScene::loadUpdate, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateListener, -1);
    scheduleUpdate();
    return true;
}

//void LoadGoldScene::onExit()
//{
//    
//}

void LoadGoldScene::update(float dt)
{
    UIManager::instance()->loop(dt);
}

void LoadGoldScene::loadUpdate(EventCustom* event)
{
    L2E_LOAD_GOLD_UPDATE info = *static_cast<L2E_LOAD_GOLD_UPDATE *>(event->getUserData());
    info.count;
    info.step;
}