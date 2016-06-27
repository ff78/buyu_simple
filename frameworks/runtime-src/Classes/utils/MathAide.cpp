//
//  MathAide.cpp
//  buyu_simple
//
//  Created by ff on 15/12/11.
//
//

#include "MathAide.h"

USING_NS_CC;

void MathAide::BuildLinear(float initX[], float initY[], int initCount, MovePoints& TraceVector, float fDistance)
{
    TraceVector.clear();
    
    if (initCount < 2) return;
    if (fDistance <= 0.0f) return;
    
    
    float disTotal = Vec2(initX[initCount - 1], initY[initCount - 1]).distance(Vec2(initX[0], initY[0]));
    if (disTotal <= 0.0f) return;
    
    float cosValue = abs(initY[initCount - 1] - initY[0]) / disTotal;
    float tAngle = acosf(cosValue);
    
    MOVE_POINT point;
    float tfDis = 0.f;
    MOVE_POINT tPos;
    
    MovePoints::size_type size;
    while (tfDis < disTotal)
    {
        size = TraceVector.size();
        
        if (initX[initCount - 1] < initX[0])
        {
            point.position.x = initX[0] - sinf(tAngle) * (fDistance * size);
        }
        else
        {
            point.position.x = initX[0] + sinf(tAngle) * (fDistance * size);
        }
        
        if (initY[initCount - 1] < initY[0])
        {
            point.position.y = initY[0] - cosf(tAngle) * (fDistance * size);
        }
        else
        {
            point.position.y = initY[0] + cosf(tAngle) * (fDistance * size);
        }
        
        float temp_dis = point.position.distance(tPos.position);
        if (temp_dis != 0.f)
        {
            float tempValue = (point.position.x - tPos.position.x) / temp_dis;
            if ((point.position.y - tPos.position.y) >= 0.f)
                point.direction = acosf(tempValue);
            else
                point.direction = -acosf(tempValue);
        }
        else
        {
            point.direction = 1.f;
        }
        
        tPos.position.x = point.position.x;
        tPos.position.y = point.position.y;
        point.direction = 90 - point.direction*180/M_PI;
        
        TraceVector.push_back(point);
        
        tfDis = point.position.distance(Vec2(initX[0], initY[0]));
    }
}

void MathAide::BuildCircle(float centerX, float centerY, float radius, Vec2* FishPos, int FishCount)
{
    assert(FishCount > 0);
    if (FishCount <= 0) return;
    float cell_radian = 2 * M_PI / FishCount;
    
    for (int i = 0; i < FishCount - 1; ++i)
    {
        FishPos[i].x = centerX + radius * cosf(i * cell_radian);
        FishPos[i].y = centerY + radius * sinf(i * cell_radian);
    }
}

void MathAide::BuildCircle(float centerX, float centerY, float radius, MovePoints &fishPath, int posCount, float beginRadian, float beginAngle)
{
    assert(posCount > 0);
    if (posCount <= 0)
        return;
    
    float addRadian = 2 * M_PI / (float)posCount;
    float addAngle = 360.0f / (float)posCount;
    
    for (int i = 0; i < posCount; ++i)
    {
        MOVE_POINT pos;
        pos.position.x = centerX + radius * cosf((float)i * addRadian + beginRadian);
        pos.position.y = centerY + radius * sinf((float)i * addRadian + beginRadian);
        pos.direction = -i * addAngle- beginAngle;
        
        fishPath.push_back(pos);
    }
}

