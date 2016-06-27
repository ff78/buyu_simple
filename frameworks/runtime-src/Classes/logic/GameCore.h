//
//  GameCore.h
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#ifndef __buyu_simple__GameCore__
#define __buyu_simple__GameCore__

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "CSProtocol.h"
#include "ELProtocol.h"
#include "EventDef.h"
#include "Load.h"
#include "data/BlackFish.h"
#include "data/FishManager.h"
#include "data/BulletManager.h"
#include "data/CannonManager.h"
#include "data/SkillManager.h"
#include "data/BlackNiuNiu.h"
#include "data/BlackMary.h"
#include "data/BlackLottery.h"

enum FISH_GAME_STATES
{
    FISH_GAME_NONE = 0,
    FISH_GAME_INIT = 1,
    FISH_GAME_NORMAL = 2,
    FISH_GAME_TIDE_TIP = 3,
    FISH_GAME_SWITCH_TIDE = 4,
    FISH_GAME_TIDE = 5,
    FISH_GAME_RAY_WAIT = 6,
    FISH_GAME_RAY = 7,
    FISH_GAME_NIUNIU = 8,
    FISH_GAME_MARY = 9,
    FISH_GAME_SETTING = 10,
    FISH_GAME_UPGRADE_LEVEL = 11,
    FISH_GAME_TASK_BOUND = 12,
    FISH_GAME_LOTTERY = 13,
    FISH_GAME_HALL = 14,
    FISH_GAME_LOAD = 15,
    FISH_GAME_NEED_VIP = 16,
    FISH_GAME_BATTLE_RECHAGE = 17
};

typedef struct __Relife_Config
{
    int relifeId;
    int countDownLimits;
    int bounds;
}Relife_Config;

typedef struct __Relife_Data
{
    int         relifeId;
    int         relifeState;
    float       relifeCountDown;
    
    __Relife_Data()
    {
        relifeId = 1;
        relifeState = 0;
        relifeCountDown = 0.0;
    }
}Relife_Data;


typedef struct __Stage_Config
{
    int stageId;                                //关卡ID
//    std::string bossRes;                        //主Boss的图片
    int bossId;                                 //主Boss的ID
    int bossCount;                              //主Boss的目标数
//    std::string subBossRes;                     //副Boss的图片
    int subBossId;                              //副Boss的ID
    int subBossCount;                           //副Boss的目标数
    int missionId;                              //刷新的任务ID
    int boundsMoney;                            //通关奖励金钱数
    int boundsDiamond;                          //通关奖励钻石数
    int dailyMoney;                             //每日关卡奖励金钱数
    int dailyDiamond;                           //每日关卡奖励钻石数
    int minMultiply;                            //最小倍数限制
    std::vector<int> dailySkills;               //每日关卡奖励技能ID
    std::vector<int> excludeFishIds;            //关卡屏蔽的鱼
}STAGE_CONFIG;

typedef struct __Stage_Mini_Config
{
    std::string bossRes;
    int bossType;
    int boundsMoney;
    int boundsDiamond;
}Stage_Mini_Config;

typedef struct __Mission_Config
{
    int missionId;
    int fishId[3];
    int fishCount[3];
    int fishGold[3];
    int boundsMoney;
    int boundsDiamond;
}MISSION_CONFIG;

typedef struct __Big_Fish_Count
{
    std::string     fishRes;
    int             fishType;
    std::string     fishName;
    int             count;
}Big_Fish_Count;

typedef struct __Recharge_Config
{
    int             rechargeId;
    int             type;
    int             price;
    int             amount;
    int             firstFactor;
    int             factor;
}Recharge_Config;

typedef struct __Vip_Config
{
    int             vipId;
    int             needMoney;
    int             dailyFeedback;
}Vip_Config;

//typedef unsigned long long int uint64_t;

//捕鱼战斗逻辑
class GameCore : public Singleton<GameCore>, public Load
{
public:
    GameCore();
    ~GameCore();
    
    //改变捕鱼战斗状态
    void changeGameState(FISH_GAME_STATES nextState);
    void enterGameState(FISH_GAME_STATES nextState);
    void exitGameState(FISH_GAME_STATES state);
    void backGameState();
    
