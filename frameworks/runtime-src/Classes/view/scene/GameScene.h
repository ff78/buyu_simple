//
//  GameScene.h
//  buyu_simple
//
//  Created by ff on 15/12/9.
//
//

#ifndef __buyu_simple__GameScene__
#define __buyu_simple__GameScene__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "../gameLayers/FishLayer.h"
#include "../gameLayers/BulletLayer.h"
#include "../gameLayers/CannonLayer.h"
#include "../gameLayers/NetLayer.h"
#include "../gameLayers/GameUILayer.hpp"
#include "../gameLayers/FishCoinLayer.hpp"
#include "../gameLayers/NiuNiuLayer.h"
#include "../gameLayers/LightEffectLayer.h"
#include "../gameLayers/MaryLayer.h"
#include "../gameLayers/UpgradeLevelLayer.h"
#include "../gameLayers/GameSettingLayer.h"
#include "../gameLayers/StageCompleteLayer.h"
#include "../gameLayers/NextStageLayer.h"
#include "../gameLayers/TaskBoundLayer.h"
#include "../gameLayers/TipLayer.h"
#include "../gameLayers/CaseSplashLayer.h"
#include "../gameLayers/GameLottery.h"
#include "../gameLayers/LuckyDrawLayer.h"
#include "../gameLayers/DropItemLayer.h"
#include "../gameLayers/MultiplyNotEnoughLayer.h"
#include "../gameLayers/GoldRechargeLayer.h"
#include "../gameLayers/DiamondRechargeLayer.h"
#include "../gameLayers/VipIntroLayer.h"
#include "../gameLayers/VipGiftLayer.h"

enum UI_FISH_GAME_STATE
{
    UI_FISH_GAME_NORMAL = 0,
    UI_FISH_GAME_SETTINGS
};

class GameScene : public cocos2d::Scene {
public:
    ~GameScene();
    
    bool init();
    
    CREATE_FUNC(GameScene);
    
    void setupView();
    void updateScene(int sceneIndex);
    
    void update(float dt);
    
    void changeGameState(UI_FISH_GAME_STATE nextState);
    void enterGameState(UI_FISH_GAME_STATE nextState);
    void exitGameState(UI_FISH_GAME_STATE state);
    
    bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* evnt);
    void touchScreen(cocos2d::Touch* touch, cocos2d::Event* evnt);
    void touchMove(cocos2d::Touch* touch, cocos2d::Event* event);
    
//    void scheduleTide(cocos2d::EventCustom* event);
    void showTideTip(cocos2d::EventCustom* event);
    void switchTideBg(cocos2d::EventCustom* event);
//    void swith2Tide(float dt);
    
    void showBossTip(cocos2d::EventCustom *event);
    void hideBossTip();
    
    void endOfSwitchTide();
    
    void scheduleAutoFire();
    void stopAutoFire();
    void autoTouch();
    void scheduleTraceFire();
    void stopTraceFire();
    void traceFire();
    
    
    void shakeScreen();
    void iceScreen();
    void iceEnd();
    
    void readyRay();
    void endRay();
    void autoRay();
    
    void readyPlane();
    void endPlane();
    void autoPlane();
    void endRaidShow();
    void throwBomb(float x, float y);
    
    void rayAttack(cocos2d::EventCustom* event);
    void bombAttack(cocos2d::EventCustom* event);
    
    void splashNiuNiu();
    void enterNiuNiu(cocos2d::EventCustom* event);
    void exitNiuNiu();
    void splashMary();
    void enterMary(cocos2d::EventCustom* event);
    void exitMary();
    
    void enterLuckyDraw(cocos2d::EventCustom *event);
    void exitLuckyDraw();
    void enterLottery(cocos2d::EventCustom *event);
    void endLottery();
    
    void showUpgradeLevel(cocos2d::EventCustom* event);
    void hideUpgradeLevel(cocos2d::EventCustom* event);
    void completeBossTask(cocos2d::EventCustom *event);
    void showSetting();
    void exitSetting();
    void showNextStage();
    void showTaskBoundDialog(cocos2d::EventCustom* event);
    void takeTaskBound();
    void hideTaskBound();
    
    void showMultiplyNotEnough(cocos2d::EventCustom* event);
    void hideMultiplyNotEnough(cocos2d::EventCustom* event);
    void showLackGoldTip();
    void showRayNeedVipTip(cocos2d::EventCustom* event);
    void showAutoFireNeedVipTip(cocos2d::EventCustom* event);
    
    void rechargeOk(cocos2d::EventCustom *event);
    void showRecharge(cocos2d::EventCustom* event);
    void removeRecharge();
    void showVipIntro(cocos2d::EventCustom* event);    
    void showVipGift();
    void askShowVip();
    void askShowRecharge();
    void closeRecharge();
    void showBombNeedUnlockTip();
    void showSkillNeedDiamondTip();
    void showUnlockNeedDiamondTip();
