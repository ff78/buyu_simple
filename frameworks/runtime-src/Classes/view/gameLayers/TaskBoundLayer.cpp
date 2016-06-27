//
//  TaskBoundLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/4/6.
//
//

#include "TaskBoundLayer.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"
#include "GameDef.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"


USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool TaskBoundLayer::init()
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
    
    animateNode = CSLoader::createNode(TASK_BOUND_UI);
    animateNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    animateNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(animateNode);
    
    root = animateNode->getChildByName("ms_bg");
    auto closeBtn = (Button*)root->getChildByName("close_btn");
    closeBtn->addClickEventListener(CC_CALLBACK_1(TaskBoundLayer::clickClose, this));
    
    return true;
}

void TaskBoundLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_TASK_BOUND info = *static_cast<L2E_SHOW_TASK_BOUND*>(event->getUserData());
    auto boxOpenSpt = root->getChildByName("box_open");
    auto boxCloseSpt = root->getChildByName("box_close");
    boxOpenSpt->setVisible(info.taskState == TASK_BOUND);
    boxCloseSpt->setVisible(info.taskState == TASK_OPEN);
    
    auto boundGoldLabel = (TextAtlas*)root->getChildByName("bound_gold_label");
    boundGoldLabel->setString(Convert2String(info.boundGold));
    auto boundDiamondLabel = (TextAtlas*)root->getChildByName("bound_diamond_label");
    boundDiamondLabel->setString(Convert2String(info.boundDiamond));
    
    auto takeBtn = (Button*)root->getChildByName("take_btn");
    takeBtn->setVisible(info.taskState == TASK_BOUND);
    takeBtn->addClickEventListener(CC_CALLBACK_1(TaskBoundLayer::clickTake, this));

    for (int i = 0; i < MAX_TASK_COUNT; i++) {
        auto bg = root->getChildByName("fish_term_"+Convert2String(i+1));
        auto fishCountLabel = (TextAtlas*)bg->getChildByName("count_label");
        fishCountLabel->setString(Convert2String(info.fishMaxCount[i]));
        auto subGoldLabel = (TextAtlas*)bg->getChildByName("bound_gold_label");
        subGoldLabel->setString(Convert2String(info.fishGold[i]));
        auto finishSpt = bg->getChildByName("finished");
        finishSpt->setVisible(info.complete[i]);
        
        std::string resDir("game_scene/fish/small_fish_alive/");
        int fishType = info.fishType[i];
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
        resDir += info.fishRes[i];
        resDir += "_0.png";
        auto fishSpt = (Sprite*)bg->getChildByName("fish_img");
        fishSpt->initWithFile(resDir);
        fishSpt->setScale(30/(float)fishSpt->getContentSize().height);
    }
}

void TaskBoundLayer::clickTake(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_TAKE_BOSS_BOUND info;
    info.eProtocol = e2l_take_task_bounds;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void TaskBoundLayer::clickClose(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_HIDE_TASK_BOUND info;
    info.eProtocol = e2l_hide_task_bounds;
    ClientLogic::instance()->ProcessUIRequest(&info);
//    removeFromParent();
}
