//
//  ProcessUIRequest.cpp
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#include "ClientLogic.h"
#include "Login.h"
#include "Hall.h"
#include "GoldGame.h"
#include "EventDef.h"
#include "GameCore.h"

void ClientLogic::ProcessUIRequest(void *pMsg)
{
    if (NULL == pMsg)
        return;
    
    E2LProtocol	eProtocol	= *((E2LProtocol*)pMsg);
    if (eProtocol >= e2l_max)
        return;
    //分发到各个协议的处理函数
    if (this->m_UI2LogicFunc[eProtocol] != NULL)
    {
        (this->*m_UI2LogicFunc[eProtocol])(pMsg);
    }
}


void ClientLogic::UIRequestLogin(void *pMsg)
{
    E2L_LOGIN	*pMsgInfo	= (E2L_LOGIN*)pMsg;
    Login::instance()->isLoginSuc(*pMsgInfo);

//    g_pClientPlayer->Login(pMsgInfo);
}


void ClientLogic::UIRequestEnterGame(void *pMsg)
{
    E2L_ENTERGAME	*pMsgInfo	= (E2L_ENTERGAME*)pMsg;
    Hall::instance()->enterGame(*pMsgInfo);
}

void ClientLogic::UIRequestDiff(void *pMsg)
{
    E2L_GOLD_DIFF	*pMsgInfo	= (E2L_GOLD_DIFF*)pMsg;
    GoldGame::instance()->requestDiff(*pMsgInfo);
}

void ClientLogic::UISelDiff(void *pMsg)
{
    E2L_GOLD_SEL_DIFF	*pMsgInfo	= (E2L_GOLD_SEL_DIFF*)pMsg;
    GoldGame::instance()->selDiff(*pMsgInfo);
}

void ClientLogic::UIRequestStage(void *pMsg)
{
    E2L_SEL_GOLD	*pMsgInfo	= (E2L_SEL_GOLD*)pMsg;
    GoldGame::instance()->selStage(*pMsgInfo);
}

void ClientLogic::UIOpenStageBox(void *pMsg)
{
    E2L_OPEN_BOX	*pMsgInfo	= (E2L_OPEN_BOX*)pMsg;
    GoldGame::instance()->openStageBox(*pMsgInfo);
}
void ClientLogic::UIOpenFateBox(void *pMsg)
{
    E2L_OPEN_BOX	*pMsgInfo	= (E2L_OPEN_BOX*)pMsg;
    GoldGame::instance()->openFateBox(*pMsgInfo);
}

void ClientLogic::UIStartRoll(void *pMsg)
{
    E2L_GOLD_START_ROLL	*pMsgInfo	= (E2L_GOLD_START_ROLL*)pMsg;
    GoldGame::instance()->startRoll(*pMsgInfo);
}


void ClientLogic::UIStartGoldGame(void *pMsg)
{
    E2L_GOLD_START_GAME	*pMsgInfo	= (E2L_GOLD_START_GAME*)pMsg;
    GoldGame::instance()->startGame(*pMsgInfo);
}

void ClientLogic::UIGoldGameResult(void *pMsg)
{
    E2L_GOLD_RESULT	*pMsgInfo	= (E2L_GOLD_RESULT*)pMsg;
    GoldGame::instance()->gameResult(*pMsgInfo);
}

void ClientLogic::UIGoldGainObj(void *pMsg)
{
    E2L_GOLD_GAIN_OBJ *pMsgInfo = (E2L_GOLD_GAIN_OBJ*)pMsg;
    GoldGame::instance()->gainObj();
}
void ClientLogic::UIRemoveFish(void *pMsg)
{
    E2L_REMOVE_OUT_FISH *pMsgInfo = (E2L_REMOVE_OUT_FISH*)pMsg;
    GameCore::instance()->removeFish(*pMsgInfo);
}

void ClientLogic::UITideSwitch(void *pMsg)
{
    GameCore::instance()->tideSwitchOver();
}

void ClientLogic::UIFire(void *pMsg)
{
    E2L_CANNON_FIRE *pMsgInfo = (E2L_CANNON_FIRE*)pMsg;
    GameCore::instance()->heroFire(*pMsgInfo);
}

void ClientLogic::UIFishDead(void *pMsg)
{
    E2L_FISH_DEAD *pMsgInfo = (E2L_FISH_DEAD*)pMsg;
    GameCore::instance()->catchFish(*pMsgInfo);
}

void ClientLogic::UIFishLight(void *pMsg)
{
    E2L_FISH_LIGHT_DEAD *pMsgInfo = (E2L_FISH_LIGHT_DEAD*)pMsg;
    GameCore::instance()->lightFish(*pMsgInfo);
}
void ClientLogic::UIChooseCannon(void *pMsg)
{
    E2L_CHOOSE_CANNON *pMsgInfo = (E2L_CHOOSE_CANNON*)pMsg;
    GameCore::instance()->chooseCannon(*pMsgInfo);
}