private:
    cocos2d::Sprite* bgSprite;
    cocos2d::Sprite* scorllSprite;
    cocos2d::Sprite* tideTipSprite;
    cocos2d::Sprite* warningSprite;
    cocos2d::Sprite* tideWaveSprite;
    cocos2d::Sprite* waveBgSprite;
    cocos2d::Sprite* bossTipSprite;
    cocos2d::Sprite* bossTipFaceSprite;
    cocos2d::Sprite* rayTipSprite;
    cocos2d::Sprite* planeTipSprite;
    cocos2d::Sprite* planeShadowSprite;
    cocos2d::Sprite* planeNetSprite;
    
    cocos2d::Node* iceSprite;
    cocostudio::timeline::ActionTimeline* iceAction;
    
    cocos2d::Node* raidSprite;
    cocostudio::timeline::ActionTimeline* raidAction;
    
    cocos2d::Node* nextDialog;
    
    
//    cocos2d::EventListenerCustom *tideListener;
    cocos2d::EventListenerCustom *tideTipListener;
    cocos2d::EventListenerCustom *tideSwitchListener;
    cocos2d::EventListenerCustom *startAutoListener;
    cocos2d::EventListenerCustom *stopAutoListener;
    cocos2d::EventListenerCustom *bossTipListener;
    
    cocos2d::EventListenerCustom *shakeListener;
    cocos2d::EventListenerCustom *iceListener;
    cocos2d::EventListenerCustom *unfrozenListener;
    
    cocos2d::EventListenerCustom *lockListener;
    cocos2d::EventListenerCustom *stopLockListener;
    cocos2d::EventListenerCustom *readyRayListener;
    cocos2d::EventListenerCustom *endRayListener;
    cocos2d::EventListenerCustom *rayAttackListener;
    cocos2d::EventListenerCustom *readyPlaneListener;
    cocos2d::EventListenerCustom *endPlaneListener;
    cocos2d::EventListenerCustom *planeAttackListener;
    cocos2d::EventListenerCustom *enterNiuNiuListener;
    cocos2d::EventListenerCustom *enterMaryListener;
    cocos2d::EventListenerCustom *showUpgradeListener;
    cocos2d::EventListenerCustom *hideUpgradeListener;
    cocos2d::EventListenerCustom *showSettingListener;
    cocos2d::EventListenerCustom *compBossTaskListener;
    cocos2d::EventListenerCustom *nextStageDialogListener;
    cocos2d::EventListenerCustom *showTaskBoundListener;
    cocos2d::EventListenerCustom *takeTaskBoundListener;
    cocos2d::EventListenerCustom *hideSettingListener;
    cocos2d::EventListenerCustom *hideNiuNiuListener;
    cocos2d::EventListenerCustom *hideMaryListener;
    cocos2d::EventListenerCustom *showNiuNiuSplashListener;
    cocos2d::EventListenerCustom *showMarySplashListener;
    cocos2d::EventListenerCustom *hideTaskBoundListener;
    cocos2d::EventListenerCustom *enterLuckyDrawListener;
    cocos2d::EventListenerCustom *exitLuckyDrawListener;
    cocos2d::EventListenerCustom *enterLotteryListener;
    cocos2d::EventListenerCustom *endLotteryListener;
    cocos2d::EventListenerCustom *multiNotEnoughListener;
    cocos2d::EventListenerCustom *hideMultiNotEnoughListener;
    cocos2d::EventListenerCustom *lackGoldListener;
    cocos2d::EventListenerCustom *rayNeedVipTipListener;
    cocos2d::EventListenerCustom *autoFireNeedVipTipListener;
    cocos2d::EventListenerCustom* rechargeOkListener;
    cocos2d::EventListenerCustom* showRechargeListener;
    cocos2d::EventListenerCustom* showVipIntroListener;
    cocos2d::EventListenerCustom* showVipGiftListener;
    cocos2d::EventListenerCustom* closeRechargeListener;
    cocos2d::EventListenerCustom* bombNeedUnlockListener;
    cocos2d::EventListenerCustom* skillNeedDiamondListener;
    cocos2d::EventListenerCustom* unlockNeedDiamondListener;
    
    
    
    bool noCollide;
    float readyRayDuration;
    cocos2d::ui::TextAtlas *readyRayCount;
    float readyPlaneDuration;
    cocos2d::ui::TextAtlas *readyPlaneCount;
    
    CC_SYNTHESIZE(int, sceneId, SceneId);
    CC_SYNTHESIZE(int, scrollSceneId, ScrollSceneId);
    
    CC_SYNTHESIZE_READONLY(FishLayer*, fishLayer, FishLayer);
    CC_SYNTHESIZE_READONLY(BulletLayer*, bulletLayer, BulletLayer);
    CC_SYNTHESIZE_READONLY(CannonLayer*, cannonLayer, CannonLayer);
    CC_SYNTHESIZE_READONLY(NetLayer*, netLayer, NetLayer);
    CC_SYNTHESIZE_READONLY(GameUILayer*, uiLayer, UiLayer);
    CC_SYNTHESIZE_READONLY(FishCoinLayer*, coinLayer, CoinLayer);
    CC_SYNTHESIZE_READONLY(NiuNiuLayer*, niuLayer, NiuLayer);
    CC_SYNTHESIZE_READONLY(LightEffectLayer*, lightLayer, LightLayer);
    CC_SYNTHESIZE_READONLY(MaryLayer*, maryLayer, MaryLayer);
    CC_SYNTHESIZE_READONLY(UpgradeLevelLayer*, upgradeLayer, UpgradeLayer);
    CC_SYNTHESIZE_READONLY(GameSettingLayer*, settingLayer, SettingLayer);
    CC_SYNTHESIZE_READONLY(StageCompleteLayer*, completeLayer, CompleteLayer);
    CC_SYNTHESIZE_READONLY(NextStageLayer*, nextLayer, NextLayer);
    CC_SYNTHESIZE_READONLY(TaskBoundLayer*, taskLayer, TaskLayer);
    CC_SYNTHESIZE_READONLY(TipLayer*, tipLayer, TipLayer);
    CC_SYNTHESIZE_READONLY(CaseSplashLayer*, splashLayer, SplashLayer);
    CC_SYNTHESIZE_READONLY(GameLottery*, lotteryLayer, LotteryLayer);
    CC_SYNTHESIZE_READONLY(LuckyDrawLayer*, luckyLayer, LuckyLayer);
    CC_SYNTHESIZE_READONLY(DropItemLayer*, dropLayer, DropLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::LayerColor*, maskLayer, MaskLayer);
    CC_SYNTHESIZE_READONLY(MultiplyNotEnoughLayer*, mulNotEnoughLayer, MulNotEnoughLayer);
    CC_SYNTHESIZE_READONLY(UI_FISH_GAME_STATE, currState, CurrState);
    CC_SYNTHESIZE_READONLY(UI_FISH_GAME_STATE, lastState, LastState);

    
    std::map<int, int> bulletFishId;
    std::vector<int> collideBulletId;
    
    float offsetX, offsetY;
    bool shake;
    float shakeDuration;
    
};

#endif /* defined(__buyu_simple__GameScene__) */
