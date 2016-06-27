//
//  BlackGoldGame.h
//  buyu_simple
//
//  Created by ff on 15/12/23.
//
//

#ifndef BlackGoldGame_h
#define BlackGoldGame_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "GoldObj.h"
#include "cocostudio/CocoStudio.h"
#include "GameDef.h"

#define GRID_SIZE 16

#define MAX_COL 53
#define MAX_ZONE_ROW 10
#define A_ZONE_ROW 10

typedef struct __GRID_INFO
{
    int x;
    int y;
    int xVal;
    int yVal;
    
    bool operator()(__GRID_INFO& val)
    {
        return x == val.x && y == val.y;
    }
}GRID_INFO;

typedef struct __OBJ_INFO
{
    int serno;
    cocos2d::Rect collideRect;
    int moveType;
}OBJ_INFO;

class BlackGoldGame : public Singleton<BlackGoldGame>
{
public:
    BlackGoldGame();
    ~BlackGoldGame();
    
    //读取关卡文件，完成关卡初始化
    void readFile(int idx, std::string filename);
    
    static void initIdTable(rapidjson::Document &_doc);
    
    //碰撞到物体
    void onTouchThings(cocos2d::EventCustom* event);
    
    //获得某道具
    void addItem(GOLD_ITEM_TYPE type, bool inGame);
    
    //使用某道具
    void useItem(GOLD_ITEM_TYPE type);
    
    //挖到某物品
    void gainObj(int objSerno);
    
    //更新游戏倒计时
    void updateGameTime(float dt);
    
    //获得炸弹数量
    int getBombCount();
    
    //获得钥匙数量
    int getKeyCount();
    
    //清除钥匙计数
    void clearKey();
    
    //使用已选择的难度开始挑战
    void selDiff2Challenge();
    
    //开启下一关卡
    bool openNextStage();
    
    //难度选择时判断是否可以挑战
    int canChallenge();
    
    //是否能进某关
    bool canEnterStage(int stage);

    //获得挑战应付多少
    int resChallengeFee();
    
    //获得当前难度下的关卡索引
    int getDiffStage();
    
    //加钱
    void addMoney(int val);
    
    //用掉所有关卡道具
    void useAllItems();
    
    //清除设定数据
    void clearData();
private:
    bool initData(int idx, rapidjson::Document& _doc);
    
    //排列物品
    bool arrangeObj(float zoneOffY, std::vector<GoldObj> &Objs);
    
    void initGrids();
    
    //安排一个物品
    bool arrangeOneObj(GoldObj& obj, float zoneOffY);
public:
    static std::map<int, int> idTable;
    
    std::map<int, GoldObj> GoldObjs;
    
private:
    int zoneGridX[MAX_ZONE_ROW][MAX_COL];
    int zoneGridY[MAX_ZONE_ROW][MAX_COL];
    
    std::vector<GoldObj> AGoldObjs;
    std::vector<GoldObj> BGoldObjs;
    std::vector<GoldObj> CGoldObjs;
    
    cocos2d::EventListenerCustom* touchThingsListener;
    
    int itemCount[GOLD_ITEM_MAX_TYPE];
    
    CC_SYNTHESIZE_READONLY(int, sceneId, SceneId);
    CC_SYNTHESIZE_READONLY(int, missionType, MissionType);
    CC_SYNTHESIZE_READONLY(int, missionGold, MissionGold);
    CC_SYNTHESIZE_READONLY(int, missionKeyCount, MissionKeyCount);
    CC_SYNTHESIZE_READONLY(std::string, bgRes, BgRes);
    CC_SYNTHESIZE_READONLY(std::string, maskRes, MaskRes);

    
    CC_SYNTHESIZE_READONLY(int, money, Money);
    CC_SYNTHESIZE_READONLY(float, speedScale, SpeedScale);
    CC_SYNTHESIZE_READONLY(float, gameTime, GameTime);
    CC_SYNTHESIZE_READONLY(float, diamondScale, DiamondScale);
    CC_SYNTHESIZE_READONLY(float, goldScale, GoldScale);
    CC_SYNTHESIZE(float, luckScale, LuckScale);
    
    
    CC_SYNTHESIZE_READONLY(int, aliveCount, AliveCount);
    
    CC_SYNTHESIZE_READONLY(int, difficulty, Difficulty);
    CC_SYNTHESIZE_READONLY(int, challengeCount, ChanllengeCount);
    CC_SYNTHESIZE_READONLY(int, challengeLimit, ChallengeLimit);
    CC_SYNTHESIZE_READONLY(int, challengeFee, ChallengeFee);
    CC_SYNTHESIZE(int, tempChallengeCount, TempChallengeCount);
    
    CC_SYNTHESIZE_READONLY(int, diff1Stage, Diff1Stage);
    CC_SYNTHESIZE_READONLY(int, diff2Stage, Diff2Stage);
    CC_SYNTHESIZE_READONLY(int, diff3Stage, Diff3Stage);
    CC_SYNTHESIZE(int, stageBoxState, StageBoxState);
    CC_SYNTHESIZE(int, fateBoxState, FateBoxState);
    
    CC_SYNTHESIZE(bool, hasRolled, HasRolled);
    CC_SYNTHESIZE(bool, sucNextStage, SucNextStage);
    
    CC_SYNTHESIZE_READONLY(int, stageMoney, StageMoney);
};


#endif /* BlackGoldGame_h */
