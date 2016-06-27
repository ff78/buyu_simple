//
//  GoldStageScene.cpp
//  buyu_simple
//
//  Created by ff on 16/1/6.
//
//

#include "GoldStageScene.h"
#include "../UIManager.h"
#include "GameDef.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"


GoldStageScene::~GoldStageScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(openBoxListener);
}

bool GoldStageScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create("common/treasure_map.png");
    bg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(bg);
    
    root = CSLoader::createNode("SelInstance.csb");
    root->setAnchorPoint(Vec2(0.5, 0.5));
    root->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(root);
    
    auto exitBtn = (Button*)root->getChildByName("exitBtn");
    exitBtn->addTouchEventListener(CC_CALLBACK_2(GoldStageScene::clickExit, this));
    
    updateListener = EventListenerCustom::create(UPDATE_GOLD_STAGE_EVENT, CC_CALLBACK_1(GoldStageScene::updateData, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateListener, -1);
    openBoxListener = EventListenerCustom::create(GOLD_OPEN_BOX, CC_CALLBACK_1(GoldStageScene::openBox, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(openBoxListener, -1);
    return true;
}

void GoldStageScene::updateData(EventCustom* event)
{
    L2E_UPDATE_SELGOLD info = *static_cast<L2E_UPDATE_SELGOLD *>(event->getUserData());
    if (info.errorCode == 1) {
        MessageBox(SSTRING("gold_ban_rechallenge").c_str(), SSTRING("gold_stage_open_fail").c_str());
        return;
    }
    
    int lastStage = info.lastStage;
    for (int i = 0; i < lastStage; i++) {
        std::string btnStr("StageBtn");
        btnStr += Convert2String(i+1);
        auto btn = (Button*)root->getChildByName(btnStr);
        btn->setEnabled(true);
        btn->setTouchEnabled(true);
        btn->setBright(true);
        btn->addTouchEventListener(CC_CALLBACK_2(GoldStageScene::clickStageBtn, this));
        
        if (i == MAX_GOLD_STAGE-1) {
            auto lockSpt = (Sprite*)btn->getChildByName("treasure_lock_21");
            lockSpt->setVisible(false);
        }else{
            std::string numStr("num");
            numStr += Convert2String(i+1);
            auto numSpt = (Sprite*)btn->getChildByName(numStr);
            numSpt->setVisible(true);
        }
        
    }
    
    auto btn = (Button*)root->getChildByName("BoxBtn");
    auto lockSpt = (Sprite*)btn->getChildByName("boxLock");
    if (info.boxState == 0) {
        lockSpt->setVisible(true);
        btn->setEnabled(false);
        btn->setTouchEnabled(false);
        btn->setBright(true);
    }else if (info.boxState == 1) {
        lockSpt->setVisible(false);
        btn->setEnabled(true);
        btn->setTouchEnabled(true);
        btn->setBright(true);
        btn->addTouchEventListener(CC_CALLBACK_2(GoldStageScene::clickOpenBox, this));
    }else if (info.boxState == 2) {
        lockSpt->setVisible(false);
        btn->setEnabled(false);
        btn->setTouchEnabled(false);
        btn->setBright(false);
    }
    
    auto focusSpt = (Sprite*)root->getChildByName("treasure_site2_23");
    std::string btnStr("StageBtn");
    btnStr += Convert2String(lastStage);
    auto stageBtn = (Button*)root->getChildByName(btnStr);
    focusSpt->setPosition(stageBtn->getPosition());
    focusSpt->setVisible(info.hasFocus);
}

void GoldStageScene::clickStageBtn(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            int tag = ((Button*)pSender)->getTag();
            E2L_SEL_GOLD info;
            info.eProtocol = e2l_sel_gold;
            info.selStage = tag;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}

void GoldStageScene::clickOpenBox(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            E2L_OPEN_BOX info;
            info.eProtocol = e2l_gold_stage_box;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}


void GoldStageScene::openBox(EventCustom* event)
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
    ask.eProtocol = e2l_gold_open_stage_box;
    ClientLogic::instance()->ProcessUIRequest(&ask);
}

void GoldStageScene::clickExit(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            E2L_GOLD_BACK info;
            info.eProtocol = e2l_gold_back_diff;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}
