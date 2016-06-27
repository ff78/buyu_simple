//
//  CannonNode.h
//  buyu_simple
//
//  Created by ff on 16/1/18.
//
//

#ifndef CannonNode_h
#define CannonNode_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../Share/Header/EventDef.h"
#include "../Share/Header/ELProtocol.h"
#include "cocostudio/CocoStudio.h"

class CannonNode : public cocos2d::Node {
public:
    CannonNode();
    ~CannonNode();
    bool init();
    
    CREATE_FUNC(CannonNode);
    
    void updateRelife(cocos2d::EventCustom* event);
    void updateCannon(cocos2d::EventCustom* event);
    void setupCannon(cocos2d::EventCustom* event);
    void changeCannon(int typeId);
    void changeMultiply(int val);
    void changeBulletCount(int val);
    
    void toward(float angle_in);
    void setLock(bool lock, float dis);
    
    void questFire(int fishId, float bulletX = -2000, float bulletY = -2000);
    
    void fire();
    
    void clickAdd(Ref* pSender);
    void clickMinus(Ref* pSender);
    void hideEffect();
    
    void switchAutoCircle(bool open);
    void switchFireCircle(bool open);
    
    void readyRay();
    void endRay(bool autoFire);
    void hideRay();
    
    void readyPlane();
    void endPlane(bool autoFire);
    void hidePlane();

    
    void takRelife();
private:
    cocos2d::Node* animationNode;
    cocostudio::timeline::ActionTimeline* animateAction;
    cocos2d::Node* energyNode;
    cocostudio::timeline::ActionTimeline* energyAction;
    cocos2d::Sprite* carriage;
    cocos2d::Sprite* foudation;
    cocos2d::Sprite* autoCircle;
    cocos2d::Sprite* fireCircle;
    
    cocos2d::Node* rayEffectNode;
    cocostudio::timeline::ActionTimeline* rayEffectAction;
    
    cocos2d::ui::Button* relifeButton;
    cocos2d::ui::Button* addButton;
    cocos2d::ui::Button* minusButton;
    cocos2d::ui::TextAtlas* multiLabel;
    cocos2d::Sprite* multiBg;
    cocos2d::Sprite* bankRuptcySpt;
    
    cocos2d::ui::TextAtlas* relifeCountLabel;
    cocos2d::ui::TextAtlas* boundLabel;
    
    cocos2d::Node* mulEffectNode;
    cocostudio::timeline::ActionTimeline* effectAction;
    
    cocos2d::Sprite* sdPoint[100];
    cocos2d::EventListenerCustom* clearLockListener;
    cocos2d::EventListenerCustom* startRushListener;
    cocos2d::EventListenerCustom* endRushListener;
    cocos2d::EventListenerCustom* startAutoListener;
    cocos2d::EventListenerCustom* endAutoListener;
    cocos2d::EventListenerCustom* updateRelifeListener;
    
    CC_SYNTHESIZE(int, carriageType, CarriageType);
    CC_SYNTHESIZE(int, cannonType, CannonType);
    CC_SYNTHESIZE(int, multiple, Multiple);
    CC_SYNTHESIZE(int, bulletCount, BulletCount);
    CC_SYNTHESIZE(float, angle, Angle);
    CC_SYNTHESIZE(bool, isHero, IsHero);
    CC_SYNTHESIZE(int, chairId, ChairId);

};

#endif /* CannonNode_h */