void ClientLogic::UIChangeCannonMul(void *pMsg)
{
    E2L_CHANGE_CANNON_MULTIPLY *pMsgInfo = (E2L_CHANGE_CANNON_MULTIPLY*)pMsg;
    GameCore::instance()->changeCannonMul(*pMsgInfo);
}

void ClientLogic::UIUseSkill(void *pMsg)
{
    E2L_USE_SKILL *pMsgInfo = (E2L_USE_SKILL*)pMsg;
    GameCore::instance()->useSkill(*pMsgInfo);
}

void ClientLogic::UIFishClear(void *pMsg)
{
    E2L_FISH_ALL_REMOVE *pMsgInfo = (E2L_FISH_ALL_REMOVE*)pMsg;
    GameCore::instance()->fishCleared();
}

void ClientLogic::UINiuNiuSplashOver(void *pMsg)
{
    GameCore::instance()->enterNiuNiu();
}

void ClientLogic::UIShowHand(void *pMsg)
{
    E2L_NIUNIU_ASK_SHOWHAND *pMsgInfo = (E2L_NIUNIU_ASK_SHOWHAND*)pMsg;
    GameCore::instance()->askShowHand();
}
void ClientLogic::UINiuNiuEnd(void *pMsg)
{
    E2L_NIUNIU_END *pMsgInfo = (E2L_NIUNIU_END*)pMsg;
    GameCore::instance()->niuniuEnd(*pMsgInfo);
}

void ClientLogic::UIGetFromStageBox(void *pMsg)
{
    E2L_OPEN_BOX	*pMsgInfo	= (E2L_OPEN_BOX*)pMsg;
    GoldGame::instance()->getFromStageBox(*pMsgInfo);
}

void ClientLogic::UIGetFromFateBox(void *pMsg)
{
    E2L_OPEN_BOX	*pMsgInfo	= (E2L_OPEN_BOX*)pMsg;
    GoldGame::instance()->getFromFateBox(*pMsgInfo);
}

void ClientLogic::UIGetFromFateRoll(void *pMsg)
{
    E2L_OPEN_BOX	*pMsgInfo	= (E2L_OPEN_BOX*)pMsg;
    GoldGame::instance()->getFromFateRoll(*pMsgInfo);
}

void ClientLogic::UIBack2Diff(void *pMsg)
{
    ChangeState(GAME_STATE_GOLDEN_DIFF);
}

void ClientLogic::UIBack2Stage(void *pMsg)
{
    ChangeState(GAME_STATE_SEL_GOLDEN);
}
void ClientLogic::UIMarySplashOver(void *pMsg)
{
    GameCore::instance()->enterMary();
}
void ClientLogic::UIMaryInit(void *pMsg)
{
    GameCore::instance()->initMary();
}

void ClientLogic::UIMaryExit(void *pMsg)
{
    GameCore::instance()->exitMary();
}

void ClientLogic::UIShowLuckyDraw(void *pMsg)
{
    GameCore::instance()->enterLuckyDraw();
}

void ClientLogic::UIExitLuckyDraw(void *pMsg)
{
    GameCore::instance()->exitLuckyDraw();
}

void ClientLogic::UIUpdateLuckyDraw(void *pMsg)
{
    E2L_UPDATE_LUCKY_DRAW	*pMsgInfo	= (E2L_UPDATE_LUCKY_DRAW*)pMsg;
    GameCore::instance()->updateLuckyDraw(*pMsgInfo);
}

void ClientLogic::UIEnterLottery(void *pMsg)
{
    GameCore::instance()->enterLottery();
}

void ClientLogic::UIEndLottery(void *pMsg)
{
    GameCore::instance()->endLottery();
}

void ClientLogic::UIUnlockCannonLevel(void *pMsg)
{
    GameCore::instance()->unlockCannonLevel();
}

void ClientLogic::UIChargeUnlockLevel(void *pMsg)
{
    E2L_CHARGE_UNLOCK_LEVEL *pMsgInfo = (E2L_CHARGE_UNLOCK_LEVEL*)pMsg;
    GameCore::instance()->chargeUnlockLevel(*pMsgInfo);
}

void ClientLogic::UIShowSetting(void *pMsg)
{
    GameCore::instance()->showSetting();
}

void ClientLogic::UIExitSetting(void *pMsg)
{
    GameCore::instance()->exitSetting();
}

void ClientLogic::UICollectBossBound(void *pMsg)
{
    GameCore::instance()->collectBossBound();
}

void ClientLogic::UIShowTaskBound(void *pMsg)
{
    GameCore::instance()->showTask();
}

void ClientLogic::UITakeTaskBound(void *pMsg)
{
    GameCore::instance()->takeTaskBound();
}

