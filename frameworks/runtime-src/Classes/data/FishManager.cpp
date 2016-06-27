//
//  FishManager.cpp
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#include "FishManager.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "utils/GameReader.h"
#include "EventDef.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> FishManager::idTable;


FishManager::FishManager() :
IncrementID(0),
IncrementGroupId(0),
emitInterval(1),
selFishDuration(0),
selFishInterval(0),
createFishIdx(1),
sceneProb(1),
cannonProb(1),
priceSum(0),
bPause(false),
pauseDuration(0)
{
    GameUtils::getRandArray(smallPathRand, MAX_SMALL_PATH);
    GameUtils::getRandArray(bigPathRand, MAX_BIG_PATH);
    GameUtils::getRandArray(hugePathRand, MAX_HUGE_PATH);
    GameUtils::getRandArray(specialPathRand, MAX_SPECIAL_PATH);
    GameUtils::getRandArray(arrayPathRand, MAX_FISHARRAY_PATH);
    GameUtils::getRandArray(sceneFishStyleRand, MAX_FISHARRAY_STYLE);
    
    
    clear();
//    hitListener = EventListenerCustom::create(HIT_FISHES, CC_CALLBACK_1(FishManager::hitFishes, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hitListener, -1);
}

FishManager::~FishManager()
{
    smallPathRand.clear();
    bigPathRand.clear();
    hugePathRand.clear();
    specialPathRand.clear();
    arrayPathRand.clear();
    clear();
}

void FishManager::clear()
{
//    cannonProb = 1;
//    sceneProb = 1;
    IncrementID = 0;
    IncrementGroupId = 0;
    createFishIdx = 1;
    bPause = false;
    pauseDuration = 0;
    bombFishId = -1;
    allNetFishId = -1;
    priceSum = 0;
    fishMap.clear();
    delayCreateCmd.clear();
    

}
void FishManager::setPause(bool val)
{
    log("setPause");
    bPause = val;
}
void FishManager::tideClear()
{
    for(auto fishItr : fishMap)
    {
        if(!fishMap[fishItr.first].InsideScreen())
        {
            fishMap[fishItr.first].setOutScreen(true);
        }
    }
}

void FishManager::logic(float dt)
{
//    log("this ==== : %08x === ", this);
//    assert(bPause == false);
    if (bPause) {
        pauseDuration -= dt;
        if (pauseDuration <= 0) {
            pauseDuration = 0;
            setPause(false);
            EventCustom event(UNFROZEN_SCREEN);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }else{
//        if (selFishInterval != 0) {
//        selFishDuration += dt;
//        if (selFishDuration >= selFishInterval) {
//            selFishInterval = 0;
//            selFishDuration = 0;
//            createFish(cmdCreate);
//            if(findNextFish(createFishIdx, 1))
//            {
//                doCreateCmd();
//            }
//            
//        }
//        createRandFish(dt);
        
        for (std::vector<CMD_S_CREATE_FISH>::iterator it = delayCreateCmd.begin(); it != delayCreateCmd.end();) {
            it->fDelay -= dt;
            if (it->fDelay <= 0) {
                createDelayGroupFish(*it);
                it = delayCreateCmd.erase(it);
            }else{
                it++;
            }
        }
        for(auto fishItr : fishMap)
        {
            fishMap[fishItr.first].OnUpdate(dt);
            //        fishItr.second.OnUpdate(dt);
        }        
    }
    

//    }
//    for(auto fishItr : fishMap)
//    {
//        fishMap[fishItr.first].OnUpdate(dt);
////        fishItr.second.OnUpdate(dt);
//    }        
}

void FishManager::createNewFish(CMD_S_CREATE_FISH &cmd)
{
    IncrementID++;
    BlackFish bFish;
    bFish.create(cmd);
    
    fishMap[IncrementID] = bFish;
//    createFishIdx = cmd.nID;
    
    L2E_CREATE_FISH info;
    info.eProtocol = l2e_create_fish;
    info.fishId = IncrementID;
    ClientLogic::instance()->pass2Engine(&info);
    
    if (bFish.getFishType() == BOSS_FISH) {
        L2E_BOSS_COME_TIP info;
        info.eProtocol = l2e_boss_come;
        info.bossRes = getFishRes(bFish.getTypeID());
        info.bossType = getFishType(bFish.getTypeID());
        ClientLogic::instance()->pass2Engine(&info);
    }
    
}
void FishManager::createFish(CMD_S_CREATE_FISH &cmd)
{
    if (cmd.nType == FISH_TYPE_EX_28) {
        mGroupThreeIDList[cmd.nData] = GroupFirstIdSet(cmd.nID, 0);
        return;
    }
    IncrementID++;
    BlackFish bFish;
    bFish.create(cmd);
    if(mGroupThreeIDList.size() != 0)
    {
        for(auto &it : mGroupThreeIDList)
        {
            if(cmd.nData == it.first)
            {
                bFish.setGroupType(FishGroupType::THREE_GROUP);
                bFish.setGroupFirstId(mGroupThreeIDList[cmd.nData].group);
                if((mGroupThreeIDList[cmd.nData].count += 1) ==3)
                {
                    mGroupThreeIDList.erase(cmd.nData);
                    break;
                }
            }
        }
//    }else{
//        cocos2d::log("debug");
    }
    fishMap[IncrementID] = bFish;
    createFishIdx = cmd.nID;
    
    L2E_CREATE_FISH info;
    info.eProtocol = l2e_create_fish;
    info.fishId = IncrementID;
    ClientLogic::instance()->pass2Engine(&info);
}

void FishManager::removeFish(int fishId)
{
    fishMap.erase(fishId);
}

void FishManager::readFishFile()
{
    
}

void FishManager::initIdTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()){
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            idTable.insert(std::make_pair(objId, i));
        }
    }
}

void FishManager::readFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(FishManager::idTable.empty())
    {
        FishManager::initIdTable(*doc);
    }
    initData(idx, *doc);
}

bool FishManager::initData(int idx, rapidjson::Document& _doc)
{
    int index = idTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
//    CMD_S_CREATE_FISH cmd2;
    cmdCreate.nID = DICTOOL->getIntValue_json(dic, "ID");
    cmdCreate.nPathType = (FISH_PATH_TYPE)DICTOOL->getIntValue_json(dic, "pathType");
    cmdCreate.nType = DICTOOL->getIntValue_json(dic, "type");
    cmdCreate.nPathID = DICTOOL->getIntValue_json(dic, "pathId");
    cmdCreate.nElaspe = DICTOOL->getIntValue_json(dic, "elaspe");
    cmdCreate.fDelay = DICTOOL->getFloatValue_json(dic, "delay");
    cmdCreate.fSpeed = DICTOOL->getFloatValue_json(dic, "speed");
    cmdCreate.Score = DICTOOL->getIntValue_json(dic, "score");
    cmdCreate.nData = DICTOOL->getIntValue_json(dic, "data");
    cmdCreate.OffestX = DICTOOL->getFloatValue_json(dic, "offsetX");
    cmdCreate.OffestY = DICTOOL->getFloatValue_json(dic, "offsetY");
    cmdCreate.interval = DICTOOL->getFloatValue_json(dic, "interval");
    cmdCreate.randThres = DICTOOL->getFloatValue_json(dic, "randRate");
    
    return true;
}

bool FishManager::doCreateCmd()
{
    
    int groupCount  = 1;
    if (cmdCreate.nType == FISH_TYPE_EX_28) {
        groupCount = 4;
    }else if (cmdCreate.nType == FISH_TYPE_EX_29) {
        groupCount = 5;
    }
    
    selFishInterval = 0;
    selFishDuration = 0;
    
    if (abs(rand())%10000 > cmdCreate.randThres) {
        if(findNextFish(cmdCreate.nID, groupCount))
        {
            doCreateCmd();
        }
        
        return false;
    }
    
    if (cmdCreate.interval == 0) {
        createFish(cmdCreate);
        if(findNextFish(cmdCreate.nID, 1))
        {
            doCreateCmd();
        }
        
        return true;
    }
    
    selFishInterval = cmdCreate.interval;
    selFishDuration = 0;
    createFishIdx = cmdCreate.nID;
    return true;
}

bool FishManager::findNextFish(int idx, int groupLen)
{
    int index = idTable.find(idx)->second;
    if (index >= idTable.size()-groupLen) {
        index = 0;
        if (inTide) {
            return false;
        }
    }else{
        index+=groupLen;
    }
    
    auto _doc = GameReader::getDocInstance(TEST_CREATE_FISH_CONFIG_FILE);
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*_doc, index);
    int nextIdx = DICTOOL->getIntValue_json(dic, "ID");
    
    readFile(nextIdx, TEST_CREATE_FISH_CONFIG_FILE);
    return true;
}

void FishManager::createNextFish()
{
    readFile(createFishIdx, TEST_CREATE_FISH_CONFIG_FILE);
    doCreateCmd();
}

void FishManager::speedUpAll()
{
    bPause = false;
    pauseDuration = 0;
    for(auto fishItr : fishMap)
    {
        fishMap[fishItr.first].AddSpeed(10);
        //        fishItr.second.OnUpdate(dt);
    }
}

