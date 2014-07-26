#ifndef __CSENDER_H__
#define __CSENDER_H__

#include <string>
#include "curl\curl.h"


typedef void (*SendCallback)(int percent, int total);
typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);


#define BUFF_SIZE 256

class CSender
{
public:
	CSender(std::string strUrl, SendCallback pCallback);
	
	~CSender(void);
	static CSender* create(string strUrl, SendCallback pCallback);
	bool init();
	bool sendData(CRequest *pclRequest);
	
	template<typename T>
	bool setOpt(CURLoption opt, T val) {
		return  CURLE_OK == curl_easy_setopt(m_pCurl, opt, val);
	}

private:
	std::string m_strUrl;
	SendCallback m_pCallback;
	CURL * m_pCurl;
	char m_chErrBuff[BUFF_SIZE];
	UINT32 m_uTimeOutRead;
	UINT32 m_uTimeOutConn;

};


#endif  __CSENDER_H__

