//
//  GoldGameScene.cpp
//  buyu_simple
//
//  Created by ff on 15/12/22.
//
//

#include "GoldGameScene.h"
#include "data/BlackPliers.h"
#include "../UIManager.h"
#include "logic/ClientLogic.h"
#include "sound/SoundManager.h"
#include "GameDef.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"

GoldGameScene::~GoldGameScene()
{
    unscheduleUpdate();
    Director::getInstance()->getEventDispatcher()->removeEventListener(changePilersListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(addBombListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(addKeyListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resultListener);
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile(BLAST_PLST);
}

bool GoldGameScene::init()
{
    if(!Scene::init())
    {
        return false;
    }
    
    auto gameData = BlackGoldGame::instance();
    
    
    auto winSize = Director::getInstance()->getWinSize();
    std::string filePath("img/");
    filePath += gameData->getBgRes();
    auto bgSpt = Sprite::create(filePath);
    bgSpt->setPosition(winSize.width/2, winSize.height/2);
    addChild(bgSpt);
    
    filePath = "img/";
    filePath += gameData->getMaskRes();
    auto maskSpt = Sprite::create(filePath);
    maskSpt->setPosition(winSize.width/2, winSize.height/2);
    addChild(maskSpt);
    
    auto uiLayer = Layer::create();
    filePath = "img/tg_up.png";
    auto uiBgSpt = Sprite::create(filePath);
    uiBgSpt->setPosition(Vec2(winSize.width/2, winSize.height));
    uiBgSpt->setAnchorPoint(Vec2(0.5, 1));
    uiLayer->addChild(uiBgSpt);
    
    score = TextAtlas::create("0123456789", "img/game_number_1.png", 14.5, 18, "0");
    score->setString(Convert2String(gameData->getStageMoney()));
    score->setAnchorPoint(Vec2(0, 0.5));
    score->setPosition(Vec2(61, 64));
    uiBgSpt->addChild(score);
    
    bombBtn = Button::create("img/tg_prop_1.png");
    bombBtn->setPosition(Vec2(uiBgSpt->getContentSize().width/2 + 150, 70));
    bombBtn->addTouchEventListener(CC_CALLBACK_2(GoldGameScene::clickBomb, this));
    uiBgSpt->addChild(bombBtn);
    
    bombCount = TextAtlas::create("0123456789", "img/game_number_1.png", 14.5, 18, "0");
    bombCount->setPosition(Vec2(bombBtn->getContentSize().width/2, bombBtn->getContentSize().height/2));
    bombBtn->addChild(bombCount);
    if (gameData->getBombCount() <= 0) {
        bombBtn->setVisible(false);
    }else{
        bombCount->setString(Convert2String(gameData->getBombCount()));
    }
    
    auto stageLabel = Label::createWithSystemFont(Convert2String((int)gameData->getSceneId()), "Arifa", 26);
    stageLabel->setAnchorPoint(Vec2(0, 0.5));
    stageLabel->setPosition(Vec2(290, 100));
    uiBgSpt->addChild(stageLabel);
    
    timeLabel = Label::createWithSystemFont(Convert2String((int)gameData->getGameTime()), "Arifa", 26);
    score->setAnchorPoint(Vec2(0, 0.5));
    timeLabel->setPosition(Vec2(290, 64));
    uiBgSpt->addChild(timeLabel);
    
    int keyCount = gameData->getKeyCount();
    for (int i = 0; i < 5; i++) {
        if (i < keyCount) {
            filePath = "img/tg_goods_12.png";
        }else{
            filePath = "img/tg_key.png";
        }
        keySpt[i] = Sprite::create(filePath);
        keySpt[i]->setPosition(Vec2(uiBgSpt->getContentSize().width/2+210+i*33, 80));
        uiBgSpt->addChild(keySpt[i]);
    }
    
    addChild(uiLayer);
    
    auto pliersLayer = Layer::create();
    auto rudder = Sprite::create("img/tg_rudder.png");
    rudder->setPosition(Vec2(winSize.width/2, winSize.height-45));
    pliersLayer->addChild(rudder);
    
    
    auto clipper = ClippingNode::create();
    clipper->setContentSize(  Size(winSize.width, 200) );
    clipper->setAnchorPoint(  Vec2(0.5, 0) );
    clipper->setPosition(winSize.width/2, winSize.height-45);
    clipper->setInverted(true);
    pliersLayer->addChild(clipper);
    auto stencil = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, 0);
    rectangle[1] = Vec2(clipper->getContentSize().width, 0);
    rectangle[2] = Vec2(clipper->getContentSize().width, clipper->getContentSize().height);
    rectangle[3] = Vec2(0, clipper->getContentSize().height);
    
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clipper->setStencil(stencil);
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ROPE_ARMATURE);
    rope = Armature::create("rope");

    rope->setPosition(clipper->getContentSize().width / 2, 0);
    clipper->addChild(rope);
    rope->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(GoldGameScene::RopeMovementListener, this));

    auto rotate1 = RotateTo::create(ROTATE_DURATION/2, MIN_ANGLE);
    auto rotate2 = RotateTo::create(ROTATE_DURATION, MAX_ANGLE);
    auto rotate3 = RotateTo::create(ROTATE_DURATION/2, 0);
