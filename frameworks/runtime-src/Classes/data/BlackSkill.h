//
//  BlackSkill.hpp
//  buyu_simple
//
//  Created by ff on 16/3/4.
//
//

#ifndef BlackSkill_h
#define BlackSkill_h

#include "cocos2d.h"

class BlackSkill
{
public:
    
private:
    CC_SYNTHESIZE(int, skillId, SkillID);
    CC_SYNTHESIZE(float, coolDuration, CD);
    CC_SYNTHESIZE(int, count, Count);
    CC_SYNTHESIZE(int, price, Price);
    CC_SYNTHESIZE(bool, work, CanWork);
};

#endif /* BlackSkill_h */
