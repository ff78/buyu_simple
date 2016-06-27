//
//  CoinNode.hpp
//  buyu_simple
//
//  Created by ff on 16/2/1.
//
//

#ifndef CoinNode_hpp
#define CoinNode_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

enum CoinState
{
    WAVE = 0,
    STATIC = 1,
    MOVE = 2,
    DEAD = 3
};

class CoinNode : public cocos2d::Node
{
public:
    bool init();
    
    CREATE_FUNC(CoinNode);
    
    void waveEnd();
    void moveEnd();
    
    void move2Target(cocos2d::Vec2 target);
public:
    cocos2d::Node* animationNode;
    cocostudio::timeline::ActionTimeline* editorAction;
    CC_SYNTHESIZE(cocos2d::Vec2, targetPos, TargetPos);
    CC_SYNTHESIZE(CoinState, state, State);
    
    
    
};

#endif /* CoinNode_hpp */
