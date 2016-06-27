//
//  GoldGameScene.h
//  buyu_simple
//
//  Created by ff on 15/12/22.
//
//

#ifndef GoldGameScene_h
#define GoldGameScene_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "data/BlackGoldGame.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#define MAX_ANGLE 75
#define MIN_ANGLE -75
#define ROTATE_DURATION 4

#define EFFECT_TAG 100

class GoldGameScene : public Scene {
public:
    ~GoldGameScene();
    bool init();
    
    CREATE_FUNC(GoldGameScene);
    
    void update(float dt);
    
    void onChangePilers(EventCustom* event);
    void onAddBomb(EventCustom* event);
    void onAddKey(EventCustom* event);
    void onResult(EventCustom* event);
    
    void RopeMovementListener(Armature *, MovementEventType, const std::string &);
    
    bool touchBegan(Touch* touch, Event* evnt);
    void touchScreen(Touch* touch, Event* evnt);
    
    void clickResult(Ref* pSender, Widget::TouchEventType type);
    
    void clickBomb(Ref* pSender, Widget::TouchEventType type);
    
    void removeEffect();
    
    void flipGoldNode(Armature *node);
    
private:
    CC_SYNTHESIZE(int, gold, Gold);
    
    Armature *rope;
    
    Sprite *flagSpt;
    
    EventListenerCustom* changePilersListener;
    EventListenerCustom* addBombListener;
    EventListenerCustom* addKeyListener;
    EventListenerCustom* resultListener;
    
    Vec2 pliersVec;
    
    Layer *objLayer;
    Layer *resultLayer;
    
    std::vector<int> collideObjId;
    
    std::vector<OBJ_INFO> objInfoVec;
    
    TextAtlas* score;
    Label* timeLabel;
    TextAtlas* bombCount;
    
    Button* bombBtn;
    Sprite* keySpt[5];
};

#endif /* GoldGameScene_h */
