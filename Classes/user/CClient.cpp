

#include "CClient.h"

CClient::CClient(int nID, int nFD, int nType):CUserIF(nID,nFD,nType) {

}

CClient::~CClient() {
	
}

void CClient::addClient(CUserIF* pUser) {
	int nType = pUser->getType();
	auto iter = m_mapClients.find(nType);
	if (iter == m_mapClients.end()) {
		std::vector<CUserIF *> v;
		m_mapClients[nType] = v;
		m_mapClients[nType].push_back(pUser);
	} else {
		iter->second.push_back(pUser);
	}
}


void CClient::sendPicture() {

}

void CClient::sendMessage(std::string msg) {

}

void CClient::sendFile() {

}

bool CClient::openLocalFiles() {

	return false;
}

