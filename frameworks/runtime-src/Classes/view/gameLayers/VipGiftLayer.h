//
//  VipGiftLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#ifndef VipGiftLayer_h
#define VipGiftLayer_h

#include "cocos2d.h"

class VipGiftLayer : public cocos2d::LayerColor {
public:
    ~VipGiftLayer();
    
    bool init();
    
    void removeOff();
    
    void clickRecharge();
    
    void clickClose();
    
    CREATE_FUNC(VipGiftLayer);
private:
    cocos2d::Node* root;
    
    cocos2d::EventListenerCustom* rechargeOkListener;
    cocos2d::EventListenerCustom* closeListener;
};

#endif /* VipGiftLayer_h */
