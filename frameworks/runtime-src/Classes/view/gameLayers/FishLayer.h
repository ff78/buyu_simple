//
//  FishLayer.h
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#ifndef __buyu_simple__FishLayer__
#define __buyu_simple__FishLayer__

#include "cocos2d.h"
#include "../Share/Header/EventDef.h"
#include "../Share/Header/ELProtocol.h"
#include "../node/FishNode.h"

class FishLayer : public cocos2d::Layer
{
public:
    FishLayer();
    ~FishLayer();
    
    bool init();
    
    CREATE_FUNC(FishLayer);
    
    bool createFish(cocos2d::EventCustom* event);
    
    void update(float dt);
    
    void clearLockFish();
    FishNode* lockFish;
    Vector<FishNode*> lightFishes;
private:

    cocos2d::EventListenerCustom* createListener;
    cocos2d::EventListenerCustom* clearLockListener;
    CC_SYNTHESIZE_READONLY(int, lockFishId, LockFishId);
};

#endif /* defined(__buyu_simple__FishLayer__) */
