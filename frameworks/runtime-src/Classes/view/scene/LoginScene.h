//
//  LoginScene.h
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#ifndef __buyu_simple__LoginScene__
#define __buyu_simple__LoginScene__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class LoginScene : public cocos2d::Scene
{
public:
    bool init();
    CREATE_FUNC(LoginScene);
    
    void start(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void failTip();
    
    void update(float dt);
    
    cocos2d::EventListenerCustom *loginFailed;
};

#endif /* defined(__buyu_simple__LoginScene__) */
