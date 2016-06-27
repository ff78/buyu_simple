//
//  BulletManager.h
//  buyu_simple
//
//  Created by ff on 16/1/20.
//
//

#ifndef BulletManager_h
#define BulletManager_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "BlackBullet.h"

enum BULLET_MANAGER_STATE
{
    BULLET_MANAGER_NORMAL,
    BULLET_MANAGER_READY_RAY,
    BULLET_MANAGER_RAY,
    BULLET_MANAGER_READY_PLANE,
    BULLET_MANAGER_PLANE
};
class BulletManager : public Singleton<BulletManager>
{
public:
    BulletManager();
    ~BulletManager();
    
    //清除子弹
    void clear();
    
    void logic(float dt);
    
    //创建子弹
    bool createBullet(CREATE_BULLET_CMD &cmd);
    
    //处理击中鱼的子弹
    void bulletHit(cocos2d::EventCustom *event);
    
    void readyRay();
    void endRay();
    
    void readyPlane();
    void endPlane();
public:
    std::map<int, BlackBullet> bulletMap;
    cocos2d::EventListenerCustom* bulletHitListener;
    cocos2d::EventListenerCustom* startRushListener;
    cocos2d::EventListenerCustom* endRushListener;
private:
    int incrementId;
    BULLET_MANAGER_STATE state;
    
    CC_SYNTHESIZE(bool, readyFire, ReadyFire);
    
    CC_SYNTHESIZE(bool, rushFire, RushFire);
    
    CC_SYNTHESIZE_READONLY(int, expMulti, ExpMulti);
    
};

#endif /* BulletManager_h */
