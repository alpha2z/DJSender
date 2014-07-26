

#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include "CNavigationLy.h"
#include "cocos2d.h"
USING_NS_CC;


struct lua_State;
class CButton : public CCNode
{
public:

	CButton(CNavigationLy *, SEL_ClickCallback, lua_State*, int);
	virtual ~CButton();
	int getID() const;
	float getX() const;
	float getY() const;
	CCPoint getXY();
	float getTexHeight() const;
	CCSize & getButtonSize();
	bool switchStatus(bool bSelect);
	void onChoosen(bool bChoosen);

	const char *getTitle();

private:
	int m_nID;
	float m_fX;
	float m_fY;
	std::string m_strUp;
	std::string m_strDown;
	std::string m_strTitle;

	CCSprite * m_pUnSelected;
	CCSprite * m_pSelected;

	CCSize m_szButtonSize;
	bool m_bSelect;

	CNavigationLy *m_pObj;
	SEL_ClickCallback m_Callback;

};

#endif//__CBUTTON_H__


