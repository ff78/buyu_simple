//
//  LightNode.h
//  buyu_simple
//
//  Created by ff on 16/3/22.
//
//

#ifndef LightNode_h
#define LightNode_h

#include "cocos2d.h"

class LightNode : public cocos2d::Node {
public:
    bool init();
    
    CREATE_FUNC(LightNode);
    
    void setupView();
    
    void onRemove();
private:
    cocos2d::Node* animateNode;
    
    CC_SYNTHESIZE(float, angle, Angle);
    CC_SYNTHESIZE(float, distance, Distance);
};

#endif /* LightNode_h */
