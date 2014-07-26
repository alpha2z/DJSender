

#include "CButton.h"
#include "textures\CCTexture2D.h"
#include "platform\CCImage.h"

#include "lua.hpp"

CButton::CButton(CNavigationLy * obj, SEL_ClickCallback call, lua_State *L, int id)
:m_nID(id),
m_pObj(obj),
m_Callback(call)
{
	assert(lua_istable(L, -1));

	lua_pushstring(L, "x");
	lua_rawget(L, -2);
	m_fX = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "y");
	lua_rawget(L, -2);
	m_fY = lua_tonumber(L, -1);
	lua_pop(L, 1);

	size_t len = 0;
	
	m_strUp = "";
	lua_pushstring(L, "up");
	lua_rawget(L, -2);
	const char* p = lua_tolstring(L, -1, &len);
	m_strUp.append(p, len);
	lua_pop(L, 1);

	m_strDown = "";
	lua_pushstring(L, "down");
	lua_rawget(L, -2);
	p = lua_tolstring(L, -1, &len);
	m_strDown.append(p, len);
	lua_pop(L, 1);

	m_strTitle = "";
	lua_pushstring(L, "title");
	lua_rawget(L, -2);
	p = lua_tolstring(L, -1, &len);
	m_strTitle.append(p, len);
	lua_pop(L, 1);

	CCTexture2D * pTexture = CCTextureCache::sharedTextureCache()->addImage(m_strUp.c_str());
	m_pSelected = CCSprite::createWithTexture(pTexture);
	
	pTexture = CCTextureCache::sharedTextureCache()->addImage(m_strDown.c_str());
	m_pUnSelected = CCSprite::createWithTexture(pTexture);
	this->addChild(m_pUnSelected);
	this->addChild(m_pSelected);
	m_szButtonSize = m_pSelected->getContentSize();
	m_bSelect = false;
	m_pSelected->setVisible(m_bSelect);
	m_pUnSelected->setVisible(!m_bSelect);
}
	
CButton::~CButton() {

}

int CButton::getID() const {
	return m_nID;
}

float CButton::getX() const {
	return m_fX;
}

float CButton::getY() const {
	return m_fY;
}

CCPoint CButton::getXY() {
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float x = m_fX * visibleSize.width;
	float y = m_fY * this->getContentSize().height;
	return ccp(x, y);
}

float CButton::getTexHeight() const {
	return this->m_pSelected->getContentSize().height;
}

CCSize & CButton::getButtonSize() {
	return m_szButtonSize;
}

bool CButton::switchStatus(bool bSelect) {
	if (m_bSelect == bSelect)
		return false;

	m_bSelect = bSelect;
	m_pSelected->setVisible(m_bSelect);
	m_pUnSelected->setVisible(!m_bSelect);
	return true;
}

void CButton::onChoosen(bool bChoosen) {
	bool bChange = switchStatus(bChoosen);
	if (bChange && bChoosen) {
		(m_pObj->*m_Callback)(this);
	}

}

const char* CButton::getTitle() {
	return m_strTitle.c_str();
}
