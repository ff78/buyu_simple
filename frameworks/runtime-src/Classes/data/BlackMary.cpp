//
//  BlackMary.cpp
//  buyu_simple
//
//  Created by ff on 16/3/24.
//
//

#include "BlackMary.h"

BlackMary::BlackMary()
:stageScore(0),
stageDiamond(0)
{
    roundBtnIndex = roundTypeId = -1;
    for (int i = 0; i < 4; i++) {
        centerTypeId[i] = -1;
    }
}


BlackMary::~BlackMary()
{
    
}

void BlackMary::startMary(int cannonMulti)
{
    roundBtnIndex = 0;
    stageScore = 0;
    stageDiamond = 0;
    multi = cannonMulti;
    roundCount = 0;
}

void BlackMary::randRoll()
{
    int total = 100;
    if(roundBtnIndex != -1)
    {
        //减掉上次占用的概率，避免抽到和上次相同的项
        total -= roundRollRate[roundBtnIndex];
        //第一轮不能选到退出，或者没有任何收益的情况下也不会选退出
        if (roundCount == 0 || (stageScore == 0 && stageDiamond == 0)) {
            total -= 20;
        }
    }else{
        roundBtnIndex = 0;
    }
    
    int rollCount = abs(rand())%total;
    bool found = false;
    //第8轮必退出
    if (roundCount == 10) {
        int index = 1;
        for (int i = 0; i < 22; i++) {
            if (roundBtnTypeId[(i+roundBtnIndex+1)%24] != 0) {
                continue;
            }
            if (rollCount <= 25*index) {
                roundBtnIndex = (i+roundBtnIndex+1)%24;
                
                found = true;
                break;
            }
            rollCount -= 25;
            index++;
        }
        if (!found) {
            roundBtnIndex = 0;
        }
    }else{
        for (int i = 0; i < 22; i++) {
            //第一轮，或者没有任何收益的情况下不能选到退出
            if ((roundCount == 0 || (stageScore == 0 && stageDiamond == 0)) && roundBtnTypeId[(i+roundBtnIndex+1)%24] == 0) {
                continue;
            }
            if (rollCount <= roundRollRate[(i+roundBtnIndex+1)%24]) {
                roundBtnIndex = (i+roundBtnIndex+1)%24;
                
                found = true;
                break;
            }
            rollCount -= roundRollRate[(i+roundBtnIndex+1)%24];
        }
        if (found == false) {
            roundBtnIndex = (roundBtnIndex + 23)%24;
        }
    }
    
    
    
    roundTypeId = roundBtnTypeId[roundBtnIndex];
    
    for (int i = 0; i < 4; i++) {
        rollCount = abs(rand())%100;
        for (int j = 0; j < 11; j++) {
            if (rollCount <= centerRollRate[j]) {
                centerTypeId[i] = j+1;
                break;
            }
            rollCount -= centerRollRate[j];
        }
    }
    
    //如果第一次没有出现相同的，再试一次
    if(fillResult()==0)
    {
        for (int i = 0; i < 4; i++) {
            rollCount = abs(rand())%100;
            for (int j = 0; j < 11; j++) {
                if (rollCount <= centerRollRate[j]) {
                    centerTypeId[i] = j+1;
                    break;
                }
                rollCount -= centerRollRate[j];
            }
        }
        fillResult();
    }
    roundCount++;
}

int BlackMary::fillResult()
{
    if (roundTypeId <= 0) {
        return 0;
    }
    
    //中区找相同的数量
    int foundCount = 0;
    for (int i = 0; i < 4; i++) {
        if (centerTypeId[i] == roundTypeId) {
            foundCount++;
        }        
    }
    if (foundCount == 0) {
        //中区没有相同的花色，不会加分
        cardType = 0;
        return 0;
    }
    
    //判断连续相同的牌型
    if (foundCount == 4) {        
        cardType = 2;
        foundCount = 1;
    }else if (foundCount == 3 && centerTypeId[1] == centerTypeId[2]){
        if (centerTypeId[0] != roundTypeId) {
            cardType = 3;
        } else {
            cardType = 1;
        }
        foundCount = 1;
    }else{
        cardType = 0;
    }
    
    //计分，小于1不得分
    int roundScore = typeRollMulti[roundTypeId-1]*maryCardMulti[cardType]*multi*foundCount;
    if (roundTypeId == 9 || roundTypeId == 10) {
        //钻石的分加到钻石上
        stageDiamond += (int)roundScore;
    }else{
        stageScore += (int)roundScore;
    }
    return foundCount;
}
