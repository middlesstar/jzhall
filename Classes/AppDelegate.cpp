/*-----------------说明----------------
* 引擎部分代码有改动，修改部分都应添加注释，关键字 “------------qfli修改-----------”
*/

#include "AppDelegate.h"
#include "HallGameScene.h"
#include "Window/WindowManager.h"
#include "Network/NetManager.h"
#include "Player/Player.h"
#include "LanguageConfig.h"
#include "GlobalTimer.h"
#include "GlobalTickObject.h"
#include "df/JZCore.h"
#include "MacroDefineCommon.h"
#define LOGIN_SERVER_IP_ID			999999		//登录服务器
#define LOGIN_SERVER_PORT_ID		999998
// 
// #define CHAT_SERVER_IP_ID			999997		//聊天服务器
// #define CHAT_SERVER_PORT_ID			999996


#define      game_comeToBack           "comeToBackground"
#define      game_comefromBack         "comeFromBackground"
USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1074, 740);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSrize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
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

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("jzhall", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("jzhall");
#endif
        director->setOpenGLView(glview);
    }

	printf("====================AppDelegate::applicationDidFinishLaunching===");
    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
    Size frameSize = glview->getFrameSize();

    register_all_packages();
	HallGameScene::m_gSearchPathVec = FileUtils::getInstance()->getSearchPaths();

	WindowManager::getInstance()->init();
	LanguageConfig::getInstance()->init("lang_zh-cn.xml");
	NetManager::getInstance()->setAddress(GETSTRING(LOGIN_SERVER_IP_ID).c_str(), atoi(GETSTRING(LOGIN_SERVER_PORT_ID).c_str()));		//外网服务器
	Player::getInstance()->requestVersionInfo();			//请求版本信息和ios屏蔽开关

	JZCore::getInstance()->initPlatform();

    // create a scene. it's an autorelease object
	auto scene = HallGameScene::createScene();

    // run
    director->runWithScene(scene);

	Director::getInstance()->getScheduler()->schedule(
		schedule_selector(GlobalTickObject::gameTick), GlobalTickObject::getInstance(), (float)(1.0 / 60.0), false
		);

	//开启全局定时器
	GlobalTimer::Getinstance()->StarPlay();


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	HALL_NOTIFICATION(game_comeToBack,nullptr);
#endif

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		HALL_NOTIFICATION(game_comefromBack,nullptr);
#endif 

	
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
