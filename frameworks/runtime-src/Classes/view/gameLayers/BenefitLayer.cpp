//
//  BenefitLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/31.
//
//

#include "BenefitLayer.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "GameDef.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;

BenefitLayer::~BenefitLayer()
{
    
}

bool BenefitLayer::init()
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
        
    return true;
}

void BenefitLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_BENEFITS info = *static_cast<L2E_SHOW_BENEFITS*>(event->getUserData());
    if (info.type == 1) {
        auto rootNode = (Sprite*)CSLoader::createNode(BENEFITS_NORMAL_UI);
        rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        rootNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
        addChild(rootNode);
        root = (Sprite*)rootNode->getChildByName("ms_bg");
        auto closeBtn = (Button*)root->getChildByName("close_button");
        closeBtn->addClickEventListener(CC_CALLBACK_0(BenefitLayer::removeOff, this));
        
//        auto codeText = (TextField*)root->getChildByName("input_bg")->getChildByName("code_textfield");
        auto editBox = EditBox::create(Size(285, 41), "hall_scene/benefits/blank.png");
        editBox->setInputMode(EditBox::InputMode::NUMERIC);
        editBox->setMaxLength(6);
        editBox->setPosition(Vec2(290, 30));
        editBox->setTag(1);
        root->getChildByName("input_bg")->addChild(editBox);
//        root->removeChild(codeText, true);
        
        
        auto benefit1 = (Sprite*)root->getChildByName("benefits_1");
        auto benefit2 = (Sprite*)root->getChildByName("benefits_2");
        auto jumpButton = (Button*)root->getChildByName("jump_button");
        jumpButton->addClickEventListener(CC_CALLBACK_0(BenefitLayer::clickJumpBtn, this));
        
        float posY1 = benefit1->getPositionY();
        float posY2 = benefit2->getPositionY();

        if (info.hasReceive1) {
            jumpButton->setPositionY(posY2);
            benefit2->setPositionY(posY1);
            benefit1->setVisible(false);
        } else if (info.hasReceive2) {
            jumpButton->setPositionY(posY2);
            benefit2->setVisible(false);
        }
        
        auto lotteryBtn = (Button*)root->getChildByName("lottery_button");
        lotteryBtn->addClickEventListener(CC_CALLBACK_0(BenefitLayer::clickLotteryBtn, this));
    } else {
        auto rootNode = (Sprite*)CSLoader::createNode(BENEFITS_ONLINE_UI);
        rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        rootNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
        addChild(rootNode);
        
        root = (Sprite*)rootNode->getChildByName("ms_bg");
        auto closeBtn = (Button*)root->getChildByName("close_button");
        closeBtn->addClickEventListener(CC_CALLBACK_0(BenefitLayer::removeOff, this));
        
        auto linkBtn = (Button*)root->getChildByName("link_button");
        linkBtn->addClickEventListener(CC_CALLBACK_0(BenefitLayer::clickLinkBtn, this));
    }
}

void BenefitLayer::removeOff()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeFromParent();
}

void BenefitLayer::clickJumpBtn()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHOW_BENEFITS info;
    info.eProtocol = e2l_show_benefits;
    info.type = 2;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void BenefitLayer::clickLinkBtn()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_LINK_TO_NETGAME info;
    info.eProtocol = e2l_link_to_netgame;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void BenefitLayer::clickLotteryBtn()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    auto codeText = (EditBox*)root->getChildByName("input_bg")->getChildByTag(1);
    
    E2L_CASH_BENEFIT_CODE info;
    info.eProtocol = e2l_cash_benefit_code;
    info.codeStr = codeText->getText();
    ClientLogic::instance()->ProcessUIRequest(&info);
}
