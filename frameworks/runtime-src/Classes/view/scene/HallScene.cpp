//
//  HallScene.cpp
//  buyu_simple
//
//  Created by ff on 15/12/9.
//
//

#include "HallScene.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "GameDef.h"
#include "EventDef.h"
#include "sound/SoundManager.h"
#include "logic/GameCore.h"
#include "../node/GoldFallen.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSHelper.h"
#endif

USING_NS_CC;
using namespace ui;

HallScene::~HallScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(showDiaryListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showRechargeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showVipIntroListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showVipGiftListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateMoneyListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(rechargeOkListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showBenefitsListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(cashBenefitCodeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showFreeGoldListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateShareResultListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(takeShareCountBoundListener);
}

bool HallScene::init()
{
    if (!Scene::init()) {
        return false;
    }
//    auto winSize = Director::getInstance()->getWinSize();
    
//    auto root = CSLoader::createNode("HallScene.csb");
//    root->setAnchorPoint(Vec2(0.5, 0.5));
//    root->setPosition(Vec2(winSize.width/2, winSize.height/2));
//    addChild(root);
//    
////    auto button = Button::create("BEGIN_GAME_IMG");
////    button->addTouchEventListener(CC_CALLBACK_2(HallScene::start, this));
//    
//    auto button = (Button*)root->getChildByName("StartBtn");
//    button->addTouchEventListener(CC_CALLBACK_2(HallScene::start, this));
    
    auto bgSpt = Sprite::create("hall_scene/game_hall/background_0.png");
    bgSpt->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    bgSpt->setScaleX(GameUtils::winSize.width/bgSpt->getContentSize().width);
    addChild(bgSpt);
    
    auto root = CSLoader::createNode(HALL_STAGE_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    leftRoot = CSLoader::createNode(HALL_LEFT_UI);
    leftRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    leftRoot->setPosition(Vec2(0, GameUtils::winSize.height/2));
    addChild(leftRoot);
    
    rightRoot = CSLoader::createNode(HALL_RIGHT_UI);
    rightRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rightRoot->setPosition(Vec2(GameUtils::winSize.width, GameUtils::winSize.height/2));
    addChild(rightRoot);
    
    rightAction = CSLoader::createTimeline(HALL_RIGHT_UI);
    rightRoot->runAction(rightAction);
    rightAction->play("play", true);
    topRoot = root->getChildByName("top_node");
    
    auto bottomSpt = (Sprite*)root->getChildByName("gh_down_8");    
    if (GameUtils::winSize.width > 960) {
        topRoot->setScaleX(GameUtils::winSize.width/960);
        bottomSpt->setScaleX(GameUtils::winSize.width/960);
    }
    
    auto rechargeBtn = (Button*)topRoot->getChildByName("recharge_btn");
    rechargeBtn->addClickEventListener(CC_CALLBACK_0(HallScene::clickRecharge, this));
    auto topAction = CSLoader::createTimeline(HALL_TOP_UI);
    topAction->play("play", true);
    topRoot->runAction(topAction);
//    Vector<FiniteTimeAction*> scaleVec;
//    scaleVec.pushBack(ScaleTo::create(0.2, 1.2));
//    scaleVec.pushBack(ScaleTo::create(0.1, 0.8));
//    scaleVec.pushBack(ScaleTo::create(0.2, 1.2));
//    scaleVec.pushBack(ScaleTo::create(0.1, 0.8));
//    scaleVec.pushBack(ScaleTo::create(0.2, 1.2));
//    scaleVec.pushBack(ScaleTo::create(0.1, 0.8));
//    scaleVec.pushBack(ScaleTo::create(0.1, 1.5));
//    scaleVec.pushBack(ScaleTo::create(0.1, 0.8));
//    auto seq = Sequence::create(scaleVec);
//    rechargeBtn->runAction(RepeatForever::create(seq));
    
    auto freeGoldBtn = (Button*)leftRoot->getChildByName("freegold_btn");
    freeGoldBtn->addClickEventListener(CC_CALLBACK_0(HallScene::clickFreeGold, this));
    
    stageScroll = (ScrollView*)root->getChildByName("stage_scroll");
    auto menuButton = (Button*)rightRoot->getChildByName("menu_btn");
    menuButton->addClickEventListener(CC_CALLBACK_0(HallScene::switchMenu, this));
    menuBg = (Sprite*)rightRoot->getChildByName("gh_menu_bg");
    auto soundButton = (Button*)menuBg->getChildByName("sound_button");
    soundButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickSound, this));
    auto serviceButton = (Button*)menuBg->getChildByName("service_button");
    serviceButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickService, this));
    auto exitButton = (Button*)menuBg->getChildByName("exit_button");
    exitButton->addClickEventListener(CC_CALLBACK_0(HallScene::clickExit, this));
    
    auto benefitBtn = (Button*)leftRoot->getChildByName("benefits_button");
    benefitBtn->addClickEventListener(CC_CALLBACK_0(HallScene::clickBenefit, this));
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    benefitBtn->setVisible(iOSHelper::bOnline);
#endif
    
    auto vipBtn = (Button*)root->getChildByName("vip_gift_button");
    vipBtn->addClickEventListener(CC_CALLBACK_0(HallScene::clickVip, this));
    focus = 0;
    quickStage = 0;
    openMenu = false;
    
    tipLayer = TipLayer::create();
    addChild(tipLayer, 1000);
    
    showDiaryListener = EventListenerCustom::create(UI_SHOW_DIARY, CC_CALLBACK_1(HallScene::showDiary, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showDiaryListener, -1);
    showRechargeListener = EventListenerCustom::create(UI_SHOW_HALL_RECHARGE, CC_CALLBACK_1(HallScene::showRecharge, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showRechargeListener, -1);
    showVipIntroListener = EventListenerCustom::create(UI_SHOW_VIP_INTRO, CC_CALLBACK_1(HallScene::showVipIntro, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showVipIntroListener, -1);
    showVipGiftListener = EventListenerCustom::create(UI_SHOW_VIP_GIFT, CC_CALLBACK_0(HallScene::showVipGift, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showVipGiftListener, -1);
    updateMoneyListener = EventListenerCustom::create(UI_UPDATE_HALL_MONEY, CC_CALLBACK_1(HallScene::updateMoney, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateMoneyListener, -1);
    rechargeOkListener = EventListenerCustom::create(UI_RECHARGE_OK, CC_CALLBACK_1(HallScene::rechargeOk, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(rechargeOkListener, -1);
    showBenefitsListener = EventListenerCustom::create(UI_SHOW_BENEFITS, CC_CALLBACK_1(HallScene::showBenefits, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showBenefitsListener, -1);
    cashBenefitCodeListener = EventListenerCustom::create(UI_CASH_BENEFIT_CODE, CC_CALLBACK_1(HallScene::cashBenefitCode, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(cashBenefitCodeListener, -1);
    
    showFreeGoldListener = EventListenerCustom::create(UI_SHOW_SHARE, CC_CALLBACK_1(HallScene::showFreeGold, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showFreeGoldListener, -1);
    updateShareResultListener = EventListenerCustom::create(UI_UPDATE_SHARE_RESULT, CC_CALLBACK_1(HallScene::updateShareResult, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateShareResultListener, -1);
    takeShareCountBoundListener = EventListenerCustom::create(UI_TAKE_SHARE_COUNT_BOUND, CC_CALLBACK_1(HallScene::takeShareCountBound, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(takeShareCountBoundListener, -1);
    
    AUDIO_PLAY("room", AUDIO_TYPE::MUSIC_TYPE);
    return true;
}

void HallScene::start(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            E2L_ENTERGAME info;
            info.eProtocol = e2l_enter_game;
            info.stage = 0;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}

void HallScene::setupView(L2E_TO_HALL &info)
{
    quickStage = info.maxUnlockStage;
    
    auto goldLabel = (TextAtlas*)leftRoot->getChildByName("gold_bg")->getChildByName("gold_label");
    auto diamondLabel = (TextAtlas*)leftRoot->getChildByName("diamond_bg")->getChildByName("diamond_label");
    goldLabel->setString(Convert2String(info.gold));
    diamondLabel->setString(Convert2String(info.diamond));
    
    auto levelLabel = (Text*)rightRoot->getChildByName("level_bg")->getChildByName("level_label");
    auto expLabel = (Text*)rightRoot->getChildByName("level_bg")->getChildByName("exp_label");
    auto expBar = (LoadingBar*)rightRoot->getChildByName("level_bg")->getChildByName("exp_bar");
    int expPercent = info.exp*100.0/info.maxExp;
    levelLabel->setString(Convert2String(info.level));
    expBar->setPercent(expPercent);
    expLabel->setString(Convert2String(expPercent)+"%");
    
    auto card0 = stageScroll->getChildByName("stage_card");
    auto scrollSize = Size(card0->getPositionX()*info.maxStage*2, 400);
    stageScroll->getInnerContainer()->setContentSize(scrollSize);
    card0->setTag(1);
    for (int i = 1; i < info.maxStage; i++) {
        auto tempCard = CSLoader::createNode(STAGE_CARD_UI);
        tempCard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        tempCard->setPosition(Vec2(card0->getPositionX()*(i*2+1), card0->getPositionY()));
        tempCard->setTag(i+1);
        stageScroll->getInnerContainer()->addChild(tempCard);
    }
    
    for (int i = 0; i < info.maxStage; i++) {
        int tagNum = i+1;
        auto card = stageScroll->getInnerContainer()->getChildByTag(tagNum);
        
        auto titleSpt = (Sprite*)card->getChildByName("card_bg")->getChildByName("title_spt");
        titleSpt->initWithFile(("hall_scene/fish_room/fr_title_"+Convert2String(tagNum) + ".png").c_str());
        
        auto boundBg = (Sprite*)card->getChildByName("card_bg")->getChildByName("reward_spt");
        auto goldLabel = (Text*)boundBg->getChildByName("bound_gold_label");
        auto diamondLabel = (Text*)boundBg->getChildByName("bound_diamond_label");
        goldLabel->setString(Convert2String(info.levelBoundGold[i]));
        diamondLabel->setString(Convert2String(info.levelBoundDiamond[i]));
        boundBg->setVisible(tagNum > info.maxUnlockStage);
        
        auto flagSpt = (Sprite*)card->getChildByName("card_bg")->getChildByName("pass_flag");
        flagSpt->setVisible(tagNum <= info.maxUnlockStage);
        if (tagNum == info.maxUnlockStage) {
            flagSpt->initWithFile("hall_scene/fish_room/fr_flag1.png");
        }
        auto startBtn = (Button*)card->getChildByName("start_btn");
        startBtn->setEnabled(tagNum <= info.maxUnlockStage);
        startBtn->addClickEventListener(std::bind(&HallScene::startStage, this, tagNum));
        
        
        if (tagNum <= info.maxUnlockStage+1) {
            std::string resDir("game_scene/fish/small_fish_alive/");
            int fishType = info.levelBossType[i];
            switch (fishType) {
                case SMALL_FISH:
                {
                    resDir = "game_scene/fish/small_fish_alive/";
                }
                    break;
                case TINY_BOSS_FISH:
                {
                    resDir = "game_scene/fish/tiny_boss_fish_alive/";
                }
                    break;
                case BOSS_FISH:
                {
                    resDir = "game_scene/fish/boss_fish_alive/";
                }
                    break;
                case SPECIAL_FISH:
                {
                    resDir = "game_scene/fish/special_fish_alive/";
                }
                    break;
                    
                default:
                {
                    resDir = "game_scene/fish/small_fish_alive/";
                }
                    break;
            }
            resDir += info.levelBossRes[i];
            resDir += "_0.png";
            auto bossSpt = (Sprite*)card->getChildByName("card_bg")->getChildByName("boss_spt");
            bossSpt->initWithFile(resDir);
            if (bossSpt->getContentSize().width>150) {
                bossSpt->setScale(150/(float)bossSpt->getContentSize().width);
            }
            
        }
    }
    
    stageScroll->jumpToPercentHorizontal((info.maxUnlockStage-1)*100.0/info.maxStage);
    
    quickStartBtn = (Button*)rightRoot->getChildByName("quick_start_btn");
    quickStartBtn->addClickEventListener(CC_CALLBACK_0(HallScene::quickStart, this));
    
    auto diaryBtn = (Button*)leftRoot->getChildByName("journal_button");
    diaryBtn->addClickEventListener(CC_CALLBACK_0(HallScene::clickDiary, this));
}

void HallScene::startStage(int index)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_ENTERGAME info;
    info.eProtocol = e2l_enter_game;
    info.stage = index;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::quickStart()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    if (quickStage == 0) {
        return;
    }
    
    startStage(quickStage);
}

void HallScene::clickDiary()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_DIARY info;
    info.eProtocol = e2l_show_diary;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::showDiary(cocos2d::EventCustom *event)
{
    auto diarylayer = DiaryLayer::create();
    diarylayer->setupView(event);
    addChild(diarylayer);
}

void HallScene::switchMenu()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    openMenu = !openMenu;
    menuBg->stopAllActions();
    if (openMenu) {
        float duration = (58+menuBg->getPositionX())/58*0.2;
        auto moveAction = MoveTo::create(duration, Vec2(-58, 162));
        menuBg->runAction(moveAction);
    }else{
        float duration = fabs(menuBg->getPositionX())/58*0.2;
        auto moveAction = MoveTo::create(duration, Vec2(0, 162));
        menuBg->runAction(moveAction);
    }
}

void HallScene::clickService()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    serviceLayer = LayerColor::create(Color4B(10, 10, 10, 128));
    addChild(serviceLayer);
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,serviceLayer);
    
    auto root = CSLoader::createNode(SERVICE_TIP_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    serviceLayer->addChild(root);
    
    auto closeBtn = (Button*)root->getChildByName("common_tips")->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_0(HallScene::removeService, this));
}

void HallScene::removeService()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeChild(serviceLayer);
}

void HallScene::clickSound()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    soundLayer = LayerColor::create(Color4B(10, 10, 10, 128));
    addChild(soundLayer);
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,soundLayer);
    
    soundRoot = CSLoader::createNode(HALL_SOUND_TIP_UI);
    soundRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundRoot->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    soundLayer->addChild(soundRoot);
    
    auto closeBtn = (Button*)soundRoot->getChildByName("common_tips")->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_0(HallScene::removeSound, this));
    
    auto musicBtn = (Button*)soundRoot->getChildByName("common_tips")->getChildByName("music_button");
    auto effectBtn = (Button*)soundRoot->getChildByName("common_tips")->getChildByName("sound_button");
    musicBtn->addClickEventListener(CC_CALLBACK_0(HallScene::switchMusic, this));
    effectBtn->addClickEventListener(CC_CALLBACK_0(HallScene::switchSound, this));
    if(!SoundManager::shared()->getSoundSwitch())
    {
        effectBtn->loadTextureNormal("hall_scene/game_hall/gh_sound_close.png");
    }
    if(!SoundManager::shared()->getMusicSwitch())
    {
        musicBtn->loadTextureNormal("hall_scene/game_hall/gh_sound_close.png");
    }
    
    
}

void HallScene::removeSound()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeChild(soundLayer);
}

void HallScene::switchSound()
{
    bool soundOpen = SoundManager::shared()->getSoundSwitch();
    soundOpen = !soundOpen;
    SoundManager::shared()->setSoundSwitch(soundOpen);
    auto effectBtn = (Button*)soundRoot->getChildByName("common_tips")->getChildByName("sound_button");
    if (soundOpen) {
        effectBtn->loadTextureNormal("hall_scene/game_hall/gh_sound_open.png");
        AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    }else{
        effectBtn->loadTextureNormal("hall_scene/game_hall/gh_sound_close.png");
    }
}

void HallScene::switchMusic()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    bool musicOpen = SoundManager::shared()->getMusicSwitch();
    musicOpen = !musicOpen;
    SoundManager::shared()->setMusicSwitch(musicOpen);
    auto musicBtn = (Button*)soundRoot->getChildByName("common_tips")->getChildByName("music_button");
    if (musicOpen) {
        musicBtn->loadTextureNormal("hall_scene/game_hall/gh_sound_open.png");
        AUDIO_PLAY("room", AUDIO_TYPE::MUSIC_TYPE);
    }else{
        musicBtn->loadTextureNormal("hall_scene/game_hall/gh_sound_close.png");
        SoundManager::shared()->stopMusic();
    }
}

void HallScene::clickExit()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    GameCore::instance()->save2Json();
    Director::getInstance()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
}

void HallScene::clickRecharge()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_HALL_RECHARGE info;
    info.eProtocol = e2l_show_hall_recharge;
    info.type = 1;
    ClientLogic::instance()->ProcessUIRequest(&info);
//    rechargeLayer = LayerColor::create(Color4B(10, 10, 10, 128));
//    addChild(rechargeLayer);
//    
//    auto callback = [](Touch * ,Event *)
//    {
//        return true;
//    };
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = callback;
//    listener->setSwallowTouches(true);
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,rechargeLayer);
//    
//    rechargeRoot = CSLoader::createNode("hall_scene/recharge/GoldRecharge.csb");
//    rechargeRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    rechargeRoot->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
//    rechargeLayer->addChild(rechargeRoot);
//    
//    auto closeBtn = (Button*)rechargeRoot->getChildByName("ms_bg")->getChildByName("close_button");
//    closeBtn->addClickEventListener(CC_CALLBACK_0(HallScene::removeRecharge, this));
    
    
}

