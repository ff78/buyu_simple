//
//  UIManager.h
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#ifndef __buyu_simple__UIManager__
#define __buyu_simple__UIManager__

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "../Share/Header/ELProtocol.h"
#include "../Share/Header/EventDef.h"

class UIManager : public Singleton<UIManager>, public cocos2d::Ref
{
public:
    UIManager();
    
    void processLogicResponse(void *pMsg);
    void (UIManager::*m_Logic2EngineFunc[l2e_max])(void *pMsg);
    
    void loop(float dt);
private:
    void showLoadLogin(void *pMsg);
    void updateLoadLogin(void *pMsg);
    void showLogin(void *pMsg);
    void showLoginResult(void *pMsg);
    void showHall(void *pMsg);
    void showGameLoading(void *pMsg);
    void updateGameLoading(void *pMsg);
    void showGame(void *pMsg);
    void updateLoadGold(void *pMsg);
    void showGoldResult(void *pMsg);    
    void showGoldDiff(void *pMsg);
    void updateGoldDiff(void *pMsg);
    void showGoldStage(void *pMsg);
    void updateGoldStage(void *pMsg);    
    void showGoldRunner(void *pMsg);
    void updateGoldRunner(void *pMsg);
    void openBox(void *pMsg);
    void startGoldRoll(void *pMsg);
    
    void createFish(void *pMsg);
    void scheduleTide(void *pMsg);
    void tideComeTip(void *pMsg);
    void tideSwitchBg(void *pMsg);
    void heroFire(void *pMsg);
    void createCannon(void *pMsg);
    void changeCannon(void *pMsg);
    void changeCannonMul(void *pMsg);
    void castNet(void *pMsg);
    void readyRay(void *pMsg);
    void readyPlane(void *pMsg);
//    void endRay(void *pMsg);
    void showCannonMulNotEnough(void *pMsg);
    void hideCannonMulNotEnough(void *pMsg);
    void showGoldLackTip(void *pMsg);
    
    void bossComeTip(void *pMsg);
    
    void splashNiuNiu(void *pMsg);
    void enterNiuNiu(void *pMsg);
    void showHand(void *pMsg);
    void exitNiuNiu(void *pMsg);
    
    void splashMary(void *pMsg);
    void enterMary(void *pMsg);
    void maryInit(void *pMsg);
    void exitMary(void *pMsg);
    
    void enterLuckyDraw(void *pMsg);
    void exitLuckyDraw(void *pMsg);
    void updateLuckyDraw(void *pMsg);
    void enterLottery(void *pMsg);
    void endLottery(void *pMsg);
    
    void unlockCannonLevel(void *pMsg);
    void updateCannonLevel(void *pMsg);
    void hideCannonLevel(void *pMsg);
    void showCannonUnlockDialog(void *pMsg);
    void chargeUnlockLevel(void *pMsg);
    void hideUnlockLevel(void *pMsg);
    void showUnlockNeedDiamondTip(void *pMsg);
    
    void updateRelifeButton(void *pMsg);
    void updateStarBox(void *pMsg);
    void updateGold(void *pMsg);
    void updateDiamond(void *pMsg);
    void updateEnergy(void *pMsg);
    void updateHeadInfo(void *pMsg);
    void levelupTip(void *pMsg);
    void updateSkillCount(void *pMsg);
    void useSkill(void *pMsg);
    void chargeSkill(void *pMsg);
    void chargeUseSkill(void *pMsg);
    void showBombNeedUnlockTip(void *pMsg);
    
    void showSetting(void *pMsg);
    void exitSetting(void *pMsg);
    void showFishSetting(void *pMsg);
    void showCannonSetting(void *pMsg);
    void updateCannonSetting(void *pMsg);
    
    void updateBossTask(void *pMsg);
    void initBossTask(void *pMsg);
    void completeBossTask(void *pMsg);
    void initTask(void *pMsg);
    void updateTask(void *pMsg);
    void completeSubTask(void *pMsg);
    void showTaskBound(void *pMsg);
    void takeTaskBound(void *pMsg);
    void hideTaskBound(void *pMsg);
    
    void tipDisableSkill(void *pMsg);
    
    void showDiary(void *pMsg);
    
    void dropItem(void *pMsg);
    
    void showHallRecharge(void *pMsg);
    void showVipIntro(void *pMsg);
    void showVipGift(void *pMsg);
    void showRayNeedVipTip(void *pMsg);
    void showAutoFireNeedVipTip(void *pMsg);
    void canUseAutoFire(void *pMsg);
    void updateHallMoney(void *pMsg);
    void rechargeOk(void *pMsg);
    void closeRecharge(void *pMsg);
    
    void showBenefits(void *pMsg);
    void cashBenefitCode(void *pMsg);
    
    void showShare(void *pMsg);
    void updateShareResult(void *pMsg);
    void takeShareCountBound(void *pMsg);
    void showComment(void *pMsg);
};

#endif /* defined(__buyu_simple__UIManager__) */
