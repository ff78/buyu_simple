//
//  GameCore.cpp
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#include "GameCore.h"
#include "ClientLogic.h"
#include "utils/GameReader.h"
#include "utils/GameUtils.h"
#include "utils/libs.h"
#include "tinyxml2/tinyxml2.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSHelper.h"
#endif


USING_NS_CC;
using namespace cocostudio;
using namespace tinyxml2;
using namespace rapidjson;

std::map<int, int> GameCore::idStageTable;
std::map<int, int> GameCore::levelIdTable;
std::map<int, int> GameCore::taskIdTable;
std::map<int, int> GameCore::relifeIdTable;
std::map<int, int> GameCore::rechargeIdTable;
std::map<int, int> GameCore::vipIdTable;

GameCore::GameCore()
:fishManager(nullptr),
bulletManager(nullptr),
cannonManager(nullptr),
skillManager(nullptr),
niuManager(nullptr),
maryManager(nullptr),
lotteryManager(nullptr)
{
    nextTideDuration = -1;
//    changeGameState(FISH_GAME_INIT);
    hitListener = cocos2d::EventListenerCustom::create(HIT_FISHES, CC_CALLBACK_1(GameCore::hitFishes, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hitListener, -1);
    rayHitListener = cocos2d::EventListenerCustom::create(RAY_HIT_FISHES, CC_CALLBACK_1(GameCore::rayHitFishes, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(rayHitListener, -1);
    bombListener = cocos2d::EventListenerCustom::create(BOMB_FISHES, CC_CALLBACK_1(GameCore::bombFishes, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(bombListener, -1);
    frozenListener = cocos2d::EventListenerCustom::create(FROZEN_SCREEN, CC_CALLBACK_1(GameCore::frozenScreen, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(frozenListener, -1);
    readyRayListener = cocos2d::EventListenerCustom::create(READY_RAY_FIRE, CC_CALLBACK_1(GameCore::readyRay, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(readyRayListener, -1);
    endRayListener = cocos2d::EventListenerCustom::create(END_RAY_FIRE, CC_CALLBACK_1(GameCore::endRayFire, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(endRayListener, -1);
    readyPlaneListener = cocos2d::EventListenerCustom::create(READY_PLANE_BOMB, CC_CALLBACK_1(GameCore::readyPlane, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(readyPlaneListener, -1);
    endPlaneListener = cocos2d::EventListenerCustom::create(END_PLANE_BOMB, CC_CALLBACK_1(GameCore::endPlane, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(endPlaneListener, -1);
    jniRechargeListener = cocos2d::EventListenerCustom::create(JNI_RECHARGE_OK, CC_CALLBACK_1(GameCore::jniRecharge, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(jniRechargeListener, -1);
    
    currState = FISH_GAME_NONE;
    currStage = 1;
    maxUnlockStage = 1;
    maxStage = 1;
    energyDuration = 0;

    totalScore = 3000;
    totalDiamond = 10;
    energyScore = 0;
    expScore = 0;
    maxExp = 1000;
    level = 1;
    vip = 0;
    rechargeRecord = 0;
    for (int i = 0; i < MAX_RECHARGE_ITEM; i++) {
        firstRecharge[i] = true;
    }
    stageTargetState = STAGE_TARGET_INIT;
    killBoss1Count = 0;
    killBoss2Count = 0;
    title = gTitles[level/10];
    currCannonMulti = 1;
    unlockCannonLvl = 1;
    currCannonType = CANNON_TYPE_XINSHOU;
    skillManager = SkillManager::instance();
    skillManager->loadConfig();
    for (int i = 0; i < 5; i++) {
        skillCount[i] = skillManager->skillMap[i+1].count;
    }
    
    for (int i = 0; i < MAX_TASK_COUNT; i++) {
        killTaskCount[i] = 0;
    }
    stageMissionState = TASK_INIT;
    
    bigFishCountMap.clear();
    
    dropItemIncreacement = 0;
    dropItemMap.clear();
    
    hasReceiveBenefit1 = false;
    hasReceiveBenefit2 = false;
    
    shareCount = 0;
    for (int i = 0; i < 4; i++) {
        shareCountState[i] = 0;
    }

    killBigCount = 0;
    killStarCount = 0;
    killBigScore = 0;
    
    probForceCount = 0;
    probForceRecharge = 0;
    probForceDuration = 0;
    probTotalDuration = 0;
    rebateDuration = 0;
    sceneProbDuration = 0;
    sceneProb = 1;
    probForceGold = totalScore;
    
    
    coreDataStr = "";
    
    evaluated = false;
    openedEvaluate = false;
    evaluateDay = "";
    
    jniRechargeOk = false;
    jniRechargeId = 0;
    
//    deleteJson();
    if (!loadJson()) {
        sceneProb = gRebateProb;
        sceneProbDuration = probTotalDuration = REBATE_TIME_LIMIT;
        save2CoreString();
        save2Json();
    }
    loadTaskJson();
    
}

GameCore::~GameCore()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(hitListener);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(frozenListener);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(readyRayListener);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(endRayListener);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(readyPlaneListener);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(endPlaneListener);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(bombListener);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(rayHitListener);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(jniRechargeListener);
    clearAll();
}

void GameCore::clearAll()
{
    fishManager->clear();
}
void GameCore::fishCleared()
{
    if (currState == FISH_GAME_TIDE_TIP) {
        changeGameState(FISH_GAME_SWITCH_TIDE);
    }
}
void GameCore::askShowHand()
{
    L2E_NINIU_REQ_SHOWHAND info;
    info.eProtocol = l2e_niuniu_showhand;
    info.cardType = niuManager->showHand();
    switch (info.cardType) {
        case NIU_0:
        {
            info.result = NIU_RESULT_0;
        }
            break;
        case NIU_10:
        {
            info.result = NIU_RESULT_NIU;
        }
            break;
        default:
        {
            if (info.cardType < NIU_7) {
                info.result = NIU_RESULT_SMALL;
            }else{
                info.result = NIU_RESULT_BIG;
            }
        }
            break;
    }
    ClientLogic::instance()->pass2Engine(&info);
}

//不会进入状态准备方法，返回上一个状态
void GameCore::backGameState()
{
    exitGameState(currState);
    currState = lastState;
    log("back to currState: %d", currState);
}

void GameCore::niuniuEnd(E2L_NIUNIU_END info)
{
    changeGold(info.result);
    backGameState();
    
    L2E_HIDE_NIUNIU hideInfo;
    hideInfo.eProtocol = l2e_hide_niuniu;
    ClientLogic::instance()->pass2Engine(&hideInfo);
}

void GameCore::logic(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    log("logic");
    if (jniRechargeOk) {
        log("jni recharge ok");
        jniRechargeOk = false;
        S2C_RECHARGE info;
        info.eProtocol = s2c_recharge;
        info.rechargeId = jniRechargeId;
        info.errNo = 0;
        jniRechargeId = 0;
        responseRecharge(info);
    }
#endif
    switch (currState) {
        case FISH_GAME_INIT:
            changeGameState(FISH_GAME_NORMAL);
            return;
        case FISH_GAME_LOAD:
        {
            load();
        }
            return;
        case FISH_GAME_TIDE:
        {
            fishManager->logic(dt);
            bulletManager->logic(dt);

            if (fishManager->fishMap.empty() && fishManager->delayCreateCmd.empty()) {
                changeGameState(FISH_GAME_NORMAL);
                break;
            }
            
        }
            break;
        case FISH_GAME_TIDE_TIP:
        {
            if (fishManager->fishMap.empty()) {
                changeGameState(FISH_GAME_SWITCH_TIDE);
                break;
            }
            fishManager->tideClear();
            fishManager->logic(dt);
            bulletManager->logic(dt);
        }
            break;
        case FISH_GAME_NORMAL:
        {
            fishManager->logic(dt);
            fishManager->createRandFish(dt);
            bulletManager->logic(dt);
            if (cannonManager->getHeroCannon().getState() == CANNON_NORMAL) {
                if (nextTideDuration > 0) {
                    nextTideDuration -= dt;
                    if (nextTideDuration <= 0) {
                        nextTideDuration = -1;
                        tideCome();
                    }
                }
            }
            
        }
            break;
        case FISH_GAME_NIUNIU:
        case FISH_GAME_MARY:
        case FISH_GAME_SETTING:
        case FISH_GAME_UPGRADE_LEVEL:
        case FISH_GAME_TASK_BOUND:
        case FISH_GAME_LOTTERY:
        case FISH_GAME_HALL:
        case FISH_GAME_BATTLE_RECHAGE:
        case FISH_GAME_NEED_VIP:
        {
            
        }
            return;
        default:
            break;
    }
    
    sceneProbDuration -= dt;
    if (sceneProbDuration <= 0) {
        //场景的命中倍率因子持续时间到时改变场景命中倍率因子
        changeSceneProb();
    }
    
    if (energyDuration > 0) {
        energyDuration -= dt;
        if (energyDuration <= 0) {
            //能量炮时间到时，结束能量炮形态，返回之前选择的炮
            endEnergyCannon();
        }
    }
    
    //金币数少于关卡最小倍率，进入救济金逻辑
    if (totalScore < cannonManager->levelConfigs[stageConfig.minMultiply].multiply) {
        
        if (relifeData.relifeState == 1) {
            //relifeState == 1 :倒数计时
            getRelifeFCS();
            relifeData.relifeCountDown -= dt;
            if (relifeData.relifeCountDown <= 0) {
                //relifeState == 1 :计时结束，可领取救济金
                relifeData.relifeCountDown = 0;
                relifeData.relifeState = 2;
                Relife_Config config;
                readRelifeConfig(relifeData.relifeId, config, RELIFE_CONFIG_FILE);
                L2E_UPDATE_RELIFE info;
                info.eProtocol = l2e_update_relife;
                info.state = 2;
                info.countDown = 0;
                info.bound = config.bounds;
                getVipFCS();
                if (vip >= 3) {
                    info.bound *= 2;
                }
                ClientLogic::instance()->pass2Engine(&info);
            }else{
                L2E_UPDATE_RELIFE info;
                info.eProtocol = l2e_update_relife;
                info.state = 1;
                info.countDown = relifeData.relifeCountDown;
                info.bound = 0;
                ClientLogic::instance()->pass2Engine(&info);
            }
            saveRelife2CS();
        }else if (relifeData.relifeState == 0 && bulletManager->bulletMap.size() == 0) {
            //relifeState == 0 :还未开始计时，等待所有子弹都消除掉
            getRelifeFCS();
            Relife_Config config;
            readRelifeConfig(relifeData.relifeId, config, RELIFE_CONFIG_FILE);
            relifeData.relifeCountDown = config.countDownLimits;
            relifeData.relifeState = 1;
            saveRelife2CS();
            
            L2E_UPDATE_RELIFE info;
            info.eProtocol = l2e_update_relife;
            info.state = 1;
            info.countDown = relifeData.relifeCountDown;
            info.bound = 0;
            ClientLogic::instance()->pass2Engine(&info);

        }
        
    }
    
}

void GameCore::removeFish(E2L_REMOVE_OUT_FISH info)
{
    fishManager->removeFish(info.fishId);
}

void GameCore::tideCome()
{
    cocos2d::log("tide tide tide come> >> >>>> >>>>>>>>>>>");
    changeGameState(FISH_GAME_TIDE_TIP);
}

void GameCore::tideSwitchOver()
{
    changeGameState(FISH_GAME_TIDE);
}

void GameCore::heroFire(E2L_CANNON_FIRE info)
{
    //暴走状态下，会发3颗子弹
    int count = bulletManager->getRushFire()?3:1;
    //激光炮和炸弹不消耗子弹
    if (cannonManager->getHeroCannon().getState() == CANNON_NORMAL) {
        if (cannonManager->getHeroCannon().getCannonMul() < stageConfig.minMultiply) {
            //不满足关卡最低倍率
            L2E_CANNON_MULTI_NOT_ENOUGH info;
            info.eProtocol = l2e_cannon_multi_not_enough;
            info.requireMultiply = cannonManager->levelConfigs[stageConfig.minMultiply].multiply;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
        if (totalScore < cannonManager->getHeroCannon().getMultiple()*count ) {
            //金币不足以发炮
            if (currState == FISH_GAME_BATTLE_RECHAGE) {
                return;
            }
            changeGameState(FISH_GAME_BATTLE_RECHAGE);
            getVipFCS();
            if (vip == 0) {
                showVipGift();
            }else{
                showHallRecharge(1);
            }
            return;
        }
    }
    
    cannonManager->getHeroCannon().setAngle(info.angle);
    
    CREATE_BULLET_CMD cmd;
    cmd.angle = cannonManager->getHeroCannon().getAngle();
    cmd.angle = info.angle;
    cmd.bulletCount = cannonManager->getHeroCannon().getBulletCount();
    cmd.chariId = cannonManager->getHeroCannon().getChairId();
    cmd.multiply = cannonManager->getHeroCannon().getMultiple();
    auto offVec = cocos2d::Vec2::forAngle(CC_DEGREES_TO_RADIANS(90-info.angle));
    if (info.posX == -2000 || info.posY == -2000) {
        cmd.pos = cannonManager->getHeroCannon().getPosition() + offVec*45;
    }else{
        cmd.pos = cocos2d::Vec2(info.posX, info.posY);
    }
    cmd.speed = 1;
    cmd.isHero = true;
    cmd.cannonType = cannonManager->getHeroCannon().getCannonType();
    cmd.traceFishId = info.traceFishId;
    cmd.expMulti = cannonManager->getHeroCannon().getExpMulti();

    if(bulletManager->createBullet(cmd))
    {
        if (cannonManager->getHeroCannon().getState() == CANNON_NORMAL) {
            //消耗金币
            changeGold(-cannonManager->getHeroCannon().getMultiple()*count);
            
            //自动降倍率
            int mul = cannonManager->getHeroMul();
            int val = 0;
            while (totalScore < cannonManager->levelConfigs[mul].multiply) {
                if (mul <= stageConfig.minMultiply) {
                    break;
                }
                val++;
                mul--;
                
            }
            if (val > 0) {
                cannonManager->changeCannonMul(-val, stageConfig.minMultiply);
                saveMulti2CS();
            }
            
        }
    }
}

void GameCore::catchFish(E2L_FISH_DEAD info)
{
    int fishTypeId = fishManager->fishMap[info.fishId].getTypeID();
    int catchScore = fishManager->fishMap[info.fishId].getCatchScore();
    //物品掉落
    Fish_Drop_Item item = fishManager->fishMap[info.fishId].caculateDrop();
    if (item.dropItemCount > 0) {
        dropItemIncreacement++;
        dropItemMap[dropItemIncreacement] = item;
        
        L2E_DROP_ITEM dropInfo;
        dropInfo.eProtocol = l2e_drop_item;
        dropInfo.dropIncreaceId = dropItemIncreacement;
        dropInfo.dropType = item.dropItemType;
        dropInfo.dropId = item.dropItemId;
        dropInfo.dropCount = item.dropItemCount;
        dropInfo.posX = fishManager->fishMap[info.fishId].getPosition().x;
        dropInfo.posY = fishManager->fishMap[info.fishId].getPosition().y;
        ClientLogic::instance()->pass2Engine(&dropInfo);
    }
    
    fishManager->removeFish(info.fishId);
    
    //牛牛和玛丽两种特殊鱼，切换游戏状态，进小游戏
    if (currState != FISH_GAME_NIUNIU && currState != FISH_GAME_MARY) {
        if (fishTypeId == FISH_TYPE_EX_39) {
            changeGameState(FISH_GAME_NIUNIU);
        }else if(fishTypeId == FISH_TYPE_EX_41) {
            changeGameState(FISH_GAME_MARY);
        }
    }

    
    checkBossTarget(fishTypeId);
    checkTask(fishTypeId);
    if(checkKillBig(fishTypeId))
    {
        //打中大鱼，加星，加积分
        getKillBigFCS();
        lotteryManager->addKillBig(catchScore*0.1);
        saveKillBig2CS();
        
        //为捕鱼日志加鱼的类型
        if (bigFishCountMap.find(fishTypeId) == bigFishCountMap.end()) {
            Big_Fish_Count fishCount;
            fishCount.fishType = fishManager->getFishType(fishTypeId);
            fishCount.fishRes = fishManager->getFishRes(fishTypeId);
            fishCount.fishName = fishManager->getFishName(fishTypeId);
            fishCount.count = 1;
            bigFishCountMap[fishTypeId] = fishCount;
        } else {
            bigFishCountMap[fishTypeId].count++;
        }
    }
}

bool GameCore::checkKillBig(int fishTypeId)
{
    int fishType = fishManager->getFishType(fishTypeId);
    if (fishType == TINY_BOSS_FISH || fishType == BOSS_FISH || fishType == SPECIAL_BOSS_FISH)
    {
        return true;
    }
    
    return false;
}

bool GameCore::checkTask(int fishTypeId)
{
//    getTaskFCS();
    parseTaskJson();
    if (stageMissionState != TASK_OPEN) {
        return false;
    }
    
    //找到鱼的类型是否关系到某个子任务
    int index = 0;
    for (; index < MAX_TASK_COUNT; index++) {
        if (fishTypeId == missionConfig.fishId[index]) {
            break;
        }
    }
    
    if (index == MAX_TASK_COUNT) {
        return false;
    }

    if (killTaskCount[index] >= missionConfig.fishCount[index]) {
        return false;
    }
    
    killTaskCount[index]++;
    
    //检查是否完成某个子任务
    if (killTaskCount[index] >= missionConfig.fishCount[index]) {
        L2E_COMPLETE_SUB_TASK info;
        info.eProtocol = l2e_complete_sub_task;
        info.gold = missionConfig.fishGold[index];
        ClientLogic::instance()->pass2Engine(&info);
        
        changeGold(missionConfig.fishGold[index]);
    }
    
    //检查是否完成所有任务
    bool complete = true;
    for (int i = 0; i < MAX_TASK_COUNT; i++) {
        if (killTaskCount[i] < missionConfig.fishCount[i]) {
            complete = false;
            break;
        }
    }
    if (complete) {
        stageMissionState = TASK_BOUND;
    }else{
        stageMissionState = TASK_OPEN;
    }
    
    L2E_UPDATE_TASK info;
    info.eProtocol = l2e_update_task;
    memcpy(info.fishCount, killTaskCount, sizeof(int)*MAX_TASK_COUNT);
    memcpy(info.fishMaxCount, missionConfig.fishCount, sizeof(int)*MAX_TASK_COUNT);
    info.taskState = stageMissionState;
    ClientLogic::instance()->pass2Engine(&info);
    
    save2TaskCoreString();
    return true;
}
bool GameCore::checkBossTarget(int fishTypeId)
{
    getBossTaskFCS();
    if (stageTargetState != STAGE_TARGET_OPEN) {
        return false;
    }


    if (fishTypeId == stageConfig.bossId) {
        killBoss1Count++;
    } else if (fishTypeId == stageConfig.subBossId) {
        killBoss2Count++;
    }else{
        return false;
    }
    
    killBoss1Count = MIN(killBoss1Count, stageConfig.bossCount);
    killBoss2Count = MIN(killBoss2Count, stageConfig.subBossCount);
    
    bool complete = (killBoss2Count == stageConfig.subBossCount && killBoss1Count == stageConfig.bossCount);
    if (complete) {
        L2E_COMPLETE_BOSS_TASK info;
        info.eProtocol = l2e_complete_boss_task;
        info.boundGold = stageConfig.boundsMoney;
        info.boundDiamond = stageConfig.boundsDiamond;
        ClientLogic::instance()->pass2Engine(&info);
        maxUnlockStage++;
        maxUnlockStage = MIN(maxUnlockStage, maxStage);
        killBoss1Count = 0;
        killBoss2Count = 0;
            
    //        return true;
        if (maxUnlockStage >= maxStage) {
            stageTargetState = STAGE_TARGET_CLOSE;
        }else{
            stageTargetState = STAGE_TARGET_BOUND;
        }
        
    }else{
        stageTargetState = STAGE_TARGET_OPEN;
    }
    
    L2E_UPDATE_BOSS_TASK info;
    info.eProtocol = l2e_update_boss_task;
    info.bossCount = killBoss1Count;
    info.bossMaxCount = stageConfig.bossCount;
    info.subBossCount = killBoss2Count;
    info.subBossMaxCount = stageConfig.subBossCount;
    info.targetState = stageTargetState;
    ClientLogic::instance()->pass2Engine(&info);

    saveBossTask2CS();
    
    return complete;

}

void GameCore::nextStage()
{
    getBossTaskFCS();
    if (stageTargetState != STAGE_TARGET_BOUND) {
        return;
    }
    
    if (maxUnlockStage >= maxStage) {
        return;
    }
    if (currStage != maxUnlockStage-1) {
        return;
    }
    
    //下一关按钮仅在最后一个关卡出现
    currStage = maxUnlockStage;
    stageTargetState = STAGE_TARGET_INIT;
    killBoss1Count = 0;
    killBoss2Count = 0;
    saveBossTask2CS();
    
    //清除所有掉落，并加给玩家
    clearDrop();
    save2Json();
    //清除所有子弹，把金币返还给玩家
    bulletManager->clear();
    bulletManager->setRushFire(false);
    //结束技能的使用状态
    for (int i = 1; i <= 5; i++) {
        skillManager->endSkill(i);
    }
    
    
    changeGameState(FISH_GAME_LOAD);
    
}

void GameCore::quitStage()
{
    getBossTaskFCS();
    if (currStage != maxUnlockStage) {
        stageTargetState = STAGE_TARGET_INIT;
    }
    saveBossTask2CS();
    //清除所有掉落，并加给玩家
    clearDrop();
    save2Json();
    //清除所有子弹，把金币返还给玩家
    bulletManager->clear();
    bulletManager->setRushFire(false);
    //结束技能的使用状态
    for (int i = 1; i <= 5; i++) {
        skillManager->endSkill(i);
    }
    ClientLogic::instance()->ChangeState(GAME_STATE_HALL);
}

void GameCore::lightFish(E2L_FISH_LIGHT_DEAD info)
{
    fishManager->fishMap[info.fishId].setStatus(FISH_DIED);
}

void GameCore::chooseCannon(E2L_CHOOSE_CANNON info)
{
    getCannonTypeFCS();
    getCannonHoldFCS();
    if (!cannonManager->holdCannon[info.cannonType]) {
        return;
    }
    cannonManager->changeCannon(info.cannonType);
    saveCannonType2CS();
}

void GameCore::changeCannonMul(E2L_CHANGE_CANNON_MULTIPLY info)
{
    getMultiFCS();
    cannonManager->changeCannonMul(info.val, stageConfig.minMultiply);
    saveMulti2CS();
}

void GameCore::useSkill(E2L_USE_SKILL info)
{
    //潮汐来临前20秒或潮汐来临提示时，提示不可使用技能
    if ((nextTideDuration <= 20 && currState == FISH_GAME_NORMAL) || currState == FISH_GAME_TIDE_TIP) {
        L2E_TIDE_COME_DISABLE_SKILL info;
        info.eProtocol = l2e_tide_come_disable_skill;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    getVipFCS();
    if (info.skillId == 4) {
        //vip4之前不能用激光炮
        if (vip < 4) {
            changeGameState(FISH_GAME_NEED_VIP);
            L2E_RAY_NEED_VIP info;
            info.eProtocol = l2e_ray_need_vip;
            info.needVipLevel = 4;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
    } else if (info.skillId == 5) {
        //2000炮之前不能用炸弹
        if(cannonManager->levelConfigs[cannonManager->getUnlockLevel()].multiply < 2000){
            L2E_BOMB_NEED_UNLOCK info;
            info.eProtocol = l2e_bomb_need_unlock;
            ClientLogic::instance()->pass2Engine(&info);
            return;
        }
    }

    getSkillCountFCS();
    
    if (skillManager->skillMap[info.skillId].count == 0) {
        //技能数量不足，消耗钻石
        if (totalDiamond >= skillManager->skillMap[info.skillId].price) {
            changeDiamond(-skillManager->skillMap[info.skillId].price);
        }else{
            //钻石不足，提示后进钻石充值或vip礼包
            changeGameState(FISH_GAME_BATTLE_RECHAGE);
            L2E_SKILL_CHARGE chargeInfo;
            chargeInfo.eProtocol = l2e_skill_charge;
            chargeInfo.skillId = info.skillId;
            ClientLogic::instance()->pass2Engine(&chargeInfo);
            return;
        }
    }
    skillManager->useSkill(info.skillId);
    if (info.skillId == 1 && vip >= 8) {
        //vip8锁定不计次数
        skillManager->skillMap[info.skillId].count = -1;
    }else if (info.skillId == 3 && vip >= 9){
        //vip9暴走不计次数
        skillManager->skillMap[info.skillId].count = -1;
    }
    
    saveSkillCount2CS();
    //更新技能数量
    L2E_UPDATE_SKILL skillInfo;
    skillInfo.eProtocol = l2e_update_skill;
    for (int i = 0; i < 5; i++) {
        skillInfo.skillCount[i] = skillManager->skillMap[i+1].count;
        skillInfo.skillPrice[i] = skillManager->skillMap[i+1].price;
    }
    ClientLogic::instance()->pass2Engine(&skillInfo);
    
    //更新技能CD
    L2E_USE_SKILL useInfo;
    useInfo.eProtocol = l2e_use_skill;
    useInfo.useSkillId = info.skillId;
    useInfo.cd = skillManager->skillMap[info.skillId].coolDuration;
    getVipFCS();
    if (vip >= 2 && info.skillId == 1) {
        //vip2锁定CD时间翻倍
        useInfo.cd *= 2;
    }
    ClientLogic::instance()->pass2Engine(&useInfo);
}

void GameCore::changeGameState(FISH_GAME_STATES nextState)
{
    if(currState == nextState)
        return;
    log("change state from %d to %d", currState, nextState);
    exitGameState(currState);
    lastState	= currState;
    currState		= nextState;
    enterGameState(nextState);

}

void GameCore::enterGameState(FISH_GAME_STATES nextState)
{
    switch (nextState) {
        case FISH_GAME_UPGRADE_LEVEL:
        {
            originState = lastState;
        }
            break;
        case FISH_GAME_INIT:
        {
            srand((unsigned int)time(NULL));
            if (stageTargetState == STAGE_TARGET_OPEN) {
                stageTargetState = STAGE_TARGET_INIT;
            }
            if (stageMissionState == TASK_OPEN) {
                stageMissionState = TASK_INIT;
            }

            getMultiFCS();
            cannonManager->loadConfig(unlockCannonLvl);
            cannonManager->loadCannonConfig(stageConfig.minMultiply);
            
            getProbFCS();
            fishManager->setSceneProb(sceneProb);
            
            L2E_ENTERGAME info;
            info.eProtocol = l2e_enter_game;
            info.scnenId = 0;
            ClientLogic::instance()->pass2Engine(&info);
            
        }
            break;
        case FISH_GAME_LOAD:
        {
            for (auto bullet : bulletManager->bulletMap) {
                if(bullet.second.getBulletType() == 1){
                    changeGold(bullet.second.getMultiply());
                }
            }
            bulletManager->clear();
            
            addLoadFunc(Load::LoadFunc(&GameCore::loadTask));
            addLoadFunc(Load::LoadFunc(&GameCore::loadFish));
            addLoadFunc(Load::LoadFunc(&GameCore::loadPath));
            addLoadFunc(Load::LoadFunc(&GameCore::loadCannon));
            
            L2E_ENTER_LOAD_SCENE info;
            info.eProtocol = l2e_enter_load_scene;
            ClientLogic::instance()->pass2Engine(&info);
        }
            break;
        case FISH_GAME_HALL:
        {
            cannonManager = CanonnManager::instance();
            bulletManager = BulletManager::instance();
            fishManager = FishManager::instance();
            skillManager = SkillManager::instance();
            lotteryManager = BlackLottery::instance();
            
            skillManager->loadConfig();
            lotteryManager->loadConfig();
            for (auto bullet : bulletManager->bulletMap) {
                if(bullet.second.getBulletType() == 1){
                    changeGold(bullet.second.getMultiply());
                }
            }

            bulletManager->clear();
            
            
            auto doc = GameReader::getDocInstance(STAGE_CONFIG_FILE);
            if(GameCore::idStageTable.empty())
            {
                GameCore::initIdStageTable(*doc);
                maxStage = (int)idStageTable.size();
            }
            
            getLevelFCS();
            readLevelFile(level, LEVEL_CONFIG_FILE);
            
            getMultiFCS();
            cannonManager->loadConfig(unlockCannonLvl);
            
            
            excludeFishIds.clear();
            fishManager->loadFishConfig(excludeFishIds);
            
            loadDiaryJson();
            loadStageMiniConfigs();
            
            getTotalScoreFromCoreString();
            getTotalDiamondFromCoreString();
            getLevelFCS();
            readLevelFile(level, LEVEL_CONFIG_FILE);
            getExpScoreFCS();
            getBossTaskFCS();
        }
            break;
        case FISH_GAME_NORMAL:
        {
            srand((unsigned int)time(NULL));
            if(cannonManager->cannonMap.empty())
            {
                auto winSize = cocos2d::Director::getInstance()->getWinSize();
                getMultiFCS();
                getCannonTypeFCS();
                CREATE_CANNON_CMD tmd;
                tmd.chairId = 0;
                tmd.isHero = true;
                    
                tmd.cannonMul = currCannonMulti;
                
                tmd.bulletCount = 2;
                tmd.typeId = currCannonType;
                tmd.pos = cocos2d::Vec2(winSize.width/2, 45);
                cannonManager->createCannon(tmd);
            }
            
            clearAll();
            bulletManager->setReadyFire(true);
            
            fishManager->setInTide(false);
            
            nextTideDuration = abs(rand())%30+360;
            
            if (cannonManager->CannonMaxLevel()) {
                L2E_CANNON_MAX_LEVEL info;
                info.eProtocol = l2e_cannon_max_level;
                ClientLogic::instance()->pass2Engine(&info);
            }else{
                int nextLvl = cannonManager->getUnlockLevel()+1;
                L2E_CANNON_UPDATE_LEVEL info;
                info.eProtocol = l2e_cannon_update_level;
                info.totalDiamond = totalDiamond;
                info.needDiamond = cannonManager->levelConfigs[nextLvl].needDiamondCount;
                info.nextMultiply = cannonManager->levelConfigs[nextLvl].multiply;
                info.boundGolds = cannonManager->levelConfigs[nextLvl].boundGolds;
                info.canUnlock = cannonManager->checkCanUnlockLevel(totalDiamond);
                ClientLogic::instance()->pass2Engine(&info);
                
            }
            
            getTotalScoreFromCoreString();
            L2E_UPDATE_GOLD infoGold;
            infoGold.eProtocol = l2e_update_gold;
            infoGold.gold = totalScore;
            ClientLogic::instance()->pass2Engine(&infoGold);
            
            getTotalDiamondFromCoreString();
            L2E_UPDATE_DIAMOND infoDiamond;
            infoDiamond.eProtocol = l2e_update_diamond;
            infoDiamond.diamond = totalDiamond;
            ClientLogic::instance()->pass2Engine(&infoDiamond);
            
            
            L2E_UPDATE_ENERGY infoEnergy;
            infoEnergy.eProtocol = l2e_update_energy;
            infoEnergy.currVal = energyScore;
            infoEnergy.maxVal = ENERGY_THRES;
            ClientLogic::instance()->pass2Engine(&infoEnergy);
            
            getExpScoreFCS();
            getVipFCS();
            getLevelFCS();
            readLevelFile(level, LEVEL_CONFIG_FILE);
            L2E_UPDATE_HEADINFO infoHead;
            infoHead.eProtocol = l2e_update_headInfo;
            infoHead.currExp = expScore;
            infoHead.maxExp = maxExp;
            infoHead.level = level;
            infoHead.vip = vip;
            infoHead.title = title;
            ClientLogic::instance()->pass2Engine(&infoHead);
            
            getSkillCountFCS();
            L2E_UPDATE_SKILL skillInfo;
            skillInfo.eProtocol = l2e_update_skill;
            for (int i = 0; i < 5; i++) {
                skillInfo.skillCount[i] = skillManager->skillMap[i+1].count;
                skillInfo.skillPrice[i] = skillManager->skillMap[i+1].price;
            }
            ClientLogic::instance()->pass2Engine(&skillInfo);
            
            if (stageTargetState == STAGE_TARGET_INIT) {
                L2E_INIT_BOSS_TASK info;
                info.eProtocol = l2e_init_boss_task;
                info.boundGold = stageConfig.boundsMoney;
                info.boundDiamond = stageConfig.boundsDiamond;
                
                info.bossRes = fishManager->getFishRes(stageConfig.bossId);
                info.bossType = fishManager->getFishType(stageConfig.bossId);
                info.bossCount = killBoss1Count;
                info.bossMaxCount = stageConfig.bossCount;
                
                info.subBossRes = fishManager->getFishRes(stageConfig.subBossId);
                info.subBossType = fishManager->getFishType(stageConfig.subBossId);
                info.subBossCount = killBoss2Count;
                info.subBossMaxCount = stageConfig.subBossCount;
                
                ClientLogic::instance()->pass2Engine(&info);
                stageTargetState = STAGE_TARGET_OPEN;
                saveBossTask2CS();
            }else{
                L2E_UPDATE_BOSS_TASK info;
                info.eProtocol = l2e_update_boss_task;
                info.bossCount = killBoss1Count;
                info.bossMaxCount = stageConfig.bossCount;
                info.subBossCount = killBoss2Count;
                info.subBossMaxCount = stageConfig.subBossCount;
                info.targetState = stageTargetState;
                ClientLogic::instance()->pass2Engine(&info);
            }
            
            if (stageMissionState == TASK_INIT) {
                L2E_INIT_TASK infoTask;
                infoTask.eProtocol = l2e_init_task;
                for (int i = 0; i < MAX_TASK_COUNT; i++) {
                    infoTask.fishRes[i] = fishManager->getFishRes(missionConfig.fishId[i]);
                    infoTask.fishType[i] = fishManager->getFishType(missionConfig.fishId[i]);
                    infoTask.fishMaxCount[i] = missionConfig.fishCount[i];
                    infoTask.fishCount[i] = killTaskCount[i];
                }
                ClientLogic::instance()->pass2Engine(&infoTask);
                stageMissionState = TASK_OPEN;
                save2TaskCoreString();
            }else{
                L2E_UPDATE_TASK info;
                info.eProtocol = l2e_update_task;
                info.taskState = stageMissionState;
                memcpy(info.fishCount, killTaskCount, sizeof(int)*MAX_TASK_COUNT);
                memcpy(info.fishMaxCount, missionConfig.fishCount, sizeof(int)*MAX_TASK_COUNT);
                ClientLogic::instance()->pass2Engine(&info);
            }
            
            getKillBigFCS();
            L2E_UPDATE_BIG_STAR info;
            info.eProtocol = l2e_update_big_star;            
            info.starCount = lotteryManager->getkillStarCount();
            info.boxEnable = (lotteryManager->getkillStarCount()==5);
            ClientLogic::instance()->pass2Engine(&info);
            
            getRelifeFCS();
            if (relifeData.relifeState == 2) {
                Relife_Config config;
                readRelifeConfig(relifeData.relifeId, config, RELIFE_CONFIG_FILE);
                L2E_UPDATE_RELIFE info;
                info.eProtocol = l2e_update_relife;
                info.state = 2;
                info.countDown = 0;
                info.bound = config.bounds;
                getVipFCS();
                if (vip >= 3) {
                    info.bound *= 2;
                }
                ClientLogic::instance()->pass2Engine(&info);
            }
            
       }
            break;
            
        case FISH_GAME_TIDE_TIP:
        {
            
            fishManager->speedUpAll();
            fishManager->setSelFishInterval(0);
            
            for (auto bullet : bulletManager->bulletMap) {
                if(bullet.second.getBulletType() == 1){
                    changeGold(bullet.second.getMultiply());
                }
            }
            bulletManager->clear();
            bulletManager->setReadyFire(false);
            
            
            L2E_TIDE_COME_TIP info;
            info.eProtocol = l2e_tide_come;
            ClientLogic::instance()->pass2Engine(&info);
        }
            break;

        case FISH_GAME_SWITCH_TIDE:
        {
            bulletManager->setReadyFire(true);
            
            L2E_TIDE_SWITCH info;
            info.eProtocol = l2e_tide_switch;
            info.sceneId = abs(rand())%3*2;
            ClientLogic::instance()->pass2Engine(&info);
        }
            break;

        case FISH_GAME_TIDE:
        {
            bulletManager->setReadyFire(true);
            
            clearAll();
            fishManager->setInTide(true);
            fishManager->createSceneFish();
        }
            break;

        case FISH_GAME_NIUNIU:
        {
            L2E_NIUNIU_SPLASH info;
            info.eProtocol = l2e_niuniu_splash;
            ClientLogic::instance()->pass2Engine(&info);
        }
            break;
            
        case FISH_GAME_MARY:
        {
            L2E_MARY_SPLASH info;
            info.eProtocol = l2e_mary_splash;
            ClientLogic::instance()->pass2Engine(&info);
        }
        default:
            break;
    }
}

void GameCore::exitGameState(FISH_GAME_STATES state)
{
    
}

void GameCore::frozenScreen(cocos2d::EventCustom *event)
{
    cocos2d::Value val = *static_cast<cocos2d::Value*>(event->getUserData());
    fishManager->setPause(true);
    fishManager->setPauseDuration(val.asInt());
    cocos2d::EventCustom event1(ICE_SCREEN);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
    
}

void GameCore::bombFishes(cocos2d::EventCustom *event)
{
    
    std::map<int, int> info = *static_cast<std::map<int, int>*>(event->getUserData());
    
    int scoreSum = 0;
    fishManager->clearCatchGroups();
    for (auto obj: info) {
        int bulletId = obj.second;
        int fishId = obj.first;
        //炸弹不能炸组鱼
        if (fishManager->fishMap[fishId].getGroupType() != NONE_GROUP) {
            continue;
        }
        //炸弹只能炸小鱼
        if(fishManager->getFishType(fishManager->fishMap[fishId].getTypeID()) != SMALL_FISH)
        {
            continue;
        }
        
        int chairId = bulletManager->bulletMap[bulletId].getChairId();
        bool bomb = (bulletManager->bulletMap[bulletId].getBulletType() == 3);
        //炸弹的倍率固定为1000
        scoreSum += fishManager->catchFish(fishId, 1000, 0, chairId, bomb);
        
    }
    
    if(scoreSum > 0)
    {
        changeGold(scoreSum);
        if (energyDuration == 0) {
            //能量炮积分以鱼的价格为准
            if (energyScore + fishManager->getPriceSum() >= ENERGY_THRES) {
                startEnergyCannon();
            }else{
                changeEnergy(energyScore + fishManager->getPriceSum());
            }
        }
    }
    
    if (fishManager->getPriceSum()>0) {
        //经验以鱼的价格为准
        addExp(fishManager->getPriceSum());
    }
}

void GameCore::hitFishes(cocos2d::EventCustom *event)
{
    std::map<int, int> info = *static_cast<std::map<int, int>*>(event->getUserData());
    //普通子弹击中鱼的数量有个降几率的因子
    int fishCount = (int)info.size();
    float rateFactor = 1;
    if (fishCount <= 0 || fishCount > MAX_FISH_PER_BULLET)
    {
        rateFactor = 1;
    }else{
        rateFactor = gCommonBulletRate[fishCount-1];
    }
    
    int scoreSum = 0;
    fishManager->clearCatchGroups();
//    fishManager->setPriceSum(0);
    for (auto obj: info) {
        int bulletId = obj.second;
        int fishId = obj.first;
        
        int multi = bulletManager->bulletMap[bulletId].getMultiply();
        int chairId = bulletManager->bulletMap[bulletId].getChairId();
        bool bomb = (bulletManager->bulletMap[bulletId].getBulletType() == 3);
        int expMulti = bulletManager->bulletMap[bulletId].getExpMulti();
        scoreSum += fishManager->catchFish(fishId, multi, expMulti, chairId, bomb, rateFactor);

    }
    scoreSum += fishManager->catchLightningFish();
    scoreSum += fishManager->catchFishTypeBombFish();
    scoreSum += fishManager->catchAllNetFish();

    fishManager->catchGroupsFish();

    scoreSum += fishManager->doBombClear();
    
    if(scoreSum > 0)
    {
        changeGold(scoreSum);
        if (energyDuration == 0) {
            if (energyScore + fishManager->getPriceSum() >= ENERGY_THRES) {
                startEnergyCannon();
            }else{
                changeEnergy(energyScore + fishManager->getPriceSum());
            }
        }        
    }
    
    if (fishManager->getPriceSum()>0) {
        addExp(fishManager->getPriceSum());
    }

}

void GameCore::rayHitFishes(cocos2d::EventCustom *event)
{
    std::map<int, int> info = *static_cast<std::map<int, int>*>(event->getUserData());
    
    int scoreSum = 0;
    fishManager->clearCatchGroups();
    for (auto obj: info) {
        int bulletId = obj.second;
        int fishId = obj.first;
        
        int chairId = bulletManager->bulletMap[bulletId].getChairId();
        bool bomb = (bulletManager->bulletMap[bulletId].getBulletType() == 3);
        //激光炮的倍率固定为1000
        scoreSum += fishManager->catchFish(fishId, 1000, 0, chairId, bomb);
        
    }
    scoreSum += fishManager->catchLightningFish();
    scoreSum += fishManager->catchFishTypeBombFish();
    scoreSum += fishManager->catchAllNetFish();
    
    fishManager->catchGroupsFish();
    
    scoreSum += fishManager->doBombClear();
    
    if(scoreSum > 0)
    {
        changeGold(scoreSum);
        if (energyDuration == 0) {
            if (energyScore + fishManager->getPriceSum() >= ENERGY_THRES) {
                startEnergyCannon();
            }else{
                changeEnergy(energyScore + fishManager->getPriceSum());
            }
        }
    }
    
    if (fishManager->getPriceSum()>0) {
        addExp(fishManager->getPriceSum());
    }
    
}


void GameCore::initLevelTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()) {
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            levelIdTable.insert(std::make_pair(objId, i));
        }
    }
}

void GameCore::initRelifeTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()) {
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            relifeIdTable.insert(std::make_pair(objId, i));
        }
    }
}

void GameCore::initIdStageTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()) {
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            idStageTable.insert(std::make_pair(objId, i));
        }
    }
}

void GameCore::initIdTaskTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()) {
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            taskIdTable.insert(std::make_pair(objId, i));
        }
    }
}

void GameCore::initRechargeTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()) {
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            rechargeIdTable.insert(std::make_pair(objId, i));
        }
    }
}

void GameCore::initVipTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()) {
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            vipIdTable.insert(std::make_pair(objId, i));
        }
    }
}

bool GameCore::initRelifeData(int idx, Relife_Config& config, rapidjson::Document &_doc)
{
    int index = relifeIdTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    

    config.relifeId = DICTOOL->getIntValue_json(dic, "ID");
    config.countDownLimits = DICTOOL->getIntValue_json(dic, "second");
    config.bounds = DICTOOL->getIntValue_json(dic, "Bounds");

    return true;
}

bool GameCore::initLevelData(int idx, rapidjson::Document &_doc)
{
    int index = levelIdTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    maxExp = DICTOOL->getIntValue_json(dic, "exp");
    levelBoundGold = DICTOOL->getIntValue_json(dic, "gold");
    levelBoundDiamond = DICTOOL->getIntValue_json(dic, "diamond");
    return true;
}

bool GameCore::initStageData(int idx, rapidjson::Document &_doc)
{
    int index = idStageTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    stageConfig.stageId = DICTOOL->getIntValue_json(dic, "ID");
    stageConfig.bossId = DICTOOL->getIntValue_json(dic, "BossID");
    fishManager->setBossFishId(stageConfig.bossId);
    stageConfig.bossCount = DICTOOL->getIntValue_json(dic, "BossCount");
    stageConfig.subBossId = DICTOOL->getIntValue_json(dic, "fishId");
    stageConfig.subBossCount = DICTOOL->getIntValue_json(dic, "fishCount");
    stageConfig.missionId = DICTOOL->getIntValue_json(dic, "missionId");
    stageConfig.boundsMoney = DICTOOL->getIntValue_json(dic, "BoundsMoney");
    stageConfig.boundsDiamond = DICTOOL->getIntValue_json(dic, "BoundsDiamond");
    stageConfig.dailyMoney = DICTOOL->getIntValue_json(dic, "DailyMoney");
    stageConfig.dailyDiamond = DICTOOL->getIntValue_json(dic, "DailyDiamond");
    stageConfig.minMultiply = DICTOOL->getIntValue_json(dic, "MinCannon");
    std::string excludeStr = DICTOOL->getStringValue_json(dic, "ExcludeFishID");
    std::vector<std::string> excludeVec = GameUtils::splitStr(excludeStr, ":");
    excludeFishIds.clear();
    for (auto idStr : excludeVec) {
        int fishId = atoi(idStr.c_str());
        excludeFishIds.push_back(fishId);
    }
    
    std::string dialySkillStr = DICTOOL->getStringValue_json(dic, "DailySkill");

    dailySkills.clear();
    std::vector<std::string> skillVec = GameUtils::splitStr(dialySkillStr, ":");
    for (auto idStr : skillVec) {
        int skillId = atoi(idStr.c_str());
        dailySkills.push_back(skillId);
    }

    return true;
}

bool GameCore::initRechargeData(int idx, Recharge_Config &config, rapidjson::Document &_doc)
{
    int index = idStageTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    config.rechargeId = DICTOOL->getIntValue_json(dic, "ID");
    config.type = DICTOOL->getIntValue_json(dic, "type");
    config.price = DICTOOL->getIntValue_json(dic, "price");
    config.amount = DICTOOL->getIntValue_json(dic, "amount");
    config.firstFactor = DICTOOL->getIntValue_json(dic, "first");
    config.factor = DICTOOL->getIntValue_json(dic, "factor");
    return true;
}

bool GameCore::initVipData(int idx, Vip_Config &config, rapidjson::Document &_doc)
{
    int index = idStageTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    config.vipId = DICTOOL->getIntValue_json(dic, "ID");
    config.needMoney = DICTOOL->getIntValue_json(dic, "need");
    config.dailyFeedback = DICTOOL->getIntValue_json(dic, "daily");
    return true;
}

void GameCore::loadStageMiniConfigs()
{
    auto doc = GameReader::getDocInstance(STAGE_CONFIG_FILE);
    if (doc->IsArray()) {
        for (int i = 0; i < doc->Size(); i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
            if (dic.IsNull()) {
                return;
            }
            
            Stage_Mini_Config stageConfig;
            int bossId = DICTOOL->getIntValue_json(dic, "BossID");
            stageConfig.bossRes = fishManager->getFishRes(bossId).c_str();
            stageConfig.bossType = fishManager->getFishType(bossId);
            stageConfig.boundsMoney = DICTOOL->getIntValue_json(dic, "BoundsMoney");
            stageConfig.boundsDiamond = DICTOOL->getIntValue_json(dic, "BoundsDiamond");
            int stageId = DICTOOL->getIntValue_json(dic, "ID");
            stageMiniConfigs[stageId] = stageConfig;
        }
    }

}

bool GameCore::initTaskData(int idx, rapidjson::Document &_doc)
{
    int index = idStageTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    missionConfig.missionId = DICTOOL->getIntValue_json(dic, "ID");
    for (int i = 0; i < MAX_TASK_COUNT; i++) {
        std::string temp("Fish");
        temp += Convert2String(i+1);
        missionConfig.fishId[i] = DICTOOL->getIntValue_json(dic, (temp + "ID").c_str());
        missionConfig.fishCount[i] = DICTOOL->getIntValue_json(dic, (temp + "Count").c_str());
        missionConfig.fishGold[i] = DICTOOL->getIntValue_json(dic, (temp + "Gold").c_str());
    }
    missionConfig.boundsMoney = DICTOOL->getIntValue_json(dic, "BoundsMoney");
    missionConfig.boundsDiamond = DICTOOL->getIntValue_json(dic, "BoundsDiamond");
    
    return true;
}

void GameCore::readStageFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(GameCore::idStageTable.empty())
    {
        GameCore::initIdStageTable(*doc);
        maxStage = (int)idStageTable.size();
    }
    initStageData(idx, *doc);
}

