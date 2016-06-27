//
//  FishCoinNode.hpp
//  buyu_simple
//
//  Created by ff on 16/1/29.
//
//

#ifndef FishCoinNode_hpp
#define FishCoinNode_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CoinNode.hpp"
#include "ui/CocosGUI.h"

enum CoinEffState
{
    SHOWBOMB = 0,
    SHOWTURNTABLE = 1,
    SHOWCOIN = 2,
    SHOWCIRCLE = 3,
    SHOWRICH = 4,
    SHOWBLAST = 5
};

class FishCoinNode : public cocos2d::Node
{
public:
    bool init();
    
    CREATE_FUNC(FishCoinNode);
    
    void setupView();
    
    void createCoins();
    
    void createCoin(int index, int row, int rowCount);
    
    void turnEnd();
    
    void bombEnd();
    
    void fullScreenEnd();
    
    void createTurnTabel();
    
    void createCircle();
    
    void createRich();
    
    void createBlast();
    
    void removeOff();
    
    void update(float dt);
    
    void changeState(CoinEffState nextState);
    
    void enterState(CoinEffState nextState);
    
    void exitState(CoinEffState currState);
    
    void onCoinFrameEvent(cocostudio::timeline::Frame *frame);
private:
    cocos2d::Node* coinNode;
    cocos2d::Node* scoreLabelNode;
    cocos2d::Node* turnTableNode;
    
    cocos2d::Node* bombEffect;
    
    cocos2d::Node* fullScreenEffect;
    cocos2d::ui::TextAtlas* showScoreLable;
    int showScore;
    int scoreDecimalNumber;
    bool startChangeScore;
    
    int coinCount;
    int coinCount2;
    cocos2d::Vec2 firstPos;
    
    cocos2d::Vector<CoinNode*> coinVec;
    
    CC_SYNTHESIZE(int, score, Score);
    CC_SYNTHESIZE(cocos2d::Vec2, targerPos, TargetPos);
    CC_SYNTHESIZE(int, bombType, BombType);
    CC_SYNTHESIZE(int, baseScore, BaseScore);
    CC_SYNTHESIZE_READONLY(CoinEffState, state, State);
    CC_SYNTHESIZE(int, fishType, FishType);
    CC_SYNTHESIZE(cocos2d::Vec2, fishPos, FishPos);
    

};

#endif /* FishCoinNode_hpp */