    //清除所有活动物体，用于清场
    void clearAll();
    //根据状态分发逻辑
    void logic(float dt);
    
    //清除某条鱼
    void removeFish(E2L_REMOVE_OUT_FISH info);
    
    //潮汐来临
    void tideCome();
    //潮汐换屏结束
    void tideSwitchOver();
    
    //玩家发炮
    void heroFire(E2L_CANNON_FIRE info);
    
    //捕到某条鱼，并做完死亡动画
    void catchFish(E2L_FISH_DEAD info);
    
    void lightFish(E2L_FISH_LIGHT_DEAD info);
    void showCannonHold();
    void updateCannonHold();
    void cannonUpgradeVip();
    //选炮
    void chooseCannon(E2L_CHOOSE_CANNON info);
    //改变炮倍率
    void changeCannonMul(E2L_CHANGE_CANNON_MULTIPLY info);
    void useSkill(E2L_USE_SKILL info);
    
    //网碰到鱼
    void hitFishes(cocos2d::EventCustom* event);
    void bombFishes(cocos2d::EventCustom* event);
    void rayHitFishes(cocos2d::EventCustom *event);
    void frozenScreen(cocos2d::EventCustom *event);
    void readyRay(cocos2d::EventCustom* event);
    void endRayFire(cocos2d::EventCustom* event);
    void readyPlane(cocos2d::EventCustom* event);
    void endPlane(cocos2d::EventCustom* event);
    
    void readLevelFile(int idx, std::string filename);
    static void initLevelTable(rapidjson::Document &_doc);
    
    bool readRelifeConfig(int idx, Relife_Config& config, std::string filename);
    static void initRelifeTable(rapidjson::Document &_doc);
    
    bool readRechargeConfig(int idx, Recharge_Config& config, std::string filename);
    static void initRechargeTable(rapidjson::Document &_doc);
    
    bool readVipConfig(int idx, Vip_Config& config, std::string filename);
    static void initVipTable(rapidjson::Document &_doc);
    
    //读取指定关卡的数据
    void readStageFile(int idx, std::string filename);
    
    static void initIdStageTable(rapidjson::Document &_doc);
    
    void readTaskFile(int idx, std::string filename);
    
    static void initIdTaskTable(rapidjson::Document &_doc);
    
    void startRebate();
    
    void changeSceneProb();
    
    void startEnergyCannon();
    void endEnergyCannon();
    
    void fishCleared();
    
    void enterNiuNiu();
    void askShowHand();
    void niuniuEnd(E2L_NIUNIU_END pMsgInfo);
    
    void enterMary();
    void initMary();
    void exitMary();
    
    void enterLuckyDraw();
    void exitLuckyDraw();
    void updateLuckyDraw(E2L_UPDATE_LUCKY_DRAW pMsgInfo);
    void enterLottery();
    void endLottery();
    
    void addExp(int deltaVal);
    void levelUp();
    void changeEnergy(int deltaVal);
    void changeGold(int deltaVal);
    void changeDiamond(int deltaVal);
    void unlockCannonLevel();
    void hideUnlockLevel();
    void chargeUnlockLevel(E2L_CHARGE_UNLOCK_LEVEL pMsgInfo);
    
    void showSetting();
    void exitSetting();
    
    bool checkBossTarget(int fishTypeId);
    void collectBossBound();
    
    void showTask();
    void takeTaskBound();
    void hideTaskBound();
    void showDiary();
    void showFishSetting();
    
    void takeRelife();
    
    bool checkTask(int fishTypeId);
    
    bool checkKillBig(int fishTypeId);
    void save2Json();
    void save2CoreString();
    void save2TaskCoreString();
    void save2DiaryJson();
    bool readSavedData(std::string filePath,rapidjson::Document &_doc);
    bool loadJson();
    bool loadTaskJson();
    bool parseTaskJson();
    bool loadDiaryJson();
    void deleteJson();
    
