//
//  LoadGoldScene.h
//  buyu_simple
//
//  Created by ff on 15/12/22.
//
//

#ifndef LoadGoldScene_h
#define LoadGoldScene_h

#include "cocos2d.h"

class LoadGoldScene : public cocos2d::Scene
{
public:
    ~LoadGoldScene();
    
    bool init();
    
//    void onExit();
    
    CREATE_FUNC(LoadGoldScene);
    
    void update(float dt);
    
    void loadUpdate(cocos2d::EventCustom* event);
private:
    cocos2d::EventListenerCustom *updateListener;
};

#endif /* LoadGoldScene_h */
