//
//  MaryLayer.h
//  buyu_simple
//
//  Created by ff on 16/3/23.
//
//

#ifndef MaryLayer_h
#define MaryLayer_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ELProtocol.h"

enum MARY_GAME_STATE
{
    MARY_ROLL_INIT = 0,
    MARY_ROLL_START,
    MARY_ROLL_RUN,
    MARY_ROLL_SLOW,
    MARY_ROUND_RESULT,
    MARY_EXIT_RESULT
};


#define FOCUS_TAG 1000
//typedef unsigned long long int uint64_t;

class MaryLayer : public cocos2d::LayerColor {
public:
    MaryLayer();
    ~MaryLayer();
    
    bool init();
    
    CREATE_FUNC(MaryLayer);
    
    void changeGameState(int nextState);
    
    void exitGameState(int preState);
    void enterGameState(int nextState);
    
    void initData(cocos2d::EventCustom *event);
    void beginMary(cocos2d::EventCustom *event);
    
    void update(float dt);
    void showhand(int index);
    void exitMary();
    void remove();
private:
    cocos2d::Node *animationNode;
    cocostudio::timeline::ActionTimeline *dialogAction;
    
    cocos2d::Sprite *focusSpt;
    cocos2d::Sprite *lightSpt[4];
    
    bool lightOpen[4];
    int gameState;
    int lastState;
    int focus;
    int targetFocus;
    int targetTypeId;
    int centerTypeId[4];
    int targetCenterTypeId[4];
    int stageDiamond;
    int stageScore;
    int targetStageSocre;
    int scoreDecimalNumber;
    uint64_t totalScore;
    int cardType;
    
    int rollStep;
    int slowStep;
    int lampStep;
    
    float lampInterval;
    float lampDuration;
    float resultDuration;
    float blinkDuration;
    
    cocos2d::EventListenerCustom *initDataListener;
    
    cocos2d::Node *centerRollNode[4];
    cocostudio::timeline::ActionTimeline *centerAction[4];
    cocos2d::ui::TextAtlas *totalScoreText;
    cocos2d::ui::TextAtlas *cannonMultiText;
    cocos2d::ui::TextAtlas *stageScoreText;
};

#endif /* MaryLayer_h */
