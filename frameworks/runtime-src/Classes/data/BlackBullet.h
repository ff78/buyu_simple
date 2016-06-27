//
//  BlackBullet.h
//  buyu_simple
//
//  Created by ff on 16/1/19.
//
//

#ifndef BlackBullet_h
#define BlackBullet_h

#include "GameEntity.h"
#include "cocos2d.h"

enum BulletStatus
{
    BULLET_INVALID = 0,
    BULLET_FLYING,
    BULLET_CASTING_NET,
    BULLET_DIE
};

typedef struct __CREATE_BULLET_CMD
{
    int chariId;
    int multiply;
    int bulletCount;
    cocos2d::Vec2 pos;
    float angle;
    float speed;
    bool isHero;
    int cannonType;
    int traceFishId;
    int expMulti;
}CREATE_BULLET_CMD;

class BlackBullet : public GameEntity {
public:
    BlackBullet();
    ~BlackBullet();
    
    //创建子弹的初始化
    void create(CREATE_BULLET_CMD &cmd);

private:
    //发射炮的位置ID
    CC_SYNTHESIZE(int, chairId, ChairId);
    //击中会带来的倍率
    CC_SYNTHESIZE(int, multiply, Multiply);
    //撒出的网的半径
    CC_SYNTHESIZE_READONLY(float, radius, Radius);
    
    //子弹状态
    CC_SYNTHESIZE(BulletStatus, state, State);
    //子弹数量
    CC_SYNTHESIZE(int, bulletCount, BulletCount);
    //是否为玩家的子弹
    CC_SYNTHESIZE(bool, isHero, IsHero);
    //炮的类型，决定子弹的外观
    CC_SYNTHESIZE_READONLY(int, cannonType, CannonType);
    //角度
    CC_SYNTHESIZE_READONLY(float, angle, Angle);
    //速度
    CC_SYNTHESIZE_READONLY(float, speed, Speed);
    
    CC_SYNTHESIZE_READONLY(int, traceFishId, TraceFishId);
    
    CC_SYNTHESIZE_READONLY(int, expMulti, ExpMulti);
    
    CC_SYNTHESIZE(int, bulletType, BulletType);
};

#endif /* BlackBullet_h */
