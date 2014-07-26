#ifndef __CTCPMNG_H__
#define __CTCPMNG_H__

#define QUEUE_LENGTH 256

#include <string>

#include "pthread\pthread.h"
#include <WinSock2.h>

#include "list.h"

enum ServerType { 
	_emServer,
	_smClient
};

class CPacket;
class CTCPMng
{
public:
	CTCPMng();
	~CTCPMng();

	void start();
	bool contract(char* ip, int nPort);

	void swapReadQueue();
	void swapWriteQueue();

	bool pushPacket(CPacket *p);

	bool config();
	bool initServer(int nPort);

public:
	int m_nListenFD;
	fd_set m_fdset;
	struct list_head m_stReadBuff;	// sub thread logic
	struct list_head m_stWriteBuff;	// sub thread logic

private:

	pthread_t m_acc;
	pthread_t m_read;
	pthread_t m_send;

	struct list_head m_stReadList; // main thread logic
	struct list_head m_stWriteList; // main thread logic

	pthread_mutex_t m_RLock;
	pthread_mutex_t m_WLock;

	bool m_bInitFlag;
	bool m_bConfigFlag;


};







#endif//__CTCPMNG_H__


