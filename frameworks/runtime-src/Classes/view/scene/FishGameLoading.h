//
//  FishGameLoading.h
//  buyu_simple
//
//  Created by ff on 16/5/10.
//
//

#ifndef FishGameLoading_h
#define FishGameLoading_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class FishGameLoading : public cocos2d::Scene {
public:
    FishGameLoading();
    ~FishGameLoading();
    
    bool init();
    
    CREATE_FUNC(FishGameLoading);
    
    void update(float dt);
    
    void loadUpdate(cocos2d::EventCustom* event);
    
private:
    cocos2d::EventListenerCustom *updateListener;
    cocos2d::ui::LoadingBar* loadBar;
    
};

#endif /* FishGameLoading_h */