int FishManager::catchFish(int fishId, int multiply, int expMultiply, int chairId, bool bomb, float rateFactor)
{
    //.......todo 一网打尽  &   同类炸弹
    if (allNetFishId != -1) {
        if (fishMap[fishId].getGroupType() == ALL_NET_GROUP) {
            return 0;
        }
    }
    if (find(bombTypeGroups.begin(), bombTypeGroups.end(), fishMap[fishId].getTypeID()) != bombTypeGroups.end()) {
        return 0;
    }
    if (find(lightGroups.begin(), lightGroups.end(), fishMap[fishId].getTypeID()) != lightGroups.end()) {
        return 0;
    }
    
    if (fishMap[fishId].getGroupType() != NONE_GROUP) {
        if (find(hurtGroups.begin(), hurtGroups.end(), fishMap[fishId].getGroupFirstId()) != hurtGroups.end()) {
            return 0;
        }
        if (find(dyingGroups.begin(), dyingGroups.end(), fishMap[fishId].getGroupFirstId()) != dyingGroups.end()) {
            return 0;
        }
    }
    
    int score = 0;
    float prob = sceneProb * cannonProb * fishMap[fishId].getCatchRate()*rateFactor;
    int temp = abs(rand()%1000);
//    if(true)
    if(temp <= prob || bomb)
    {
//        if (fishMap[fishId].getGroupType() == THREE_GROUP)
//        {
//            multiply *= 3;
//        }else if (fishMap[fishId].getGroupType() == FOUR_GROUP){
//            multiply *= 4;
//        }
//    log(">>>>>>>>>>>>>>>>>>>>>>>>");
//    log("总输赢参数：%f", sceneProb);
//    log("能量炮加成：%f", cannonProb);
//    log("鱼%d的概率：%d", fishMap[fishId].getTypeID(), fishMap[fishId].getCatchRate());
//    log("一弹多鱼因子：%f", rateFactor);
//    log("总阈值：%f", prob);
//    log("随机值：%d", temp);
        
        fishMap[fishId].setStatus(FISH_DIED);
        fishMap[fishId].setCatchMultiply(multiply);
        fishMap[fishId].setCatchExpMultiply(expMultiply);
        fishMap[fishId].setCatchScore(fishMap[fishId].getBaseScore()*multiply);
        fishMap[fishId].setCatchChairId(chairId);
        score = fishMap[fishId].getCatchScore();
//        priceSum += fishMap[fishId].getBaseScore()*expMultiply;
        priceSum += fishMap[fishId].getBaseScore();
        
        if (fishMap[fishId].getTypeID() == FISH_TYPE_EX_30 && fishMap[fishId].getGroupType() == NONE_GROUP) {
            
            EventCustom event(FROZEN_SCREEN);
            Value val(5);
            event.setUserData(&val);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            return 0;
        } else if(fishMap[fishId].getTypeID() == FISH_TYPE_EX_32 && fishMap[fishId].getGroupType() == NONE_GROUP) {
            bombFishId = fishId;
//            bombMultiply = multiply;
            return 0;
        } else if(fishMap[fishId].getTypeID() == FISH_TYPE_EX_39 || fishMap[fishId].getTypeID() == FISH_TYPE_EX_41) {
            return 0;

        } else if(fishMap[fishId].getTypeID() == FISH_TYPE_EX_40) {
            fishMap[fishId].setStatus(FISH_LIGHTNING);
            if (find(lightGroups.begin(), lightGroups.end(), fishMap[fishId].getTypeID()) == lightGroups.end()) {
                lightGroups.push_back(fishId);
//                lightMultiply = multiply;
//                bombMultiply = multiply;
            }
            return 0;
        }

    }else{
        fishMap[fishId].setStatus(FISH_HURT);
    }

    
    if (fishMap[fishId].getGroupType() == NONE_GROUP) {
        return score;
    }
    
    if (fishMap[fishId].getGroupType() == THREE_GROUP || fishMap[fishId].getGroupType() == FOUR_GROUP) {
        int groupId = fishMap[fishId].getGroupFirstId();
        if (fishMap[fishId].getStatus() == FISH_HURT) {
            hurtGroups.push_back(groupId);
        }else if (fishMap[fishId].getStatus() == FISH_DIED){
            dyingGroups.push_back(groupId);
        }
    }
    if (fishMap[fishId].getStatus() == FISH_DIED) {
        if (fishMap[fishId].getGroupType() == ALL_NET_GROUP ) {
            allNetFishId = fishId;
//            bombMultiply = multiply;
        }else if (fishMap[fishId].getGroupType() == FISH_TYPE_BOMB_GROUP) {
            if (find(bombTypeGroups.begin(), bombTypeGroups.end(), fishMap[fishId].getTypeID()) == bombTypeGroups.end()) {
                bombTypeGroups.push_back(fishMap[fishId].getTypeID());
                bombTypeIdGroups.push_back(fishId);
//                bombMultiply = multiply;
            }
        }
    }
    return score;
}

void FishManager::catchGroupsFish()
{
    for (auto fish : fishMap) {
        if (fish.second.getStatus() != FISH_ALIVE) {
            continue;
        }
        
        if (fish.second.getGroupFirstId() == -1) {
            continue;
        }
        
        for (auto groupId : hurtGroups) {
            if (fish.second.getGroupFirstId() == groupId) {
                fishMap[fish.first].setStatus(FISH_HURT);
                break;
            }
        }
        
        for (auto groupId : dyingGroups) {
            if (fish.second.getGroupFirstId() == groupId) {
                fishMap[fish.first].setStatus(FISH_DIED);
                break;
            }
        }
        
    }
    
//    clearCatchGroups();

}

int FishManager::catchAllNetFish()
{
    if (allNetFishId == -1) {
        return 0;
    }
    
    int sum = fishMap[allNetFishId].getBaseScore();
    int chairId = fishMap[allNetFishId].getCatchChairId();
    for (auto fish : fishMap) {
        if(fish.second.getStatus() == FISH_DIED || fish.second.getStatus() == FISH_OUT || fish.second.getStatus() == FISH_LIGHTNING)
        {
            continue;
        }
        if (fish.second.getGroupType() != ALL_NET_GROUP) {
            continue;
        }
        if (fish.second.getPosition().x > GameUtils::winSize.width || fish.second.getPosition().x < 0 || fish.second.getPosition().y > GameUtils::winSize.height || fish.second.getPosition().y < 0) {
            continue;
        }
//        fishMap[fish.first].setStatus(FISH_CLEAR);
        fishMap[fish.first].setStatus(FISH_DIED);
        fishMap[fish.first].setCatchChairId(chairId);
        sum += fishMap[fish.first].getBaseScore();
    }
    
//    priceSum += sum*fishMap[allNetFishId].getCatchExpMultiply();
    priceSum += sum;
//    int catchScore = sum*bombMultiply;
    int catchScore = sum*fishMap[allNetFishId].getCatchMultiply();
    fishMap[allNetFishId].setBaseScore(sum);
    fishMap[allNetFishId].setCatchScore(catchScore);
    
    allNetFishId = -1;
    return catchScore;
}

int FishManager::catchFishTypeBombFish()
{
    if (bombTypeIdGroups.empty()) {
        return 0;
    }
    
    int allSum = 0;
    int sum = 0;
    for (auto bombTypeFishId : bombTypeIdGroups) {
        sum = fishMap[bombTypeFishId].getBaseScore();
        int chairId = fishMap[bombTypeFishId].getCatchChairId();
        for (auto fish : fishMap) {
            if(fish.second.getStatus() == FISH_DIED || fish.second.getStatus() == FISH_OUT || fish.second.getStatus() == FISH_LIGHTNING)
            {
                continue;
            }
            if (fish.second.getTypeID() != fishMap[bombTypeFishId].getTypeID()) {
                continue;
            }
            if (fish.second.getPosition().x > GameUtils::winSize.width || fish.second.getPosition().x < 0 || fish.second.getPosition().y > GameUtils::winSize.height || fish.second.getPosition().y < 0) {
                continue;
            }
            
//            fishMap[fish.first].setStatus(FISH_CLEAR);
            fishMap[fish.first].setStatus(FISH_DIED);
            fishMap[fish.first].setCatchChairId(chairId);
            
            sum += fishMap[fish.first].getBaseScore();
        }
//        int catchScore = sum*bombMultiply;
        int catchScore = sum*fishMap[bombTypeFishId].getCatchMultiply();
        fishMap[bombTypeFishId].setBaseScore(sum);
        fishMap[bombTypeFishId].setCatchScore(catchScore);
        
//        priceSum += sum*fishMap[bombTypeFishId].getCatchExpMultiply();
        priceSum += sum;
        allSum += catchScore;
    }
    
    return allSum;
}

