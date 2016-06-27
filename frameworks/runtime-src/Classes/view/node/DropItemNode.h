//
//  DropItemNode.h
//  buyu_simple
//
//  Created by ff on 16/5/16.
//
//

#ifndef DropItemNode_h
#define DropItemNode_h

#include "cocos2d.h"

class DropItemNode : public cocos2d::Node
{
public:
    bool init();
    
    void setupView(cocos2d::EventCustom *event);
    
    void removeOff();
    
    CREATE_FUNC(DropItemNode);
    
private:
    cocos2d::Sprite *itemSpt;
    int increaceId;
};

#endif /* DropItemNode_h */
