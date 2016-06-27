//
//  BlackGoldGame.cpp
//  buyu_simple
//
//  Created by ff on 15/12/23.
//
//

#include "BlackGoldGame.h"
#include "utils/GameReader.h"
#include "EventDef.h"
#include "GameDef.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> BlackGoldGame::idTable;

BlackGoldGame::BlackGoldGame():
money(10000),
stageMoney(0),
speedScale(1),
goldScale(1),
diamondScale(1),
luckScale(1),
gameTime(60),
difficulty(0),
challengeLimit(MAX_GOLD_CHALLENGE_TIMES),
challengeFee(GOLD_CHALLENGE_FEE1),
challengeCount(0),
tempChallengeCount(0),
diff1Stage(1),
//diff2Stage(1),
//diff3Stage(1),
stageBoxState(0),
fateBoxState(0),
hasRolled(false),
sucNextStage(true),
sceneId(1)
{
    itemCount[GOLD_ITEM_BOMB_TYPE] = 3;
    itemCount[GOLD_ITEM_KEY_TYPE] = 0;
    touchThingsListener = EventListenerCustom::create(FIND_SOMETHING, CC_CALLBACK_1(BlackGoldGame::onTouchThings, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchThingsListener, -1);
}

BlackGoldGame::~BlackGoldGame()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(touchThingsListener);
}



void BlackGoldGame::onTouchThings(cocos2d::EventCustom* event)
{
    std::vector<int> info = *static_cast<std::vector<int>*>(event->getUserData());
    int foundObjId = -1;
    for (auto obj: info) {
        foundObjId = obj;
        int type = GoldObjs[obj].getTypeId();
        if (type >= 1 && type <= 2) {
            break;
        }else if (GoldObjs[obj].getMoveType() == 1){
            break;
        }
    }
    if (foundObjId != -1) {
        if (GoldObjs[foundObjId].getTypeId() == 14) {
            EventCustom event1(PILERS_BOMB);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
            
            GoldObjs[foundObjId].setAlive(false);
            aliveCount--;
            
            for (int i = 0 ; i<GoldObjs.size(); i++) {
                if (!GoldObjs[i].getAlive()) {
                    continue;
                }
                
                if (i == foundObjId) {
                    continue;
                }
                
                if (GoldObjs[i].getPos().getDistance(GoldObjs[foundObjId].getPos()) > 50) {
                    continue;
                }
                
                GoldObjs[i].setAlive(false);
                aliveCount--;
            }
        }else{
            EventCustom event1(PLIERS_FIND_THING);
            event1.setUserData(&GoldObjs[foundObjId]);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
            
            GoldObjs[foundObjId].setAlive(false);
            aliveCount--;            
        }
        

    }
    
}


void BlackGoldGame::initGrids()
{
    for (int i = 0; i < MAX_COL; i++) {
        zoneGridX[0][i] = MAX_COL-i;
    }
    
    for (int i = 1; i < MAX_ZONE_ROW; i++) {
        memcpy(zoneGridX[i], zoneGridX[0], sizeof(int) * MAX_COL);
    }
    
    for (int i = 0; i < MAX_ZONE_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            zoneGridY[i][j] = MAX_ZONE_ROW - i;
        }
    }
    
    for (int i = 0; i < MAX_ZONE_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("%d ", zoneGridX[i][j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < MAX_ZONE_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("%d ", zoneGridY[i][j]);
        }
        printf("\n");
    }
    
}

bool BlackGoldGame::arrangeObj(float zoneOffY, std::vector<GoldObj> &Objs)
{
    bool arrangeSuc = false;
    int arrangeTimes = 0;
    
    srand((unsigned int)time(NULL));
    while (arrangeTimes < 3 && !arrangeSuc) {
        initGrids();
        for (int i = 0; i<Objs.size(); i++) {
            arrangeSuc &= arrangeOneObj(Objs[i], zoneOffY);
        }
        
        if (!arrangeSuc) {
            arrangeTimes++;
        }
    }
    if (!arrangeSuc) {
        cocos2d::log(">>>>arrange try 3 times, but failed. Lost some obj, save the others.");
    }
    
    for (std::vector<GoldObj>::iterator it = Objs.begin(); it != Objs.end();) {
        if (!it->getArrangeInside()) {
            it = Objs.erase(it);
        }else{
            it++;
        }
    }
    
    return arrangeSuc;
}

