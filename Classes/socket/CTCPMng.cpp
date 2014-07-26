#include "CTCPMng.h"
#include <string.h>
#include <WinSock2.h>
#include "CPacket.h"
#include <assert.h>
#include <pthread/pthread.h>

#pragma comment(lib, "WS2_32.lib")

#define BLOCK_LISTEN 32

static int max_fd = 0;
static int max_rlength = 10240;
static bool KEEP_ALIVE = true;

CTCPMng::CTCPMng()
:m_bInitFlag(false),
m_bConfigFlag(false),
m_nListenFD(-1) {
	INIT_LIST_HEAD(&m_stReadList);
	INIT_LIST_HEAD(&m_stReadBuff);
	INIT_LIST_HEAD(&m_stWriteList);
	INIT_LIST_HEAD(&m_stWriteBuff);
	pthread_mutex_init(&m_RLock, nullptr);
	pthread_mutex_init(&m_WLock, nullptr);
	FD_ZERO(&m_fdset);


}

CTCPMng::~CTCPMng() {
	pthread_mutex_destroy(&m_RLock);
	pthread_mutex_destroy(&m_WLock);

}

bool CTCPMng::config() {
	if (m_bConfigFlag)
		return true;

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 1, 1 );
	int code;
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		code = WSAGetLastError();
		return false;
	}
	m_bConfigFlag = true;
	return true;
}

bool CTCPMng::contract(char* ip, int nPort) { 
	bool ret = false;
	int nErrCode;

	do {
		if (!config()) {
			break;
		}

		SOCKET client_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (client_fd == INVALID_SOCKET) {
			nErrCode = WSAGetLastError();
			break;
		}

		int reuse = 1;
		if (setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1) {
			nErrCode = WSAGetLastError();
			break;
		}

		struct sockaddr_in cl_addr;
		memset(&cl_addr, 0, sizeof(struct sockaddr_in));
		cl_addr.sin_family = AF_INET;
		cl_addr.sin_port = htons(nPort);
		cl_addr.sin_addr.s_addr = inet_addr(ip);

		if (connect(client_fd, (struct sockaddr *)&cl_addr, sizeof(struct sockaddr_in)) == -1) {
			nErrCode = WSAGetLastError();
			break;
		}

		FD_SET(client_fd, &m_fdset);
		m_bInitFlag = true;
		ret = true;
	} while (0);

	return ret;
}

bool CTCPMng::initServer(int nPort) {
	bool ret = false;
	int nErrCode;
	SOCKET listen_fd;
	do {

		if (!config())
			break;

		listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_fd == INVALID_SOCKET) {
			nErrCode = WSAGetLastError();
			break;
		}

		int reuse = 1;
		if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1) {
			nErrCode = WSAGetLastError();
			break;
		}

		struct sockaddr_in my_addr;
		memset(&my_addr, 0, sizeof(struct sockaddr_in));
		my_addr.sin_family = AF_INET;
		my_addr.sin_port = htons(nPort);
		my_addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(listen_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
			nErrCode = WSAGetLastError();
			break;
		}

		if (listen(listen_fd, BLOCK_LISTEN) < 0) {
			nErrCode = WSAGetLastError();
			break;
		}

		m_bInitFlag = true;
		ret = true;
	} while (0);

	if (ret) {
		m_nListenFD = listen_fd;
	} else {
		if (listen_fd > 0)
			closesocket(listen_fd);
	}
	return ret;
}


void * accept_coming(void * arg) {
	CTCPMng *pMng = (CTCPMng*)arg;
	struct sockaddr s;
	int len = sizeof(struct sockaddr);
	int client_fd = 0;
	while(KEEP_ALIVE) {
		client_fd = accept(pMng->m_nListenFD, &s, &len);
		if (client_fd < 0) {
			exit(-1);
		}
		FD_SET(client_fd, &pMng->m_fdset);
		max_fd = max_fd > client_fd + 1 ? max_fd : client_fd + 1;

	}
	return nullptr;
}
#include "cocos2d.h"
void * read_data(void * arg) {
	CTCPMng *pMng = (CTCPMng*)arg;
	struct timeval timeout;
	timeout.tv_sec = 60;
	timeout.tv_usec = 0;
	char *buff = (char *)malloc(max_rlength);
	int len = 0;
	while(KEEP_ALIVE) {
		pMng->swapReadQueue();

		int fd = select(max_fd, &pMng->m_fdset, (fd_set *)nullptr, (fd_set *)nullptr, &timeout);
		memset(buff, 0, max_rlength);		
		
		if (FD_ISSET(fd, &pMng->m_fdset)) {
			CCLOG("=======================> fd:%d", fd);
			char header[2] = {0};
			int r = recv(fd, header, 2, 0);
			if (r == 2) {
				len = (header[0] << 1) | header[1];
				if (len > max_rlength) {
					free(buff);
					max_rlength = len * 2;
					buff = (char *)malloc(max_rlength);
					assert(buff, "buffer null");
				}
			} else { // error
				continue;
			}

			r = recv(fd, buff, len, MSG_WAITALL);
			if (r == len) {
				CPacket *p = new CPacket(fd, buff, len);
				list_add_tail(&p->node, &pMng->m_stReadBuff);
			} else { // error
				continue;
			}
		}
	}
	return nullptr;
}

void *send_data(void *arg) {
	CTCPMng *pMng = (CTCPMng*)arg;

	while(KEEP_ALIVE) {
		pMng->swapWriteQueue();
		struct list_head* pos;
		list_for_each(pos, &pMng->m_stWriteBuff) {
			CPacket * p = list_entry(pos, CPacket, node);
			int r = send(p->getFD(), p->getBuff(), p->getLength(), 0);
			if (r < p->getLength()) {
				fprintf(stderr, "send data error");
			}
			CC_SAFE_DELETE(p);
		}
		Sleep(2);
	}
	return nullptr;
}

void CTCPMng::start() {
	assert(m_bInitFlag, "init first");

	if (m_nListenFD < 0)
		return;

	if (pthread_create(&m_acc, nullptr, accept_coming, this)) {
		assert(false);
	}

	if (pthread_create(&m_read, nullptr, read_data, this)) {
		assert(false);
	}

	if (pthread_create(&m_send, nullptr, send_data, this)) {
		assert(false);
	}


}

bool CTCPMng::pushPacket(CPacket *p) {
	list_add_tail(&m_stWriteList, &p->node);
	return true;
}


void CTCPMng::swapReadQueue() {
	pthread_mutex_lock(&m_RLock);
	list_splice(&m_stReadBuff, &m_stReadList);
	pthread_mutex_unlock(&m_RLock);
	INIT_LIST_HEAD(&m_stReadBuff);
}

void CTCPMng::swapWriteQueue() {
	if (list_empty(&m_stWriteList))
		return;

	pthread_mutex_lock(&m_WLock);
	list_splice(&m_stWriteList, &m_stWriteBuff);
	INIT_LIST_HEAD(&m_stWriteList);
	pthread_mutex_unlock(&m_WLock);

}




