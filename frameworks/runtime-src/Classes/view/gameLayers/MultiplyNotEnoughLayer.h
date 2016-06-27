//
//  MultiplyNotEnoughLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/24.
//
//

#ifndef MultiplyNotEnoughLayer_h
#define MultiplyNotEnoughLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class MultiplyNotEnoughLayer : public cocos2d::LayerColor
{
public:
    bool init();
    
    void setupView(cocos2d::EventCustom *event);
    
    void clickClose();
    
    void clickOk();
    
    CREATE_FUNC(MultiplyNotEnoughLayer);
private:
    cocos2d::Node *root;
};

#endif /* MultiplyNotEnoughLayer_h */
