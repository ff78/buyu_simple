//
//  GameLottery.cpp
//  buyu_simple
//
//  Created by ff on 16/4/29.
//
//

#include "GameLottery.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "GameDef.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

GameLottery::GameLottery()
{
    
}

GameLottery::~GameLottery()
{
    unscheduleUpdate();
}

bool GameLottery::init()
{
    canClickCard = false;
    if (!LayerColor::initWithColor(Color4B(10,10,10,128)))
    {
        return false;
    }
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    root = CSLoader::createNode(LOTTERY_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    startBtn = (Button*)root->getChildByName("lottery_button");
    startBtn->addClickEventListener(CC_CALLBACK_1(GameLottery::clickStartBtn, this));
    
    endBtn = (Button*)root->getChildByName("exit_button");
    countdownLabel = (TextAtlas*)root->getChildByName("countdown_label");
    countdownDuration = 0;
    
    
    for (int i = 0; i < 6; i++) {
        std::string cardName("card");
        cardName += Convert2String(i+1);
        
        card[i] = (Sprite*)root->getChildByName(cardName);
    }
    
    uiAction = CSLoader::createTimeline(LOTTERY_UI);
    root->runAction(uiAction);
    
    
    return true;
}

void GameLottery::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_LOTTERY info = *static_cast<L2E_SHOW_LOTTERY*>(event->getUserData());
    memcpy(originBoundtypes, info.boundtypes, sizeof(int)*6);
    memcpy(originBoundIds, info.boundIds, sizeof(int)*6);
    memcpy(originBoundCount, info.boundCount, sizeof(int)*6);
    memcpy(resultHand, info.result, sizeof(int)*6);
    
    for (int i = 0; i < 6; i++) {
        auto boundSpt = (Sprite*)card[i]->getChildByName("bound_sprite");
        if (info.boundtypes[i] == 1) {
            boundSpt->initWithFile("game_scene/gamer_ui/lucky_draw/ld_gold.png");
        }else if (info.boundtypes[i] == 2) {
            boundSpt->initWithFile("game_scene/gamer_ui/lucky_draw/ld_diamond.png");
        }else{
            std::string temp("common/element/element_");
            temp += Convert2String(info.boundIds[i]+1);
            temp += ".png";
            
            boundSpt->initWithFile(temp);
        }
        
        auto countLabel = (TextAtlas*)card[i]->getChildByName("count_label");
        countLabel->setString(Convert2String(info.boundCount[i]));
    }
    
    cardframeSpt = nullptr;
    cardLightSpt = nullptr;
    uiAction->play("showCards", false);
    countdownDuration = 12;
    countdownLabel->setVisible(true);
    changeGameState(SHOW_CARD);
    scheduleUpdate();
}

void GameLottery::clickStartBtn(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    startShuffel();
}

void GameLottery::startShuffel()
{
    countdownDuration = 0;
    countdownLabel->setVisible(false);
    changeGameState(SHUFFLE_CARD);
    uiAction->play("shuffle", false);
    uiAction->setLastFrameCallFunc(CC_CALLBACK_0(GameLottery::endShuffel, this));
}

bool GameLottery::clickCardBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    if(!canClickCard)
    {
        return false;
    }
    Vec2 locationInNode = this->convertToWorldSpace(touch->getLocation());
    for (int i = 0; i < 6; i++) {
        auto origin = this->convertToWorldSpace(card[i]->getPosition());
        origin += Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2);
        Size s = card[i]->getContentSize();
        Rect rect = Rect(origin.x-s.width/2, origin.y-s.height/2, s.width, s.height);
        if (rect.containsPoint(locationInNode)){
            log("click card %d began", card[i]->getTag());
//            E2L_CHOOSE_LOTTERY_CARD info;
//            info.eProtocol = e2l_cannon_unlock_level;
//            ClientLogic::instance()->ProcessUIRequest(&info);
            
            openCard(i);
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            return true;
        }
    }
    
    return false;
}