void HallScene::showRecharge(EventCustom* event)
{
    L2E_SHOW_HALL_RECHARGE info = *static_cast<L2E_SHOW_HALL_RECHARGE*>(event->getUserData());
    removeChildByTag(1003);
    removeChildByTag(1004);
    removeChildByTag(1005);
    if (info.type == 1) {
        auto layer = GoldRechargeLayer::create();
        layer->setTag(1003);
        layer->setupView(event);
        addChild(layer);
    }else if(info.type == 0){
        auto layer = DiamondRechargeLayer::create();
        layer->setTag(1003);
        layer->setupView(event);
        addChild(layer);
    }
}

void HallScene::removeRecharge()
{
    removeChild(rechargeLayer);
}

void HallScene::showVipIntro(cocos2d::EventCustom *event)
{
    removeChildByTag(1003);
    removeChildByTag(1004);
    removeChildByTag(1005);
    
    auto layer = VipIntroLayer::create();
    layer->setTag(1004);
    layer->setupView(event);
    addChild(layer);
}

void HallScene::showVipGift()
{
    removeChildByTag(1003);
    removeChildByTag(1004);
    removeChildByTag(1005);
    
    auto layer = VipGiftLayer::create();
    layer->setTag(1005);
    addChild(layer);
}

void HallScene::clickVip()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_VIP info;
    info.eProtocol = e2l_show_vip;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::updateMoney(cocos2d::EventCustom *event)
{
    L2E_UPDATE_HALL_MONEY info = *(static_cast<L2E_UPDATE_HALL_MONEY*>(event->getUserData()));
    auto goldLabel = (TextAtlas*)leftRoot->getChildByName("gold_bg")->getChildByName("gold_label");
    auto diamondLabel = (TextAtlas*)leftRoot->getChildByName("diamond_bg")->getChildByName("diamond_label");
    goldLabel->setString(Convert2String(info.gold));
    diamondLabel->setString(Convert2String(info.diamond));
}

