#ifndef _PacketAide_H_
#define _PacketAide_H_

#include <string>
#include "Convert.h"
#include "df/types.h"
#include "df/Define.h"

/**
 * 数据包读取辅助
 */
class PacketAide
{
private:
	unsigned char* mData;
	mutable int mPos;
	bool m_bDataIsNew;			//标志mData是否new出来的
	int m_nDataNewLen;
public:
	/**
	 * @param data 网络数据
	 * @param index 起始下标
	 */
	PacketAide();
	PacketAide(unsigned char* data, int pos = 0);
	PacketAide(unsigned char* data, bool bDataNew, int nDataNewLen, int pos = 0);

	PacketAide(const PacketAide& packetData);

	~PacketAide();

	PacketAide& operator=(const PacketAide& packetData);

	void setPosition(int pos);
	int getPosition() const;
	unsigned char* getBuffer() const;
	unsigned int readByte();
	unsigned int read2Byte();
	unsigned int read4Byte();
	unsigned int* read4Bytes(unsigned int* ary, int count);
	long long read8Byte();
	float readFloat();

	void writeByte(unsigned int val);
	void write2Byte(unsigned int val);
	void write4Byte(unsigned int val);
	void write4Bytes(unsigned int* ary, int count);
	void write8Byte(long long val);
	void writeFloat(float val);

	//新增
	byte rbyte() const;
	word rword() const;
	char rchar() const;
	dword rdword() const;
	long rlong() const;
	SCORE rSCORE() const;
	const char* rchars(char* const pArray, int count) const;
	const char* rchars_for_single(char* const pArray, int count) const;				//以一个字节读入char

	void wbyte(byte val);
	void wword(word val);
	void wchar(char val);
	void wdword(dword val);
	void wlong(long val);
	void wSCORE(SCORE val);
	void wchars(char* const pArray, int count);
	void wchars_for_single(char* const pArray, int count);			//以一个字节写入char
	/**
	 * @param len (utf16的长度/2)
	 */
	void writeString(const std::string& utf8, int len);
	/**
	 * @param len (utf16的长度/2)
	 */
	std::string readString(int len) const;

	//水浒传
	void writeUTF16(const unsigned short* data, int len);
	void readUTF16(unsigned short* data, int len) const;
	u2string readUTF16(int len) const;
	void writeUTF16(const u2string& u2, int len);
};

#define PACKET_AIDE_SIZE(size) unsigned char __PacketBuffer__[size]={0}; PacketAide packet(__PacketBuffer__)
#define PACKET_AIDE_DATA(data) PacketAide packet((unsigned char*)data)

#define PACKET_AIDE_SIZE_NEW(size) unsigned char* pPacketBuffer = new unsigned char[size]; PacketAide packet(pPacketBuffer,true,size)

#endif // _PacketAide_H_
