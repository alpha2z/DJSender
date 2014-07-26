#ifndef __CCONFIG_H__
#define __CCONFIG_H__

#include "lua.hpp"

class CConfig
{
public:
	CConfig();
	~CConfig();
	static CConfig * sharedCConfig();
	lua_State * getState();
	bool init();

private:
	lua_State *m_pLuaState;

};


#endif//__CCONFIG_H__