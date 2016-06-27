//
//  ChangeWeaponLayer.h
//  buyu_simple
//
//  Created by ff on 16/1/26.
//
//

#ifndef ChangeWeaponLayer_h
#define ChangeWeaponLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ChangeWeaponLayer : public cocos2d::LayerColor
{
public:
    bool init();
    
    CREATE_FUNC(ChangeWeaponLayer);
    
    void clickExit(cocos2d::Ref* pSender);
    
    void clickCannon(cocos2d::Ref* pSender);
    
    void clickOk(cocos2d::Ref* pSender);
    
    void exit();
    
    void updateCannonBtn();
private:
    cocos2d::Layer* weaponUI;
    cocos2d::Sprite* chooseFlag;
    
    int focus;
    cocostudio::timeline::ActionTimeline* actionTimeLine;
};

#endif /* ChangeWeaponLayer_h */
