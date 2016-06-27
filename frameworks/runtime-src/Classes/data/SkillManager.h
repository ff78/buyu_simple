//
//  SkillManager.hpp
//  buyu_simple
//
//  Created by ff on 16/3/4.
//
//

#ifndef SkillManager_h
#define SkillManager_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "utils/GameReader.h"


typedef struct __Skill_Data
{
    int skillId;
    float coolDuration;
    int count;
    int price;
    bool work;
    bool running;
}SKILL_DATA;

class SkillManager : public Singleton<SkillManager>
{
public:
    void loadConfig();
    
    void useSkill(int skillId);
    
    void endSkill(int skillId);
    
    static void initIdTable(rapidjson::Document &_doc);
public:
    static std::map<int, int> idTable;
    
    std::map<int, SKILL_DATA> skillMap;
    
};

#endif /* SkillManager_h */
