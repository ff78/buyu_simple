//
//  LuckyDrawLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/3.
//
//

#include "LuckyDrawLayer.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "GameDef.h"
#include "EventDef.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

LuckyDrawLayer::LuckyDrawLayer()
{
    
}

LuckyDrawLayer::~LuckyDrawLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateListener);
}
bool LuckyDrawLayer::init()
{
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
    
    auto LuckyLayer = CSLoader::createNode(LUCKY_DRAW_UI);
    LuckyLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    LuckyLayer->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(LuckyLayer);
    
    root = LuckyLayer->getChildByName("ld_bg");
    activeArea = (Sprite*)root->getChildByName("ld_down");
    auto closeBtn = (Button*)root->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_1(LuckyDrawLayer::clickClose, this));
    
    for (int i = 0; i < 6; i++) {
        std::string cardName("ld_card_");
        cardName += Convert2String(i);
        
        card[i] = (Sprite*)root->getChildByName("ld_yellow")->getChildByName(cardName);
    }
    
    for (int i = 1; i <= 6; i++) {
        std::string buttonName("level_button_");
        buttonName += Convert2String(i);
        
        levelBtn[i-1] = (Button*)root->getChildByName(buttonName);
        levelBtn[i-1]->addClickEventListener(CC_CALLBACK_1(LuckyDrawLayer::clickLevel, this));
    }
    
    updateListener = EventListenerCustom::create(UI_UPDATE_LUCKY_DRAW, CC_CALLBACK_1(LuckyDrawLayer::updateLuckyDraw, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateListener, -1);
    return true;
}

void LuckyDrawLayer::enterLuckyDraw(cocos2d::EventCustom *event)
{
    L2E_SHOW_LUCKY_DRAW info = *static_cast<L2E_SHOW_LUCKY_DRAW*>(event->getUserData());
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
    
    auto currScore = (Text*)root->getChildByName("Text_1");
    currScore->setString(Convert2String(info.killScore));
    for (int i = 0; i < 6; i++) {
        std::string buttonName("level_button_");
        buttonName += Convert2String(i+1);
        auto btn = (Button*)root->getChildByName(buttonName);
        btn->setEnabled(i+1 != info.level);
    }
    std::string panelName("Panel_");
    panelName += Convert2String(info.showType);
    
    for (auto layer : activeArea->getChildren()) {
        layer->setVisible(false);
    }
    
    
    auto activePanel = activeArea->getChildByName(panelName);
    activePanel->setVisible(true);
    level = info.level;
    switch (info.showType) {
        case 1:
        {
            auto showBossBtn = (Button*)activePanel->getChildByName("boss_button");
            auto killBar = (LoadingBar*)activePanel->getChildByName("kill_bar");
            auto killLevelSpt = (Sprite*)activePanel->getChildByName("kill_level");
            auto killBarLabel = (TextAtlas*)activePanel->getChildByName("kill_label");
            
            killBar->setPercent((float)info.killScore*100.0/info.maxScore);
            
            std::string temp("game_scene/gamer_ui/lucky_draw/ld_type");
            temp += Convert2String(level);
            temp += ".png";
            killLevelSpt->initWithFile(temp);
            
            std::string lableStr(Convert2String(info.killScore));
            lableStr += "/";
            lableStr += Convert2String(info.maxScore);
            killBarLabel->setString(lableStr);
            
            showBossBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::showBossDialog, this));
        }
            break;
        case 2:
        {
            level = info.level + 1;
            nextTitle = info.nextTitle;
            auto lotteryBtn = (Button*)activePanel->getChildByName("lottery_button");
            lotteryBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::showConfimDialog, this));
            auto killBar = (LoadingBar*)activePanel->getChildByName("kill_bar");
            auto killLevelSpt = (Sprite*)activePanel->getChildByName("kill_level");
            auto killBarLabel = (TextAtlas*)activePanel->getChildByName("kill_label");
            
            killBar->setPercent((float)info.killScore*100.0/info.maxScore);
            score = info.killScore;
            maxScore = info.maxScore;
            
            std::string temp("game_scene/gamer_ui/lucky_draw/ld_type");
            temp += Convert2String(level);
            temp += ".png";
            killLevelSpt->initWithFile(temp);
            
            std::string lableStr(Convert2String(info.killScore));
            lableStr += "/";
            lableStr += Convert2String(info.maxScore);
            killBarLabel->setString(lableStr);
        }
            break;
        case 4:
        {
            auto lotteryBtn = (Button*)activePanel->getChildByName("lottery_button");
            lotteryBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::enterLottery, this));
        }
            break;
            

        default:
            break;
    }
}

