//
//  DiaryLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/12.
//
//

#ifndef DiaryLayer_h
#define DiaryLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class DiaryLayer : public cocos2d::LayerColor
{
public:
    bool init();
    
    void setupView(cocos2d::EventCustom *event);
    
    void closeDiary();
    
    CREATE_FUNC(DiaryLayer);
private:
    cocos2d::Node *root;
};

#endif /* DiaryLayer_h */
