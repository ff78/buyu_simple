//
//  GoldRechargeLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/26.
//
//

#ifndef GoldRechargeLayer_h
#define GoldRechargeLayer_h

#include "cocos2d.h"

class GoldRechargeLayer : public cocos2d::LayerColor {
public:
    ~GoldRechargeLayer();
    
    bool init();
    
    void setupView(cocos2d::EventCustom* event);
    
    void updateView(cocos2d::EventCustom* event);
    
    void removeOff();
    
    void clickDiamond();
    
    void clickVipIntro();
    
    void clickRechargeButton(cocos2d::Ref *pSender);
    
    void clickClose();
    
    CREATE_FUNC(GoldRechargeLayer);
    
    
private:
    cocos2d::Node* root;
    cocos2d::EventListenerCustom* updateViewListener;
    cocos2d::EventListenerCustom* closeListener;
};

#endif /* GoldRechargeLayer_h */
