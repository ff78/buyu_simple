//
//  CannonManager.h
//  buyu_simple
//
//  Created by ff on 16/1/20.
//
//

#ifndef CannonManager_h
#define CannonManager_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "BlackCannon.h"
#include "ELProtocol.h"
#include "EventDef.h"
#include "GameDef.h"
#include "utils/GameReader.h"
#include "cocostudio/CocoStudio.h"

typedef struct __Cannon_Level_Config
{
    int ConfigId;
    int multiply;
    int needDiamondCount;
    int boundGolds;
}Cannon_Level_Config;

class CanonnManager : public Singleton<CanonnManager>
{
public:
    CanonnManager();
    ~CanonnManager();
    
    //清除拥有的炮的种类，清除炮
    void clear();
    
    void logic(float dt);
    
    //创建炮，初始化
    bool createCannon(CREATE_CANNON_CMD &cmd);
    
    //换炮
    void changeCannon(int typeId);
    
    void change2Energy();
    void endEnergy();
    //换倍率档
    void changeCannonMul(int deltaVal, int min);
    
    void readyRay();
    void endRay();
    
    void readyPlane();
    void endPlane();
    
    void loadCannonConfig(int configId);
    //读取关卡文件，完成关卡初始化
    void readFile(int idx, std::string filename);
    
    static void initIdTable(rapidjson::Document &_doc);
    bool initData(int idx, rapidjson::Document& _doc);
    
    void loadConfig(int minLevel);
    
    bool unlockCannonLevel();
    
    inline bool checkCanUnlockLevel(int diamond){
        return diamond >= levelConfigs[unlockLevel+1].needDiamondCount;
    };
    
    inline bool CannonMaxLevel(){
        return unlockLevel == maxLevel;
    };
    
    inline bool checkCannonLevel(int minLevel) {
        return unlockLevel >= minLevel;
    }
    
    inline int getHeroCannonType()
    {
        return heroCannonType;
    }
    
    inline void setHeroCannonType(int val)
    {
        heroCannonType = val;
    }
    
    inline void setHeroMul(int val)
    {
        heroMul = val;
        heroCannon.setCannonMul(val);
    }
public:
    std::map<int, BlackCannon> cannonMap;
    
    bool holdCannon[CANNON_TYPE_MAX_EX];
    
    std::map<int, Cannon_Level_Config> levelConfigs;
    static std::map<int, int> configIdTable;
    static int maxLevel;
private:
    int heroCannonType;
    CC_SYNTHESIZE(BlackCannon, heroCannon, HeroCannon);
//    CC_SYNTHESIZE(int, heroCannonType, HeroCannonType);
    CC_SYNTHESIZE(int, unlockLevel, UnlockLevel);
    CC_SYNTHESIZE_READONLY(int, heroMul, HeroMul);
};

#endif /* CannonManager_h */