void GameCore::readLevelFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(GameCore::levelIdTable.empty())
    {
        GameCore::initLevelTable(*doc);
    }
    initLevelData(idx, *doc);
}

bool GameCore::readRelifeConfig(int idx, Relife_Config& config, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(GameCore::relifeIdTable.empty())
    {
        GameCore::initRelifeTable(*doc);
    }
    return initRelifeData(idx, config, *doc);
}

void GameCore::readTaskFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(GameCore::taskIdTable.empty())
    {
        GameCore::initIdTaskTable(*doc);
    }
    initTaskData(idx, *doc);
}

bool GameCore::readRechargeConfig(int idx, Recharge_Config &config, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(GameCore::rechargeIdTable.empty())
    {
        GameCore::initRechargeTable(*doc);
    }
    return initRechargeData(idx, config, *doc);
}

bool GameCore::readVipConfig(int idx, Vip_Config &config, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(GameCore::vipIdTable.empty())
    {
        GameCore::initVipTable(*doc);
    }
    return initVipData(idx, config, *doc);
}

void GameCore::startRebate()
{
    rebateDuration = REBATE_TIME_LIMIT;
    fishManager->setSceneProb(gRebateProb);
}

void GameCore::changeSceneProb()
{
    getProbFCS();
    probForceDuration += probTotalDuration;
    //判断是不是要强制大降几率，还是一般的改几率
    bool forceDown = false;
    if (probForceCount == 0) {
        probForceCount = 1;
        if (probForceDuration >= FIRST_FORCE_TIME) {
            forceDown = true;
        }
    }else if(probForceDuration >= NORMAL_FORCE_TIME){
        forceDown = true;
    }
    
    if(!forceDown)
    {
//        sceneProbDuration = probTotalDuration = (abs(rand())%30+1)*60;
        //1-300秒重新生成一个几率因子
        sceneProbDuration = probTotalDuration = (abs(rand())%300+1);
        float currProb = sceneProb;
        int randProb = 0;
        do{
            randProb = abs(rand())%MAX_SCENE_PROB_STYLE;
            int a = abs(rand())%100;
            for (int i = 0; i<MAX_SCENE_PROB_STYLE; i++) {
                if (a <= gSceneProbRate[i]) {
                    randProb = i;
                    break;
                }
                a -= gSceneProbRate[i];
            }
    
        }while (currProb == gSceneProb[randProb]);
        
        sceneProb = gSceneProb[randProb];
        
    }else{
        
        getTotalScoreFromCoreString();
        //根据有没有充值，决定几率
        if (!probForceRecharge) {
            sceneProb = gSceneLowProb;
            sceneProbDuration = probTotalDuration = PROB_DOWN_TIME;
        }else{
            //根据有没有赢，决定升或降
            if (probForceGold+probForceRecharge >= totalScore) {
                sceneProb = gSceneUpProb;
                sceneProbDuration = probTotalDuration = PROB_UP_TIME;
            }else{
                sceneProb = gSceneLowProb;
                sceneProbDuration = probTotalDuration = PROB_DOWN_TIME;
            }
        }
        probForceDuration = 0;
        probForceRecharge = 0;
        probForceGold = totalScore;
        probForceCount++;
    }
    fishManager->setSceneProb(sceneProb);
    saveProb2CS();
}