    bool getTotalScoreFromCoreString();
    bool getTotalDiamondFromCoreString();
    void getExpScoreFCS();
    void getLevelFCS();
    void getBossTaskFCS();
    void getTaskFCS();
    void getMultiFCS();
    void getCannonTypeFCS();
    void getSkillCountFCS();
    void getKillBigFCS();
    void getVipFCS();
    void getCannonHoldFCS();
    void getRelifeFCS();
    void getRechargeFCS();
    void getBenefitsFCS();
    void getShareFCS();
    void getProbFCS();
    
    void saveTotalScore2CS();
    void saveTotalDiamond2CS();
    void saveExpScore2CS();
    void saveLevel2CS();
    void saveBossTask2CS();
    void saveTask2CS();
    void saveMulti2CS();
    void saveCannonType2CS();
    void saveSkillCount2CS();
    void saveKillBig2CS();
    void saveVip2CS();
    void saveCannonHold2CS();
    void saveRelife2CS();
    void saveRecharge2CS();
    void saveBenefits2CS();
    void saveShare2CS();
    void saveProb2CS();
    
    bool getDataFromCoreString();
    
    void loadStageMiniConfigs();
    
    void nextStage();
    
    void quitStage();
    
    typedef void (GameCore::*LoadFunc)();
    //加载过程逻辑处理
    void load();
    
    void loadPath();
    
    void loadTask();
    
    void loadFish();
    
    void loadCannon();

    void clearDrop();
    void removeDrop(int dropId);
    void receiveDropItem(Fish_Drop_Item item);
    
    void unlock2Require(E2L_UNLOCK_TO_REQUIRE pMsgInfo);
    void hideMultiNotEnough();
    
    void showHallRecharge(int type);
    void showVipIntro();
    void showVipGift();
    void showVip();
    
    void closeRecharge();
    void showBattleRecharge(E2L_SHOW_GAME_RECHARGE pMsgInfo);
    
    void recharge(E2L_RECHARGE info);
    void responseRecharge(S2C_RECHARGE info);
    void checkAutoFire();
    
    void showBenefits(E2L_SHOW_BENEFITS info);
    void cashBenefitCode(E2L_CASH_BENEFIT_CODE info);
    void responseCashBenefitCode(S2C_CASH_BENEFIT_CODE s2cinfo);
    void link2NetGame();
    
    void showShare();
    void share(E2L_SHARE info);
    void responseShare(S2C_SHARE s2cInfo);
    void takeShareCountBound(E2L_SHARE_COUNT_BOUND info);
protected:
    virtual void onLoadFinish();
private:
    bool initStageData(int idx, rapidjson::Document& _doc);
    bool initLevelData(int idx, rapidjson::Document &_doc);
    bool initTaskData(int idx, rapidjson::Document &_doc);
    bool initRelifeData(int idx, Relife_Config& config, rapidjson::Document &_doc);
    bool initRechargeData(int idx, Recharge_Config& config, rapidjson::Document &_doc);
    bool initVipData(int idx, Vip_Config& config, rapidjson::Document &_doc);
public:
    static std::map<int, int> idStageTable;
    static std::map<int, int> levelIdTable;
    static std::map<int, int> taskIdTable;
    static std::map<int, int> relifeIdTable;
    static std::map<int, int> rechargeIdTable;
    static std::map<int, int> vipIdTable;
    
    std::map<int, Stage_Mini_Config> stageMiniConfigs;
    std::map<int, Big_Fish_Count> bigFishCountMap;
    std::map<int, Fish_Drop_Item> dropItemMap;
private:
    FishManager *fishManager;
    BulletManager *bulletManager;
    CanonnManager *cannonManager;
    SkillManager *skillManager;
    BlackNiuNiu *niuManager;
    BlackMary *maryManager;
    BlackLottery *lotteryManager;
    
    cocos2d::EventListenerCustom* hitListener;
    cocos2d::EventListenerCustom* rayHitListener;
    cocos2d::EventListenerCustom* bombListener;
    cocos2d::EventListenerCustom* frozenListener;
    cocos2d::EventListenerCustom* readyRayListener;
    cocos2d::EventListenerCustom* endRayListener;
    cocos2d::EventListenerCustom* readyPlaneListener;
    cocos2d::EventListenerCustom* endPlaneListener;
    
