//
//  EvaluateLayer.h
//  buyu_simple
//
//  Created by ff on 16/6/28.
//
//

#ifndef EvaluateLayer_h
#define EvaluateLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class EvaluateLayer : public cocos2d::LayerColor
{
public:
    bool init();
    
    void removeOff();
    
    void clickOk();
    
    CREATE_FUNC(EvaluateLayer);
private:
    CC_SYNTHESIZE_READONLY(cocos2d::Node*, root, Root);
};
#endif /* EvaluateLayer_h */
