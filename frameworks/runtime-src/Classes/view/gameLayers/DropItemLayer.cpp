//
//  DropItemLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/5/16.
//
//

#include "DropItemLayer.h"
#include "EventDef.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"
#include "../node/DropItemNode.h"

USING_NS_CC;

DropItemLayer::~DropItemLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(addDropItemListener);
}

bool DropItemLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    addDropItemListener = EventListenerCustom::create(UI_DROP_ITEM, CC_CALLBACK_1(DropItemLayer::addDropItem, this) );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addDropItemListener, -1);
    return true;
}

void DropItemLayer::addDropItem(cocos2d::EventCustom *event)
{
    L2E_DROP_ITEM info = *static_cast<L2E_DROP_ITEM*>(event->getUserData());
    int count = MIN(10, info.dropCount);
    
    for (int i = 0;  i < count; i++) {
        auto item = DropItemNode::create();
        item->setPosition(Vec2(info.posX+rand()%50, info.posY+rand()%50));
        item->setupView(event);
        addChild(item);
    }
    
}