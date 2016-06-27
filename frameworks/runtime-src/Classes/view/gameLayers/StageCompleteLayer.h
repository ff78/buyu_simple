//
//  StageCompleteLayer.h
//  buyu_simple
//
//  Created by ff on 16/4/5.
//
//

#ifndef StageCompleteLayer_h
#define StageCompleteLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ELProtocol.h"

class StageCompleteLayer : public cocos2d::LayerColor
{
public:
    bool init();
    
    void setupView(cocos2d::EventCustom *event);
    
    void clickOk(cocos2d::Ref *ref);
    CREATE_FUNC(StageCompleteLayer);
    
private:
    cocos2d::Node *dialogNode;
};
#endif /* StageCompleteLayer_h */
