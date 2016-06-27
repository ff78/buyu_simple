//
//  CannonNode.cpp
//  buyu_simple
//
//  Created by ff on 16/1/18.
//
//

#include "CannonNode.h"

#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"
#include "GameDef.h"
#include "sound/SoundManager.h"
#include "GoldFallen.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace timeline;

CannonNode::CannonNode()
{
    
}

CannonNode::~CannonNode()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(clearLockListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startRushListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(endRushListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startAutoListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(endAutoListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateRelifeListener);
}
bool CannonNode::init()
{
    if (!Node::init()) {
        return false;
    }
    

    cannonType = CANNON_TYPE_XINSHOU;
    carriageType = 0;
    multiple = 10;
    bulletCount = 2;
    angle = 30;
    
//    Size winSize = Director::getInstance()->getWinSize();
//    std::string carrStr("game_scene/cannon/gun/gundown_gunup");
////    carrStr += std::to_string(carriageType);
//    carrStr += Convert2String(carriageType);
//    carrStr += ".png";
//    carriage = Sprite::createWithSpriteFrameName(carrStr);
//    carriage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    addChild(carriage);
//    
//    std::string resStr("game_scene/cannon/gun/Cannon_1.csb");
//    animationNode = CSLoader::createNode(resStr);
//    animationNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    animationNode->setPosition(Vec2::ZERO);
//    animationNode->setRotation(angle);
//    
//    animateAction = CSLoader::createTimeline(resStr);
//    std::string animStr("stop_");
////    animStr += std::to_string(bulletCount);
//    animStr += Conver2String(bulletCount);
//    animationNode->runAction(animateAction);
//    animateAction->play(animStr, false);
//    
//    addChild(animationNode);
//    
//    std::string foudStr("game_scene/cannon/gun/gundown_gundown");
////    foudStr += std::to_string(carriageType);
//    foudStr += Convert2String(carriageType);
//    foudStr += ".png";
//    carriage = Sprite::createWithSpriteFrameName(foudStr);
//    carriage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    carriage->setPosition(Vec2(0, -18));
//    addChild(carriage);
    
    return true;
}

void CannonNode::setupCannon(cocos2d::EventCustom *event)
{
    removeAllChildren();
    
    L2E_CREATE_CANNON info = *static_cast<L2E_CREATE_CANNON*>(event->getUserData());
    cannonType = info.cannonType;
    multiple = info.multiple;
    angle = info.angle;
    chairId = info.chairId;
    isHero = info.isHero;
    bulletCount = info.bulletCount;
    
    carriageType = gCannonIndex[cannonType];
    
    std::string carrStr("game_scene/cannon/gun/gundown_gunup");
//    carrStr += std::to_string(carriageType);
    carrStr += Convert2String(carriageType);
    carrStr += ".png";
    carriage = Sprite::create(carrStr);
    carriage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(carriage);
    
    autoCircle = Sprite::create(AUTO_CIRCLE_RES);
    auto circle = RotateBy::create(1.5, -360);
    auto repeate = RepeatForever::create(circle);
    autoCircle->runAction(repeate);
    autoCircle->setVisible(false);
    addChild(autoCircle);
 
    fireCircle = Sprite::create(FIRE_CIRCLE_RES);
    auto circle1 = RotateBy::create(1, -360);
    auto repeate1 = RepeatForever::create(circle1);
    fireCircle->setVisible(false);
    fireCircle->runAction(repeate1);
    addChild(fireCircle);


    std::string resStr("game_scene/cannon/gun/Cannon_");
//    resStr += std::to_string(info.cannonType+1);
    resStr += Convert2String(info.cannonType+1);
    resStr += ".csb";
    
    Size winSize = Director::getInstance()->getWinSize();
    animationNode = CSLoader::createNode(resStr);
    animationNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    animationNode->setPosition(Vec2::ZERO);
    addChild(animationNode);
    
    animateAction = CSLoader::createTimeline(resStr);
    std::string animStr("stop_");
//    animStr += std::to_string(bulletCount);
    animStr += Convert2String(bulletCount);
    animationNode->runAction(animateAction);
    animateAction->play(animStr, false);
    
    for (int i = 0; i<100; i++) {
        sdPoint[i] = Sprite::create("game_scene/right_menu/rm_sd2.png");
        sdPoint[i]->setPosition(Vec2(0, 16*(i+1)));
        sdPoint[i]->setVisible(false);
        animationNode->addChild(sdPoint[i]);
    }
    
    rayEffectNode = CSLoader::createNode(RAY_CIRCLE_ANIM);
    rayEffectAction = CSLoader::createTimeline(RAY_CIRCLE_ANIM);
    rayEffectNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rayEffectNode->setPosition(Vec2(0,40));
    rayEffectNode->runAction(rayEffectAction);
//    rayEffectAction->play("wait", true);
    rayEffectNode->setVisible(false);
    animationNode->addChild(rayEffectNode);
    
    
    std::string foudStr("game_scene/cannon/gun/gundown_gundown");
//    foudStr += std::to_string(carriageType);
    foudStr += Convert2String(carriageType);
    foudStr += ".png";
    foudation = Sprite::create(foudStr);
    foudation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    foudation->setPosition(Vec2(0, -18));
    addChild(foudation);
//    if (cannonType == CANNON_TYPE_NENGLIANG) {
    
        energyNode = CSLoader::createNode(CANNON_ENERGY_EFFECT);
        energyNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        energyNode->setPosition(Vec2(66, 40));
        energyAction = CSLoader::createTimeline(CANNON_ENERGY_EFFECT);
        energyNode->runAction(energyAction);
        energyAction->play("play", true);
        foudation->addChild(energyNode, 100);
        energyNode->setVisible(false);
//    }
    multiBg= Sprite::create("game_scene/cannon/gun/user_display.png");
    multiBg->setPosition(Vec2(0, -30));
    addChild(multiBg);
    multiLabel = TextAtlas::create("0123456789", "bmfont/game_number_3.png", 21, 26, "0");
//    multiLabel->setString(std::to_string(multiple));
    multiLabel->setString(Convert2String(multiple));
    multiLabel->setPosition(Vec2(0, -30));
    multiLabel->setScale(0.7);
    addChild(multiLabel);
    
    if (isHero) {
        addButton = Button::create("game_scene/cannon/gun/gun_add.png", "", "", TextureResType::LOCAL);
        addButton->setPosition(Vec2(100, 0));
        addButton->addClickEventListener(CC_CALLBACK_1(CannonNode::clickAdd, this));
        addChild(addButton);
        
        minusButton = Button::create("game_scene/cannon/gun/gun_minus.png", "", "", TextureResType::LOCAL);
        minusButton->setPosition(Vec2(-100, 0));
        minusButton->addClickEventListener(CC_CALLBACK_1(CannonNode::clickMinus, this));
        addChild(minusButton);
    }
    
    mulEffectNode = CSLoader::createNode(CANNON_MULTIPLY_EFFECT);
    addChild(mulEffectNode);

    effectAction = CSLoader::createTimeline(CANNON_MULTIPLY_EFFECT);
    mulEffectNode->runAction(effectAction);
    effectAction->play("play", false);
    effectAction->setLastFrameCallFunc(CC_CALLBACK_0(CannonNode::hideEffect, this));
    
    AUDIO_PLAY("gun_up", AUDIO_TYPE::EFFECT_TYPE);
    
    relifeButton = Button::create("game_scene/gamer_ui/mianscence/ms_relief1.png",
                                  "",
                                  "game_scene/gamer_ui/mianscence/ms_relief0.png");
    relifeButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    relifeButton->setPosition(Vec2(0, 100));
    relifeButton->setEnabled(false);
    relifeButton->setTouchEnabled(false);
    relifeButton->setBright(false);
    relifeButton->setVisible(false);
    addChild(relifeButton);
    
    relifeCountLabel = TextAtlas::create("0123456789:", "bmfont/game_number_5.png", 14, 22, "0");
    relifeCountLabel->setPosition(Vec2(230, 22));
    relifeCountLabel->setString("00:00");
    relifeButton->addChild(relifeCountLabel);
    
    boundLabel = TextAtlas::create("0123456789", "bmfont/game_number_8.png", 15, 20, "0");
    boundLabel->setPosition(Vec2(230, 22));
    boundLabel->setString("0");
    boundLabel->setVisible(false);
    relifeButton->addChild(boundLabel);
    
    bankRuptcySpt = Sprite::create("common/tp_bankruptcy.png");
    bankRuptcySpt->setVisible(false);
    addChild(bankRuptcySpt);
    
    clearLockListener = EventListenerCustom::create(CLEAR_LOCK_FISH, std::bind(&CannonNode::setLock, this, false, 0));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(clearLockListener, -1);
    
    startRushListener = EventListenerCustom::create(START_RUSH_FIRE, std::bind(&CannonNode::switchFireCircle, this, true));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startRushListener, -1);
    endRushListener = EventListenerCustom::create(END_RUSH_FIRE, std::bind(&CannonNode::switchFireCircle, this, false));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(endRushListener, -1);
    startAutoListener = EventListenerCustom::create(START_AUTO_CIRCLE, std::bind(&CannonNode::switchAutoCircle, this, true));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startAutoListener, -1);
    endAutoListener = EventListenerCustom::create(STOP_AUTO_CIRCLE, std::bind(&CannonNode::switchAutoCircle, this, false));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(endAutoListener, -1);
    
    updateRelifeListener = EventListenerCustom::create(UI_UPDATE_RELIFE_BUTTON, CC_CALLBACK_1(CannonNode::updateRelife, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateRelifeListener, -1);
    
    setScale(0.8);
}

