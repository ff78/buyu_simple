//
//  ChangeWeaponLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/1/26.
//
//

#include "ChangeWeaponLayer.h"
#include "data/CannonManager.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

bool ChangeWeaponLayer::init()
{
    if(!LayerColor::initWithColor(Color4B(10,10,10,128)))
    {
        return false;
    }
    auto winSize = Director::getInstance()->getWinSize();
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    weaponUI = (Layer*)CSLoader::createNode("hall_scene/personal_setting/ChangeWeapon.csb");
    weaponUI->setPosition(Vec2(winSize.width/2, winSize.height/2));
    weaponUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(weaponUI);
    
    
    actionTimeLine = CSLoader::createTimeline("hall_scene/personal_setting/ChangeWeapon.csb");
    actionTimeLine->play("show", false);
    weaponUI->runAction(actionTimeLine);
    
    auto ExitBtn = (Button*)weaponUI->getChildByName("notice_background1_1")->getChildByName("ExitBtn");
    ExitBtn->addClickEventListener(CC_CALLBACK_1(ChangeWeaponLayer::clickExit, this));
    
    auto okBtn = (Button*)weaponUI->getChildByName("notice_background1_1")->getChildByName("OKBtn");
    okBtn->addClickEventListener(CC_CALLBACK_1(ChangeWeaponLayer::clickOk, this));
    
    for (int i = CANNON_TYPE_XINSHOU; i < CANNON_TYPE_NENGLIANG; i++) {
        auto bg = weaponUI->getChildByName("notice_background1_1");
        auto btn = (Button*)bg->getChildByName("Gun"+Convert2String(i+1)+"Btn");
        btn->addClickEventListener(CC_CALLBACK_1(ChangeWeaponLayer::clickCannon, this));
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hall_scene/personal_setting/weapon.plist");
    updateCannonBtn();
    chooseFlag = (Sprite*)weaponUI->getChildByName("notice_background1_1")->getChildByName("choose");
    chooseFlag->removeFromParent();
    focus = CanonnManager::instance()->getHeroCannon().getCannonType();
    auto btn = (Button*)weaponUI->getChildByName("notice_background1_1")->getChildByName("Gun"+Convert2String(focus+1)+"Btn");
    btn->addChild(chooseFlag);
    chooseFlag->setPosition(Vec2::ZERO);
    return true;
}

void ChangeWeaponLayer::clickExit(Ref* pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    actionTimeLine->play("hide", false);
    actionTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(ChangeWeaponLayer::exit, this));
    
}

void ChangeWeaponLayer::exit()
{
    removeFromParent();
}

void ChangeWeaponLayer::updateCannonBtn()
{
    for (int i = CANNON_TYPE_ZIYUAN; i < CANNON_TYPE_NENGLIANG; i++) {
        auto bg = weaponUI->getChildByName("notice_background1_1");
        bg->getChildByName("Gun"+Convert2String(i+1)+"Btn")->setColor(CanonnManager::instance()->holdCannon[i]?Color3B::WHITE:Color3B(77,77,77));
    }
    
    auto btn = (Button*)weaponUI->getChildByName("notice_background1_1")->getChildByName("Gun"+Convert2String(CANNON_TYPE_ANMIE+1)+"Btn");
    std::string btnResName("hall_scene/personal_setting/me_gun8.png");
    if (!CanonnManager::instance()->holdCannon[CANNON_TYPE_ANMIE]) {
//        btn->setColor(Color3B::WHITE);
        btnResName = "hall_scene/personal_setting/me_gun108.png";
    }
    btn->loadTextureNormal(btnResName, TextureResType::PLIST);
    
}

void ChangeWeaponLayer::clickCannon(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    int cannonTypeId = ((Button*)pSender)->getTag()-1;
    log("click:>>>>%d<<<<<",((Button*)pSender)->getTag());
    
    if (CanonnManager::instance()->holdCannon[cannonTypeId]) {
        chooseFlag->retain();
        chooseFlag->removeFromParent();

        auto btn = (Button*)weaponUI->getChildByName("notice_background1_1")->getChildByName("Gun"+Convert2String(cannonTypeId+1)+"Btn");
        btn->addChild(chooseFlag);
        chooseFlag->release();
        focus = cannonTypeId;
//    } else {
        
    }
}

void ChangeWeaponLayer::clickOk(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CHOOSE_CANNON info;
    info.eProtocol = e2l_choose_cannon;
    info.cannonType = focus;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    clickExit(nullptr);
}
