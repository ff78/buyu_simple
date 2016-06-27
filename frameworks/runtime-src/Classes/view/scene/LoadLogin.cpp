//
//  LoadLogin.cpp
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#include "LoadLogin.h"
#include "../UIManager.h"

USING_NS_CC;

LoadLogin::~LoadLogin()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateListener);
}

bool LoadLogin::init()
{
    if (!Scene::init()) {
        return false;
    }
    updateListener = EventListenerCustom::create(LOAD_LOGIN_UPDATE_EVENT, CC_CALLBACK_1(LoadLogin::loadUpdate, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateListener, -1);
    scheduleUpdate();
    return true;
}

void LoadLogin::onExit()
{
    unscheduleUpdate();
}

void LoadLogin::update(float dt)
{
    UIManager::instance()->loop(dt);
}

void LoadLogin::loadUpdate(EventCustom* event)
{
    LOAD_LOGIN_UPDATE info = *static_cast<LOAD_LOGIN_UPDATE *>(event->getUserData());
    info.count;
    info.step;
}