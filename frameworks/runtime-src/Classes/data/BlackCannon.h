//
//  BlackCannon.h
//  buyu_simple
//
//  Created by ff on 16/1/20.
//
//

#ifndef BlackCannon_h
#define BlackCannon_h

#include "cocos2d.h"
#include "GameEntity.h"

typedef struct __CREATE_CANNON_CMD
{
    bool isHero;
    int chairId;
    int typeId;
    int cannonMul;
    int bulletCount;
    int multiply;
    cocos2d::Vec2 pos;
//    int bulletCount;
}CREATE_CANNON_CMD;

enum CANNON_STATE
{
    CANNON_NORMAL,
    CANNON_READY_RAY,
    CANNON_RAY,
    CANNON_READY_PLANE,
    CANNON_PLANE
};

class BlackCannon : public GameEntity
{
public:
    //创建炮的初始化
    void create(CREATE_CANNON_CMD &cmd);
    
    void readyRay();
    
    void endRay();
    
    void readyPlane();
    void endPlane();
private:
    //位置ID
    CC_SYNTHESIZE(int, chairId, ChairId);
    //炮的类型
    CC_SYNTHESIZE(int, cannonType, CannonType);
    //倍率
    CC_SYNTHESIZE(int, multiple, Multiple);
    //子弹数
    CC_SYNTHESIZE(int, bulletCount, BulletCount);
    //角度
    CC_SYNTHESIZE(float, angle, Angle);
    //是否是玩家的炮
    CC_SYNTHESIZE(bool, isHero, IsHero);
    //倍率档
    CC_SYNTHESIZE(int, cannonMul, CannonMul);
    
    CC_SYNTHESIZE(int, expMulti, ExpMulti);
    
    CC_SYNTHESIZE_READONLY(CANNON_STATE, state, State);
};

#endif /* BlackCannon_h */
