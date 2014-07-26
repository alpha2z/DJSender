

#ifndef __CCLIENT_H__
#define __CCLIENT_H__

#include <map>
#include <vector>

#include "CUserIF.h"
#include "CMsgHelper.h"

class CClient : public CMsgHelper, CUserIF
{
public:
	CClient(int nID, int nFD, int nType);
	~CClient();

	void sendPicture();
	void sendMessage(std::string msg);
	void sendFile();

	bool openLocalFiles();

	void addClient(CUserIF* pUser);

private:
	std::map<int, std::vector<CUserIF *>> m_mapClients;


};

#endif//__CCLIENT_H__


