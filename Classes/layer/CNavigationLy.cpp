#include "CNavigationLy.h"
#include "CConfig.h"
#include "CButton.h"
#include "CSceneMng.h"
#include "CMainScene.h"


const char* localfile = "prop/navigator.lua";
CNavigationLy::CNavigationLy(CCLabelTTF * pLabel)
:m_fButtonHeight(0.0f),
m_pTitle(pLabel)
{

}


CNavigationLy::~CNavigationLy(void)
{

}

CNavigationLy *CNavigationLy::create(CCLabelTTF *pLabel) {
	CNavigationLy *p = new CNavigationLy(pLabel);
	if (p && p->init()) {
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

bool CNavigationLy::init() {
	bool ret = false;
	do {
		lua_State *L = CConfig::sharedCConfig()->getState();
		int top = lua_gettop(L);
		luaL_dofile(L, localfile);
		if (!lua_istable(L, -1)) {
			const char* err = lua_tostring(L, -1);
			CCLOG("err ===>: %s", err);
			assert(false);
		}
		lua_pushstring(L, "btn");		   // stack: config, btn
		lua_rawget(L, -2);                 // stack: config, tbl
				  
		int id, tempTop;
		lua_pushnil(L);					  // stack: config, tbl, nil
		while(lua_next(L, -2)) {
			id = luaL_checknumber(L, -2);
			luaL_checktype(L, -1, LUA_TTABLE);
			tempTop = lua_gettop(L);
			CButton *pBtn = new CButton(this, button_selector(CNavigationLy::clickCallback), L, id);
			lua_settop(L, tempTop);
			assert(pBtn);
			assert(m_mapButton.end() == m_mapButton.find(id));
			m_mapButton[id] = pBtn;

			lua_pop(L, 1);
		}
		CC_BREAK_IF(!config());
			
		ret = true;
		lua_settop(L, top);
	} while (0);

	return ret;
}

bool CNavigationLy::config() {
	if (m_mapButton.size() <= 0)
		return false;

	float h;
	CButton * pFirst = nullptr;
	for (auto it = m_mapButton.begin(); it != m_mapButton.end(); it++) {
		CButton *b = it->second;
		if (!pFirst)
			pFirst = b;

		CCPoint pos = b->getXY();
		this->addChild(b, 0);
		b->setPosition(pos);
		h =  b->getTexHeight();
		m_fButtonHeight = m_fButtonHeight > h ? m_fButtonHeight : h;		
	}
	updateButton(pFirst->getID());

	this->setTouchMode(kCCTouchesOneByOne);
	this->setTouchPriority(1);
	this->setTouchEnabled(true);
	return true;
}

int CNavigationLy::clickCallback(CButton *pBtn) {
	CCLOG("clickCallback :%d", pBtn->getID());
	m_pTitle->setString(pBtn->getTitle());
	return pBtn->getID();
}


float CNavigationLy::getButtonHeight() {
	return m_fButtonHeight;
}

void CNavigationLy::updateButton(int id) {
	CButton* pBtn = nullptr;
	for (auto it = m_mapButton.begin(); it != m_mapButton.end(); it++) {
		pBtn = it->second;
		if (pBtn->getID() == id) {
			pBtn->onChoosen(true);
		} else {
			pBtn->onChoosen(false);
		}
	}
}

bool CNavigationLy::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	CCPoint touchPos = pTouch->getLocation();
	CCPoint p;
	CButton* pBtn = nullptr;
	m_bTouched = false;
	int id = -1;
	for (auto it = m_mapButton.begin(); it != m_mapButton.end(); it++) {
		pBtn = it->second;
		p = pBtn->getPosition();
		p = convertToWorldSpace(p);
		
		CCSize& sz = pBtn->getButtonSize();
		if ((touchPos.x - (p.x - sz.width)) * (touchPos.x - (p.x + sz.width)) < 0 
			&& (touchPos.y - (p.y - sz.height) * (touchPos.y - (p.y + sz.height)) < 0)){
			m_bTouched = true;
			id = pBtn->getID();
		}
	}
	if (m_bTouched) {
		updateButton(id);
	}
	return m_bTouched;
}

void CNavigationLy::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
	
}

void CNavigationLy::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
	m_bTouched = false;
}

void CNavigationLy::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
	m_bTouched = false;
}
