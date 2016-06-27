//
//  FishCoinLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/2/2.
//
//

#include "FishCoinLayer.hpp"

USING_NS_CC;

FishCoinLayer::~FishCoinLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(addCoinListener);
}

bool FishCoinLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    addCoinListener = EventListenerCustom::create(ADD_COIN, CC_CALLBACK_1(FishCoinLayer::addFishCoin, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addCoinListener, -1);
    
    return true;
}

void FishCoinLayer::addFishCoin(cocos2d::EventCustom *event)
{
    CMD_CREATE_COIN cmd = *static_cast<CMD_CREATE_COIN*>(event->getUserData());
    
    auto eff = FishCoinNode::create();
    eff->setTargetPos(cmd.targetPos);
    eff->setBombType(cmd.type);
    eff->setScore(cmd.score);
    eff->setBaseScore(cmd.baseScore);
    eff->setPosition(cmd.fishPos);
    eff->setFishPos(cmd.fishPos);
    eff->setFishType(cmd.fishEffectType);
    eff->setupView();
    addChild(eff);
}

void FishCoinLayer::update(float dt)
{
    auto coinVec = getChildren();
    for(auto coin : coinVec)
    {
        ((FishCoinNode*)coin)->update(dt);
    }
}