void GameLottery::openCard(int index)
{
    selFocus = index;
    for (int j = 0, k = 1; j < 6; j++, k++) {
        auto boundSpt = (Sprite*)card[j]->getChildByName("bound_sprite");
        auto countLabel = (TextAtlas*)card[j]->getChildByName("count_label");
        int boundType = originBoundtypes[resultHand[k]];
        int boundId = originBoundIds[resultHand[k]];
        int boundCount = originBoundCount[resultHand[k]];
        if (index == j) {
            boundType = originBoundtypes[resultHand[0]];
            boundId = originBoundIds[resultHand[0]];
            boundCount = originBoundCount[resultHand[0]];
            
            k--;
        }
        
        if (boundType == 1) {
            boundSpt->initWithFile("game_scene/gamer_ui/lucky_draw/ld_gold.png");
        }else if (boundType == 2) {
            boundSpt->initWithFile("game_scene/gamer_ui/lucky_draw/ld_diamond.png");
        }else{
            std::string temp("common/element/element_");
            temp += Convert2String(boundId+1);
            temp += ".png";
            
            boundSpt->initWithFile(temp);
        }
        
        countLabel->setString(Convert2String(boundCount));
    }
    canClickCard = false;
    
    countdownDuration = 0;
    countdownLabel->setVisible(false);
//    cardAction = CSLoader::createTimeline("LOTTERY_UI");
//    card[selFocus]->runAction(cardAction);
//    cardAction->play("showhand", false);
//    nextHand = 1;
//    cardAction->setLastFrameCallFunc(std::bind(&GameLottery::showNextHand, this, nextHand));
    
    uiAction->play("showhand", false);
    nextHand = 1;
    uiAction->setLastFrameCallFunc(std::bind(&GameLottery::showNextHand, this, nextHand));
}

void GameLottery::clickCardEnd(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void GameLottery::endShuffel()
{
    for (int i = 0; i < 6; i++) {
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(GameLottery::clickCardBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(GameLottery::clickCardEnd, this);
        card[i]->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, card[i]);
    }
    canClickCard = true;
    countdownDuration = 12;
    countdownLabel->setVisible(true);
//    uiAction->setLastFrameCallFunc(nullptr);
//    uiAction->play("showhand", false);
//    nextHand = 1;
//    uiAction->setLastFrameCallFunc(std::bind(&GameLottery::showNextHand, this, nextHand));
}

void GameLottery::showNextHand(int index)
{
//    std::string animationName("showhand");
//    animationName += Convert2String(index);
//    index++;
//    if (index == 3) {
//        index++;
//    }
//    
//    if (index > 6) {
//        uiAction->setLastFrameCallFunc(nullptr);
//        uiAction->play("showEndBtn", false);
//        return;
//    }
    
//    uiAction->play(animationName, false);
//    uiAction->setLastFrameCallFunc(std::bind(&GameLottery::showNextHand, this, index));
    endBtn->addClickEventListener(CC_CALLBACK_1(GameLottery::clickEndBtn, this));
    countdownDuration = 12;
    countdownLabel->setVisible(true);
    cardframeSpt = Sprite::create("game_scene/gamer_ui/lucky_draw/ld_card_frame.png");
    cardLightSpt = Sprite::create("game_scene/gamer_ui/lucky_draw/ld_card_light.png");
    cardframeSpt->setPosition(Vec2(card[selFocus]->getContentSize().width/2, card[0]->getContentSize().height/2));
    cardLightSpt->setPosition(Vec2(card[selFocus]->getContentSize().width/2, card[0]->getContentSize().height/2));
    card[selFocus]->addChild(cardframeSpt);
    card[selFocus]->addChild(cardLightSpt);
    
    changeGameState(SHOWHAND);
}

void GameLottery::clickEndBtn(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    endLottery();
}

void GameLottery::endLottery()
{
    endBtn->setVisible(false);
    countdownDuration = 0;
    countdownLabel->setVisible(false);
    
    initWithColor(Color4B(0, 0, 0, 0));
    for (int i = 0; i < 6; i++) {
        card[i]->setVisible(false);
    }
    card[selFocus]->setVisible(true);
    
    auto scaleAction = ScaleTo::create(0.3, 0.15);
    auto move = MoveTo::create(0.5, Vec2(0, 40-GameUtils::winSize.height/2));
    auto spawn = Spawn::create(scaleAction, move, NULL);
    auto func = CallFunc::create(CC_CALLBACK_0(GameLottery::exitLottery, this));
    auto seq = Sequence::create(spawn, func, NULL);
    card[selFocus]->runAction(seq);
    
    changeGameState(RECEIVE_CHIPS);
}

void GameLottery::exitLottery()
{
    E2L_END_LOTTERY info;
    info.eProtocol = e2l_end_lottery;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameLottery::update(float dt)
{
    if (countdownDuration > 0) {
        countdownDuration -= dt;
        countdownLabel->setString(Convert2String((int)countdownDuration));
        if (countdownDuration <= 0) {
            countdownDuration = 0;
            switch (currState) {
                case SHOW_CARD:
                {
                    startShuffel();
                }
                    break;
                case SHUFFLE_CARD:
                {
                    openCard(0);
                }
                    break;
                case SHOWHAND:
                {
                    endLottery();
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

void GameLottery::changeGameState(Lottery_State nextState)
{
    currState = nextState;
}
