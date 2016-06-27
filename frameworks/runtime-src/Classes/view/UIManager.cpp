//
//  UIManager.cpp
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#include "UIManager.h"
#include "scene/LoadLogin.h"
#include "scene/LoginScene.h"
#include "scene/HallScene.h"
#include "scene/GameScene.h"
#include "scene/LoadGoldScene.h"
#include "scene/GoldGameScene.h"
#include "scene/GoldDifSelScene.h"
#include "scene/GoldStageScene.h"
#include "scene/GoldRunnerScene.h"
#include "scene/FishGameLoading.h"

#include "cocos2d.h"
#include "logic/ClientLogic.h"


USING_NS_CC;

UIManager::UIManager()
{
    memset(m_Logic2EngineFunc, 0, sizeof(m_Logic2EngineFunc));
    
    m_Logic2EngineFunc[l2e_load_login] = &UIManager::showLoadLogin;
    m_Logic2EngineFunc[l2e_load_login_update] = &UIManager::updateLoadLogin;
    m_Logic2EngineFunc[l2e_login] = &UIManager::showLogin;
    m_Logic2EngineFunc[l2e_login_result] = &UIManager::showLoginResult;
    m_Logic2EngineFunc[l2e_to_hall] = &UIManager::showHall;
    m_Logic2EngineFunc[l2e_enter_load_scene] = &UIManager::showGameLoading;
    m_Logic2EngineFunc[l2e_update_load_stage] = &UIManager::updateGameLoading;
    m_Logic2EngineFunc[l2e_enter_game] = &UIManager::showGame;
    m_Logic2EngineFunc[l2e_load_gold] = &UIManager::updateLoadGold;
    m_Logic2EngineFunc[l2e_result_goldgame] = &UIManager::showGoldResult;
    m_Logic2EngineFunc[l2e_show_gold_diff] = &UIManager::showGoldDiff;
    m_Logic2EngineFunc[l2e_update_gold_diff] = &UIManager::updateGoldDiff;
    m_Logic2EngineFunc[l2e_show_sel_gold] = &UIManager::showGoldStage;
    m_Logic2EngineFunc[l2e_update_sel_gold] = &UIManager::updateGoldStage;
    m_Logic2EngineFunc[l2e_show_gold_runner] = &UIManager::showGoldRunner;
    m_Logic2EngineFunc[l2e_open_box] = &UIManager::openBox;
    m_Logic2EngineFunc[l2e_update_gold_runner] = &UIManager::updateGoldRunner;
    m_Logic2EngineFunc[l2e_start_gold_roll] = &UIManager::startGoldRoll;
    m_Logic2EngineFunc[l2e_create_fish] = &UIManager::createFish;
    m_Logic2EngineFunc[l2e_schedule_tide] = &UIManager::scheduleTide;
    m_Logic2EngineFunc[l2e_tide_come] = &UIManager::tideComeTip;
    m_Logic2EngineFunc[l2e_tide_switch] = &UIManager::tideSwitchBg;
    m_Logic2EngineFunc[l2e_cannon_fire] = &UIManager::heroFire;
    m_Logic2EngineFunc[l2e_create_cannon] = &UIManager::createCannon;
    m_Logic2EngineFunc[l2e_cast_net] = &UIManager::castNet;
    m_Logic2EngineFunc[l2e_change_cannon] = &UIManager::changeCannon;
    m_Logic2EngineFunc[l2e_change_cannon_mul] = &UIManager::changeCannonMul;
    m_Logic2EngineFunc[l2e_boss_come] = &UIManager::bossComeTip;
    
    m_Logic2EngineFunc[l2e_ready_ray] = &UIManager::readyRay;
    m_Logic2EngineFunc[l2e_ready_plane] =  &UIManager::readyPlane;
//    m_Logic2EngineFunc[l2e_end_ray] = &UIManager::endRay;
    
    m_Logic2EngineFunc[l2e_niuniu_splash] = &UIManager::splashNiuNiu;
    m_Logic2EngineFunc[l2e_enter_niuniu] = &UIManager::enterNiuNiu;
    m_Logic2EngineFunc[l2e_niuniu_showhand] = &UIManager::showHand;
    m_Logic2EngineFunc[l2e_hide_niuniu] = &UIManager::exitNiuNiu;
    m_Logic2EngineFunc[l2e_mary_splash] = &UIManager::splashMary;
    m_Logic2EngineFunc[l2e_enter_mary] = &UIManager::enterMary;
    m_Logic2EngineFunc[l2e_mary_init] = &UIManager::maryInit;
    m_Logic2EngineFunc[l2e_hide_mary] = &UIManager::exitMary;
    
    m_Logic2EngineFunc[l2e_show_lucky_draw] = &UIManager::enterLuckyDraw;
    m_Logic2EngineFunc[l2e_exit_lucky_draw] = &UIManager::exitLuckyDraw;
    m_Logic2EngineFunc[l2e_update_lucky_draw] = &UIManager::updateLuckyDraw;
    m_Logic2EngineFunc[l2e_show_lottery] = &UIManager::enterLottery;
    m_Logic2EngineFunc[l2e_end_lottery] = &UIManager::endLottery;
    
    m_Logic2EngineFunc[l2e_cannon_max_level] = &UIManager::hideCannonLevel;
    m_Logic2EngineFunc[l2e_cannon_unlock_level] = &UIManager::unlockCannonLevel;
    m_Logic2EngineFunc[l2e_cannon_update_level] = &UIManager::updateCannonLevel;
    m_Logic2EngineFunc[l2e_cannon_show_unlock_dialog] = &UIManager::showCannonUnlockDialog;
    m_Logic2EngineFunc[l2e_charge_unlock_level] = &UIManager::chargeUnlockLevel;
    m_Logic2EngineFunc[l2e_cannon_hide_unlock_dialog] = &UIManager::hideUnlockLevel;
    m_Logic2EngineFunc[l2e_cannon_multi_not_enough] = &UIManager::showCannonMulNotEnough;
    m_Logic2EngineFunc[l2e_hide_multi_not_enough] = &UIManager::hideCannonMulNotEnough;
    m_Logic2EngineFunc[l2e_cannon_unlock_need_diamond] = &UIManager::showUnlockNeedDiamondTip;
    m_Logic2EngineFunc[l2e_lack_gold] = &UIManager::showGoldLackTip;
    
    m_Logic2EngineFunc[l2e_update_relife] = &UIManager::updateRelifeButton;
    m_Logic2EngineFunc[l2e_update_big_star] = &UIManager::updateStarBox;
    m_Logic2EngineFunc[l2e_update_gold] = &UIManager::updateGold;
    m_Logic2EngineFunc[l2e_update_diamond] = &UIManager::updateDiamond;
    m_Logic2EngineFunc[l2e_update_energy] = &UIManager::updateEnergy;
    m_Logic2EngineFunc[l2e_update_headInfo] = &UIManager::updateHeadInfo;
    m_Logic2EngineFunc[l2e_levelup_tip] = &UIManager::levelupTip;
    m_Logic2EngineFunc[l2e_update_skill] = &UIManager::updateSkillCount;
    m_Logic2EngineFunc[l2e_use_skill] = &UIManager::useSkill;
    m_Logic2EngineFunc[l2e_skill_charge] = &UIManager::chargeSkill;
    m_Logic2EngineFunc[l2e_charge_use_skill] = &UIManager::chargeUseSkill;
    m_Logic2EngineFunc[l2e_bomb_need_unlock] = &UIManager::showBombNeedUnlockTip;
    
    m_Logic2EngineFunc[l2e_show_setting] = &UIManager::showSetting;
    m_Logic2EngineFunc[l2e_exit_setting] = &UIManager::exitSetting;
    m_Logic2EngineFunc[l2e_show_fish] = &UIManager::showFishSetting;
    m_Logic2EngineFunc[l2e_show_cannon_setting] = &UIManager::showCannonSetting;
    m_Logic2EngineFunc[l2e_update_cannon_setting] = &UIManager::updateCannonSetting;
    
    m_Logic2EngineFunc[l2e_update_boss_task] = &UIManager::updateBossTask;
    m_Logic2EngineFunc[l2e_init_boss_task] = &UIManager::initBossTask;
    m_Logic2EngineFunc[l2e_complete_boss_task] = &UIManager::completeBossTask;
    
    m_Logic2EngineFunc[l2e_init_task] = &UIManager::initTask;
    m_Logic2EngineFunc[l2e_show_task_bound] = &UIManager::showTaskBound;
    m_Logic2EngineFunc[l2e_complete_sub_task] = &UIManager::completeSubTask;
    m_Logic2EngineFunc[l2e_update_task] = &UIManager::updateTask;
    m_Logic2EngineFunc[l2e_take_task_bound] = &UIManager::takeTaskBound;
    m_Logic2EngineFunc[l2e_hide_task_bound] = &UIManager::hideTaskBound;
    
    m_Logic2EngineFunc[l2e_tide_come_disable_skill] = &UIManager::tipDisableSkill;
    
    m_Logic2EngineFunc[l2e_show_diary] = &UIManager::showDiary;
    
    m_Logic2EngineFunc[l2e_drop_item] = &UIManager::dropItem;
    
    m_Logic2EngineFunc[l2e_show_hall_recharge] = &UIManager::showHallRecharge;
    m_Logic2EngineFunc[l2e_show_vip_info] = &UIManager::showVipIntro;
    m_Logic2EngineFunc[l2e_show_vip_gift] = &UIManager::showVipGift;
    m_Logic2EngineFunc[l2e_ray_need_vip] = &UIManager::showRayNeedVipTip;
    m_Logic2EngineFunc[l2e_auto_fire_need_vip] = &UIManager::showAutoFireNeedVipTip;
    m_Logic2EngineFunc[l2e_check_auto_fire] = &UIManager::canUseAutoFire;
    m_Logic2EngineFunc[l2e_update_hall_money] = &UIManager::updateHallMoney;
    m_Logic2EngineFunc[l2e_recharge_ok] = &UIManager::rechargeOk;
    m_Logic2EngineFunc[l2e_close_recharge] = &UIManager::closeRecharge;
    
    m_Logic2EngineFunc[l2e_show_benefits] = &UIManager::showBenefits;
    m_Logic2EngineFunc[l2e_cash_benefit_code] = &UIManager::cashBenefitCode;
    
    m_Logic2EngineFunc[l2e_show_share] = &UIManager::showShare;
    m_Logic2EngineFunc[l2e_share_result] = &UIManager::updateShareResult;
    m_Logic2EngineFunc[l2e_share_count_bound] = &UIManager::takeShareCountBound;
}

