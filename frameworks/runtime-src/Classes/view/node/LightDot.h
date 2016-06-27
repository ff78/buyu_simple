//
//  LightDot.h
//  buyu_simple
//
//  Created by ff on 16/4/18.
//
//

#ifndef LightDot_h
#define LightDot_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class LightDot : public cocos2d::Node {
public:
    bool init();
    
    void setupView();
    
    void onRemove();
    
    CREATE_FUNC(LightDot);
private:
    cocos2d::Node* animateNode;
};

#endif /* LightDot_h */
