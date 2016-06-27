//
//  GoldDifSelScene.cpp
//  buyu_simple
//
//  Created by ff on 16/1/5.
//
//

#include "GoldDifSelScene.h"
#include "utils/GameUtils.h"
#include "../UIManager.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "utils/StringData.h"

GoldDifSelScene::~GoldDifSelScene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateListener);
}

bool GoldDifSelScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create("common/treasure_map.png");
    bg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(bg);
    
    root = CSLoader::createNode("SelStage.csb");
    root->setAnchorPoint(Vec2(0.5, 0.5));
    root->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(root);
    
    auto panel = root->getChildByName("Panel_1")->getChildByName("common_panel_2");
    for (int i = 0; i < 3; i++) {
        std::string btnStr("Dif");
        btnStr += Convert2String(i+1);
        btnStr += "Btn";
        auto btn = (Button*)panel->getChildByName(btnStr);
        
        btn->addTouchEventListener(CC_CALLBACK_2(GoldDifSelScene::clickDiffBtn, this));
    }
    auto btn = (Button*)panel->getChildByName("treasure_paper_5")->getChildByName("StartBtn");
    btn->addTouchEventListener(CC_CALLBACK_2(GoldDifSelScene::clickStartBtn, this));
    

    updateListener = EventListenerCustom::create(UPDATE_GOLD_DIFF_EVENT, CC_CALLBACK_1(GoldDifSelScene::updateData, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateListener, -1);

    return true;
}

void GoldDifSelScene::updateData(EventCustom* event)
{
    L2E_GOLD_DIFF info = *static_cast<L2E_GOLD_DIFF *>(event->getUserData());
    if (info.errCode != 0) {
        switch (info.errCode) {
            case 1:
                MessageBox(SSTRING("gold_lack_money").c_str(), SSTRING("gold_ban_challenge").c_str());
                break;
            case 2:
                MessageBox(SSTRING("gold_challenge_exhaust").c_str(), SSTRING("gold_ban_challenge").c_str());
                break;
            default:
                break;
        }
        return;
    }
    
    auto panel = root->getChildByName("Panel_1")->getChildByName("common_panel_2");
    
    std::string btnStr("Dif");
    btnStr += Convert2String(focus+1);
    btnStr += "Btn";
    auto btn = (Button*)panel->getChildByName(btnStr);
    btn->setBright(true);
    btn->setTouchEnabled(true);
    btn->setEnabled(true);
    
    std::string picStr("treasure");
    picStr += Convert2String(focus + 1);
    picStr += "Pic";
    auto pic = (Sprite*)panel->getChildByName("treasure_paper_5")->getChildByName(picStr);
    pic->setVisible(false);

    fee = info.fee;
    challengeCount = info.validCount;
    timeLimit = info.timeLimit;
    focus = info.diff;
    
    btnStr = "Dif";
    btnStr += Convert2String(focus+1);
    btnStr += "Btn";
    btn = (Button*)panel->getChildByName(btnStr);
    btn->setBright(false);
    btn->setTouchEnabled(false);
    btn->setEnabled(false);

    picStr = "treasure";
    picStr += Convert2String(focus + 1);
    picStr += "Pic";
    pic = (Sprite*)panel->getChildByName("treasure_paper_5")->getChildByName(picStr);
    pic->setVisible(true);
    
    auto countLabel = (Text*)panel->getChildByName("treasure_paper_5")->getChildByName("countLabel");
    countLabel->setString(Convert2String(challengeCount));
    auto limitLabel = (Text*)panel->getChildByName("treasure_paper_5")->getChildByName("timeLimitLabel");
    limitLabel->setString(Convert2String(timeLimit));
    
    auto feeLabel = (Text*)panel->getChildByName("treasure_paper_5")->getChildByName("feeLabel");
    if (fee == 0) {
        feeLabel->setString(SSTRING("gold_free"));
    }else{
        feeLabel->setString(Convert2String(fee));
    }
}

void GoldDifSelScene::clickDiffBtn(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            int tag = ((Button*)pSender)->getTag();
            E2L_GOLD_DIFF info;
            info.eProtocol = e2l_gold_diff;
            info.diff = tag;
            
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}

void GoldDifSelScene::clickStartBtn(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            E2L_GOLD_SEL_DIFF info;
            info.eProtocol = e2l_gold_start_diff;
            info.diff = focus;
            
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}