bool BlackGoldGame::arrangeOneObj(GoldObj &obj, float zoneOffY)
{
    if (obj.getMoveType() == 1) {
        int x = abs(rand()%(MAX_COL*GRID_SIZE));
        int y = abs(rand()%(MAX_ZONE_ROW*GRID_SIZE));
        
        obj.setPos(cocos2d::Vec2(x, zoneOffY-y));
        obj.setArrangeInside(true);
        return true;
    }
    
    cocos2d::Size gridSize = obj.getGridSize();
    int objWidth = gridSize.width;
    int objHeight = gridSize.height;
    
    std::vector<GRID_INFO> tempVec;
    for (int i = 0; i <= MAX_ZONE_ROW - objHeight; i++) {
        for (int j = 0; j <= MAX_COL - objWidth; j++) {
            if (zoneGridX[i][j] >= objWidth && zoneGridY[i][j] >= objHeight) {
                bool found = true;
                for (int k = 1; k < objHeight; k++) {
                    if(zoneGridX[i+k][j] < objWidth)
                    {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    GRID_INFO info;
                    info.x = j;
                    info.y = i;
                    info.xVal = zoneGridX[i][j];
                    info.yVal = zoneGridY[i][j];
                    tempVec.push_back(info);
                }
                
            }
        }
    }
    
    if (tempVec.empty()) {
        obj.setArrangeInside(false);
        return false;
    }
    

    int selGridIdx = abs(rand())%tempVec.size();
    GRID_INFO selNode = tempVec[selGridIdx];
    for (int i = selNode.y; i < selNode.y+objHeight; i++) {
        for (int j = selNode.x; j < selNode.x+objWidth; j++) {
            zoneGridX[i][j] = 0;
            zoneGridY[i][j] = 0;
            
            //            GRID_INFO val;
            //            val.x = j;
            //            val.y = i;
            //            auto t = find_if(tempVec.begin(), tempVec.end(), GRID_INFO(val));
            //            if (t != tempVec.end()) {
            //                tempVec.erase(t);
            //            }
        }
    }
    
    for (int j = selNode.x; j < selNode.x+objWidth; j++) {
        for (int i = selNode.y-1, h = 1; i >= 0; i--, h++) {
            if (zoneGridY[i][j] <= 0) {
                break;
            }else{
                zoneGridY[i][j] = h;
            }
        }
    }
    
    for (int i = selNode.y; i < selNode.y + objHeight; i++) {
        for (int j = selNode.x-1, w = 1; j >= 0; j--, w++) {
            if (zoneGridX[i][j] <= 0) {
                break;
            }else{
                zoneGridX[i][j] = w;
            }
        }
    }
    
    for (int i = 0; i < MAX_ZONE_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("%d ", zoneGridX[i][j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < MAX_ZONE_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("%d ", zoneGridY[i][j]);
        }
        printf("\n");
    }
    
    
    cocos2d::Vec2 lt = cocos2d::Vec2(selNode.x*GRID_SIZE, zoneOffY-selNode.y*GRID_SIZE);
    cocos2d::Vec2 rb = cocos2d::Vec2((selNode.x+objWidth)*GRID_SIZE, zoneOffY-(selNode.y+objHeight)*GRID_SIZE);
    cocos2d::Vec2 md = (rb).getMidpoint(lt);
    
    obj.setPos(md);
    obj.setArrangeInside(true);
    return true;
    
}

void BlackGoldGame::readFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(BlackGoldGame::idTable.empty())
    {
        BlackGoldGame::initIdTable(*doc);
    }
    initData(idx, *doc);
}

bool BlackGoldGame::initData(int idx, rapidjson::Document& _doc)
{
    int index = idTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    clearData();
    
    sceneId = DICTOOL->getIntValue_json(dic, "ID");
    bgRes = DICTOOL->getStringValue_json(dic, "bg");
    maskRes = DICTOOL->getStringValue_json(dic, "mask");
    
    missionType = DICTOOL->getIntValue_json(dic, "missionType");
    missionGold = DICTOOL->getIntValue_json(dic, "missionGold");
    missionKeyCount = DICTOOL->getIntValue_json(dic, "missionKeyCount");
    
    std::string temp = DICTOOL->getStringValue_json(dic, "A-zone");
    std::vector<std::string> objVec = GameUtils::splitStr(temp, ";");
    for (int i = 0; i < objVec.size(); i++) {
        std::vector<std::string> objPair = GameUtils::splitStr(objVec[i], "|");
        int objId = atoi(objPair[0].c_str());
        int objCount = atoi(objPair[1].c_str());
        
        
        for (int j=0; j<objCount; j++) {
            GoldObj gold;
            gold.readFile(objId, GOLD_OBJ_CONFIG_FILE);
            AGoldObjs.push_back(gold);
        }
        
    }
    
    arrangeObj(MAX_ZONE_ROW*GRID_SIZE*2 + A_ZONE_ROW*GRID_SIZE, AGoldObjs);
    objVec.clear();

    temp = DICTOOL->getStringValue_json(dic, "B-zone");
    objVec = GameUtils::splitStr(temp, ";");
    for (int i = 0; i < objVec.size(); i++) {
        std::vector<std::string> objPair = GameUtils::splitStr(objVec[i], "|");
        int objId = atoi(objPair[0].c_str());
        int objCount = atoi(objPair[1].c_str());
        
        
        for (int j=0; j<objCount; j++) {
            GoldObj gold;
            gold.readFile(objId, GOLD_OBJ_CONFIG_FILE);
            BGoldObjs.push_back(gold);
        }
        
    }
    
    arrangeObj(MAX_ZONE_ROW*GRID_SIZE*2, BGoldObjs);
    objVec.clear();
    
    temp = DICTOOL->getStringValue_json(dic, "C-zone");
    objVec = GameUtils::splitStr(temp, ";");
    for (int i = 0; i < objVec.size(); i++) {
        std::vector<std::string> objPair = GameUtils::splitStr(objVec[i], "|");
        int objId = atoi(objPair[0].c_str());
        int objCount = atoi(objPair[1].c_str());
        
        
        for (int j=0; j<objCount; j++) {
            GoldObj gold;
            gold.readFile(objId, GOLD_OBJ_CONFIG_FILE);
            CGoldObjs.push_back(gold);
        }
        
    }
    
    arrangeObj(MAX_ZONE_ROW*GRID_SIZE, CGoldObjs);
    
    GoldObjs.clear();
    for (int i = 0; i < AGoldObjs.size(); i++)
    {
        GoldObjs[i] = AGoldObjs[i];
        GoldObjs[i].setSerNo(i);
    }
    int count = (int)AGoldObjs.size();
    for (int i = 0; i < BGoldObjs.size(); i++)
    {
        GoldObjs[count+i] = BGoldObjs[i];
        GoldObjs[count+i].setSerNo(count+i);
    }
    count += BGoldObjs.size();
    for (int i = 0; i < CGoldObjs.size(); i++)
    {
        GoldObjs[count+i] = CGoldObjs[i];
        GoldObjs[count+i].setSerNo(count+i);
    }
    
    aliveCount = (int)GoldObjs.size();
    
//    itemCount[GOLD_ITEM_BOMB_TYPE] = 3;
//    itemCount[GOLD_ITEM_KEY_TYPE] = 0;
////    money = 0;
//    speedScale = 1;
//    goldScale = 1;
//    diamondScale = 1;
//    luckScale = 1;
//    gameTime = 5;
    return true;
    
}

void BlackGoldGame::clearData()
{
    speedScale = 1;
    goldScale = 1;
    diamondScale = 1;
    luckScale = 1;
    gameTime = 60;
    stageMoney = 0;
    GoldObjs.clear();
    AGoldObjs.clear();
    BGoldObjs.clear();
    CGoldObjs.clear();
}

void BlackGoldGame::initIdTable(rapidjson::Document &_doc)
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

void BlackGoldGame::addItem(GOLD_ITEM_TYPE type, bool inGame)
{
    switch (type) {
        case GOLD_ITEM_BEAUTY_TYPE:
            useItem(type);
            break;
            
        case GOLD_ITEM_KEY_TYPE:
        {
            itemCount[type]++;
            if (itemCount[type] >= 5) {
                itemCount[type] = 5;
                if (fateBoxState == 0) {
                    fateBoxState = 1;
                }
            }
            
            EventCustom event1(ADD_KEY);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
        }
            break;
          
        case GOLD_ITEM_BOMB_TYPE:
        {
            itemCount[type]++;
            EventCustom event1(ADD_BOMB);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event1);
        }
            break;
            
        default:
            if (!inGame) {
                itemCount[type] = 1;
            }else{
                useItem(type);
            }
            break;
    }
}

