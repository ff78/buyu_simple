//
//  GoldRunnerScene.cpp
//  buyu_simple
//
//  Created by ff on 16/1/6.
//
//

#include "GoldRunnerScene.h"
#include "utils/GameUtils.h"
#include "../UIManager.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "utils/StringData.h"

GoldRunnerScene::~GoldRunnerScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(openBoxListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(rollListener);
}

bool GoldRunnerScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create("common/treasure_map.png");
    bg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(bg);
    
    node = CSLoader::createNode("StartGold.csb");
    node->setAnchorPoint(Vec2(0.5, 0.5));
    node->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(node);
    
    auto exitBtn = (Button*)node->getChildByName("exitBtn");
    exitBtn->addTouchEventListener(CC_CALLBACK_2(GoldRunnerScene::clickExit, this));
    
    auto helpBtn = (Button*)node->getChildByName("helpBtn");
    helpBtn->addClickEventListener(CC_CALLBACK_1(GoldRunnerScene::clickHelp, this));
    
    auto root = node->getChildByName("treasure_base_2")->getChildByName("treasure_roulette_0_3");
    auto startBtn = (Button*)root->getChildByName("StartGame");
    startBtn->addClickEventListener(CC_CALLBACK_1(GoldRunnerScene::clickStartGame, this));
    
    
    startRunBtn = (ui::Button*)root->getChildByName("StartRun");
    startRunBtn->addClickEventListener(CC_CALLBACK_1(GoldRunnerScene::clickStartRunner, this));
    
    updateListener = EventListenerCustom::create(UPDATE_GOLD_RUNNER_EVENT, CC_CALLBACK_1(GoldRunnerScene::updateData, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateListener, -1);
    openBoxListener = EventListenerCustom::create(GOLD_OPEN_BOX, CC_CALLBACK_1(GoldRunnerScene::openBox, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(openBoxListener, -1);
    rollListener = EventListenerCustom::create(GOLD_START_ROLL, CC_CALLBACK_1(GoldRunnerScene::startRoll, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(rollListener, -1);

//    treasure_introduce.png

    
    return true;

}


void GoldRunnerScene::endOfStart()
{
    runnerAction->play("run", true);
    runDuration = abs(rand())%5+3;
    auto func = CallFunc::create(CC_CALLBACK_0(GoldRunnerScene::stopRunner, this));
    auto delay = DelayTime::create(runDuration);
    auto seq = Sequence::create(delay, func, NULL);
    node->runAction(seq);
    runnerAction->clearLastFrameCallFunc();
}

void GoldRunnerScene::stopRunner()
{
//    randCount = abs(rand())%10+1;
    std::string actionName("stop");
    actionName += Convert2String(randCount);
    runnerAction->play(actionName, false);
    runnerAction->setLastFrameCallFunc(CC_CALLBACK_0(GoldRunnerScene::getThing, this));
}

void GoldRunnerScene::getThing()
{
    //    runnerAction->stop();
    runnerAction->clearLastFrameCallFunc();
    auto root = node->getChildByName("treasure_base_2")->getChildByName("treasure_roulette_0_3");
    root->getChildByName("StartRun")->setVisible(false);
    root->getChildByName("StartGame")->setVisible(true);
    
    E2L_OPEN_BOX ask;
    ask.eProtocol = e2l_gold_open_fate_roll;
    ClientLogic::instance()->ProcessUIRequest(&ask);
}

void GoldRunnerScene::clickStartRunner(Ref* pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_GOLD_START_ROLL info;
    info.eProtocol = e2l_gold_start_runner;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GoldRunnerScene::startRoll(EventCustom* event)
{
    L2E_START_ROLL info = *static_cast<L2E_START_ROLL*>(event->getUserData());
    randCount = info.itemType;
    
    startRunBtn->setVisible(false);
    runnerAction = CSLoader::createTimeline("StartGold.csb");
    runnerAction->play("startRun", false);
    runnerAction->setLastFrameCallFunc(CC_CALLBACK_0(GoldRunnerScene::endOfStart, this));
    node->runAction(runnerAction);
}

void GoldRunnerScene::clickStartGame(Ref* pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_GOLD_START_GAME info;
    info.eProtocol = e2l_gold_start_game;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GoldRunnerScene::updateData(cocos2d::EventCustom *event)
{
    L2E_UPDATE_GOLD_RUNNER info = *static_cast<L2E_UPDATE_GOLD_RUNNER *>(event->getUserData());
    if (info.errorCode != 0) {
        return;
    }
    auto root = node->getChildByName("treasure_base_2");
    int count = info.keyCount;
    for (int i = 0; i < 5; i++) {
        std::string nokeyStr("treasure_nokey");
        nokeyStr += Convert2String(i+1);
        std::string keyStr("treasure_key");
        keyStr += Convert2String(i+1);
        
        bool lock = (i>=count);
        root->getChildByName(nokeyStr)->setVisible(lock);
        root->getChildByName(keyStr)->setVisible(!lock);
    }
    
    auto boxBtn = (ui::Button*)root->getChildByName("openBoxBtn");
    auto lock = boxBtn->getChildByName("treasure_box_lock_9");
    if (info.boxState == 0) {
        lock->setVisible(true);
        boxBtn->setEnabled(false);
        boxBtn->setTouchEnabled(false);
        boxBtn->setBright(true);
    }else if (info.boxState == 1) {
        lock->setVisible(false);
        boxBtn->setEnabled(true);
        boxBtn->setTouchEnabled(true);
        boxBtn->setBright(true);
        boxBtn->addTouchEventListener(CC_CALLBACK_2(GoldRunnerScene::clickOpenBox, this));
    }else if(info.boxState == 2){
        lock->setVisible(false);
        boxBtn->setEnabled(false);
        boxBtn->setTouchEnabled(false);
        boxBtn->setBright(false);
//        boxBtn->addTouchEventListener(CC_CALLBACK_2(GoldRunnerScene::openBox, this));
    }
    
    auto root1 = node->getChildByName("treasure_base_2")->getChildByName("treasure_roulette_0_3");
    root1->getChildByName("StartRun")->setVisible(!info.hasRolled);
    root1->getChildByName("StartGame")->setVisible(info.hasRolled);
    
}

void GoldRunnerScene::clickOpenBox(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            E2L_OPEN_BOX info;
            info.eProtocol = e2l_gold_fate_box;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}


void GoldRunnerScene::clickExit(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            E2L_GOLD_BACK info;
            info.eProtocol = e2l_gold_back_stage;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}


void GoldRunnerScene::openBox(EventCustom* event)
{
    L2E_OPEN_BOX info = *static_cast<L2E_OPEN_BOX*>(event->getUserData());
    if (info.errorCode == 1) {
        MessageBox(SSTRING("gold_need_keys").c_str(), SSTRING("gold_box_locked").c_str());
        return;
    }else if (info.errorCode == 2){
        MessageBox(SSTRING("gold_box_need_stage").c_str(), SSTRING("gold_box_locked").c_str());
        return;
    }else if(info.errorCode == 3){
        MessageBox(SSTRING("gold_box_has_used").c_str(), SSTRING("gold_box_locked").c_str());
        return;
    }
    
    int itemType = info.itemType;
    int boxMoney = info.money;
    if (itemType != 0) {
        MessageBox(SSTRING("gold_box_open_bound").c_str(), SSTRING("gold_box_unlock").c_str());
    }else if (boxMoney != 0) {
        MessageBox(SSTRING("gold_box_open_big_bound").c_str(), SSTRING("gold_box_unlock").c_str());
    }else{
        MessageBox(SSTRING("gold_box_open_empty").c_str(), SSTRING("gold_box_unlock").c_str());
        return;
    }
    
    E2L_OPEN_BOX ask;
    ask.eProtocol = e2l_gold_open_fate_box;
    ClientLogic::instance()->ProcessUIRequest(&ask);
}

void GoldRunnerScene::clickHelp(Ref* pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    auto winSize = Director::getInstance()->getWinSize();
    helpLayer = Layer::create();
    auto spt = Sprite::create("img/treasure_introduce.png");
    spt->setAnchorPoint(Vec2(0.5, 1));
    spt->setPosition(Vec2(winSize.width/2, winSize.height));
    helpLayer->addChild(spt);
    addChild(helpLayer);
    helpLayer->setVisible(false);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GoldRunnerScene::clickHelpScreen, this);
    listener->setSwallowTouches(true);
    helpLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,helpLayer);
    helpLayer->setVisible(true);
}

bool GoldRunnerScene::clickHelpScreen(cocos2d::Touch *touch, cocos2d::Event *evnt)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    helpLayer->removeFromParent();
    return true;
}