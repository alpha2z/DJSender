#include "AppDelegate.h"
#include "CMainScene.h"
#include "CTCPMng.h"
#include "CConfig.h"
#include "CSceneMng.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CConfig *p = CConfig::sharedCConfig();
	
	CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	
    // create a scene. it's an autorelease object
    // CCScene *pScene = CMainScene::create();
    CCScene *pScene = CSceneMng::sharedCSceneMng()->getMainScene();

    // run
    pDirector->runWithScene(pScene);
	CTCPMng *pServer = new CTCPMng();
	pServer->contract("10.121.9.10", 8888);
	pServer->start();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