void BlackGoldGame::useItem(GOLD_ITEM_TYPE type)
{
    switch (type) {
        case GOLD_ITEM_BEAUTY_TYPE:
        {
            money -= abs(rand())%500 + 500;
            money = MAX(0, money);
            
            stageMoney -= abs(rand())%500 + 500;
            stageMoney = MAX(0, stageMoney);
        }
            break;
            
        case GOLD_ITEM_BOMB_TYPE:
        {
            if (itemCount[type] == 0)
            {
                return;
            }
            itemCount[type]--;
            EventCustom event(PILERS_BOMB);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
            return;
            
        case GOLD_ITEM_KEY_TYPE:
            if (itemCount[type] >= 5) {
                itemCount[type] = 0;
            }
            return;
            
        case GOLD_ITEM_ALCHEMY_TYPE:
            goldScale += 0.5;
            break;
        case GOLD_ITEM_SKULL_TYPE:
            goldScale = 0.5;
            break;
        case GOLD_ITEM_WAND_TYPE:
            goldScale += 1.0;
            break;
            
        case GOLD_ITEM_WINE_TYPE:
            speedScale += 0.2;
            break;
        case GOLD_ITEM_GLOVES_TYPE:
            speedScale += 1.0;
            break;
            
        case GOLD_ITEM_DIAMOND_TYPE:
            diamondScale = 2.0;
            break;
            
        case GOLD_ITEM_CLOCK_TYPE:
        {
            gameTime -= 20;
            gameTime = MAX(0, gameTime);            
        }
            break;
            
        case GOLD_ITEM_GRASS_TYPE:
            luckScale = 2;
            break;
        default:
            break;
    }
    itemCount[type] = 0;
}

void BlackGoldGame::useAllItems()
{
    for (int i = 0; i<GOLD_ITEM_MAX_TYPE; i++) {
        if (i == GOLD_ITEM_BOMB_TYPE || i == GOLD_ITEM_KEY_TYPE) {
            continue;
        }
        if (itemCount[i] == 0) {
            continue;
        }
        useItem((GOLD_ITEM_TYPE)i);
    }
}

void BlackGoldGame::gainObj(int objSerno)
{
    int price = GoldObjs[objSerno].getPrice();
    int dropItemId = GoldObjs[objSerno].getItemId();

    int typeId = GoldObjs[objSerno].getTypeId();
    if (typeId == 8) {
        if (abs(rand())%100 > 50/luckScale) {
            if (price > 0) {
                money += price*goldScale;
                stageMoney += price*goldScale;
                return;
            }
        }
        
        addItem((GOLD_ITEM_TYPE)(abs(rand())%GOLD_ITEM_MAX_TYPE), true);
    }else{
        if (price > 0) {
            if (GoldObjs[objSerno].getTypeId() == 6) {
                price *= diamondScale;
            }
            money += price*goldScale;
            stageMoney += price*goldScale;
        }else if(dropItemId == 0) {
            
        }else if(dropItemId != -1 && dropItemId < GOLD_ITEM_MAX_TYPE){
            addItem((GOLD_ITEM_TYPE)dropItemId, true);
        }        
    }
    

    
    
}

void BlackGoldGame::updateGameTime(float dt)
{
    gameTime -= dt;
    gameTime = MAX(0, gameTime);
}

int BlackGoldGame::getBombCount()
{
    return itemCount[GOLD_ITEM_BOMB_TYPE];
}

int BlackGoldGame::getKeyCount()
{
    return itemCount[GOLD_ITEM_KEY_TYPE];
}

void BlackGoldGame::clearKey()
{
    useItem(GOLD_ITEM_KEY_TYPE);
}

void BlackGoldGame::selDiff2Challenge()
{
    money -= challengeFee;    
    challengeCount++;
    tempChallengeCount = challengeCount;
}

bool BlackGoldGame::openNextStage()
{
    if (diff1Stage >= MAX_GOLD_STAGE) {
        return false;
    }
    
    if (diff1Stage == 10 && stageBoxState != 2) {
        stageBoxState = 1;
        return false;
    }
    
    diff1Stage++;
    return true;
}

int BlackGoldGame::canChallenge()
{
    int fee = GOLD_CHALLENGE_FEE1;
    switch (challengeCount) {
        case 1:
            fee = GOLD_CHALLENGE_FEE2;
            break;
        case 2:
            fee = GOLD_CHALLENGE_FEE3;
            break;
        default:
            break;
    }
    if (money < fee) {
        return  1;
    }
    
    if (challengeCount >= challengeLimit) {
        return 2;
    }
    
    challengeFee = fee;
    return 0;
}

int BlackGoldGame::resChallengeFee()
{
    int fee = GOLD_CHALLENGE_FEE1;
    switch (challengeCount) {
        case 1:
            fee = GOLD_CHALLENGE_FEE2;
            break;
        case 2:
            fee = GOLD_CHALLENGE_FEE3;
            break;
        default:
            break;
    }
    return fee;
}

int BlackGoldGame::getDiffStage()
{
    int lastStage = 0;
//    switch(difficulty)
//    {
//        case 0:
            lastStage = diff1Stage;
//            break;
//        case 1:
//            lastStage = diff2Stage;
//            break;
//        case 2:
//            lastStage = diff3Stage;
//            break;
//    }
    
    return lastStage;
}

void BlackGoldGame::addMoney(int val)
{
    money += val;
}

bool BlackGoldGame::canEnterStage(int stage)
{
    if (!sucNextStage) {
        return false;
    }
    if (stageBoxState == 1) {
        return false;
    }
    
    if (stage != getDiffStage() ) {
        return false;
    }
    
    return true;
}
