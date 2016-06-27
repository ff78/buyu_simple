//
//  DelayLabelTip.cpp
//  buyu_simple
//
//  Created by ff on 16/4/27.
//
//

#include "DelayLabelTip.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameDef.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

bool DelayLabelTip::init()
{
    if (!Node::init()) {
        return false;
    }

    return true;
}

void DelayLabelTip::setupView(fp *delayFunc)
{
    root = CSLoader::createNode(COMMON_TIP_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(Vec2(0, 50));
    addChild(root);
    
    auto label1 = (Text*)root->getChildByName("Text_1");
    if (text1 == "") {
        label1->setVisible(false);
    }else{
        label1->setString(text1);
    }
    
    auto label2 = (Text*)root->getChildByName("Text_2");
    if (text2 == "") {
        label2->setVisible(false);
    }else{
        label2->setString(text2);
    }
    
    auto btn = (Text*)root->getChildByName("ButtonOk");
    btn->setVisible(false);
    
    auto delay = DelayTime::create(delayDuration);
    auto func = CallFunc::create(CC_CALLBACK_0(DelayLabelTip::removeOff, this));
    Sequence *seq;
    if (delayFunc != nullptr) {
        seq = Sequence::create(delay, CallFunc::create(*delayFunc), func, NULL);
    }else{
        seq = Sequence::create(delay, func, NULL);
    }
    runAction(seq);
}

void DelayLabelTip::removeOff()
{
    removeFromParent();
}
