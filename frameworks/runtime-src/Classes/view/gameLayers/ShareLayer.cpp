//
//  ShareLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/6/1.
//
//

#include "ShareLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "GameDef.h"
#include "sound/SoundManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

ShareLayer::~ShareLayer()
{
    
}

bool ShareLayer::init()
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
    
    auto rootNode = CSLoader::createNode(HALL_SHARE_UI);
    rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rootNode->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(rootNode);
    
    root = rootNode->getChildByName("ms_bg");
    auto closeBtn = (Button*)root->getChildByName("close_button");
    closeBtn->addClickEventListener(CC_CALLBACK_0(ShareLayer::removeOff, this));
    
    auto circleBtn = (Button*)root->getChildByName("circle_button");
    circleBtn->addClickEventListener(std::bind(&ShareLayer::clickShare, this, 2));
     auto weixinBtn = (Button*)root->getChildByName("weixin_button");
    weixinBtn->addClickEventListener(std::bind(&ShareLayer::clickShare, this, 1));
//    auto qzoneBtn = (Button*)root->getChildByName("qzone_button");
//    qzoneBtn->addClickEventListener(std::bind(&ShareLayer::clickShare, this, 3));
    
    return true;
}

void ShareLayer::setupView(EventCustom* event)
{
    L2E_SHOW_SHARE info = *static_cast<L2E_SHOW_SHARE*>(event->getUserData());
    auto countText = (TextAtlas*)root->getChildByName("count_text");
    countText->setString(Convert2String(info.shareCount));
    auto bar = (LoadingBar*)root->getChildByName("count_bar_bg")->getChildByName("count_bar");
    
    float percent = 0;
    if (info.shareCount >= gShareCountThres[3]) {
        percent = 100;
    }else{
        for (int i = 0; i < 4; i++) {
            if (gShareCountThres[i] >= info.shareCount) {
                if (i >= 1) {
                    percent = (i-1)*33;
                    percent += (info.shareCount-gShareCountThres[i-1])*33.0/(gShareCountThres[i]-gShareCountThres[i-1]);
                    percent = MIN(100, percent);
                }
                
                break;
            }
        }
    }

    bar->setPercent(percent);
    
    
    for (int i = 0; i < 4; i++) {
        auto countBtn = (Button*)root->getChildByName(("count_button_"+Convert2String(i+1)).c_str());
        switch (info.countState[i]) {
            case 0:
            {
                countBtn->loadTextureDisabled("hall_scene/freegold/freegold_box_0.png");
                countBtn->setEnabled(false);
                countBtn->setTouchEnabled(false);
                countBtn->setBright(false);
            }
                break;
            case 1:
            {
                countBtn->addClickEventListener(std::bind(&ShareLayer::clickCount, this, i+1));
            }
                break;
            case 2:
            {
                countBtn->setEnabled(false);
                countBtn->setTouchEnabled(false);
                countBtn->setBright(false);
            }
                break;
                
            default:
                break;
        }
    }
}


void ShareLayer::removeOff()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    removeFromParent();
}

void ShareLayer::clickShare(int index)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHARE info;
    info.eProtocol = e2l_share;
    info.channalId = index;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ShareLayer::clickCount(int index)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_SHARE_COUNT_BOUND info;
    info.eProtocol = e2l_share_count_bound;
    info.index = index;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
