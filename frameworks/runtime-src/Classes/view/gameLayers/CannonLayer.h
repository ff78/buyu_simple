//
//  CannonLayer.h
//  buyu_simple
//
//  Created by ff on 16/1/18.
//
//

#ifndef CannonLayer_h
#define CannonLayer_h

#include "cocos2d.h"
#include "../node/CannonNode.h"
#include "../Share/Header/EventDef.h"
#include "../Share/Header/ELProtocol.h"

class CannonLayer : public cocos2d::Layer {
public:
    ~CannonLayer();
    
    bool init();
    
    CREATE_FUNC(CannonLayer);
    
    void heroTowardPoint(cocos2d::Vec2 &fingle);
    
    void questHeroFire();
    
    void questHeroTraceFire(int fishId);
    
    void questHeroBombFire(float bulletX, float bulletY);
    
    void heroFire(cocos2d::EventCustom* event);
    
    void createCannon(cocos2d::EventCustom* event);
    
    void changeCannon(cocos2d::EventCustom* event);
    
    void changeCannonMultiply(cocos2d::EventCustom* event);
private:
    cocos2d::EventListenerCustom* createListener;
    cocos2d::EventListenerCustom* fireListener;
    cocos2d::EventListenerCustom* changeListener;
    cocos2d::EventListenerCustom* mulListener;
    
    CC_SYNTHESIZE_READONLY(CannonNode*, heroCannon, HeroCannon);
};

#endif /* CannonLayer_h */
