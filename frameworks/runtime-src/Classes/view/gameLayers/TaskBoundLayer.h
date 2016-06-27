//
//  TaskBoundLayer.hpp
//  buyu_simple
//
//  Created by ff on 16/4/6.
//
//

#ifndef TaskBoundLayer_hpp
#define TaskBoundLayer_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class TaskBoundLayer : public cocos2d::LayerColor {
public:
    bool init();
    
    void setupView(cocos2d::EventCustom *event);
    
    void clickTake(cocos2d::Ref *pSender);
    void clickClose(cocos2d::Ref *pSender);
    
    CREATE_FUNC(TaskBoundLayer);
private:
    cocos2d::Node* animateNode;
    cocos2d::Node* root;
};

#endif /* TaskBoundLayer_hpp */
