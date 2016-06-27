//
//  GoldObj.cpp
//  buyu_simple
//
//  Created by ff on 15/12/21.
//
//

#include "GoldObj.h"
#include "utils/GameReader.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> GoldObj::idTable;

void GoldObj::initIdTable(rapidjson::Document &_doc)
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

bool GoldObj::initData(int idx, rapidjson::Document& _doc)
{
    int index = idTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    moveType = DICTOOL->getIntValue_json(dic, "moveType");
    typeId = DICTOOL->getIntValue_json(dic, "ID");
    pullRes = res = DICTOOL->getStringValue_json(dic, "res");
    price = DICTOOL->getIntValue_json(dic, "price");

    if (price == -1) {
        int min = DICTOOL->getIntValue_json(dic, "minPrice");
        int max = DICTOOL->getIntValue_json(dic, "maxPrice");
        price = abs(rand()%(max-min))+min;
    }
    itemId = DICTOOL->getIntValue_json(dic, "itemId");
    pullSpeed = DICTOOL->getFloatValue_json(dic, "pullSpeed");
    const char* tempStr = DICTOOL->getStringValue_json(dic, "collideRect");
    DealWithRectString(tempStr, collideRect);
    
    gridSize = Vec2(0, 0);
    std::string clsSkillStr = DICTOOL->getStringValue_json(dic, "gridSize");
    if (clsSkillStr != "null") {
        std::vector<std::string> splitVec = GameUtils::splitStr(clsSkillStr, ";");
        if (!splitVec.empty()) {
            int width = atoi(splitVec[0].c_str());
            int height = atoi(splitVec[1].c_str());
            gridSize = Size(width, height);
            
        }
    }
    alive = true;
    

    return true;
}

void GoldObj::readFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(GoldObj::idTable.empty())
    {
        GoldObj::initIdTable(*doc);
    }
    initData(idx, *doc);
}

void GoldObj::DealWithRectString(std::string strTemp, cocos2d::Rect &rect)
{
    /************************************************************************/
    // 处理范围字符串
    // 1. 判断字符串的格式是否正确
    int nTemp = static_cast<int>(count(strTemp.begin(), strTemp.end(), ','));
    if (nTemp != 3)
        return;
    
    // 2. 替换字符串中的"(" , ")" , ","为空的字符
    replace(strTemp.begin(), strTemp.end(), '(', ' ');
    replace(strTemp.begin(), strTemp.end(), ')', ' ');
    replace(strTemp.begin(), strTemp.end(), ',', ' ');
    
    // 3. 解析该字符串并输出到Rect中去
    float fTemp[4] = { 0 };
    std::istringstream stream(strTemp);
    for (int nIndex = 0; nIndex < 4; ++nIndex)
    {
        stream >> fTemp[nIndex];
    }
    
    rect = Rect(fTemp[0], fTemp[1], fTemp[2], fTemp[3]);
}