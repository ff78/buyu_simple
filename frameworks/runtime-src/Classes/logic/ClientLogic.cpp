//
//  ClientLogic.cpp
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#include "ClientLogic.h"
#include "view/UIManager.h"
#include "server/ServerManager.h"
#include "Login.h"
#include "GoldGame.h"
#include "GameCore.h"


ClientLogic::ClientLogic()
{
    m_eLastState = GAME_STATE_INIT;
    m_eState = GAME_STATE_INIT;
    
    memset(m_UI2LogicFunc, 0, sizeof(m_UI2LogicFunc));
    memset(m_LogicStateFunc, 0, sizeof(m_LogicStateFunc));
    
    //处理显示层请求的函数对应关系
    m_UI2LogicFunc[e2l_login] = &ClientLogic::UIRequestLogin;
    m_UI2LogicFunc[e2l_enter_game] = &ClientLogic::UIRequestEnterGame;
    m_UI2LogicFunc[e2l_gold_diff] = &ClientLogic::UIRequestDiff;
    m_UI2LogicFunc[e2l_gold_start_diff] = &ClientLogic::UISelDiff;
    m_UI2LogicFunc[e2l_sel_gold] = &ClientLogic::UIRequestStage;
    m_UI2LogicFunc[e2l_gold_stage_box] = &ClientLogic::UIOpenStageBox;
    m_UI2LogicFunc[e2l_gold_open_stage_box] = &ClientLogic::UIGetFromStageBox;
    m_UI2LogicFunc[e2l_gold_open_fate_box] = &ClientLogic::UIGetFromFateBox;
    m_UI2LogicFunc[e2l_gold_back_diff] = &ClientLogic::UIBack2Diff;
    m_UI2LogicFunc[e2l_gold_back_stage] = &ClientLogic::UIBack2Stage;
    m_UI2LogicFunc[e2l_gold_fate_box] = &ClientLogic::UIOpenFateBox;
    m_UI2LogicFunc[e2l_gold_start_runner] = &ClientLogic::UIStartRoll;
    m_UI2LogicFunc[e2l_gold_open_fate_roll] = &ClientLogic::UIGetFromFateRoll;
    m_UI2LogicFunc[e2l_gold_start_game] = &ClientLogic::UIStartGoldGame;
    m_UI2LogicFunc[e2l_gold_result] = &ClientLogic::UIGoldGameResult;
    m_UI2LogicFunc[e2l_gold_gain_obj] = &ClientLogic::UIGoldGainObj;
    m_UI2LogicFunc[e2l_remove_out_fish] = &ClientLogic::UIRemoveFish;
    m_UI2LogicFunc[e2l_tide_switch] = &ClientLogic::UITideSwitch;
    m_UI2LogicFunc[e2l_cannon_fire] = &ClientLogic::UIFire;
    m_UI2LogicFunc[e2l_fish_dead] = &ClientLogic::UIFishDead;
    m_UI2LogicFunc[e2l_fish_light_dead] = &ClientLogic::UIFishLight;
    m_UI2LogicFunc[e2l_show_cannon_setting] = &ClientLogic::UIShowCannonHold;
    m_UI2LogicFunc[e2l_cannon_upgrade_vip] = &ClientLogic::UICannonUpgradeVip;
    m_UI2LogicFunc[e2l_choose_cannon] = &ClientLogic::UIChooseCannon;
    m_UI2LogicFunc[e2l_change_cannon_multi] = &ClientLogic::UIChangeCannonMul;
    m_UI2LogicFunc[e2l_use_skill] = &ClientLogic::UIUseSkill;
    m_UI2LogicFunc[e2l_charge_skill] = &ClientLogic::UIChargeSkill;
    m_UI2LogicFunc[e2l_fish_all_remove] = &ClientLogic::UIFishClear;
    m_UI2LogicFunc[e2l_niuniu_splash_over] = &ClientLogic::UINiuNiuSplashOver;
    m_UI2LogicFunc[e2l_niuniu_ask_showHand] = &ClientLogic::UIShowHand;
    m_UI2LogicFunc[e2l_niuniu_end] = &ClientLogic::UINiuNiuEnd;
    m_UI2LogicFunc[e2l_mary_splash_over] = &ClientLogic::UIMarySplashOver;
    m_UI2LogicFunc[e2l_mary_init] = &ClientLogic::UIMaryInit;
    m_UI2LogicFunc[e2l_mary_exit] = &ClientLogic::UIMaryExit;
    m_UI2LogicFunc[e2l_cannon_unlock_level] = &ClientLogic::UIUnlockCannonLevel;
    m_UI2LogicFunc[e2l_charge_unlock_level] = &ClientLogic::UIChargeUnlockLevel;
    m_UI2LogicFunc[e2l_show_settings] = &ClientLogic::UIShowSetting;
    m_UI2LogicFunc[e2l_exit_settings] = &ClientLogic::UIExitSetting;
    m_UI2LogicFunc[e2l_show_fish] = &ClientLogic::UIShowFishSetting;
    m_UI2LogicFunc[e2l_boss_task_bounds] = &ClientLogic::UICollectBossBound;
    m_UI2LogicFunc[e2l_show_task_bounds] = &ClientLogic::UIShowTaskBound;
    m_UI2LogicFunc[e2l_take_task_bounds] = &ClientLogic::UITakeTaskBound;
    m_UI2LogicFunc[e2l_cannon_hide_unlock_level] = &ClientLogic::UIHideUnlockLevel;
    m_UI2LogicFunc[e2l_hide_task_bounds] = &ClientLogic::UIHideTaskBound;
    m_UI2LogicFunc[e2l_show_lucky_draw] = &ClientLogic::UIShowLuckyDraw;
    m_UI2LogicFunc[e2l_exit_lucky_draw] = &ClientLogic::UIExitLuckyDraw;
    m_UI2LogicFunc[e2l_update_lucky_draw] = &ClientLogic::UIUpdateLuckyDraw;
    m_UI2LogicFunc[e2l_show_lottery] = &ClientLogic::UIEnterLottery;
    m_UI2LogicFunc[e2l_end_lottery] = &ClientLogic::UIEndLottery;
    m_UI2LogicFunc[e2l_next_stage] = &ClientLogic::UINextStage;
    m_UI2LogicFunc[e2l_quit_stage] = &ClientLogic::UIQuitStage;
    m_UI2LogicFunc[e2l_show_diary] = &ClientLogic::UIShowDiary;
    m_UI2LogicFunc[e2l_receive_drop_item] = &ClientLogic::UIReceiveDropItem;
    m_UI2LogicFunc[e2l_take_relife] = &ClientLogic::UITakeRelife;
    m_UI2LogicFunc[e2l_unlock_to_require] = &ClientLogic::UIUnlock2Require;
    m_UI2LogicFunc[e2l_hide_multi_not_enough] = &ClientLogic::UIHideMultiNotEnough;
    
    m_UI2LogicFunc[e2l_show_hall_recharge] = &ClientLogic::UIShowHallRecharge;
    m_UI2LogicFunc[e2l_show_vip_intro] = &ClientLogic::UIShowVipIntro;
    m_UI2LogicFunc[e2l_show_vip_gift] = &ClientLogic::UIShowVipGift;
    m_UI2LogicFunc[e2l_show_vip] = &ClientLogic::UIShowVip;
    m_UI2LogicFunc[e2l_recharge] = &ClientLogic::UIRecharge;
    m_UI2LogicFunc[e2l_check_auto_fire] = &ClientLogic::UICheckAutoFire;
    m_UI2LogicFunc[e2l_close_recharge] = &ClientLogic::UICloseRecharge;
    m_UI2LogicFunc[e2l_show_game_recharge] = &ClientLogic::UIShowGameRecharge;
    
    m_UI2LogicFunc[e2l_show_benefits] = &ClientLogic::UIShowBenefits;
    m_UI2LogicFunc[e2l_cash_benefit_code] = &ClientLogic::UICashBenefitCode;
    m_UI2LogicFunc[e2l_link_to_netgame] = &ClientLogic::UILink2NetGame;
    
    m_UI2LogicFunc[e2l_show_share] = &ClientLogic::UIShowShare;
    m_UI2LogicFunc[e2l_share] = &ClientLogic::UIShare;
    m_UI2LogicFunc[e2l_share_count_bound] = &ClientLogic::UITaskShareCountBound;
    
    //处理服务层回应的函数对应关系
    m_Server2ClientFunc[s2c_recharge] = &ClientLogic::ResRecharge;
    m_Server2ClientFunc[s2c_cash_benefit_code] = &ClientLogic::ResCashBenefitCode;
    m_Server2ClientFunc[s2c_share] = &ClientLogic::ResShare;
    
    //逻辑主循环状态
    m_LogicStateFunc[GAME_STATE_INIT] = &ClientLogic::onStateInit;
    m_LogicStateFunc[GAME_STATE_LOADLOGIN] = &ClientLogic::onStateLoadLogin;
    m_LogicStateFunc[GAME_STATE_LOAD_GOLDEN] = &ClientLogic::onStateLoadGold;
    m_LogicStateFunc[GAME_STATE_GOLDEN_GAME]  = &ClientLogic::onStateGoldGame;
    m_LogicStateFunc[GAME_STATE_GAME] = &ClientLogic::onStateFishGame;
}

