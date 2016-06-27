//
//  CaseSplashLayer.h
//  buyu_simple
//
//  Created by ff on 16/4/28.
//
//

#ifndef CaseSplashLayer_h
#define CaseSplashLayer_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ELProtocol.h"

enum CASE_TYPE
{
    NIUNIU,
    MARY
};

class CaseSplashLayer : public cocos2d::LayerColor
{
public:
    bool init();
    
    void setupView();
    
    void removeOff();
    
    void showAnimation();
    
    CREATE_FUNC(CaseSplashLayer);
private:
    cocos2d::ParticleSystemQuad *bubbleParticle;
    cocos2d::Node* root;
    cocostudio::timeline::ActionTimeline* rootAction;
    CC_SYNTHESIZE(CASE_TYPE, splashCaseType, SplashCaseType);
};

#endif /* CaseSplashLayer_h */
