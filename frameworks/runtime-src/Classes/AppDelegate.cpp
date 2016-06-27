#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "lua_module_register.h"
#include "utils/libs.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
#include "ide-support/CodeIDESupport.h"
#endif

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
#include "runtime/Runtime.h"
#include "ide-support/RuntimeLuaImpl.h"
#endif

#include "logic/ClientLogic.h"

//#include "view/scene/GoldDifSelScene.h"
#include "view/scene/GoldStageScene.h"
#include "GameDef.h"
#include "utils/GameUtils.h"
#include "logic/GameCore.h"
#include "sound/SoundManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "server/AndroidHelper.h"
#endif

//#include "external/xxtea/CryptXXTEA.h"

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

//static cocos2d::Size designResolutionSize = cocos2d::Size(1074, 740);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif

}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // set default FPS
    Director::getInstance()->setAnimationInterval(1.0 / 60.0f);

    // register lua module
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);

    register_all_packages();

    LuaStack* stack = engine->getLuaStack();
    stack->setXXTEAKeyAndSign(xxteaKeyStr.c_str(), strlen(xxteaKeyStr.c_str()), xxteaSignStr.c_str(), strlen(xxteaSignStr.c_str()));

    std::string uuStr("pojiedesiquanjia");
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    uuStr += iOSHelper::uuidStr;
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    uuStr += AndroidHelper::instance()->getDeviceID().c_str();
    #endif
    coAesSetKey(uuStr.substr(0,32).c_str());
    CryptXXTEA::getInstance()->setXXTEAKeyAndSign(xxteaKeyStr.c_str(), strlen(xxteaKeyStr.c_str()), xxteaSignStr.c_str(), strlen(xxteaSignStr.c_str()));
    //register custom function
    //LuaStack* stack = engine->getLuaStack();
    //register_custom_function(stack->getLuaState());

//#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
//    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
//    auto runtimeEngine = RuntimeEngine::getInstance();
//    runtimeEngine->addRuntime(RuntimeLuaImpl::create(), kRuntimeEngineLua);
//    runtimeEngine->start();
//#else
//    if (engine->executeScriptFile("src/main.lua"))
//    {
//        return false;
//    }
//#endif
    auto glView = Director::getInstance()->getOpenGLView();
    glView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    
    Director::getInstance()->setDisplayStats(true);
    GameUtils::winSize = Director::getInstance()->getWinSize();
    
    SoundManager::shared();
    //从登陆状态开启游戏
//    ClientLogic::instance()->ChangeState(GAME_STATE_GOLDEN_DIFF);
    ClientLogic::instance()->ChangeState(GAME_STATE_LOGIN);
    
//    auto scene = GoldStageScene::create();
//    Director::getInstance()->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    GameCore::instance()->clearDrop();
    GameCore::instance()->save2Json();
    Director::getInstance()->stopAnimation();

    SoundManager::shared()->pause();
//    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SoundManager::shared()->resume();
//    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
