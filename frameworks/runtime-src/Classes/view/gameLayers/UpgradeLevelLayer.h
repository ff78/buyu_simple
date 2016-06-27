//
//  UpgradeLevelLayer.h
//  buyu_simple
//
//  Created by ff on 16/3/29.
//
//

#ifndef UpgradeLevelLayer_h
#define UpgradeLevelLayer_h

#include "cocos2d.h"
#include "ELProtocol.h"

class UpgradeLevelLayer : public cocos2d::LayerColor {
public:
    ~UpgradeLevelLayer();
    bool init();
    CREATE_FUNC(UpgradeLevelLayer);
    
    void setupView(cocos2d::EventCustom *event);
    
//    bool onTouchCardBegan(cocos2d::Touch *touch, cocos2d::Event *event);
//    void onTouchCard(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void clickUpgrade(cocos2d::Ref *pSender);
    void clickClose(cocos2d::Ref *pSender);
//    void clickDialogOk(cocos2d::Ref *pSender);
    
    void unlockSuccess(cocos2d::EventCustom* event);
private:
    cocos2d::Node *card[5];
    cocos2d::Node *bg;
    
    cocos2d::Node *tipNode;
    
    cocos2d::EventListenerCustom* unlockSuccessListener;
};

#endif /* UpgradeLevelLayer_h */
