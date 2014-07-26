#include "CConfig.h"
#include "cocos2d.h"



static CConfig * g_pInstance = nullptr;

CConfig::CConfig()
:m_pLuaState(nullptr) {

}

CConfig::~CConfig() {
	lua_close(m_pLuaState);
}

CConfig * CConfig::sharedCConfig() {
	if (g_pInstance)
		return g_pInstance;

	g_pInstance = new CConfig;
	if (g_pInstance && g_pInstance->init()) {
		return g_pInstance;
	}
	CC_SAFE_DELETE(g_pInstance);
	return nullptr;
}


bool CConfig::init() {
	bool ret = false;
	do {
		m_pLuaState = luaL_newstate();
		assert(m_pLuaState);


		ret = true;
	} while (0);
	return ret;
}

lua_State * CConfig::getState() {
	assert(m_pLuaState);
	return m_pLuaState;
}