void GameCore::startEnergyCannon()
{
    energyDuration = ENERGY_TIME_LIMIT;
//    energyScore = 0;
    changeEnergy(0);
    fishManager->setCannonProb(gEnergyCanProb);
    cannonManager->change2Energy();
    
    L2E_START_ENERGY info;
    info.eProtocol = l2e_start_energy_cannon;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::endEnergyCannon()
{
    getCannonTypeFCS();
    energyDuration = 0;
    energyScore = 0;
    fishManager->setCannonProb(1.0);
//    cannonManager->changeCannon(cannonManager->getHeroCannonType());
    cannonManager->endEnergy();
    
//    L2E_END_ENERGY info;
//    info.eProtocol = l2e_end_energy_cannon;
    
}

void GameCore::readyPlane(cocos2d::EventCustom *event)
{
    cannonManager->readyPlane();
    bulletManager->readyPlane();
    
    L2E_READY_PLANE info;
    info.eProtocol = l2e_ready_plane;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::endPlane(cocos2d::EventCustom *event)
{
    cannonManager->endPlane();
    bulletManager->endPlane();
    
    L2E_END_PLANE info;
    info.eProtocol = l2e_end_plane;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::readyRay(cocos2d::EventCustom *event)
{
    cannonManager->readyRay();
    bulletManager->readyRay();
    
    L2E_READY_RAY info;
    info.eProtocol = l2e_ready_ray;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::endRayFire(cocos2d::EventCustom *event)
{
    cannonManager->endRay();
    bulletManager->endRay();
    
    L2E_END_RAY info;
    info.eProtocol = l2e_end_ray;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::initMary()
{
    maryManager->randRoll();
    
    L2E_MARY_INIT info;
    info.eProtocol = l2e_mary_init;
    info.roundIndex = maryManager->getRoundBtnIndex();
    info.roundTypeId = maryManager->getRoundTypeId();
    info.cardType = maryManager->getCardType();
    info.stageScore = maryManager->getStageScore();
    info.stageDiamond = maryManager->getStageDiamond();
    info.totalScore = totalScore;
    memcpy(info.centerIndex, maryManager->centerTypeId, sizeof(int)*4);
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::exitMary()
{
    backGameState();
    changeGold(maryManager->getStageScore());
    changeDiamond(maryManager->getStageDiamond());
    L2E_HIDE_MARY info;
    info.eProtocol = l2e_hide_mary;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::updateLuckyDraw(E2L_UPDATE_LUCKY_DRAW pMsgInfo)
{
    lotteryManager->updateLuckyDraw(pMsgInfo.level);
}

void GameCore::enterLuckyDraw()
{
    changeGameState(FISH_GAME_LOTTERY);
    lotteryManager->enterLuckyDraw();
}

void GameCore::exitLuckyDraw()
{
    backGameState();
    L2E_EXIT_LUCKY_DRAW info;
    info.eProtocol = l2e_exit_lucky_draw;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::enterLottery()
{
    lotteryManager->enterLottery();
    saveKillBig2CS();
}

void GameCore::endLottery()
{
    backGameState();
    int lotteryBoundType = 0;
    int lotteryBoundId = 0;
    int lotteryBoundCount = 0;
    int lotteryResultLevel = lotteryManager->getResultLevel();
    int lotteryResultIdx = lotteryManager->getResultIdx();
    
    lotteryBoundType = lotteryManager->lotteryMap[lotteryResultLevel].boundsType[lotteryResultIdx];
    lotteryBoundId = lotteryManager->lotteryMap[lotteryResultLevel].boundsId[lotteryResultIdx];
    lotteryBoundCount = lotteryManager->lotteryMap[lotteryResultLevel].boundsCount[lotteryResultIdx];
    
    switch (lotteryBoundType) {
        case 1:
            changeGold(lotteryBoundCount);
            break;
        case 2:
            changeDiamond(lotteryBoundCount);
            break;
        case 3:
            getSkillCountFCS();
            if (skillManager->skillMap[lotteryBoundId].count >= 0) {
                skillManager->skillMap[lotteryBoundId].count += lotteryBoundCount;
            }
            
            saveSkillCount2CS();
            L2E_UPDATE_SKILL skillInfo;
            skillInfo.eProtocol = l2e_update_skill;
            for (int i = 0; i < 5; i++) {
                skillInfo.skillCount[i] = skillManager->skillMap[i+1].count;
                skillInfo.skillPrice[i] = skillManager->skillMap[i+1].price;
            }
            ClientLogic::instance()->pass2Engine(&skillInfo);
            
            break;
        default:
            break;
    }
    
    lotteryManager->endLottery();
    
    L2E_END_LOTTERY info;
    info.eProtocol = l2e_end_lottery;
    ClientLogic::instance()->pass2Engine(&info);
    
}

void GameCore::addExp(int deltaVal)
{
    getExpScoreFCS();
    expScore += deltaVal;
    if (expScore >= maxExp) {
        levelUp();
    }
    
    L2E_UPDATE_HEADINFO info;
    info.eProtocol = l2e_update_headInfo;
    info.currExp = expScore;
    info.maxExp = maxExp;
    info.level = level;
    info.vip = vip;
    info.title = title;
    ClientLogic::instance()->pass2Engine(&info);
    saveExpScore2CS();
}

void GameCore::levelUp()
{
    getLevelFCS();
    if (level >= MAX_LEVEL) {
        expScore = maxExp;
        return;
    }
    
    expScore = 0;
    level++;
    title = gTitles[level/10];
    saveLevel2CS();
    
    changeGold(levelBoundGold);
    changeDiamond(levelBoundDiamond);
    readLevelFile(level, LEVEL_CONFIG_FILE);
    
    L2E_LEVELUP_TIP info;
    info.eProtocol = l2e_levelup_tip;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::changeEnergy(int val)
{
    energyScore = val;
    energyScore = MAX(0, energyScore);
    
    L2E_UPDATE_ENERGY info;
    info.eProtocol = l2e_update_energy;
    info.currVal = energyScore;
    info.maxVal = ENERGY_THRES;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::changeGold(int deltaVal)
{
    getTotalScoreFromCoreString();
//    log(">>>>>>>>>>>gold<<<<<<<<<<<<<");
//    log("load totalScore:%llu", totalScore);
//    log("deltaGold:%d", deltaVal);
    if (totalScore <= 0 && deltaVal > 0) {
        getRelifeFCS();
        relifeData.relifeState = 0;
        relifeData.relifeCountDown = 0;
        saveRelife2CS();
        L2E_UPDATE_RELIFE info;
        info.eProtocol = l2e_update_relife;
        info.state = 0;
        info.countDown = 0;
        info.bound = 0;
        ClientLogic::instance()->pass2Engine(&info);
    }
    totalScore += deltaVal;
    totalScore = MAX(0, totalScore);
//    log("sum:%llu",totalScore);
    if (deltaVal != 0) {
        L2E_UPDATE_GOLD info;
        info.eProtocol = l2e_update_gold;
        info.gold = totalScore;
        ClientLogic::instance()->pass2Engine(&info);
        saveTotalScore2CS();
    }
    
    L2E_UPDATE_HALL_MONEY info;
    info.eProtocol = l2e_update_hall_money;
    info.gold = totalScore;
    info.diamond = totalDiamond;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::changeDiamond(int deltaVal)
{
    getTotalDiamondFromCoreString();
    totalDiamond += deltaVal;
    totalDiamond = MAX(0, totalDiamond);
    
    if (deltaVal != 0) {
        //钻石变化会更新炮台升级界面
        if (!cannonManager->CannonMaxLevel()) {
            int nextLvl = cannonManager->getUnlockLevel()+1;
            L2E_CANNON_UPDATE_LEVEL info;
            info.eProtocol = l2e_cannon_update_level;
            info.totalDiamond = totalDiamond;
            info.needDiamond = cannonManager->levelConfigs[nextLvl].needDiamondCount;
            info.nextMultiply = cannonManager->levelConfigs[nextLvl].multiply;
            info.boundGolds = cannonManager->levelConfigs[nextLvl].boundGolds;
            info.canUnlock = cannonManager->checkCanUnlockLevel(totalDiamond);
            ClientLogic::instance()->pass2Engine(&info);
        }
        L2E_UPDATE_DIAMOND info;
        info.eProtocol = l2e_update_diamond;
        info.diamond = totalDiamond;
        ClientLogic::instance()->pass2Engine(&info);
        saveTotalDiamond2CS();
    }
    L2E_UPDATE_HALL_MONEY info;
    info.eProtocol = l2e_update_hall_money;
    info.gold = totalScore;
    info.diamond = totalDiamond;
    ClientLogic::instance()->pass2Engine(&info);
}
void GameCore::hideUnlockLevel()
{
    lastState = originState;
    backGameState();
    
    L2E_CANNON_HIDE_UNLOCK_DIALOG info;
    info.eProtocol = l2e_cannon_hide_unlock_dialog;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::unlockCannonLevel()
{
    getMultiFCS();
    int nextLvl = cannonManager->getUnlockLevel()+1;
    //钻石不足
    if (totalDiamond < cannonManager->levelConfigs[nextLvl].needDiamondCount) {
        changeGameState(FISH_GAME_UPGRADE_LEVEL);
        L2E_CANNON_SHOW_UNLOCK_DIALOG info;
        info.eProtocol = l2e_cannon_show_unlock_dialog;
        info.cardCount = CanonnManager::maxLevel - cannonManager->getUnlockLevel();
        info.cardCount = MIN(5, info.cardCount);
        if (info.cardCount <= 0) {
            return;
        }
        
        int nextLvl = cannonManager->getUnlockLevel()+1;
        for (int i = 0; i < info.cardCount; i++) {
            info.nextMultiply[i] = cannonManager->levelConfigs[nextLvl+i].multiply;
            info.needDiamond[i] = cannonManager->levelConfigs[nextLvl+i].needDiamondCount;
            info.boundGolds[i] = cannonManager->levelConfigs[nextLvl+i].boundGolds;
        }
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    L2E_CANNON_UNLOCK_LEVEL info;
    info.eProtocol = l2e_cannon_unlock_level;
    info.errNo = 0;
    if (cannonManager->CannonMaxLevel()) {
        info.errNo = 1;
    }else{
        if (!cannonManager->unlockCannonLevel()) {
            info.errNo = 1;
        }else{
            changeDiamond(-cannonManager->levelConfigs[cannonManager->getUnlockLevel()].needDiamondCount);
            changeGold(cannonManager->levelConfigs[cannonManager->getUnlockLevel()].boundGolds);
            int nextLvl = cannonManager->getUnlockLevel()+1;
            if (cannonManager->CannonMaxLevel()) {
                L2E_CANNON_MAX_LEVEL hideInfo;
                hideInfo.eProtocol = l2e_cannon_max_level;
                ClientLogic::instance()->pass2Engine(&hideInfo);
                info.errNo = 2;
//                saveMulti2CS();
//                return;
            }
            info.needDiamond = cannonManager->levelConfigs[nextLvl].needDiamondCount;
            info.nextMultiply = cannonManager->levelConfigs[nextLvl].multiply;
            info.boundGolds = cannonManager->levelConfigs[nextLvl].boundGolds;
            info.canUnlock = cannonManager->checkCanUnlockLevel(totalDiamond);
        }
    }
    info.totalDiamond = totalDiamond;
    ClientLogic::instance()->pass2Engine(&info);
    saveMulti2CS();
}

void GameCore::chargeUnlockLevel(E2L_CHARGE_UNLOCK_LEVEL pMsgInfo)
{
    if (currState == FISH_GAME_BATTLE_RECHAGE) {
        return;
    }
    if (cannonManager->checkCanUnlockLevel(totalDiamond)) {
        unlockCannonLevel();
        L2E_CHARGE_UNLOCK_LEVEL info;
        info.eProtocol = l2e_charge_unlock_level;
        info.bound = cannonManager->levelConfigs[cannonManager->getUnlockLevel()].boundGolds;
        ClientLogic::instance()->pass2Engine(&info);
    }else{
        changeGameState(FISH_GAME_BATTLE_RECHAGE);
        L2E_UNLOCK_NEED_DIAMOND info;
        info.eProtocol = l2e_cannon_unlock_need_diamond;
        ClientLogic::instance()->pass2Engine(&info);
    }
}

void GameCore::showSetting()
{
    changeGameState(FISH_GAME_SETTING);
    
    L2E_SHOW_SETTING info;
    info.eProtocol = l2e_show_setting;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::exitSetting()
{
    backGameState();
    
    L2E_EXIT_SETTING info;
    info.eProtocol = l2e_exit_setting;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::collectBossBound()
{
    changeGold(stageConfig.boundsMoney);
    changeDiamond(stageConfig.boundsDiamond);
}

void GameCore::showTask()
{
    changeGameState(FISH_GAME_TASK_BOUND);
    
    L2E_SHOW_TASK_BOUND info;
    info.eProtocol = l2e_show_task_bound;
    info.boundGold = missionConfig.boundsMoney;
    info.boundDiamond = missionConfig.boundsDiamond;
    info.taskState = stageMissionState;
    for (int i = 0; i < MAX_TASK_COUNT; i++) {
        info.fishRes[i] = fishManager->getFishRes(missionConfig.fishId[i]);
        info.fishType[i] = fishManager->getFishType(missionConfig.fishId[i]);
        info.fishMaxCount[i] = missionConfig.fishCount[i];
        info.complete[i] = (killTaskCount[i] >= missionConfig.fishCount[i]);
        info.fishGold[i] = missionConfig.fishGold[i];
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::takeTaskBound()
{
    changeGold(missionConfig.boundsMoney);
    changeDiamond(missionConfig.boundsDiamond);
    backGameState();
    
    stageMissionState = TASK_CLOSE;
    L2E_UPDATE_TASK info;
    info.eProtocol = l2e_update_task;
    info.taskState = stageMissionState;
    memcpy(info.fishCount, killTaskCount, sizeof(int)*MAX_TASK_COUNT);
    memcpy(info.fishMaxCount, missionConfig.fishCount, sizeof(int)*MAX_TASK_COUNT);
    ClientLogic::instance()->pass2Engine(&info);

    L2E_TAKE_TASK_BOUND infoTake;
    infoTake.eProtocol = l2e_take_task_bound;
    ClientLogic::instance()->pass2Engine(&infoTake);
    save2TaskCoreString();
}

void GameCore::hideTaskBound()
{
    backGameState();
    
    L2E_HIDE_TASK_BOUND info;
    info.eProtocol = l2e_hide_task_bound;
    ClientLogic::instance()->pass2Engine(&info);
}

bool GameCore::readSavedData(std::string filePath,rapidjson::Document &_doc)
{
    bool ret = false;
    
    std::string path(FileUtils::getInstance()->getWritablePath());
    path += filePath;
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(path);
    contentStr = decode(contentStr.c_str());
    
    _doc.Parse < 0 > (contentStr.c_str());
    ret = !_doc.HasParseError();
    
    return ret;
}

void GameCore::getSkillCountFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    for (int i = 0; i < 5; i++) {
        std::string temp("skill");
        temp += Convert2String(i+1);
        temp += "Count";
        skillCount[i] = DICTOOL->getIntValue_json(_doc, temp.c_str());
        skillManager->skillMap[i+1].count = skillCount[i];
    }
}

void GameCore::saveSkillCount2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    for (int i = 0; i < 5; i++) {
        std::string temp("skill");
        temp += Convert2String(i+1);
        temp += "Count";
        _doc[temp.c_str()] = skillManager->skillMap[i+1].count;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getCannonTypeFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    currCannonType = DICTOOL->getIntValue_json(_doc, "currCannonType");
    
    cannonManager->getHeroCannon().setCannonType(currCannonType);
    cannonManager->setHeroCannonType(currCannonType);
}

void GameCore::saveCannonType2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["currCannonType"] = cannonManager->getHeroCannonType();
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getMultiFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    currCannonMulti = DICTOOL->getIntValue_json(_doc, "currCannonMulti");
    unlockCannonLvl = DICTOOL->getIntValue_json(_doc, "unlockCannonLevel");
    
    cannonManager->setHeroMul(currCannonMulti);
    cannonManager->setUnlockLevel(unlockCannonLvl);
}

void GameCore::saveMulti2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["currCannonMulti"] = cannonManager->getHeroMul();
    _doc["unlockCannonLevel"] = cannonManager->getUnlockLevel();
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getTaskFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    stageMissionState = (TASK_STATES)DICTOOL->getIntValue_json(_doc, "taskState");
    killTaskCount[0] = DICTOOL->getIntValue_json(_doc, "kill1Count");
    killTaskCount[1] = DICTOOL->getIntValue_json(_doc, "kill2Count");
    killTaskCount[2] = DICTOOL->getIntValue_json(_doc, "kill3Count");
}

void GameCore::saveTask2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["taskState"] = stageMissionState;
    _doc["kill1Count"] = killTaskCount[0];
    _doc["kill2Count"] = killTaskCount[1];
    _doc["kill3Count"] = killTaskCount[2];
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getBossTaskFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    stageTargetState = (STAGE_TARGET_STATES)DICTOOL->getIntValue_json(_doc, "bossTaskState");
    killBoss1Count = DICTOOL->getIntValue_json(_doc, "bossCount");
    killBoss2Count = DICTOOL->getIntValue_json(_doc, "subBossCount");
    maxUnlockStage = DICTOOL->getIntValue_json(_doc, "unLockStage");
}

void GameCore::getKillBigFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    int killBigCount = DICTOOL->getIntValue_json(_doc, "killBigCount");
    int killBigScore = DICTOOL->getIntValue_json(_doc, "killBigScore");
    int killStarCount = DICTOOL->getIntValue_json(_doc, "killStarCount");
    
    lotteryManager->setKillBigCount(killBigCount);
    lotteryManager->setkillBigScore(killBigScore);
    lotteryManager->setkillStarCount(killStarCount);
}
void GameCore::saveBossTask2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["bossTaskState"] = stageTargetState;
    _doc["bossCount"] = killBoss1Count;
    _doc["subBossCount"] = killBoss2Count;
    _doc["unLockStage"] = maxUnlockStage;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::saveKillBig2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["killBigCount"] = lotteryManager->getKillBigCount();
    _doc["killBigScore"] = lotteryManager->getkillBigScore();
    _doc["killStarCount"] = lotteryManager->getkillStarCount();
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::saveRelife2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["RelifeId"] = relifeData.relifeId;
    _doc["RelifeState"] = relifeData.relifeState;
    _doc["RelifeCount"] = relifeData.relifeCountDown;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getRelifeFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    relifeData.relifeId = DICTOOL->getIntValue_json(_doc, "RelifeId");
    relifeData.relifeState = DICTOOL->getIntValue_json(_doc, "RelifeState");
    relifeData.relifeCountDown = DICTOOL->getFloatValue_json(_doc, "RelifeCount");
}

void GameCore::saveRecharge2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["rechargeRecord"] = rechargeRecord;
    for (int i = 0; i < MAX_RECHARGE_ITEM; i++) {
        _doc[("firstRecharge"+Convert2String(i+1)).c_str()] = firstRecharge[i];
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getRechargeFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    rechargeRecord = DICTOOL->getIntValue_json(_doc, "rechargeRecord");
    for (int i = 0; i < MAX_RECHARGE_ITEM; i++) {
        firstRecharge[i] = DICTOOL->getBooleanValue_json(_doc, ("firstRecharge"+Convert2String(i+1)).c_str());
    }
}

void GameCore::saveVip2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["vip"] = vip;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getVipFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    vip = DICTOOL->getIntValue_json(_doc, "vip");
}

void GameCore::saveBenefits2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["benefit1"] = hasReceiveBenefit1;
    _doc["benefit2"] = hasReceiveBenefit2;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getBenefitsFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    hasReceiveBenefit1 = DICTOOL->getBooleanValue_json(_doc, "benefit1");
    hasReceiveBenefit2 = DICTOOL->getBooleanValue_json(_doc, "benefit2");
}

void GameCore::saveShare2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["shareCount"] = shareCount;
    for (int i = 0; i < 4; i++) {
        _doc[("shareCountState"+Convert2String(i+1)).c_str()] = shareCountState[i];
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getShareFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    shareCount = DICTOOL->getIntValue_json(_doc, "shareCount");
    for (int i = 0; i < 4; i++) {
        shareCountState[i] = DICTOOL->getIntValue_json(_doc, ("shareCountState"+Convert2String(i+1)).c_str());
    }
}

void GameCore::saveProb2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["probForceCount"] = probForceCount;
    _doc["probForceRecharge"] = probForceRecharge;
    _doc["probForceGold"] = probForceGold;
    _doc["probForceDuration"] = probForceDuration;
    _doc["probTotalDuration"] = probTotalDuration;
    _doc["sceneProbDuration"] = sceneProbDuration;
    _doc["sceneProb"] = sceneProb;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getProbFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    probForceCount = DICTOOL->getIntValue_json(_doc, "probForceCount");
    probForceGold = DICTOOL->getUint64Value_json(_doc, "probForceGold");
//    std::string scoreStr = DICTOOL->getStringValue_json(_doc, "probForceGold");
//    std::stringstream strScoreValue;
//    
//    strScoreValue << scoreStr.c_str();
//    
//    strScoreValue >>  probForceGold;
////    probForceGold = std::stoull(scoreStr);
    probForceRecharge = DICTOOL->getUint64Value_json(_doc, "probForceRecharge");
//    std::string rechargeStr = DICTOOL->getStringValue_json(_doc, "probForceRecharge");
//    std::stringstream strValue;
//    
//    strValue << rechargeStr.c_str();
//    
//    strValue >>  probForceRecharge;
////    probForceRecharge = std::stoull(rechargeStr);
    probForceDuration = DICTOOL->getFloatValue_json(_doc, "probForceDuration");
    probTotalDuration = DICTOOL->getFloatValue_json(_doc, "probTotalDuration");
    sceneProbDuration = DICTOOL->getFloatValue_json(_doc, "sceneProbDuration");
    sceneProb = DICTOOL->getFloatValue_json(_doc, "sceneProb");
}

void GameCore::getLevelFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    level = DICTOOL->getIntValue_json(_doc, "level");
    title = gTitles[level/10];
}
void GameCore::saveLevel2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["level"] = level;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}
void GameCore::getExpScoreFCS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    expScore = DICTOOL->getIntValue_json(_doc, "exp");
}
void GameCore::saveExpScore2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["exp"] = expScore;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}
bool GameCore::getTotalScoreFromCoreString()
{
    if (coreDataStr == "") {
        return false;
    }
    rapidjson::Document _doc;
    
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    
    totalScore = DICTOOL->getUint64Value_json(_doc, "totalGold");
//    std::string scoreStr = DICTOOL->getStringValue_json(_doc, "totalGold");
//
//    std::stringstream strValue;
//    
//    strValue << scoreStr.c_str();
//    
//    strValue >>  totalScore;
//    totalScore = std::stoull(scoreStr);
    return true;
}

void GameCore::saveTotalScore2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
//    std::stringstream ss;
//    ss<<totalScore;
//    log("save totalScore:%s",ss.str().c_str());
//    _doc["totalGold"] = rapidjson::StringRef(ss.str().c_str());
    _doc["totalGold"] = totalScore;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

bool GameCore::getTotalDiamondFromCoreString()
{
    if (coreDataStr == "") {
        return false;
    }

    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    totalDiamond = DICTOOL->getIntValue_json(_doc, "totalDiamond");
    return true;
}

void GameCore::saveTotalDiamond2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["totalDiamond"] = totalDiamond;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::getCannonHoldFCS()
{
    if (coreDataStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    for (int i = 0; i < CANNON_TYPE_MAX_EX; i++) {
        cannonManager->holdCannon[i] = DICTOOL->getBooleanValue_json(_doc, ("holdCannon"+Convert2String(i)).c_str());
    }
    return;
}

void GameCore::getEvaluateFCS()
{
    if (coreDataStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    evaluated = DICTOOL->getBooleanValue_json(_doc, "evaluated");
    openedEvaluate = DICTOOL->getBooleanValue_json(_doc, "openedEvaluate");
    evaluateDay = DICTOOL->getStringValue_json(_doc, "evaluateDay");
    return;
}

void GameCore::saveEvaluate2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    _doc["evaluated"] = evaluated;
    _doc["openedEvaluate"] = openedEvaluate;
    _doc["evaluateDay"] = rapidjson::StringRef(evaluateDay.c_str());
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

void GameCore::saveCannonHold2CS()
{
    if (coreDataStr == "") {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    for (int i = 0; i < CANNON_TYPE_MAX_EX; i++) {
        _doc[("holdCannon"+Convert2String(i)).c_str()] = cannonManager->holdCannon[i];
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    coreDataStr = encode(s);
}

bool GameCore::parseTaskJson()
{
    if (taskDataStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(taskDataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    stageMissionState = (TASK_STATES)DICTOOL->getIntValue_json(_doc[currStage-1], "taskState");
    killTaskCount[0] = DICTOOL->getIntValue_json(_doc[currStage-1], "kill1Count");
    killTaskCount[1] = DICTOOL->getIntValue_json(_doc[currStage-1], "kill2Count");
    killTaskCount[2] = DICTOOL->getIntValue_json(_doc[currStage-1], "kill3Count");

    return true;
}

void GameCore::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "buyusave.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);

    
    path = FileUtils::getInstance()->getWritablePath();
    path += "buyutask.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
    
    
    path = FileUtils::getInstance()->getWritablePath();
    path += "buyudiary.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

bool GameCore::loadJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "buyusave.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreDataStr = FileUtils::getInstance()->getStringFromFile(path);
    if (coreDataStr == "") {
        return false;
    }
    
    return true;
}

bool GameCore::loadTaskJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "buyutask.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    taskDataStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseTaskJson()) {
        return false;
    }

    return true;
}

bool GameCore::loadDiaryJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "buyudiary.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    std::string dataStr = FileUtils::getInstance()->getStringFromFile(path);
    rapidjson::Document _doc;
    _doc.Parse < 0 > (dataStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    if (!_doc.IsArray()) {
        return false;
    }
    
    bigFishCountMap.clear();
    for (int i = 0; i < _doc.Size(); i++) {
        int fishTypeId = DICTOOL->getIntValue_json(_doc[i], "typeId");
        int fishCount = DICTOOL->getIntValue_json(_doc[i], "count");
        
        Big_Fish_Count bigFishCount;
        bigFishCount.fishName = fishManager->getFishName(fishTypeId);
        bigFishCount.fishRes = fishManager->getFishRes(fishTypeId);
        bigFishCount.fishType = fishManager->getFishType(fishTypeId);
        bigFishCount.count = fishCount;
        bigFishCountMap[fishTypeId] = bigFishCount;
    }
    return true;
}
void GameCore::save2DiaryJson()
{
    if (bigFishCountMap.empty()) {
        return;
    }
    rapidjson::Document _doc;
    _doc.SetArray();
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    
    for (auto bigFishCount : bigFishCountMap) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("typeId", bigFishCount.first, allocator);
        object.AddMember("count", bigFishCount.second.count, allocator);
        _doc.PushBack(object, allocator);
    }
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "buyudiary.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(s.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("GameCore::save2DiaryJson error! can not open %s",path.c_str());
    }
}

void GameCore::save2TaskCoreString()
{
    rapidjson::Document _doc;
    bool exist = false;
    if (taskDataStr != "") {
        std::string tempStr = decode(taskDataStr.c_str());
        exist = true;
        _doc.Parse < 0 > (tempStr.c_str());
        if(_doc.HasParseError())
        {
            return;
        }
    }else{
        _doc.SetArray();
        
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    if (exist) {
        _doc[currStage-1]["taskState"] = stageMissionState;
        _doc[currStage-1]["kill1Count"] = killTaskCount[0];
        _doc[currStage-1]["kill2Count"] = killTaskCount[1];
        _doc[currStage-1]["kill3Count"] = killTaskCount[2];
    } else {
        for (int i = 0; i < maxStage; i++) {
            rapidjson::Value object(rapidjson::kObjectType);
            bool isCurrStage = ((i+1)==currStage);
            object.AddMember("id", i+1, allocator);
            object.AddMember("taskState", isCurrStage?stageMissionState:TASK_INIT, allocator);
            object.AddMember("kill1Count", isCurrStage?killTaskCount[0]:0, allocator);
            object.AddMember("kill2Count", isCurrStage?killTaskCount[1]:0, allocator);
            object.AddMember("kill3Count", isCurrStage?killTaskCount[2]:0, allocator);
            _doc.PushBack(object, allocator);
        }
    }
    
    
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    taskDataStr = encode(s);
    //    log(">>>>>>>>>>>>>>>>>>>>>>>\n");
    //    log("%s\n", coreDataStr.c_str());
}
void GameCore::save2CoreString()
{
    rapidjson::Document _doc;
    bool exist = false;
    if (coreDataStr != "") {
        std::string tempStr = decode(coreDataStr.c_str());
        exist = true;
        _doc.Parse < 0 > (tempStr.c_str());
        if(_doc.HasParseError())
        {
            return;
        }
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    if (exist) {
        _doc["unLockStage"] = maxUnlockStage;
        _doc["totalGold"] = totalScore;
//        _doc["totalGold"] = rapidjson::StringRef(Convert2String(totalScore).c_str());
        _doc["totalDiamond"] = totalDiamond;
        _doc["currCannonMulti"] = cannonManager->getHeroCannon().getCannonMul();
        _doc["unlockCannonLevel"] = cannonManager->getUnlockLevel();
        _doc["currCannonType"] = cannonManager->getHeroCannonType();
        for (int i = 0; i < CANNON_TYPE_MAX_EX; i++) {
            _doc[("holdCannon"+Convert2String(i)).c_str()] = cannonManager->holdCannon[i];
        }
        _doc["level"] = level;
        _doc["exp"] = expScore;
        _doc["vip"] = vip;
        _doc["rechargeRecord"] = rechargeRecord;
        for(int i = 0; i < MAX_RECHARGE_ITEM; i++)
        {
            _doc[("firstRecharge"+Convert2String(i+1)).c_str()] = firstRecharge[i];
        }
        
        _doc["shareCount"] = shareCount;
        for (int i = 0; i < 4; i++) {
            _doc[("shareCountState"+Convert2String(i+1)).c_str()] = shareCountState[i];
        }

        if (stageTargetState == STAGE_TARGET_BOUND) {
            _doc["bossTaskState"] = STAGE_TARGET_CLOSE;
        }else{
            _doc["bossTaskState"] = stageTargetState;
        }
        
        _doc["bossCount"] = killBoss1Count;
        _doc["subBossCount"] = killBoss2Count;
        _doc["skill1Count"] = skillManager->skillMap[1].count;
        _doc["skill2Count"] = skillManager->skillMap[2].count;
        _doc["skill3Count"] = skillManager->skillMap[3].count;
        _doc["skill4Count"] = skillManager->skillMap[4].count;
        _doc["skill5Count"] = skillManager->skillMap[5].count;
//        _doc["skill5Count"] = 0;
        _doc["killBigCount"] = lotteryManager->getKillBigCount();
        _doc["killBigScore"] = lotteryManager->getkillBigScore();
        _doc["killStarCount"] = lotteryManager->getkillStarCount();
        
        _doc["benefit1"] = hasReceiveBenefit1;
        _doc["benefit2"] = hasReceiveBenefit2;
        
        _doc["probForceCount"] = probForceCount;
//        _doc["probForceRecharge"] = rapidjson::StringRef(Convert2String(probForceRecharge).c_str());
//        _doc["probForceGold"] = rapidjson::StringRef(Convert2String(probForceGold).c_str());
        _doc["probForceRecharge"] = probForceRecharge;
        _doc["probForceGold"] = probForceGold;

        _doc["probForceDuration"] = probForceDuration;
        _doc["probTotalDuration"] = probTotalDuration;
        _doc["sceneProbDuration"] = sceneProbDuration;
        _doc["sceneProb"] = sceneProb;
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        _doc["evaluated"] = evaluated;
        _doc["openedEvaluate"] = openedEvaluate;
        _doc["evaluateDay"] = rapidjson::StringRef(iOSHelper::todayStr.c_str());
#endif
    } else {
        _doc.AddMember("unLockStage", maxUnlockStage, allocator);
//        _doc.AddMember("totalGold", rapidjson::StringRef(Convert2String(totalScore).c_str()), allocator);
        _doc.AddMember("totalGold", totalScore, allocator);
        _doc.AddMember("totalDiamond", totalDiamond, allocator);
        
        _doc.AddMember("benefit1", hasReceiveBenefit1, allocator);
        _doc.AddMember("benefit2", hasReceiveBenefit2, allocator);
        
        _doc.AddMember("probForceCount", probForceCount, allocator);
//        _doc.AddMember("probForceRecharge", rapidjson::StringRef(Convert2String(probForceRecharge).c_str()), allocator);
//        _doc.AddMember("probForceGold", rapidjson::StringRef(Convert2String(probForceGold).c_str()), allocator);
        _doc.AddMember("probForceRecharge", probForceRecharge, allocator);
        _doc.AddMember("probForceGold", probForceGold, allocator);

        _doc.AddMember("probForceDuration", probForceDuration, allocator);
        _doc.AddMember("probTotalDuration", probTotalDuration, allocator);
        _doc.AddMember("sceneProbDuration", sceneProbDuration, allocator);
        _doc.AddMember("sceneProb", sceneProb, allocator);
        
        _doc.AddMember("shareCount", shareCount, allocator);
        _doc.AddMember("shareCountState1", shareCountState[0], allocator);
        _doc.AddMember("shareCountState2", shareCountState[1], allocator);
        _doc.AddMember("shareCountState3", shareCountState[2], allocator);
        _doc.AddMember("shareCountState4", shareCountState[3], allocator);
        
        _doc.AddMember("currCannonMulti", currCannonMulti, allocator);
        _doc.AddMember("unlockCannonLevel", unlockCannonLvl, allocator);
        _doc.AddMember("currCannonType", currCannonType, allocator);
        _doc.AddMember("holdCannon0", true, allocator);
        _doc.AddMember("holdCannon1", false, allocator);
        _doc.AddMember("holdCannon2", false, allocator);
        _doc.AddMember("holdCannon3", false, allocator);
        _doc.AddMember("holdCannon4", false, allocator);
        _doc.AddMember("holdCannon5", false, allocator);
        _doc.AddMember("holdCannon6", false, allocator);
        _doc.AddMember("holdCannon7", false, allocator);
        _doc.AddMember("holdCannon8", false, allocator);
        _doc.AddMember("holdCannon9", false, allocator);
        
//        for (int i = 1; i < CANNON_TYPE_MAX_EX; i++) {
////            std::string tempName("holdCannon");
////            tempName += Convert2String(i);
//            _doc.AddMember(rapidjson::StringRef(("holdCannon"+Convert2String(i)).c_str()), false, allocator);
//        }

        _doc.AddMember("level", level, allocator);
        _doc.AddMember("exp", expScore, allocator);
        
        _doc.AddMember("vip", vip, allocator);
        _doc.AddMember("firstRecharge1", firstRecharge[0], allocator);
        _doc.AddMember("firstRecharge2", firstRecharge[1], allocator);
        _doc.AddMember("firstRecharge3", firstRecharge[2], allocator);
        _doc.AddMember("firstRecharge4", firstRecharge[3], allocator);
        _doc.AddMember("firstRecharge5", firstRecharge[4], allocator);
        _doc.AddMember("firstRecharge6", firstRecharge[5], allocator);
        _doc.AddMember("firstRecharge7", firstRecharge[6], allocator);
        _doc.AddMember("firstRecharge8", firstRecharge[7], allocator);
        _doc.AddMember("firstRecharge9", firstRecharge[8], allocator);
        _doc.AddMember("firstRecharge10", firstRecharge[9], allocator);
        _doc.AddMember("firstRecharge11", firstRecharge[10], allocator);
        _doc.AddMember("firstRecharge12", firstRecharge[11], allocator);
        _doc.AddMember("rechargeRecord", rechargeRecord, allocator);
        
        if (stageTargetState == STAGE_TARGET_BOUND) {
            _doc.AddMember("bossTaskState", STAGE_TARGET_CLOSE, allocator);
        }else{
            _doc.AddMember("bossTaskState", stageTargetState, allocator);
        }
        _doc.AddMember("bossCount", killBoss1Count, allocator);
        _doc.AddMember("subBossCount", killBoss2Count, allocator);
        _doc.AddMember("skill1Count", skillCount[0], allocator);
        _doc.AddMember("skill2Count", skillCount[1], allocator);
        _doc.AddMember("skill3Count", skillCount[2], allocator);
        _doc.AddMember("skill4Count", skillCount[3], allocator);
        _doc.AddMember("skill5Count", skillCount[4], allocator);
        _doc.AddMember("killBigCount", killBigCount, allocator);
        _doc.AddMember("killBigScore", killBigScore, allocator);
        _doc.AddMember("killStarCount", killStarCount, allocator);
        _doc.AddMember("RelifeId", relifeData.relifeId, allocator);
        _doc.AddMember("RelifeState", relifeData.relifeState, allocator);
        _doc.AddMember("RelifeCount", relifeData.relifeCountDown, allocator);
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        _doc.AddMember("evaluated", evaluated, allocator);
        _doc.AddMember("openedEvaluate", openedEvaluate, allocator);
        _doc.AddMember("evaluateDay", rapidjson::StringRef(iOSHelper::todayStr.c_str()), allocator);
#endif
    }
    
    
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
//    log("========================\n");
//    log("%s\n", s.c_str());
    coreDataStr = encode(s);
//    log(">>>>>>>>>>>>>>>>>>>>>>>\n");
//    log("%s\n", coreDataStr.c_str());

}

void GameCore::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "buyusave.json";

    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(coreDataStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
    
    auto taskPath = FileUtils::getInstance()->getWritablePath();
    taskPath += "buyutask.json";
    
    FILE* pTaskFile = fopen(taskPath.c_str(),"wb");
    if (pTaskFile)
    {
        fputs(taskDataStr.c_str(),pTaskFile);
        fclose(pTaskFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",taskPath.c_str());
    }

    save2DiaryJson();
}

void GameCore::enterNiuNiu()
{
    niuManager = BlackNiuNiu::instance();
    niuManager->deal();
    niuManager->getHandCard();
    
    L2E_ENTER_NIUNIU info;
    info.eProtocol = l2e_enter_niuniu;
    memcpy(info.cards, niuManager->getHandCard(), 5*sizeof(int));
    info.cannonMultiply = cannonManager->getHeroCannon().getMultiple();
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::enterMary()
{
    maryManager = BlackMary::instance();
    maryManager->startMary(cannonManager->getHeroCannon().getMultiple());
    maryManager->randRoll();
    
    L2E_ENTER_MARY info;
    info.eProtocol = l2e_enter_mary;
    info.roundIndex = maryManager->getRoundBtnIndex();
    info.roundTypeId = maryManager->getRoundTypeId();
    info.cardType = maryManager->getCardType();
    info.stageScore = maryManager->getStageScore();
    info.stageDiamond = maryManager->getStageDiamond();
    info.totalScore = totalScore;
    info.cannonMulti = maryManager->getCannonMulti();
    memcpy(info.centerIndex, maryManager->centerTypeId, sizeof(int)*4);
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::onLoadFinish()
{
    Load::onLoadFinish();
    //加载完成，转入登陆状态
    changeGameState(FISH_GAME_INIT);
}

void GameCore::load()
{
    L2E_UPDATE_LOAD_STAGE info;
    info.eProtocol = l2e_update_load_stage;
    info.isOver = false;
    info.step = 0;
    info.count = (int)m_vecLoadFunc.size();
    int result = Load::load();
    if (result != -1) {
        info.step = result;
        //更新加载界面
        ClientLogic::instance()->pass2Engine(&info);
    }else{
        onLoadFinish();
    }
    
}

void GameCore::loadCannon()
{
    cannonManager->clear();
    getCannonHoldFCS();
    getCannonTypeFCS();
    getMultiFCS();
    if (currCannonMulti < stageConfig.minMultiply) {
        if (unlockCannonLvl < stageConfig.minMultiply) {
            currCannonMulti = unlockCannonLvl;
            return;
        }
    }
    currCannonMulti = MAX(stageConfig.minMultiply, currCannonMulti);
    cannonManager->setHeroMul(currCannonMulti);
    saveMulti2CS();
}

void GameCore::loadPath()
{
    if (!PathManager::instance()->getLoadOver()) {
        PathManager::instance()->initPath("path/");
    }
}

void GameCore::loadTask()
{
    getBossTaskFCS();
    readStageFile(currStage, STAGE_CONFIG_FILE);
    readTaskFile(stageConfig.missionId, MISSION_CONFIG_FILE);
    
    if (currStage != maxUnlockStage) {
        //除最后一关外，其他关都不显示下一关按钮
        stageTargetState = STAGE_TARGET_CLOSE;
    }
    
    parseTaskJson();
}

void GameCore::loadFish()
{
    fishManager->loadFishConfig(excludeFishIds);
}

void GameCore::showDiary()
{
    L2E_SHOW_DIARY info;
    info.eProtocol = l2e_show_diary;
    info.fishTypeCount = (int)bigFishCountMap.size();
    int i = 0;
    for (auto bigFishCount : bigFishCountMap) {
        if (i >= 100) {
            break;
        }
        info.killFishType[i] = bigFishCount.second.fishType;
        info.killRes[i] = bigFishCount.second.fishRes;
        info.killTitle[i] = bigFishCount.second.fishName;
        info.killCount[i] = bigFishCount.second.count;
        i++;
    }
    
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::showFishSetting()
{
    L2E_SHOW_FISH info;
    info.eProtocol = l2e_show_fish;
    std::vector<FishConfig> showConfigs;
    showConfigs.clear();
    fishManager->getShowFishConfig(showConfigs);
    int index = 0;
    for (auto showConfig : showConfigs) {
        if (showConfig.type == SMALL_FISH) {
            info.fishType[index] = SMALL_FISH;
            info.fishRes[index] = showConfig.resource;
            info.fishPrice[index] = showConfig.price;
            index++;
        }
    }
    for (auto showConfig : showConfigs) {
        if (showConfig.type == TINY_BOSS_FISH) {
            info.fishType[index] = TINY_BOSS_FISH;
            info.fishRes[index] = showConfig.resource;
            info.fishPrice[index] = showConfig.price;
            index++;
        }
    }
    for (auto showConfig : showConfigs) {
        if (showConfig.type == BOSS_FISH) {
            info.fishType[index] = SPECIAL_BOSS_FISH;
            info.fishRes[index] = showConfig.resource;
            info.fishPrice[index] = showConfig.price;
            index++;
        }
    }

    info.fishTypeCount = index;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::removeDrop(int dropId)
{
    if (dropItemMap.empty()) {
        return;
    }
    if (dropItemMap.find(dropId) == dropItemMap.end())
    {
        return;
    }
    
    dropItemMap[dropId].receiveCount--;
    if (dropItemMap[dropId].receiveCount == 0) {
        receiveDropItem(dropItemMap[dropId]);
        dropItemMap.erase(dropId);
    }
}

void GameCore::clearDrop()
{
    if (dropItemMap.empty()) {
        return;
    }

    auto itr = dropItemMap.begin();
    while(itr != dropItemMap.end())
    {
        receiveDropItem(itr->second);
        itr = dropItemMap.erase(itr);
    }
}

void GameCore::receiveDropItem(Fish_Drop_Item item)
{
    switch (item.dropItemType) {
        case 0:
        {
            changeDiamond(item.dropItemCount);
        }
            break;
        case 1:
        {
            getSkillCountFCS();
            if (skillManager->skillMap[item.dropItemId].count >= 0) {
                skillManager->skillMap[item.dropItemId].count += item.dropItemCount;
            }
            
            saveSkillCount2CS();
            L2E_UPDATE_SKILL skillInfo;
            skillInfo.eProtocol = l2e_update_skill;
            for (int i = 0; i < 5; i++) {
                skillInfo.skillCount[i] = skillManager->skillMap[i+1].count;
                skillInfo.skillPrice[i] = skillManager->skillMap[i+1].price;
            }
            ClientLogic::instance()->pass2Engine(&skillInfo);
        }
            break;
            
        default:
            break;
    }
}

void GameCore::showCannonHold()
{
    getCannonHoldFCS();
    getCannonTypeFCS();
    L2E_SHOW_CANNON_SETTING info;
    info.eProtocol = l2e_show_cannon_setting;
    info.focus = cannonManager->getHeroCannonType();
    for (int i = 0; i < CANNON_TYPE_MAX_EX; i++) {
        info.holdCannon[i] = cannonManager->holdCannon[i];
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::updateCannonHold()
{
    L2E_UPDATE_CANNON_SETTING info;
    info.eProtocol = l2e_update_cannon_setting;
    for (int i = 0; i < CANNON_TYPE_MAX_EX; i++) {
        info.holdCannon[i] = cannonManager->holdCannon[i];
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::cannonUpgradeVip()
{
    getVipFCS();
    if (vip == 0) {
        showVipGift();
    }else{
        showVipIntro();
    }
}

void GameCore::takeRelife()
{
    getRelifeFCS();
    if (relifeData.relifeState == 2) {
        Relife_Config config;
        readRelifeConfig(relifeData.relifeId, config, RELIFE_CONFIG_FILE);
        
        relifeData.relifeId++;
        relifeData.relifeId = MIN(3, relifeData.relifeId);
        relifeData.relifeState = 0;
        relifeData.relifeCountDown = 0;
        saveRelife2CS();
        
        L2E_UPDATE_RELIFE info;
        info.eProtocol = l2e_update_relife;
        info.state = 3;
        info.countDown = 0;
        info.bound = 0;
        ClientLogic::instance()->pass2Engine(&info);
        int bounds = config.bounds;
        getVipFCS();
        if (vip >= 3) {
            bounds *= 2;
        }

        changeGold(bounds);
    }
    
}

void GameCore::unlock2Require(E2L_UNLOCK_TO_REQUIRE pMsgInfo)
{
    hideMultiNotEnough();
    getMultiFCS();
    
    if (cannonManager->CannonMaxLevel()) {
        return;
    }
    
    L2E_CANNON_UNLOCK_LEVEL info;
    info.eProtocol = l2e_cannon_unlock_level;
    info.errNo = 0;
    
    int unlockCount = 0;
    while (cannonManager->getUnlockLevel() < stageConfig.minMultiply)
    {
        int nextLvl = cannonManager->getUnlockLevel()+1;
        if (totalDiamond < cannonManager->levelConfigs[nextLvl].needDiamondCount) {
            changeGameState(FISH_GAME_UPGRADE_LEVEL);
            L2E_CANNON_SHOW_UNLOCK_DIALOG info;
            info.eProtocol = l2e_cannon_show_unlock_dialog;
            info.cardCount = CanonnManager::maxLevel - cannonManager->getUnlockLevel();
            info.cardCount = MIN(5, info.cardCount);
            if (info.cardCount <= 0) {
                break;
            }
            
            for (int i = 0; i < info.cardCount; i++) {
                info.nextMultiply[i] = cannonManager->levelConfigs[nextLvl+i].multiply;
                info.needDiamond[i] = cannonManager->levelConfigs[nextLvl+i].needDiamondCount;
                info.boundGolds[i] = cannonManager->levelConfigs[nextLvl+i].boundGolds;
            }
            ClientLogic::instance()->pass2Engine(&info);
            if (unlockCount == 0) {
                return;
            }
            break;
        }
        if (cannonManager->unlockCannonLevel()) {
            changeDiamond(-cannonManager->levelConfigs[nextLvl].needDiamondCount);
            changeGold(cannonManager->levelConfigs[nextLvl].boundGolds);
            if (cannonManager->CannonMaxLevel()) {
                L2E_CANNON_MAX_LEVEL maxInfo;
                maxInfo.eProtocol = l2e_cannon_max_level;
                ClientLogic::instance()->pass2Engine(&maxInfo);
                info.errNo = 2;
            }
            unlockCount++;
        }else{
            info.errNo = 1;
            break;
        }
    }
    if (info.errNo == 0) {
        info.needDiamond = cannonManager->levelConfigs[cannonManager->getUnlockLevel()+1].needDiamondCount;
        info.nextMultiply = cannonManager->levelConfigs[cannonManager->getUnlockLevel()+1].multiply;
        info.boundGolds = cannonManager->levelConfigs[cannonManager->getUnlockLevel()+1].boundGolds;
        info.canUnlock = cannonManager->checkCanUnlockLevel(totalDiamond);
    }
    info.totalDiamond = totalDiamond;
    ClientLogic::instance()->pass2Engine(&info);
    saveMulti2CS();
    
}

void GameCore::hideMultiNotEnough()
{
    L2E_HIDE_MULTI_NOT_ENOUGH info;
    info.eProtocol = l2e_hide_multi_not_enough;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::showBattleRecharge(E2L_SHOW_GAME_RECHARGE pMsgInfo)
{
    changeGameState(FISH_GAME_BATTLE_RECHAGE);
    getVipFCS();
    if (vip >= 1) {
        showHallRecharge(pMsgInfo.type);
    }else{
        showVipGift();
    }
    
}
void GameCore::showHallRecharge(int type)
{
    getVipFCS();
    getRechargeFCS();
    Vip_Config config;
    if (vip < 10){
        int vipId = vip+1;
        vipId = MIN(10, vipId);
        readVipConfig(vipId, config, VIP_CONFIG_FILE);
    }
    
    L2E_SHOW_HALL_RECHARGE info;
    info.eProtocol = l2e_show_hall_recharge;
    info.vip = vip;
    if (vip >= 10) {
        info.nextNeedMoney = 0;
        info.nextPercent = 100;
    }else{
        info.nextNeedMoney = config.needMoney - rechargeRecord;
        info.nextPercent = rechargeRecord*100.0/config.needMoney;
    }
    for (int i = 0; i < MAX_RECHARGE_ITEM; i++) {
        info.firstRecharge[i] = firstRecharge[i];
    }
    info.type = type;
    
    ClientLogic::instance()->pass2Engine(&info);
    
}

void GameCore::showVipIntro()
{
    getVipFCS();
    getRechargeFCS();
    Vip_Config config;
    if (vip < 10){
        int vipId = vip+1;
        vipId = MIN(10, vipId);
        readVipConfig(vipId, config, VIP_CONFIG_FILE);
    }
    
    L2E_SHOW_VIP_INTRO info;
    info.eProtocol = l2e_show_vip_info;
    info.vip = vip;
    if (vip == 10) {
        info.nextNeedMoney = 0;
        info.nextPercent = 100;
    }else{
        info.nextNeedMoney = config.needMoney - rechargeRecord;
        info.nextPercent = rechargeRecord*100.0/config.needMoney;
    }
    
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::showVipGift()
{
    L2E_SHOW_VIP_GIFT info;
    info.eProtocol = l2e_show_vip_gift;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::showVip()
{
    getVipFCS();
    if (vip == 0) {
        showVipGift();
    }else{
        showVipIntro();
    }
}

void GameCore::recharge(E2L_RECHARGE info)
{
    C2S_RECHARGE c2sInfo;
    c2sInfo.eProtocol = c2s_recharge;
    c2sInfo.rechargeId = info.rechargeId;
    Recharge_Config config;
    readRechargeConfig(info.rechargeId, config, RECHARGE_CONFIG_FILE);
    c2sInfo.price = config.price;
    ClientLogic::instance()->pass2Server(&c2sInfo);
}

void GameCore::responseRecharge(S2C_RECHARGE info)
{
    log("response recharge");
    log("response err:%d", info.errNo);
    if (info.errNo == 0) {
        Recharge_Config config;
        readRechargeConfig(info.rechargeId, config, RECHARGE_CONFIG_FILE);
        getVipFCS();
        getRechargeFCS();
        log("order:%d", info.rechargeId);
        log("amount:%d", config.amount);
        //最终数额=(基本数额+充值加成)*VIP加成
        int result = config.amount;
        if (firstRecharge[info.rechargeId-1]) {
            result *= (float)config.firstFactor/100.0;
            firstRecharge[info.rechargeId-1] = false;
            if (info.rechargeId < 7) {
                //每个金币的首充都送100钻石
                changeDiamond(100);
            }
        }else{
            result += result*(float)config.factor/100.0;
        }
        if (vip >= 10) {
            result *= 1.2;
        }else if (vip >= 5) {
            result *= 1.1;
        }
//        log("add:%d",result);
        
        if (rechargeRecord == 0) {
            sceneProbDuration = probTotalDuration = REBATE_TIME_LIMIT;
            sceneProb = gRebateProb;
            saveProb2CS();
        }
        //记录充值金额，最大不超过100W
        rechargeRecord += config.price;
        rechargeRecord = MIN(1000000, rechargeRecord);
        saveRecharge2CS();
        
        
        if(config.type == 0)
        {
            changeDiamond(result);
        } else if (config.type == 1) {
            changeGold(result);
            getProbFCS();
            probForceRecharge += result;
            saveProb2CS();
        }
        
        if (vip < 10) {
            Vip_Config vipConfig;
            readVipConfig(vip+1, vipConfig, VIP_CONFIG_FILE);
            while (rechargeRecord >= vipConfig.needMoney) {
                vip++;
                saveVip2CS();
                
                //vip升级后，解锁炮
                getCannonHoldFCS();
                for (int i = 0; i < CANNON_TYPE_MAX_EX-1; i++) {
                    cannonManager->holdCannon[i] = (vip>=i);
                }
                saveCannonHold2CS();
                updateCannonHold();
                
                switch (vip) {
                    case 6:
                    {
                        //送10个激光炮
                        getSkillCountFCS();
                        if (skillManager->skillMap[4].count >= 0) {
                            skillManager->skillMap[4].count += 10;
                        }
                        
                        saveSkillCount2CS();
                        L2E_UPDATE_SKILL skillInfo;
                        skillInfo.eProtocol = l2e_update_skill;
                        for (int i = 0; i < 5; i++) {
                            skillInfo.skillCount[i] = skillManager->skillMap[i+1].count;
                            skillInfo.skillPrice[i] = skillManager->skillMap[i+1].price;
                        }
                        ClientLogic::instance()->pass2Engine(&skillInfo);
                    }
                        break;
                    case 7:
                    {
                        //送10个炸弹
                        getSkillCountFCS();
                        if (skillManager->skillMap[5].count >= 0) {
                            skillManager->skillMap[5].count += 10;
                        }
                        
                        saveSkillCount2CS();
                        L2E_UPDATE_SKILL skillInfo;
                        skillInfo.eProtocol = l2e_update_skill;
                        for (int i = 0; i < 5; i++) {
                            skillInfo.skillCount[i] = skillManager->skillMap[i+1].count;
                            skillInfo.skillPrice[i] = skillManager->skillMap[i+1].price;
                        }
                        ClientLogic::instance()->pass2Engine(&skillInfo);
                    }
                        break;
                    case 8:
                    {
                        //锁定无限制使用
                        getSkillCountFCS();
                        skillManager->skillMap[1].count = -1;
                        saveSkillCount2CS();
                        L2E_UPDATE_SKILL skillInfo;
                        skillInfo.eProtocol = l2e_update_skill;
                        for (int i = 0; i < 5; i++) {
                            skillInfo.skillCount[i] = skillManager->skillMap[i+1].count;
                            skillInfo.skillPrice[i] = skillManager->skillMap[i+1].price;
                        }
                        ClientLogic::instance()->pass2Engine(&skillInfo);
                    }
                        break;
                    case 9:
                    {
                        //暴走无限制使用
                        getSkillCountFCS();
                        skillManager->skillMap[3].count = -1;
                        saveSkillCount2CS();
                        L2E_UPDATE_SKILL skillInfo;
                        skillInfo.eProtocol = l2e_update_skill;
                        for (int i = 0; i < 5; i++) {
                            skillInfo.skillCount[i] = skillManager->skillMap[i+1].count;
                            skillInfo.skillPrice[i] = skillManager->skillMap[i+1].price;
                        }
                        ClientLogic::instance()->pass2Engine(&skillInfo);
                    }
                        break;
                        
                    default:
                        break;
                }
                if (vip >= 10) {
                    break;
                }
                readVipConfig(vip+1, vipConfig, VIP_CONFIG_FILE);
            }
        }
        
        save2Json();
        L2E_RECHARGE_OK l2eInfo;
        l2eInfo.eProtocol = l2e_recharge_ok;
        l2eInfo.vip = vip;
        if (vip >= 10) {
            l2eInfo.nextNeedMoney = 0;
            l2eInfo.nextPercent = 100;
        }else{
            Vip_Config vipConfig;
            readVipConfig(vip+1, vipConfig, VIP_CONFIG_FILE);
            l2eInfo.nextNeedMoney = vipConfig.needMoney - rechargeRecord;
            l2eInfo.nextPercent = rechargeRecord*100.0/vipConfig.needMoney;
        }
        for (int i = 0; i < MAX_RECHARGE_ITEM; i++) {
            l2eInfo.firstRecharge[i] = firstRecharge[i];
        }
        l2eInfo.type = config.type;
        ClientLogic::instance()->pass2Engine(&l2eInfo);
    }
}

void GameCore::checkAutoFire()
{
    getVipFCS();
    
    if (vip >= 1) {
        L2E_CHECK_AUTO_FIRE info;
        info.eProtocol = l2e_check_auto_fire;
        info.errNo = 0;
        ClientLogic::instance()->pass2Engine(&info);
    }else{
        changeGameState(FISH_GAME_NEED_VIP);
        L2E_AUTO_FIRE_NEED_VIP info;
        info.eProtocol = l2e_auto_fire_need_vip;
        info.needVipLevel = 1;
        ClientLogic::instance()->pass2Engine(&info);
    }
    
}

void GameCore::closeRecharge()
{
    if (currState != FISH_GAME_HALL) {
        backGameState();
    }
    L2E_CLOSE_RECHARGE info;
    info.eProtocol = l2e_close_recharge;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::showBenefits(E2L_SHOW_BENEFITS info)
{
    getBenefitsFCS();
    L2E_SHOW_BENEFITS showInfo;
    showInfo.eProtocol = l2e_show_benefits;
    showInfo.hasReceive1 = hasReceiveBenefit1;
    showInfo.hasReceive2 = hasReceiveBenefit2;
    if (hasReceiveBenefit1 && hasReceiveBenefit2) {
        showInfo.type = 2;
    }else{
        showInfo.type = info.type;
    }
    
    ClientLogic::instance()->pass2Engine(&showInfo);
}

void GameCore::cashBenefitCode(E2L_CASH_BENEFIT_CODE info)
{
    C2S_CASH_BENEFIT_CODE c2sInfo;
    c2sInfo.eProtocol = c2s_cash_benefit_code;
    c2sInfo.code = info.codeStr;
    ClientLogic::instance()->pass2Server(&c2sInfo);
}

void GameCore::responseCashBenefitCode(S2C_CASH_BENEFIT_CODE s2cinfo)
{
    L2E_CASH_BENEFIT_CODE info;
    info.eProtocol = l2e_cash_benefit_code;
    if (s2cinfo.errNo != 0) {
        info.errNo = s2cinfo.errNo;
    } else {
        getBenefitsFCS();
        if (s2cinfo.result == 1) {
            if (hasReceiveBenefit1) {
                info.errNo = 2;
            }else{
                hasReceiveBenefit1 = true;
                info.errNo = 0;
                saveBenefits2CS();
                
                changeGold(10000);
                changeDiamond(100);
                save2Json();
            }
            
        }else if (s2cinfo.result == 2){
            if (hasReceiveBenefit2) {
                info.errNo = 2;
            }else{
                hasReceiveBenefit2 = true;
                info.errNo = 0;
                saveBenefits2CS();
                
                changeGold(5000);
                changeDiamond(100);
                save2Json();
            }
        }
    }
    if (hasReceiveBenefit2 && hasReceiveBenefit1) {
        info.type = 2;
    }else{
        info.type = 1;
    }
    info.hasReceive1 = hasReceiveBenefit1;
    info.hasReceive2 = hasReceiveBenefit2;

    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::link2NetGame(E2L_LINK_TO_NETGAME info)
{
    C2S_LINK_TO_NETGAME c2sinfo;
    c2sinfo.eProtocol = c2s_link_to_netgame;
    c2sinfo.type = info.type;
    ClientLogic::instance()->pass2Server(&c2sinfo);
}

void GameCore::openCommnet(E2L_COMMENT info)
{
    C2S_LINK_TO_NETGAME c2sinfo;
    c2sinfo.eProtocol = c2s_link_to_netgame;
    c2sinfo.type = 0;
    ClientLogic::instance()->pass2Server(&c2sinfo);
    evaluated = true;
    changeGold(3000);
    saveEvaluate2CS();
}

void GameCore::showShare()
{
    getShareFCS();
    L2E_SHOW_SHARE info;
    info.eProtocol = l2e_show_share;
    info.shareCount = shareCount;
    memcpy(info.countState, shareCountState, sizeof(int)*4);
    ClientLogic::instance()->pass2Engine(&info);
    
}

void GameCore::share(E2L_SHARE info)
{
    C2S_SHARE c2sInfo;
    c2sInfo.eProtocol = c2s_share;
    c2sInfo.channalId = info.channalId;
    ClientLogic::instance()->pass2Server(&c2sInfo);
}

void GameCore::responseShare(S2C_SHARE s2cInfo)
{
    getShareFCS();
    L2E_SHARE_RESULT info;
    info.eProtocol = l2e_share_result;
    info.errNo = s2cInfo.errNo;
    if (s2cInfo.errNo == 0) {
        shareCount++;
        for (int i = 0; i < 4; i++) {
            if (gShareCountThres[i] > shareCount) {
                break;
            }
            
            if (shareCountState[i] == 0) {
                shareCountState[i] = 1;
            }
        }
        info.shareCount = shareCount;
        memcpy(info.countState, shareCountState, sizeof(int)*4);
        saveShare2CS();
    }
    ClientLogic::instance()->pass2Engine(&info);

}

void GameCore::takeShareCountBound(E2L_SHARE_COUNT_BOUND info)
{
    getShareFCS();
    if (shareCountState[info.index-1] != 1) {
        return;
    }
    
    shareCountState[info.index-1] = 2;
    changeGold(gShareCountBounds[info.index-1]);
    saveShare2CS();
    save2Json();
    
    L2E_SHARE_COUNT_BOUND info1;
    info1.eProtocol = l2e_share_count_bound;
    info1.shareCount = shareCount;
    memcpy(info1.countState, shareCountState, sizeof(int)*4);
    ClientLogic::instance()->pass2Engine(&info1);
}

bool GameCore::showComment()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if (!iOSHelper::bOnline) {
        return false;
    }
    getEvaluateFCS();
    if (evaluated) {
        return false;
    }
    if (iOSHelper::todayStr == evaluateDay) {
        if (openedEvaluate) {
            return false;
        }
    }
    
    openedEvaluate = true;
    evaluateDay = iOSHelper::todayStr;
    saveEvaluate2CS();
    return true;
#else
    return false;
#endif
    
//    L2E_SHOW_EVALUATE info;
//    info.eProtocol = l2e_show_evaluate;
//    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::jniRecharge(cocos2d::EventCustom *event)
{
    log("jniRecharge");
    S2C_RECHARGE info = *static_cast<S2C_RECHARGE*>(event->getUserData());
    log("jni recharge err:%d",info.errNo);
    if (info.errNo == 0) {
        jniRechargeId = info.rechargeId;
        jniRechargeOk = true;
        log("set var ok");
    }
    
}