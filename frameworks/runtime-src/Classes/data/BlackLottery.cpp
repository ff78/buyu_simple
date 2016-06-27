//
//  BlackLottery.cpp
//  buyu_simple
//
//  Created by ff on 16/5/3.
//
//

#include "BlackLottery.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "GameDef.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> BlackLottery::idTable;

BlackLottery::BlackLottery()
:killBigCount(0),
killStarCount(0),
killBigScore(0)
{
    
}

BlackLottery::~BlackLottery()
{
    
}

void BlackLottery::loadFromJson(int bigCount, int starCount, int bigScore)
{
    killStarCount = starCount;
    killBigCount = bigCount;
    killBigScore = bigScore;
    
    L2E_UPDATE_BIG_STAR info;
    info.eProtocol = l2e_update_big_star;
    info.starCount = killStarCount;
    info.boxEnable = (killStarCount==5);
    ClientLogic::instance()->pass2Engine(&info);
}

void BlackLottery::addKillBig(int deltaScore)
{
    killBigCount++;
    killStarCount++;
    killStarCount = MIN(5, killStarCount);
    
    killBigScore += deltaScore;
    
    
    L2E_UPDATE_BIG_STAR info;
    info.eProtocol = l2e_update_big_star;
    info.starCount = killStarCount;
    info.boxEnable = (killStarCount==5);
    ClientLogic::instance()->pass2Engine(&info);
}

void BlackLottery::enterLuckyDraw()
{
    L2E_SHOW_LUCKY_DRAW info;
    info.eProtocol = l2e_show_lucky_draw;
    info.level = 0;
    for (int i = 1; i <= lotteryMap.size(); i++) {
        if (killBigScore < lotteryMap[i].boundsThres) {
            break;
        }
        info.level++;
    }
    info.level = MIN(lotteryMap.size(), info.level);
    if (info.level == lotteryMap.size()) {
        //最高奖可以开
        info.showType = 4;
    }else if (info.level == 0) {
        //最低奖也不能开
        info.level = 1;
        info.showType = 1;
    } else {
        //可以开到这档
        info.showType = 2;
    }
    memcpy(info.boundtypes, lotteryMap[info.level].boundsType, sizeof(int)*6);
    memcpy(info.boundIds, lotteryMap[info.level].boundsId, sizeof(int)*6);
    memcpy(info.boundCount, lotteryMap[info.level].boundsCount, sizeof(int)*6);
    
    info.killScore = killBigScore;
    if (info.showType == 2) {
        info.maxScore = lotteryMap[info.level+1].boundsThres;
        info.nextTitle = lotteryMap[info.level+1].title;
    }else if (info.showType == 1){
        info.maxScore = lotteryMap[info.level].boundsThres;
    }
    
    
    ClientLogic::instance()->pass2Engine(&info);
}

void BlackLottery::enterLottery()
{
    
    L2E_SHOW_LOTTERY info;
    info.eProtocol = l2e_show_lottery;
    
    info.level = 0;
    for (int i = 1; i <= lotteryMap.size(); i++) {
        if (killBigScore < lotteryMap[i].boundsThres) {
            break;
        }
        info.level++;
    }
    info.level = MIN(lotteryMap.size(), info.level);

    memcpy(info.boundtypes, lotteryMap[info.level].boundsType, sizeof(int)*6);
    memcpy(info.boundIds, lotteryMap[info.level].boundsId, sizeof(int)*6);
    memcpy(info.boundCount, lotteryMap[info.level].boundsCount, sizeof(int)*6);
    
    std::vector<int> cardId;
    GameUtils::getRandArray(cardId, 6);
    int randCount = abs(rand())%100;
    int selCard = 0;
    for (auto itr = cardId.begin(); itr != cardId.end();) {
        if (randCount < lotteryMap[info.level].boundsRate[*itr-1]) {
            selCard = *itr-1;
            itr = cardId.erase(itr);
            break;
        }else{
            randCount -= lotteryMap[info.level].boundsRate[*itr-1];
            itr++;
        }
    }
    
    resultIdx = selCard;
    resultLevel = info.level;
    info.result[0] = selCard;
    int index = 1;
    for (auto itr = cardId.begin(); itr != cardId.end(); itr++,index++) {
        info.result[index] = *itr-1;
    }
    
    ClientLogic::instance()->pass2Engine(&info);
    
    killStarCount = 0;
    killBigScore = 0;
}