void ClientLogic::onStateInit(float dt)
{
    ChangeState(GAME_STATE_LOGIN);
}

void ClientLogic::onStateLoadLogin(float dt)
{
    Login::instance()->load();
}

void ClientLogic::onStateLoadGold(float dt)
{
    GoldGame::instance()->load();
}

void ClientLogic::onStateGoldGame(float dt)
{
    GoldGame::instance()->logic(dt);
}

void ClientLogic::onStateFishGame(float dt)
{
    GameCore::instance()->logic(dt);
}

//改变游戏状态
void ClientLogic::ChangeState(E_GAME_STATE eState)
{
    if(m_eState == eState)
        return;
    exitGameMode(m_eState);
    m_eLastState	= m_eState;
    m_eState		= eState;
    enterGameMode(eState);
}

//进入游戏状态前的准备处理
bool ClientLogic::enterGameMode(E_GAME_STATE next)
{
    switch (next) {
        case GAME_STATE_GOLDEN_RUNNER:
        {
            auto gameData = BlackGoldGame::instance();
            L2E_UPDATE_GOLD_RUNNER info;
            info.eProtocol = l2e_show_gold_runner;
            info.keyCount = gameData->getKeyCount();
            info.boxState = gameData->getFateBoxState();
            info.hasRolled = gameData->getHasRolled();
            info.errorCode = 0;
            pass2Engine(&info);
        }
            break;
        case GAME_STATE_SEL_GOLDEN:
        {
            auto gameData = BlackGoldGame::instance();
            L2E_UPDATE_SELGOLD info;
            info.eProtocol = l2e_show_sel_gold;
            info.lastStage = gameData->getDiffStage();
            info.boxState = gameData->getStageBoxState();
            info.hasFocus = gameData->getSucNextStage();
            info.errorCode = 0;
            pass2Engine(&info);
        }
            break;
        case GAME_STATE_GOLDEN_DIFF:
        {
            auto gameData = BlackGoldGame::instance();
            gameData->setTempChallengeCount(gameData->getChanllengeCount());
            L2E_GOLD_DIFF info;
            info.eProtocol = l2e_show_gold_diff;
            info.diff = gameData->getDifficulty();
            info.timeLimit = gameData->getChallengeLimit();
            info.validCount = gameData->getChanllengeCount();
            info.fee = gameData->getChallengeFee();
            info.errCode = 0;
            pass2Engine(&info);
        }
            break;
        case GAME_STATE_GOLDEN_GAME:
        {
            L2E_LOAD_GOLD_UPDATE info;
            info.eProtocol = l2e_load_gold;
            info.isOver = true;
            info.count = 0;
            info.step = 0;
            pass2Engine(&info);
        }
            break;
//        case GAME_STATE_LOAD_GAME:
//        {
//            
//        }
//            break;
        case GAME_STATE_GAME:
        {
            //todo 炮最低倍数限制
            GameCore::instance()->changeGameState(FISH_GAME_LOAD);
        }
            break;
            
        case GAME_STATE_HALL:
        {
            GameCore::instance()->changeGameState(FISH_GAME_HALL);
            
            L2E_TO_HALL info;
            info.eProtocol = l2e_to_hall;
            info.gold = GameCore::instance()->getTotalScore();
            info.diamond = GameCore::instance()->getTotalDiamond();
            info.level = GameCore::instance()->getLevel();
            info.exp = GameCore::instance()->getExpScore();
            info.maxExp = GameCore::instance()->getMaxExp();
            info.maxStage = GameCore::instance()->getMaxStage();
            info.currStage = GameCore::instance()->getCurrStage();
            info.maxUnlockStage = GameCore::instance()->getMaxUnlockStage();
            for (int i = 0; i < GameCore::instance()->stageMiniConfigs.size(); i++) {
                if (i >= info.maxStage) {
                    break;
                }
                Stage_Mini_Config stageConfig = GameCore::instance()->stageMiniConfigs[i+1];
                info.levelBossRes[i] = stageConfig.bossRes;
                info.levelBossType[i] = stageConfig.bossType;
                info.levelBoundGold[i] = stageConfig.boundsMoney;
                info.levelBoundDiamond[i] = stageConfig.boundsDiamond;
            }
            pass2Engine(&info);
        }
            break;
        case GAME_STATE_LOGIN:
        {
            LOAD_LOGIN_UPDATE info;
            info.eProtocol = l2e_login;
            info.isOver = true;
            info.step = 0;
            pass2Engine(&info);
        }
            break;
        case GAME_STATE_LOADLOGIN:
        {
            LOAD_LOGIN info;
            info.eProtocol = l2e_load_login;
            pass2Engine(&info);
        }
            break;
        case GAME_STATE_LOAD_GOLDEN:
        {
            L2E_LOAD_GOLD_UPDATE info;
            info.eProtocol = l2e_load_gold;
            info.isOver = false;
            info.count = 0;
            info.step = 0;
            pass2Engine(&info);
        }
            break;
            
        default:
            break;
    }
    return true;
}

//离开游戏状态的结束处理
void ClientLogic::exitGameMode(E_GAME_STATE currMode)
{
    switch (currMode) {
            
        default:
            break;
    }
}

//游戏逻辑主循环，由各个状态的逻辑函数具体处理。
void ClientLogic::mainLoop(float dt)
{
    if (this->m_LogicStateFunc[m_eState] == nullptr) {
        return;
    }
    (this->*m_LogicStateFunc[m_eState])(dt);
}

//发送结构体通知给显示层
void ClientLogic::pass2Engine(void *pMsg)
{
    UIManager::instance()->processLogicResponse(pMsg);
}

void ClientLogic::pass2Server(void *pMsg)
{
    ServerManager::instance()->processClientQuest(pMsg);
}
