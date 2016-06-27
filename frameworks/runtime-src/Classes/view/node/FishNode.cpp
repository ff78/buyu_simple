
//  FishNode.cpp
//  buyu_simple
//
//  Created by ff on 16/1/12.
//
//

#include "FishNode.h"
#include "data/FishManager.h"
#include "../Share/Header/EventDef.h"
#include "../Share/Header/ELProtocol.h"
#include "logic/ClientLogic.h"
#include "utils/MathAide.h"
#include "data/SkillManager.h"
#include "sound/SoundManager.h"

bool FishNode::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    dying = false;
    light = false;
    return true;
}

void FishNode::initWithRes()
{
//    auto map = FishManager::instance()->fishMap;
    if(FishManager::instance()->fishMap.find(fishId) == FishManager::instance()->fishMap.end())
    {
        return;
    }
    
    auto data = FishManager::instance()->fishMap[fishId];
    
    soundName = data.getSound();
    std::string resDir("game_scene/fish/small_fish_alive/");
    int fishType = data.getFishType();
    switch (fishType) {
        case SMALL_FISH:
        {
           resDir = "game_scene/fish/small_fish_alive/";
        }
            break;
        case TINY_BOSS_FISH:
        {
            resDir = "game_scene/fish/tiny_boss_fish_alive/";
        }
            break;
        case BOSS_FISH:
        {
            resDir = "game_scene/fish/boss_fish_alive/";
        }
            break;
        case SPECIAL_FISH:
        {
            resDir = "game_scene/fish/special_fish_alive/";
        }
            break;
            
        default:
        {
            resDir = "game_scene/fish/small_fish_alive/";
        }
            break;
    }
    resDir += data.getResource();
    resDir += ".csb";
    
    resName = resDir;
    
    if (data.getGroupType() == FishGroupType::THREE_GROUP) {
        backNode = CSLoader::createNode(THREE_GROUP_ANIM);
        backNode->setAnchorPoint(Vec2(0.5, 0.5));
        backNode->setPosition(Vec2::ZERO);
        addChild(backNode);
        
        ActionTimeline *action = CSLoader::createTimeline(THREE_GROUP_ANIM);
        backNode->runAction(action);
        
        action->play("alive", true);
    }else if (data.getGroupType() == FishGroupType::FOUR_GROUP) {
        backNode = CSLoader::createNode(FOUR_GROUP_ANIM);
        backNode->setAnchorPoint(Vec2(0.5, 0.5));
        backNode->setPosition(Vec2::ZERO);
        addChild(backNode);
        
        ActionTimeline *action = CSLoader::createTimeline(FOUR_GROUP_ANIM);
        backNode->runAction(action);
        
        action->play("alive", true);
    }else if (data.getGroupType() == FishGroupType::ALL_NET_GROUP) {
        backNode = CSLoader::createNode(ALL_NET_ANIM);
        backNode->setAnchorPoint(Vec2(0.5, 0.5));
        backNode->setPosition(Vec2::ZERO);
        addChild(backNode);
        
        ActionTimeline *action = CSLoader::createTimeline(ALL_NET_ANIM);
        backNode->runAction(action);
        
        action->play("alive", true);
    }else if (data.getGroupType() == FishGroupType::FISH_TYPE_BOMB_GROUP) {
        backNode = CSLoader::createNode(BOMB_TYPE_FISH_ANIM);
        backNode->setAnchorPoint(Vec2(0.5, 0.5));
        backNode->setPosition(Vec2::ZERO);
        addChild(backNode);
        
        ActionTimeline *action = CSLoader::createTimeline(BOMB_TYPE_FISH_ANIM);
        backNode->runAction(action);
        
        action->play("alive", true);
    }
    
    animationNode = CSLoader::createNode(resName);
    animationNode->setAnchorPoint(Vec2(0.5, 0.5));
    animationNode->setPosition(Vec2::ZERO);
    addChild(animationNode);
    
    actTimeLine = CSLoader::createTimeline(resName);
    animationNode->runAction(actTimeLine);
    
    actTimeLine->play("alive", true);
    if (fishType == SMALL_FISH) {
        animationNode->setScale(0.8);
    }
    

    sdSprite = Sprite::create("game_scene/right_menu/rm_sd0.png");
    sdSprite->setVisible(false);
    addChild(sdSprite);
    
    alive = true;
    dying = false;
    
    auto originRect = data.getCollideRect();
    if (fishType == SMALL_FISH) {
        originRect.size = originRect.size.operator*(0.8);
    }
    
//    Vec2 center = Vec2(animationNode->getContentSize().width/2,animationNode->getContentSize().height/2);
//    auto rectNode = DrawNode::create();
//    Vec2 rectangle[4];
//    rectangle[0] = Vec2(-originRect.origin.x, originRect.origin.y-originRect.size.height);
//    rectangle[1] = Vec2(originRect.size.width-originRect.origin.x, originRect.origin.y-originRect.size.height);
//    rectangle[2] = Vec2(originRect.size.width-originRect.origin.x, originRect.origin.y);
//    rectangle[3] = Vec2(-originRect.origin.x, originRect.origin.y);
//
//    Color4F white(1, 1, 1, 1);
//    rectNode->drawPolygon(rectangle, 4, white, 1, white);
//    animationNode->addChild(rectNode);
    
//    vec1 = Vec2(1, 0);
//    vec2 = Vec2(-1, 0);
//    vec3 = Vec2(0, 1);
//    vec4 = Vec2(0, -1);
    collideSize = Size(originRect.size.width, originRect.size.height);
    warnRadius = Vec2(originRect.size.width, originRect.size.height).length()/2;
    deathRadius = MIN(originRect.size.width, originRect.size.height)/2;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(FishNode::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(FishNode::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(FishNode::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
//    pointHitTest(Vec2(30,0));

//    scheduleUpdate();
    setPosition(-2000,-2000);
    
//    rectHitTest(Vec2::ZERO, Size(64, 1000), 0, Vec2(40, 12), Size(40, 78), 0);
}

void FishNode::update(float dt)
{
    if (dying) {
        return;
    }
//    auto map = FishManager::instance()->fishMap;
    if(FishManager::instance()->fishMap.find(fishId) == FishManager::instance()->fishMap.end())
    {
        return;
    }
    
    auto data = FishManager::instance()->fishMap[fishId];
    if(data.getPause())
    {
        return;
    }
    if (data.getOutScreen()) {
        data.setStatus(FISH_OUT);
        alive = false;

        E2L_REMOVE_OUT_FISH info;
        info.eProtocol = e2l_remove_out_fish;
        info.fishId = fishId;
        ClientLogic::instance()->ProcessUIRequest(&info);
        
        return;
    }
    
    switch (data.getStatus()) {
        case FISH_LIGHTNING:
        {
            light = true;
            E2L_FISH_LIGHT_DEAD info;
            info.eProtocol = e2l_fish_light_dead;
            info.fishId = fishId;
            ClientLogic::instance()->ProcessUIRequest(&info);
            return;
        }
            break;
        case FISH_DIED:
        {
            dying = true;
            light = false;
            if (soundName != "null") {
                AUDIO_PLAY(soundName, AUDIO_TYPE::EFFECT_TYPE);
            }
//            if (data.getGroupFirstId() != -1 && data.getCatchScore() != 0) {
//                animationNode->setScale(1.3f);
//                actTimeLine->play("dead", false);
//                actTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(FishNode::dead, this));
//                coinType = 2;
//            }else{
//                if (data.getTypeID() == FISH_TYPE_EX_32 && data.getGroupFirstId() == -1) {
//                    actTimeLine->stop();
//                    animationNode->setVisible(false);
//                    coinType = 0;
//                    dead();
//                }else
            int fishType = data.getFishType();
            switch (fishType) {
                case SMALL_FISH:
                {
                    animationNode->setScale(1.0f);
                    actTimeLine->play("dead", false);
                    actTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(FishNode::dead, this));
                    coinType = -1;
                    fishEffectType = SMALL_FISH;
                }
                    break;
                case TINY_BOSS_FISH:
                {
                    animationNode->setScale(1.0f);
                    actTimeLine->play("dead", false);
                    actTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(FishNode::dead, this));
                    coinType = -1;
                    fishEffectType = TINY_BOSS_FISH;
                }
                    break;
                case BOSS_FISH:
                {
                    actTimeLine->stop();
                    animationNode->setVisible(false);
                    coinType = 0;
                    if (data.getTypeID() == FISH_TYPE_EX_22 || data.getTypeID() == FISH_TYPE_EX_36) {
                        fishEffectType = SPECIAL_BOSS_FISH;
                    }else{
                        fishEffectType = BOSS_FISH;
                    }
                    dead();
                }
                    break;
                case GROUP_FISH:
                {
                    actTimeLine->stop();
                    animationNode->setVisible(false);
                    if (data.getCatchScore() > 0) {
                        coinType = 2;
                        fishEffectType = SPECIAL_FISH;
                    }else{
                        coinType = 2;
                        fishEffectType = SMALL_FISH;
                    }
                    
                    dead();
                }
                    break;
                case SPECIAL_FISH:
                {
                    if (data.getTypeID() == FISH_TYPE_EX_39 || data.getTypeID() == FISH_TYPE_EX_41) {
                        actTimeLine->play("dead", false);
                        actTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(FishNode::dead, this));
                    }else{
                        actTimeLine->stop();
                        animationNode->setVisible(false);
                        coinType = 0;
                        if (data.getTypeID() == FISH_TYPE_EX_32) {
                            fishEffectType = SPECIAL_BOSS_FISH;
                        }else{
                            fishEffectType = SPECIAL_FISH;
                        }
                        
                        dead();
                    }
                }
                    break;
                    
                default:
                    break;
            }
//                if (data.getFishType() == SMALL_FISH || data.getFishType() == TINY_BOSS_FISH) {
////                    animationNode->setScale(1.3f);
//                    actTimeLine->play("dead", false);
//                    actTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(FishNode::dead, this));
//                    coinType = -1;
//                } else if (data.getFishType() == BOSS_FISH) {
//                    actTimeLine->stop();
//                    animationNode->setVisible(false);
//                    coinType = 0;
//                    dead();
//                } else if (data.getFishType() == GROUP_FISH) {
//                    actTimeLine->stop();
//                    animationNode->setVisible(false);
//                    coinType = 2;
//                    dead();
//                } else if (data.getFishType() == SPECIAL_FISH) {
//                    if (data.getTypeID() == FISH_TYPE_EX_39 || data.getTypeID() == FISH_TYPE_EX_41) {
//                        actTimeLine->play("dead", false);
//                        actTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(FishNode::dead, this));
//                    }else{
//                        actTimeLine->stop();
//                        animationNode->setVisible(false);
//                        coinType = 0;
//                        dead();
//                    }
//                    
//                }else{
//                    actTimeLine->stop();
//                    animationNode->setVisible(false);
//                    coinType = 1;
//                    dead();
//                }
            
//            }
            
            if ((data.getFishType() != SMALL_FISH && (data.getTypeID() != FISH_TYPE_EX_16 && data.getTypeID() != FISH_TYPE_EX_17) && data.getFishType() != GROUP_FISH)
//            if ((data.getTypeID() >= FISH_TYPE_EX_18 && data.getTypeID() <= FISH_TYPE_EX_33)
////                || (data.getTypeID() == FISH_TYPE_EX_4 && data.getGroupFirstId() == -1)
                ) {
                EventCustom event(SHAKE_SCREEN);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
            if (lock) {
                EventCustom event(CLEAR_LOCK_FISH);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
        }
            return;
        case FISH_CLEAR:
        {
            alive = false;
//            dead();
            E2L_REMOVE_OUT_FISH info;
            info.eProtocol = e2l_remove_out_fish;
            info.fishId = fishId;
            ClientLogic::instance()->ProcessUIRequest(&info);
//            dying = true;
//            light = false;
//            
//            if (data.getFishType() == SMALL_FISH || data.getFishType() == TINY_BOSS_FISH) {
//                animationNode->setScale(1.3f);
//                actTimeLine->play("dead", false);
//                actTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(FishNode::dead, this));
//                coinType = -1;
//            } else if (data.getFishType() == BOSS_FISH) {
//                actTimeLine->stop();
//                animationNode->setVisible(false);
//                coinType = 0;
//                dead();
//            } else if (data.getFishType() == GROUP_FISH) {
//                actTimeLine->stop();
//                animationNode->setVisible(false);
//                coinType = 2;
//                dead();
//            } else if (data.getFishType() == SPECIAL_FISH) {
//                if (data.getTypeID() == FISH_TYPE_EX_39 || data.getTypeID() == FISH_TYPE_EX_41) {
//                    actTimeLine->play("dead", false);
//                    actTimeLine->setLastFrameCallFunc(CC_CALLBACK_0(FishNode::dead, this));
//                }else{
//                    actTimeLine->stop();
//                    animationNode->setVisible(false);
//                    coinType = 0;
//                    dead();
//                }
//                
//            }else{
//                actTimeLine->stop();
//                animationNode->setVisible(false);
//                coinType = 1;
//                dead();
//            }
            
            if (lock) {
                EventCustom event(CLEAR_LOCK_FISH);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
        }
            return;
            
            case FISH_HURT:
        {
            animationNode->setColor(Color3B::RED);
            FishManager::instance()->fishMap[fishId].setStatus(FISH_ALIVE);
            stopActionByTag(10);
            auto white = CallFunc::create(CC_CALLBACK_0(FishNode::back2Normal, this));
            auto seq = Sequence::create(DelayTime::create(0.3f), white, NULL);
            seq->setTag(10);
            runAction(seq);            
        }
            break;
            
        default:
            break;
    }
    
    
    setPosition(data.getPosition());
    setRotation(data.getDirection());
    if (lock) {
        auto winSize = Director::getInstance()->getWinSize();
        if (getPositionX()<0 || getPositionX() >winSize.width || getPositionY()<0 || getPositionY() > winSize.height) {
            EventCustom event(CLEAR_LOCK_FISH);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }
}

bool FishNode::pointHitTest(cocos2d::Vec2 bulletPos)
{
//    auto map = FishManager::instance()->fishMap;
    auto data = FishManager::instance()->fishMap[fishId];
    if(data.getPause())
    {
        return false;
    }
    if (data.getOutScreen())
    {
        return false;
    }
    auto position = getPosition();
    auto bPos = position-bulletPos;
    
    return MathAide::pointHitTest(bPos, collideSize, -data.getDirection(), warnRadius, deathRadius);
}

bool FishNode::rectRayHitTest(cocos2d::Vec2 bulletPos, float angle, cocos2d::Size bulletSize)
{
    auto data = FishManager::instance()->fishMap[fishId];
    if(data.getPause())
    {
        return false;
    }
    if (data.getOutScreen())
    {
        return false;
    }
    
    return MathAide::rectHitTest(bulletPos, bulletSize, angle, getPosition(), collideSize, -data.getDirection());
//    auto center = Vec2::forAngle(CC_DEGREES_TO_RADIANS(angle))*bulletSize.height/2;
//    center += bulletPos;
//    
////    bool inRay = false;
//    auto position = getPosition()-center;
//    Vec2 collideNormal = Vec2::forAngle(CC_DEGREES_TO_RADIANS(-data.getDirection()));
//    
//    int offX[4] = {1,1,-1,-1};
//    int offY[4] = {1,-1,-1,1};
//    Vec2 pos[4];
//    for (int i = 0; i < 4; i++) {
//        pos[i] = position+Vec2(offX[i]*collideNormal*collideSize.width/2, offY[i]*collideNormal*collideSize.height/2);
//        bool result = MathAide::pointHitTest(pos[i], bulletSize, angle);
//        if (result) {
//            return true;
//        }
//    }
//
//    return false;
}

bool FishNode::rectHitTest(Vec2 posA, Size sizeA, float angleA, Vec2 posB, Size sizeB, float angleB)
{
    
    float wariningRadiusA = Vec2(sizeA.width, sizeA.height).length()/2;
    float deathRadiusA = MIN(sizeA.width, sizeA.height)/2;
    float wariningRadiusB = Vec2(sizeB.width, sizeB.height).length()/2;
    float deathRadiusB = MIN(sizeA.width, sizeA.height)/2;
    
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
    auto bPos = posA - posB;
    for (int i = 0; i < 4; i++) {
        cornerB[i] += bPos;
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if(Vec2::isLineIntersect(cornerA[i%4], cornerA[(i+1)%4], cornerB[j%4], cornerB[(j+1)%4]))
            {
                return true;
            }
        }
    }
    
    
    return false;
}

bool FishNode::circleHitTest(cocos2d::Vec2 bulletPos, float r)
{

    
//    auto map = FishManager::instance()->fishMap;
    auto data = FishManager::instance()->fishMap[fishId];
    if(data.getPause())
    {
        return false;
    }
    if (data.getOutScreen())
    {
        return false;
    }
    
    auto position = getPosition();
    auto bPos = position-bulletPos;
    
    return MathAide::circleHitTest(bPos, r, collideSize, -data.getDirection(), warnRadius, deathRadius);
}

void FishNode::back2Normal()
{
    animationNode->setColor(Color3B::WHITE);
}

void FishNode::dead()
{
    alive = false;

    
//    auto map = FishManager::instance()->fishMap;
    auto data = FishManager::instance()->fishMap[fishId];
//    if (data.getCatchScore() != 0) {
        auto winSize = Director::getInstance()->getWinSize();
        CMD_CREATE_COIN cmd;
        cmd.fishPos = getPosition();
        cmd.targetPos = Vec2(winSize.width/2, 45);
        cmd.score = data.getCatchScore();
        cmd.baseScore = data.getBaseScore();
        cmd.type = coinType;
        cmd.fishEffectType = fishEffectType;
    
        EventCustom event(ADD_COIN);
        event.setUserData(&cmd);
        Director::getInstance()->getInstance()->getEventDispatcher()->dispatchEvent(&event);
//    }
    
    
    E2L_FISH_DEAD info;
    info.eProtocol = e2l_fish_dead;
    info.fishId = fishId;
    ClientLogic::instance()->ProcessUIRequest(&info);

}

bool FishNode::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    Vec2 locationInNode = this->convertToWorldSpace(touch->getLocation());
//    Size s = this->getContentSize();
//    Rect rect = Rect(0, 0, s.width, s.height);
//    
//    if (rect.containsPoint(locationInNode))
    if (!SkillManager::instance()->skillMap[1].running) {
        return false;
    }
    auto data = FishManager::instance()->fishMap[fishId];
    if (data.getPause()) {
        return false;
    }
    if (data.getStatus() != FISH_ALIVE && data.getStatus() != FISH_HURT) {
        return false;
    }
    if (pointHitTest(touch->getLocation()))
    {
        click = true;
//        animationNode->setColor(Color3B::RED);
        log("fish%d touch began", fishId);
        
        return true;
    }

    return false;
}

void FishNode::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    log("fish%d touch end",fishId);
    cocos2d::Vec2 v;
    cocos2d::Rect r;
}

void FishNode::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    log("fish%d touch cancel",fishId);
}

void FishNode::setLock(bool lock)
{
    this->lock = lock;
    sdSprite->setVisible(lock);
    if (lock) {
        AUDIO_PLAY("locking", AUDIO_TYPE::EFFECT_TYPE);
    }
    
//    if (lock) {
//        animationNode->setColor(Color3B::RED);
//    }else{
//        animationNode->setColor(Color3B::WHITE);
//    }
}
