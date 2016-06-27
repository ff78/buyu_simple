//
//  DiaryLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/12.
//
//

#include "DiaryLayer.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "GameDef.h"
#include "sound/SoundManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool DiaryLayer::init()
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
    
    root = CSLoader::createNode(DIALY_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(root);
    
    auto closeBtn = (Button*)root->getChildByName("close_btn");
    closeBtn->addClickEventListener(CC_CALLBACK_0(DiaryLayer::closeDiary, this));
    return true;
}

void DiaryLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_DIARY info = *static_cast<L2E_SHOW_DIARY*>(event->getUserData());
    auto cardScroll = (ScrollView*)root->getChildByName("cards_scroll");
    if (info.fishTypeCount > 4) {
        cardScroll->getInnerContainer()->setContentSize(Size(676.00, 210*((info.fishTypeCount-1)/4+1)));
    }
    

    for (int i = 0; i < info.fishTypeCount; i++) {
        auto tempCard = CSLoader::createNode(DIALY_CARD_UI);
        tempCard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        float firstY = cardScroll->getInnerContainer()->getContentSize().height-110;
        tempCard->setPosition(Vec2(90+i%4*165, firstY-i/4*210));
        auto titleLabel = (Text*)tempCard->getChildByName("card_bg")->getChildByName("title_bg")->getChildByName("title_label");
        titleLabel->setString(info.killTitle[i]);
        auto countLabel = (TextAtlas*)tempCard->getChildByName("card_bg")->getChildByName("count_label");
        countLabel->setString(Convert2String(info.killCount[i]));
        
        std::string resDir("game_scene/fish/small_fish_alive/");
        int fishType = info.killFishType[i];
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
        resDir += info.killRes[i];
        resDir += "_0.png";
        auto bossSpt = (Sprite*)tempCard->getChildByName("card_bg")->getChildByName("fish_spt");
        bossSpt->initWithFile(resDir);
        if (bossSpt->getContentSize().width>100) {
            bossSpt->setScale(100/(float)bossSpt->getContentSize().width);
        }
        
        cardScroll->getInnerContainer()->addChild(tempCard);
    }
    
    cardScroll->jumpToTop();

}

void DiaryLayer::closeDiary()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeFromParent();
}
