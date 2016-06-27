//
//  NetNode.h
//  buyu_simple
//
//  Created by ff on 16/1/22.
//
//

#ifndef NetNode_h
#define NetNode_h

#include "cocos2d.h"

class NetNode : public cocos2d::Node
{
public:
    bool init();
    
    CREATE_FUNC(NetNode);
    
    void setupView();
    
    void onRemove();
    
private:
    cocos2d::Node* animateNode;
    cocos2d::Sprite* spt;
    
    CC_SYNTHESIZE(int, cannonType, CannonType);
    CC_SYNTHESIZE(int, bulletCount, bulletCount);
};

#endif /* NetNode_h */