void HallScene::rechargeOk(cocos2d::EventCustom* event)
{
    tipLayer->addRechargeOkTip();
    GOLD_FALL_ANIMATION();
    L2E_RECHARGE_OK info = *static_cast<L2E_RECHARGE_OK*>(event->getUserData());
    if(info.type == 1)
    {
        EventCustom event1(UI_RECHARGE_GOLD_OK);
        event1.setUserData(&info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
    }else if (info.type == 0) {
        EventCustom event2(UI_RECHARGE_DIAMOND_OK);
        event2.setUserData(&info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event2);
    }
}

void HallScene::clickBenefit()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_BENEFITS info;
    info.eProtocol = e2l_show_benefits;
    info.type = 1;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::showBenefits(cocos2d::EventCustom *event)
{
    removeChildByTag(1006);
    auto layer = BenefitLayer::create();
    layer->setupView(event);
    layer->setTag(1006);
    addChild(layer);
}

void HallScene::cashBenefitCode(cocos2d::EventCustom *event)
{
    L2E_CASH_BENEFIT_CODE info = *static_cast<L2E_CASH_BENEFIT_CODE*>(event->getUserData());
    switch (info.errNo) {
        case 1:
        {
            tipLayer->addBenefitCodeError();
        }
            break;
        case 2:
        {
            tipLayer->addBenefitCodeUsed();
        }
            break;
        case 0:
        {
            L2E_SHOW_BENEFITS info1;
            info1.eProtocol = l2e_show_benefits;
            info1.hasReceive1 = info.hasReceive1;
            info1.hasReceive2 = info.hasReceive2;
            info1.type = info.type;
            
            EventCustom event1(UI_SHOW_BENEFITS);
            event1.setUserData(&info1);
            
            removeChildByTag(1006);
            auto layer = BenefitLayer::create();
            layer->setupView(&event1);
            layer->setTag(1006);
            addChild(layer);
            GOLD_FALL_ANIMATION();
        }
            break;
            
        default:
            break;
    }
    
    
}

void HallScene::clickFreeGold()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_SHARE info;
    info.eProtocol = e2l_show_share;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallScene::showFreeGold(cocos2d::EventCustom *event)
{
    removeChildByTag(1007);
    auto layer = ShareLayer::create();
    layer->setupView(event);
    layer->setTag(1007);
    addChild(layer);
}

void HallScene::updateShareResult(EventCustom* event)
{
    L2E_SHARE_RESULT info = *static_cast<L2E_SHARE_RESULT*>(event->getUserData());
    switch (info.errNo) {
        case 0:
        {
            L2E_SHOW_SHARE updateInfo;
            updateInfo.eProtocol = l2e_show_share;
            updateInfo.shareCount = info.shareCount;
            memcpy(updateInfo.countState, info.countState, sizeof(int)*4);
            
            EventCustom updateEvent(UI_SHOW_SHARE);
            updateEvent.setUserData(&updateInfo);
            
            removeChildByTag(1007);
            auto layer = ShareLayer::create();
            layer->setupView(&updateEvent);
            layer->setTag(1007);
            addChild(layer);

        }
            break;
        case 1:
        {
            tipLayer->addShareErrorTip();
        }
            break;
        case 2:
        {
            tipLayer->addQQUninstallTip();
        }
            break;
            
        default:
            break;
    }
}

void HallScene::takeShareCountBound(cocos2d::EventCustom *event)
{
    L2E_SHARE_COUNT_BOUND info = *static_cast<L2E_SHARE_COUNT_BOUND*>(event->getUserData());
    L2E_SHOW_SHARE updateInfo;
    updateInfo.eProtocol = l2e_show_share;
    updateInfo.shareCount = info.shareCount;
    memcpy(updateInfo.countState, info.countState, sizeof(int)*4);
    
    EventCustom updateEvent(UI_SHOW_SHARE);
    updateEvent.setUserData(&updateInfo);
    
    removeChildByTag(1007);
    auto layer = ShareLayer::create();
    layer->setupView(&updateEvent);
    layer->setTag(1007);
    addChild(layer);
    GOLD_FALL_ANIMATION();
}