void ClientLogic::UIHideUnlockLevel(void *pMsg)
{
    GameCore::instance()->hideUnlockLevel();
}

void ClientLogic::UIHideTaskBound(void *pMsg)
{
    GameCore::instance()->hideTaskBound();
}

void ClientLogic::UINextStage(void *pMsg)
{
    GameCore::instance()->nextStage();
}

void ClientLogic::UIQuitStage(void *pMsg)
{
    GameCore::instance()->quitStage();
}

void ClientLogic::UIShowDiary(void *pMsg)
{
    GameCore::instance()->showDiary();
}

void ClientLogic::UIReceiveDropItem(void *pMsg)
{
    E2L_RECEIVE_DROP_ITEM *pMsgInfo = (E2L_RECEIVE_DROP_ITEM*)pMsg;
    GameCore::instance()->removeDrop(pMsgInfo->dropIncreaceId);
}

void ClientLogic::UIShowFishSetting(void *pMsg)
{
    GameCore::instance()->showFishSetting();
}

void ClientLogic::UIShowCannonHold(void *pMsg)
{
    GameCore::instance()->showCannonHold();
}

void ClientLogic::UICannonUpgradeVip(void *pMsg)
{
    GameCore::instance()->cannonUpgradeVip();
}

void ClientLogic::UITakeRelife(void *pMsg)
{
    GameCore::instance()->takeRelife();
}

void ClientLogic::UIUnlock2Require(void *pMsg)
{
    E2L_UNLOCK_TO_REQUIRE *pMsgInfo = (E2L_UNLOCK_TO_REQUIRE*)pMsg;
    GameCore::instance()->unlock2Require(*pMsgInfo);
}

void ClientLogic::UIHideMultiNotEnough(void *pMsg)
{
    GameCore::instance()->hideMultiNotEnough();
}

void ClientLogic::UIShowHallRecharge(void *pMsg)
{
    E2L_SHOW_HALL_RECHARGE *pMsgInfo = (E2L_SHOW_HALL_RECHARGE*)pMsg;
    GameCore::instance()->showHallRecharge(pMsgInfo->type);
}

void ClientLogic::UIShowGameRecharge(void *pMsg)
{
    E2L_SHOW_GAME_RECHARGE *pMsgInfo = (E2L_SHOW_GAME_RECHARGE*)pMsg;
    GameCore::instance()->showBattleRecharge(*pMsgInfo);
}

void ClientLogic::UIShowVipIntro(void *pMsg)
{
    GameCore::instance()->showVipIntro();
}

void ClientLogic::UIShowVipGift(void *pMsg)
{
    GameCore::instance()->showVipGift();
}

void ClientLogic::UIShowVip(void *pMsg)
{
    GameCore::instance()->showVip();
}

void ClientLogic::UIRecharge(void *pMsg)
{
    E2L_RECHARGE *pMsgInfo = (E2L_RECHARGE*)pMsg;
    GameCore::instance()->recharge(*pMsgInfo);
}

void ClientLogic::UICloseRecharge(void *pMsg)
{
    GameCore::instance()->closeRecharge();
}

void ClientLogic::UICheckAutoFire(void *pMsg)
{
    GameCore::instance()->checkAutoFire();
}

void ClientLogic::UIShowBenefits(void *pMsg)
{
    E2L_SHOW_BENEFITS *pMsgInfo = (E2L_SHOW_BENEFITS*)pMsg;
    GameCore::instance()->showBenefits(*pMsgInfo);
}

void ClientLogic::UICashBenefitCode(void *pMsg)
{
    E2L_CASH_BENEFIT_CODE *pMsgInfo = (E2L_CASH_BENEFIT_CODE*)pMsg;
    GameCore::instance()->cashBenefitCode(*pMsgInfo);
}

void ClientLogic::UILink2NetGame(void *pMsg)
{
    E2L_LINK_TO_NETGAME *pMsgInfo = (E2L_LINK_TO_NETGAME*)pMsg;
    GameCore::instance()->link2NetGame(*pMsgInfo);
}

void ClientLogic::UIComment(void *pMsg)
{
    E2L_COMMENT *pMsgInfo = (E2L_COMMENT*)pMsg;
    GameCore::instance()->openCommnet(*pMsgInfo);
}

void ClientLogic::UIShowShare(void *pMsg)
{
    GameCore::instance()->showShare();
}

void ClientLogic::UIShare(void *pMsg)
{
    E2L_SHARE *pMsgInfo = (E2L_SHARE*)pMsg;
    GameCore::instance()->share(*pMsgInfo);
}

void ClientLogic::UITaskShareCountBound(void *pMsg)
{
    E2L_SHARE_COUNT_BOUND *pMsgInfo = (E2L_SHARE_COUNT_BOUND*)pMsg;
    GameCore::instance()->takeShareCountBound(*pMsgInfo);
}
