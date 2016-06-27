//
//  TipEffect.h
//  buyu_simple
//
//  Created by ff on 16/4/7.
//
//

#ifndef TipEffect_h
#define TipEffect_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class TipEffect : public cocos2d::Node
{
public:
    bool init();
    
    void setupView(std::string imgPath);
    
    void onRemove();
    
    CREATE_FUNC(TipEffect);
    
private:
    cocos2d::ParticleSystemQuad *starParticle;
    cocos2d::Node *root;
    cocostudio::timeline::ActionTimeline *uiAction;
};

#endif /* TipEffect_h */
