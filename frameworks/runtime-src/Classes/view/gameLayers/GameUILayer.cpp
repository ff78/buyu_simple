//
//  GameUILayer.cpp
//  buyu_simple
//
//  Created by ff on 16/1/26.
//
//

#include "GameUILayer.hpp"
#include "ChangeWeaponLayer.h"
#include "EventDef.h"
#include "GameDef.h"
#include "data/SkillManager.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

GameUILayer::GameUILayer()
{
    
}

GameUILayer::~GameUILayer()
{
    unscheduleUpdate();
    Director::getInstance()->getEventDispatcher()->removeEventListener(upgradeHideListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(upgradeUnlockListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(upgradeUpdateListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateGoldListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateDiamondListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateEnergyListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateHeadinfoListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateBossTaskListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(initBossTaskListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(initTaskListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateSkillListener);
//    Director::getInstance()->getEventDispatcher()->removeEventListener(showSkillChargeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideSkillChargeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateStarBoxListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateTaskListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(useSkillListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(canUseAutoFireListener);
}

bool GameUILayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    readyRayFire = false;
    rayFiring = false;
    readyPlaneFire = false;
    planeFiring = false;
    canUnlock = true;
    hideTick = 0;
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_scene/right_menu/GameMenu.plist");
    auto winSize = Director::getInstance()->getWinSize();
    
    MenuLayer = CSLoader::createNode("GameMenuNode.csb");
//    MenuLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    MenuLayer->setPosition(Vec2(winSize.width, winSize.height/2));
    addChild(MenuLayer);
    
    uiAction = CSLoader::createTimeline("GameMenuNode.csb");
    MenuLayer->runAction(uiAction);
    
//    auto weaponBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("WeaponBtn");
//    weaponBtn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::clickWeapon, this));
    
    auto autoBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("AutoBtn");
    autoBtn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::clickAuto, this));
    autoFire = false;
    
    auto skillMap = SkillManager::instance()->skillMap;
    for(auto skillItr : skillMap)
    {
        int skillId = skillItr.first;
        auto Btn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(skillId);
        Btn->setEnabled(skillMap[skillId].work);
        Btn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::clickLock, this));
        auto countLabel = (TextAtlas*)Btn->getChildByName("count_label");
        auto diamondSpt = Btn->getChildByName("diamond");
        auto diamondLabel = (TextAtlas*)Btn->getChildByName("diamond_label");
        if (skillMap[skillId].count>0) {
            countLabel->setVisible(true);
            countLabel->setString(Convert2String(skillMap[skillId].count));
            diamondSpt->setVisible(false);
            diamondLabel->setVisible(false);
        } else if (skillMap[skillId].count == -1) {
            countLabel->setVisible(false);
//            countLabel->setString(Convert2String(skillMap[skillId].count));
            diamondSpt->setVisible(false);
            diamondLabel->setVisible(false);
        }else{
            countLabel->setVisible(false);
            diamondSpt->setVisible(true);
            diamondLabel->setVisible(true);
            diamondLabel->setString(Convert2String(skillMap[skillId].price));
        }
        
    }
//    lockSkillBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("LockBtn");
//    lockSkillBtn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::clickLock, this));
//    
//    iceSkillBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("IceBtn");
//    iceSkillBtn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::clickIce, this));
//    
//    rushSkillBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("RushBtn");
//    rushSkillBtn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::clickRush, this));
//    
//    raySkillBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("RayBtn");
//    raySkillBtn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::clickRay, this));
//
//    bombSkillBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("BombBtn");
//    bombSkillBtn->addTouchEventListener(CC_CALLBACK_2(GameUILayer::clickBomb, this));
    
    auto hideBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("MoveRightBtn");
    hideBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickHide, this));
    
    auto showBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("MoveLeftBtn");
    showBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickShow, this));
    settingBtn = (Button*)MenuLayer->getChildByName("settingButton");
    settingBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickSetting, this));
    
    auto leftRoot = CSLoader::createNode(GAME_LEFT_MENU_UI);
    leftRoot->setPosition(Vec2(0, winSize.height/2));
    addChild(leftRoot);
    leftMenuAction = CSLoader::createTimeline(GAME_LEFT_MENU_UI);
    leftRoot->runAction(leftMenuAction);
    
    auto rechargeBtn = (Button*)leftRoot->getChildByName("recharge_btn");
    rechargeBtn->addClickEventListener(CC_CALLBACK_0(GameUILayer::clickRechargeBtn, this));

    Vector<FiniteTimeAction*> scaleVec;
    scaleVec.pushBack(ScaleTo::create(0.4, 1.2));
    scaleVec.pushBack(ScaleTo::create(0.2, 0.8));
    scaleVec.pushBack(ScaleTo::create(0.4, 1.2));
    scaleVec.pushBack(ScaleTo::create(0.2, 0.8));
    scaleVec.pushBack(ScaleTo::create(0.4, 1.2));
    scaleVec.pushBack(ScaleTo::create(0.2, 0.8));
    scaleVec.pushBack(ScaleTo::create(0.1, 1.5));
    scaleVec.pushBack(ScaleTo::create(0.1, 0.8));
    auto seq = Sequence::create(scaleVec);
    
    rechargeBtn->runAction(RepeatForever::create(seq));
    

    bossTaskBg = leftRoot->getChildByName("ms_level_target_bg");
    bossTaskBtn = (Button*)leftRoot->getChildByName("success_btn");
    bossTaskBtn->setVisible(false);
    bossTaskBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickSuc, this));
    sucAction = CSLoader::createTimeline(GAME_LEFT_MENU_UI);
    runAction(sucAction);
    
