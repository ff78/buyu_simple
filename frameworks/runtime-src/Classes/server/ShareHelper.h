//
//  ShareHelper.h
//  buyu_simple
//
//  Created by ff on 16/6/1.
//
//

#ifndef ShareHelper_h
#define ShareHelper_h

#include "cocos2d.h"

class ShareHelper
{
public:
    void share(int index);
    static ShareHelper *getInstance();

private:
    static ShareHelper *instance;
    CC_SYNTHESIZE_READONLY(int, channalId, ChannalId);
};


#endif /* ShareHelper_h */
