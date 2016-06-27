//
//  BlackNiuNiu.cpp
//  buyu_simple
//
//  Created by ff on 16/3/17.
//
//

#include "BlackNiuNiu.h"

std::vector<int> BlackNiuNiu::cards;
int BlackNiuNiu::handCard[5];

BlackNiuNiu::BlackNiuNiu()
{
    cards.clear();

    for (int i = 0; i < 5; i++) {
        handCard[i] = -1;
    }
}

BlackNiuNiu::~BlackNiuNiu()
{
    cards.clear();
}

int *BlackNiuNiu::getHandCard()
{
    return handCard;
}

void BlackNiuNiu::shuffle()
{
    GameUtils::getRandArray(cards, gNiuMaxCards);
}

void BlackNiuNiu::deal()
{
    //发过了一半牌，就重新洗牌
    if (cards.empty()) {
        shuffle();
    }
    if (cards.size() < gNiuMaxCards/2) {
        shuffle();
    }
    
    for (int i = 0; i < 5; i++) {
        handCard[i] = cards[cards.size()-1];
        cards.pop_back();
    }
}

int BlackNiuNiu::showHand()
{
    //10点和花牌的数量
    int royalCardCount = 0;
    //电话号码牌的点数总和
    int smallRankSum = 0;
    //电话号码牌
    std::vector<int> smallRank;
    
    //统计10点和花牌的数量，和电话号码牌及他们的点数总和
    for (int i = 0; i < 5; i++) {
        int point = (handCard[i]-1)%13;
        if (point/9 == 1)
        {
            royalCardCount++;
        }else{
            smallRankSum += (point+1);
            smallRank.push_back(point+1);
        }
    }
    
    int result  = 0;
    switch (royalCardCount) {
        case 5:
            //5张花牌必牛牛
            return NIU_10;
        case 4:
        case 3:
            //小牌的和决定牛数，2张牌不可能到20
            if (smallRankSum == 10) {
                return NIU_10;
            }else{
                return smallRankSum%10;
            }
        case 2:
        {
            //2张花牌时，小牌为10时为牛牛
            if (smallRankSum < 10) {
                return NIU_0;
            }
            if (smallRankSum % 10 == 0) {
                return NIU_10;
            }
            result = 0;
            for (int i = 0; i < smallRank.size(); i++) {
                //2张牌不可能到20
                if (smallRankSum-smallRank[i] == 10) {
                    result = smallRank[i];
                }
            }
            if (result == 0) {
                return NIU_0;
            }
            return result;
        }
            break;
            
        default:
            break;
    }
    
    //小牌数量小于3的情况，上面已排除。
    //小牌和点小于等于10，不可能有牛。刚好只有3张，且和为10已排除过。
    if (smallRankSum <= 10) {
        return NIU_0;
    }
    
    //5张任意抽2张，可穷尽。4张还需要判断任意抽1张的情况。
    for (int i = 0; i < smallRank.size()-1; i++) {
        int sum = 0;
        if (smallRank.size() == 4) {
            if ((smallRankSum-smallRank[i])%10 == 0) {
                result = MAX(result, smallRank[i]);
            }
        }
        for (int j = i+1; j < smallRank.size(); j++) {
            sum = smallRank[i]+smallRank[j];
           
            if (sum == 10) { //两张和出现牛
                //剩余牌点数，决定牛数
                if ((smallRankSum-sum)%10 == 0) {
                    return NIU_10;
                }else{
                    result = MAX(result, (smallRankSum-sum)%10);
                }
            }else if ((smallRankSum-sum)%10 == 0) { //剩余牌出现牛
                result = MAX(result, sum%10);
            }
        }
    }
    
    if (result == 0) {
        return NIU_0;
    }
    
    
    return result;
}