void LuckyDrawLayer::showConfimDialog()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    confirmLayer = LayerColor::create(Color4B(10, 10, 10, 128));
    addChild(confirmLayer);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,confirmLayer);
    
    confirmDialog = CSLoader::createNode(LOTTERY_TIP_UI);
    confirmDialog->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    confirmDialog->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    auto okBtn = (Button*)confirmDialog->getChildByName("common_tips_1")->getChildByName("ok_button");
    auto cancelBtn = (Button*)confirmDialog->getChildByName("common_tips_1")->getChildByName("cancel_button");
    auto titleLabel = (Text*)confirmDialog->getChildByName("common_tips_1")->getChildByName("level_label");
    auto scoreLabel = (Text*)confirmDialog->getChildByName("common_tips_1")->getChildByName("score_label");
    titleLabel->setString(nextTitle);
    scoreLabel->setString(Convert2String(maxScore-score));
    okBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::enterLottery, this));
    cancelBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::removeConfirm, this));
    confirmLayer->addChild(confirmDialog);
    
}

void LuckyDrawLayer::showBossDialog()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    bossLayer = LayerColor::create(Color4B(10,10,10,128));
    addChild(bossLayer);
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,bossLayer);
    
    bossDialog = CSLoader::createNode(SHOW_BOSS_UI);
    bossDialog->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bossDialog->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    
    auto closeBtn = (Button*)bossDialog->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::removeBossDialog, this));
    bossLayer->addChild(bossDialog);
}

void LuckyDrawLayer::removeBossDialog()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeChild(bossLayer);
}

void LuckyDrawLayer::enterLottery()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_LOTTERY info;
    info.eProtocol = e2l_show_lottery;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void LuckyDrawLayer::removeConfirm()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeChild(confirmLayer);
}

void LuckyDrawLayer::updateLuckyDraw(EventCustom* event)
{
    L2E_UPDATE_LUCKY_DRAW info = *static_cast<L2E_UPDATE_LUCKY_DRAW*>(event->getUserData());
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
    
    auto currScore = (Text*)root->getChildByName("Text_1");
    currScore->setString(Convert2String(info.killScore));

    for (int i = 0; i < 6; i++) {
        std::string buttonName("level_button_");
        buttonName += Convert2String(i+1);
        auto btn = (Button*)root->getChildByName(buttonName);
        btn->setEnabled(i+1 != info.level);
    }
    
    std::string panelName("Panel_");
    panelName += Convert2String(info.showType);
    
    for (auto layer : activeArea->getChildren()) {
        layer->setVisible(false);
    }
    
    auto activePanel = activeArea->getChildByName(panelName);
    activePanel->setVisible(true);
    level = info.level;
    switch (info.showType) {
        case 1:
        {
            auto showBossBtn = (Button*)activePanel->getChildByName("boss_button");
            auto killBar = (LoadingBar*)activePanel->getChildByName("kill_bar");
            auto killLevelSpt = (Sprite*)activePanel->getChildByName("kill_level");
            auto killBarLabel = (TextAtlas*)activePanel->getChildByName("kill_label");
            
            killBar->setPercent((float)info.killScore*100.0/info.maxScore);
            
            std::string temp("game_scene/gamer_ui/lucky_draw/ld_type");
            temp += Convert2String(level);
            temp += ".png";
            killLevelSpt->initWithFile(temp);
            
            std::string lableStr(Convert2String(info.killScore));
            lableStr += "/";
            lableStr += Convert2String(info.maxScore);
            killBarLabel->setString(lableStr);
            
            showBossBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::showBossDialog, this));
        }
            break;
        case 2:
        {
            level = info.level + 1;
            nextTitle = info.nextTitle;
            auto lotteryBtn = (Button*)activePanel->getChildByName("lottery_button");
            lotteryBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::showConfimDialog, this));
            auto killBar = (LoadingBar*)activePanel->getChildByName("kill_bar");
            auto killLevelSpt = (Sprite*)activePanel->getChildByName("kill_level");
            auto killBarLabel = (TextAtlas*)activePanel->getChildByName("kill_label");
            
            killBar->setPercent((float)info.killScore*100.0/info.maxScore);
            score = info.killScore;
            maxScore = info.maxScore;
            
            std::string temp("game_scene/gamer_ui/lucky_draw/ld_type");
            temp += Convert2String(level);
            temp += ".png";
            killLevelSpt->initWithFile(temp);
            
            std::string lableStr(Convert2String(info.killScore));
            lableStr += "/";
            lableStr += Convert2String(info.maxScore);
            killBarLabel->setString(lableStr);
        }
            break;
        case 4:
        {
            auto lotteryBtn = (Button*)activePanel->getChildByName("lottery_button");
            lotteryBtn->addClickEventListener(CC_CALLBACK_0(LuckyDrawLayer::enterLottery, this));
        }
            break;
            
            
        default:
            break;
    }
}

void LuckyDrawLayer::clickLevel(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    int levelBtnTag = ((Button*)pSender)->getTag();
    
    E2L_UPDATE_LUCKY_DRAW info;
    info.eProtocol = e2l_update_lucky_draw;
    info.level = levelBtnTag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void LuckyDrawLayer::clickClose(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_EXIT_LUCKY_DRAW info;
    info.eProtocol = e2l_exit_lucky_draw;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
