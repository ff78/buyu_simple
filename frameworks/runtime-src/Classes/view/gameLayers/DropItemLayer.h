//
//  DropItemLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/16.
//
//

#ifndef DropItemLayer_h
#define DropItemLayer_h

#include "cocos2d.h"

class DropItemLayer : public cocos2d::Layer
{
public:
    ~DropItemLayer();
    
    bool init();
    
    void addDropItem(cocos2d::EventCustom *event);
    
    CREATE_FUNC(DropItemLayer);
private:
    cocos2d::EventListenerCustom *addDropItemListener;
};

#endif /* DropItemLayer_h */
