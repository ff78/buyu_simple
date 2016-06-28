//
//  FishManager.h
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#ifndef __buyu_simple__FishManager__
#define __buyu_simple__FishManager__

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "BlackFish.h"
#include "cocostudio/CocoStudio.h"

struct GroupFirstIdSet
{
    GroupFirstIdSet():group(0),count(0){}
    GroupFirstIdSet(int i, int j):group(i), count(j){}
    int group;
    int count;
};

typedef struct __FishConfig
{
    int configId;
    int type;
    int pathType;
    int price;
    int probability;
    float speed;
    int refreshInterval;
    int refreshMinCount;
    int refreshMaxCount;
    int refreshType;
    std::string resource;
    std::string name;
    std::string sound;
    cocos2d::Rect rect;
    
    int dropThres;
    int itemType[3];
    int itemId[3];
    int itemThres[3];
    int itemCountMax[3];
    int itemCountMin[3];
}FishConfig;

class FishManager : public Singleton<FishManager>
{
public:
    FishManager();
    ~FishManager();
    
    void logic(float dt);
    
    void tideClear();
    
    void createRandFish(float dt);
    void createSceneFish();
    //清除鱼
    void clear();
    
    //删除掉某条鱼
    void removeFish(int fishId);
    
    //读取鱼的数据，并初始化创建鱼的命令
    void readFile(int idx, std::string filename);
    
    static void initIdTable(rapidjson::Document &_doc);
        
    //加速所有的鱼
    void speedUpAll();
    
//    void hitFishes(cocos2d::EventCustom* event);
    
    //捕到鱼
    int catchFish(int fishId, int multiply, int expMultiply, int chairId, bool bomb = false, float rateFactor = 1);
    //捕到鱼组里其他的与
    void catchGroupsFish();
    //清除捕获的鱼组
    void clearCatchGroups();
    //炸弹清屏
    int doBombClear();
    
    int catchAllNetFish();
    
    int catchFishTypeBombFish();
    
    int catchLightningFish();
    //加载发鱼配置文件
    void loadFishConfig(std::vector<int> &excludeFish);
    
    void createGroupFish(FishConfig &config);
    
    void createDelayGroupFish(CMD_S_CREATE_FISH &cmd);
    
    void createNewFish(CMD_S_CREATE_FISH &cmd);
    
    std::string getFishName(int fishTypeId);
    std::string getFishRes(int fishTypeId);
    std::string getFishSound(int fishTypeId);
    int getFishType(int fishTypeId);
    void setPause(bool val);
    
    void getShowFishConfig(std::vector<FishConfig> &showConfig);
private:
    bool initData(int idx, rapidjson::Document& _doc);
    
public:
//    std::unordered_map<int, BlackFish> fishMap1;
    std::unordered_map<int, BlackFish> fishMap;
    
    std::vector<int> smallPathRand;
    std::vector<int> bigPathRand;
    std::vector<int> hugePathRand;
    std::vector<int> arrayPathRand;
    std::vector<int> specialPathRand;
    std::vector<int> hurtGroups;
    std::vector<int> dyingGroups;
    std::vector<int> bombTypeGroups;
    std::vector<int> bombTypeIdGroups;
    std::vector<int> lightGroups;
    
    std::vector<int> sceneFishStyleRand;
public:
    static std::map<int, int> idTable;
    
    static std::map<int, int> productTable;
    
    std::vector<CMD_S_CREATE_FISH> delayCreateCmd;
private:
    unsigned int IncrementID;
    unsigned int IncrementGroupId;
    
    std::map<int, GroupFirstIdSet>  mGroupThreeIDList;
    std::map<int, FishConfig> configs;
    float emitDuration[MAX_FISH_TYPE_EX];
   
    
    float selFishDuration;
    int selFishId;
    CMD_S_CREATE_FISH cmdCreate;
    int bombFishId;
    int bombMultiply;
    int lightMultiply;
    int allNetFishId;
//    cocos2d::EventListenerCustom* hitListener;
    bool bPause;
    
    
    CC_SYNTHESIZE_READONLY(float, emitInterval, EmitInterval);
    CC_SYNTHESIZE_READONLY(int, createFishIdx, CreateFishIdx);
    CC_SYNTHESIZE(float, selFishInterval, SelFishInterval);
    
    CC_SYNTHESIZE(bool, inTide, InTide);
    
//    CC_SYNTHESIZE(bool, pause, Pause);
    CC_SYNTHESIZE(float, pauseDuration, PauseDuration);
    CC_SYNTHESIZE(int, bossFishId, BossFishId);
    CC_SYNTHESIZE(float, sceneProb, SceneProb);
    CC_SYNTHESIZE(float, cannonProb, CannonProb);
    
    CC_SYNTHESIZE(int, priceSum, PriceSum);
};

#endif /* defined(__buyu_simple__FishManager__) */