//    auto seq = Sequence::create(rotate1, RepeatForever::create(Sequence::create(rotate2, rotate3, nullptr)), nullptr);
    
    auto seq = Sequence::create(rotate1, rotate2, rotate3, nullptr);
    auto repeat = RepeatForever::create(seq);

    rope->runAction(repeat);
//    auto speedVec = Vec2(300,0);
////    speedVec.normalize();
//    auto otherVec = speedVec.rotateByAngle(Vec2::ZERO, -30);
////    speedVec.scale(300);
//    auto act1 = MoveBy::create(3.0, otherVec);
//    auto act2 = MoveTo::create(3.0, otherVec);
//    content->runAction(act2);
    
    auto skull = Sprite::create("img/tg_skull.png");
    skull->setPosition(Vec2(winSize.width/2, winSize.height-45));
    pliersLayer->addChild(skull);
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile(BLAST_PLST);
    
    
    Vector<SpriteFrame*> animFrames(7);
    char str[100] = {0};
    for(int i = 1; i < 7; i++)
    {
        sprintf(str, "blast_%d.png",i);
        auto frame = cache->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.03f);
    // Add an animation to the Cache
    AnimationCache::getInstance()->addAnimation(animation, "blast");

    
    objLayer = Layer::create();
    int objCount = (int)gameData->GoldObjs.size();
    objInfoVec.clear();
    
    float offX = winSize.width/2 - MAX_COL*GRID_SIZE/2;
    for(int i = 0; i<objCount; i++ )
    {
        
        if (gameData->GoldObjs[i].getMoveType() == 1) {
            filePath = "armature/";
            filePath += gameData->GoldObjs[i].getResourceName();
            filePath += "/";
            filePath += gameData->GoldObjs[i].getResourceName();
            filePath += ".ExportJson";
            ArmatureDataManager::getInstance()->addArmatureFileInfo(filePath);
            auto objSpt = Armature::create(gameData->GoldObjs[i].getResourceName());
            objSpt->getAnimation()->playWithIndex(0);
            objSpt->setPosition(Vec2(gameData->GoldObjs[i].getPos().x+offX, gameData->GoldObjs[i].getPos().y));
            OBJ_INFO *info = new OBJ_INFO();
            info->serno = i;
            info->moveType = 1;
            Rect originRect = gameData->GoldObjs[i].getCollideRect();
            info->collideRect = Rect(-originRect.origin.x, -originRect.origin.y, originRect.size.width, originRect.size.height);
            objSpt->setUserData(info);
            
            
            float dis = abs(rand())%100+100;
            float duration = abs(rand())%3+2;
            bool flip = abs(rand())%100>50;
            int sign = flip?1:-1;
            auto move1 = MoveTo::create(duration/2, Vec2(objSpt->getPositionX()+sign*dis/2, objSpt->getPositionY()));
            auto move2 = MoveTo::create(duration, Vec2(objSpt->getPositionX()-sign*dis/2, objSpt->getPositionY()));
            auto move3 = MoveTo::create(duration/2, Vec2(objSpt->getPositionX(), objSpt->getPositionY()));

            auto act1 = CallFunc::create( std::bind(&GoldGameScene::flipGoldNode, this, objSpt));
            auto seq1 = Sequence::create(move1, act1, move2, act1, move3, NULL);
            objSpt->setScaleX(sign);
            objSpt->runAction(RepeatForever::create(seq1));

            objLayer->addChild(objSpt);
            
//            Vec2 center = Vec2(objSpt->getContentSize().width/2,objSpt->getContentSize().height/2);
//            auto rectNode = DrawNode::create();
//            Vec2 rectangle[4];
//            rectangle[0] = Vec2(-originRect.origin.x, originRect.origin.y-originRect.size.height);
//            rectangle[1] = Vec2(originRect.size.width-originRect.origin.x, originRect.origin.y-originRect.size.height);
//            rectangle[2] = Vec2(originRect.size.width-originRect.origin.x, originRect.origin.y);
//            rectangle[3] = Vec2(-originRect.origin.x, originRect.origin.y);
//            
//            Color4F white(1, 1, 1, 1);
//            rectNode->drawPolygon(rectangle, 4, white, 1, white);
//            objSpt->addChild(rectNode);
        }else{
            filePath = "img/";
            filePath += gameData->GoldObjs[i].getResourceName();
            filePath += ".png";
            auto objSpt = Sprite::create(filePath);
            objSpt->setAnchorPoint(Vec2(0.5, 0.5));
            objSpt->setPosition(Vec2(gameData->GoldObjs[i].getPos().x+offX, gameData->GoldObjs[i].getPos().y));
            OBJ_INFO *info = new OBJ_INFO();
            info->serno = i;
            info->moveType = 0;
            Rect originRect = gameData->GoldObjs[i].getCollideRect();
            info->collideRect = Rect(objSpt->getPositionX()-originRect.origin.x, objSpt->getPositionY()-originRect.origin.y, originRect.size.width, originRect.size.height);
            objSpt->setUserData(info);
    //        objInfoVec.push_back(info);
            objLayer->addChild(objSpt);
            
//            Vec2 center = Vec2(objSpt->getContentSize().width/2,objSpt->getContentSize().height/2);
//            auto rectNode = DrawNode::create();
//            Vec2 rectangle[4];
//            rectangle[0] = Vec2(center.x - originRect.origin.x, center.y + originRect.origin.y-originRect.size.height);
//            rectangle[1] = Vec2(center.x + originRect.size.width-originRect.origin.x, center.y + originRect.origin.y-originRect.size.height);
//            rectangle[2] = Vec2(center.x + originRect.size.width-originRect.origin.x, center.y + originRect.origin.y);
//            rectangle[3] = Vec2(center.x - originRect.origin.x, center.y + originRect.origin.y);
//            
//            Color4F white(1, 1, 1, 1);
//            rectNode->drawPolygon(rectangle, 4, white, 1, white);
//            objSpt->addChild(rectNode);
        }
    }
    
    addChild(objLayer);
    
    addChild(pliersLayer);
    
    flagSpt = Sprite::create("img/tg_pliers.png");
    addChild(flagSpt);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GoldGameScene::touchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GoldGameScene::touchScreen, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    pliersLayer->addTouchEventListener(CC_CALLBACK_2(GoldGameScene::touchScreen, this));
    scheduleUpdate();
    
    changePilersListener = EventListenerCustom::create(CHANGE_PLIERS_ACTION, CC_CALLBACK_1(GoldGameScene::onChangePilers, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(changePilersListener, -1);
    addBombListener = EventListenerCustom::create(ADD_BOMB, CC_CALLBACK_1(GoldGameScene::onAddBomb, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addBombListener, -1);
    addKeyListener = EventListenerCustom::create(ADD_KEY, CC_CALLBACK_1(GoldGameScene::onAddKey, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addKeyListener, -1);
    resultListener = EventListenerCustom::create(GOLD_RESULT, CC_CALLBACK_1(GoldGameScene::onResult, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resultListener, -1);
    
    return true;
}

void GoldGameScene::flipGoldNode(Armature *node)
{
//    Sprite* spt = (Sprite*)node;
    float scaleX = node->getScaleX();
    node->setScaleX(scaleX*-1);
}

void GoldGameScene::onChangePilers(EventCustom* event)
{
    auto winSize = Director::getInstance()->getWinSize();
    auto pliers = BlackPliers::instance();
    std::string actionName = pliers->getActionName();
    if (actionName == "idle") {
        rope->setPosition(Vec2(winSize.width/2,0));
        rope->getAnimation()->play("push");
        rope->getAnimation()->gotoAndPause(0);
        int sign = ((abs(rand())%10>5)?1:-1);
        float angle = sign*(MAX_ANGLE-abs(rand()%10));
        rope->setRotation(angle);
        
        float rightDuration = ROTATE_DURATION*fabs(MIN_ANGLE-angle)/140;
        float leftDuration = ROTATE_DURATION*fabs(MAX_ANGLE-angle)/140;
        
        auto rotate1 = RotateTo::create(ROTATE_DURATION/2, MIN_ANGLE);
        auto rotate2 = RotateTo::create(ROTATE_DURATION, MAX_ANGLE);
        auto rotate3 = RotateTo::create(ROTATE_DURATION/2, 0);
        if((abs(rand())%10>5)?1:-1)
        {
            rotate1 = RotateTo::create(rightDuration, MIN_ANGLE);
            rotate2 = RotateTo::create(ROTATE_DURATION, MAX_ANGLE);
            rotate3 = RotateTo::create(leftDuration, angle);
        }else{
            rotate1 = RotateTo::create(leftDuration, MAX_ANGLE);
            rotate2 = RotateTo::create(ROTATE_DURATION, MIN_ANGLE);
            rotate3 = RotateTo::create(rightDuration, angle);
        }
        
        auto seq = Sequence::create(rotate1, rotate2, rotate3, nullptr);
        auto repeat = RepeatForever::create(seq);
        
        rope->runAction(repeat);
    }else if(actionName == "push"){
        rope->stopAllActions();
        rope->getAnimation()->play("push");
    }else if(actionName == "back"){
        auto vec2 = pliersVec-Vec2(0, winSize.height-45);
        rope->setPosition(vec2);
        rope->getAnimation()->play("back");
    }else if(actionName == "find"){
        auto vec2 = pliersVec-Vec2(0, winSize.height-45);
        rope->setPosition(vec2);
        rope->getAnimation()->play("find");
        
        
        std::string pullRes = BlackPliers::instance()->getPullObjRes();
        
        auto skin = Skin::create();
        if (BlackPliers::instance()->getPullMoveType() == 1) {
            
            
//            auto bone = Bone::create("LayerArmature");
//            skin->setBone(bone);
            
//            std::string filePath = "armature/";
//            filePath += pullRes;
//            filePath += "/";
//            filePath += pullRes;
//            filePath += ".ExportJson";
//            ArmatureDataManager::getInstance()->addArmatureFileInfo(filePath);
//            
//            auto objSpt = Armature::create(pullRes);
//            objSpt->getAnimation()->playWithIndex(0);
//            rope->getBone("Layer3")->setArmature(objSpt);

            skin->initWithFile("img/"+pullRes+"_0.png");
        }else{

            skin->initWithFile("img/"+pullRes+".png");
//        rope->getBone("Layer3")->changeDisplayWithName(pullRes+".png", true);
        }
        skin->setAnchorPoint(Vec2(0.5, 1));
        rope->getBone("Layer3")->addDisplay(skin, 0);
        rope->getBone("Layer3")->changeDisplayWithIndex(0, true);

    }else if(actionName == "pull"){
//        auto vec2 = pliersVec-Vec2(0, winSize.height-45);
//        rope->setPosition(vec2);
        float speed = BlackPliers::instance()->getSpeedFactor();
        speed *= 0.1;
        speed *= BlackGoldGame::instance()->getSpeedScale();
        rope->getAnimation()->setSpeedScale(speed);
        rope->getAnimation()->play("pull");
        
    }else if(actionName == "cancel"){
        rope->getAnimation()->setSpeedScale(1);
        auto vec2 = pliersVec-Vec2(0, winSize.height-45);
        rope->setPosition(vec2);
        rope->getAnimation()->play("back");
    }else if(actionName == "bomb"){
        auto pliersInfo = rope->getBone("Layer2")->getWorldInfo();
        
        float angle = rope->getRotation();
        float pliersDis = Vec2(pliersInfo->x, pliersInfo->y).length();
        auto rotateVec = Vec2::forAngle(CC_DEGREES_TO_RADIANS(180-90-angle));
        auto offVec = rotateVec*pliersDis;
        
        auto pos = rope->getPosition() + offVec;
        
        rope->setPosition(pos);
        rope->getAnimation()->setSpeedScale(1);
        rope->getAnimation()->play("bomb");
        
        auto animCache = AnimationCache::getInstance();
        
        auto normal = animCache->getAnimation("blast");
        
        auto sp1 = Sprite::createWithSpriteFrameName("blast_0.png");
        sp1->setPosition(pos);
        sp1->setTag(EFFECT_TAG);
        rope->getParent()->addChild(sp1);
//        normal->setRestoreOriginalFrame(true);
        auto blast = Animate::create(normal);
        auto removeFunc = CallFunc::create(CC_CALLBACK_0(GoldGameScene::removeEffect, this));
        auto seq = Sequence::create(blast, removeFunc, NULL);
        sp1->runAction(seq);
    }else if(actionName == "bombBack"){
        rope->stopAllActions();
        rope->getAnimation()->play("back");
    
    }else if(actionName == "get"){
        rope->getAnimation()->setSpeedScale(1);
        auto vec2 = pliersVec-Vec2(0, winSize.height-45);
        rope->setPosition(Vec2(winSize.width/2,0));
        rope->getAnimation()->play("get");
        E2L_GOLD_GAIN_OBJ info;
        info.eProtocol = e2l_gold_gain_obj;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
}
void GoldGameScene::removeEffect()
{
    rope->getParent()->removeChildByTag(EFFECT_TAG);
}

void GoldGameScene::RopeMovementListener(Armature *armture, MovementEventType type, const std::string &movementName)
{
    if (type == MovementEventType::COMPLETE)
    {
        EventCustom event(PLIERS_ACTION_OVER);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

bool GoldGameScene::touchBegan(cocos2d::Touch *touch, cocos2d::Event *evnt)
{
    return true;
}

void GoldGameScene::touchScreen(Touch* touch, Event* evnt)
{
    AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
    EventCustom event(CLICK_SCREEN);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void GoldGameScene::update(float dt)
{
    auto pliersInfo = rope->getBone("Layer2")->getWorldInfo();
    
    float angle = rope->getRotation();
    float pliersDis = Vec2(pliersInfo->x, pliersInfo->y).length();
    auto rotateVec = Vec2::forAngle(CC_DEGREES_TO_RADIANS(-90-angle));
    pliersVec = rotateVec * pliersDis;
    auto winSize = Director::getInstance()->getWinSize();
    pliersVec += Vec2(winSize.width/2, winSize.height-45);
    
    if (!Rect(Vec2::ZERO, winSize).containsPoint(pliersVec)) {
        EventCustom event(OUT_OF_SCREEN);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
    
    auto pliers = BlackPliers::instance();
    PLIERSTATE currState = pliers->getState();
    if (currState == BACK || currState == PULL || currState == CANCEL ||  currState == BOMBBACK) {
        if (rope->getAnimation()->getCurrentFrameIndex() > 0) {
            float totalDis = rope->getPosition().distance(Vec2(winSize.width/2, 0));
            if (totalDis-pliersDis <= 60) {
                EventCustom event(ARRIVE_HOME);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
        }

    }else if (currState == PUSH){
        auto objs = objLayer->getChildren();
        collideObjId.clear();
        for (auto obj : objs ) {
            if (obj->isVisible()) {
                OBJ_INFO info = *static_cast<OBJ_INFO*>(obj->getUserData());
                if (info.moveType == 0) {
                    if (info.collideRect.containsPoint(pliersVec)) {
                        collideObjId.push_back(info.serno);
                    }
                }else{
                    Rect rect = info.collideRect;
                    rect.origin += obj->getPosition();
                    if (rect.containsPoint(pliersVec)) {
                        collideObjId.push_back(info.serno);
                    }
                }

            }
        }
        
        if (!collideObjId.empty()) {
            EventCustom event(FIND_SOMETHING);
            event.setUserData(&collideObjId);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        }
    }
    
    auto objs = objLayer->getChildren();

    for (auto obj : objs ) {
        if (obj->isVisible()) {
            OBJ_INFO *info = static_cast<OBJ_INFO*>(obj->getUserData());
            auto gameData = BlackGoldGame::instance();
            if(!gameData->GoldObjs[info->serno].getAlive())
            {
                obj->setVisible(false);
            }
        }
    }

    score->setString(Convert2String(BlackGoldGame::instance()->getStageMoney()));
    timeLabel->setString(Convert2String((int)BlackGoldGame::instance()->getGameTime()));
    UIManager::instance()->loop(dt);
}

void GoldGameScene::clickBomb(Ref* pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
            if(BlackPliers::instance()->getState() == PULL)
            {
                AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
                auto gameData = BlackGoldGame::instance();
                gameData->useItem(GOLD_ITEM_BOMB_TYPE);
                
                if (gameData->getBombCount() <= 0) {
                    bombBtn->setVisible(false);
                }else{
                    bombCount->setString(Convert2String(gameData->getBombCount()));
                }
            }
            break;
            
        default:
            break;
    }
}

void GoldGameScene::onAddBomb(EventCustom* event)
{
    auto gameData = BlackGoldGame::instance();
    
    if (gameData->getBombCount() <= 0) {
        bombBtn->setVisible(false);
    }else{
        bombCount->setString(Convert2String(gameData->getBombCount()));
    }
}

void GoldGameScene::onAddKey(EventCustom* event)
{
    auto gameData = BlackGoldGame::instance();
    
    int count = gameData->getKeyCount();
    for (int i = 0; i < 5; i++) {
        std::string filePath = "img/";
        if (i < count) {
            filePath += "tg_goods_12.png";
        }else{
            filePath += "tg_key.png";
        }
        keySpt[i]->setTexture(filePath);
    }

}

void GoldGameScene::onResult(EventCustom* event)
{
    L2E_GOLD_RESULT info = *static_cast<L2E_GOLD_RESULT *>(event->getUserData());
    auto winSize = Director::getInstance()->getWinSize();
    
    resultLayer = Layer::create();
    auto bgSpt = Sprite::create("img/common_little.png");
    bgSpt->setPosition(Vec2(winSize.width/2, winSize.height/2));
    resultLayer->addChild(bgSpt);
    
    auto exitBtn = Button::create("img/common_yes.png");
    exitBtn->setPosition(Vec2(bgSpt->getContentSize().width/2, 80));
    exitBtn->addTouchEventListener(CC_CALLBACK_2(GoldGameScene::clickResult, this));
    bgSpt->addChild(exitBtn);
    
    std::string goldResultStr;
    if (info.success) {
        goldResultStr = SSTRING("gold_pass_stage")+Convert2String(info.moneyResult);
    }else{
        goldResultStr = SSTRING("gold_pass_fail");
    }

    auto tipLabel = Label::createWithSystemFont(goldResultStr, "Arifa", 32);
    tipLabel->setColor(Color3B::MAGENTA);
    tipLabel->setAlignment(TextHAlignment::CENTER);
    tipLabel->setAnchorPoint(Vec2(0.5, 1));
    tipLabel->setPosition(Vec2(bgSpt->getContentSize().width/2, 220));
    bgSpt->addChild(tipLabel);
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    resultLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,resultLayer);

    
    addChild(resultLayer);
}

void GoldGameScene::clickResult(Ref* pSender, Widget::TouchEventType type)
{
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            resultLayer->removeFromParent();
            E2L_GOLD_RESULT info;
            info.eProtocol = e2l_gold_result;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
            
        default:
            break;
    }
}

