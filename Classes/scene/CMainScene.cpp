#include "CMainScene.h"
#include "CNavigationLy.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool CMainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(CMainScene::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + pCloseItem->getContentSize().width/2 ,
		origin.y - pCloseItem->getContentSize().height/2 + visibleSize.height));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, nullptr);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    m_pTitle = CCLabelTTF::create("Hello XingYun", "Arial", 24);
    
    // position the label on the center of the screen
    m_pTitle->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - m_pTitle->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(m_pTitle, 1);

    // add "CMainScene" splash screen"
    CCSprite* pSprite = CCSprite::create("background.png");
	CCSize sz = pSprite->getContentSize();
    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(pSprite);

	CNavigationLy *pNavLayer = CNavigationLy::create(m_pTitle);
	pNavLayer->setPosition(ccp(visibleSize.width / 2, pNavLayer->getButtonHeight() / 2));
	this->addChild(pNavLayer);

    return true;
}


void CMainScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
