//
//  ClientLogic.h
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#ifndef __buyu_simple__ClientLogic__
#define __buyu_simple__ClientLogic__

#include "utils/GameUtils.h"
#include "../Share/Header/ELProtocol.h"
#include "../Share/Header/CSProtocol.h"

enum E_GAME_STATE
{
    GAME_STATE_INIT,
    GAME_STATE_LOADLOGIN,
    GAME_STATE_LOGIN,
    GAME_STATE_HALL,
    GAME_STATE_GAME,
    GAME_STATE_LOAD_GAME,
    GAME_STATE_GOLDEN_DIFF,
    GAME_STATE_SEL_GOLDEN,
    GAME_STATE_GOLDEN_RUNNER,
    GAME_STATE_LOAD_GOLDEN,
    GAME_STATE_GOLDEN_GAME,
    GAME_STATE_IN_TOWN,
    GAME_STATE_IN_WORLD_MAP,
    GAME_STATE_BEGIN_EMBATTLE,
    GAME_STATE_BEGIN_BATTLE,
    GAME_STATE_IN_BATTLE,
    GAME_STATE_FINISH_BATTLE,
    GAME_STATE_MAX,
};

//游戏主要逻辑处理，分发
class ClientLogic : public Singleton<ClientLogic>
{
public:
    ClientLogic();
    
    void mainLoop(float dt);
    
    void (ClientLogic::*m_UI2LogicFunc[e2l_max])(void *pMsg);
    void (ClientLogic::*m_LogicStateFunc[GAME_STATE_MAX])(float dt);
    
    void ChangeState(E_GAME_STATE eState);
    
    //向显示层发送结构体消息
    void pass2Engine(void *pMsg);
    //处理显示层请求，实现见ProcessUIRequest.cpp
    void ProcessUIRequest(void *pMsg);
    
    void (ClientLogic::*m_Server2ClientFunc[s2c_max])(void *pMsg);
    void pass2Server(void *pMsg);
    void ProcessServerResponse(void *pMsg);
   
private:
    void UIRequestLogin(void *pMsg);
    void UIRequestEnterGame(void *pMsg);
    void UIRequestDiff(void *pMsg);
    void UISelDiff(void *pMsg);
    void UIRequestStage(void *pMsg);
    void UIOpenStageBox(void *pMsg);
    void UIGetFromStageBox(void *pMsg);
    void UIGetFromFateBox(void *pMsg);
    void UIBack2Diff(void *pMsg);
    void UIBack2Stage(void *pMsg);
    void UIOpenFateBox(void *pMsg);
    void UIStartRoll(void *pMsg);
    void UIGetFromFateRoll(void *pMsg);
    void UIStartGoldGame(void *pMsg);
    void UIGoldGameResult(void *pMsg);
    void UIGoldGainObj(void *pMsg);
    
    void UIRemoveFish(void *pMsg);
    void UITideSwitch(void *pMsg);
    void UIFire(void *pMsg);
    void UIFishDead(void *pMsg);
    void UIFishLight(void *pMsg);
    void UIShowCannonHold(void *pMsg);
    void UIChooseCannon(void *pMsg);
    void UIChangeCannonMul(void *pMsg);
    void UIUnlockCannonLevel(void *pMsg);
    void UIHideUnlockLevel(void *pMsg);
    void UIChargeUnlockLevel(void *pMsg);
    void UIReceiveDropItem(void *pMsg);
    
    void UIChargeSkill(void *pMsg);
    void UIUseSkill(void *pMsg);
    void UIFishClear(void *pMsg);
    
    void UINiuNiuSplashOver(void *pMsg);
    void UIShowHand(void *pMsg);
    void UINiuNiuEnd(void *pMsg);
    void UIMarySplashOver(void *pMsg);
    void UIMaryInit(void *pMsg);
    void UIMaryExit(void *pMsg);
    
    void UIShowSetting(void *pMsg);
    void UIExitSetting(void *pMsg);
    void UIShowFishSetting(void *pMsg);
    void UICannonUpgradeVip(void *pMsg);
    
    void UICollectBossBound(void *pMsg);
    void UIShowTaskBound(void *pMsg);
    void UITakeTaskBound(void *pMsg);
    void UIHideTaskBound(void *pMsg);
    
    void UIShowLuckyDraw(void *pMsg);
    void UIExitLuckyDraw(void *pMsg);
    void UIUpdateLuckyDraw(void *pMsg);
    void UIEnterLottery(void *pMsg);
    void UIEndLottery(void *pMsg);
    
    void UINextStage(void *pMsg);
    void UIQuitStage(void *pMsg);
    void UIShowDiary(void *pMsg);
    
    void UITakeRelife(void *pMsg);
    void UIUnlock2Require(void *pMsg);
    void UIHideMultiNotEnough(void *pMsg);
    
    void UIShowHallRecharge(void *pMsg);
    void UIShowVipIntro(void *pMsg);
    void UIShowVipGift(void *pMsg);
    void UIShowVip(void *pMsg);
    void UIRecharge(void *pMsg);
    void UICheckAutoFire(void *pMsg);
    void UICloseRecharge(void *pMsg);
    void UIShowGameRecharge(void *pMsg);
    
    void UIShowBenefits(void *pMsg);
    void UICashBenefitCode(void *pMsg);
    void UILink2NetGame(void *pMsg);
    
    void UIShowShare(void *pMsg);
    void UIShare(void *pMsg);
    void UITaskShareCountBound(void *pMsg);
    
    void ResRecharge(void *pMsg);
    void ResCashBenefitCode(void *pMsg);
    void ResShare(void *pMsg);
    
    void onStateInit(float dt);
    void onStateLoadLogin(float dt);
    void onStateLoadGold(float dt);
    void onStateGoldGame(float dt);
    void onStateFishGame(float dt);

    bool enterGameMode(E_GAME_STATE next);
    void exitGameMode(E_GAME_STATE currMode);
    
private:
    E_GAME_STATE m_eLastState;
    E_GAME_STATE m_eState;
    E_GAME_STATE cacheGameMode;

};

extern ClientLogic	*g_pClientLogic;

#endif /* defined(__buyu_simple__ClientLogic__) */