int FishManager::catchLightningFish()
{
    if (lightGroups.empty()) {
        return 0;
    }
    int allSum = 0;
    int sum = 0;
    int count = 0;
    for (auto bombTypeFishId : lightGroups) {
        sum = fishMap[bombTypeFishId].getBaseScore();
        int chairId = fishMap[bombTypeFishId].getCatchChairId();
        for (auto fish : fishMap) {
            if(fish.second.getStatus() == FISH_DIED || fish.second.getStatus() == FISH_OUT || fish.second.getStatus() == FISH_LIGHTNING)
            {
                continue;
            }

            if (fish.second.getGroupType() != NONE_GROUP) {
                continue;
            }
            if (fish.second.getFishType() != SMALL_FISH) {
                continue;
            }
            
            if (fish.second.getPosition().x > GameUtils::winSize.width || fish.second.getPosition().x < 0 || fish.second.getPosition().y > GameUtils::winSize.height || fish.second.getPosition().y < 0) {
                continue;
            }
            
            fishMap[fish.first].setStatus(FISH_LIGHTNING);
            fishMap[fish.first].setCatchScore(fishMap[bombTypeFishId].getCatchMultiply()*fishMap[fish.first].getBaseScore());
            fishMap[fish.first].setCatchChairId(chairId);
            fishMap[fish.first].setCatchMultiply(fishMap[bombTypeFishId].getCatchMultiply());
            sum += fishMap[fish.first].getBaseScore();
            count++;
            if (count > 14) {
                break;
            }
        }
        int catchScore = sum*fishMap[bombTypeFishId].getCatchMultiply();
        fishMap[bombTypeFishId].setBaseScore(sum);
        fishMap[bombTypeFishId].setCatchScore(catchScore);
//        priceSum += sum*fishMap[bombTypeFishId].getCatchExpMultiply();
        priceSum += sum;
        allSum += sum*fishMap[bombTypeFishId].getCatchMultiply();
    }

    
    return allSum;
}

void FishManager::clearCatchGroups()
{
    hurtGroups.clear();
    dyingGroups.clear();
    bombTypeIdGroups.clear();
    bombTypeGroups.clear();
    lightGroups.clear();
    allNetFishId = -1;
    bombFishId = -1;
    priceSum = 0;
}
int FishManager::doBombClear()
{
    if (bombFishId == -1) {
        return 0;
    }
    
    int sum = 0;
    int groupId = -1;
    int chairId = fishMap[bombFishId].getCatchChairId();
    for (auto fish : fishMap) {
        if(fish.second.getStatus() == FISH_DIED || fish.second.getStatus() == FISH_OUT || fish.second.getStatus() == FISH_LIGHTNING)
        {
            continue;
        }
        if (fish.first == bombFishId) {
            continue;
        }
        if (fish.second.getPosition().x > GameUtils::winSize.width || fish.second.getPosition().x < 0 || fish.second.getPosition().y > GameUtils::winSize.height || fish.second.getPosition().y < 0) {
            continue;
        }
        
//        fishMap[fish.first].setStatus(FISH_CLEAR);
        fishMap[fish.first].setStatus(FISH_DIED);
        fishMap[fish.first].setCatchChairId(chairId);

        if (groupId == fish.second.getGroupFirstId() && fish.second.getGroupFirstId() != -1) {
            continue;
        }
//        fish.second.setStatus(FISH_CLEAR);
        groupId = fish.second.getGroupFirstId();
        sum += fish.second.getBaseScore();
        
    }
    
//    int catchScore = sum*bombMultiply;
    int catchScore = sum*fishMap[bombFishId].getCatchMultiply();
    fishMap[bombFishId].setBaseScore(sum);
    fishMap[bombFishId].setCatchScore(catchScore);
//    priceSum += sum*fishMap[bombFishId].getCatchExpMultiply();
    priceSum += sum;
    
    bombFishId = -1;
    return catchScore;
}
//void FishManager::hitFishes(cocos2d::EventCustom *event)
//{
//    std::map<int, int> info = *static_cast<std::map<int, int>*>(event->getUserData());
//    int foundObjId = -1;
//    std::vector<int> hurtGroups;
//    hurtGroups.clear();
//    std::vector<int> dyingGroups;
//    dyingGroups.clear();
//    
//    for (auto obj: info) {
//        if(abs(rand()%10000) > 5000)
//        {
//            fishMap[obj.first].setStatus(FISH_DIED);
//        }else{
//            fishMap[obj.first].setStatus(FISH_HURT);
//        }
//        
//        if (fishMap[obj.first].getGroupType() == 0) {
//            continue;
//        }
//        
//        if (fishMap[obj.first].getGroupType() == THREE_GROUP || fishMap[obj.first].getGroupType() == FOUR_GROUP) {
//            if (fishMap[obj.first].getStatus() == FISH_HURT) {
//                hurtGroups.push_back(fishMap[obj.first].getGroupFirstId());
//            }else if (fishMap[obj.first].getStatus() == FISH_DIED){
//                dyingGroups.push_back(fishMap[obj.first].getGroupFirstId());
//            }
//            
//        }
//    }
//    
//    for (auto fish : fishMap) {
//        if (fish.second.getStatus() != FISH_ALIVE) {
//            continue;
//        }
//        
//        if (fish.second.getGroupFirstId() == 0) {
//            continue;
//        }
//        
//        for (auto groupId : hurtGroups) {
//            if (fish.second.getGroupFirstId() == groupId) {
//                fishMap[fish.first].setStatus(FISH_HURT);
//                break;
//            }
//        }
//        
//        for (auto groupId : dyingGroups) {
//            if (fish.second.getGroupFirstId() == groupId) {
//                fishMap[fish.first].setStatus(FISH_DIED);
//                break;
//            }
//        }
//        
//    }
//}

void FishManager::loadFishConfig(std::vector<int> &excludeFish)
{
    auto doc = GameReader::getDocInstance(FISH_CONFIG_FILE);
    
    if (doc->IsArray()){
        int size = doc->Size();
        configs.clear();
        for (int i = 0; i<size; i++) {
            FishConfig config;
            config.configId =DICTOOL->getIntValue_json((*doc)[i], "ID");
            if (find(excludeFish.begin(), excludeFish.end(), config.configId) != excludeFish.end()) {
                continue;
            }
            config.type = DICTOOL->getIntValue_json((*doc)[i], "type");
            config.price = DICTOOL->getIntValue_json((*doc)[i], "Price");
            config.probability = DICTOOL->getIntValue_json((*doc)[i], "Probability");
            config.speed = DICTOOL->getFloatValue_json((*doc)[i], "Speed");
            config.refreshInterval = DICTOOL->getIntValue_json((*doc)[i], "Refresh_Time");
            config.refreshMinCount = DICTOOL->getIntValue_json((*doc)[i], "Refresh_Num_Min");
            config.refreshMaxCount = DICTOOL->getIntValue_json((*doc)[i], "Refresh_Num_Max");
            config.pathType = DICTOOL->getIntValue_json((*doc)[i], "pathType");
            config.resource = DICTOOL->getStringValue_json((*doc)[i], "Resource");
            config.refreshType = DICTOOL->getIntValue_json((*doc)[i], "RefreshType");
            config.name = DICTOOL->getStringValue_json((*doc)[i], "Descript");
            config.sound = DICTOOL->getStringValue_json((*doc)[i], "sound");
            
            GameUtils::DealWithRectString(DICTOOL->getStringValue_json((*doc)[i], "Size"), config.rect);
            
            config.dropThres = DICTOOL->getIntValue_json((*doc)[i], "dropItemRand");
            for (int j = 0; j < 3; j++) {
                config.itemType[j] = DICTOOL->getIntValue_json((*doc)[i], ("dropItemType"+Convert2String(j+1)).c_str());
                config.itemId[j] = DICTOOL->getIntValue_json((*doc)[i], ("dropItemID"+Convert2String(j+1)).c_str());
                config.itemThres[j] = DICTOOL->getIntValue_json((*doc)[i], ("dItemRate"+Convert2String(j+1)).c_str());
                config.itemCountMin[j] = DICTOOL->getIntValue_json((*doc)[i], ("dropItemMin"+Convert2String(j+1)).c_str());
                config.itemCountMax[j] = DICTOOL->getIntValue_json((*doc)[i], ("dropItemMax"+Convert2String(j+1)).c_str());
            }
            configs[config.configId] = config;
            emitDuration[config.configId] = config.refreshInterval;
        }
    }
}

void FishManager::getShowFishConfig(std::vector<FishConfig> &showConfig)
{
    auto doc = GameReader::getDocInstance(FISH_CONFIG_FILE);
    
    if (doc->IsArray()){
        int size = doc->Size();
        for (int i = 0; i<size; i++) {
            
            FishConfig config;
            config.configId =DICTOOL->getIntValue_json((*doc)[i], "ID");
            config.type = DICTOOL->getIntValue_json((*doc)[i], "type");
            config.price = DICTOOL->getIntValue_json((*doc)[i], "Price");
            config.resource = DICTOOL->getStringValue_json((*doc)[i], "Resource");
            
            if (config.type == SMALL_FISH || config.type == TINY_BOSS_FISH) {
                showConfig.push_back(config);
            } else if(config.type == BOSS_FISH) {
                if (config.configId == FISH_TYPE_EX_22 || config.configId == FISH_TYPE_EX_36) {
                    showConfig.push_back(config);
                }
            }
        }
    }
}

