//
//  SkillManager.cpp
//  buyu_simple
//
//  Created by ff on 16/3/4.
//
//

#include "SkillManager.h"
#include "EventDef.h"
#include "GameDef.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> SkillManager::idTable;

void SkillManager::loadConfig()
{
    auto doc = GameReader::getDocInstance(SKILL_CONFIG_FILE);
    
    if(SkillManager::idTable.empty())
    {
        SkillManager::initIdTable(*doc);
    }
    
    for (int i = 0; i < idTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        if (dic.IsNull()) {
            return;
        }
        
        SKILL_DATA data;
        data.skillId = DICTOOL->getIntValue_json(dic, "ID");
        data.count = DICTOOL->getIntValue_json(dic, "DefaultCount");
        data.price = DICTOOL->getIntValue_json(dic, "Price");
        data.coolDuration = DICTOOL->getIntValue_json(dic, "CD");
        data.work = (data.count>0);
        data.running = false;
        skillMap[data.skillId] = data;
    }

    
}

void SkillManager::initIdTable(rapidjson::Document &_doc)
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

void SkillManager::useSkill(int skillId)
{
    switch (skillId) {
        case 2:
        {
            EventCustom event(FROZEN_SCREEN);
            Value val(10);
            event.setUserData(&val);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
            break;
        case 3:
        {
            EventCustom event(START_RUSH_FIRE);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
            break;
        case 4:
        {
            EventCustom event(READY_RAY_FIRE);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
            break;
        case 5:
        {
            EventCustom event(READY_PLANE_BOMB);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
            break;
        default:
            break;
    }
    skillMap[skillId].count--;
    skillMap[skillId].count = MAX(0, skillMap[skillId].count);
//    skillMap[skillId].work = (skillMap[skillId].count > 0);
    skillMap[skillId].running = true;
}

void SkillManager::endSkill(int skillId)
{
    skillMap[skillId].running = false;
}