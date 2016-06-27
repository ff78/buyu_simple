//
//  GoldGame.h
//  buyu_simple
//
//  Created by ff on 15/12/21.
//
//

#ifndef GoldGame_h
#define GoldGame_h

#include "utils/GameUtils.h"
#include "Load.h"
#include "ELProtocol.h"
#include "data/BlackGoldGame.h"
#include "data/BlackPliers.h"
#include "cocos2d.h"

enum GOLD_GAME_STATES
{
    GOLD_GAME_SHOW_MISSION = 1,
    GOLD_GAME_NORMAL,
    GOLD_GAME_GET,
    GOLD_GAME_SUCCESS,
    GOLD_GAME_FAILED
};

class GoldGame : public Singleton<GoldGame>, Load{
    
    
public:
    GoldGame();
    ~GoldGame();
    
    typedef void (GoldGame::*LoadFunc)();
    void load();
    void initLoadFunc();
    
    void logic(float dt);
    
    //黄金矿工游戏状态控制
    void changeGameState(GOLD_GAME_STATES nextState);
    
    void enterGameState(GOLD_GAME_STATES nextState);
    
    void exitGameState(GOLD_GAME_STATES state);
    
    //难度选择请求
    void requestDiff(E2L_GOLD_DIFF &info);
    //选定难度
    void selDiff(E2L_GOLD_SEL_DIFF &info);
    //选择关卡
    void selStage(E2L_SEL_GOLD &info_in);
    //开启关卡选择界面的宝箱
    void openStageBox(E2L_OPEN_BOX &info);
    //开启命运转盘界面的宝箱
    void openFateBox(E2L_OPEN_BOX &info_in);
    //从关卡宝箱获得物品
    void getFromStageBox(E2L_OPEN_BOX &info);
    //从命运转盘宝箱获得物品
    void getFromFateBox(E2L_OPEN_BOX &info_in);
    //从命运转盘获得物品
    void getFromFateRoll(E2L_OPEN_BOX &info_in);
    //开始转动命运转盘
    void startRoll(E2L_GOLD_START_ROLL &info);
    //开始黄金矿工游戏
    void startGame(E2L_GOLD_START_GAME &info);
    //游戏结果处理
    void gameResult(E2L_GOLD_RESULT &info);
    //获取道具物品
    void gainObj();
protected:
    virtual void onLoadFinish();
private:
    void load1();
    
    void genGameMap();
    void useAllItems();
public:
    BlackGoldGame* gameData;
    
    BlackPliers* pliers;
    
private:
    CC_SYNTHESIZE_READONLY(GOLD_GAME_STATES, currState, CurrState);
    CC_SYNTHESIZE_READONLY(GOLD_GAME_STATES, lastState, LastState);
    
    int randStageMoney;
    int randStageItem;
    int randFateMoney;
    int randFateItem;
    int randRollItem;
    int sceneId;
};

#endif /* GoldGame_h */
