//
//  GoldRunnerScene.h
//  buyu_simple
//
//  Created by ff on 16/1/6.
//
//

#ifndef GoldRunnerScene_h
#define GoldRunnerScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace cocostudio::timeline;

class GoldRunnerScene : public cocos2d::Scene
{
public:
    ~GoldRunnerScene();
    
    bool init();
    
    CREATE_FUNC(GoldRunnerScene);
    
    void endOfStart();
    
    void stopRunner();
    
    void getThing();
    
    void clickStartRunner(cocos2d::Ref* pSender);
    void clickStartGame(Ref* pSender);
    void clickHelp(Ref* pSender);
    
    bool clickHelpScreen(cocos2d::Touch *touch, cocos2d::Event *evnt);

    void updateData(cocos2d::EventCustom* event);
    void openBox(EventCustom* event);
    void startRoll(EventCustom* event);
//    void startGame(EventCustom* event);
    
    void clickOpenBox(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void clickExit(Ref *pSender, Widget::TouchEventType type);
private:
    cocostudio::timeline::ActionTimeline* runnerAction;
    
    float runDuration;
    
    cocos2d::Node *node;
    
    int randCount;
    
    cocos2d::ui::Button* startRunBtn;
    
    EventListenerCustom* updateListener;
    EventListenerCustom* openBoxListener;
    EventListenerCustom* rollListener;
    
    Layer* helpLayer;
};

#endif /* GoldRunnerScene_h */
