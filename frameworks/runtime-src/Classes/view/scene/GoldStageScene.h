//
//  GoldStageScene.h
//  buyu_simple
//
//  Created by ff on 16/1/6.
//
//

#ifndef GoldStageScene_h
#define GoldStageScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace cocostudio::timeline;

class GoldStageScene : public Scene
{
public:
    ~GoldStageScene();
    
    bool init();
    
    CREATE_FUNC(GoldStageScene);
    
    void updateData(EventCustom* event);
    void openBox(EventCustom* event);
    
    void clickStageBtn(Ref *pSender, Widget::TouchEventType type);
    
    void clickOpenBox(Ref *pSender, Widget::TouchEventType type);

    void clickExit(Ref *pSender, Widget::TouchEventType type);
private:
    EventListenerCustom* updateListener;
    EventListenerCustom* openBoxListener;
    Node* root;
};

#endif /* GoldStageScene_h */
