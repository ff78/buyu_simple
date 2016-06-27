//
//  BlackPilers.cpp
//  buyu_simple
//
//  Created by ff on 15/12/24.
//
//

#include "BlackPliers.h"
#include "EventDef.h"
#include "GoldObj.h"

BlackPliers::BlackPliers()
{
    startPushListener = EventListenerCustom::create(CLICK_SCREEN, CC_CALLBACK_1(BlackPliers::onStartPush, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPushListener, -1);
    findObjListener = EventListenerCustom::create(PLIERS_FIND_THING, CC_CALLBACK_1(BlackPliers::onFindObj, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(findObjListener, -1);
    actionOverListener = EventListenerCustom::create(PLIERS_ACTION_OVER, CC_CALLBACK_1(BlackPliers::onActionOver, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(actionOverListener, -1);
    
    arriveHomeListener = EventListenerCustom::create(ARRIVE_HOME, CC_CALLBACK_1(BlackPliers::onArriveHome, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(arriveHomeListener, -1);
    bombListener = EventListenerCustom::create(PILERS_BOMB, CC_CALLBACK_1(BlackPliers::onBomb, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(bombListener, -1);
    outofScreenListener = EventListenerCustom::create(OUT_OF_SCREEN, CC_CALLBACK_1(BlackPliers::onOutofScreen, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(outofScreenListener, -1);
}

BlackPliers::~BlackPliers()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPushListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(findObjListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(actionOverListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(arriveHomeListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(bombListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(outofScreenListener);
}

void BlackPliers::onStartPush(cocos2d::EventCustom *event)
{
    if (state == IDLE) {
        changeState(PUSH);
    }
}

void BlackPliers::onFindObj(cocos2d::EventCustom *event)
{
    if (state == PUSH) {
        GoldObj obj = *static_cast<GoldObj *>(event->getUserData());
        speedFactor = obj.getPullSpeed();
        pullObjId = obj.getTypeId();
        pullObjRes = obj.getPullRes();
        pullSerNo = obj.getSerNo();
        pullMoveType = obj.getMoveType();
        changeState(FIND);
    }
}

void BlackPliers::onActionOver(cocos2d::EventCustom *event)
{
    switch (state) {
        case FIND:
            changeState(PULL);
            break;
            
        case GET:
            changeState(IDLE);
            break;
            
        case PUSH:
            changeState(BACK);
            break;
            
        case BOMB:
            changeState(BOMBBACK);
            break;
            
            
        default:
            break;
    }
}

void BlackPliers::onArriveHome(cocos2d::EventCustom *event)
{
    switch (state) {
        case PULL:
            changeState(GET);
            break;
            
        case CANCEL:
        case BOMBBACK:
            changeState(IDLE);
            break;
            
        case BACK:
            changeState(IDLE);
            break;
            
            
        default:
            break;
    }
}

void BlackPliers::onBomb(cocos2d::EventCustom *event)
{
    if (state == PULL) {
        changeState(BOMB);
    }
}

void BlackPliers::onOutofScreen(cocos2d::EventCustom *event)
{
    if (state == PUSH) {
        changeState(CANCEL);
    }
}

void BlackPliers::changeState(PLIERSTATE nextState)
{
    if(state == nextState)
        return;
    exitState(state);
    lastState	= state;
    state		= nextState;
    enterState(nextState);
    
    EventCustom event(CHANGE_PLIERS_ACTION);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
}

void BlackPliers::enterState(PLIERSTATE nextState)
{
    switch (nextState) {
        case IDLE:
            actionName = "idle";
            break;
            
        case PUSH:
            actionName = "push";
            break;
            
        case PULL:
            actionName = "pull";
            break;
            
        case FIND:
            actionName = "find";
            break;
            
        case GET:
            actionName = "get";

            break;
            
        case BOMB:
            actionName = "bomb";
            break;
            
        case BACK:
            actionName = "back";
            break;
            
        case BOMBBACK:
            actionName = "bombBack";
            break;
            
        case CANCEL:
            actionName = "cancel";
            break;
        default:
            break;
    }
}

void BlackPliers::exitState(PLIERSTATE currState)
{
    switch (currState) {
        case IDLE:
            
            break;
            
        default:
            break;
    }
}