//    CannonUpgrade = CSLoader::createNode("game_scene/gamer_ui/mianscence/CannonUpgrade.csb");
    CannonUpgrade = leftRoot->getChildByName("cannon_node");
//    CannonUpgrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    CannonUpgrade->setPosition(Vec2(40, 420));
    CannonUpgrade->setVisible(false);
    showUnlockBtn = (Button*)CannonUpgrade->getChildByName("show_unlock_btn");
    showUnlockBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickShowUnlock, this));
//    addChild(CannonUpgrade);
    
    TopLayer = CSLoader::createNode(GAME_TOP_MENU_UI);
    TopLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    TopLayer->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height));
    addChild(TopLayer);
    showTaskBtn = (Button*)TopLayer->getChildByName("show_task_btn");
    showTaskBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickShowTask, this));
    showTaskBoundBtn = (Button*)TopLayer->getChildByName("show_bound_btn");
    showTaskBoundBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickShowTask, this));
    showTaskBoundBtn->setVisible(false);
    taskBoundAction = CSLoader::createTimeline(GAME_TOP_MENU_UI);
    runAction(taskBoundAction);
    
    BottomLayer = CSLoader::createNode(GAME_BOTTOM_MENU_UI);
    BottomLayer->setPosition(Vec2(GameUtils::winSize.width/2, 0));
    addChild(BottomLayer);
    energyBar = (LoadingBar *)BottomLayer->getChildByName("user_place_1")->getChildByName("energy_bar_bg")->getChildByName("energy_bar");
    energyBar->setPercent(0);
    lotteryBtn = (Button*)BottomLayer->getChildByName("BoxButton");
    lotteryBtn->setScale(0.76);
    lotteryBtn->setEnabled(false);
    lotteryBtn->setTouchEnabled(false);
    lotteryBtn->setBright(false);
    lotteryBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickLotteryBox, this));
    for (int i = 1; i <= 5; i++) {
        std::string starBgName("user_star0_");
        starBgName += Convert2String(i);
        starSprite[i-1] = (Sprite*)BottomLayer->getChildByName("user_place_1")->getChildByName(starBgName)->getChildByName("user_star");
    }
    
