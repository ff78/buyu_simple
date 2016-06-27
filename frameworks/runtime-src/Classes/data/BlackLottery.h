//
//  BlackLottery.h
//  buyu_simple
//
//  Created by ff on 16/5/3.
//
//

#ifndef BlackLottery_h
#define BlackLottery_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "utils/GameReader.h"

typedef struct __LOTTERY_DATA
{
    int level;
    int boundsType[6];
    int boundsId[6];
    int boundsCount[6];
    int boundsRate[6];
    int boundsThres;
    std::string title;
}LOTTERY_DATA;

class BlackLottery : public Singleton<BlackLottery>
{
public:
    BlackLottery();
    ~BlackLottery();
    
    void addKillBig(int deltaScore);
    
    void enterLuckyDraw();
    void updateLuckyDraw(int level);
    void enterLottery();
    void endLottery();
    
    void loadConfig();
    
    static void initIdTable(rapidjson::Document &_doc);
    
    void loadFromJson(int bigCount, int starCount, int bigScore);
public:
    static std::map<int, int> idTable;
    
    std::map<int, LOTTERY_DATA> lotteryMap;
private:
    CC_SYNTHESIZE_READONLY(int, resultIdx, ResultIdx);
    CC_SYNTHESIZE_READONLY(int, resultLevel, ResultLevel);
    CC_SYNTHESIZE(int, killBigCount, KillBigCount);
    CC_SYNTHESIZE(int, killStarCount, killStarCount);
    CC_SYNTHESIZE(int, killBigScore, killBigScore);
    
    
};

#endif /* BlackLottery_h */
