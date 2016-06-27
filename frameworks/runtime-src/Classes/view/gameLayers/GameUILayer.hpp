//
//  GameUILayer.hpp
//  buyu_simple
//
//  Created by ff on 16/1/26.
//
//

#ifndef GameUILayer_hpp
#define GameUILayer_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class GameUILayer : public cocos2d::Layer
{
public:
    GameUILayer();
    ~GameUILayer();
    
    bool init();
    
    void update(float dt);
    
    CREATE_FUNC(GameUILayer);
    
    void clickWeapon(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void clickAuto(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void clickLock(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void clickIce(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void clickRush(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void clickRay(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void clickBomb(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
    
    void clickSkillBtn(cocos2d::ui::Button* btn, float duration);
    
    void clickHide(Ref* pSender);
    void clickShow(Ref* pSender);
    void clickSetting(Ref *pSender);
    void clickSuc(Ref *pSender);
    void clickShowTask(Ref *pSender);
    void clickShowUnlock(Ref *pSender);
    void clickLotteryBox(Ref *pSender);
    void clickRechargeBtn();
    
    void reopenLockButton();
    void reopenButton(Ref* pSender);
    
    void readyRay();
    void endRay();
    
    void readyPlane();
    void endPlane();
    
    inline void hideUpgrade(){
        CannonUpgrade->setVisible(false);
    };
    
    void useSkill(cocos2d::EventCustom* event);
    void updateSkillCount(cocos2d::EventCustom* event);
    void updateUpgrade(cocos2d::EventCustom* event);
    void unlockUpgrade(cocos2d::EventCustom* event);
    void updateGold(cocos2d::EventCustom* event);
    void updateDiamond(cocos2d::EventCustom* event);
    void updateEnergy(cocos2d::EventCustom* event);
    void updateHeadinfo(cocos2d::EventCustom* event);
    void updateBossTask(cocos2d::EventCustom* event);
    void initBossTask(cocos2d::EventCustom* event);
    
    void initTask(cocos2d::EventCustom* event);
    void updateTask(cocos2d::EventCustom* event);
    
    void updateStarBox(cocos2d::EventCustom* event);
    
    bool onUpgradeClickBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onUpgradeClickEnd(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void showSkillCharge(cocos2d::EventCustom* event);
    void clickChargeOk(cocos2d::Ref *pSender);
    void hideSkillCharge(cocos2d::EventCustom* event);
    
    void useAutoFire();
    void setAutoFire(bool bAutoFire);
private:
    cocos2d::Node* MenuLayer;
    cocos2d::Node* CannonUpgrade;
    cocos2d::Node* BottomLayer;
    cocos2d::ui::LoadingBar* energyBar;
    cocos2d::Node* HeadInfoLayer;
    cocos2d::ui::LoadingBar* expBar;
    cocos2d::Node* HeadBg;
    cocos2d::Node* bossTaskBg;
    
    cocos2d::Node* TopLayer;
    
    cocos2d::Node* tipNode;
    
    
    cocostudio::timeline::ActionTimeline *uiAction;
    cocostudio::timeline::ActionTimeline *sucAction;
    cocostudio::timeline::ActionTimeline *taskBoundAction;
    cocostudio::timeline::ActionTimeline *leftMenuAction;
    
    cocos2d::ui::Button *lockSkillBtn;
    cocos2d::ui::Button *iceSkillBtn;
    cocos2d::ui::Button *rushSkillBtn;
    cocos2d::ui::Button *raySkillBtn;
    cocos2d::ui::Button *bombSkillBtn;
    cocos2d::ui::Button *settingBtn;
    cocos2d::ui::Button *bossTaskBtn;
    cocos2d::ui::Button *showTaskBtn;
    cocos2d::ui::Button *showTaskBoundBtn;
    cocos2d::ui::Button *showUnlockBtn;
    cocos2d::ui::Button *lotteryBtn;
    
    cocos2d::Sprite *starSprite[5];
    
    cocos2d::EventListenerCustom *upgradeUpdateListener;
    cocos2d::EventListenerCustom *upgradeUnlockListener;
    cocos2d::EventListenerCustom *upgradeHideListener;
    cocos2d::EventListenerCustom *updateGoldListener;
    cocos2d::EventListenerCustom *updateDiamondListener;
    cocos2d::EventListenerCustom *updateEnergyListener;
    cocos2d::EventListenerCustom *updateHeadinfoListener;
    cocos2d::EventListenerCustom *updateBossTaskListener;
    cocos2d::EventListenerCustom *initBossTaskListener;
    cocos2d::EventListenerCustom *initTaskListener;
    cocos2d::EventListenerCustom *updateTaskListener;
    cocos2d::EventListenerCustom *updateSkillListener;
    cocos2d::EventListenerCustom *useSkillListener;
//    cocos2d::EventListenerCustom *showSkillChargeListener;
    cocos2d::EventListenerCustom *hideSkillChargeListener;
    cocos2d::EventListenerCustom *updateStarBoxListener;
    cocos2d::EventListenerCustom *canUseAutoFireListener;
    
    int chargeSkillId;
    CC_SYNTHESIZE_READONLY(bool, autoFire, AutoFire);
    CC_SYNTHESIZE(bool, readyRayFire, ReadyRayFire);
    CC_SYNTHESIZE(bool, rayFiring, RayFiring);
    CC_SYNTHESIZE(bool, readyPlaneFire, ReadyPlaneFire);
    CC_SYNTHESIZE(bool, planeFiring, PlaneFiring);
    CC_SYNTHESIZE_READONLY(bool, canUnlock, CanUnlock);
    CC_SYNTHESIZE_READONLY(float, hideTick, HideTick);

};

#endif /* GameUILayer_hpp */
