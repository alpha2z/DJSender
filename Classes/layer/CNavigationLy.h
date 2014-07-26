#ifndef __CNAVIGATIONLY_H__
#define __CNAVIGATIONLY_H__

#include "cocos2d.h"
#include <map>
USING_NS_CC;



class CButton;
class CNavigationLy :public CCLayer
{
public:
	static CNavigationLy * create(CCLabelTTF *);
	CNavigationLy(CCLabelTTF *);
	~CNavigationLy(void);
	bool init();
	bool config();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	float getButtonHeight();
	void updateButton(int id);
	int clickCallback(CButton *);

private:
	std::map<int, CButton*> m_mapButton;
	float m_fButtonHeight;
	bool m_bTouched;
	CCLabelTTF * m_pTitle;
};


typedef int (CNavigationLy::*SEL_ClickCallback)(CButton*);

#define button_selector(_SELECTOR) (SEL_ClickCallback)(&_SELECTOR)


#endif//__CNAVIGATIONLY_H__