void FishManager::createRandFish(float dt)
{
    if (bPause) {
        return;
    }
    for (auto config : configs)
    {
        int index = config.first;
        emitDuration[index] -= dt;
        if (emitDuration[index] <= 0) {
            emitDuration[index] = configs[index].refreshInterval;
            if (configs[index].configId >= FISH_TYPE_EX_25 && configs[index].configId <= FISH_TYPE_EX_29) {
                if (configs[index].type == GROUP_FISH) {
                    createGroupFish(configs[index]);
                    continue;
                }
            }
            
            
            if (configs[index].type == BOSS_FISH && configs[index].configId != bossFishId) {
                continue;
            }
            
//            if (configs[index].type == GROUP_FISH) {
//                IncrementGroupId++;
//            }
            
            int count = abs(rand());
            count %= (configs[index].refreshMaxCount+1-configs[index].refreshMinCount);
            count += configs[index].refreshMinCount;
            int pathId = 0;
            for (int j = 0; j<count; j++) {
                CMD_S_CREATE_FISH cmd;
                cmd.nID = IncrementID;
                cmd.nPathType = (FISH_PATH_TYPE)configs[index].pathType;
                cmd.nType = configs[index].configId;
                cmd.randThres = configs[index].probability;
                cmd.collideRect = configs[index].rect;
                cmd.resource = configs[index].resource;
                cmd.sound = configs[index].sound;
                
                cmd.nElaspe = 0;
                cmd.fDelay = 0;
                cmd.fSpeed = configs[index].speed;
                cmd.Score = configs[index].price;
                cmd.nData = 0;

                cmd.groupType = NONE_GROUP;
                cmd.groupId = -1;
                cmd.fishType = (FishType)configs[index].type;
                
                cmd.dropThres = configs[index].dropThres;
                for (int i = 0; i < 3; i++) {
                    cmd.dropItemType[i] = configs[index].itemType[i];
                    cmd.dropItemId[i] = configs[index].itemId[i];
                    cmd.dropItemThres[i] = configs[index].itemThres[i];
                    if (configs[index].itemCountMax[i] <= configs[index].itemCountMin[i]) {
                        cmd.dropItemCount[i] = configs[index].itemCountMin[i];
                    }else{
                        cmd.dropItemCount[i] = configs[index].itemCountMin[i]+abs(rand())%(configs[index].itemCountMax[i]-configs[index].itemCountMin[i]);
                    }
                }

                if (configs[index].refreshType == 1 || j == 0) {
                    //                int maxPathId = MAX_SMALL_PATH;
                    switch (cmd.nPathType) {
                        case SMALL_FISH_PATH:
                        {
                            pathId = smallPathRand[smallPathRand.size()-1];
                            smallPathRand.pop_back();
                            if (smallPathRand.empty()) {
                                GameUtils::getRandArray(smallPathRand, MAX_SMALL_PATH);
                            }
    //                        maxPathId = MAX_SMALL_PATH;
                        }
                        break;
                        case BIG_FISH_PATH:
                        {
                            pathId = bigPathRand[bigPathRand.size()-1];
                            bigPathRand.pop_back();
                            if (bigPathRand.empty()) {
                                GameUtils::getRandArray(bigPathRand, MAX_BIG_PATH);
                            }
    //                        maxPathId = MAX_BIG_PATH;
                        }
                        break;
                        case HUGE_FISH_PATH:
                        {
                            pathId = hugePathRand[hugePathRand.size()-1];
                            hugePathRand.pop_back();
                            if (hugePathRand.empty()) {
                                GameUtils::getRandArray(hugePathRand, MAX_HUGE_PATH);
                            }
    //                        maxPathId = MAX_HUGE_PATH;
                        }
                        break;
                        case SPECIAL_FISH_PATH:
                        {
                            pathId = specialPathRand[specialPathRand.size()-1];
                            specialPathRand.pop_back();
                            if (specialPathRand.empty()) {
                                GameUtils::getRandArray(specialPathRand, MAX_SPECIAL_PATH);
                            }
    //                        maxPathId = MAX_SPECIAL_PATH;
                        }
                        break;
                        case FISH_ARRAY_PATH:
                        {
                            pathId = arrayPathRand[arrayPathRand.size()-1];
                            arrayPathRand.pop_back();
                            if (arrayPathRand.empty()) {
                                GameUtils::getRandArray(arrayPathRand, MAX_FISHARRAY_PATH);
                            }
//                            cmd.nData = IncrementGroupId;
//                            cmd.groupId = IncrementGroupId;
                            if (configs[index].configId == FISH_TYPE_EX_37) {
                                cmd.groupType = ALL_NET_GROUP;
                            }else if (configs[index].configId == FISH_TYPE_EX_38) {
                                cmd.groupType = FISH_TYPE_BOMB_GROUP;
                            }
                            int fishType = abs(rand()) % FISH_TYPE_EX_13;
                            cmd.nType = fishType;
                            cmd.Score = configs[fishType].price;
                            cmd.resource = configs[fishType].resource;
                            
                        }
                            break;
                    }
                    cmd.nPathID = pathId;

                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                }else{
                    cmd.nPathID = pathId;
                    cmd.OffestX = rand()%100;
                    cmd.OffestY = rand()%60;
                }
//                cmd.nPathID = abs(rand())%maxPathId;
                createNewFish(cmd);
            }

        }
    }
}

void FishManager::createGroupFish(FishConfig &config)
{
    if (config.configId == FISH_TYPE_EX_26 || config.configId == FISH_TYPE_EX_27) {
        return;
    }

    IncrementGroupId++;
    int pathId = arrayPathRand[arrayPathRand.size()-1];
    arrayPathRand.pop_back();
    if (arrayPathRand.empty()) {
        GameUtils::getRandArray(arrayPathRand, MAX_FISHARRAY_PATH);
    }
    switch (config.configId) {
        case FISH_TYPE_EX_25:
        {
//            int fishType = abs(rand())%FISH_TYPE_EX_13;
//            int pathId = abs(rand())%MAX_FISHARRAY_PATH;
//            int delayTime = abs(rand())%30/10;
//            CMD_S_CREATE_FISH cmd;
//            cmd.nType = FISH_TYPE_EX_25;
//            cmd.nPathID = pathId;
//            cmd.nPathType = FISH_ARRAY_PATH;
//            cmd.nElaspe = 0;
//            cmd.fSpeed = config.speed;
//            cmd.randThres = config.probability;
//            cmd.Score = config.price;
//            cmd.nData = IncrementGroupId;
//            cmd.collideRect = config.rect;
//            cmd.interval = 0;
//            cmd.OffestX = cmd.OffestY = 0;
//            cmd.fDelay = 0;
//            cmd.groupType = SNAKE_HEAD;
//            cmd.groupId = IncrementGroupId;
//            createNewFish(cmd);
//            
//            
//            int bodyCount = abs(rand())%5+5;
//            for (int i = 0; i < bodyCount; i++) {
//                CMD_S_CREATE_FISH cmd;
//                cmd.nType = fishType;
//                cmd.nPathID = pathId;
//                cmd.nPathType = FISH_ARRAY_PATH;
//                cmd.nElaspe = 0;
//                cmd.fSpeed = config.speed;
//                cmd.randThres = configs[FISH_TYPE_EX_26].probability;
//                cmd.Score = configs[fishType].price;
//                cmd.nData = IncrementGroupId;
//                cmd.collideRect = configs[FISH_TYPE_EX_26].rect;
//                cmd.interval = 0;
//                cmd.OffestX = cmd.OffestY = 0;
//                cmd.fDelay = delayTime*(i+1);
//                cmd.groupType = SNAKE_BODY;
//                cmd.groupId = IncrementGroupId;
//                delayCreateCmd.push_back(cmd);
//            }
//            
//            CMD_S_CREATE_FISH cmdTail;
//            cmdTail.nType = FISH_TYPE_EX_27;
//            cmdTail.nPathID = pathId;
//            cmdTail.nPathType = FISH_ARRAY_PATH;
//            cmdTail.nElaspe = 0;
//            cmdTail.fSpeed = config.speed;
//            cmdTail.randThres = configs[FISH_TYPE_EX_27].probability;
//            cmdTail.Score = 0;
//            cmdTail.nData = IncrementGroupId;
//            cmdTail.collideRect = configs[FISH_TYPE_EX_27].rect;
//            cmdTail.interval = 0;
//            cmdTail.OffestX = cmdTail.OffestY = 0;
//            cmdTail.fDelay = delayTime*bodyCount;
//            cmdTail.groupType = SNAKE_TAIL;
//            cmdTail.groupId = IncrementGroupId;
//            delayCreateCmd.push_back(cmdTail);
            
        }
        break;
        
        case FISH_TYPE_EX_28:
        {
            int fishType = abs(rand())%FISH_TYPE_EX_13;
//            int pathId = abs(rand())%MAX_FISHARRAY_PATH;
            float delayTime = abs(rand())%10/10.0;
            delayTime += 0.2;
            for (int i = 0; i<3; i++) {
                CMD_S_CREATE_FISH cmd;
                cmd.nType = fishType;
                cmd.nPathID = pathId;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = config.speed;
                cmd.randThres = config.probability;
                cmd.Score = configs[fishType].price*3;
                cmd.nData = IncrementGroupId;
                cmd.collideRect = config.rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = delayTime*(i+1);
                cmd.groupType = THREE_GROUP;
                cmd.groupId = IncrementGroupId;
                cmd.fishType = (FishType)config.type;
                cmd.resource = configs[fishType].resource;
                cmd.sound = configs[fishType].sound;
                
                cmd.dropThres = config.dropThres;
                for (int i = 0; i < 3; i++) {
                    cmd.dropItemType[i] = config.itemType[i];
                    cmd.dropItemId[i] = config.itemId[i];
                    cmd.dropItemThres[i] = config.itemThres[i];
                    if (config.itemCountMin[i] >= config.itemCountMax[i]) {
                        cmd.dropItemCount[i] = config.itemCountMin[i];
                    }else{
                        cmd.dropItemCount[i] = config.itemCountMin[i]+abs(rand())%(config.itemCountMax[i]-config.itemCountMin[i]);
                    }
                    
                }
//                cmd.fishType = GROUP_FISH;
                delayCreateCmd.push_back(cmd);
            }
            
        }
        break;
        
        case FISH_TYPE_EX_29:
        {
            int fishType = abs(rand())%FISH_TYPE_EX_13;
//            int pathId = abs(rand())%MAX_FISHARRAY_PATH;
            float delayTime = abs(rand())%10/10.0;
            delayTime += 0.2;
            for (int i = 0; i<4; i++) {
                CMD_S_CREATE_FISH cmd;
                cmd.nType = fishType;
                cmd.nPathID = pathId;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = config.speed;
                cmd.randThres = config.probability;
                cmd.Score = configs[fishType].price*4;
                cmd.nData = IncrementGroupId;
                cmd.collideRect = config.rect;
                cmd.interval = 0;
//                cmd.OffestX = cmd.OffestY = 0;
//                cmd.fDelay = delayTime*(i+1);
                cmd.OffestX = (i%2)*30*(1-(i/2)*2);
                cmd.OffestY = (i+1)%2*30*(1-((i+1)/2)*2);
                cmd.fDelay = 0;
                cmd.groupType = FOUR_GROUP;
                cmd.groupId = IncrementGroupId;
                cmd.fishType = (FishType)config.type;
                cmd.resource = configs[fishType].resource;
                cmd.sound = configs[fishType].sound;
                
                cmd.dropThres = config.dropThres;
                for (int i = 0; i < 3; i++) {
                    cmd.dropItemType[i] = config.itemType[i];
                    cmd.dropItemId[i] = config.itemId[i];
                    cmd.dropItemThres[i] = config.itemThres[i];
                    if (config.itemCountMin[i] >= config.itemCountMax[i]) {
                        cmd.dropItemCount[i] = config.itemCountMin[i];
                    }else{
                        cmd.dropItemCount[i] = config.itemCountMin[i]+abs(rand())%(config.itemCountMax[i]-config.itemCountMin[i]);
                    }
                }

                delayCreateCmd.push_back(cmd);
            }
        }
        break;
        
        default:
        break;
    }
    
}

