//
//  NiuNiuLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/3/18.
//
//

#include "NiuNiuLayer.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "EventDef.h"
#include "../node/GoldFallen.h"
#include "sound/SoundManager.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

NiuNiuLayer::NiuNiuLayer()
{
    clickCount = 0;
    cardResult = -1;
    clickDuration = -1;
    clickResult = 0;
}

NiuNiuLayer::~NiuNiuLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(showhandListener);
}

bool NiuNiuLayer::init()
{
    if(!LayerColor::initWithColor(Color4B(10,10,10,128))) {
        return false;
    }
    auto winSize = Director::getInstance()->getWinSize();
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    
    dialogNode = CSLoader::createNode(NIUNIU_UI);
    dialogNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    dialogNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(dialogNode);
    dialogAction = CSLoader::createTimeline(NIUNIU_UI);
    dialogAction->setFrameEventCallFunc(CC_CALLBACK_1(NiuNiuLayer::onFrameEvent, this));
    dialogNode->runAction(dialogAction);
    
    showhandListener = EventListenerCustom::create(UI_SHOW_HAND, CC_CALLBACK_1(NiuNiuLayer::showHand, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showhandListener, -1);
    scheduleUpdate();
    return true;
}

void NiuNiuLayer::setupView(EventCustom* event)
{
    L2E_ENTER_NIUNIU info = *static_cast<L2E_ENTER_NIUNIU*>(event->getUserData());
    multi = info.cannonMultiply;
    
    root = dynamic_cast<Sprite*>(dialogNode->getChildByName("nn_bg"));
    clockSpt = dynamic_cast<Sprite*>(root->getChildByName("clockSprite"));
    ClickTimer = dynamic_cast<TextAtlas*>(clockSpt->getChildByName("ClickTimer"));
    ClickTimer->setVisible(true);
    
    MultiLabel = dynamic_cast<TextAtlas*>(root->getChildByName("MultiLabel"));
    MultiLabel->setString(Convert2String(multi));
    
    startDealBtn = dynamic_cast<Button*>(root->getChildByName("StartBtn"));
    startDealBtn->addClickEventListener(CC_CALLBACK_1(NiuNiuLayer::clickStartDeal, this));
    
    clickBtn = dynamic_cast<Button*>(root->getChildByName("PressBtn"));
    clickBtn->addClickEventListener(CC_CALLBACK_1(NiuNiuLayer::clickCountBtn, this));
    PressCount = dynamic_cast<TextAtlas *>(root->getChildByName("PressCount"));
    
    MoneyResult = dynamic_cast<TextAtlas*>(root->getChildByName("AtlasLabel_15"));
    
//    testLabel = TextAtlas::create("123456789xjqk", "bmfont/cards_number.png", 26, 29, "1");
//    testLabel->setString("x");
//    testLabel->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
//    bg->addChild(testLabel);
    for (int i = 1; i <= 5; i++) {
        int rank = (info.cards[i-1]-1)%13;
        int suit = (info.cards[i-1]-1)/13;
        auto card = dynamic_cast<Sprite*>(root->getChildByTag(i));
        auto number1 = dynamic_cast<TextBMFont*>(card->getChildByName("Number1"));
        number1->setString(rankName[rank]);
        auto number2 = dynamic_cast<TextBMFont*>(card->getChildByName("Number2"));
        number2->setString(rankName[rank]);
        
//        auto suitSpt = dynamic_cast<Sprite*>(card->getChildByName("CardSprite"));
        std::string resultRes("game_scene/gamer_ui/niuniu/");
        resultRes += suitName[suit];
//        suitSpt->initWithFile(resultRes);
        cardSuitRes[i-1]= resultRes;
    }
}

void NiuNiuLayer::clickStartDeal(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    startDealBtn->setVisible(false);
    dialogAction->play("deal", false);
    dialogAction->setLastFrameCallFunc(CC_CALLBACK_0(NiuNiuLayer::endDeal, this));
}

void NiuNiuLayer::endDeal()
{
    dialogAction->play("waitDraw", true);
    dialogAction->setLastFrameCallFunc(nullptr);
    
    auto card = dynamic_cast<Sprite*>(root->getChildByTag(5));
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(NiuNiuLayer::onTouchCardBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(NiuNiuLayer::onTouchCard, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, card);
}

bool NiuNiuLayer::onTouchCardBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void NiuNiuLayer::onTouchCard(cocos2d::Touch *touch, cocos2d::Event *event)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    auto card = dynamic_cast<Sprite*>(root->getChildByTag(5));
    _eventDispatcher->removeEventListenersForTarget(card);
    E2L_NIUNIU_ASK_SHOWHAND info;
    info.eProtocol = e2l_niuniu_ask_showHand;
    ClientLogic::instance()->ProcessUIRequest(&info);
//    dialogAction->play("draw", true);
//    dialogAction->setLastFrameCallFunc(nullptr);
}

void NiuNiuLayer::showHand(cocos2d::EventCustom *event)
{
    L2E_NINIU_REQ_SHOWHAND info = *static_cast<L2E_NINIU_REQ_SHOWHAND*>(event->getUserData());
    int result = info.result;
    int cardType = info.cardType;
    cardResult = result;
    clickResult = 0;
    
    auto resultSpt = dynamic_cast<Sprite*>(root->getChildByName("Sprite_15"));
    std::string resultRes("game_scene/gamer_ui/niuniu/nn_result");
    resultRes += Convert2String(cardType);
    resultRes += ".png";
    resultSpt->initWithFile(resultRes);
    auto addTime = dynamic_cast<TextAtlas *>(root->getChildByName("AddTime"));
    addTime->setString(Convert2String(gNiuNiuSecond[result]-6));

    dialogAction->play("draw", false);
    dialogAction->setLastFrameCallFunc(std::bind(&NiuNiuLayer::endShowHand, this, result) );
    AUDIO_PLAY("card", AUDIO_TYPE::EFFECT_TYPE);
}

void NiuNiuLayer::endShowHand(int result)
{
    ClickTimer->setString(Convert2String(gNiuNiuSecond[result]));
    
    dialogAction->play("waitClick", true);
    dialogAction->setLastFrameCallFunc(nullptr);
    
    clickCount = 0;
    clickDuration = 0;
    auto delay = DelayTime::create(3);
    auto func = CallFunc::create(std::bind(&NiuNiuLayer::startClick, this));
    auto seq = Sequence::create(delay, func, NULL);
    seq->setTag(101);
    runAction(seq);
}

void NiuNiuLayer::startClick()
{
    PressCount->setVisible(true);
    PressCount->setString(Convert2String(clickCount));
    dialogAction->play("bang", false);
    clickDuration = gNiuNiuSecond[cardResult];
}

void NiuNiuLayer::clickCountBtn(cocos2d::Ref *pSender)
{
    if (clickDuration == -1) {
        return;
    }
    if (clickDuration == 0) {
        stopActionByTag(101);
        clickDuration = gNiuNiuSecond[cardResult];
    }
    clickCount++;
    PressCount->setString(Convert2String(clickCount));
    dialogAction->play("bang", false);
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
}

void NiuNiuLayer::onFrameEvent(cocostudio::timeline::Frame *frame)
{
    EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
    if(!evnt)
        return;
    
    std::string str = evnt->getEvent();
    
    if (str == "showSuit1")
    {
        auto card = dynamic_cast<Sprite*>(root->getChildByTag(1));
        
        auto suitSpt = dynamic_cast<Sprite*>(card->getChildByName("CardSprite"));
        suitSpt->initWithFile(cardSuitRes[0]);
    } else if (str == "showSuit2") {
        auto card = dynamic_cast<Sprite*>(root->getChildByTag(2));
        
        auto suitSpt = dynamic_cast<Sprite*>(card->getChildByName("CardSprite"));
        suitSpt->initWithFile(cardSuitRes[1]);
    } else if (str == "showSuit3") {
        auto card = dynamic_cast<Sprite*>(root->getChildByTag(3));
        
        auto suitSpt = dynamic_cast<Sprite*>(card->getChildByName("CardSprite"));
        suitSpt->initWithFile(cardSuitRes[2]);
    } else if (str == "showSuit4") {
        auto card = dynamic_cast<Sprite*>(root->getChildByTag(4));
        
        auto suitSpt = dynamic_cast<Sprite*>(card->getChildByName("CardSprite"));
        suitSpt->initWithFile(cardSuitRes[3]);
    } else if (str == "showSuit5") {
        auto card = dynamic_cast<Sprite*>(root->getChildByTag(5));
        
        auto suitSpt = dynamic_cast<Sprite*>(card->getChildByName("CardSprite"));
        suitSpt->initWithFile(cardSuitRes[4]);
    }
    AUDIO_PLAY("card", AUDIO_TYPE::EFFECT_TYPE);
}

void NiuNiuLayer::update(float dt)
{
    if (cardResult != -1 && clickDuration > 0) {
        clickDuration -= dt;
        if (clickDuration < 0) {
            clickDuration = -1;
            clickResult = clickCount * multi;
            MoneyResult->setString(Convert2String(clickResult));
            dialogAction->play("showResult", false);
            dialogAction->setLastFrameCallFunc(CC_CALLBACK_0(NiuNiuLayer::endShowResult, this));
        }else{
            ClickTimer->setString(Convert2String((int)clickDuration));
        }
        
        
    }
}

void NiuNiuLayer::endShowResult()
{
    if (clickResult > 0) {
        GOLD_FALL_ANIMATION();
    }
    auto delay = DelayTime::create(2);
    auto func = CallFunc::create(std::bind(&NiuNiuLayer::removeSelf, this));
    auto seq = Sequence::create(delay, func, NULL);
    runAction(seq);
}

void NiuNiuLayer::removeSelf()
{
    E2L_NIUNIU_END info;
    info.eProtocol = e2l_niuniu_end;
    info.result = clickResult;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
//    removeFromParent();
}
