

#ifndef __CUSERIF_H__
#define __CUSERIF_H__

#include "cocos2d.h"

class CUserIF
{
public:
	CUserIF(int nID, int nFD, int nType):m_nFD(nFD),m_nType(nType),m_nID(nID){}

	virtual ~CUserIF(){CCLOG("==============> deconstruct CUserIF ");}
	virtual int getType() const { return m_nType; };
	virtual int getFD() const { return m_nFD; };
	virtual int getID() const { return m_nID; };

protected:
	int m_nFD;
	int m_nType;
	int m_nID;

};

#endif//__CUSERIF_H__


