//
//  CaseSplashLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/4/28.
//
//

#include "CaseSplashLayer.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "GameDef.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool CaseSplashLayer::init()
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
    
    splashCaseType = MARY;
    
    return true;
}

void CaseSplashLayer::setupView()
{
    bubbleParticle = ParticleSystemQuad::create(BUBBLE_PARTICLE);
    bubbleParticle->setPosition(GameUtils::winSize.width/2, 0);
    addChild(bubbleParticle);
    
    root = CSLoader::createNode(ENTER_MARY_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    root->setVisible(false);
    addChild(root);
    
    rootAction = CSLoader::createTimeline(ENTER_MARY_UI);
    root->runAction(rootAction);
    
    if (splashCaseType == NIUNIU) {
        auto titleSpt = (Sprite*)root->getChildByName("ge_mary_2");
        titleSpt->initWithFile("game_scene/game_effect/game_effect/ge_niuniu.png");
    }
    
    auto delay = DelayTime::create(2.0f);
    auto func = CallFunc::create(CC_CALLBACK_0(CaseSplashLayer::showAnimation, this));
    auto seq = Sequence::create(delay, func, NULL);
    seq->setTag(20);
    runAction(seq);
}

void CaseSplashLayer::removeOff()
{
    removeFromParent();
    if (splashCaseType == MARY) {
        E2L_MARY_SPLASH_OVER info;
        info.eProtocol = e2l_mary_splash_over;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }else{
        E2L_NIUNIU_SPLASH_OVER info;
        info.eProtocol = e2l_niuniu_splash_over;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
    
}

void CaseSplashLayer::showAnimation()
{
    bubbleParticle->stopSystem();
    stopActionByTag(20);
    root->setVisible(true);
    rootAction->play("play", false);
    rootAction->setLastFrameCallFunc(CC_CALLBACK_0(CaseSplashLayer::removeOff, this));
}
