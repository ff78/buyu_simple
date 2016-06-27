//
//  FishCoinNode.cpp
//  buyu_simple
//
//  Created by ff on 16/1/29.
//
//

#include "FishCoinNode.hpp"
#include "utils/GameUtils.h"
#include "GameDef.h"
#include "sound/SoundManager.h"

#define COIN_D 40

USING_NS_CC;
using namespace cocostudio;
using namespace timeline;

bool FishCoinNode::init()
{
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

void FishCoinNode::setupView()
{
    if (bombType != -1 && score != 0) {
        std::string resStr("animation/effect0");
        resStr += Convert2String(bombType);
        resStr += "/effect";
        resStr += Convert2String(bombType);
        resStr += ".csb";
        bombEffect = CSLoader::createNode(resStr);
        addChild(bombEffect);
        
        auto bombAction = CSLoader::createTimeline(resStr);
        bombAction->setLastFrameCallFunc(CC_CALLBACK_0(FishCoinNode::bombEnd, this));
        bombAction->play("play", true);
        bombEffect->runAction(bombAction);
        bombType = -1;
        AUDIO_PLAY("blast", AUDIO_TYPE::EFFECT_TYPE);
//        if (score > 0) {
//            if (baseScore < 50) {
//                changeState(SHOWBOMB);
//            }else{
//                changeState(SHOWTURNTABLE);
//            }
//        }else{
//            changeState(SHOWCOIN);
//        }
        
        
//    }
        
//    } else {
//        if (score > 0) {
//            if (baseScore < 50) {
//                changeState(SHOWCOIN);
//            }else{
//                changeState(SHOWTURNTABLE);
//            }
//        }else{
//            changeState(SHOWCOIN);
//        }
        
    }
    switch (fishType) {
        case SMALL_FISH:
        {
            changeState(SHOWCOIN);
        }
            break;
        case TINY_BOSS_FISH:
        {
            changeState(SHOWTURNTABLE);
        }
            break;
        case SPECIAL_FISH:
        {
            changeState(SHOWCIRCLE);
        }
            break;
        case BOSS_FISH:
        {
            changeState(SHOWRICH);
        }
            break;
        case SPECIAL_BOSS_FISH:
        {
            changeState(SHOWBLAST);
        }
            break;
            
        default:
            break;
    }
}

void FishCoinNode::bombEnd()
{
    bombEffect->setVisible(false);
    bombEffect->stopAllActions();
    if (state == SHOWBOMB) {
        changeState(SHOWCOIN);
    }
    
}

void FishCoinNode::createCoins()
{
    
    coinCount = MIN(85, baseScore) / 5 + 1;
    coinCount2 = 0;
    if (coinCount > 5) {
        coinCount /= 2;
        
        coinCount2 = coinCount/2;
    }
    
//    float coin1stX = getPositionX()-(coinCount-1)%2*COIN_D/2-(coinCount-1)/2*COIN_D;
//    float coin1stY = getPositionY()-((coinCount2==0)?0:20);
    float coin1stX = -(coinCount-1)%2*COIN_D/2-(coinCount-1)/2*COIN_D;
    float coin1stY = ((coinCount2==0)?0:20);
    firstPos = Vec2(coin1stX, coin1stY);
    
    coinVec.clear();
    
    createCoin(0,0,(coinCount2==0)?1:2);
    Vector<FiniteTimeAction*> actionVec;
//    auto func = CallFunc::create(std::bind(&FishCoinNode::createCoin, this, 0, 0, (coinCount2==0)?1:2));
//    actionVec.pushBack(func);

    
    for (int i = 1; i < coinCount; i++) {
        auto delay = DelayTime::create(0.1f);
        auto func1 = CallFunc::create(std::bind(&FishCoinNode::createCoin, this, i, 0, (coinCount2==0)?1:2));
        actionVec.pushBack(delay);
        actionVec.pushBack(func1);
    }
    
    for (int i = 0; i < coinCount2; i++) {
        auto delay = DelayTime::create(0.1f);
        auto func1 = CallFunc::create(std::bind(&FishCoinNode::createCoin, this, i, 1, 2));
        actionVec.pushBack(delay);
        actionVec.pushBack(func1);
    }
    if (!actionVec.empty()) {
        auto seq = Sequence::create(actionVec);
        runAction(seq);
    }
    
}

void FishCoinNode::createCoin(int index, int row, int rowCount)
{
    auto coin = CoinNode::create();
    float x = firstPos.x + index*COIN_D;
    float y = firstPos.y - row*40;
    coin->setPosition(Vec2(x, y));
    coinVec.pushBack(coin);
    addChild(coin);
}

void FishCoinNode::removeOff()
{
    removeFromParent();
}


void FishCoinNode::update(float dt)
{
    switch (state) {
        case SHOWCOIN:
        {
            int staticCount = 0;
            for (auto itr : coinVec) {
//                itr->update(dt);
                if (itr->getState() == STATIC) {
                    staticCount++;
                }
            }
            
            if (staticCount == coinVec.size()) {
                for (auto itr : coinVec) {
                    itr->move2Target(targerPos-getPosition());
                }
            }
            
            for (auto itr = coinVec.begin(); itr != coinVec.end();) {
                if ((*itr)->getState() == DEAD) {
                    itr = coinVec.erase(itr);
                }else{
                    itr++;
                }
            }
            
//            if (coinVec.empty() && coinCount == 0 && coinCount2 == 0) {
            if (coinVec.empty()) {
               removeOff();
            }
        }
            break;
            
        case SHOWRICH:
        case SHOWBLAST:
        {
            if (startChangeScore) {
                if (showScore < score) {
                    int temp = 1;
                    for (int i = 1; i <= scoreDecimalNumber; i++) {
                        
                        if (showScore/temp%10 < score/temp%10) {
                            showScore += temp;
                        }
                        
                        
                        temp *= 10;
                    }
                    //                stageScore++;
                    showScoreLable->setString(Convert2String(showScore));
                }else{
                    startChangeScore = false;
                }
            }
            
        }
            break;
            
        default:
            break;
    }
}

void FishCoinNode::changeState(CoinEffState nextState)
{
    if(state == nextState)
        return;
    exitState(state);
    state		= nextState;
    enterState(nextState);
}

void FishCoinNode::enterState(CoinEffState nextState)
{
    switch (nextState) {
        case SHOWBOMB:
        {
            
        }
            break;
            
        case SHOWCOIN:
        {
            createCoins();
            if (score > 0) {
                scoreLabelNode = CSLoader::createNode(SCORE_LABEL_ANIM);
                auto label = (ui::TextAtlas*)scoreLabelNode->getChildByName("AtlasLabel_1");
                label->setString(Convert2String(score));
                
                auto timeAction = CSLoader::createTimeline(SCORE_LABEL_ANIM);
                scoreLabelNode->runAction(timeAction);
                timeAction->play("play", false);
                
                addChild(scoreLabelNode);
                AUDIO_PLAY("get_gold", AUDIO_TYPE::EFFECT_TYPE);
            }
//            scoreLabelNode = CSLoader::createNode(SCORE_LABEL_ANIM);
//            auto label = (ui::TextAtlas*)scoreLabelNode->getChildByName("AtlasLabel_1");
//            label->setString(Conver2String(score));
//            
//            auto timeAction = CSLoader::createTimeline(SCORE_LABEL_ANIM);
//            scoreLabelNode->runAction(timeAction);
//            timeAction->play("play", false);
//            
//            addChild(scoreLabelNode);
        }
            break;
            
        case SHOWTURNTABLE:
        {
            createTurnTabel();
            setPosition(fishPos);
            AUDIO_PLAY("award_turn", AUDIO_TYPE::EFFECT_TYPE);
        }
            break;
        case SHOWCIRCLE:
        {
            createCircle();
            AUDIO_PLAY("award_turn", AUDIO_TYPE::EFFECT_TYPE);
        }
            break;
        case SHOWRICH:
        {
            startChangeScore = false;
            createRich();
        }
            break;
        case SHOWBLAST:
        {
            startChangeScore = false;
            createBlast();
        }
            break;
            
        default:
            break;
    }
}

void FishCoinNode::exitState(CoinEffState currState)
{
    switch (currState) {
        case SHOWRICH:
        case SHOWBLAST:
        {
//            fullScreenEffect->setVisible(false);
        }
            break;
            
        default:
            break;
    }
}

void FishCoinNode::createTurnTabel()
{
    turnTableNode = CSLoader::createNode(TURNTABLE_ANIM);
    auto label = (ui::TextAtlas*)turnTableNode->getChildByName("Sprite_1")->getChildByName("AtlasLabel_1");
    label->setString(Convert2String(score));
    addChild(turnTableNode);
    auto turnTimeline = CSLoader::createTimeline(TURNTABLE_ANIM);
    turnTimeline->setLastFrameCallFunc(CC_CALLBACK_0(FishCoinNode::turnEnd, this));
    turnTimeline->play("play", false);
    turnTableNode->runAction(turnTimeline);
}

void FishCoinNode::createCircle()
{
    turnTableNode = CSLoader::createNode(TURN_CIRCLE_ANIM);
    auto label = (ui::TextAtlas*)turnTableNode->getChildByName("count_label");
    label->setString(Convert2String(score));
    addChild(turnTableNode);
    auto turnTimeline = CSLoader::createTimeline(TURN_CIRCLE_ANIM);
    turnTimeline->setLastFrameCallFunc(CC_CALLBACK_0(FishCoinNode::turnEnd, this));
    turnTimeline->play("play", false);
    turnTableNode->runAction(turnTimeline);
}

void FishCoinNode::createRich()
{
    showScore = 0;
    scoreDecimalNumber = 1;
    int temp = score/10;
    while (temp > 0) {
        temp /= 10;
        scoreDecimalNumber++;
    }
    
    fullScreenEffect = CSLoader::createNode(RICH_TIP_ANIM);
    showScoreLable = (ui::TextAtlas*)fullScreenEffect->getChildByName("ge_rich0_1")->getChildByName("AtlasLabel_1");
    showScoreLable->setString(Convert2String(showScore));
//    fullScreenEffect->
    setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(fullScreenEffect);
    setLocalZOrder(1000);
    
    auto turnTimeline = CSLoader::createTimeline(RICH_TIP_ANIM);
    turnTimeline->setFrameEventCallFunc(CC_CALLBACK_1(FishCoinNode::onCoinFrameEvent, this));
    turnTimeline->setLastFrameCallFunc(CC_CALLBACK_0(FishCoinNode::fullScreenEnd, this));
    turnTimeline->play("play", false);
    fullScreenEffect->runAction(turnTimeline);
}

void FishCoinNode::createBlast()
{
    showScore = 0;
    scoreDecimalNumber = 1;
    int temp = score/10;
    while (temp > 0) {
        temp /= 10;
        scoreDecimalNumber++;
    }
    
    fullScreenEffect = CSLoader::createNode(BLAST_TIP_EFFECT);
    showScoreLable = (ui::TextAtlas*)fullScreenEffect->getChildByName("ge_blasting_star_2")->getChildByName("AtlasLabel_1");
    showScoreLable->setString(Convert2String(showScore));
    auto bgSpt = (Sprite*)fullScreenEffect->getChildByName("ge_blasting_bg_1");
    bgSpt->setScaleX(GameUtils::winSize.width/960);
//    fullScreenEffect->
    setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(fullScreenEffect);
    setLocalZOrder(1000);
    
    auto turnTimeline = CSLoader::createTimeline(BLAST_TIP_EFFECT);
    turnTimeline->setFrameEventCallFunc(CC_CALLBACK_1(FishCoinNode::onCoinFrameEvent, this));
    turnTimeline->setLastFrameCallFunc(CC_CALLBACK_0(FishCoinNode::fullScreenEnd, this));
    turnTimeline->play("play", false);
    fullScreenEffect->runAction(turnTimeline);
}
void FishCoinNode::fullScreenEnd()
{
    fullScreenEffect->setVisible(false);
    changeState(SHOWTURNTABLE);
}

void FishCoinNode::turnEnd()
{
    turnTableNode->setVisible(false);
    baseScore = 85;
    changeState(SHOWCOIN);
}

void FishCoinNode::onCoinFrameEvent(cocostudio::timeline::Frame *frame)
{
    EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
    if(!evnt)
        return;
    
    std::string str = evnt->getEvent();
    
    if (str == "change_count")
    {
        startChangeScore = true;
    }
}