void UIManager::loop(float dt)
{
    ClientLogic::instance()->mainLoop(dt);
}

void UIManager::processLogicResponse(void *pMsg)
{
    if (NULL == pMsg)
    return;
    
    L2EProtocol	eProtocol	= *((L2EProtocol*)pMsg);
    if (eProtocol >= l2e_max)
        return;
    if (this->m_Logic2EngineFunc[eProtocol] != NULL)
    {
        (this->*m_Logic2EngineFunc[eProtocol])(pMsg);
    }
}

void UIManager::showGame(void *pMsg)
{
    L2E_ENTERGAME info = *(L2E_ENTERGAME*)(pMsg);
    auto scene = GameScene::create();
    scene->setSceneId(info.scnenId);
    scene->setupView();
    Director::getInstance()->replaceScene(scene);
}

void UIManager::showGameLoading(void *pMsg)
{
    auto scene = FishGameLoading::create();
    Director::getInstance()->replaceScene(scene);
}

void UIManager::updateGameLoading(void *pMsg)
{
    EventCustom event(UI_UPDATE_LOAD_STAGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHall(void *pMsg)
{
    L2E_TO_HALL info = *(L2E_TO_HALL*)(pMsg);
    auto scene = HallScene::create();
    scene->setupView(info);
    Director::getInstance()->replaceScene(scene);
}

void UIManager::showLoginResult(void *pMsg)
{
    L2E_LOGIN_RESULTE result = *(L2E_LOGIN_RESULTE*)(pMsg);
    if (!result.success) {
        EventCustom event(LOGIN_FAILED);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }else{
        
    }
}
void UIManager::updateLoadGold(void *pMsg)
{
    L2E_LOAD_GOLD_UPDATE result = *(L2E_LOAD_GOLD_UPDATE*)(pMsg);
    if (result.count == 0 && !result.isOver) {
        auto scene = LoadGoldScene::create();
        Director::getInstance()->replaceScene(scene);
    }else if (result.isOver){
        auto scene = GoldGameScene::create();
        Director::getInstance()->replaceScene(scene);
    }else{
        EventCustom event(LOAD_GOLD_UPDATE_EVENT);
        event.setUserData(pMsg);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void UIManager::showGoldResult(void *pMsg)
{
//    L2E_GOLD_RESULT result = *(L2E_GOLD_RESULT*)(pMsg);
    EventCustom event(GOLD_RESULT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showGoldDiff(void *pMsg)
{
    auto scene = GoldDifSelScene::create();
    Director::getInstance()->replaceScene(scene);
    
    EventCustom event(UPDATE_GOLD_DIFF_EVENT);
    event.setUserData(pMsg);
    scene->updateData(&event);
}

void UIManager::updateGoldDiff(void *pMsg)
{
    EventCustom event(UPDATE_GOLD_DIFF_EVENT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showGoldStage(void *pMsg)
{
    auto scene = GoldStageScene::create();
    Director::getInstance()->replaceScene(scene);
    
    
    EventCustom event(UPDATE_GOLD_STAGE_EVENT);
    event.setUserData(pMsg);
    scene->updateData(&event);
}

void UIManager::updateGoldRunner(void *pMsg)
{
    EventCustom event(UPDATE_GOLD_RUNNER_EVENT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::startGoldRoll(void *pMsg)
{
    EventCustom event(GOLD_START_ROLL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showGoldRunner(void *pMsg)
{
    auto scene = GoldRunnerScene::create();
    Director::getInstance()->replaceScene(scene);
    
    EventCustom event(UPDATE_GOLD_RUNNER_EVENT);
    event.setUserData(pMsg);
    //    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    scene->updateData(&event);
}

void UIManager::updateGoldStage(void *pMsg)
{
    EventCustom event(UPDATE_GOLD_STAGE_EVENT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//    scene->updateData(&event);
}

void UIManager::scheduleTide(void *pMsg)
{
    EventCustom event(SCHEDULE_TIDE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    //    scene->updateData(&event);
}

void UIManager::tideComeTip(void *pMsg)
{
    EventCustom event(TIDE_TIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::bossComeTip(void *pMsg)
{
    EventCustom event(BOSS_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::tideSwitchBg(void *pMsg)
{
    EventCustom event(TIDE_SWITCH);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::heroFire(void *pMsg)
{
    EventCustom event(HERO_FIRE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::createCannon(void *pMsg)
{
    EventCustom event(CREATE_CANNON);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::castNet(void *pMsg)
{
    EventCustom event(CAST_NET);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::changeCannon(void *pMsg)
{
    EventCustom event(CHANGE_CANNON);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::changeCannonMul(void *pMsg)
{
    EventCustom event(CHANGE_CANNON_MULTIPLY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showLogin(void *pMsg)
{
    auto scene = LoginScene::create();
    Director::getInstance()->replaceScene(scene);
}

void UIManager::showLoadLogin(void *pMsg)
{
    auto scene = LoadLogin::create();
    Director::getInstance()->runWithScene(scene);
}

void UIManager::updateLoadLogin(void *pMsg)
{
    EventCustom event(LOAD_LOGIN_UPDATE_EVENT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::openBox(void *pMsg)
{
    EventCustom event(GOLD_OPEN_BOX);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::createFish(void *pMsg)
{
    EventCustom event(CREATE_FISH);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::readyRay(void *pMsg)
{
    EventCustom event(UI_READY_RAY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::readyPlane(void *pMsg)
{
    EventCustom event(UI_READY_PLANE);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
//void UIManager::endRay(void *pMsg)
//{
//    EventCustom event(UI_END_RAY);
//    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//}

void UIManager::splashNiuNiu(void *pMsg)
{
    EventCustom event(UI_SPLASH_NIUNIU);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::enterNiuNiu(void *pMsg)
{
    EventCustom event(UI_ENTER_NIUNIU);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::splashMary(void *pMsg)
{
    EventCustom event(UI_SPLASH_MARY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::enterMary(void *pMsg)
{
    EventCustom event(UI_ENTER_MARY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
void UIManager::showHand(void *pMsg)
{
    EventCustom event(UI_SHOW_HAND);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::maryInit(void *pMsg)
{
    EventCustom event(UI_MARY_INIT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::unlockCannonLevel(void *pMsg)
{
    EventCustom event(UI_UNLOCK_CANNON_LEVEL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateCannonLevel(void *pMsg)
{
    EventCustom event(UI_UPDATE_CANNON_LEVEL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::hideCannonLevel(void *pMsg)
{
    EventCustom event(UI_HIDE_CANNON_LEVEL);

    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showCannonUnlockDialog(void *pMsg)
{
    EventCustom event(UI_SHOW_CANNON_UNLOCK_DIALOG);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::chargeUnlockLevel(void *pMsg)
{
    EventCustom event(UI_CHARGE_UNLOCK_LEVEL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
void UIManager::hideUnlockLevel(void *pMsg)
{
    EventCustom event(UI_HIDE_UNLOCK_LEVEL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateGold(void *pMsg)
{
    EventCustom event(UI_UPDATE_GOLD);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateDiamond(void *pMsg)
{
    EventCustom event(UI_UPDATE_DIAMOND);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateEnergy(void *pMsg)
{
    EventCustom event(UI_UPDATE_ENERGY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateHeadInfo(void *pMsg)
{
    EventCustom event(UI_UPDATE_HEADINFO);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::exitSetting(void *pMsg)
{
    EventCustom event(UI_HIDE_SETTING);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showSetting(void *pMsg)
{
    EventCustom event(UI_SHOW_SETTING);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showFishSetting(void *pMsg)
{
    EventCustom event(UI_SHOW_FISH_SETTING);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateBossTask(void *pMsg)
{
    EventCustom event(UI_UPDATE_BOSS_TASK);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::initBossTask(void *pMsg)
{
    EventCustom event(UI_INIT_BOSS_TASK);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::completeBossTask(void *pMsg)
{
    EventCustom event(UI_COMP_BOSS_TASK);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::initTask(void *pMsg)
{
    EventCustom event(UI_INIT_TASK);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showTaskBound(void *pMsg)
{
    EventCustom event(UI_SHOW_TASK_BOUND);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::completeSubTask(void *pMsg)
{
    EventCustom event(UI_COMP_SUB_TASK);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateTask(void *pMsg)
{
    EventCustom event(UI_UPDATE_TASK);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::exitNiuNiu(void *pMsg)
{
    EventCustom event(UI_HIDE_NIUNIU);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::exitMary(void *pMsg)
{
    EventCustom event(UI_HIDE_MARY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::takeTaskBound(void *pMsg)
{
    EventCustom event(UI_TAKE_TASK_BOUND);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::hideTaskBound(void *pMsg)
{
    EventCustom event(UI_HIDE_TASK_BOUND);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::levelupTip(void *pMsg)
{
    EventCustom event(UI_LEVELUP_TIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateSkillCount(void *pMsg)
{
    EventCustom event(UI_UPDATE_SKILL_COUNT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::useSkill(void *pMsg)
{
    EventCustom event(UI_USE_SKILL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::chargeSkill(void *pMsg)
{
    EventCustom event(UI_SKILL_CHARGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::chargeUseSkill(void *pMsg)
{
    EventCustom event(UI_CHARGE_USE_SKILL);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::tipDisableSkill(void *pMsg)
{
    EventCustom event(UI_DISABLE_SKILL_TIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::enterLuckyDraw(void *pMsg)
{
    EventCustom event(UI_ENTER_LUCKY_DRAW);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::exitLuckyDraw(void *pMsg)
{
    EventCustom event(UI_EXIT_LUCKY_DRAW);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateLuckyDraw(void *pMsg)
{
    EventCustom event(UI_UPDATE_LUCKY_DRAW);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::enterLottery(void *pMsg)
{
    EventCustom event(UI_ENTER_LOTTERY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::endLottery(void *pMsg)
{
    EventCustom event(UI_END_LOTTERY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateStarBox(void *pMsg)
{
    EventCustom event(UI_UPDATE_STAR_BOX);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showDiary(void *pMsg)
{
    EventCustom event(UI_SHOW_DIARY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::dropItem(void *pMsg)
{
    EventCustom event(UI_DROP_ITEM);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showCannonSetting(void *pMsg)
{
    EventCustom event(UI_SHOW_CANNON_SETTING);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateCannonSetting(void *pMsg)
{
    EventCustom event(UI_UPDATE_CANNON_SETTING);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateRelifeButton(void *pMsg)
{
    EventCustom event(UI_UPDATE_RELIFE_BUTTON);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showCannonMulNotEnough(void *pMsg)
{
    EventCustom event(UI_MULTIPLY_NOT_ENOUGH);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::hideCannonMulNotEnough(void *pMsg)
{
    EventCustom event(UI_HIDE_MULTIPLY_NOT_ENOUGH);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showGoldLackTip(void *pMsg)
{
    EventCustom event(UI_LACK_GOLD_TIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showHallRecharge(void *pMsg)
{
    EventCustom event(UI_SHOW_HALL_RECHARGE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showVipIntro(void *pMsg)
{
    EventCustom event(UI_SHOW_VIP_INTRO);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showVipGift(void *pMsg)
{
    EventCustom event(UI_SHOW_VIP_GIFT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showRayNeedVipTip(void *pMsg)
{
    EventCustom event(UI_SHOW_RAY_NEED_VIP_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showAutoFireNeedVipTip(void *pMsg)
{
    EventCustom event(UI_SHOW_AUTO_FIRE_NEED_VIP_TIP);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::canUseAutoFire(void *pMsg)
{
    EventCustom event(UI_CAN_USE_AUTO_FIRE);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateHallMoney(void *pMsg)
{
    EventCustom event(UI_UPDATE_HALL_MONEY);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::rechargeOk(void *pMsg)
{
    EventCustom event(UI_RECHARGE_OK);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::closeRecharge(void *pMsg)
{
    EventCustom event(UI_CLOSE_RECHARGE);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showBenefits(void *pMsg)
{
    EventCustom event(UI_SHOW_BENEFITS);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::cashBenefitCode(void *pMsg)
{
    EventCustom event(UI_CASH_BENEFIT_CODE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showShare(void *pMsg)
{
    EventCustom event(UI_SHOW_SHARE);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::updateShareResult(void *pMsg)
{
    EventCustom event(UI_UPDATE_SHARE_RESULT);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::takeShareCountBound(void *pMsg)
{
    EventCustom event(UI_TAKE_SHARE_COUNT_BOUND);
    event.setUserData(pMsg);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showBombNeedUnlockTip(void *pMsg)
{
    EventCustom event(UI_SHOW_BOMB_NEED_UNLOCK_TIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void UIManager::showUnlockNeedDiamondTip(void *pMsg)
{
    EventCustom event(UI_SHOW_UNLOCK_NEED_DIAMOND_TIP);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