//    HeadInfoLayer = CSLoader::createNode("game_scene/gamer_ui/user/HeadBar.csb");
//    HeadInfoLayer->setPosition(Vec2(0, GameUtils::winSize.height));
//    addChild(HeadInfoLayer);
//    HeadBg = HeadInfoLayer->getChildByName("head_bg");
    HeadBg = leftRoot->getChildByName("head_bg");
    expBar = (LoadingBar*)HeadBg->getChildByName("exp_bg")->getChildByName("exp_bar");
    expBar->setPercent(0);
    
    auto pad = (Sprite*)CannonUpgrade->getChildByName("ms_unlock_pad");
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameUILayer::onUpgradeClickBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameUILayer::onUpgradeClickEnd, this);
    
    pad->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pad);

    upgradeHideListener = EventListenerCustom::create(UI_HIDE_CANNON_LEVEL, CC_CALLBACK_0(GameUILayer::hideUpgrade, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(upgradeHideListener, -1);
    upgradeUpdateListener = EventListenerCustom::create(UI_UPDATE_CANNON_LEVEL, CC_CALLBACK_1(GameUILayer::updateUpgrade, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(upgradeUpdateListener, -1);
    upgradeUnlockListener = EventListenerCustom::create(UI_UNLOCK_CANNON_LEVEL, CC_CALLBACK_1(GameUILayer::unlockUpgrade, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(upgradeUnlockListener, -1);
    
    updateGoldListener = EventListenerCustom::create(UI_UPDATE_GOLD, CC_CALLBACK_1(GameUILayer::updateGold, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateGoldListener, -1);
    updateDiamondListener = EventListenerCustom::create(UI_UPDATE_DIAMOND, CC_CALLBACK_1(GameUILayer::updateDiamond, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateDiamondListener, -1);
    
    updateEnergyListener = EventListenerCustom::create(UI_UPDATE_ENERGY, CC_CALLBACK_1(GameUILayer::updateEnergy, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateEnergyListener, -1);
    
    updateHeadinfoListener = EventListenerCustom::create(UI_UPDATE_HEADINFO, CC_CALLBACK_1(GameUILayer::updateHeadinfo, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateHeadinfoListener, -1);
    
    updateBossTaskListener = EventListenerCustom::create(UI_UPDATE_BOSS_TASK, CC_CALLBACK_1(GameUILayer::updateBossTask, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateBossTaskListener, -1);
    
    initBossTaskListener = EventListenerCustom::create(UI_INIT_BOSS_TASK, CC_CALLBACK_1(GameUILayer::initBossTask, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(initBossTaskListener, -1);
    
    initTaskListener = EventListenerCustom::create(UI_INIT_TASK, CC_CALLBACK_1(GameUILayer::initTask, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(initTaskListener, -1);
    updateTaskListener = EventListenerCustom::create(UI_UPDATE_TASK, CC_CALLBACK_1(GameUILayer::updateTask, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateTaskListener, -1);
    
    updateSkillListener = EventListenerCustom::create(UI_UPDATE_SKILL_COUNT, CC_CALLBACK_1(GameUILayer::updateSkillCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateSkillListener, -1);
    useSkillListener = EventListenerCustom::create(UI_USE_SKILL, CC_CALLBACK_1(GameUILayer::useSkill, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(useSkillListener, -1);
//    showSkillChargeListener = EventListenerCustom::create(UI_SKILL_CHARGE, CC_CALLBACK_1(GameUILayer::showSkillCharge, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showSkillChargeListener, -1);
    hideSkillChargeListener = EventListenerCustom::create(UI_CHARGE_USE_SKILL, CC_CALLBACK_1(GameUILayer::hideSkillCharge, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideSkillChargeListener, -1);
    updateStarBoxListener = EventListenerCustom::create(UI_UPDATE_STAR_BOX, CC_CALLBACK_1(GameUILayer::updateStarBox, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateStarBoxListener, -1);
    canUseAutoFireListener = EventListenerCustom::create(UI_CAN_USE_AUTO_FIRE, CC_CALLBACK_0(GameUILayer::useAutoFire, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(canUseAutoFireListener, -1);
    
    scheduleUpdate();

    return true;
}

void GameUILayer::update(float dt)
{
    if(hideTick > 0)
    {
        hideTick -= dt;
        if (hideTick <= 0) {
            hideTick = 0;
            leftMenuAction->play("hideUnlockBoard", false);
        }
    }
}

void GameUILayer::clickWeapon(Ref* pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto changeWeapn = ChangeWeaponLayer::create();
            Director::getInstance()->getRunningScene()->addChild(changeWeapn);
        }
            break;
            
        default:
            break;
    }
}

void GameUILayer::clickAuto(Ref* pSender, Widget::TouchEventType type)
{
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            E2L_CHECK_AUTO_FIRE info;
            info.eProtocol = e2l_check_auto_fire;
            ClientLogic::instance()->ProcessUIRequest(&info);            
        }
            break;
            
        default:
            break;
    }
}

void GameUILayer::useAutoFire()
{
    auto autoBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("AutoBtn");
    autoFire = !autoFire;
    if (autoFire) {
        autoBtn->loadTextureNormal("common/element/element_1.png", Widget::TextureResType::LOCAL);
        EventCustom event(AUTO_FIRE);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        if (!SkillManager::instance()->skillMap[3].running) {
            EventCustom event(START_AUTO_CIRCLE);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }else{
        autoBtn->loadTextureNormal("common/element/element_0.png", Widget::TextureResType::LOCAL);
        EventCustom event(STOP_AUTO);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        EventCustom event1(STOP_AUTO_CIRCLE);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
    }
}

void GameUILayer::setAutoFire(bool bAutoFire)
{
    autoFire = bAutoFire;
    auto autoBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByName("AutoBtn");
    if (autoFire) {
        autoBtn->loadTextureNormal("common/element/element_1.png", Widget::TextureResType::LOCAL);
        if (!SkillManager::instance()->skillMap[3].running) {
            EventCustom event(START_AUTO_CIRCLE);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }else{
        autoBtn->loadTextureNormal("common/element/element_0.png", Widget::TextureResType::LOCAL);
        EventCustom event(STOP_AUTO_CIRCLE);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void GameUILayer::clickLock(Ref* pSender, Widget::TouchEventType type)
{
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            int btnTag = ((Button*)pSender)->getTag();
            
            E2L_USE_SKILL info;
            info.eProtocol = e2l_use_skill;
            info.skillId = btnTag;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}

void GameUILayer::clickSkillBtn(cocos2d::ui::Button *btn, float duration)
{
    switch (btn->getTag()) {
        case 1:
            AUDIO_PLAY("locking", AUDIO_TYPE::EFFECT_TYPE);
            break;
        case 3:
            AUDIO_PLAY("ballistic", AUDIO_TYPE::EFFECT_TYPE);
            break;
            
        default:
            break;
    }
    btn->setEnabled(false);
    auto func = CallFunc::create(std::bind(&GameUILayer::reopenButton, this, btn));
    auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100), ProgressTo::create(duration, 0));
    auto timeLeft = ProgressTimer::create(Sprite::create("game_scene/right_menu/rm_black.png"));
    timeLeft->setType( ProgressTimer::Type::RADIAL );
    timeLeft->setReverseDirection(true);
    timeLeft->setPosition(Vec2(btn->getContentSize().width/2, btn->getContentSize().height/2));
    btn->addChild(timeLeft);
    
    auto seq = Sequence::create(to1, func, NULL);
    timeLeft->runAction(seq);
}

void GameUILayer::reopenButton(cocos2d::Ref *pSender)
{
    int skillId = ((Button*)pSender)->getTag();
    ((Button*)pSender)->setEnabled(SkillManager::instance()->skillMap[skillId].work);
    SkillManager::instance()->endSkill(skillId);
    if (skillId == 1) {
        EventCustom event(CLEAR_LOCK_FISH);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }else if(skillId == 2){
        EventCustom event(UNFROZEN_SCREEN);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }else if (skillId == 3) {
        EventCustom event(END_RUSH_FIRE);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        if (autoFire) {
            EventCustom event(START_AUTO_CIRCLE);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }
    
    if (skillId <= 3) {
        bool tempWork = SkillManager::instance()->skillMap[1].running || SkillManager::instance()->skillMap[2].running || SkillManager::instance()->skillMap[3].running;
        
        if (!tempWork) {
            auto rayBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(4);
            rayBtn->setEnabled(SkillManager::instance()->skillMap[4].work);
            auto bombBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(5);
            bombBtn->setEnabled(SkillManager::instance()->skillMap[5].work);
        }
    }
    
}

void GameUILayer::reopenLockButton()
{
    lockSkillBtn->setEnabled(true);
}

void GameUILayer::clickIce(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
            clickSkillBtn((Button*)pSender, 13);
            break;
            
        default:
            break;
    }
}

void GameUILayer::clickRush(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    
}

void GameUILayer::clickRay(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    
}

void GameUILayer::clickBomb(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    
}

void GameUILayer::clickHide(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    uiAction->play("hide", false);
}

void GameUILayer::clickShow(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    uiAction->play("show", false);
}

void GameUILayer::clickSetting(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_SETTING info;
    info.eProtocol = e2l_show_settings;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameUILayer::clickSuc(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    EventCustom event(UI_NEXT_STAGE_DIALOG);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void GameUILayer::readyRay()
{
    for (int i = 1; i <= 5; i++) {
        auto Btn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(i);
        Btn->setEnabled(false);
    }
    readyRayFire = true;
}

void GameUILayer::endRay()
{
    for (int i = 1; i <= 5; i++) {
        if (i == 4) {
            continue;
        }
        auto Btn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(i);
        Btn->setEnabled(SkillManager::instance()->skillMap[i].work && !SkillManager::instance()->skillMap[i].running);
    }
    rayFiring = false;
    readyRayFire = false;
}

void GameUILayer::readyPlane()
{
    for (int i = 1; i <= 5; i++) {
        auto Btn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(i);
        Btn->setEnabled(false);
    }
    readyPlaneFire = true;
}

void GameUILayer::endPlane()
{
    for (int i = 1; i <= 5; i++) {
        if (i == 5) {
            continue;
        }
        auto Btn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(i);
        Btn->setEnabled(SkillManager::instance()->skillMap[i].work && !SkillManager::instance()->skillMap[i].running);
    }
    planeFiring = false;
    readyPlaneFire = false;
}

void GameUILayer::updateUpgrade(cocos2d::EventCustom *event)
{
    auto pad = (Sprite*)CannonUpgrade->getChildByName("ms_unlock_pad");
    auto multiLabel = (TextAtlas*)pad->getChildByName("NextMultiply");
        auto giveSpt = (Sprite*)pad->getChildByName("NextBoundGive");
        auto bounds = (TextAtlas*)pad->getChildByName("NextBounds");
        auto coinSpt = (Sprite*)pad->getChildByName("NextBoundCoin");
        auto maskSpt = (Sprite*)pad->getChildByName("ms_unlock_mask");
    auto barBg = (Sprite*)pad->getChildByName("ms_unlock_bar_bg");
    auto loadBar = (LoadingBar*)barBg->getChildByName("ms_unlock_bar");
    auto loadText = (TextAtlas*)barBg->getChildByName("ms_unlock_label");
    auto diamondSpt = (Sprite*)pad->getChildByName("ms_diamond");
    
    L2E_CANNON_UPDATE_LEVEL info = *static_cast<L2E_CANNON_UPDATE_LEVEL*>(event->getUserData());
    multiLabel->setString(Convert2String(info.nextMultiply));
    if(info.canUnlock)
    {
        bounds->setString(Convert2String(info.boundGolds));
        giveSpt->setVisible(true);
        bounds->setVisible(true);
        coinSpt->setVisible(true);
        maskSpt->setVisible(true);
        barBg->setVisible(false);
        diamondSpt->setVisible(false);
    } else {
        std::string temp(Convert2String(info.totalDiamond));
        temp += "/";
        temp += Convert2String(info.needDiamond);
        
        loadBar->setPercent(((float)info.totalDiamond)/info.needDiamond*100);
        loadText->setString(temp);
        
        giveSpt->setVisible(false);
        bounds->setVisible(false);
        coinSpt->setVisible(false);
        maskSpt->setVisible(false);
        barBg->setVisible(true);
        diamondSpt->setVisible(true);
    }
    CannonUpgrade->setVisible(true);
    
    if (!info.canUnlock) {
        if (!canUnlock && hideTick == 0) {
            pad->setVisible(true);
            leftMenuAction->play("showUnlockBoard", false);
        }
        hideTick = 15;
    } else {
        if (!canUnlock && hideTick == 0) {
            pad->setVisible(true);
            leftMenuAction->play("showUnlockBoard", false);
        }
        hideTick = 0;
    }
    canUnlock = info.canUnlock;
}

void GameUILayer::unlockUpgrade(cocos2d::EventCustom *event)
{
    L2E_CANNON_UNLOCK_LEVEL info = *static_cast<L2E_CANNON_UNLOCK_LEVEL*>(event->getUserData());
    if (info.errNo == 1) {
        return;
    }else if (info.errNo == 2)
    {
        EventCustom event1(UI_UNLOCK_TIP);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
        return;
    }
    
    auto pad = (Sprite*)CannonUpgrade->getChildByName("ms_unlock_pad");
    auto multiLabel = (TextAtlas*)pad->getChildByName("NextMultiply");
    auto giveSpt = (Sprite*)pad->getChildByName("NextBoundGive");
    auto bounds = (TextAtlas*)pad->getChildByName("NextBounds");
    auto coinSpt = (Sprite*)pad->getChildByName("NextBoundCoin");
    auto maskSpt = (Sprite*)pad->getChildByName("ms_unlock_mask");
    auto barBg = (Sprite*)pad->getChildByName("ms_unlock_bar_bg");
    auto loadBar = (LoadingBar*)barBg->getChildByName("ms_unlock_bar");
    auto loadText = (TextAtlas*)barBg->getChildByName("ms_unlock_label");
    auto diamondSpt = (Sprite*)pad->getChildByName("ms_diamond");
    

    multiLabel->setString(Convert2String(info.nextMultiply));
    if(info.canUnlock)
    {
        bounds->setString(Convert2String(info.boundGolds));
        giveSpt->setVisible(true);
        bounds->setVisible(true);
        coinSpt->setVisible(true);
        maskSpt->setVisible(true);
        barBg->setVisible(false);
        diamondSpt->setVisible(false);
    } else {
        std::string temp(Convert2String(info.totalDiamond));
        temp += "/";
        temp += Convert2String(info.needDiamond);
        
        loadBar->setPercent(((float)info.totalDiamond)/info.needDiamond*100);
        loadText->setString(temp);
        
        giveSpt->setVisible(false);
        bounds->setVisible(false);
        coinSpt->setVisible(false);
        maskSpt->setVisible(false);
        barBg->setVisible(true);
        diamondSpt->setVisible(true);
    }
    CannonUpgrade->setVisible(true);
    
    if (!info.canUnlock) {
        if (!canUnlock && hideTick == 0) {
            pad->setVisible(true);
            leftMenuAction->play("showUnlockBoard", false);
        }
        hideTick = 15;
    } else {
        if (!canUnlock && hideTick == 0) {
            pad->setVisible(true);
            leftMenuAction->play("showUnlockBoard", false);
        }
        hideTick = 0;
    }
    canUnlock = info.canUnlock;
    
    EventCustom event1(UI_UNLOCK_TIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
}

bool GameUILayer::onUpgradeClickBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto pad = (Sprite*)CannonUpgrade->getChildByName("ms_unlock_pad");
    if (!pad->isVisible()) {
        return false;
    }
    Vec2 locationInNode = this->convertToWorldSpace(touch->getLocation());
    auto iconPos = CannonUpgrade->getPosition()+Vec2(0, GameUtils::winSize.height/2);
    auto origin = this->convertToWorldSpace(pad->getPosition());
    origin += iconPos;
    Size s = pad->getContentSize();
    Rect rect = Rect(origin.x-s.width/2, origin.y-s.height/2, s.width, s.height);
    //
    if (rect.containsPoint(locationInNode)){
        AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
        log("click upgrade began");
        E2L_CANNON_UNLOCK_LEVEL info;
        info.eProtocol = e2l_cannon_unlock_level;
        ClientLogic::instance()->ProcessUIRequest(&info);
        return true;
        
    }
    return false;
}

void GameUILayer::onUpgradeClickEnd(cocos2d::Touch *touch, cocos2d::Event *event)
{
    log("click upgrade end");
}

void GameUILayer::updateGold(cocos2d::EventCustom *event)
{
    L2E_UPDATE_GOLD info = *static_cast<L2E_UPDATE_GOLD*>(event->getUserData());
    auto goldLabel = (TextAtlas*)BottomLayer->getChildByName("gold_bg")->getChildByName("gold_label");
    goldLabel->setString(Convert2String(info.gold));
}

void GameUILayer::updateDiamond(cocos2d::EventCustom *event)
{
    L2E_UPDATE_DIAMOND info = *static_cast<L2E_UPDATE_DIAMOND*>(event->getUserData());
    auto diamondLabel = (TextAtlas*)BottomLayer->getChildByName("diamond_bg")->getChildByName("diamond_label");
    diamondLabel->setString(Convert2String(info.diamond));
}

void GameUILayer::updateEnergy(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ENERGY info = *static_cast<L2E_UPDATE_ENERGY*>(event->getUserData());
    energyBar->setPercent((float)info.currVal*100.0/info.maxVal);
}

void GameUILayer::updateHeadinfo(cocos2d::EventCustom *event)
{
    L2E_UPDATE_HEADINFO info = *static_cast<L2E_UPDATE_HEADINFO*>(event->getUserData());
    expBar->setPercent((float)info.currExp*100/info.maxExp);
    auto levelLabel = (TextAtlas*)HeadBg->getChildByName("level_label");
    levelLabel->setString(Convert2String(info.level));
    auto titleLabel = (Text*)HeadBg->getChildByName("title_label");
    titleLabel->setString(info.title);
    
    auto vipLabel = (TextAtlas *)HeadBg->getChildByName("vip_bg")->getChildByName("vip_label");
    vipLabel->setString(Convert2String(info.vip));
}

void GameUILayer::initBossTask(cocos2d::EventCustom *event)
{
    L2E_INIT_BOSS_TASK info = *static_cast<L2E_INIT_BOSS_TASK*>(event->getUserData());
    std::string resDir("game_scene/fish/small_fish_alive/");
    int fishType = info.bossType;
    switch (fishType) {
        case SMALL_FISH:
        {
            resDir = "game_scene/fish/small_fish_alive/";
        }
        break;
        case TINY_BOSS_FISH:
        {
            resDir = "game_scene/fish/tiny_boss_fish_alive/";
        }
        break;
        case BOSS_FISH:
        {
            resDir = "game_scene/fish/boss_fish_alive/";
        }
        break;
        case SPECIAL_FISH:
        {
            resDir = "game_scene/fish/special_fish_alive/";
        }
        break;
        
        default:
        {
            resDir = "game_scene/fish/small_fish_alive/";
        }
        break;
    }
    resDir += info.bossRes;
    resDir += "_0.png";
    auto bossSpt = (Sprite*)bossTaskBg->getChildByName("boss_img_1");
    bossSpt->initWithFile(resDir);
    bossSpt->setScale(30/(float)bossSpt->getContentSize().height);
    
    fishType = info.subBossType;
    switch (fishType) {
        case SMALL_FISH:
        {
            resDir = "game_scene/fish/small_fish_alive/";
        }
        break;
        case TINY_BOSS_FISH:
        {
            resDir = "game_scene/fish/tiny_boss_fish_alive/";
        }
        break;
        case BOSS_FISH:
        {
            resDir = "game_scene/fish/boss_fish_alive/";
        }
        break;
        case SPECIAL_FISH:
        {
            resDir = "game_scene/fish/special_fish_alive/";
        }
        break;
        
        default:
        {
            resDir = "game_scene/fish/small_fish_alive/";
        }
        break;
    }
    resDir += info.subBossRes;
    resDir += "_0.png";
    auto subBossSpt = (Sprite*)bossTaskBg->getChildByName("boss_img_2");
    subBossSpt->initWithFile(resDir);
    subBossSpt->setScale(30/(float)subBossSpt->getContentSize().height);
    
    auto bossLabel = (TextAtlas*)bossTaskBg->getChildByName("boss_count_label_1");
    std::string temp(Convert2String(info.bossCount));
    temp += "/";
    temp += Convert2String(info.bossMaxCount);
    bossLabel->setString(temp);
    
    auto subBossLabel = (TextAtlas*)bossTaskBg->getChildByName("boss_count_label_2");
    std::string temp2(Convert2String(info.subBossCount));
    temp2 += "/";
    temp2 += Convert2String(info.subBossMaxCount);
    subBossLabel->setString(temp2);
    
    auto goldLabel = (TextAtlas*)bossTaskBg->getChildByName("bound_gold");
    goldLabel->setString(Convert2String(info.boundGold));
    
    auto diamondLabel = (TextAtlas*)bossTaskBg->getChildByName("bound_diamond");
    diamondLabel->setString(Convert2String(info.boundDiamond));
}

void GameUILayer::updateBossTask(cocos2d::EventCustom *event)
{
    L2E_UPDATE_BOSS_TASK info =  *static_cast<L2E_UPDATE_BOSS_TASK*>(event->getUserData());
    switch(info.targetState)
    {
        case STAGE_TARGET_BOUND:
        {
            bossTaskBtn->setVisible(true);
            bossTaskBg->setVisible(false);
            sucAction->play("next", true);
        }
        break;
        
        case STAGE_TARGET_OPEN:
        {
            auto bossLabel = (TextAtlas*)bossTaskBg->getChildByName("boss_count_label_1");
            std::string temp(Convert2String(info.bossCount));
            temp += "/";
            temp += Convert2String(info.bossMaxCount);
            bossLabel->setString(temp);
            
            auto subBossLabel = (TextAtlas*)bossTaskBg->getChildByName("boss_count_label_2");
            std::string temp2(Convert2String(info.subBossCount));
            temp2 += "/";
            temp2 += Convert2String(info.subBossMaxCount);
            subBossLabel->setString(temp2);
        }
        break;
        case STAGE_TARGET_CLOSE:
        {
            bossTaskBtn->setVisible(false);
            bossTaskBg->setVisible(false);
        }
            break;
    }
    
}

void GameUILayer::initTask(cocos2d::EventCustom *event)
{
    L2E_INIT_TASK info = *static_cast<L2E_INIT_TASK*>(event->getUserData());
    for (int i = 0; i < MAX_TASK_COUNT; i++) {
        std::string resDir("game_scene/fish/small_fish_alive/");
        int fishType = info.fishType[i];
        switch (fishType) {
            case SMALL_FISH:
            {
                resDir = "game_scene/fish/small_fish_alive/";
            }
                break;
            case TINY_BOSS_FISH:
            {
                resDir = "game_scene/fish/tiny_boss_fish_alive/";
            }
                break;
            case BOSS_FISH:
            {
                resDir = "game_scene/fish/boss_fish_alive/";
            }
                break;
            case SPECIAL_FISH:
            {
                resDir = "game_scene/fish/special_fish_alive/";
            }
                break;
                
            default:
            {
                resDir = "game_scene/fish/small_fish_alive/";
            }
                break;
        }
        resDir += info.fishRes[i];
        resDir += "_0.png";
        auto fishSpt = (Sprite*)showTaskBtn->getChildByName("fish_img_"+Convert2String(i+1));
        fishSpt->initWithFile(resDir);
        fishSpt->setScale(30/(float)fishSpt->getContentSize().height);
        
        auto countLabel = (TextAtlas*)showTaskBtn->getChildByName("fish_count_"+Convert2String(i+1));
        std::string temp(Convert2String(info.fishCount[i]));
        temp += "/";
        temp += Convert2String(info.fishMaxCount[i]);
        countLabel->setString(temp);
    }
    
}

void GameUILayer::updateTask(cocos2d::EventCustom *event)
{
    L2E_UPDATE_TASK info = *static_cast<L2E_UPDATE_TASK*>(event->getUserData());
    switch (info.taskState) {
        case TASK_OPEN:
        {
            for (int i = 0; i < MAX_TASK_COUNT; i++) {
                auto countLabel = (TextAtlas*)showTaskBtn->getChildByName("fish_count_"+Convert2String(i+1));
                std::string temp(Convert2String(info.fishCount[i]));
                temp += "/";
                temp += Convert2String(info.fishMaxCount[i]);
                countLabel->setString(temp);
            }
        }
            break;
        case TASK_BOUND:
        {
            showTaskBtn->setVisible(false);
            showTaskBoundBtn->setVisible(true);
            taskBoundAction->play("play", true);
        }
            break;
        case TASK_CLOSE:
        {
            showTaskBtn->setVisible(false);
            showTaskBoundBtn->setVisible(false);
            taskBoundAction->stop();
            showTaskBoundBtn->stopAllActions();
        }
            break;
            
        default:
            break;
    }
}

void GameUILayer::clickShowTask(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_TASK_BOUND info;
    info.eProtocol = e2l_show_task_bounds;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameUILayer::clickShowUnlock(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
//    if (!canUnlock) {
        auto pad = (Sprite*)CannonUpgrade->getChildByName("ms_unlock_pad");
        if (hideTick == 0) {
            hideTick = 15;
            pad->setVisible(true);
            leftMenuAction->play("showUnlockBoard", false);
        }else{
            hideTick = 0;
            leftMenuAction->play("hideUnlockBoard", false);
        }
        
//    }
}

void GameUILayer::updateSkillCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_SKILL info = *static_cast<L2E_UPDATE_SKILL*>(event->getUserData());
    for (int i = 0; i < 5; i++) {
        int skillId = i+1;
        auto Btn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(skillId);
        auto countLabel = (TextAtlas*)Btn->getChildByName("count_label");
        auto diamondSpt = Btn->getChildByName("diamond");
        auto diamondLabel = (TextAtlas*)Btn->getChildByName("diamond_label");
        if (info.skillCount[i] > 0) {
            countLabel->setVisible(true);
            countLabel->setString(Convert2String(info.skillCount[i]));
            diamondSpt->setVisible(false);
            diamondLabel->setVisible(false);
        } else if(info.skillCount[i] == -1) {
            countLabel->setVisible(false);
//            countLabel->setString(Convert2String(info.skillCount[i]));
            diamondSpt->setVisible(false);
            diamondLabel->setVisible(false);
        }else{
            countLabel->setVisible(false);
            diamondSpt->setVisible(true);
            diamondLabel->setVisible(true);
            diamondLabel->setString(Convert2String(info.skillPrice[i]));
        }
    }
    

}

void GameUILayer::useSkill(cocos2d::EventCustom *event)
{
    L2E_USE_SKILL info = *static_cast<L2E_USE_SKILL*>(event->getUserData());
    auto btn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(info.useSkillId);
    clickSkillBtn(btn, info.cd);
    //            if (btnTag >= 1 && btnTag <= 3) {
    auto rayBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(4);
    rayBtn->setEnabled(false);
    auto bombBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(5);
    bombBtn->setEnabled(false);
    //            }
    
}

void GameUILayer::showSkillCharge(cocos2d::EventCustom *event)
{
//    confirmLayer = LayerColor::create(Color4B(10, 10, 10, 128));
//    auto callback = [](Touch * ,Event *)
//    {
//        return true;
//    };
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = callback;
//    listener->setSwallowTouches(true);
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,confirmLayer);
//    
//    auto root = CSLoader::createNode(NEXT_STAGE_UI);
//    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
//    confirmLayer->addChild(root);
//    addChild(confirmLayer);
//    auto tipLabel = (Text*)root->getChildByName("Text_1");
//    auto okBtn = (Button*)root->getChildByName("ok_btn");
//    auto cancelBtn = (Button*)root->getChildByName("cancel_btn");
//    tipLabel->setString("升级VIP可获得新的捕鱼神器！");
//    okBtn->addClickEventListener(CC_CALLBACK_0(ChangeCannonLayer::upgradeVip, this));
//    cancelBtn->addClickEventListener(CC_CALLBACK_0(ChangeCannonLayer::removeConfirm, this));
    L2E_SKILL_CHARGE info = *static_cast<L2E_SKILL_CHARGE*>(event->getUserData());
    chargeSkillId = info.skillId;
    tipNode = CSLoader::createNode(COMMON_TIP_UI);
    tipNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tipNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tipNode);
    
    auto text1 = (Text*)tipNode->getChildByName("Text_1");
    auto text2 = (Text*)tipNode->getChildByName("Text_2");
    text1->setString("Congratulations，you get 10 diamonds！");
    text2->setVisible(false);
//    auto boundText = (TextAtlas*)card[0]->getChildByName("Bounds");
//    std::string boundStr("获得奖励金币");
//    boundStr += boundText->getString();
//    text2->setString(boundStr);
    
    auto okBtn = (Button*)tipNode->getChildByName("ButtonOk");
    okBtn->addClickEventListener(CC_CALLBACK_1(GameUILayer::clickChargeOk, this));
}

void GameUILayer::clickChargeOk(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CHARGE_SKILL info;
    info.eProtocol = e2l_charge_skill;
    info.skillId = chargeSkillId;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameUILayer::hideSkillCharge(cocos2d::EventCustom *event)
{
    removeChild(tipNode);
    L2E_CHARGE_USE_SKILL info = *static_cast<L2E_CHARGE_USE_SKILL*>(event->getUserData());
    auto btn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(info.useSkillId);
    clickSkillBtn(btn, SkillManager::instance()->skillMap[info.useSkillId].coolDuration);
    //            if (btnTag >= 1 && btnTag <= 3) {
    auto rayBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(4);
    rayBtn->setEnabled(false);
    auto bombBtn = (Button*)MenuLayer->getChildByName("rm_right_menu_1")->getChildByTag(5);
    bombBtn->setEnabled(false);
}

void GameUILayer::clickLotteryBox(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_LUCKY_DRAW info;
    info.eProtocol = e2l_show_lucky_draw;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameUILayer::updateStarBox(cocos2d::EventCustom *event)
{
    L2E_UPDATE_BIG_STAR info = *static_cast<L2E_UPDATE_BIG_STAR*>(event->getUserData());
    for (int i = 1; i <= 5; i++) {
        starSprite[i-1]->setVisible(i<=info.starCount);
    }
    lotteryBtn->setEnabled(info.boxEnable);
    lotteryBtn->setTouchEnabled(info.boxEnable);
    lotteryBtn->setBright(info.boxEnable);
    if (info.boxEnable) {
        Vector<FiniteTimeAction*> scaleVec;
        scaleVec.pushBack(ScaleTo::create(0.4, 1.2));
        scaleVec.pushBack(ScaleTo::create(0.2, 0.8));
        scaleVec.pushBack(ScaleTo::create(0.4, 1.2));
        scaleVec.pushBack(ScaleTo::create(0.2, 0.8));
        scaleVec.pushBack(ScaleTo::create(0.4, 1.2));
        scaleVec.pushBack(ScaleTo::create(0.2, 0.8));
        scaleVec.pushBack(ScaleTo::create(0.1, 1.5));
        scaleVec.pushBack(ScaleTo::create(0.1, 0.8));
        auto seq = Sequence::create(scaleVec);
        lotteryBtn->runAction(RepeatForever::create(seq));
    } else {
        lotteryBtn->stopAllActions();
        lotteryBtn->setScale(0.76);
    }
}

void GameUILayer::clickRechargeBtn()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_GAME_RECHARGE info;
    info.eProtocol = e2l_show_game_recharge;
    info.type = 1;
    ClientLogic::instance()->ProcessUIRequest(&info);
}