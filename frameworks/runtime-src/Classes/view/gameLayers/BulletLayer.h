//
//  BulletLayer.h
//  buyu_simple
//
//  Created by ff on 16/1/18.
//
//

#ifndef BulletLayer_h
#define BulletLayer_h

#include "cocos2d.h"
#include "../Share/Header/EventDef.h"
#include "../Share/Header/ELProtocol.h"

class BulletLayer : public cocos2d::Layer
{
public:
    BulletLayer();
    ~BulletLayer();
    
    bool init();
    
    CREATE_FUNC(BulletLayer);
    
    void createBullet(cocos2d::EventCustom* event);
    
    void update(float dt, int traceId);
    
    void updateTrace(cocos2d::Vec2 pos);
    
    void pauseAll();
    
    void resumeAll();
private:
    cocos2d::EventListenerCustom* createListener;
};

#endif /* BulletLayer_h */
