#include "Sender.h"

#include "platform\CCPlatformMacros.h"

using namespace std;

#define BREAK_IF(cond)  if (cond) break;



CSender::CSender(string strUrl, SendCallback pCallback)
:m_strUrl(strUrl),
m_pCallback(pCallback),
m_uTimeOutRead(1),
m_uTimeOutConn(1)
{
	m_pCurl = curl_easy_init();
	memset(m_chErrBuff, 0, BUFF_SIZE);

}


CSender::~CSender(void)
{
	curl_easy_cleanup(m_pCurl);

	
}

bool CSender::init() {
	if (!m_pCurl)
		return false;

	int code;
	code = curl_easy_setopt(m_pCurl, CURLOPT_ERRORBUFFER, m_chErrBuff);
	if (code != CURLE_OK)
		return false;

	code = curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, m_uTimeOutRead);
	if (code != CURLE_OK)
		return false;

	code = curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, m_uTimeOutConn);
	if (code != CURLE_OK)
		return false;

	code = curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);
	if (code != CURLE_OK)
		return false;

	return true;
}


CSender* CSender::create(string strUrl, SendCallback pCallback) {
	CSender *p = new CSender(strUrl, pCallback);
	if (p && p->init())
		return p;
	CC_SAFE_DELETE(p);
	return nullptr;
}

bool CSender::sendData(CRequest *pclRequest) {
	bool ret = false;
	
	std::vector<std::string> headers = pclRequest->getHeaders();
	if (!headers.empty()) {
		for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
			m_pHeaders = curl_slist_append(m_pHeaders, it->c_str());				
		}
		if (!setOpt(CURLOPT_HTTPHEADER, m_pHeaders))
			return false;
	}

	do {
		BREAK_IF(setOpt(CURLOPT_WRITEFUNCTION, pclRequest->getBodyCallback()));
		BREAK_IF(setOpt(CURLOPT_WRITEDATA, pclRequest->getWriteData()));
		BREAK_IF(setOpt(CURLOPT_HEADERFUNCTION, pclRequest->getHeaderCallback()));
		BREAK_IF(setOpt(CURLOPT_HEADERDATA, pclRequest->getHeaderData()));
		ret = true;
	} while(0);
	return true;
}


