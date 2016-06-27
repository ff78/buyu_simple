//
//  NetLayer.h
//  buyu_simple
//
//  Created by ff on 16/1/25.
//
//

#ifndef NetLayer_h
#define NetLayer_h

#include "cocos2d.h"

class NetLayer : public cocos2d::Layer {
public:
    NetLayer();
    ~NetLayer();
    
    bool init();
    
    CREATE_FUNC(NetLayer);
    
    void castNet(cocos2d::EventCustom* event);
private:
    cocos2d::EventListenerCustom* castListener;
};

#endif /* NetLayer_h */
