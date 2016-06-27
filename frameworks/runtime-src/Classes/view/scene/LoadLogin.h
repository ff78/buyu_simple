//
//  LoadLogin.h
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#ifndef __buyu_simple__LoadLogin__
#define __buyu_simple__LoadLogin__

#include "cocos2d.h"

class LoadLogin : public cocos2d::Scene
{
public:
    ~LoadLogin();
    
    bool init();
    
    void onExit();
    
    CREATE_FUNC(LoadLogin);
    
    void update(float dt);
    
    void loadUpdate(cocos2d::EventCustom* event);

private:
    void load1();
    cocos2d::EventListenerCustom *updateListener;
};
#endif /* defined(__buyu_simple__LoadLogin__) */
