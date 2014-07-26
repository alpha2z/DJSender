#ifndef __CMAINSCENE_H__
#define __CMAINSCENE_H__

#include "cocos2d.h"

class CMainScene : public cocos2d::CCScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(CMainScene);

private:
	cocos2d::CCLabelTTF* m_pTitle;
};

#endif // __CMAINSCENE_H__
