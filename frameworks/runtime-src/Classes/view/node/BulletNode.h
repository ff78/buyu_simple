//
//  BulletNode.h
//  buyu_simple
//
//  Created by ff on 16/1/20.
//
//

#ifndef BulletNode_h
#define BulletNode_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "../Share/Header/EventDef.h"
#include "../Share/Header/ELProtocol.h"


class BulletNode : public cocos2d::Node {
public:
    bool init();
    
    CREATE_FUNC(BulletNode);
    
    void initWithRes(L2E_CANNON_FIRE &info);
    
    void update(float dt, int traceId);
    
    void traceFish(cocos2d::Vec2 targetPos);
    
    void syncCurrPos();
    
    void endRay();
    void endPlane();
    void onRayFrameEvent(cocostudio::timeline::Frame *frame);
    
private:
    cocos2d::Sprite* bullet;
    
    cocos2d::Node* bulletAnimation;
    cocostudio::timeline::ActionTimeline* bulletAction;
    
//    cocostudio::
    CC_SYNTHESIZE(int, bulletId, BulletId);
    CC_SYNTHESIZE_READONLY(float, angle, Angle);
    CC_SYNTHESIZE_READONLY(bool, alive, Alive);
    CC_SYNTHESIZE_READONLY(std::string, resName, ResName);
    CC_SYNTHESIZE_READONLY(float, radius, Radius);
    CC_SYNTHESIZE_READONLY(int, traceFishId, TraceFishId);
    CC_SYNTHESIZE_READONLY(float, startDis, StartDistance);
    CC_SYNTHESIZE_READONLY(bool, halfDis, HalfDis);
    CC_SYNTHESIZE_READONLY(float, rayLength, RayLength);
    CC_SYNTHESIZE_READONLY(int, bulletType, BulletType);
};

#endif /* BulletNode_h */