void BlackLottery::updateLuckyDraw(int level)
{
    L2E_UPDATE_LUCKY_DRAW info;
    info.eProtocol = l2e_update_lucky_draw;
    info.level = level;
    
    int minLevel = 0;
    for (int i = 1; i <= lotteryMap.size(); i++) {
        if (killBigScore < lotteryMap[i].boundsThres) {
            break;
        }
        minLevel++;
    }
    minLevel = MIN(lotteryMap.size(), minLevel);
    if (killBigScore >= lotteryMap[level].boundsThres) {
        //可以开本档
        if (level >= lotteryMap.size()) {
            //已是最高档
            info.showType = 4;
        }else if(level < minLevel){
            //可以开更高档
            info.showType = 3;
        }else{
            info.showType = 2;
        }
    }else{
        //不能开本档
        info.showType = 1;
    }
    memcpy(info.boundtypes, lotteryMap[info.level].boundsType, sizeof(int)*6);
    memcpy(info.boundIds, lotteryMap[info.level].boundsId, sizeof(int)*6);
    memcpy(info.boundCount, lotteryMap[info.level].boundsCount, sizeof(int)*6);
    
    info.killScore = killBigScore;
    
    if (info.showType == 2) {
        info.maxScore = lotteryMap[info.level+1].boundsThres;
        info.nextTitle = lotteryMap[info.level+1].title;
    }else if (info.showType == 1){
        info.maxScore = lotteryMap[info.level].boundsThres;
    }
    
    ClientLogic::instance()->pass2Engine(&info);
}

void BlackLottery::loadConfig()
{
    auto doc = GameReader::getDocInstance(LOTTERY_CONFIG_FILE);
    
    if(BlackLottery::idTable.empty())
    {
        BlackLottery::initIdTable(*doc);
    }
    
    for (int i = 0; i < idTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        if (dic.IsNull()) {
            return;
        }
        
        LOTTERY_DATA data;
        data.level = DICTOOL->getIntValue_json(dic, "level");
        data.boundsThres = DICTOOL->getIntValue_json(dic, "thres");
        for (int i = 0; i < 6; i++) {
            std::string typeName("boundsType");
//            typeName += std::to_string(i+1);
            typeName += Convert2String(i+1);
            data.boundsType[i] = DICTOOL->getIntValue_json(dic, typeName.c_str());
            std::string boundIdName("boundsId");
//            boundIdName += std::to_string(i+1);
            boundIdName += Convert2String(i+1);
            data.boundsId[i] = DICTOOL->getIntValue_json(dic, boundIdName.c_str());
            std::string countName("boundsCount");
//            countName += std::to_string(i+1);
            countName += Convert2String(i+1);
            data.boundsCount[i] = DICTOOL->getIntValue_json(dic, countName.c_str());
            std::string rateName("boundsRate");
//            rateName += std::to_string(i+1);
            rateName += Convert2String(i+1);
            data.boundsRate[i] = DICTOOL->getIntValue_json(dic, rateName.c_str());
        }
        data.title = DICTOOL->getStringValue_json(dic, "discript");

        lotteryMap[data.level] = data;
    }
}

void BlackLottery::initIdTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()){
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId = DICTOOL->getIntValue_json(_doc[i], "level");
            idTable.insert(std::make_pair(objId, i));
        }
    }
}

void BlackLottery::endLottery()
{
    L2E_UPDATE_BIG_STAR info;
    info.eProtocol = l2e_update_big_star;
    info.starCount = killStarCount;
    info.boxEnable = (killStarCount==5);
    ClientLogic::instance()->pass2Engine(&info);
}
