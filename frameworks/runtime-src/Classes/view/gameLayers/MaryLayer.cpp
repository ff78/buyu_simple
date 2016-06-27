//
//  MaryLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/3/23.
//
//

#include "MaryLayer.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "EventDef.h"
#include "../node/GoldFallen.h"
#include "sound/SoundManager.h"
#include "GameDef.h"

USING_NS_CC;
using namespace ui;

MaryLayer::MaryLayer()
{
    initDataListener = EventListenerCustom::create(UI_MARY_INIT, CC_CALLBACK_1(MaryLayer::initData, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(initDataListener, -1);
    gameState = lastState = -1;
}

MaryLayer::~MaryLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(initDataListener);
}

bool MaryLayer::init()
{
//    if (!LayerColor::initWithColor(Color4B(10,10,10,128))) {
    if (!LayerColor::initWithColor(Color4B::BLUE)) {
        return false;
    }
    
//    initDataListener = EventListenerCustom::create(UI_MARY_INIT, CC_CALLBACK_1(MaryLayer::initData, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(initDataListener, -1);
//    gameState = lastState = -1;
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto bgSpt = Sprite::create("game_scene/gamer_ui/mary/mary_bg0.png");
    bgSpt->setPosition(GameUtils::winSize.width/2, GameUtils::winSize.height/2);
    bgSpt->setScaleX(GameUtils::winSize.width/960);
    addChild(bgSpt);
    
    animationNode = CSLoader::createNode(MARY_UI);
    animationNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    animationNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    dialogAction = CSLoader::createTimeline(MARY_UI);
    animationNode->runAction(dialogAction);
    addChild(animationNode);
    
    for (int i = 0; i < 4; i++) {
        centerRollNode[i] = animationNode->getChildByName("mary_bg_1")->getChildByName("FileNode"+Convert2String(i+1));
        centerAction[i] = CSLoader::createTimeline(MARY_SCROLL_UI);
        centerRollNode[i]->runAction(centerAction[i]);
    }
    
    cannonMultiText = (TextAtlas *)animationNode->getChildByName("mary_bg_1")->getChildByName("CannonLabel");
    stageScoreText = (TextAtlas *)animationNode->getChildByName("mary_bg_1")->getChildByName("SumScoreLabel");
    stageScoreText->setString(Convert2String(0));
    totalScoreText = (TextAtlas *)animationNode->getChildByName("mary_bg_1")->getChildByName("TotalScoreLabel");
    
    focus = 0;
    for (int i = 0; i < 4; i++) {
        centerTypeId[i] = 1;
    }

    stageDiamond = 0;
    stageScore = 0;
    scoreDecimalNumber = 0;
    scheduleUpdate();
    return true;
}

void MaryLayer::enterGameState(int nextState)
{
    switch (nextState) {
       case MARY_ROLL_INIT:
        {
            E2L_MARY_INIT info;
            info.eProtocol = e2l_mary_init;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
        case MARY_ROLL_START:
        {
            auto panel = animationNode->getChildByName("mary_bg_1");
            std::string tempStr("cardgreen");
            for (int i = 1; i <= 3; i++) {
                auto cardGreen = panel->getChildByName(tempStr + Convert2String(i));
                cardGreen->setVisible(false);
            }
            
            tempStr = "mary_multiple";
            for (int i = 1; i <= 10; i++)
            {
                auto multiple = panel->getChildByName(tempStr + Convert2String(i));
                multiple->setVisible(true);
                auto redMultiple = panel->getChildByName(tempStr + "_red_" + Convert2String(i));
                redMultiple->setVisible(false);
            }
            
            tempStr = "mary_light";
            for (int i = 1; i <= 4; i++) {
                auto light = panel->getChildByName(tempStr + Convert2String(i));
                light->setVisible(false);
            }
            
            tempStr = "Button";
            for (int i = 0; i < 24; i++) {
                auto btn = (Button*)animationNode->getChildByName(tempStr + Convert2String(i));
                btn->setBright(false);
            }
            
            auto btn = (Button*)animationNode->getChildByName(tempStr + Convert2String(focus));
            btn->setBright(true);
            
            
            lampInterval = 0.4;
            lampDuration = 0.4;
            lampStep = 0;
            rollStep = abs(rand())%3+3;
            
            
        }
            break;
            
        case MARY_ROLL_RUN:
        {
            auto panel = animationNode->getChildByName("mary_bg_1");
            for (int i = 0; i < 4; i++) {
                //                centerRollNode[i]->stopAllActions();
                stopActionByTag(10000+i);
                centerRollNode[i]->setVisible(true);
                centerAction[i]->play("roll", true);
                auto delay = DelayTime::create(1.5+i*1.2);
                auto func = CallFunc::create(std::bind(&MaryLayer::showhand, this, i));
                auto seq = Sequence::create(delay, func, NULL);
                seq->setTag(10000+i);
                runAction(seq);
                
                auto rollSpt = (Sprite*)panel->getChildByName("mary_fish"+Convert2String(i+1));
                rollSpt->initWithFile("game_scene/gamer_ui/mary/mary_fish"+Convert2String(targetCenterTypeId[i])+".png");
                rollSpt->setVisible(false);
            }

            
            lampInterval = 0.05;
            lampDuration = 0.05;
            lampStep = 0;
            int roundCount = abs(rand())%3+2;
            int dis = (targetFocus+24-focus)%24;
            slowStep = abs(rand())%6+3;
            rollStep = roundCount*24+dis-slowStep;
        }
            break;
        case MARY_ROLL_SLOW:
        {
            lampInterval = 0.2;
            lampDuration = 0.2;
            lampStep = 0;
        }
            break;
        case MARY_ROUND_RESULT:
        {
            auto panel = animationNode->getChildByName("mary_bg_1");
            for (int i = 0; i < 4; i++) {
                lightOpen[i] = (targetTypeId == targetCenterTypeId[i]);
                lightSpt[i] = (Sprite*)panel->getChildByName("mary_light"+Convert2String(i+1));
                lightSpt[i]->setVisible(targetTypeId == targetCenterTypeId[i]);
            }
            blinkDuration = 0.5;
            
            if (targetTypeId != 0) {
                auto multiLabel = panel->getChildByName("mary_multiple_red_"+Convert2String(targetTypeId));
                multiLabel->setVisible(true);
            }
            
            if (cardType != 0) {
                auto green = panel->getChildByName("cardgreen"+Convert2String(cardType));
                green->setVisible(true);
            }
            auto focusBtn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
            focusSpt = Sprite::create("game_scene/gamer_ui/mary/mary_selected.png");
            if (targetTypeId == 1) {
                focusSpt->initWithFile("game_scene/gamer_ui/mary/mary_selected2.png");
            }
            focusSpt->setPosition(Vec2(focusBtn->getContentSize().width/2, focusBtn->getContentSize().height/2));
            focusBtn->addChild(focusSpt);
            
            resultDuration = 3;
            stageScore = targetStageSocre;
            stageScoreText->setString(Convert2String(stageScore));
            totalScoreText->setString(Convert2String(totalScore + targetStageSocre));
        }
            break;
        case MARY_EXIT_RESULT:
        {
            auto focusBtn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
            focusSpt = Sprite::create("game_scene/gamer_ui/mary/mary_selected.png");
            if (targetTypeId == 1) {
                focusSpt->initWithFile("game_scene/gamer_ui/mary/mary_selected2.png");
            }
            focusSpt->setPosition(Vec2(focusBtn->getContentSize().width/2, focusBtn->getContentSize().height/2));
            focusBtn->addChild(focusSpt);
            
            dialogAction->play("win", false);
            dialogAction->setLastFrameCallFunc(CC_CALLBACK_0(MaryLayer::exitMary, this));
            
            auto winLabel = (TextAtlas*)animationNode->getChildByName("WinLabel");
            winLabel->setString(Convert2String(stageScore));
            auto winLabel2 = (TextAtlas*)animationNode->getChildByName("WinLabel_0");
            winLabel2->setString(Convert2String(stageDiamond));
            if (stageScore > 0) {
                GOLD_FALL_ANIMATION();
            }
        }
            break;
        default:
            break;
    }
}

void MaryLayer::exitGameState(int preState)
{
    switch (preState) {
        case MARY_ROUND_RESULT:
        {
            auto focusBtn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
            focusBtn->removeAllChildren();
        }
            break;
            
        default:
            break;
    }
}

void MaryLayer::changeGameState(int nextState)
{
    if(gameState == nextState)
        return;
    exitGameState(lastState);
    lastState	= gameState;
    gameState		= nextState;
    enterGameState(nextState);
}

void MaryLayer::initData(cocos2d::EventCustom *event)
{
    L2E_MARY_INIT info = *static_cast<L2E_MARY_INIT *>(event->getUserData());
    targetFocus = info.roundIndex;
    targetTypeId = info.roundTypeId;
    cardType = info.cardType;
    targetStageSocre = info.stageScore;
    stageDiamond = info.stageDiamond;
    scoreDecimalNumber = 1;
    int temp = targetStageSocre/10;
    while (temp > 0) {
        temp /= 10;
        scoreDecimalNumber++;
    }
//    totalScore = info.totalScore;
//    totalScoreText->setString(Convert2String(info.totalScore));
    
    memcpy(targetCenterTypeId, info.centerIndex, sizeof(int)*4);
    changeGameState(MARY_ROLL_START);
}

void MaryLayer::beginMary(cocos2d::EventCustom *event)
{
    L2E_ENTER_MARY info = *static_cast<L2E_ENTER_MARY*>(event->getUserData());
    targetFocus = info.roundIndex;
    targetTypeId = info.roundTypeId;
    cardType = info.cardType;
    targetStageSocre = info.stageScore;
    stageDiamond = info.stageDiamond;
    scoreDecimalNumber = 1;
    int temp = targetStageSocre/10;
    while (temp > 0) {
        temp /= 10;
        scoreDecimalNumber++;
    }
    totalScore = info.totalScore;
    cannonMultiText->setString(Convert2String(info.cannonMulti));
    totalScoreText->setString(Convert2String(info.totalScore));

    memcpy(targetCenterTypeId, info.centerIndex, sizeof(int)*4);
    changeGameState(MARY_ROLL_START);
}

void MaryLayer::update(float dt)
{
    switch (gameState) {
        case MARY_ROLL_START:
        {
            if (lampDuration <= 0) {
                if (lampStep >= rollStep) {
                    changeGameState(MARY_ROLL_RUN);
                    return;
                }
                
                auto btn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
                btn->setBright(false);
                focus++;
                focus %= 24;
                auto nextBtn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
                nextBtn->setBright(true);
                
                lampInterval -= 0.2;
                lampInterval = MAX(0.15, lampInterval);
                lampDuration = lampInterval;
                lampStep++;

                AUDIO_PLAY("turn", AUDIO_TYPE::EFFECT_TYPE);
            }
            lampDuration -= dt;
        }
            break;
            
        case MARY_ROLL_RUN:
        {
            if (lampDuration <= 0) {
                if (lampStep >= rollStep) {
                    changeGameState(MARY_ROLL_SLOW);
                    return;
                }
                auto btn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
                btn->setBright(false);
                focus++;
                focus %= 24;
                auto nextBtn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
                nextBtn->setBright(true);
                
                lampDuration = lampInterval;
                lampStep++;
                AUDIO_PLAY("turn", AUDIO_TYPE::EFFECT_TYPE);
            }
            lampDuration -= dt;
        }
            break;
        case MARY_ROLL_SLOW:
        {
            if (lampDuration <= 0) {
                if (lampStep >= slowStep) {
                    if (targetTypeId != 0) {
                        changeGameState(MARY_ROUND_RESULT);
                    }else{
                        changeGameState(MARY_EXIT_RESULT);
                    }
                    
                    return;
                }
                auto btn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
                btn->setBright(false);
                focus++;
                focus %= 24;
                auto nextBtn = (Button*)animationNode->getChildByName("Button" + Convert2String(focus));
                nextBtn->setBright(true);
                
                lampInterval += 0.2;
                lampInterval = MIN(0.6, lampInterval);
                lampDuration = lampInterval;
                lampStep++;
                AUDIO_PLAY("turn", AUDIO_TYPE::EFFECT_TYPE);
                
            }
            lampDuration -= dt;
        }
            break;
            
        case MARY_ROUND_RESULT:
        {
//            if (stageScore < targetStageSocre) {
//                int temp = 1;
//                for (int i = 1; i <= scoreDecimalNumber; i++) {
//                    
//                    if (stageScore/temp%10 < targetStageSocre/temp%10) {
//                        stageScore += temp;
//                    }
//                    
//                    
//                    temp *= 10;
//                }
////                stageScore++;
//                stageScoreText->setString(Convert2String(stageScore));
//            }

            blinkDuration -= dt;
            if (blinkDuration <= 0) {
                blinkDuration = 0.5f;
                focusSpt->setVisible(!focusSpt->isVisible());
                for (int i = 0; i < 4; i++) {
                    if (lightOpen[i]) {
                        lightSpt[i]->setVisible(!lightSpt[i]->isVisible());
                    }
                }
               
            }

            if (resultDuration > 0) {
                resultDuration -= dt;
                if (resultDuration <= 0) {
                    changeGameState(MARY_ROLL_INIT);
                    return;
                }
            }
        }
            break;
        default:
            break;
    }
}

void MaryLayer::showhand(int index)
{
    centerAction[index]->stop();
    centerRollNode[index]->setVisible(false);
    
    auto panel = animationNode->getChildByName("mary_bg_1");
    auto rollSpt = (Sprite*)panel->getChildByName("mary_fish"+Convert2String(index+1));
    rollSpt->setVisible(true);
    
    centerTypeId[index] = targetCenterTypeId[index];
}

void MaryLayer::exitMary()
{
    stopAllActions();
    auto delay = DelayTime::create(3);
    auto func = CallFunc::create(CC_CALLBACK_0(MaryLayer::remove, this));
    auto seq = Sequence::create(delay, func, NULL);
    
    runAction(seq);
}

void MaryLayer::remove()
{
    E2L_MARY_EXIT info;
    info.eProtocol = e2l_mary_exit;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
//    removeFromParent();
}
