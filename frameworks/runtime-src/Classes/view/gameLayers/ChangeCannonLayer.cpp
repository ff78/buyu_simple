//
//  ChangeCannonLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/18.
//
//

#include "ChangeCannonLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "GameDef.h"
#include "EventDef.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "utils/StringData.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

ChangeCannonLayer::~ChangeCannonLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateListener);
}

bool ChangeCannonLayer::init()
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
    
    root = CSLoader::createNode(CHANGE_WEAPON_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    auto closeBtn = (Button*)root->getChildByName("settings_bg")->getChildByName("close_btn");
    closeBtn->addClickEventListener(CC_CALLBACK_0(ChangeCannonLayer::clickClose, this));
    
    auto okBtn = (Button*)root->getChildByName("settings_bg")->getChildByName("ok_button");
    okBtn->addClickEventListener(CC_CALLBACK_0(ChangeCannonLayer::clickOk, this));
    
    updateListener = EventListenerCustom::create(UI_UPDATE_CANNON_SETTING, CC_CALLBACK_1(ChangeCannonLayer::updateView, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateListener, -1);
    
    return true;
}

void ChangeCannonLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_CANNON_SETTING info = *static_cast<L2E_SHOW_CANNON_SETTING*>(event->getUserData());
    focus = info.focus+1;
    for (int i = 0; i < CANNON_TYPE_MAX_EX-1; i++) {
        auto button = (Button*)root->getChildByName("settings_bg")->getChildByTag(i+1);
        auto cannonSpt = (Sprite*)button->getChildByName("gun_sprite");
        auto cannonLabel = (Text*)button->getChildByName("Text");
        hold[i] = info.holdCannon[i];
//        button->setBright(info.holdCannon[i]);
        if (!info.holdCannon[i]) {
            cannonSpt->setColor(Color3B(77, 77, 77));
            if (i == CANNON_TYPE_ANMIE) {
                if (!info.holdCannon[i-1]) {
                    cannonSpt->setColor(Color3B::BLACK);
                    cannonLabel->setString(SSTRING("mystery_cannon"));
                }else{
                    cannonSpt->setColor(Color3B(77, 77, 77));
                    cannonLabel->setString(SSTRING("anmie_cannon"));
                }
                
            }
        }
        button->addClickEventListener(CC_CALLBACK_1(ChangeCannonLayer::clickCannon, this));
    }
    
    auto button = (Button*)root->getChildByName("settings_bg")->getChildByTag(focus);
    auto blackBg = (Sprite*)button->getChildByName("settings_black");
    blackBg->initWithFile("game_scene/gamer_ui/settings/settings_red.png");
}

void ChangeCannonLayer::updateView(cocos2d::EventCustom *event)
{
    L2E_UPDATE_CANNON_SETTING info = *static_cast<L2E_UPDATE_CANNON_SETTING*>(event->getUserData());
    for (int i = 0; i < CANNON_TYPE_MAX_EX-1; i++) {
        auto button = (Button*)root->getChildByName("settings_bg")->getChildByTag(i+1);
        auto cannonSpt = (Sprite*)button->getChildByName("gun_sprite");
        auto cannonLabel = (Text*)button->getChildByName("Text");
        hold[i] = info.holdCannon[i];
//        button->setBright(info.holdCannon[i]);
        if (!info.holdCannon[i]) {
            cannonSpt->setColor(Color3B(77, 77, 77));
            if (i == CANNON_TYPE_ANMIE) {
                if (!info.holdCannon[i-1]) {
                    cannonSpt->setColor(Color3B::BLACK);
                    cannonLabel->setString(SSTRING("mystery_cannon"));
                }else{
                    cannonSpt->setColor(Color3B(77, 77, 77));
                    cannonLabel->setString(SSTRING("anmie_cannon"));
                }
                
            }
        }else{
            cannonSpt->setColor(Color3B::WHITE);
        }
    }
}

void ChangeCannonLayer::clickClose()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeFromParent();
}

void ChangeCannonLayer::clickCannon(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    auto button = ((Button*)pSender);
    if(hold[button->getTag()-1])
    {
        if (focus != button->getTag()) {
            auto blackBg = (Sprite*)button->getChildByName("settings_black");
            blackBg->initWithFile("game_scene/gamer_ui/settings/settings_red.png");
            
            auto redBg = (Sprite*)root->getChildByName("settings_bg")->getChildByTag(focus)->getChildByName("settings_black");
            redBg->initWithFile("game_scene/gamer_ui/settings/settings_black.png");
            
            focus = button->getTag();
        }
        
    } else {
        confirmLayer = LayerColor::create(Color4B(10, 10, 10, 128));
        auto callback = [](Touch * ,Event *)
        {
            return true;
        };
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = callback;
        listener->setSwallowTouches(true);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,confirmLayer);
        
        auto root = CSLoader::createNode(NEXT_STAGE_UI);
        root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
        confirmLayer->addChild(root);
        addChild(confirmLayer);
        auto tipLabel = (Text*)root->getChildByName("Text_1");
        auto okBtn = (Button*)root->getChildByName("ok_btn");
        auto cancelBtn = (Button*)root->getChildByName("cancel_btn");
        tipLabel->setString(SSTRING("weapon_need_vip"));
        okBtn->addClickEventListener(CC_CALLBACK_0(ChangeCannonLayer::upgradeVip, this));
        cancelBtn->addClickEventListener(CC_CALLBACK_0(ChangeCannonLayer::removeConfirm, this));
    }
}

void ChangeCannonLayer::clickOk()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CHOOSE_CANNON info;
    info.eProtocol = e2l_choose_cannon;
    info.cannonType = focus-1;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    removeFromParent();
}

void ChangeCannonLayer::upgradeVip()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CANNON_UPGRADE_VIP info;
    info.eProtocol = e2l_cannon_upgrade_vip;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    removeChild(confirmLayer);
}

void ChangeCannonLayer::removeConfirm()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeChild(confirmLayer);
}
