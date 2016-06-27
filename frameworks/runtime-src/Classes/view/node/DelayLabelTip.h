//
//  DelayLabelTip.h
//  buyu_simple
//
//  Created by ff on 16/4/27.
//
//

#ifndef DelayLabelTip_h
#define DelayLabelTip_h

#include "cocos2d.h"

class DelayLabelTip : public cocos2d::Node {
public:
    typedef std::function<void()> fp;
    
    bool init();
    
    void setupView(fp *delayFunc=nullptr);
    
    void removeOff();
    
    CREATE_FUNC(DelayLabelTip);
    
private:
    cocos2d::Node *root;
    
    CC_SYNTHESIZE(float, delayDuration, DelayDuration);
    CC_SYNTHESIZE(std::string, text1, Text1);
    CC_SYNTHESIZE(std::string, text2, Text2);
};

#endif /* DelayLabelTip_h */