void CannonNode::hideEffect()
{
    mulEffectNode->setVisible(false);
}

void CannonNode::updateCannon(cocos2d::EventCustom *event)
{
    L2E_UPDATE_CANNON info = *static_cast<L2E_UPDATE_CANNON*>(event->getUserData());
    std::string resStr("game_scene/cannon/gun/Cannon_");
//    resStr += std::to_string(info.cannonType+1);
    resStr += Convert2String(info.cannonType+1);
    resStr += ".csb";
    
    if(info.cannonType != cannonType)
    {
        Size winSize = Director::getInstance()->getWinSize();
        animationNode = CSLoader::createNode(resStr);
        animationNode->setAnchorPoint(Vec2(0.5, 0.5));
        animationNode->setPosition(Vec2::ZERO);
        if (animationNode->getParent()!=this) {
            addChild(animationNode);
        }
        
        cannonType = info.cannonType;
    }
    
    if (info.multiple != multiple)
    {
        
        multiple = info.multiple;
    }
    ActionTimeline *action = CSLoader::createTimeline(resStr);
    std::string animStr("stop_");
//    animStr += std::to_string(bulletCount);
    animStr += Convert2String(bulletCount);
    animationNode->runAction(action);
    action->play(animStr, false);
    
}

void CannonNode::changeCannon(int typeId)
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
//    cache->addSpriteFramesWithFile("game_scene/cannon/gun/carriage.plist");
//    cache->addSpriteFramesWithFile("game_scene/cannon/gun/foudation.plist");
    
    cannonType = typeId;
    carriageType = gCannonIndex[cannonType];
    
    std::string carrStr("game_scene/cannon/gun/gundown_gunup");
