//
//  BlackCannon.cpp
//  buyu_simple
//
//  Created by ff on 16/1/20.
//
//

#include "BlackCannon.h"
#include "GameDef.h"


void BlackCannon::create(CREATE_CANNON_CMD &cmd)
{
    cannonMul = cmd.cannonMul;
    chairId = cmd.chairId;
    cannonType = cmd.typeId;
    multiple = cmd.multiply;
    isHero = cmd.isHero;
    m_Position = cmd.pos;
    angle = 0;
    bulletCount = cmd.bulletCount;
    state = CANNON_NORMAL;
    int multi = gExpMulti[5];
    for (int i = 0; i < 5; i++) {
        if (multiple < gExpMultiThres[i]) {
            multi = gExpMulti[i];
            break;
        }
    }
    expMulti = multi;
    
//    bulletCount = (multiple-20)/30;
//    bulletCount = MAX(0, bulletCount);
//    bulletCount += 2;
    
}

void BlackCannon::readyRay()
{
    state = CANNON_READY_RAY;
}

void BlackCannon::endRay()
{
    state = CANNON_NORMAL;
}

void BlackCannon::readyPlane()
{
    state = CANNON_READY_PLANE;
}

void BlackCannon::endPlane()
{
    state = CANNON_NORMAL;
}