bool MathAide::pointHitTest(cocos2d::Vec2 posVal, cocos2d::Size collideSize, float angle, float warnRadius, float deathRadius)
{
    float dis = posVal.length();
    if (warnRadius == 0) {
        warnRadius = Vec2(collideSize.width, collideSize.height).length()/2;
    }
    if (deathRadius == 0) {
        deathRadius = MIN(collideSize.width, collideSize.height)/2;
    }
    
    if (dis > warnRadius) {
        return false;
    }
    
    if (dis < deathRadius) {
        return true;
    }
    
    Vec2 corner[4];
    corner[0] = Vec2(collideSize.width/2, collideSize.height/2);
    corner[1] = Vec2(collideSize.width/2, -collideSize.height/2);
    corner[2] = Vec2(-collideSize.width/2, -collideSize.height/2);
    corner[3] = Vec2(-collideSize.width/2, collideSize.height/2);
    
    
    for (int i = 0; i < 4; i++) {
        corner[i].rotate(Vec2::ZERO, CC_DEGREES_TO_RADIANS(angle));
    }
    
    Vec2 line1 = posVal-corner[0];
    Vec2 line2;
    int count1 = 0;
    int count2 = 0;
    for (int i = 0; i < 4; i++) {
        line1 = posVal-corner[i];
        line2 = corner[i]-corner[(i+1)%4];
        float dotVal = line1.dot(line2);
        if (dotVal > 0) {
            count1++;
        }else if (dotVal < 0){
            count2++;
        }
    }
    
    return (0 == count1 || 0 == count2);

}

bool MathAide::circleHitTest(cocos2d::Vec2 centerVal, float r, cocos2d::Size collideSize, float angle, float warnRadius, float deathRadius)
{
    float dis = centerVal.length();
    if (warnRadius == 0) {
        warnRadius = Vec2(collideSize.width, collideSize.height).length()/2+r;
    }
    if (deathRadius == 0) {
        deathRadius = MIN(collideSize.width, collideSize.height)/2+r;
    }
    
    if (dis > warnRadius) {
        return false;
    }
    
    if (dis < deathRadius) {
        return true;
    }
    
    Vec2 corner[4];
    corner[0] = Vec2(collideSize.width/2, collideSize.height/2);
    corner[1] = Vec2(collideSize.width/2, -collideSize.height/2);
    corner[2] = Vec2(-collideSize.width/2, -collideSize.height/2);
    corner[3] = Vec2(-collideSize.width/2, collideSize.height/2);
    
    for (int i = 0; i < 4; i++) {
        corner[i].rotate(Vec2::ZERO, CC_DEGREES_TO_RADIANS(angle));
    }
    
    for (int i = 0; i < 4; i++) {
        if (centerVal.distance(corner[i]) < r) {
            return true;
        }
    }
    
    cocos2d::Size verSize = Size(collideSize.width, collideSize.height+2*r);
    cocos2d::Size horSize = Size(collideSize.width+2*r, collideSize.height);
    
    if (pointHitTest(centerVal, verSize, angle)) {
        return true;
    }
    
    if (pointHitTest(centerVal, horSize, angle)) {
        return true;
    }
    
    return false;
}

bool MathAide::rectHitTest(Vec2 posA, Size sizeA, float angleA, Vec2 posB, Size sizeB, float angleB)
{
    
    float wariningRadiusA = Vec2(sizeA.width, sizeA.height).length()/2;
    float deathRadiusA = MIN(sizeA.width, sizeA.height)/2;
    float wariningRadiusB = Vec2(sizeB.width, sizeB.height).length()/2;
    float deathRadiusB = MIN(sizeB.width, sizeB.height)/2;
    
    float distance = posA.distance(posB);
    if (distance > wariningRadiusA + wariningRadiusB) {
        return false;
    }
    if (distance < deathRadiusA + deathRadiusB) {
        return true;
    }
    
    int offX[4] = {1,1,-1,-1};
    int offY[4] = {1,-1,-1,1};
    Vec2 cornerA[4];
    Vec2 cornerB[4];
    for (int i = 0; i < 4; i++) {
        cornerA[i] = Vec2(offX[i]*sizeA.width/2, offY[i]*sizeA.height/2);
        cornerB[i] = Vec2(offX[i]*sizeB.width/2, offY[i]*sizeB.height/2);
    }
    
    for (int i = 0; i < 4; i++) {
        cornerA[i].rotate(Vec2::ZERO, CC_DEGREES_TO_RADIANS(angleA));
        cornerB[i].rotate(Vec2::ZERO, CC_DEGREES_TO_RADIANS(angleB));
    }
//    auto bPos = posB - posA;
    for (int i = 0; i < 4; i++) {
        cornerA[i] += posA;
        cornerB[i] += posB;
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if(Vec2::isSegmentIntersect(cornerA[i%4], cornerA[(i+1)%4], cornerB[j%4], cornerB[(j+1)%4]))
            {
                return true;
            }
        }
    }
    
    
    return false;
}