    std::vector<int> excludeFishIds;
    std::vector<int> dailySkills;
    
    float nextTideDuration;
    
    STAGE_CONFIG stageConfig;
    MISSION_CONFIG missionConfig;
    int killTaskCount[MAX_TASK_COUNT];
    int skillCount[5];
    bool firstRecharge[12];
    int rechargeRecord;
    
    bool hasReceiveBenefit1;
    bool hasReceiveBenefit2;
    
    int shareCount;
    int shareCountState[4];
    
    int probForceCount;
    uint64_t probForceRecharge;
    uint64_t probForceGold;
    float probForceDuration;
    float probTotalDuration;
    float sceneProb;
    
    Relife_Data relifeData;
    
    int dropItemIncreacement;
    
    CC_SYNTHESIZE_READONLY(std::string, coreDataStr, CoreDataStr);
    CC_SYNTHESIZE_READONLY(std::string, taskDataStr, TaskDataStr);
    CC_SYNTHESIZE_READONLY(int, maxStage, MaxStage);
    CC_SYNTHESIZE_READONLY(int, maxUnlockStage, MaxUnlockStage);
    CC_SYNTHESIZE(int, currStage, CurrStage);
    CC_SYNTHESIZE_READONLY(int, totalDiamond, TotalDiamond);
    CC_SYNTHESIZE_READONLY(uint64_t, totalScore, TotalScore);
    CC_SYNTHESIZE_READONLY(int, energyScore, EnergyScore);
    CC_SYNTHESIZE_READONLY(float, energyDuration, EnergyDuration);
    CC_SYNTHESIZE_READONLY(int, expScore, ExpScore);
    CC_SYNTHESIZE_READONLY(int, maxExp, MaxExp);
    CC_SYNTHESIZE_READONLY(int, level, Level);
    CC_SYNTHESIZE_READONLY(std::string, title, Title);
    CC_SYNTHESIZE_READONLY(int, levelBoundGold, LevelBoundGold);
    CC_SYNTHESIZE_READONLY(int, levelBoundDiamond, LevelBoundDiamond);
    CC_SYNTHESIZE_READONLY(int, vip, Vip);
    CC_SYNTHESIZE_READONLY(int, currCannonMulti, CurrCannonMulti);
    CC_SYNTHESIZE_READONLY(int, unlockCannonLvl, UnlockCannonLvl);
    CC_SYNTHESIZE_READONLY(int, currCannonType, CurrCannonType);
    
    CC_SYNTHESIZE_READONLY(float, sceneProbDuration, SceneProbDuration);
    CC_SYNTHESIZE_READONLY(float, rebateDuration, RebateDuration);
    CC_SYNTHESIZE_READONLY(FISH_GAME_STATES, currState, CurrState);
    CC_SYNTHESIZE_READONLY(FISH_GAME_STATES, lastState, LastState);
    CC_SYNTHESIZE_READONLY(FISH_GAME_STATES, originState, OriginState);
    
    CC_SYNTHESIZE_READONLY(STAGE_TARGET_STATES, stageTargetState, StageTargetState);
    CC_SYNTHESIZE_READONLY(int, killBoss1Count, KillBoss1Count);
    CC_SYNTHESIZE_READONLY(int, killBoss2Count, KillBoss2Count);
//    CC_SYNTHESIZE_READONLY(std::string, bossRes, BossRes);
//    CC_SYNTHESIZE_READONLY(std::string, subBossRes, SubBossRes);
    CC_SYNTHESIZE_READONLY(TASK_STATES, stageMissionState, StageMissionState);
//    CC_SYNTHESIZE_READONLY(int, killTask1Count, KillTask1Count);
//    CC_SYNTHESIZE_READONLY(int, killTask2Count, KillTask2Count);
//    CC_SYNTHESIZE_READONLY(int, killTask3Count, KillTask3Count);
    CC_SYNTHESIZE_READONLY(int, killBigCount, KillBigCount);
    CC_SYNTHESIZE_READONLY(int, killStarCount, KillStarCount);
    CC_SYNTHESIZE_READONLY(int, killBigScore, KillBigScore);
    

};

#endif /* defined(__buyu_simple__GameCore__) */
