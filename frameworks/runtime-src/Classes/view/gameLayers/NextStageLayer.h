//
//  NextStageLayer.hpp
//  buyu_simple
//
//  Created by ff on 16/4/5.
//
//

#ifndef NextStageLayer_hpp
#define NextStageLayer_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class NextStageLayer : public cocos2d::LayerColor {
public:
    bool init();
    
    void clickNext(cocos2d::Ref *ref);
    void clickCancel(cocos2d::Ref *ref);
    CREATE_FUNC(NextStageLayer);
    
};

#endif /* NextStageLayer_hpp */
