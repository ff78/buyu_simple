//
//  DropItemNode.cpp
//  buyu_simple
//
//  Created by ff on 16/5/16.
//
//

#include "DropItemNode.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"

USING_NS_CC;

bool DropItemNode::init()
{
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

void DropItemNode::setupView(cocos2d::EventCustom *event)
{
    L2E_DROP_ITEM info = *static_cast<L2E_DROP_ITEM*>(event->getUserData());
    increaceId = info.dropIncreaceId;
    switch(info.dropType)
    {
        case 0:
        {
            itemSpt = Sprite::create("game_scene/gamer_ui/user/user_diamond.png");
            addChild(itemSpt);
        }
            break;
        case 1:
        {
            std::string skillCardStr("common/element/element_");
            skillCardStr += Convert2String(info.dropId+1);
            skillCardStr += ".png";
            itemSpt = Sprite::create(skillCardStr);
            addChild(itemSpt);
        }
            break;
    }
    
    auto zoomIn = ScaleTo::create(0.1, 1.3);
    auto showTime = DelayTime::create(2);
    float distance = getPosition().getDistance(Vec2(GameUtils::winSize.width/2, 60));
    float duration = 0.2*distance/200;
    duration = MAX(0.2, duration);
    auto zoomOut = ScaleTo::create(duration, 0.1);
    auto move2Cannon = MoveTo::create(duration, Vec2(GameUtils::winSize.width/2, 60));
    
    auto func = CallFunc::create(std::bind(&DropItemNode::removeOff, this));
    
    auto spawnAction = Spawn::create(zoomOut, move2Cannon, NULL);
    auto seq = Sequence::create(zoomIn, showTime, spawnAction, func, NULL);
    runAction(seq);
}

void DropItemNode::removeOff()
{
    E2L_RECEIVE_DROP_ITEM info;
    info.eProtocol = e2l_receive_drop_item;
    info.dropIncreaceId = increaceId;
    ClientLogic::instance()->ProcessUIRequest(&info);
    removeFromParent();
}
