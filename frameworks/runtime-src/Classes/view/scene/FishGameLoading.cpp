//
//  FishGameLoading.cpp
//  buyu_simple
//
//  Created by ff on 16/5/10.
//
//

#include "FishGameLoading.h"
#include "EventDef.h"
#include "../UIManager.h"
#include "cocostudio/CocoStudio.h"
#include "sound/SoundManager.h"
#include "GameDef.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

FishGameLoading::FishGameLoading()
{
    
}

FishGameLoading::~FishGameLoading()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateListener);
    unscheduleUpdate();
}

bool FishGameLoading::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    auto bgSpt = Sprite::create("hall_scene/game_hall/background_0.png");
    bgSpt->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    bgSpt->setScaleX(GameUtils::winSize.width/bgSpt->getContentSize().width);
    addChild(bgSpt);
    
    auto root = CSLoader::createNode(GAME_LOADING_UI);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    loadBar = (LoadingBar*)root->getChildByName("loading_bar");
    loadBar->setPercent(0);
    
    updateListener = EventListenerCustom::create(UI_UPDATE_LOAD_STAGE, CC_CALLBACK_1(FishGameLoading::loadUpdate, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateListener, -1);
    
    SoundManager::shared()->stopAll();
    scheduleUpdate();
    
    return true;
}

void FishGameLoading::update(float dt)
{
    UIManager::instance()->loop(dt);
}

void FishGameLoading::loadUpdate(cocos2d::EventCustom *event)
{
    L2E_UPDATE_LOAD_STAGE info = *static_cast<L2E_UPDATE_LOAD_STAGE*>(event->getUserData());
    float percent = info.step*100.0/info.count;
    
    loadBar->setPercent(percent);
}

