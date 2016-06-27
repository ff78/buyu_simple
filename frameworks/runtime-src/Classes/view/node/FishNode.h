//
//  FishNode.h
//  buyu_simple
//
//  Created by ff on 16/1/12.
//
//

#ifndef FishNode_h
#define FishNode_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;
using namespace timeline;

class FishNode : public Node
{
public:
    bool init();
    CREATE_FUNC(FishNode);
    void initWithRes();
    
    void update(float dt);
    
    bool pointHitTest(Vec2 bulletPos);
    bool circleHitTest(Vec2 bulletPos, float r);
    bool rectHitTest(Vec2 posA, Size sizeA, float angleA, Vec2 posB, Size sizeB, float angleB);
    bool rectRayHitTest(Vec2 bulletPos, float angle, Size bulletSize);
    
    void back2Normal();
    void dead();
    
    void setLock(bool lock);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
private:
    Node* animationNode;
    Node* backNode;
    ActionTimeline *actTimeLine;
    
    Node* bombNode;
    ActionTimeline *bombTimeline;
    
    Sprite* sdSprite;
    
    int coinType;
    int fishEffectType;
    CC_SYNTHESIZE(int, fishId, FishId);
    CC_SYNTHESIZE(std::string, resName, ResName);
    CC_SYNTHESIZE_READONLY(bool, alive, Alive);
    CC_SYNTHESIZE_READONLY(std::string, soundName, SoundName);
    
//    Vec2 vec1;
//    Vec2 vec2;
//    Vec2 vec3;
//    Vec2 vec4;
    
    Size collideSize;
    float warnRadius;
    float deathRadius;
    bool dying;
    
    CC_SYNTHESIZE(bool, click, Click);
    CC_SYNTHESIZE_READONLY(bool, lock, Lock);
    CC_SYNTHESIZE_READONLY(bool, light, Light);
};

#endif /* FishNode_h */
