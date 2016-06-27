//
//  GoldDifSelScene.h
//  buyu_simple
//
//  Created by ff on 16/1/5.
//
//

#ifndef GoldDifSelScene_h
#define GoldDifSelScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class GoldDifSelScene : public cocos2d::Scene
{
public:
    ~GoldDifSelScene();
    
    bool init();
    CREATE_FUNC(GoldDifSelScene);
    
    void updateData(EventCustom* event);
    void clickDiffBtn(Ref *pSender, Widget::TouchEventType type);
    void clickStartBtn(Ref *pSender, Widget::TouchEventType type);
private:
    int fee;
    int timeLimit;
    int challengeCount;
    int focus;
    std::string feeStr;
    
    cocos2d::Node* root;
    
    EventListenerCustom* updateListener;
};

#endif /* GoldDifSelScene_h */
