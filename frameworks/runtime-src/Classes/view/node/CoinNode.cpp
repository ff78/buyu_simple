//
//  CoinNode.cpp
//  buyu_simple
//
//  Created by ff on 16/2/1.
//
//

#include "CoinNode.hpp"
#include "GameDef.h"

USING_NS_CC;
using namespace cocostudio;
using namespace timeline;

bool CoinNode::init()
{
    if (!Node::init()) {
        return false;
    }
    animationNode = CSLoader::createNode(COIN1_ANIM);
    animationNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    animationNode->setPosition(Vec2::ZERO);
    addChild(animationNode);
    
    state = WAVE;
    editorAction = CSLoader::createTimeline(COIN1_ANIM);
    editorAction->setLastFrameCallFunc(CC_CALLBACK_0(CoinNode::waveEnd, this));
    editorAction->play("wave", true);
    animationNode->runAction(editorAction);
    
    return true;
}

void CoinNode::waveEnd()
{
    state = STATIC;
    editorAction->setLastFrameCallFunc(nullptr);
    editorAction->play("static", true);
    
}

void CoinNode::move2Target(Vec2 target)
{
    state = MOVE;
    auto move = MoveTo::create(0.3, target);
    auto fade = FadeOut::create(1.0);
    
    auto spawn = Spawn::create(move, fade, NULL);
    auto func = CallFunc::create(CC_CALLBACK_0(CoinNode::moveEnd, this));
    auto seq = Sequence::create(spawn, func, NULL);
    
    runAction(seq);
}

void CoinNode::moveEnd()
{
    state = DEAD;
    setVisible(false);
}
