//
//  BlackMary.h
//  buyu_simple
//
//  Created by ff on 16/3/24.
//
//

#ifndef BlackMary_h
#define BlackMary_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "GameDef.h"

class BlackMary : public Singleton<BlackMary>
{
public:
    BlackMary();
    ~BlackMary();
    
    void randRoll();
    
    int fillResult();
    
    int centerTypeId[4];
    
    void startMary(int cannonMulti);
private:
//    int multi;
    CC_SYNTHESIZE_READONLY(int, roundBtnIndex, RoundBtnIndex);
    CC_SYNTHESIZE_READONLY(int, roundTypeId, RoundTypeId);
    CC_SYNTHESIZE_READONLY(int, cardType, CardType);
//    CC_SYNTHESIZE_READONLY(int, roundScore, RoundScore);
    CC_SYNTHESIZE_READONLY(int, stageScore, StageScore);
    CC_SYNTHESIZE_READONLY(int, multi, CannonMulti);
    CC_SYNTHESIZE_READONLY(int, stageDiamond, StageDiamond);
    CC_SYNTHESIZE_READONLY(int, roundCount, RoundCount);
    
};

#endif /* BlackMary_h */
