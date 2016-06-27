//
//  GameScene.cpp
//  buyu_simple
//
//  Created by ff on 15/12/9.
//
//

#include "GameScene.h"
#include "utils/GameUtils.h"
#include "../UIManager.h"
#include "logic/ClientLogic.h"
#include "GameDef.h"
#include "../node/FishNode.h"
#include "../node/BulletNode.h"
#include "../node/GoldFallen.h"
#include "utils/MathAide.h"
#include "data/SkillManager.h"
#include "sound/SoundManager.h"

USING_NS_CC;

GameScene::~GameScene()
{
    unschedule("tide");
    unscheduleUpdate();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
//    Director::getInstance()->getEventDispatcher()->removeEventListener(tideListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(tideTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(tideSwitchListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(stopAutoListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startAutoListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(shakeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(iceListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(unfrozenListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(lockListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(stopLockListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(readyRayListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(endRayListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(rayAttackListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(readyPlaneListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(endPlaneListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(planeAttackListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(enterNiuNiuListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(enterMaryListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showUpgradeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideUpgradeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(compBossTaskListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(nextStageDialogListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showTaskBoundListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideSettingListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideNiuNiuListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideMaryListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showNiuNiuSplashListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showMarySplashListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(takeTaskBoundListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideTaskBoundListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(enterLuckyDrawListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(exitLuckyDrawListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(enterLotteryListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(endLotteryListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(bossTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showSettingListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(multiNotEnoughListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideMultiNotEnoughListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(lackGoldListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(rayNeedVipTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(autoFireNeedVipTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(rechargeOkListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showRechargeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showVipIntroListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showVipGiftListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(closeRechargeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(bombNeedUnlockListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(skillNeedDiamondListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(unlockNeedDiamondListener);
}