void FishManager::createDelayGroupFish(CMD_S_CREATE_FISH &cmd)
{
    IncrementID++;
    BlackFish bFish;
    bFish.create(cmd);
    bFish.setGroupType(cmd.groupType);
    bFish.setGroupFirstId(cmd.groupId);
    
    fishMap[IncrementID] = bFish;
//    createFishIdx = cmd.nID;
    
    L2E_CREATE_FISH info;
    info.eProtocol = l2e_create_fish;
    info.fishId = IncrementID;
    ClientLogic::instance()->pass2Engine(&info);
}

void FishManager::createSceneFish()
{
    Point offset(0.0f, 0.0f);
    float fSpeed = 1.0;
    int st = sceneFishStyleRand[sceneFishStyleRand.size()-1];
    sceneFishStyleRand.pop_back();
    if (sceneFishStyleRand.empty()) {
        GameUtils::getRandArray(sceneFishStyleRand, MAX_FISHARRAY_STYLE);
    }
    delayCreateCmd.clear();
    std::vector<CMD_S_CREATE_FISH> cmdArray;
    switch(st)
    {
        case 1:	//! 鱼阵1
        {
            //! 右边第一个圈
            for (int i = 0; i < 30; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.nType = FISH_TYPE_EX_1;
                cmd.nPathID = i;
                cmd.nPathType = FISH_ARRAY_PATH_EX3;
                cmd.nElaspe = 0;
                cmd.fSpeed = 1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                
                
                cmdArray.push_back(cmd);
            }
            
            //! 右边第二个圈
            for (int i = 30; i < 55; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_3;
                cmd.nPathID = i;
                cmd.nPathType = FISH_ARRAY_PATH_EX3;
                cmd.nElaspe = 0;
                cmd.fSpeed = 1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmdArray.push_back(cmd);
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_3);
//                fish.SetPathID(i);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH_EX3);
//                fish.SetSpeed(1.0f);
//                m_Fishs.push_back(fish);
            }
            
            //! 右边第三个圈
            for (int i = 55; i < 75; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_5;
                cmd.nPathID = i;
                cmd.nPathType = FISH_ARRAY_PATH_EX3;
                cmd.nElaspe = 0;
                cmd.fSpeed = 1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmdArray.push_back(cmd);
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_5);
//                fish.SetPathID(i);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH_EX3);
//                fish.SetSpeed(1.0f);
//                m_Fishs.push_back(fish);
            }
            
            //! 右边第四个圈
            for (int i = 75; i < 90; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_6;
                cmd.nPathID = i;
                cmd.nPathType = FISH_ARRAY_PATH_EX3;
                cmd.nElaspe = 0;
                cmd.fSpeed = 1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmdArray.push_back(cmd);
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_6);
//                fish.SetPathID(i);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH_EX3);
//                fish.SetSpeed(1.0f);
//                m_Fishs.push_back(fish);
            }
            
            //! 左边第一个圈
            for (int i = 90; i < 120; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_2;
                cmd.nPathID = i;
                cmd.nPathType = FISH_ARRAY_PATH_EX3;
                cmd.nElaspe = 0;
                cmd.fSpeed = 1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmdArray.push_back(cmd);
            }
            
            //! 左边第二个圈
            for (int i = 120; i < 145; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_4;
                cmd.nPathID = i;
                cmd.nPathType = FISH_ARRAY_PATH_EX3;
                cmd.nElaspe = 0;
                cmd.fSpeed = 1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmdArray.push_back(cmd);
            }
            
            //! 左边第三个圈
            for (int i = 145; i < 165; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_6;
                cmd.nPathID = i;
                cmd.nPathType = FISH_ARRAY_PATH_EX3;
                cmd.nElaspe = 0;
                cmd.fSpeed = 1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmdArray.push_back(cmd);
            }
            
            //! 左边第四个圈
            for (int i = 165; i < 180; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_7;
                cmd.nPathID = i;
                cmd.nPathType = FISH_ARRAY_PATH_EX3;
                cmd.nElaspe = 0;
                cmd.fSpeed = 1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = cmd.OffestY = 0;
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmdArray.push_back(cmd);
            }
            
            //! 两个中心鱼
            CMD_S_CREATE_FISH cmd;
            cmd.dropThres = 0;
            cmd.groupId = -1;
            cmd.groupType = NONE_GROUP;
            cmd.nType = FISH_TYPE_EX_18;
            cmd.nPathID = 180;
            cmd.nPathType = FISH_ARRAY_PATH_EX3;
            cmd.nElaspe = 0;
            cmd.fSpeed = 1;
            cmd.randThres = configs[cmd.nType].probability;
            cmd.Score = configs[cmd.nType].price;
            cmd.nData = -1;
            cmd.collideRect = configs[cmd.nType].rect;
            cmd.interval = 0;
            cmd.OffestX = 0;
            cmd.OffestY = 0;
            cmd.fDelay = 0;
            cmd.fishType = (FishType)configs[cmd.nType].type;
            cmd.resource = configs[cmd.nType].resource;
            cmd.sound = configs[cmd.nType].sound;
            cmdArray.push_back(cmd);
//            CFish fish;
//            fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//            fish.SetTypeID(FISH_TYPE_EX_18);
//            fish.SetPathID(180);
//            fish.SetData(-1);
//            fish.SetPathType(FISH_ARRAY_PATH_EX3);
//            fish.SetSpeed(1.0f);
//            m_Fishs.push_back(fish);
            CMD_S_CREATE_FISH cmd1;
            cmd1.dropThres = 0;
            cmd1.groupId = -1;
            cmd1.groupType = NONE_GROUP;
            cmd1.nType = FISH_TYPE_EX_19;
            cmd1.nPathID = 181;
            cmd1.nPathType = FISH_ARRAY_PATH_EX3;
            cmd1.nElaspe = 0;
            cmd1.fSpeed = 1;
            cmd1.randThres = configs[cmd1.nType].probability;
            cmd1.Score = configs[cmd1.nType].price;
            cmd1.nData = -1;
            cmd1.collideRect = configs[cmd1.nType].rect;
            cmd1.interval = 0;
            cmd1.OffestX = 0;
            cmd1.OffestY = 0;
            cmd1.fDelay = 0;
            cmd1.fishType = (FishType)configs[cmd1.nType].type;
            cmd1.resource = configs[cmd1.nType].resource;
            cmd1.sound = configs[cmd1.nType].sound;
            cmdArray.push_back(cmd1);
