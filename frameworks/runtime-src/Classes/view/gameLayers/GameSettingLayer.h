//
//  GameSettingLayer.h
//  buyu_simple
//
//  Created by ff on 16/4/1.
//
//

#ifndef GameSettingLayer_h
#define GameSettingLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameSettingLayer : public cocos2d::LayerColor
{
public:
    ~GameSettingLayer();
    bool init();
    
    CREATE_FUNC(GameSettingLayer);

    
    void clickExit();
    
    void clickQuit();
    
    void clickWeapon();
    
    void clickFish();
    
    void clickMusic();
    
    void clickSound();
    
    void showFishSetting(cocos2d::EventCustom *event);
    
    void showCannonSetting(cocos2d::EventCustom *event);
    
private:
    cocos2d::Node *animationNode;
    cocos2d::ui::Button *soundSwitch;
    cocos2d::ui::Button *musicSwitch;
    
    cocos2d::EventListenerCustom *fishSettingListener;
    cocos2d::EventListenerCustom *cannonSettingListener;

};

#endif /* GameSettingLayer_h */
