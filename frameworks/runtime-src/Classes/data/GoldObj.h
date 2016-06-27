//
//  GoldObj.h
//  buyu_simple
//
//  Created by ff on 15/12/21.
//
//

#ifndef GoldObj_h
#define GoldObj_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class GoldObj
{
public:
    //读取物品的json
    void readFile(int idx, std::string filename);
    
    bool initData(int idx, rapidjson::Document& _doc);
    
    static void initIdTable(rapidjson::Document &_doc);
private:
    void DealWithRectString(std::string strTemp, cocos2d::Rect &rect);
    
private:
    CC_SYNTHESIZE(cocos2d::Vec2, pos, Pos);
    CC_SYNTHESIZE_READONLY(int, moveType, MoveType);
    CC_SYNTHESIZE_READONLY(int, typeId, TypeId);
    CC_SYNTHESIZE_READONLY(std::string, res, ResourceName);
    CC_SYNTHESIZE_READONLY(int, price, Price);
    CC_SYNTHESIZE_READONLY(int, itemId, ItemId);
    CC_SYNTHESIZE_READONLY(float, pullSpeed, PullSpeed);
    CC_SYNTHESIZE_READONLY(std::string, pullRes, PullRes);
    CC_SYNTHESIZE_READONLY(cocos2d::Rect, collideRect, CollideRect);
    
    CC_SYNTHESIZE_READONLY(cocos2d::Size, gridSize, GridSize);
    
    CC_SYNTHESIZE(bool, arrangeInside, ArrangeInside);
    CC_SYNTHESIZE(bool, alive, Alive);
    CC_SYNTHESIZE(int, serno, SerNo);
    
public:
    static std::map<int, int> idTable;
};

#endif /* GoldObj_h */
