//
//  ChangeCannonLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/18.
//
//

#ifndef ChangeCannonLayer_h
#define ChangeCannonLayer_h

#include "cocos2d.h"

class ChangeCannonLayer : public cocos2d::LayerColor {
public:
    ~ChangeCannonLayer();
    
    bool init();
    
    void setupView(cocos2d::EventCustom  *event);
    
    void updateView(cocos2d::EventCustom *event);
    
    void clickClose();
    
    void clickOk();
    
    void clickCannon(cocos2d::Ref *pSender);
    
    void upgradeVip();
    
    void removeConfirm();
    
    CREATE_FUNC(ChangeCannonLayer);
private:
    cocos2d::Node *root;
    
    int focus;
    
    bool hold[9];
    
    cocos2d::EventListenerCustom *updateListener;
    cocos2d::LayerColor *confirmLayer;
};

#endif /* ChangeCannonLayer_h */
