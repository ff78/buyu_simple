//
//  GameSettingLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/4/1.
//
//

#include "GameSettingLayer.h"
#include "cocostudio/CocoStudio.h"

#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "EventDef.h"
#include "ChangeWeaponLayer.h"
#include "ShowFishLayer.h"
#include "ChangeCannonLayer.h"
#include "sound/SoundManager.h"
#include "GameDef.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

GameSettingLayer::~GameSettingLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(fishSettingListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(cannonSettingListener);
    SoundManager::shared()->saveSettings();
}
bool GameSettingLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(10, 10, 10, 128))) {
        return false;
    }
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    animationNode = CSLoader::createNode(SETTINGS_UI);
    animationNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    animationNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(animationNode);
    
    auto exitBtn = (Button*)animationNode->getChildByName("settings_bg")->getChildByName("CloseBtn");
    exitBtn->addClickEventListener(CC_CALLBACK_0(GameSettingLayer::clickExit, this));
    
    auto quitBtn = (Button*)animationNode->getChildByName("settings_bg")->getChildByName("ExitBtn");
    quitBtn->addClickEventListener(CC_CALLBACK_0(GameSettingLayer::clickQuit, this));
    
    auto weaponBtn = (Button*)animationNode->getChildByName("settings_bg")->getChildByName("WeaponBtn");
    weaponBtn->addClickEventListener(CC_CALLBACK_0(GameSettingLayer::clickWeapon, this));
    
    auto fishBtn = (Button*)animationNode->getChildByName("settings_bg")->getChildByName("FishBtn");
    fishBtn->addClickEventListener(CC_CALLBACK_0(GameSettingLayer::clickFish, this));
    
    soundSwitch = (Button*)animationNode->getChildByName("settings_bg")->getChildByName("SoundBtn");
    musicSwitch = (Button*)animationNode->getChildByName("settings_bg")->getChildByName("MusicBtn");
    soundSwitch->addClickEventListener(CC_CALLBACK_0(GameSettingLayer::clickSound, this));
    musicSwitch->addClickEventListener(CC_CALLBACK_0(GameSettingLayer::clickMusic, this));
    if(!SoundManager::shared()->getSoundSwitch())
    {
        soundSwitch->loadTextureNormal("game_scene/gamer_ui/settings/settings_no.png");
    }
    if(!SoundManager::shared()->getMusicSwitch())
    {
        musicSwitch->loadTextureNormal("game_scene/gamer_ui/settings/settings_no.png");
    }
    
    fishSettingListener = EventListenerCustom::create(UI_SHOW_FISH_SETTING, CC_CALLBACK_1(GameSettingLayer::showFishSetting, this) );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(fishSettingListener, -1);
    cannonSettingListener = EventListenerCustom::create(UI_SHOW_CANNON_SETTING, CC_CALLBACK_1(GameSettingLayer::showCannonSetting, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(cannonSettingListener, -1);
    return true;
}

void GameSettingLayer::clickExit()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_EXIT_SETTING info;
    info.eProtocol = e2l_exit_settings;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameSettingLayer::clickQuit()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_QUIT_STAGE info;
    info.eProtocol = e2l_quit_stage;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

//void GameSettingLayer::exitSetting()
//{
//    removeFromParent();
//}

void GameSettingLayer::clickWeapon()
{
//    auto weaponLayer = ChangeWeaponLayer::create();
//    addChild(weaponLayer);
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_CANNON_SETTING info;
    info.eProtocol = e2l_show_cannon_setting;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameSettingLayer::clickFish()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_FISH info;
    info.eProtocol = e2l_show_fish;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameSettingLayer::showFishSetting(cocos2d::EventCustom *event)
{
    auto layer = ShowFishLayer::create();
    layer->setupView(event);
    addChild(layer);
}

void GameSettingLayer::showCannonSetting(cocos2d::EventCustom *event)
{
    auto layer = ChangeCannonLayer::create();
    layer->setupView(event);
    addChild(layer);
}

void GameSettingLayer::clickSound()
{
    bool soundOpen = SoundManager::shared()->getSoundSwitch();
    soundOpen = !soundOpen;
    SoundManager::shared()->setSoundSwitch(soundOpen);
    if (soundOpen) {
        soundSwitch->loadTextureNormal("game_scene/gamer_ui/settings/settings_yes.png");
        AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    }else{
        soundSwitch->loadTextureNormal("game_scene/gamer_ui/settings/settings_no.png");
    }
}

void GameSettingLayer::clickMusic()
{
    bool musicOpen = SoundManager::shared()->getMusicSwitch();
    musicOpen = !musicOpen;
    SoundManager::shared()->setMusicSwitch(musicOpen);
    if (musicOpen) {
        musicSwitch->loadTextureNormal("game_scene/gamer_ui/settings/settings_yes.png");
        std::string musicName("bg");
        musicName += Convert2String(abs(rand())%2+1);
        AUDIO_PLAY(musicName, AUDIO_TYPE::MUSIC_TYPE);
    }else{
        musicSwitch->loadTextureNormal("game_scene/gamer_ui/settings/settings_no.png");
        SoundManager::shared()->stopMusic();
    }
}
