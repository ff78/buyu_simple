//
//  BlackPliers.h
//  buyu_simple
//
//  Created by ff on 15/12/24.
//
//

#ifndef BlackPliers_h
#define BlackPliers_h

#include "cocos2d.h"
#include "utils/GameUtils.h"

USING_NS_CC;

typedef enum PliersStates
{
    IDLE = 0,
    PUSH,
    FIND,
    PULL,
    GET,
    BOMB,
    BACK,
    CANCEL,
    BOMBBACK
}PLIERSTATE;


class BlackPliers : public Singleton<BlackPliers>
{
public:
    BlackPliers();
    ~BlackPliers();
    
    void changeState(PLIERSTATE nextState);
    
    void enterState(PLIERSTATE nextState);
    
    void exitState(PLIERSTATE currState);
    
    //开始扔钩子
    void onStartPush(EventCustom* event);
    //钩子碰到物体
    void onFindObj(EventCustom *event);
    //动画完成
    void onActionOver(EventCustom *event);
    //钩子回到原点
    void onArriveHome(EventCustom *event);
    //道具炸弹爆炸
    void onBomb(EventCustom *event);
    //钩子超出屏幕
    void onOutofScreen(EventCustom *event);
//    void onStateIdle();
//    void onStatePush();
//    void onStateFind();
//    void onStatePull();
//    void onStateGet();
//    void onStateBomb();
//    void onStateBack();
//    void onStateCancel();
private:
    PLIERSTATE lastState;
    
    EventListenerCustom* startPushListener;
    EventListenerCustom* findObjListener;
    EventListenerCustom* actionOverListener;
    EventListenerCustom* arriveHomeListener;
    EventListenerCustom* bombListener;
    EventListenerCustom* outofScreenListener;
    
    CC_SYNTHESIZE_READONLY(PLIERSTATE, state, State);
    CC_SYNTHESIZE_READONLY(float, speedFactor, SpeedFactor);
    CC_SYNTHESIZE_READONLY(int, pullObjId, PullObjId);
    CC_SYNTHESIZE_READONLY(std::string, pullObjRes, PullObjRes);
    CC_SYNTHESIZE_READONLY(std::string, actionName, ActionName);
    CC_SYNTHESIZE_READONLY(int, pullSerNo, PullSerNo);
    CC_SYNTHESIZE_READONLY(int, pullMoveType, PullMoveType);
};

#endif /* BlackPliers_h */