//    carrStr += std::to_string(carriageType);
    carrStr += Convert2String(carriageType);
    carrStr += ".png";
    carriage->initWithFile(carrStr);
    
    std::string resStr("game_scene/cannon/gun/gun_gun");
//    resStr += std::to_string(cannonType);
    resStr += Convert2String(cannonType);
    ((Sprite*)animationNode->getChildByName("gun_gun0_2_4"))->initWithFile(resStr+"_2.png");
    ((Sprite*)animationNode->getChildByName("gun_gun0_3_2"))->initWithFile(resStr+"_3.png");
    ((Sprite*)animationNode->getChildByName("gun_gun0_4_4"))->initWithFile(resStr+"_4.png");
    
    std::string foudStr("game_scene/cannon/gun/gundown_gundown");
//    foudStr += std::to_string(carriageType);
    foudStr += Convert2String(carriageType);
    foudStr += ".png";
    foudation->initWithFile(foudStr);
    
    energyNode->setVisible(cannonType == CANNON_TYPE_NENGLIANG);
    if (cannonType == CANNON_TYPE_NENGLIANG) {
        AUDIO_PLAY("energy", AUDIO_TYPE::EFFECT_TYPE);
    }
}

void CannonNode::changeMultiply(int val)
{
    multiple = val;
//    multiLabel->setString(std::to_string(multiple));
    multiLabel->setString(Convert2String(multiple));
    mulEffectNode->setVisible(true);
    effectAction->play("play", false);
    AUDIO_PLAY("gun_up", AUDIO_TYPE::EFFECT_TYPE);
}

void CannonNode::changeBulletCount(int val)
{
    bulletCount = val;
    std::string animStr("stop_");
//    animStr += std::to_string(bulletCount);
    animStr += Convert2String(bulletCount);
    animateAction->play(animStr, false);
}

void CannonNode::toward(float angle_in)
{
    angle = 90-angle_in;
    animationNode->setRotation(angle);
}

void CannonNode::setLock(bool lock, float dis)
{
    int count = dis/(16.0*0.8);
    if (!lock) {
        for (int i = 0; i < 100; i++) {
            sdPoint[i]->setVisible(false);
        }
    }else{
        for (int i = 0; i < 100; i++) {
            sdPoint[i]->setVisible(i<count);
        }
    }
}