//            fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//            fish.SetTypeID(FISH_TYPE_EX_19);
//            fish.SetPathID(181);
//            fish.SetData(-1);
//            fish.SetPathType(FISH_ARRAY_PATH_EX3);
//            fish.SetSpeed(1.0f);
//            m_Fishs.push_back(fish);
        }
            break;
        case 2:	//! 鱼阵2
        {
            fSpeed = 1.0;
            float fDelay = 0.0f;
            
            //! 两条魔鬼鱼开路
            for (int i = 0; i < 2; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_15;
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = fSpeed;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = 0;
                cmd.OffestY = (i == 0 ? 50 : -50);
                cmd.fDelay = 0;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmdArray.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_15);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetSpeed(fSpeed);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                offset.y_ = (i == 0 ? 50 : -50);
//                fish.SetOffest(offset);
//                m_Fishs.push_back(fish);
            }
            
            fDelay = 0.3f;
            for (int i = 0; i < 25; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_17;
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = fSpeed;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = 0;
                cmd.OffestY = 0;
                cmd.fDelay = fDelay;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_17);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                fish.SetDelay(fDelay);
//                offset.y_ = 0;
//                fish.SetSpeed(fSpeed);
//                m_Fishs.push_back(fish);
                fDelay += 1.8f;
            }
            
            fDelay = 0.4f;
            for (int i = 0; i < 50; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_13;
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = fSpeed;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = 0;
                cmd.OffestY = 100;
                cmd.fDelay = fDelay;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_13);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                offset.y_ =  100;
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(fSpeed);
//                fish.SetOffest(offset);
//                m_Fishs.push_back(fish);
                fDelay += 1.0f;
            }
            
            fDelay = 0.4f;
            for (int i = 0; i < 50; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_13;
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = fSpeed;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = 0;
                cmd.OffestY = -100;
                cmd.fDelay = fDelay;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_13);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                offset.y_ =  -100;
//                fish.SetDelay(fDelay);
//                fish.SetOffest(offset);
//                fish.SetSpeed(fSpeed);
//                m_Fishs.push_back(fish);
                fDelay += 1.0f;
            }
            
            fDelay = 0.5f;
            for (int i = 0; i < 50; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_6;
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = fSpeed;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = 0;
                cmd.OffestY = 150;
                cmd.fDelay = fDelay;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_6);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                offset.y_ =  150;
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(fSpeed);
//                fish.SetOffest(offset);
//                m_Fishs.push_back(fish);
                fDelay += 1.0f;
            }
            
            fDelay = 0.5f;
            for (int i = 0; i < 50; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_6;
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = fSpeed;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = 0;
                cmd.OffestY = -150;
                cmd.fDelay = fDelay;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_6);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                offset.y_ =  -150;
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(fSpeed);
//                fish.SetOffest(offset);
//                m_Fishs.push_back(fish);
                fDelay +=1.0f;
            }
            
            fDelay = 0.7f;
            for (int i = 0; i < 50; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_1;
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = fSpeed;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = 0;
                cmd.OffestY = 200;
                cmd.fDelay = fDelay;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_1);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                offset.y_ =  200;
