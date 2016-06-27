//
//  BlackFish.cpp
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#include "BlackFish.h"
#include "PathManager.h"
#include "utils/GameUtils.h"

USING_NS_CC;

BlackFish::BlackFish() :
m_wPathID(0)
,m_Offest(0,0)
,m_fDelay(0.0f)
,m_Elaspe(0.0f)
,m_bOutScreen(false)
,m_Status(FISH_ALIVE)
,m_Prob(1000)
{
    m_eFishType = SMALL_FISH_PATH;
}

BlackFish::~BlackFish()
{
    
}

void BlackFish::OnUpdate(float dt)
{
    if(m_bPause) return;
    
    if (m_Status == FISH_OUT && m_bOutScreen) {
        return;
    }
    if (m_Status == FISH_DIED) {
        return;
    }
    
    m_Elaspe += dt*m_fSpeed;
    
    if(m_fDelay > 0.0f)
    {
        if(m_Elaspe < m_fDelay)
        {
            return;
        }
        else
        {
            m_Elaspe = 0;
            m_fDelay = 0.0f;
        }
    }
    
    const MovePoints& paths = PathManager::instance()->getPaths(m_eFishType, m_wPathID);
    if(paths.empty())
        return;
    MOVE_POINT mp;
    float time = MIN(1.0f, (m_Elaspe / m_fDuration));
    float fDiff;
    float fIndex = time * paths.size();
    int index = fIndex;
    fDiff = fIndex - index;
    
    if (index >= paths.size())
    {
        index = paths.size() - 1;
    }
    else if(index < 0 || fDiff < 0)
    {
        index = 0;
        fDiff = 0;
    }
    
    if (index<paths.size()-1)
    {
        const MOVE_POINT &move_point1 = paths.at(index);
        const MOVE_POINT &move_point2 = paths.at(index+1);
        
        mp.position = move_point1.position*(1.0-fDiff)+ move_point2.position*fDiff;
        mp.direction = move_point1.direction*(1.0-fDiff)+ move_point2.direction*fDiff;
        
        if (std::abs(move_point1.direction-move_point2.direction) > 180.0)
        {
            mp.direction = move_point1.direction;
        }
    }
    else
    {
        mp = paths.at(index);
        m_bOutScreen = true;
    }
    
    if(m_Offest.x != 0.0f || m_Offest.y != 0.0f){
        setPosition(mp.position+m_Offest);
    }
    else
    {
        setPosition(mp.position);
    }
    setDirection(mp.direction-90);
}

bool BlackFish::InsideScreen()
{
    float scaleX =  1;
    float scaleY =  1;
    
    return m_Position.x > 20.f && m_Position.x < GameUtils::winSize.width*scaleX-20.0f && m_Position.y > 20.f && m_Position.y < GameUtils::winSize.height*scaleY-20.0f;
//    return true;
}

void BlackFish::CaclutePath()
{
    const MovePoints& paths = PathManager::instance()->getPaths(m_eFishType, m_wPathID);
    
//    m_fDuration = 0.065*paths.size();//???
    m_fDuration = 0.1*paths.size();//???
}

bool BlackFish::EndPath()
{
    return m_bOutScreen;
}

void BlackFish::create(CMD_S_CREATE_FISH &cmd)
{
    m_wTypeID = cmd.nType;
    m_eFishType = cmd.nPathType;
    fishType = cmd.fishType;
    m_wPathID = cmd.nPathID;
    m_fDelay = cmd.fDelay;
    m_Offest = Vec2(cmd.OffestX, cmd.OffestY);
    setPosition(Vec2(-2000, -2000));
    m_wData = cmd.nData;
    m_fSpeed = cmd.fSpeed;
    m_GroupType = cmd.groupType;
    m_GroupFirstId = cmd.groupId;
    baseScore = cmd.Score;
    catchScore = 0;
    catchChairId = -1;
    catchMultiply = 1;
    catchRateThres = cmd.randThres;
    collideRect = cmd.collideRect;
    resource = cmd.resource;
    sound = cmd.sound;
    
    dropThres = cmd.dropThres;
    memcpy(dropItemType, cmd.dropItemType, sizeof(int)*3);
    memcpy(dropItemId, cmd.dropItemId, sizeof(int)*3);
    memcpy(dropItemThres, cmd.dropItemThres, sizeof(int)*3);
    memcpy(dropItemCount, cmd.dropItemCount, sizeof(int)*3);
    
    CaclutePath();
}

Fish_Drop_Item BlackFish::caculateDrop()
{
    Fish_Drop_Item result;
    result.dropItemCount = 0;
    result.dropItemId = 0;
    result.dropItemType = 0;
    result.receiveCount = 0;
    if (abs(rand())%10000 < dropThres) {
        int val = abs(rand())%10000;
        for (int i = 0; i < 3; i++) {
            if (val < dropItemThres[i]) {
                result.dropItemType = dropItemType[i];
                result.dropItemId = dropItemId[i];
                result.dropItemCount = dropItemCount[i];
                result.receiveCount = result.dropItemCount;
            }
        }
    }
    
    return result;
}