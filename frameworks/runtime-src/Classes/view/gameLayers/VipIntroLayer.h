//
//  VipIntroLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#ifndef VipIntroLayer_h
#define VipIntroLayer_h

#include "cocos2d.h"

class VipIntroLayer : public cocos2d::LayerColor {
public:
    ~VipIntroLayer();
    
    bool init();
    
    void setupView(cocos2d::EventCustom* event);
    
    void updateView(cocos2d::EventCustom* event);
    
    void removeOff();
    
    void clickGift();
    
    void clickGold();
    
    void clickClose();
    
    CREATE_FUNC(VipIntroLayer);
private:
    cocos2d::Node* root;
//    cocos2d::EventListenerCustom* updateViewListener;
    cocos2d::EventListenerCustom* closeListener;
};

#endif /* VipIntroLayer_h */
