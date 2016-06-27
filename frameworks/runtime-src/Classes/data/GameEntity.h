//
//  GameEntity.h
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#ifndef __buyu_simple__GameEntity__
#define __buyu_simple__GameEntity__

#include "cocos2d.h"

class GameEntity
{
public:
    GameEntity();
    virtual ~GameEntity();
    //更新实体
    virtual	void OnUpdate(float dt){};
    
    //加速实体
    void    AddSpeed(int rate){m_fSpeed*=rate;}
private:
    //实体标识ID
    CC_SYNTHESIZE(int, m_wID, ID);
    //实体类型标识ID
    CC_SYNTHESIZE(int, m_wTypeID, TypeID);
    //实体创建时间
    CC_SYNTHESIZE(int, m_wCreateTime, CreateTime);
    //实体大小
    CC_SYNTHESIZE(cocos2d::Size, m_Size, Size);
    //实体位置
    CC_SYNTHESIZE(cocos2d::Vec2, m_Position, Position);
    //实体方向
    CC_SYNTHESIZE(float, m_fDirection, Direction);
    //获取实体速度
    CC_SYNTHESIZE(float, m_fSpeed, Speed);
    //暂停实体动画
    CC_SYNTHESIZE(bool, m_bPause, Pause);
    //实体数据
    CC_SYNTHESIZE(int, m_wData, Data);

};

#endif /* defined(__buyu_simple__GameEntity__) */
