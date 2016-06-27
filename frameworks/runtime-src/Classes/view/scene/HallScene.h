//
//  HallScene.h
//  buyu_simple
//
//  Created by ff on 15/12/9.
//
//

#ifndef __buyu_simple__HallScene__
#define __buyu_simple__HallScene__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ELProtocol.h"
#include "../gameLayers/DiaryLayer.h"
#include "../gameLayers/GoldRechargeLayer.h"
#include "../gameLayers/DiamondRechargeLayer.h"
#include "../gameLayers/VipIntroLayer.h"
#include "../gameLayers/VipGiftLayer.h"
#include "../gameLayers/TipLayer.h"
#include "../gameLayers/BenefitLayer.h"
#include "../gameLayers/ShareLayer.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;


class HallScene : public cocos2d::Scene
{
public:
    ~HallScene();
    
    bool init();
    
    void setupView(L2E_TO_HALL &info);
    
    void start(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void startStage(int index);
    
    void quickStart();
    
    void clickDiary();
    
    void showDiary(EventCustom *event);
    
    void switchMenu();

    void clickExit();
    
    void clickSound();
    
    void clickService();
    
    void removeService();
    
    void removeSound();
    
    void switchSound();
    
    void switchMusic();
    
    void clickRecharge();
    
    void showRecharge(cocos2d::EventCustom* event);
    
    void removeRecharge();
    
    void showVipIntro(cocos2d::EventCustom* event);
    
    void showVipGift();
    
    void clickVip();
    
    void updateMoney(cocos2d::EventCustom* event);
    void rechargeOk(cocos2d::EventCustom* event);
    
    void clickBenefit();
    void showBenefits(cocos2d::EventCustom* event);
    void cashBenefitCode(cocos2d::EventCustom* event);
    
    void clickFreeGold();
    void showFreeGold(cocos2d::EventCustom* event);
    void updateShareResult(cocos2d::EventCustom* event);
    void takeShareCountBound(cocos2d::EventCustom* event);
    
    CREATE_FUNC(HallScene);
private:
    int focus;
    int quickStage;
    bool openMenu;
    
    cocos2d::Node* topRoot;
    cocos2d::Node* leftRoot;
    cocos2d::Node* rightRoot;
    cocos2d::Node* bottomRoot;
    cocos2d::ui::Button* quickStartBtn;
    cocos2d::ui::ScrollView* stageScroll;
    cocos2d::Sprite* menuBg;
    cocos2d::LayerColor* serviceLayer;
    cocos2d::LayerColor* soundLayer;
    cocos2d::Node* soundRoot;
    
    GoldRechargeLayer* rechargeLayer;
    TipLayer* tipLayer;
    
    cocostudio::timeline::ActionTimeline* rightAction;
    cocos2d::EventListenerCustom* showDiaryListener;
    cocos2d::EventListenerCustom* showRechargeListener;
    cocos2d::EventListenerCustom* showVipIntroListener;
    cocos2d::EventListenerCustom* showVipGiftListener;
    cocos2d::EventListenerCustom* updateMoneyListener;
    cocos2d::EventListenerCustom* rechargeOkListener;
    
    cocos2d::EventListenerCustom* showBenefitsListener;
    cocos2d::EventListenerCustom* cashBenefitCodeListener;
    
    cocos2d::EventListenerCustom* showFreeGoldListener;
    cocos2d::EventListenerCustom* updateShareResultListener;
    cocos2d::EventListenerCustom* takeShareCountBoundListener;
};

#endif /* defined(__buyu_simple__HallScene__) */