//                fish.SetDelay(fDelay);
//                fish.SetOffest(offset);
//                fish.SetSpeed(fSpeed);
//                m_Fishs.push_back(fish);
                fDelay +=1.0f;
            }
            
            fDelay = 0.7f;
            for (int i = 0; i < 50; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_1;
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.nElaspe = 0;
                cmd.fSpeed = fSpeed;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.nData = -1;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.interval = 0;
                cmd.OffestX = 0;
                cmd.OffestY = -200;
                cmd.fDelay = fDelay;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_1);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                offset.y_ =  -200;
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(fSpeed);
//                fish.SetOffest(offset);
//                m_Fishs.push_back(fish);
                fDelay += 1.0f;
            }
        }
            break;
        case 3:
        {
            float fDelay = 0.0f;
            
            //! 鱼雨
            for (int i = 0; i < 100; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_1;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.nElaspe = 0;
                cmd.interval = 0;
                
                cmd.nPathID = 2;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.fSpeed = 1;
                cmd.nData = -1;
                cmd.OffestX = abs(rand())%200+1;
                cmd.OffestY = 0;
                cmd.fDelay = fDelay;
                
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                
//                //! 左上右下
//                offset.x_ = RandInt(1, 200);
//                fish.SetPathID(2);
//                
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(1.00f);
//                m_Fishs.push_back(fish);
                
                
                CMD_S_CREATE_FISH cmd1;
                cmd1.dropThres = 0;
                cmd1.groupId = -1;
                cmd1.groupType = NONE_GROUP;
                cmd1.nType = FISH_TYPE_EX_1;
                cmd1.randThres = configs[cmd1.nType].probability;
                cmd1.Score = configs[cmd1.nType].price;
                cmd1.fishType = (FishType)configs[cmd1.nType].type;
                cmd1.resource = configs[cmd1.nType].resource;
                cmd1.collideRect = configs[cmd1.nType].rect;
                cmd1.sound = configs[cmd1.nType].sound;
                cmd1.nElaspe = 0;
                cmd1.interval = 0;
                
                cmd1.nPathID = 2;
                cmd1.nPathType = FISH_ARRAY_PATH;
                cmd1.fSpeed = 1;
                cmd1.nData = -1;
                cmd1.OffestX = abs(rand())%200+200;
                cmd1.OffestY = 0;
                cmd1.fDelay = fDelay;
                delayCreateCmd.push_back(cmd1);
                
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                
//                //! 左上右下
//                offset.x_ = RandInt(200, 400);
//                fish.SetPathID(2);
//                
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(1.00f);
//                m_Fishs.push_back(fish);
                
                CMD_S_CREATE_FISH cmd2;
                cmd2.dropThres = 0;
                cmd2.groupId = -1;
                cmd2.groupType = NONE_GROUP;
                cmd2.nType = FISH_TYPE_EX_1;
                cmd2.randThres = configs[cmd2.nType].probability;
                cmd2.Score = configs[cmd2.nType].price;
                cmd2.fishType = (FishType)configs[cmd2.nType].type;
                cmd2.resource = configs[cmd2.nType].resource;
                cmd2.sound = configs[cmd2.nType].sound;
                cmd2.collideRect = configs[cmd2.nType].rect;
                cmd2.nElaspe = 0;
                cmd2.interval = 0;
                
                cmd2.nPathID = 2;
                cmd2.nPathType = FISH_ARRAY_PATH;
                cmd2.fSpeed = 1;
                cmd2.nData = -1;
                cmd2.OffestX = abs(rand())%200+400;
                cmd2.OffestY = 0;
                cmd2.fDelay = fDelay;
                delayCreateCmd.push_back(cmd2);
                
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                
//                //! 左上右下
//                offset.x_ = RandInt(400, 600);
//                fish.SetPathID(2);
//                
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(1.00f);
//                m_Fishs.push_back(fish);
                
                CMD_S_CREATE_FISH cmd3;
                cmd3.dropThres = 0;
                cmd3.groupId = -1;
                cmd3.groupType = NONE_GROUP;
                cmd3.nType = FISH_TYPE_EX_1;
                cmd3.randThres = configs[cmd3.nType].probability;
                cmd3.Score = configs[cmd3.nType].price;
                cmd3.fishType = (FishType)configs[cmd3.nType].type;
                cmd3.resource = configs[cmd3.nType].resource;
                cmd3.sound = configs[cmd3.nType].sound;
                cmd3.collideRect = configs[cmd3.nType].rect;
                cmd3.nElaspe = 0;
                cmd3.interval = 0;
                
                cmd3.nPathID = 2;
                cmd3.nPathType = FISH_ARRAY_PATH;
                cmd3.fSpeed = 1;
                cmd3.nData = -1;
                cmd3.OffestX = abs(rand())%200+600;
                cmd3.OffestY = 0;
                cmd3.fDelay = fDelay;
                delayCreateCmd.push_back(cmd3);
                
//                offset.x_ = RandInt(600, 800);
//                fish.SetPathID(2);
//                
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(1.00f);
//                m_Fishs.push_back(fish);
                
                //! 刷向上的小鱼
                CMD_S_CREATE_FISH cmd4;
                cmd4.dropThres = 0;
                cmd4.groupId = -1;
                cmd4.groupType = NONE_GROUP;
                cmd4.nType = FISH_TYPE_EX_1;
                cmd4.randThres = configs[cmd4.nType].probability;
                cmd4.Score = configs[cmd4.nType].price;
                cmd4.fishType = (FishType)configs[cmd4.nType].type;
                cmd4.resource = configs[cmd4.nType].resource;
                cmd4.sound = configs[cmd4.nType].sound;
                cmd4.collideRect = configs[cmd4.nType].rect;
                cmd4.nElaspe = 0;
                cmd4.interval = 0;
                
                cmd4.nPathID = 3;
                cmd4.nPathType = FISH_ARRAY_PATH;
                cmd4.fSpeed = 1;
                cmd4.nData = -1;
                cmd4.OffestX = (abs(rand())%200+1)*-1;
                cmd4.OffestY = 0;
                cmd4.fDelay = fDelay;
                delayCreateCmd.push_back(cmd4);
                
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                
//                offset.x_ = (RandInt(1, 200) * -1);
//                fish.SetPathID(3);
//                
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(1.00f);
//                m_Fishs.push_back(fish);
                
                CMD_S_CREATE_FISH cmd5;
                cmd5.dropThres = 0;
                cmd5.groupId = -1;
                cmd5.groupType = NONE_GROUP;
                cmd5.nType = FISH_TYPE_EX_1;
                cmd5.randThres = configs[cmd5.nType].probability;
                cmd5.Score = configs[cmd5.nType].price;
                cmd5.fishType = (FishType)configs[cmd5.nType].type;
                cmd5.resource = configs[cmd5.nType].resource;
                cmd5.sound = configs[cmd5.nType].sound;
                cmd5.collideRect = configs[cmd5.nType].rect;
                cmd5.nElaspe = 0;
                cmd5.interval = 0;
                
                cmd5.nPathID = 3;
                cmd5.nPathType = FISH_ARRAY_PATH;
                cmd5.fSpeed = 1;
                cmd5.nData = -1;
                cmd5.OffestX = (abs(rand())%200+200)*-1;
                cmd5.OffestY = 0;
                cmd5.fDelay = fDelay;
                delayCreateCmd.push_back(cmd5);
                
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                
//                offset.x_ = (RandInt(200, 400) * -1);
//                fish.SetPathID(3);
//                
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(1.00f);
//                m_Fishs.push_back(fish);
                
                CMD_S_CREATE_FISH cmd6;
                cmd6.dropThres = 0;
                cmd6.groupId = -1;
                cmd6.groupType = NONE_GROUP;
                cmd6.nType = FISH_TYPE_EX_1;
                cmd6.randThres = configs[cmd6.nType].probability;
                cmd6.Score = configs[cmd6.nType].price;
                cmd6.fishType = (FishType)configs[cmd6.nType].type;
                cmd6.resource = configs[cmd6.nType].resource;
                cmd6.sound = configs[cmd6.nType].sound;
                cmd6.collideRect = configs[cmd6.nType].rect;
                cmd6.nElaspe = 0;
                cmd6.interval = 0;
                
                cmd6.nPathID = 3;
                cmd6.nPathType = FISH_ARRAY_PATH;
                cmd6.fSpeed = 1;
                cmd6.nData = -1;
                cmd6.OffestX = (abs(rand())%200+400)*-1;
                cmd6.OffestY = 0;
                cmd6.fDelay = fDelay;
                delayCreateCmd.push_back(cmd6);
                
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                
//                offset.x_ = (RandInt(400, 600) * -1);
//                fish.SetPathID(3);
//                
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(1.00f);
//                m_Fishs.push_back(fish);
                
                CMD_S_CREATE_FISH cmd7;
                cmd7.dropThres = 0;
                cmd7.groupId = -1;
                cmd7.groupType = NONE_GROUP;
                cmd7.nType = FISH_TYPE_EX_1;
                cmd7.randThres = configs[cmd7.nType].probability;
                cmd7.Score = configs[cmd7.nType].price;
                cmd7.fishType = (FishType)configs[cmd7.nType].type;
                cmd7.resource = configs[cmd7.nType].resource;
                cmd7.sound = configs[cmd7.nType].sound;
                cmd7.collideRect = configs[cmd7.nType].rect;
                cmd7.nElaspe = 0;
                cmd7.interval = 0;
                
                cmd7.nPathID = 3;
                cmd7.nPathType = FISH_ARRAY_PATH;
                cmd7.fSpeed = 1;
                cmd7.nData = -1;
                cmd7.OffestX = (abs(rand())%200+600)*-1;
                cmd7.OffestY = 0;
                cmd7.fDelay = fDelay;
                delayCreateCmd.push_back(cmd7);
                
//                offset.x_ = (RandInt(600, 800) * -1);
//                fish.SetPathID(3);
//                
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetDelay(fDelay);
//                fish.SetSpeed(1.00f);
//                m_Fishs.push_back(fish);
                
                fDelay += 0.5f;
            }
            
            
            fDelay = 0.0f;
            for (int i = 0; i < 15; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = abs(rand())%4+FISH_TYPE_EX_16;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.nElaspe = 0;
                cmd.interval = 0;
                
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.fSpeed = 1;
                cmd.nData = -1;
                cmd.OffestX = 100;
                cmd.OffestY = abs(rand())%300-150;
                cmd.fDelay = fDelay;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(RandInt(FISH_TYPE_EX_16, FISH_TYPE_EX_19));
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                offset.x_ = 100;
//                offset.y_ =  RandInt(-150, 150);
//                fish.SetDelay(fDelay);
//                fish.SetOffest(offset);
//                m_Fishs.push_back(fish);
                fDelay += 3.0f;
            }
            
        }
            break;
        case 4:
        {
            fSpeed = 2.0;
            for (int i = 0; i < 205; i++)
            {
                if (i >= 0 && i < 100)
                {
                    //! 最外面的一个圈
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_1;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_1);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i == 100)
                {
                    //! 圈中间的乌龟
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_10;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);

//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_10);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i > 100 && i <= 120)
                {
                    //! 小圈2
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_3;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_3);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i == 121)
                {
                    //! 圈中间的乌龟
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_10;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_10);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i > 121 && i <= 141)
                {
                    //! 小圈3
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_4;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_4);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i == 142)
                {
                    //! 圈中间的乌龟
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_10;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_10);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i > 142 && i <= 162)
                {
                    //! 小圈4
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_5;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_5);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i == 163)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_10;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    
                    //! 圈中间的乌龟
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_10);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i > 163 && i <= 183)
                {
                    //! 小圈5
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_6;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_6);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i == 184)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_10;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    //! 圈中间的乌龟
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_10);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i > 184 && i <= 204)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_9;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX2;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    //! 小圈1
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_9);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX2);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
            }
        }
            break;	
        case 5:
        {
            fSpeed = 1.0;
            float fDelay = 0.0f;
            //! 上下两排都是乌龟
            for (int i = 0; i < 50; i++)
            {
                //! 每秒刷四只乌龟
//                CFish fish;
                if(false)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_10;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.nElaspe = 0;
                    cmd.interval = 0;
                    
                    cmd.nPathID = 0;
                    cmd.nPathType = FISH_ARRAY_PATH;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 250;
                    cmd.fDelay = fDelay;
                    delayCreateCmd.push_back(cmd);
                    
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_10);
//                    fish.SetPathID(0);
//                    offset.y_ = 250;
//                    fish.SetOffest(offset);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH);
//                    fish.SetSpeed(fSpeed);
//                    fish.SetDelay(fDelay);
//                    m_Fishs.push_back(fish);
                }
                
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_10;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.nElaspe = 0;
                cmd.interval = 0;
                
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.fSpeed = fSpeed;
                cmd.nData = -1;
                cmd.OffestX = 0;
                cmd.OffestY = 150;
                cmd.fDelay = fDelay;
                delayCreateCmd.push_back(cmd);
                
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_10);
//                fish.SetPathID(0);
//                offset.y_ = 150;
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                fish.SetSpeed(fSpeed);
//                fish.SetDelay(fDelay);
//                m_Fishs.push_back(fish);
                
                CMD_S_CREATE_FISH cmd1;
                cmd1.dropThres = 0;
                cmd1.groupId = -1;
                cmd1.groupType = NONE_GROUP;
                cmd1.nType = FISH_TYPE_EX_10;
                cmd1.randThres = configs[cmd1.nType].probability;
                cmd1.Score = configs[cmd1.nType].price;
                cmd1.fishType = (FishType)configs[cmd1.nType].type;
                cmd1.resource = configs[cmd1.nType].resource;
                cmd1.collideRect = configs[cmd1.nType].rect;
                cmd1.sound = configs[cmd1.nType].sound;
                cmd1.nElaspe = 0;
                cmd1.interval = 0;
                
                cmd1.nPathID = 0;
                cmd1.nPathType = FISH_ARRAY_PATH;
                cmd1.fSpeed = fSpeed;
                cmd1.nData = -1;
                cmd1.OffestX = 0;
                cmd1.OffestY = -150;
                cmd1.fDelay = fDelay;
                delayCreateCmd.push_back(cmd1);
                
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_10);
//                fish.SetPathID(0);
//                offset.y_ = -150;
//                fish.SetOffest(offset);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                fish.SetSpeed(fSpeed);
//                fish.SetDelay(fDelay);
//                m_Fishs.push_back(fish);
                
                if(false)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_10;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.nElaspe = 0;
                    cmd.interval = 0;
                    
                    cmd.nPathID = 0;
                    cmd.nPathType = FISH_ARRAY_PATH;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = -250;
                    cmd.fDelay = fDelay;
                    delayCreateCmd.push_back(cmd);
                    
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_10);
//                    fish.SetPathID(0);
//                    offset.y_ = -250;
//                    fish.SetOffest(offset);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH);
//                    fish.SetSpeed(fSpeed);
//                    fish.SetDelay(fDelay);
//                    m_Fishs.push_back(fish);
                }
                fDelay += 1.0f;
            }
            
            //! 中间一排鲸鱼
            fDelay = 0.5f;
            for (int i = 0; i < 25; i++)
            {
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = FISH_TYPE_EX_17;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.nElaspe = 0;
                cmd.interval = 0;
                
                cmd.nPathID = 0;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.fSpeed = fSpeed;
                cmd.nData = -1;
                cmd.OffestX = 0;
                cmd.OffestY = 0;
                cmd.fDelay = fDelay;
                delayCreateCmd.push_back(cmd);
                
//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(FISH_TYPE_EX_17);
//                fish.SetPathID(0);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                fish.SetSpeed(fSpeed);
//                fish.SetDelay(fDelay);
//                m_Fishs.push_back(fish);
                
                fDelay += 2.0f;
            }
            
        }
            break;
        case 6:
        {
            fSpeed = 2.0;
            for(int i = 0; i< 242; i++)
            {
                if (i < 50 && i >= 0)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_3;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX1;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);
                    
//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_3);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX1);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i >= 50 && i < 90)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_4;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX1;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);

//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_4);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX1);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i >= 90 && i < 120)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_9;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX1;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);

//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_9);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX1);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i == 120)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_19;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX1;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);

//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_19);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX1);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i > 120 && i <= 170)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_5;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX1;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);

//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_5);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX1);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i > 170 && i <= 210)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_2;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX1;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);

//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_2);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX1);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i > 210 && i <= 240)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_7;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX1;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);

//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_7);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX1);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
                
                if (i == 241)
                {
                    CMD_S_CREATE_FISH cmd;
                    cmd.dropThres = 0;
                    cmd.groupId = -1;
                    cmd.groupType = NONE_GROUP;
                    cmd.nType = FISH_TYPE_EX_18;
                    cmd.randThres = configs[cmd.nType].probability;
                    cmd.Score = configs[cmd.nType].price;
                    cmd.collideRect = configs[cmd.nType].rect;
                    cmd.fishType = (FishType)configs[cmd.nType].type;
                    cmd.resource = configs[cmd.nType].resource;
                    cmd.sound = configs[cmd.nType].sound;
                    cmd.interval = 0;
                    cmd.nElaspe = 0;
                    
                    cmd.nPathID = i;
                    cmd.nPathType = FISH_ARRAY_PATH_EX1;
                    cmd.fSpeed = fSpeed;
                    cmd.nData = -1;
                    cmd.OffestX = 0;
                    cmd.OffestY = 0;
                    cmd.fDelay = 0;
                    cmdArray.push_back(cmd);

//                    CFish fish;
//                    fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                    fish.SetTypeID(FISH_TYPE_EX_18);
//                    fish.SetPathID(i);
//                    fish.SetData(-1);
//                    fish.SetPathType(FISH_ARRAY_PATH_EX1);
//                    fish.SetSpeed(fSpeed);
//                    m_Fishs.push_back(fish);
                }
            }
            
        }
            break;
        case 7:
        {
            //! X鱼阵
            fSpeed = 1.2;
            float fDelay = 0.0f;
            for (int i = 0; i < 10; i++)
            {
//                int nRandFish = RandInt(FISH_TYPE_EX_13, FISH_TYPE_EX_17);
                int nRandFish = abs(rand()) % 5 + FISH_TYPE_EX_13;
                int nPathID = 4;
                CMD_S_CREATE_FISH cmd;
                cmd.dropThres = 0;
                cmd.groupId = -1;
                cmd.groupType = NONE_GROUP;
                cmd.nType = nRandFish;
                cmd.randThres = configs[cmd.nType].probability;
                cmd.Score = configs[cmd.nType].price;
                cmd.fishType = (FishType)configs[cmd.nType].type;
                cmd.resource = configs[cmd.nType].resource;
                cmd.sound = configs[cmd.nType].sound;
                cmd.collideRect = configs[cmd.nType].rect;
                cmd.nElaspe = 0;
                cmd.interval = 0;
                
                cmd.nPathID = nPathID;
                cmd.nPathType = FISH_ARRAY_PATH;
                cmd.fSpeed = fSpeed;
                cmd.nData = -1;
                cmd.OffestX = 0;
                cmd.OffestY = 0;
                cmd.fDelay = fDelay;
                delayCreateCmd.push_back(cmd);

//                CFish fish;
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(nRandFish);
//                fish.SetPathID(nPathID);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                fish.SetSpeed(fSpeed);
//                fish.SetDelay(fDelay);
//                m_Fishs.push_back(fish);
                
                nPathID = 5;
                CMD_S_CREATE_FISH cmd1;
                cmd1.dropThres = 0;
                cmd1.groupId = -1;
                cmd1.groupType = NONE_GROUP;
                cmd1.nType = nRandFish;
                cmd1.randThres = configs[cmd1.nType].probability;
                cmd1.Score = configs[cmd1.nType].price;
                cmd1.fishType = (FishType)configs[cmd1.nType].type;
                cmd1.resource = configs[cmd1.nType].resource;
                cmd1.collideRect = configs[cmd1.nType].rect;
                cmd1.sound = configs[cmd1.nType].sound;
                cmd1.nElaspe = 0;
                cmd1.interval = 0;
                
                cmd1.nPathID = nPathID;
                cmd1.nPathType = FISH_ARRAY_PATH;
                cmd1.fSpeed = fSpeed;
                cmd1.nData = -1;
                cmd1.OffestX = 0;
                cmd1.OffestY = 0;
                cmd1.fDelay = fDelay;
                delayCreateCmd.push_back(cmd1);

//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(nRandFish);
//                fish.SetPathID(nPathID);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                fish.SetSpeed(fSpeed);
//                fish.SetDelay(fDelay);
//                m_Fishs.push_back(fish);
                
                nPathID = 6;
                CMD_S_CREATE_FISH cmd2;
                cmd2.dropThres = 0;
                cmd2.groupId = -1;
                cmd2.groupType = NONE_GROUP;
                cmd2.nType = nRandFish;
                cmd2.randThres = configs[cmd2.nType].probability;
                cmd2.Score = configs[cmd2.nType].price;
                cmd2.fishType = (FishType)configs[cmd2.nType].type;
                cmd2.resource = configs[cmd2.nType].resource;
                cmd2.collideRect = configs[cmd2.nType].rect;
                cmd2.sound = configs[cmd2.nType].sound;
                cmd2.nElaspe = 0;
                cmd2.interval = 0;
                
                cmd2.nPathID = nPathID;
                cmd2.nPathType = FISH_ARRAY_PATH;
                cmd2.fSpeed = fSpeed;
                cmd2.nData = -1;
                cmd2.OffestX = 0;
                cmd2.OffestY = 0;
                cmd2.fDelay = fDelay;
                delayCreateCmd.push_back(cmd2);
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(nRandFish);
//                fish.SetPathID(nPathID);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                fish.SetSpeed(fSpeed);
//                fish.SetDelay(fDelay);
//                m_Fishs.push_back(fish);
                
                nPathID = 7;
                CMD_S_CREATE_FISH cmd3;
                cmd3.dropThres = 0;
                cmd3.groupId = -1;
                cmd3.groupType = NONE_GROUP;
                cmd3.nType = nRandFish;
                cmd3.randThres = configs[cmd3.nType].probability;
                cmd3.Score = configs[cmd3.nType].price;
                cmd3.fishType = (FishType)configs[cmd3.nType].type;
                cmd3.resource = configs[cmd3.nType].resource;
                cmd3.collideRect = configs[cmd3.nType].rect;
                cmd3.sound = configs[cmd3.nType].sound;
                cmd3.nElaspe = 0;
                cmd3.interval = 0;
                
                cmd3.nPathID = nPathID;
                cmd3.nPathType = FISH_ARRAY_PATH;
                cmd3.fSpeed = fSpeed;
                cmd3.nData = -1;
                cmd3.OffestX = 0;
                cmd3.OffestY = 0;
                cmd3.fDelay = fDelay;
                delayCreateCmd.push_back(cmd3);
//                fish.SetID(CIDGenerate::GetInstance()->GetNewID());
//                fish.SetTypeID(nRandFish);
//                fish.SetPathID(nPathID);
//                fish.SetData(-1);
//                fish.SetPathType(FISH_ARRAY_PATH);
//                fish.SetSpeed(fSpeed);
//                fish.SetDelay(fDelay);
//                m_Fishs.push_back(fish);
                fDelay += 5.0f;
            }
        }
            break;
    }
    
    for (auto cmd : cmdArray) {
        createNewFish(cmd);
    }
}

std::string FishManager::getFishName(int fishTypeId)
{
    return configs[fishTypeId].name;
}

std::string FishManager::getFishRes(int fishTypeId)
{
//    if (find(configs.begin(), configs.end(), fishTypeId) == configs.end()) {
//        return "";
//    }
    return configs[fishTypeId].resource;
}

std::string FishManager::getFishSound(int fishTypeId)
{
    return configs[fishTypeId].sound;
}

int FishManager::getFishType(int fishTypeId)
{
//    if (find(configs.begin(), configs.end(), fishTypeId) == configs.end()) {
//        return -1;
//    }
    return configs[fishTypeId].type;
}