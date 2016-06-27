//
//  BenefitLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/31.
//
//

#ifndef BenefitLayer_h
#define BenefitLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class BenefitLayer : public cocos2d::LayerColor
{
public:
    ~BenefitLayer();
    
    bool init();
    
    void removeOff();
    
    void setupView(cocos2d::EventCustom* event);
    
    void clickJumpBtn();
    
    void clickLotteryBtn();
    
    void clickLinkBtn();
    
    CREATE_FUNC(BenefitLayer);
    
private:
    cocos2d::Sprite* root;
};

#endif /* BenefitLayer_h */
