//
//  GameEntity.cpp
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#include "GameEntity.h"

USING_NS_CC;
GameEntity::GameEntity() :
m_wID(0),
m_wTypeID(0),
m_wCreateTime(0),
m_fDirection(0),
m_fSpeed(0),
m_wData(0),
m_bPause(false)
{
    m_Size = Size::ZERO;
    m_Position = Vec2::ZERO;
}

GameEntity::~GameEntity()
{
    
}