bool GameScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    fishLayer = nullptr;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::touchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::touchMove, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::touchScreen, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScene::touchScreen, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
//    tideListener = EventListenerCustom::create(SCHEDULE_TIDE, CC_CALLBACK_1(GameScene::scheduleTide, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(tideListener, -1);
    tideTipListener = EventListenerCustom::create(TIDE_TIP, CC_CALLBACK_1(GameScene::showTideTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(tideTipListener, -1);
    tideSwitchListener = EventListenerCustom::create(TIDE_SWITCH, CC_CALLBACK_1(GameScene::switchTideBg, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(tideSwitchListener, -1);
    stopAutoListener = EventListenerCustom::create(STOP_AUTO, CC_CALLBACK_0(GameScene::stopAutoFire, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(stopAutoListener, -1);
    startAutoListener = EventListenerCustom::create(AUTO_FIRE, CC_CALLBACK_0(GameScene::scheduleAutoFire, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startAutoListener, -1);
    
    lockListener = EventListenerCustom::create(TRACE_LOCK_FISH, CC_CALLBACK_0(GameScene::scheduleTraceFire, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(lockListener, -1);
    stopLockListener = EventListenerCustom::create(STOP_LOCK_FIRE, CC_CALLBACK_0(GameScene::stopTraceFire, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(stopLockListener, -1);
    
    shakeListener = EventListenerCustom::create(SHAKE_SCREEN, CC_CALLBACK_0(GameScene::shakeScreen, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(shakeListener, -1);
    
    iceListener = EventListenerCustom::create(ICE_SCREEN, CC_CALLBACK_0(GameScene::iceScreen, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(iceListener, -1);
    unfrozenListener = EventListenerCustom::create(UNFROZEN_SCREEN, CC_CALLBACK_0(GameScene::iceEnd, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(unfrozenListener, -1);
    bossTipListener = EventListenerCustom::create(BOSS_TIP, CC_CALLBACK_1(GameScene::showBossTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(bossTipListener, -1);
    
    readyRayListener = EventListenerCustom::create(UI_READY_RAY, CC_CALLBACK_0(GameScene::readyRay, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(readyRayListener, -1);
    endRayListener = EventListenerCustom::create(UI_END_RAY, CC_CALLBACK_0(GameScene::endRay, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(endRayListener, -1);
    
    rayAttackListener = EventListenerCustom::create(RAY_ATTACK, CC_CALLBACK_1(GameScene::rayAttack, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(rayAttackListener, -1);
    
    readyPlaneListener = EventListenerCustom::create(UI_READY_PLANE, CC_CALLBACK_0(GameScene::readyPlane, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(readyPlaneListener, -1);
    endPlaneListener = EventListenerCustom::create(UI_END_PLANE, CC_CALLBACK_0(GameScene::endPlane, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(endPlaneListener, -1);
    
    planeAttackListener = EventListenerCustom::create(PLANE_ATTACK, CC_CALLBACK_1(GameScene::bombAttack, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(planeAttackListener, -1);
    showNiuNiuSplashListener = EventListenerCustom::create(UI_SPLASH_NIUNIU, CC_CALLBACK_0(GameScene::splashNiuNiu, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showNiuNiuSplashListener, -1);
    enterNiuNiuListener= EventListenerCustom::create(UI_ENTER_NIUNIU, CC_CALLBACK_1(GameScene::enterNiuNiu, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(enterNiuNiuListener, -1);
    hideNiuNiuListener = EventListenerCustom::create(UI_HIDE_NIUNIU, CC_CALLBACK_0(GameScene::exitNiuNiu, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideNiuNiuListener, -1);
    
    showMarySplashListener = EventListenerCustom::create(UI_SPLASH_MARY, CC_CALLBACK_0(GameScene::splashMary, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showMarySplashListener, -1);
    enterMaryListener = EventListenerCustom::create(UI_ENTER_MARY, CC_CALLBACK_1(GameScene::enterMary, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(enterMaryListener, -1);
    hideMaryListener = EventListenerCustom::create(UI_HIDE_MARY, CC_CALLBACK_0(GameScene::exitMary, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideMaryListener, -1);
    showUpgradeListener = EventListenerCustom::create(UI_SHOW_CANNON_UNLOCK_DIALOG, CC_CALLBACK_1(GameScene::showUpgradeLevel, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showUpgradeListener, -1);
    hideUpgradeListener = EventListenerCustom::create(UI_HIDE_UNLOCK_LEVEL, CC_CALLBACK_1(GameScene::hideUpgradeLevel, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideUpgradeListener, -1);
    
    showSettingListener = EventListenerCustom::create(UI_SHOW_SETTING, CC_CALLBACK_0(GameScene::showSetting, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showSettingListener, -1);
    hideSettingListener = EventListenerCustom::create(UI_HIDE_SETTING, CC_CALLBACK_0(GameScene::exitSetting, this) );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideSettingListener, -1);
    
    compBossTaskListener = EventListenerCustom::create(UI_COMP_BOSS_TASK, CC_CALLBACK_1(GameScene::completeBossTask, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(compBossTaskListener, -1);
    
    nextStageDialogListener = EventListenerCustom::create(UI_NEXT_STAGE_DIALOG, CC_CALLBACK_0(GameScene::showNextStage, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(nextStageDialogListener, -1);

    showTaskBoundListener = EventListenerCustom::create(UI_SHOW_TASK_BOUND, CC_CALLBACK_1(GameScene::showTaskBoundDialog, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showTaskBoundListener, -1);
    takeTaskBoundListener = EventListenerCustom::create(UI_TAKE_TASK_BOUND, CC_CALLBACK_0(GameScene::takeTaskBound, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(takeTaskBoundListener, -1);
    hideTaskBoundListener = EventListenerCustom::create(UI_HIDE_TASK_BOUND, CC_CALLBACK_0(GameScene::hideTaskBound, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideTaskBoundListener, -1);
    enterLuckyDrawListener = EventListenerCustom::create(UI_ENTER_LUCKY_DRAW, CC_CALLBACK_1(GameScene::enterLuckyDraw, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(enterLuckyDrawListener, -1);
    exitLuckyDrawListener = EventListenerCustom::create(UI_EXIT_LUCKY_DRAW, CC_CALLBACK_0(GameScene::exitLuckyDraw, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(exitLuckyDrawListener, -1);
    enterLotteryListener = EventListenerCustom::create(UI_ENTER_LOTTERY, CC_CALLBACK_1(GameScene::enterLottery, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(enterLotteryListener, -1);
    endLotteryListener = EventListenerCustom::create(UI_END_LOTTERY, CC_CALLBACK_0(GameScene::endLottery, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(endLotteryListener, -1);
    multiNotEnoughListener = EventListenerCustom::create(UI_MULTIPLY_NOT_ENOUGH, CC_CALLBACK_1(GameScene::showMultiplyNotEnough, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(multiNotEnoughListener, -1);
    hideMultiNotEnoughListener = EventListenerCustom::create(UI_HIDE_MULTIPLY_NOT_ENOUGH, CC_CALLBACK_1(GameScene::hideMultiplyNotEnough, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideMultiNotEnoughListener, -1);
    lackGoldListener = EventListenerCustom::create(UI_LACK_GOLD_TIP, CC_CALLBACK_0(GameScene::showLackGoldTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(lackGoldListener, -1);
    rayNeedVipTipListener = EventListenerCustom::create(UI_SHOW_RAY_NEED_VIP_TIP, CC_CALLBACK_1(GameScene::showRayNeedVipTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(rayNeedVipTipListener, -1);
    autoFireNeedVipTipListener = EventListenerCustom::create(UI_SHOW_AUTO_FIRE_NEED_VIP_TIP, CC_CALLBACK_1(GameScene::showAutoFireNeedVipTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(autoFireNeedVipTipListener, -1);
    rechargeOkListener = EventListenerCustom::create(UI_RECHARGE_OK, CC_CALLBACK_1(GameScene::rechargeOk, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(rechargeOkListener, -1);
    showRechargeListener = EventListenerCustom::create(UI_SHOW_HALL_RECHARGE, CC_CALLBACK_1(GameScene::showRecharge, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showRechargeListener, -1);
    showVipIntroListener = EventListenerCustom::create(UI_SHOW_VIP_INTRO, CC_CALLBACK_1(GameScene::showVipIntro, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showVipIntroListener, -1);
    showVipGiftListener = EventListenerCustom::create(UI_SHOW_VIP_GIFT, CC_CALLBACK_0(GameScene::showVipGift, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showVipGiftListener, -1);
    closeRechargeListener = EventListenerCustom::create(UI_CLOSE_RECHARGE, CC_CALLBACK_0(GameScene::closeRecharge, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(closeRechargeListener, -1);
    bombNeedUnlockListener = EventListenerCustom::create(UI_SHOW_BOMB_NEED_UNLOCK_TIP, CC_CALLBACK_0(GameScene::showBombNeedUnlockTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(bombNeedUnlockListener, -1);
    skillNeedDiamondListener = EventListenerCustom::create(UI_SKILL_CHARGE, CC_CALLBACK_0(GameScene::showSkillNeedDiamondTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(skillNeedDiamondListener, -1);
    unlockNeedDiamondListener = EventListenerCustom::create(UI_SHOW_UNLOCK_NEED_DIAMOND_TIP, CC_CALLBACK_0(GameScene::showUnlockNeedDiamondTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(unlockNeedDiamondListener, -1);

    offsetX = offsetY = 0;
    shake = false;
    shakeDuration = 0;
    noCollide = false;

    std::string musicName("bg");
    musicName += Convert2String(abs(rand())%2+1);
    AUDIO_PLAY(musicName, AUDIO_TYPE::MUSIC_TYPE);
    return true;
}

void GameScene::setupView()
{
    updateScene(sceneId);
    
    addChild(bgSprite);
    scheduleUpdate();
    
    auto sunLighLayer = Layer::create();
    int row = GameUtils::winSize.height/256+2;
    int col = GameUtils::winSize.width/256+2;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            auto centerNode = CSLoader::createNode(SUN_LIGHT_EFFECT);
            auto centerAction = CSLoader::createTimeline(SUN_LIGHT_EFFECT);
            centerNode->runAction(centerAction);
            centerNode->setPosition(GameUtils::winSize.width/2-256*(col/2-j), GameUtils::winSize.height/2-256*(row/2-i));
            centerAction->play("play", true);
            sunLighLayer->addChild(centerNode);
        }
    }
    addChild(sunLighLayer);
    
    fishLayer = FishLayer::create();
    cocos2d::Node::addChild(fishLayer);
    
    auto winSize = Director::getInstance()->getWinSize();
    tideTipSprite = Sprite::create("game_scene/game_effect/game_effect/ge_tide_coming.png");
    tideTipSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
    tideTipSprite->setVisible(false);
    addChild(tideTipSprite);
    
    bossTipSprite = Sprite::create("game_scene/game_effect/game_effect/ge_boss_coming.png");
    bossTipSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
    bossTipSprite->setVisible(false);
    addChild(bossTipSprite);
    bossTipFaceSprite = Sprite::create("game_scene/fish/boss_fish_alive/afish_41_0.png");
    bossTipFaceSprite->setPosition(Vec2(winSize.width/2-200, winSize.height/2));
    bossTipFaceSprite->setVisible(false);
    addChild(bossTipFaceSprite);
    
    warningSprite = Sprite::create("game_scene/game_effect/game_effect/ge_warning.png");
    warningSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
    warningSprite->setVisible(false);
    addChild(warningSprite);
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("game_scene/game_effect/game_effect/ge_water.plist");
    
    Vector<SpriteFrame*> animFrames(2);
    char str[100] = {0};
    for(int i = 1; i < 3; i++)
    {
        sprintf(str, "game_scene/game_effect/game_effect/ge_water%d.png",i);
        auto frame = cache->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    // Add an animation to the Cache
    tideWaveSprite = Sprite::createWithSpriteFrameName("game_scene/game_effect/game_effect/ge_water1.png");
    tideWaveSprite->runAction(RepeatForever::create(Animate::create(animation)));
    tideWaveSprite->setScale(2);
    tideWaveSprite->setAnchorPoint(Vec2(0.1, 0.43));
    tideWaveSprite->setPosition(Vec2(0, winSize.height/2));
    
    std::string frameName("game_scene/game_background/game_background_");
    frameName += Convert2String(sceneId);
    frameName += ".png";
    waveBgSprite = Sprite::create(frameName);
    waveBgSprite->setAnchorPoint(Vec2(0, 0.5));
    waveBgSprite->setPosition(Vec2(winSize.width+waveBgSprite->getContentSize().width/2, winSize.height/2));
    waveBgSprite->setVisible(false);
    waveBgSprite->setScale(winSize.width/1074, winSize.height/740);
    addChild(waveBgSprite);
    
    waveBgSprite->addChild(tideWaveSprite);
    
    bulletLayer = BulletLayer::create();
    addChild(bulletLayer);
    
    netLayer = NetLayer::create();
    addChild(netLayer);
    
    coinLayer = FishCoinLayer::create();
    addChild(coinLayer);
    
    lightLayer = LightEffectLayer::create();
    addChild(lightLayer);
    
    dropLayer = DropItemLayer::create();
    addChild(dropLayer);
    
    
    iceSprite = CSLoader::createNode(ICE_SCREEN_EFFECT);
    iceSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    iceSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(iceSprite);
    
    iceAction = CSLoader::createTimeline(ICE_SCREEN_EFFECT);
//    iceAction->setLastFrameCallFunc(CC_CALLBACK_0(GameScene::iceEnd, this));
    iceAction->pause();
    iceSprite->runAction(iceAction);
    iceSprite->setVisible(false);
    
    rayTipSprite = Sprite::create("game_scene/right_menu/rm_text0.png");
    rayTipSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
    rayTipSprite->setVisible(false);
    addChild(rayTipSprite);
    
    readyRayCount = cocos2d::ui::TextAtlas::create("0123456789", "game_scene/right_menu/game_number_2.png",33, 42, "0");
    readyRayCount->setString("3");
    readyRayCount->setPosition(Vec2(150, 30));
    rayTipSprite->addChild(readyRayCount);
    
    planeNetSprite = Sprite::create("game_scene/right_menu/rm_net.png");
    planeNetSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
    planeNetSprite->setVisible(false);
    addChild(planeNetSprite);
    
    planeTipSprite = Sprite::create("game_scene/right_menu/rm_text1.png");
    planeTipSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
    planeTipSprite->setVisible(false);
    addChild(planeTipSprite);
    
    readyPlaneCount = ui::TextAtlas::create("0123456789", "game_scene/right_menu/game_number_2.png", 33, 42, "0");
    readyPlaneCount->setString("10");
    readyPlaneCount->setPosition(Vec2(150, 30));
    planeTipSprite->addChild(readyPlaneCount);
    
    raidSprite = CSLoader::createNode(BOMB_RAID_ANIM);
    raidSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    raidSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(raidSprite);
    
    raidAction = CSLoader::createTimeline(BOMB_RAID_ANIM);
    raidAction->pause();
    raidSprite->runAction(raidAction);
    raidSprite->setVisible(false);
    
    planeShadowSprite = Sprite::create("game_scene/right_menu/rm_plane.png");
    planeShadowSprite->setVisible(false);
    addChild(planeShadowSprite);

    uiLayer = GameUILayer::create();
    addChild(uiLayer);
    
    cannonLayer = CannonLayer::create();
    addChild(cannonLayer);
    
    tipLayer = TipLayer::create();
    addChild(tipLayer,10000);
    
    changeGameState(UI_FISH_GAME_NORMAL);
}

void GameScene::changeGameState(UI_FISH_GAME_STATE nextState)
{
    if(currState == nextState)
        return;
    exitGameState(currState);
    lastState	= currState;
    currState		= nextState;
    enterGameState(nextState);
}

void GameScene::enterGameState(UI_FISH_GAME_STATE nextState)
{
    switch (nextState) {
        case UI_FISH_GAME_NORMAL:
        {
            if (fishLayer->lockFish == nullptr) {
                if (uiLayer->getAutoFire()) {
                    EventCustom event(AUTO_FIRE);
                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    if (!SkillManager::instance()->skillMap[3].running) {
                        EventCustom event(START_AUTO_CIRCLE);
                        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                    }
                }
            }
            
            
        }
            break;
        case UI_FISH_GAME_SETTINGS:
        {
            bulletLayer->pauseAll();
            stopActionByTag(102);
            stopAutoFire();
        }
            break;
        default:
            break;
    }
}

void GameScene::exitGameState(UI_FISH_GAME_STATE state)
{
    switch (state) {
        case UI_FISH_GAME_SETTINGS:
        {
            bulletLayer->resumeAll();
        }
            break;
            
        default:
            break;
    }
}

void GameScene::updateScene(int sceneIndex)
{
    sceneId = sceneIndex;
    auto winSize = Director::getInstance()->getWinSize();
    std::string frameName("game_scene/game_background/game_background_");
    frameName += Convert2String(sceneId);
    frameName += ".png";
    bgSprite = Sprite::create(frameName);
    bgSprite->setScale(winSize.width/1074, winSize.height/740);
    bgSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
}

void GameScene::update(float dt)
{
    if (currState == UI_FISH_GAME_NORMAL) {
        if(shake){
            offsetX = rand() % 2 == 0 ? (10.f + rand()%5) : (-10.f - rand()%5);
            offsetY = rand() % 2 == 1 ? (10.f + rand()%5) : (-10.f - rand()%5);
            auto winSize = Director::getInstance()->getWinSize();
            shakeDuration -= dt;
            if (shakeDuration <= 0) {
                shakeDuration = 0;
                shake = false;
                offsetX = offsetY = 0;
            }
            
            bgSprite->setPosition(Vec2(winSize.width/2+offsetX, winSize.height/2+offsetY));
            bulletLayer->setPosition(Vec2(offsetX, offsetY));
            fishLayer->setPosition(Vec2(offsetX, offsetY));
        }

        
        if (fishLayer != nullptr)
        {
            fishLayer->update(dt);
            if (fishLayer->lockFish != nullptr) {
                Vec2 vec2 = Director::getInstance()->convertToGL(fishLayer->lockFish->getPosition());
                cannonLayer->heroTowardPoint(vec2);
                float dis = cannonLayer->getHeroCannon()->getPosition().getDistance(vec2);
                cannonLayer->getHeroCannon()->setLock(true, dis);
            }
        }
        
        if (bulletLayer != nullptr)
        {

            bulletLayer->update(dt, fishLayer->getLockFishId());
            if (fishLayer->getLockFishId() != -1) {
    //            Vec2 vec2 = Director::getInstance()->convertToGL(fishLayer->lockFish->getPosition());
                Vec2 vec2 = fishLayer->lockFish->getPosition();
                bulletLayer->updateTrace(vec2);
            }
        }
        
        if (!fishLayer->lightFishes.empty()) {
            if (fishLayer->lightFishes.size() > 1) {
                Vec2 pos1 =fishLayer->lightFishes.at(0)->getPosition();
                for (int i = 1; i < fishLayer->lightFishes.size(); i++) {
                    Vec2 pos2 = fishLayer->lightFishes.at(i)->getPosition();
                    Vec2 lightPos = pos1.getMidpoint(pos2);
                    
                    float dis = pos1.distance(pos2);
                    float angle = CC_RADIANS_TO_DEGREES((pos1-pos2).getAngle());
                    lightLayer->addLight(lightPos, dis, -angle);
                    pos1 = pos2;
                }
                
                for (int i = 1; i < fishLayer->lightFishes.size(); i++) {
                    Vec2 pos2 = fishLayer->lightFishes.at(i)->getPosition();
                    lightLayer->addLightDot(pos2);
                }
            }
            
            fishLayer->lightFishes.clear();
        }
        if (coinLayer != nullptr) {
            coinLayer->update(dt);
        }
        
        if (!noCollide) {
            auto fishs = fishLayer->getChildren();
            auto bullets = bulletLayer->getChildren();
            
            
            collideBulletId.clear();
            for (auto it : fishs) {
                auto fish = (FishNode*)it;
                if (!fish->getAlive() || fish->dying) {
                    continue;
                }
        //        bool found = false;
                
                for (auto bulletItr : bullets) {
                    auto bullet = (BulletNode*)bulletItr;
                    if (!bullet->getAlive()) {
                        continue;
                    }
                    if (bullet->getBulletType() != 1) {
                        continue;
                    }
                    if (bullet->getTraceFishId() != -1) {
                        if (bullet->getTraceFishId() != fish->getFishId()) {
                            continue;
                        }
                        if(fish->getPosition().getDistance(bullet->getPosition()) < 60)
                        {
                            collideBulletId.push_back(bullet->getBulletId());
                            bullet->syncCurrPos();
                        }
                    }else{
                        if(fish->pointHitTest(bullet->getPosition()))
                        {
            //                found = true;
                            collideBulletId.push_back(bullet->getBulletId());
                            bullet->syncCurrPos();
                        }                    
                    }
                    

                }
        //        if (found) {
        //            bulletFishId.push_back(fish->getFishId());
        //        }
            }
            
            bulletFishId.clear();
            for (auto colBullet : collideBulletId) {
                auto bullet = static_cast<BulletNode*>(bulletLayer->getChildByTag(colBullet));
                int count = 0;
                for (auto fishIt : fishs) {
                    auto fish = (FishNode*)fishIt;
                    if (!fish->getAlive() || fish->dying) {
                        continue;
                    }
                    if (bullet->getTraceFishId() != -1) {
                        if (bullet->getTraceFishId() != fish->getFishId()) {
                            continue;
                        }
                    }
                    
                    if (fish->circleHitTest(bullet->getPosition(), bullet->getRadius()))
                    {
                        count++;
                        bulletFishId[fish->getFishId()] = bullet->getBulletId();
                    }
                    if (count > MAX_FISH_PER_BULLET) {
                        break;
                    }
                }
            }
            if (!collideBulletId.empty()) {
        //        EventCustom event(HIT_FISHES);
        //        event.setUserData(&bulletFishId);
        //        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                
                EventCustom event1(BULLET_HIT);
                event1.setUserData(&collideBulletId);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
            }
            
            if (!bulletFishId.empty()) {
                EventCustom event(HIT_FISHES);
                event.setUserData(&bulletFishId);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }

        }
        
        if (uiLayer->getRayFiring()) {
            readyRayDuration = -1;
        }
        if (uiLayer->getPlaneFiring()) {
            readyPlaneDuration = -1;
        }
        
        if (uiLayer->getReadyRayFire() && !uiLayer->getRayFiring()) {
            if (readyRayDuration <= 0) {
                autoRay();
            }
            readyRayDuration -= dt;
            readyRayCount->setString(Convert2String((int)readyRayDuration+1));
        }
        if (uiLayer->getReadyPlaneFire() && !uiLayer->getPlaneFiring()) {
            if (readyPlaneDuration <= 0) {
                autoPlane();
            }
            readyPlaneDuration -= dt;
            readyPlaneCount->setString(Convert2String((int)readyPlaneDuration+1));
        }
    }
    
    
    UIManager::instance()->loop(dt);
}

void GameScene::bombAttack(cocos2d::EventCustom *event)
{
    if (noCollide)
    {
        return;
    }
    
    shakeScreen();
    
    //....
    BulletNode *bullet = (BulletNode*)event->getUserData();
    auto fishs = fishLayer->getChildren();
    
    bulletFishId.clear();
    for (auto it : fishs)
    {
        auto fish = (FishNode*)it;
        if (!fish->getAlive() || fish->dying)
        {
            continue;
        }
        if (fish->getPositionX() < 0 || fish->getPositionX() > GameUtils::winSize.width ||
            fish->getPositionY() < 0 || fish->getPositionY() > GameUtils::winSize.height) {
            continue;
        }
        
        
        if (fish->rectRayHitTest(bullet->getPosition(), 0, Size(200, 200)))
        {
            bulletFishId[fish->getFishId()] = bullet->getBulletId();
        }
    }
    
    
    if (!bulletFishId.empty()) {
        EventCustom event(BOMB_FISHES);
        event.setUserData(&bulletFishId);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void GameScene::rayAttack(EventCustom *event)
{
    if (noCollide)
    {
        return;
    }
    
    BulletNode *bullet = (BulletNode*)event->getUserData();
    Vec2 offsetVec = Vec2::forAngle(CC_DEGREES_TO_RADIANS(bullet->getAngle()))*bullet->getRayLength()/2;
    auto posA = bullet->getPosition()+offsetVec;
    auto fishs = fishLayer->getChildren();

    bulletFishId.clear();
    
    for (auto it : fishs)
    {
        auto fish = (FishNode*)it;
        if (!fish->getAlive() || fish->dying)
        {
            continue;
        }
        if (fish->getPositionX() < 0 || fish->getPositionX() > GameUtils::winSize.width ||
            fish->getPositionY() < 0 || fish->getPositionY() > GameUtils::winSize.height) {
            continue;
        }


        if (fish->rectRayHitTest(posA, 90+bullet->getAngle(), Size(64, bullet->getRayLength())))
        {
            bulletFishId[fish->getFishId()] = bullet->getBulletId();
        }
    }
    

    if (!bulletFishId.empty()) {
        EventCustom event(RAY_HIT_FISHES);
        event.setUserData(&bulletFishId);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

bool GameScene::touchBegan(cocos2d::Touch* touch, cocos2d::Event* evnt)
{
//    touch->getLocation();
    if (fishLayer->lockFish != nullptr)
    {
        return false;
    }
    Vec2 vec2 = Director::getInstance()->convertToGL(touch->getLocation());
    cannonLayer->heroTowardPoint(vec2);
    if (currState != UI_FISH_GAME_NORMAL) {
        return false;
    }
    if (uiLayer->getAutoFire() || uiLayer->getReadyRayFire() || uiLayer->getPlaneFiring())
    {
        return true;
    }
    
    if (uiLayer->getReadyPlaneFire()) {
        raidSprite->setPosition(vec2);
        return true;
    }


    stopActionByTag(102);
    auto func = CallFunc::create(CC_CALLBACK_0(CannonLayer::questHeroFire, cannonLayer));
    auto seq = Sequence::create(DelayTime::create(0.15), func, NULL);
    auto act = RepeatForever::create(seq);
    act->setTag(102);
    runAction(act);


    
    return true;
}
void GameScene::touchMove(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (currState != UI_FISH_GAME_NORMAL) {
        return;
    }
    if (fishLayer->lockFish != nullptr)
    {
        return;
    }
    if (uiLayer->getRayFiring())
    {
        return;
    }
    if (uiLayer->getPlaneFiring())
    {
        return;
    }
    Vec2 vec2 = Director::getInstance()->convertToGL(touch->getLocation());
    if (uiLayer->getReadyPlaneFire()) {
        raidSprite->setPosition(touch->getLocation());
//        return;
    }
    
    cannonLayer->heroTowardPoint(vec2);
}

void GameScene::touchScreen(cocos2d::Touch* touch, cocos2d::Event* evnt)
{
//    EventCustom event(CLICK_CREATE_FISH);
//    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    if (currState != UI_FISH_GAME_NORMAL) {
        return;
    }

    if (getChildren().contains(niuLayer)) {
        return;
    }
    if (getChildren().contains(maryLayer)) {
        return;
    }
    if (uiLayer->getAutoFire() && !uiLayer->getReadyRayFire() && !uiLayer->getReadyPlaneFire()) {
        return;
    }
    if (uiLayer->getRayFiring()) {
        return;
    }
    if (uiLayer->getPlaneFiring()) {
        return;
    }
    rayTipSprite->setVisible(false);
    planeTipSprite->setVisible(false);
    planeNetSprite->setVisible(false);
    stopActionByTag(102);
    
    if (uiLayer->getReadyRayFire()) {
        uiLayer->setRayFiring(true);
    }

    if (uiLayer->getReadyPlaneFire()) {
        uiLayer->setPlaneFiring(true);

        Vec2 vec2 = Director::getInstance()->convertToGL(touch->getLocation());
        
        raidSprite->setPosition(touch->getLocation());
        raidAction->play("hide", true);
        raidAction->setLastFrameCallFunc(nullptr);
        planeShadowSprite->setVisible(true);
        planeShadowSprite->setPosition(cannonLayer->getHeroCannon()->getPosition());
        planeShadowSprite->setRotation(cannonLayer->getHeroCannon()->getAngle());
        
        Vec2 offsetVec = Vec2::forAngle(CC_DEGREES_TO_RADIANS(90-cannonLayer->getHeroCannon()->getAngle()))*1000;
        auto posA = planeShadowSprite->getPosition()+offsetVec;
        auto move = MoveTo::create(1, posA);
        auto func = CallFunc::create(std::bind(&GameScene::throwBomb, this, touch->getLocation().x, touch->getLocation().y));
        auto seq = Sequence::create(move, func, NULL);
        planeShadowSprite->runAction(seq);
        return;
    }
    
    cannonLayer->questHeroFire();
}

void GameScene::autoTouch()
{
    if (uiLayer->getReadyRayFire() || uiLayer->getReadyPlaneFire())
    {
        return;
    }
    cannonLayer->questHeroFire();
}

void GameScene::traceFire()
{
    if (currState != UI_FISH_GAME_NORMAL) {
        return;
    }
    cannonLayer->questHeroTraceFire(fishLayer->getLockFishId());
}

void GameScene::scheduleAutoFire()
{
    schedule(CC_CALLBACK_0(GameScene::autoTouch, this), 0.15f, "autofire");
}
void GameScene::scheduleTraceFire()
{
    schedule(CC_CALLBACK_0(GameScene::traceFire, this), 0.15f, "tracefire");
    if (uiLayer->getAutoFire()) {
        unschedule("autofire");
    }
}

void GameScene::stopAutoFire()
{
    unschedule("autofire");
}

void GameScene::stopTraceFire()
{
    unschedule("tracefire");
    if (uiLayer->getAutoFire()) {
        scheduleAutoFire();
    }
}

void GameScene::shakeScreen()
{
    shake = true;
    shakeDuration = 1;
}

void GameScene::iceScreen()
{
    iceSprite->setVisible(true);
    iceAction->play("play", true);
    iceAction->gotoFrameAndPlay(0);
    AUDIO_PLAY("ice", AUDIO_TYPE::EFFECT_TYPE);
}

void GameScene::iceEnd()
{
    iceSprite->setVisible(false);
}

void GameScene::showTideTip(cocos2d::EventCustom *event)
{
    tideTipSprite->setVisible(true);
    auto blinkAction = Blink::create(10, 10);
    auto act = RepeatForever::create(blinkAction);
    warningSprite->runAction(act);
    iceEnd();
    noCollide = true;
    AUDIO_PLAY("scene_conver", AUDIO_TYPE::EFFECT_TYPE);
    
//    bulletLayer->removeAllChildren();
//    fishLayer->removeAllChildren();
//    netLayer->removeAllChildren();
}

void GameScene::showBossTip(cocos2d::EventCustom *event)
{
    bossTipSprite->setVisible(true);
    bossTipFaceSprite->setVisible(true);
    auto blinkAction = Blink::create(10, 10);
    auto act = RepeatForever::create(blinkAction);
    warningSprite->runAction(act);
    iceEnd();
    
    stopActionByTag(101);
    auto delay = DelayTime::create(1.5);
    auto func = CallFunc::create(CC_CALLBACK_0(GameScene::hideBossTip, this));
    auto seq = Sequence::create(delay, func, NULL);
    seq->setTag(101);
    runAction(seq);
    
    L2E_BOSS_COME_TIP info = *static_cast<L2E_BOSS_COME_TIP*>(event->getUserData());
    
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

    bossTipFaceSprite->initWithFile(resDir);
    AUDIO_PLAY("warning", AUDIO_TYPE::EFFECT_TYPE);
    AUDIO_PLAY("boss_come", AUDIO_TYPE::MUSIC_TYPE);
//    fishSpt->setScale(30/(float)fishSpt->getContentSize().height);
}

void GameScene::hideBossTip()
{
    stopActionByTag(101);
    warningSprite->stopAllActions();
    bossTipSprite->setVisible(false);
    bossTipFaceSprite->setVisible(false);
    warningSprite->setVisible(false);
    std::string musicName("bg");
    musicName += Convert2String(abs(rand())%2+1);
    AUDIO_PLAY(musicName, AUDIO_TYPE::MUSIC_TYPE);
}

void GameScene::switchTideBg(cocos2d::EventCustom *event)
{
    L2E_TIDE_SWITCH info = *static_cast<L2E_TIDE_SWITCH *>(event->getUserData());
    std::string frameName("game_scene/game_background/game_background_");
    frameName += Convert2String(info.sceneId);
    frameName += ".png";
    sceneId = info.sceneId;
    waveBgSprite->setTexture(frameName);
    
    
    tideTipSprite->setVisible(false);
    warningSprite->stopAllActions();
    warningSprite->setVisible(false);
    waveBgSprite->setVisible(true);
//    tideWaveSprite->setVisible(true);
    waveBgSprite->stopActionByTag(100);
    stopActionByTag(101);
    auto winSize = Director::getInstance()->getWinSize();
    waveBgSprite->setPosition(Vec2(winSize.width+200, winSize.height/2));
    auto move = MoveTo::create(1, Vec2(0, winSize.height/2));
    auto func = CallFunc::create(CC_CALLBACK_0(GameScene::endOfSwitchTide, this));
    auto seq = Sequence::create(move, func, NULL);
    seq->setTag(100);
    waveBgSprite->runAction(seq);
    maskLayer = LayerColor::create(Color4B(10, 10, 10, 0));
    addChild(maskLayer);
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,maskLayer);
}

void GameScene::endOfSwitchTide()
{
    waveBgSprite->setVisible(false);
    std::string frameName("game_scene/game_background/game_background_");
    frameName += Convert2String(sceneId);
    frameName += ".png";
    bgSprite->setTexture(frameName);
    noCollide = false;
    
    E2L_TIDE_SWITCH info;
    info.eProtocol = e2l_tide_switch;
    ClientLogic::instance()->ProcessUIRequest(&info);
    removeChild(maskLayer);
}

void GameScene::readyRay()
{
    uiLayer->readyRay();
    cannonLayer->getHeroCannon()->readyRay();
    
    readyRayDuration = 3;
    readyRayCount->setString(Convert2String(readyRayDuration));
    rayTipSprite->setVisible(true);
}

void GameScene::autoRay()
{
    readyRayDuration = -1;
    uiLayer->setRayFiring(true);
    rayTipSprite->setVisible(false);
    stopActionByTag(102);
    cannonLayer->questHeroFire();
}

void GameScene::endRay()
{
    rayTipSprite->setVisible(false);
    uiLayer->endRay();
    EventCustom event(END_RAY_FIRE);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void GameScene::readyPlane()
{
    uiLayer->readyPlane();
    cannonLayer->getHeroCannon()->readyPlane();
    
    raidSprite->setVisible(true);
    raidSprite->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    raidAction->play("show", false);
    raidAction->setLastFrameCallFunc(CC_CALLBACK_0(GameScene::endRaidShow, this));
    readyPlaneDuration = 10;
    readyPlaneCount->setString(Convert2String(readyPlaneDuration));
    planeTipSprite->setVisible(true);
    planeNetSprite->setVisible(true);
}

void GameScene::endPlane()
{
    planeTipSprite->setVisible(false);
    planeNetSprite->setVisible(false);
    uiLayer->endPlane();
    EventCustom event(END_PLANE_BOMB);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void GameScene::autoPlane()
{
    readyPlaneDuration = -1;
    uiLayer->setPlaneFiring(true);
    planeTipSprite->setVisible(false);
    planeNetSprite->setVisible(false);
    stopActionByTag(102);
//    cannonLayer->questHeroFire();

    
    Vec2 vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2);
    raidSprite->setPosition(vec2);
    raidAction->play("hide", true);
    raidAction->setLastFrameCallFunc(nullptr);
    planeShadowSprite->setVisible(true);
    planeShadowSprite->setPosition(cannonLayer->getHeroCannon()->getPosition());
    planeShadowSprite->setRotation(0);
    
    Vec2 offsetVec = Vec2::forAngle(CC_DEGREES_TO_RADIANS(90))*1000;
    auto posA = planeShadowSprite->getPosition()+offsetVec;
    auto move = MoveTo::create(1, posA);
    auto func = CallFunc::create(std::bind(&GameScene::throwBomb, this, vec2.x, vec2.y));
    auto seq = Sequence::create(move, func, NULL);
    planeShadowSprite->runAction(seq);
}

void GameScene::endRaidShow()
{
    raidAction->play("wait", true);
}

void GameScene::throwBomb(float x, float y)
{
    raidSprite->setVisible(false);
    planeShadowSprite->setVisible(false);
    cannonLayer->questHeroBombFire(x, y);
}
void GameScene::splashNiuNiu()
{
    changeGameState(UI_FISH_GAME_SETTINGS);
    splashLayer = CaseSplashLayer::create();
    splashLayer->setSplashCaseType(NIUNIU);
    splashLayer->setupView();
    addChild(splashLayer);
    AUDIO_PLAY("happystart", AUDIO_TYPE::EFFECT_TYPE);
}
void GameScene::enterNiuNiu(EventCustom* event)
{
//    stopActionByTag(102);
//    changeGameState(UI_FISH_GAME_SETTINGS);
    niuLayer = NiuNiuLayer::create();
    niuLayer->setupView(event);
    addChild(niuLayer);
}

void GameScene::exitNiuNiu()
{
    changeGameState(UI_FISH_GAME_NORMAL);
    removeChild(niuLayer);
}

void GameScene::splashMary()
{
    changeGameState(UI_FISH_GAME_SETTINGS);
    splashLayer = CaseSplashLayer::create();
    splashLayer->setSplashCaseType(MARY);
    splashLayer->setupView();
    addChild(splashLayer);
    AUDIO_PLAY("happystart", AUDIO_TYPE::EFFECT_TYPE);
}
void GameScene::enterMary(EventCustom* event)
{
//    stopActionByTag(102);
//    changeGameState(UI_FISH_GAME_SETTINGS);
    maryLayer = MaryLayer::create();
    maryLayer->beginMary(event);
    addChild(maryLayer);
}

void GameScene::exitMary()
{
    changeGameState(UI_FISH_GAME_NORMAL);
    removeChild(maryLayer);
}

void GameScene::showUpgradeLevel(cocos2d::EventCustom *event)
{
//    stopActionByTag(102);
    changeGameState(UI_FISH_GAME_SETTINGS);
    upgradeLayer = UpgradeLevelLayer::create();
    upgradeLayer->setupView(event);
    addChild(upgradeLayer);
}

void GameScene::hideUpgradeLevel(cocos2d::EventCustom *event)
{
    changeGameState(UI_FISH_GAME_NORMAL);
    removeChild(upgradeLayer);
}

void GameScene::showSetting()
{
//    stopActionByTag(102);
//    stopAutoFire();
    changeGameState(UI_FISH_GAME_SETTINGS);
    settingLayer = GameSettingLayer::create();
    addChild(settingLayer);
}

void GameScene::exitSetting()
{
    removeChild(settingLayer);
    changeGameState(UI_FISH_GAME_NORMAL);
}

void GameScene::completeBossTask(cocos2d::EventCustom *event)
{
    completeLayer = StageCompleteLayer::create();
    completeLayer->setupView(event);
    addChild(completeLayer);
    
    EventCustom event1(UI_KILLBOSS_TIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
}

void GameScene::showNextStage()
{
    nextLayer = NextStageLayer::create();
    addChild(nextLayer);
}

void GameScene::showTaskBoundDialog(cocos2d::EventCustom *event)
{
//    stopActionByTag(102);
//    stopAutoFire();
    changeGameState(UI_FISH_GAME_SETTINGS);
    taskLayer = TaskBoundLayer::create();
    taskLayer->setupView(event);
    addChild(taskLayer);
}

void GameScene::takeTaskBound()
{
    changeGameState(UI_FISH_GAME_NORMAL);
    GOLD_FALL_ANIMATION();
    removeChild(taskLayer);
}

void GameScene::hideTaskBound()
{
    changeGameState(UI_FISH_GAME_NORMAL);
    removeChild(taskLayer);
}

void GameScene::enterLuckyDraw(cocos2d::EventCustom *event)
{
    changeGameState(UI_FISH_GAME_SETTINGS);
    luckyLayer = LuckyDrawLayer::create();
    luckyLayer->enterLuckyDraw(event);
    addChild(luckyLayer);
//    lotteryLayer = GameLottery::create();
//    addChild(lotteryLayer);
}

void GameScene::exitLuckyDraw()
{
    changeGameState(UI_FISH_GAME_NORMAL);
    removeChild(luckyLayer);
}

void GameScene::enterLottery(cocos2d::EventCustom *event)
{
    removeChild(luckyLayer);
    lotteryLayer = GameLottery::create();
    lotteryLayer->setupView(event);
    addChild(lotteryLayer);
}

void GameScene::endLottery()
{
    changeGameState(UI_FISH_GAME_NORMAL);
    removeChild(lotteryLayer);
}

void GameScene::showMultiplyNotEnough(cocos2d::EventCustom *event)
{
    changeGameState(UI_FISH_GAME_SETTINGS);
    uiLayer->setAutoFire(false);
    mulNotEnoughLayer = MultiplyNotEnoughLayer::create();
    mulNotEnoughLayer->setupView(event);
    addChild(mulNotEnoughLayer);
}

void GameScene::hideMultiplyNotEnough(cocos2d::EventCustom *event)
{
    changeGameState(UI_FISH_GAME_NORMAL);
    removeChild(mulNotEnoughLayer);
}

void GameScene::showLackGoldTip()
{
    stopAutoFire();
    uiLayer->setAutoFire(false);
    tipLayer->addLackGoldTip();
}

void GameScene::showRayNeedVipTip(cocos2d::EventCustom* event)
{
    changeGameState(UI_FISH_GAME_SETTINGS);
    L2E_RAY_NEED_VIP info = *static_cast<L2E_RAY_NEED_VIP*>(event->getUserData());
    DelayLabelTip::fp f = std::bind(&GameScene::askShowVip, this);
    tipLayer->addRayNeedVipTip(info.needVipLevel, f);
}

void GameScene::showAutoFireNeedVipTip(cocos2d::EventCustom* event)
{
    changeGameState(UI_FISH_GAME_SETTINGS);
    L2E_AUTO_FIRE_NEED_VIP info = *static_cast<L2E_AUTO_FIRE_NEED_VIP*>(event->getUserData());
    DelayLabelTip::fp f = std::bind(&GameScene::askShowVip, this);
    tipLayer->addAutoFireNeedVipTip(info.needVipLevel, f);
}

void GameScene::showSkillNeedDiamondTip()
{
    changeGameState(UI_FISH_GAME_SETTINGS);
    DelayLabelTip::fp f = std::bind(&GameScene::askShowRecharge, this);
    tipLayer->addSkillNeedDiamondTip(f);
}

void GameScene::showUnlockNeedDiamondTip()
{
    changeGameState(UI_FISH_GAME_SETTINGS);
    DelayLabelTip::fp f = std::bind(&GameScene::askShowRecharge, this);
    tipLayer->addUnlockNeedDiamondTip(f);
}

void GameScene::rechargeOk(EventCustom *event)
{
    tipLayer->addRechargeOkTip();
    GOLD_FALL_ANIMATION();
    L2E_RECHARGE_OK info = *static_cast<L2E_RECHARGE_OK*>(event->getUserData());
    if(info.type == 1)
    {
        EventCustom event1(UI_RECHARGE_GOLD_OK);
        event1.setUserData(&info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
    }else if (info.type == 0) {
        EventCustom event2(UI_RECHARGE_DIAMOND_OK);
        event2.setUserData(&info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event2);
    }
}

void GameScene::showRecharge(EventCustom* event)
{
    stopAutoFire();
    uiLayer->setAutoFire(false);
    
    L2E_SHOW_HALL_RECHARGE info = *static_cast<L2E_SHOW_HALL_RECHARGE*>(event->getUserData());
    removeChildByTag(1003);
    removeChildByTag(1004);
    removeChildByTag(1005);
    if (info.type == 1) {
        auto layer = GoldRechargeLayer::create();
        layer->setTag(1003);
        layer->setupView(event);
        addChild(layer);
    }else if(info.type == 0){
        auto layer = DiamondRechargeLayer::create();
        layer->setTag(1003);
        layer->setupView(event);
        addChild(layer);
    }
}

void GameScene::showVipIntro(cocos2d::EventCustom *event)
{
    stopAutoFire();
    uiLayer->setAutoFire(false);
    
    removeChildByTag(1003);
    removeChildByTag(1004);
    removeChildByTag(1005);
    
    auto layer = VipIntroLayer::create();
    layer->setTag(1004);
    layer->setupView(event);
    addChild(layer);
}

void GameScene::showVipGift()
{
    stopAutoFire();
    uiLayer->setAutoFire(false);
    
    removeChildByTag(1003);
    removeChildByTag(1004);
    removeChildByTag(1005);
    
    auto layer = VipGiftLayer::create();
    layer->setTag(1005);
    addChild(layer);
}

void GameScene::askShowVip()
{
    E2L_SHOW_VIP info;
    info.eProtocol = e2l_show_vip;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameScene::askShowRecharge()
{
    E2L_SHOW_GAME_RECHARGE info;
    info.eProtocol = e2l_show_game_recharge;
    info.type = 0;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameScene::closeRecharge()
{
    changeGameState(UI_FISH_GAME_NORMAL);
}

void GameScene::showBombNeedUnlockTip()
{
    tipLayer->addBombNeedUnlockTip();
}
