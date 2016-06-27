//
//  DiamondRechargeLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#ifndef DiamondRechargeLayer_h
#define DiamondRechargeLayer_h

#include "cocos2d.h"

class DiamondRechargeLayer : public cocos2d::LayerColor {
public:
    ~DiamondRechargeLayer();
    
    bool init();
    
    void setupView(cocos2d::EventCustom* event);
    
    void updateView(cocos2d::EventCustom* event);
    
    void removeOff();
    
    void clickGold();
    
    void clickVipIntro();
    
    void clickRechargeButton(cocos2d::Ref *pSender);
    
    void clickClose();
    
    CREATE_FUNC(DiamondRechargeLayer);
private:
    cocos2d::Node* root;
    cocos2d::EventListenerCustom* updateViewListener;
    cocos2d::EventListenerCustom* closeListener;
};

#endif /* DiamondRechargeLayer_h */
