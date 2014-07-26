#ifndef __CMSGHELPER_H__
#define __CMSGHELPER_H__

#include "cocos2d.h"

class CPacket;
class CMsgHelper
{
public:
	CMsgHelper();
	virtual ~CMsgHelper();

	bool sendPacket(int fd, CPacket *p);


};


#endif//__CMSGHELPER_H__


