//
//  LightEffectLayer.h
//  buyu_simple
//
//  Created by ff on 16/3/22.
//
//

#ifndef LightEffectLayer_h
#define LightEffectLayer_h

#include "cocos2d.h"

class LightEffectLayer : public cocos2d::Layer {
    
    
public:
    bool init();
    
    CREATE_FUNC(LightEffectLayer);
    
    void addLight(cocos2d::Vec2 pos, float distance, float angle);
    
    void addLightDot(cocos2d::Vec2 pos);
};

#endif /* LightEffectLayer_h */
