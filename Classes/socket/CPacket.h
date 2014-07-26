#ifndef __CPACKET_H__
#define __CPACKET_H__


#include <string>
#include <assert.h>
#include "list.h"


typedef unsigned char GUINT8;
typedef unsigned short GUINT16;
typedef unsigned int GUINT32;

typedef char GINT8;
typedef short GINT16;
typedef int GINT32;

enum _emUIntType {
	_emUInt8,
	_emUInt16,
	_emUInt32,
};

enum _emIntType {
	_emInt8,
	_emInt16,
	_emInt32,
};


#define writeUInt8(data) writeUInt(data, _emUInt8)
#define writeUInt16(data) writeUInt(data, _emUInt16)
#define writeUInt32(data) writeUInt(data, _emUInt32)

#define writeInt8(data) writeUInt(data, _emInt8)
#define writeInt16(data) writeUInt(data, _emInt16)
#define writeInt32(data) writeUInt(data, _emInt32)

#define readUInt8(data) readUint(data, _emUInt8)
#define readUInt16(data) readUint(data, _emUInt16)
#define readUInt32(data) readUint(data, _emUInt32)

#define readInt8(data) readInt(data, _emUInt8)
#define readInt16(data) readInt(data, _emUInt16)
#define readInt32(data) readInt(data, _emUInt32)

#define DEFAULT_LENGTH 512

class CPacket
{
public:
	CPacket(GINT32 fd, char *buff = NULL, GUINT32 len = DEFAULT_LENGTH);
	~CPacket();

	char *getBuff();
	GINT32 getLength();
	GINT32 getFD();
	list_head node;

	void writeInt(GINT32 data, GINT32 type);
	void writeUInt(GUINT32 data, GINT32 type);
	void writeString(std::string &data);

	bool readInt(void *data, GINT32 type);
	bool readUint(void *data, GINT32 type);
	bool readString(std::string &data);

private:
	void expand(GUINT32 nMore);
	void writeBlock(void * data, GUINT32 len);
	bool readBlock(void *data, GUINT32 len);


	GINT32 m_nFD;
	char* m_pStart;
	GUINT32 m_uLen;
	GUINT32 m_uReal;


};

#endif//__CPACKET_H__

