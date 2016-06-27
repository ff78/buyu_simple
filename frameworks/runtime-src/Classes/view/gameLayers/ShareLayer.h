//
//  ShareLayer.h
//  buyu_simple
//
//  Created by ff on 16/6/1.
//
//

#ifndef ShareLayer_h
#define ShareLayer_h

#include "cocos2d.h"

class ShareLayer : public cocos2d::LayerColor {
public:
    ~ShareLayer();
    
    bool init();
    
    void setupView(cocos2d::EventCustom* event);
    
    void updateView(cocos2d::EventCustom* event);
    
    void removeOff();
    
    void clickShare(int index);
    
    void clickCount(int index);
    
    CREATE_FUNC(ShareLayer);
private:
    cocos2d::Node* root;
};

#endif /* ShareLayer_h */
