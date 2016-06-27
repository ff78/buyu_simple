//
//  LoginScene.cpp
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#include "LoginScene.h"
#include "logic/ClientLogic.h"
#include "EventDef.h"
#include "../UIManager.h"
#include "utils/MathAide.h"
#include "sound/SoundManager.h"
#include "utils/StringData.h"

#define RES_LOGIN_BG_PLIST       "ui/hall_scene/background/background.plist"
#define RES_ICON_PLIST           "ui/hall_scene/icon/icon.plist"
#define RES_REGISTERED_PLIST     "ui/hall_scene/registered/registered.plist"
#define RES_NOTICE_PLIST         "ui/common/notice/notice.plist"
#define RES_VIP_PLIST			 "ui/common/vip/vip.plist"
#define RES_FACE_PLIST			 "ui/common/face/face.plist"
#define HALL_PLIST               "ui/hall_scene/game_hall/game_hall.plist"
#define FISHING_ROOM_PLIST       "ui/hall_scene/fish_room/fish_room.plist"
#define MATCH_ROOM_PLIST         "ui/hall_scene/match_hall/match_hall.plist"
#define PKLAYER_ROOM_PLIST       "ui/hall_scene/pk_hall/pk_hall.plist"
#define LOGIN_LOADING_PLIST      "animations/login_loading/login_loading.plist"
#define GOLD_DROP_PLIST          "animations/gold2/gold2.plist"

#define IMG_LOGIN_BG		"background.png"
#define IMG_LOGO			"registered_logo.png"
#define IMG_SHADER			"registered_black_bottom.png"
#define BEGIN_GAME_IMG	"registered_gamestart.png"

USING_NS_CC;
using namespace cocos2d::ui;

bool LoginScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    auto bgSpt = Sprite::create("hall_scene/game_hall/background_0.png");
    bgSpt->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    bgSpt->setScaleX(GameUtils::winSize.width/bgSpt->getContentSize().width);
    addChild(bgSpt);
    
    auto root = CSLoader::createNode("Login.csb");
    root->setAnchorPoint(Vec2(0.5, 0.5));
    root->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(root);
    
//    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
//    cache->addSpriteFramesWithFile(RES_LOGIN_BG_PLIST);
//    cache->addSpriteFramesWithFile(RES_ICON_PLIST);
//    cache->addSpriteFramesWithFile(RES_REGISTERED_PLIST);
//    cache->addSpriteFramesWithFile(RES_NOTICE_PLIST);
//    cache->addSpriteFramesWithFile(RES_VIP_PLIST);
//    cache->addSpriteFramesWithFile(RES_FACE_PLIST);
//    cache->addSpriteFramesWithFile(LOGIN_LOADING_PLIST);
//    cache->addSpriteFramesWithFile(GOLD_DROP_PLIST);
//    
//    Sprite* bg = Sprite::createWithSpriteFrameName(IMG_LOGIN_BG);
//    bg->setPosition(winSize.width/2, winSize.height/2);
//    addChild(bg);
//    
//    auto logo = Sprite::createWithSpriteFrameName(IMG_LOGO);
//    logo->setPosition(winSize.width/2+40, winSize.height*3/4);
//    addChild(logo);
//    
//    auto button = Button::create("BEGIN_GAME_IMG");
//    button->addTouchEventListener(CC_CALLBACK_2(LoginScene::start, this));
    
    auto button = (Button*)root->getChildByName("StartBtn");
//    auto button = Button::create("hall_scene/registered/registered_gamestart.png");
//    button->setPosition(Vec2(winSize.width/2, winSize.height/2));
    
    button->addTouchEventListener(CC_CALLBACK_2(LoginScene::start, this));
    
    loginFailed = EventListenerCustom::create(LOGIN_FAILED, CC_CALLBACK_0(LoginScene::failTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(loginFailed, -1);
    
    AUDIO_PLAY("room", AUDIO_TYPE::MUSIC_TYPE);
    scheduleUpdate();
//    MathAide::rectHitTest(Vec2::ZERO, Size(64, 1000), 5, Vec2(53, 0), Size(40, 78), 0);
    return true;
}

void LoginScene::start(Ref *pSender, Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            AUDIO_PLAY("click", AUDIO_TYPE::EFFECT_TYPE);
            E2L_LOGIN loginInfo;
            loginInfo.eProtocol = e2l_login;
            loginInfo.account = "";
            loginInfo.password = "";
            ClientLogic::instance()->ProcessUIRequest(&loginInfo);
        }
            break;
            
        default:
            break;
    }
}

void LoginScene::failTip()
{
    MessageBox(SSTRING("password_account_error").c_str(), SSTRING("login_error_title").c_str());
}

void LoginScene::update(float dt)
{
    UIManager::instance()->loop(dt);
}