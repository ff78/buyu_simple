//
//  TipEffect.cpp
//  buyu_simple
//
//  Created by ff on 16/4/7.
//
//

#include "TipEffect.h"
#include "utils/GameUtils.h"
#include "GameDef.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool TipEffect::init()
{
    if (!Node::init()) {
        return false;
    }
    

    
    return true;
}

void TipEffect::setupView(std::string imgPath)
{
    root = CSLoader::createNode(TIP_EFFECT);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(0, 100));
    addChild(root);
    
    auto titleSpt = (Sprite*)root->getChildByName("show_title");
    titleSpt->initWithFile(imgPath);
    
    uiAction = CSLoader::createTimeline(TIP_EFFECT);
    root->runAction(uiAction);
    uiAction->play("play", false);
    uiAction->setLastFrameCallFunc(CC_CALLBACK_0(TipEffect::onRemove, this));
    
    starParticle = (ParticleSystemQuad*)root->getChildByName("star_particle");
    
    
}

void TipEffect::onRemove()
{
    removeFromParent();
}