void CannonNode::questFire(int fishId, float bulletX, float bulletY)
{
    E2L_CANNON_FIRE info;
    info.eProtocol = e2l_cannon_fire;
    info.angle = angle;
    info.traceFishId = fishId;
    info.posX = bulletX;
    info.posY = bulletY;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CannonNode::fire()
{
    if (rayEffectNode->isVisible()) {
        rayEffectAction->play("fire", false);
        rayEffectAction->setLastFrameCallFunc(CC_CALLBACK_0(CannonNode::hideRay, this));
    }
    animateAction->stop();
    std::string animStr("fire_");
//    animStr += std::to_string(bulletCount);
    animStr += Convert2String(bulletCount);
    animateAction->play(animStr, false);
}

void CannonNode::clickAdd(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CHANGE_CANNON_MULTIPLY info;
    info.eProtocol = e2l_change_cannon_multi;
    info.val = 1;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CannonNode::clickMinus(cocos2d::Ref *pSender)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_CHANGE_CANNON_MULTIPLY info;
    info.eProtocol = e2l_change_cannon_multi;
    info.val = -1;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void CannonNode::switchAutoCircle(bool open)
{
    autoCircle->setVisible(open);
}

void CannonNode::switchFireCircle(bool open)
{
    fireCircle->setVisible(open);
    if (open) {
        autoCircle->setVisible(false);
    }
}

void CannonNode::readyRay()
{
    fireCircle->setVisible(false);
    autoCircle->setVisible(false);
    
    rayEffectNode->setVisible(true);
    rayEffectAction->play("wait", true);
    rayEffectAction->setLastFrameCallFunc(nullptr);
}

void CannonNode::endRay(bool autoFire)
{
    switchAutoCircle(autoFire);
}

void CannonNode::hideRay()
{
    rayEffectNode->setVisible(false);
    rayEffectAction->stop();
}

void CannonNode::readyPlane()
{
    fireCircle->setVisible(false);
    autoCircle->setVisible(false);
    
}

void CannonNode::endPlane(bool autoFire)
{
    switchAutoCircle(autoFire);
}

void CannonNode::updateRelife(cocos2d::EventCustom *event)
{
    L2E_UPDATE_RELIFE info = *static_cast<L2E_UPDATE_RELIFE*>(event->getUserData());
    switch(info.state)
    {
        case 0:
        {
            relifeButton->setEnabled(false);
            relifeButton->setTouchEnabled(false);
            relifeButton->setBright(false);
            relifeButton->setVisible(false);
            bankRuptcySpt->setVisible(false);
        }
            break;
        case 1:
        {
            bankRuptcySpt->setVisible(true);
            relifeButton->setVisible(true);
            relifeButton->setEnabled(false);
            relifeButton->setTouchEnabled(false);
            relifeButton->setBright(false);
            boundLabel->setVisible(false);
            relifeCountLabel->setVisible(true);
            int second = (int)info.countDown % 60;
            int minutes = (int)info.countDown /60 % 60;
            std::string countStr("");
            if (minutes < 10) {
                countStr += "0";
            }
//            countStr += std::to_string(minutes);
            countStr += Convert2String(minutes);
            countStr += ":";
            if (second < 10) {
                countStr += "0";
            }
//            countStr += std::to_string(second);
            countStr += Convert2String(second);
            relifeCountLabel->setString(countStr);
        }
            break;
        case 2:
        {
            bankRuptcySpt->setVisible(true);
            relifeButton->setVisible(true);
            relifeButton->setEnabled(true);
            relifeButton->setTouchEnabled(true);
            relifeButton->setBright(true);
            relifeCountLabel->setVisible(false);
            boundLabel->setVisible(true);
            
//            boundLabel->setString(std::to_string(info.bound));
            boundLabel->setString(Convert2String(info.bound));
            relifeButton->addClickEventListener(CC_CALLBACK_0(CannonNode::takRelife, this));
        }
            break;
        case 3:
        {
            bankRuptcySpt->setVisible(false);
            relifeButton->setEnabled(false);
            relifeButton->setTouchEnabled(false);
            relifeButton->setBright(false);
            relifeButton->setVisible(false);
            GOLD_FALL_ANIMATION();
        }
            break;
    }
}

void CannonNode::takRelife()
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    E2L_TAKE_RELIFE info;
    info.eProtocol = e2l_take_relife;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
