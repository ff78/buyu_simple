//
//  BlackFish.h
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#ifndef __buyu_simple__BlackFish__
#define __buyu_simple__BlackFish__

#include "GameEntity.h"
#include "GameDef.h"
#include "PathManager.h"

enum FISH_STATUS
{
    FISH_INVALID = 0,
    FISH_ALIVE,
    FISH_DIED,
    FISH_HURT,
    FISH_OUT,
    FISH_CLEAR,
    FISH_LIGHTNING
};

enum FishGroupType
{
    NONE_GROUP = 0,
    SNAKE_HEAD = 1,
    SNAKE_BODY = 2,
    SNAKE_TAIL = 3,
    THREE_GROUP = 4,
    FOUR_GROUP = 5,
    ALL_NET_GROUP = 6,
    FISH_TYPE_BOMB_GROUP = 7
};

struct CMD_S_CREATE_FISH
{
    unsigned int	nID;
    int				nType;
    unsigned int    nPathID;
    FISH_PATH_TYPE	nPathType;
    float			OffestX;
    float			OffestY;
    float			fDelay;
    unsigned int	nElaspe;
    int			    nData;
    float			fSpeed;
    int             Score;
    float           interval;
    int             randThres;
    cocos2d::Rect   collideRect;
    FishGroupType   groupType;
    int             groupId;
    FishType        fishType;
    std::string     resource;
    std::string     sound;
    
    int             dropThres;
    int             dropItemType[3];
    int             dropItemId[3];
    int             dropItemThres[3];
    int             dropItemCount[3];
};

typedef struct __Fish_Drop_Item
{
    int             dropItemType;
    int             dropItemId;
    int             dropItemCount;
    int             receiveCount;
}Fish_Drop_Item;

class BlackFish : public GameEntity
{
public:
    BlackFish();
    virtual ~BlackFish();
    
    void create(CMD_S_CREATE_FISH &cmd);
    
    virtual void OnUpdate(float dt) override;
    
    //检查实体是否超出屏幕范围
    bool InsideScreen();
    
    //计算鱼行走路径
    void CaclutePath();
    
    bool EndPath();
    
    Fish_Drop_Item caculateDrop();
//    //检查子弹是否击中物体
//    bool BulletHitTest(CBullet* bullet);
//    
//    //检查鱼网是否击中物体
//    bool NetHitTest(CBullet* bullet);
    
private:
    std::vector<MOVE_POINT> m_path;
    float m_fDuration;        //持续时间
    float m_Elaspe;
    
    int             dropThres;
    int             dropItemType[3];
    int             dropItemId[3];
    int             dropItemThres[3];
    int             dropItemCount[3];
    
    CC_SYNTHESIZE(FishType, fishType, FishType);
    CC_SYNTHESIZE(bool, m_bOutScreen, OutScreen);		//是否超出屏幕

    
    CC_SYNTHESIZE(int, m_wPathID, PathID);
    CC_SYNTHESIZE(cocos2d::Vec2, m_Offest, Offest);
    CC_SYNTHESIZE(float, m_fDelay, Delay);
    
    CC_SYNTHESIZE(FISH_PATH_TYPE, m_eFishType, PathType);
    CC_SYNTHESIZE(int, m_Prob, Prob);
    CC_SYNTHESIZE(FISH_STATUS,m_Status, Status);			//鱼动画状态
    CC_SYNTHESIZE(int, m_GroupType, GroupType);
    CC_SYNTHESIZE(int, m_GroupFirstId, GroupFirstId);
    
    CC_SYNTHESIZE(int, baseScore, BaseScore);
    CC_SYNTHESIZE(int, catchScore, CatchScore);
    CC_SYNTHESIZE(int, catchMultiply, CatchMultiply);
    CC_SYNTHESIZE(int, catchExpMultiply, CatchExpMultiply);
    CC_SYNTHESIZE(int, catchChairId, CatchChairId);
    CC_SYNTHESIZE(int, catchRateThres, CatchRate)
    CC_SYNTHESIZE_READONLY(cocos2d::Rect, collideRect, CollideRect);
    CC_SYNTHESIZE_READONLY(std::string, resource, Resource);
    CC_SYNTHESIZE_READONLY(std::string, sound, Sound);
};

#endif /* defined(__buyu_simple__BlackFish__) */
