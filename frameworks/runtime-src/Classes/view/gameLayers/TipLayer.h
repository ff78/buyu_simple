//
//  TipLayer.h
//  buyu_simple
//
//  Created by ff on 16/4/7.
//
//

#ifndef TipLayer_h
#define TipLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../node/DelayLabelTip.h"

class TipLayer : public cocos2d::Layer
{
public:
    TipLayer();
    ~TipLayer();
    
    bool init();
    
    void addTaskTip();
    
    void addLeveupTip();
    
    void addUnlockTip();
    
    void addKillBossTip();
    
    void addDisableSkillTip();
    
    void addLackGoldTip();
    
    void addRayNeedVipTip(int vipLevel, DelayLabelTip::fp &f);
    
    void addAutoFireNeedVipTip(int vipLevel, DelayLabelTip::fp &f);
    
    void addRechargeOkTip();
    
    void addBenefitCodeError();
    
    void addBenefitCodeUsed();
    
    void addShareErrorTip();
    
    void addQQUninstallTip();
    
    void addBombNeedUnlockTip();
    
    void addSkillNeedDiamondTip(DelayLabelTip::fp &f);
    
    void addUnlockNeedDiamondTip(DelayLabelTip::fp &f);
    
    CREATE_FUNC(TipLayer);
private:
    cocos2d::EventListenerCustom *addTaskTipListener;
    cocos2d::EventListenerCustom *addLevelupTipListener;
    cocos2d::EventListenerCustom *addUnlockTipListener;
    cocos2d::EventListenerCustom *addKillBossTipListener;
    cocos2d::EventListenerCustom *disableSkillListener;
    
};

#endif /* TipLayer_h */
