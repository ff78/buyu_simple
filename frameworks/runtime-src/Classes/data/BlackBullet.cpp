//
//  BlackBullet.cpp
//  buyu_simple
//
//  Created by ff on 16/1/19.
//
//

#include "BlackBullet.h"

BlackBullet::BlackBullet() :
chairId(0),
multiply(1),
radius(12),
bulletCount(2),
bulletType(1),
expMulti(1)
{
    
}

BlackBullet::~BlackBullet()
{
    
}

void BlackBullet::create(CREATE_BULLET_CMD &cmd)
{
    cannonType = cmd.cannonType;
    m_Position = cmd.pos;
    angle = cmd.angle;
    bulletCount = cmd.bulletCount;
    multiply = cmd.multiply;
    speed = cmd.speed;
    chairId = cmd.chariId;
    state = BULLET_FLYING;
    traceFishId = cmd.traceFishId;
    expMulti = cmd.expMulti;
}
