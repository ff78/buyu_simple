//
//  CannonManager.cpp
//  buyu_simple
//
//  Created by ff on 16/1/20.
//
//

#include "CannonManager.h"
#include "logic/ClientLogic.h"

using namespace cocostudio;

std::map<int, int> CanonnManager::configIdTable;
int CanonnManager::maxLevel = 1;

CanonnManager::CanonnManager()
{
    clear();
}

CanonnManager::~CanonnManager()
{
    clear();
}

void CanonnManager::clear()
{
    for (int i = 0; i<CANNON_TYPE_MAX_EX; i++) {
        holdCannon[i] = false;
    }
    holdCannon[CANNON_TYPE_XINSHOU] = true;
//    holdCannon[CANNON_TYPE_ZIYUAN] = true;
//    holdCannon[CANNON_TYPE_ANMIE] = true;

    unlockLevel = 1;
    heroCannonType = CANNON_TYPE_XINSHOU;
    cannonMap.clear();
}

void CanonnManager::logic(float dt)
{
    
}

bool CanonnManager::createCannon(CREATE_CANNON_CMD &cmd)
{
    int bulletCount;
    cmd.multiply = levelConfigs[cmd.cannonMul].multiply;
    if (cmd.isHero) {
        heroCannon.create(cmd);
        cannonMap[cmd.chairId] = heroCannon;
        bulletCount = heroCannon.getBulletCount();
    }else{
        BlackCannon cannon;
        cannon.create(cmd);
        cannonMap[cmd.chairId] = cannon;
        bulletCount = cannon.getBulletCount();
    }

    L2E_CREATE_CANNON info;
    info.angle = 0;
    info.isHero = cmd.isHero;
    info.cannonType = cmd.typeId;
    info.multiple = cmd.multiply;
    info.chairId = cmd.chairId;
    info.bulletCount = bulletCount;
    info.eProtocol = l2e_create_cannon;
    ClientLogic::instance()->pass2Engine(&info);
    return true;
}

void CanonnManager::changeCannon(int typeId)
{
    if (typeId != CANNON_TYPE_NENGLIANG) {
        heroCannonType = typeId;
    }
    if (heroCannon.getCannonType() == CANNON_TYPE_NENGLIANG) {
        return;
    }
    heroCannon.setCannonType(typeId);
    
    L2E_CHANGE_CANNON info;
    info.eProtocol = l2e_change_cannon;
    info.cannonType = typeId;
    ClientLogic::instance()->pass2Engine(&info);    
}

void CanonnManager::changeCannonMul(int deltaVal, int min)
{
    int mul = heroCannon.getCannonMul();
    mul += deltaVal;
    if (mul > unlockLevel || mul < min) {
        return;
    }
//    mul += 10;
//    mul %= 10;
    heroCannon.setCannonMul(mul);
    heroMul = mul;
//    heroCannon.setBulletCount(gCannonBullet[mul]);
//    heroCannon.setMultiple(gCannonMultiply[mul]);
    heroCannon.setBulletCount(2);
    heroCannon.setMultiple(levelConfigs[mul].multiply);
    int multi = gExpMulti[5];
    for (int i = 0; i < 5; i++) {
        if (levelConfigs[mul].multiply < gExpMultiThres[i]) {
            multi = gExpMulti[i];
            break;
        }
    }
    heroCannon.setExpMulti(multi);
    
    L2E_CHANGE_CANNON_MULTIPLY info;
    info.eProtocol = l2e_change_cannon_mul;
    info.multiply = heroCannon.getMultiple();
    info.bulletCount = heroCannon.getBulletCount();
    ClientLogic::instance()->pass2Engine(&info);
}

void CanonnManager::change2Energy()
{
    heroCannonType = heroCannon.getTypeID();
    changeCannon(CANNON_TYPE_NENGLIANG);
}

void CanonnManager::endEnergy()
{
    heroCannon.setCannonType(heroCannonType);
    
    L2E_CHANGE_CANNON info;
    info.eProtocol = l2e_change_cannon;
    info.cannonType = heroCannonType;
    ClientLogic::instance()->pass2Engine(&info);
}

void CanonnManager::readyRay()
{
    heroCannon.readyRay();
}

void CanonnManager::endRay()
{
    heroCannon.endRay();
}

void CanonnManager::readyPlane()
{
    heroCannon.readyPlane();
}

void CanonnManager::endPlane()
{
    heroCannon.endPlane();
}

void CanonnManager::loadCannonConfig(int configId)
{
    readFile(configId, CANNON_LEVEL_CONFIG_FILE);
}

void CanonnManager::initIdTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()){
        int size = _doc.Size();
        maxLevel = (int)size;
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            configIdTable.insert(std::make_pair(objId, i));
        }
    }
}

bool CanonnManager::initData(int idx, rapidjson::Document &_doc)
{
    int index = configIdTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    Cannon_Level_Config config;
    config.ConfigId = DICTOOL->getIntValue_json(dic, "ID");
    config.multiply = DICTOOL->getIntValue_json(dic, "Level");
    config.needDiamondCount = DICTOOL->getIntValue_json(dic, "Diamonds");
    config.boundGolds = DICTOOL->getIntValue_json(dic, "Bounds");
    levelConfigs[config.ConfigId] = config;
    return true;
}

void CanonnManager::readFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(CanonnManager::configIdTable.empty())
    {
        CanonnManager::initIdTable(*doc);
    }
    initData(idx, *doc);
}

void CanonnManager::loadConfig(int minLevel)
{
    unlockLevel = MAX(minLevel, unlockLevel);
    for (int i = 1; i<=unlockLevel+5; i++) {
        loadCannonConfig(i);
    }
}

bool CanonnManager::unlockCannonLevel()
{
    if (unlockLevel == maxLevel) {
        return false;
    }
    unlockLevel++;
    loadCannonConfig(unlockLevel+5);
    int mul = heroCannon.getCannonMul();
    changeCannonMul(unlockLevel-mul, 1);
    
   
    return true;
}
