//
//  ShowFishLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/18.
//
//

#ifndef ShowFishLayer_h
#define ShowFishLayer_h

#include "cocos2d.h"

class ShowFishLayer : public cocos2d::LayerColor {
public:
    bool init();
    
    void setupView(cocos2d::EventCustom *event);
    
    void closeShowFish();
    
    CREATE_FUNC(ShowFishLayer);
private:
    cocos2d::Node *root;
};

#endif /* ShowFishLayer_h */
