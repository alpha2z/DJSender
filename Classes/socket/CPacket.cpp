
#include "CPacket.h"
#include <malloc.h>
#include <string.h>

CPacket::CPacket(GINT32 fd, char * buff, GUINT32 len)
:m_nFD(fd) {
	m_pStart = (char *)::malloc(len);
	if (buff) {
		memcpy(m_pStart, buff, len);
	}
	m_uReal = 0;
	m_uLen = len;
	INIT_LIST_HEAD(&node);
}

CPacket::~CPacket() {
	::free(m_pStart);
}


char *CPacket::getBuff() {
	return m_pStart;
}

GINT32 CPacket::getLength() {
	return m_uReal;
}


GINT32 CPacket::getFD() {
	return m_nFD;
}

void CPacket::writeInt(GINT32 data, GINT32 type) {
	switch(type) {
		case _emInt8: {
			GINT8 x = (GINT8)data;
			writeBlock(&x, sizeof(x));
			break;
		}
		case _emInt16: {
			GINT16 x = (GINT16)data;
			writeBlock(&x, sizeof(x));
			break;
		}
		case _emInt32: {
			GINT32 x = (GINT32)data;
			writeBlock(&x, sizeof(x));
			break;
		}
		default:
			assert(false);
	}
}
void CPacket::writeUInt(GUINT32 data, GINT32 type) {
	switch(type) {
		case _emUInt8: {
			GUINT8 x = (GUINT8)data;
			writeBlock(&x, sizeof(x));
			break;
		}
		case _emUInt16: {
			GUINT16 x = (GUINT16)data;
			writeBlock(&x, sizeof(x));
			break;
		}
		case _emUInt32: {
			GUINT32 x = (GUINT32)data;
			writeBlock(&x, sizeof(x));
			break;
		}
		default:
			assert(false);
	}
}

void CPacket::writeString(std::string& data) {
	int len = data.length();
	writeUInt16(len);
	writeBlock((void *)data.c_str(), len);
}

bool CPacket::readInt(void *data, GINT32 type) {
	switch(type) {
		case _emInt8: {
			return readBlock(data, sizeof(GINT8));
		}
		case _emInt16: {
			return readBlock(data, sizeof(GINT16));
		}
		case _emInt32: {
			return readBlock(data, sizeof(GINT32));
		}
		default:
			return false;
	}
}

bool CPacket::readUint(void *data, GINT32 type) {
	switch(type) {
		case _emUInt8: {
			return readBlock(data, sizeof(GUINT8));
		}
		case _emUInt16: {
			return readBlock(data, sizeof(GUINT16));
		}
		case _emUInt32: {
			return readBlock(data, sizeof(GUINT32));
		}
		default:
			return false;
	}
}

bool CPacket::readString(std::string &data) {
	GUINT16 len = 0;
	if (!readUInt16(&len))
		return false;

	char *p = (char *)::malloc(len);
	if (!readBlock(p, len)) {
		::free(p);
		return false;
	}

	data.append(p, len);
	::free(p);
	return true;
}

void CPacket::writeBlock(void * data, GUINT32 len) {
	if (m_uReal + len > m_uLen) {
		expand(len);
	}
	memcpy(m_pStart + m_uReal, data, len);
	m_uReal += len;
}

void CPacket::expand(GUINT32 nMore) {
	GUINT32 len = (m_uReal + nMore) * 2;
	char* p = (char *)::malloc(len);
	memcpy(p, m_pStart, m_uReal);
	m_uLen = len;
	::free(m_pStart);
	m_pStart = p;
}

bool CPacket::readBlock(void *data, GUINT32 len) {
	if ((m_uReal + len) > m_uLen) {
		return false;
	}

	memcpy(data, m_pStart + m_uReal, len);
	m_uReal += len;
	return true;
}

