//
//  BlackNiuNiu.h
//  buyu_simple
//
//  Created by ff on 16/3/17.
//
//

#ifndef BlackNiuNiu_h
#define BlackNiuNiu_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "GameDef.h"

class BlackNiuNiu  : public Singleton<BlackNiuNiu> {
public:
    BlackNiuNiu();
    ~BlackNiuNiu();
    //洗牌
    void shuffle();
    //发牌
    void deal();
    //翻牌，返回结果
    int showHand();
    
    int *getHandCard();
public:
    //52张底牌
    static std::vector<int> cards;
    
    //5张手牌
    static int handCard[5];
};

#endif /* BlackNiuNiu_h */
