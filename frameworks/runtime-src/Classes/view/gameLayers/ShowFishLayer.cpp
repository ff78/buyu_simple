//
//  ShowFishLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/18.
//
//

#include "ShowFishLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "GameDef.h"
#include "sound/SoundManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool ShowFishLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(10, 10, 10, 128))) {
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
    
    root = CSLoader::createNode(SHOW_FISH_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    auto closeBtn = (Button*)root->getChildByName("settings_bg")->getChildByName("close_btn");
    closeBtn->addClickEventListener(CC_CALLBACK_0(ShowFishLayer::closeShowFish, this));
    
    return true;
}

void ShowFishLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_FISH info = *static_cast<L2E_SHOW_FISH*>(event->getUserData());
    auto cardScroll = (ScrollView*)root->getChildByName("settings_bg")->getChildByName("fish_scroll");
    if (info.fishTypeCount > 4) {
        cardScroll->getInnerContainer()->setContentSize(Size(634.00, 135*((info.fishTypeCount-1)/4+1)));
    }
    
    
    for (int i = 0; i < info.fishTypeCount; i++) {
        auto tempCard = CSLoader::createNode(FISH_CARD_UI);
        tempCard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        float firstY = cardScroll->getInnerContainer()->getContentSize().height-67;
        tempCard->setPosition(Vec2(70+i%4*165, firstY-i/4*135));
        auto priceLabel = (Text*)tempCard->getChildByName("fish_bg")->getChildByName("price_bar")->getChildByName("price_label");
        priceLabel->setString(("X"+Convert2String(info.fishPrice[i])).c_str());

        auto bg = (Sprite*)tempCard->getChildByName("fish_bg");
        std::string resDir("game_scene/fish/small_fish_alive/");
        int fishType = info.fishType[i];
        switch (fishType) {
            case SMALL_FISH:
            {
                resDir = "game_scene/fish/small_fish_alive/";
                bg->initWithFile("game_scene/gamer_ui/settings/settings_frame1.png");
            }
                break;
            case TINY_BOSS_FISH:
            {
                resDir = "game_scene/fish/tiny_boss_fish_alive/";
                bg->initWithFile("game_scene/gamer_ui/settings/settings_frame2.png");
            }
                break;
            case BOSS_FISH:
            case SPECIAL_BOSS_FISH:
            {
                resDir = "game_scene/fish/boss_fish_alive/";
                bg->initWithFile("game_scene/gamer_ui/settings/settings_frame3.png");
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
        resDir += info.fishRes[i];
        resDir += "_0.png";
        auto bossSpt = (Sprite*)bg->getChildByName("fish_sprite");
        bossSpt->initWithFile(resDir);
        auto sptSize = bossSpt->getContentSize();
        float reallyLen = MAX(sptSize.width, sptSize.height);
        bossSpt->setScale(100/reallyLen);
        
        cardScroll->getInnerContainer()->addChild(tempCard);
    }
    
    cardScroll->jumpToTop();
}

void ShowFishLayer::closeShowFish()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeFromParent();
}

