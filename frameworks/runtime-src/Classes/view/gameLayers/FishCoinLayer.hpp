//
//  FishCoinLayer.hpp
//  buyu_simple
//
//  Created by ff on 16/2/2.
//
//

#ifndef FishCoinLayer_hpp
#define FishCoinLayer_hpp

#include "cocos2d.h"
#include "../node/FishCoinNode.hpp"
#include "EventDef.h"
#include "GameDef.h"


class FishCoinLayer : public cocos2d::Layer
{
public:
    ~FishCoinLayer();
    bool init();
    
    CREATE_FUNC(FishCoinLayer);
    
    void addFishCoin(cocos2d::EventCustom *event);
    
    void update(float dt);
    
private:
    cocos2d::EventListenerCustom* addCoinListener;
};

#endif /* FishCoinLayer_hpp */
