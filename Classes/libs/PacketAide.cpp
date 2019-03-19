#include "PacketAide.h"
#include "cocos2d.h"
USING_NS_CC;

PacketAide::PacketAide()
	: mData(nullptr)
	, mPos(0)
	, m_bDataIsNew(false)
	, m_nDataNewLen(0)
{

}

PacketAide::PacketAide(unsigned char* data, int pos)
	: mData(data)
	, mPos(pos)
	, m_bDataIsNew(false)
	, m_nDataNewLen(0)
{}

PacketAide::PacketAide(unsigned char* data, bool bDataNew, int nDataNewLen, int pos/* = 0*/)
	: mData(data)
	, mPos(pos)
	, m_bDataIsNew(bDataNew)
	, m_nDataNewLen(nDataNewLen)
{

}

PacketAide::PacketAide(const PacketAide& packetData)
{
	mPos = packetData.mPos;
	m_bDataIsNew = packetData.m_bDataIsNew;
	m_nDataNewLen = packetData.m_nDataNewLen;

	if (m_bDataIsNew)
	{
		mData = new unsigned char[m_nDataNewLen];

		//log("------------PacketAide[new %d]-------------", m_nDataNewLen);

		memcpy(mData, packetData.mData, m_nDataNewLen);
	}
	else
	{
		mData = packetData.mData;
	}
}

PacketAide::~PacketAide()
{
	if (m_bDataIsNew && m_nDataNewLen > 0)
	{
		delete [] mData;
		//log("------------PacketAide[delete %d]-------------", m_nDataNewLen);
		mData = nullptr;
	}
}

PacketAide& PacketAide::operator=(const PacketAide& packetData)
{
	mPos = packetData.mPos;
	m_bDataIsNew = packetData.m_bDataIsNew;
	m_nDataNewLen = packetData.m_nDataNewLen;

	if (m_bDataIsNew)
	{
		mData = new unsigned char[m_nDataNewLen];

		//log("------------PacketAide[new %d]-------------", m_nDataNewLen);

		memcpy(mData, packetData.mData, m_nDataNewLen);
	}
	else
	{
		mData = packetData.mData;
	}

	return *this;
}

void PacketAide::setPosition(int pos)
{
	mPos = pos;
}

int PacketAide::getPosition() const
{
	return mPos;
}

unsigned char* PacketAide::getBuffer() const
{
	return mData;
}

unsigned int PacketAide::readByte()
{
	unsigned int ret = mData[mPos+0];
	mPos += 1;
	return ret;
}

unsigned int PacketAide::read2Byte()
{
	unsigned int ret = ((unsigned int)((mData[mPos+1]&0xff)<<8) | (mData[mPos+0]&0xff));
	mPos +=2;
	return ret;
}

unsigned int PacketAide::read4Byte()
{
	unsigned int ret = (
		((unsigned int)(mData[mPos+3]&0xff)<<24) | 
		((unsigned int)(mData[mPos+2]&0xff)<<16) |
		((unsigned int)(mData[mPos+1]&0xff)<< 8) |
		((unsigned int)(mData[mPos+0]&0xff)));
	//ipone 会报错 ARM EXC_ARM_DA_ALIGN
	//unsigned int ret = *(unsigned int*)&mData[mPos];
	mPos +=4;
	return ret;
}

unsigned int* PacketAide::read4Bytes(unsigned int* ary, int count)
{
	int size = count*sizeof(unsigned int);
	memcpy(ary, &mData[mPos], size);
	mPos+=size;
	return ary;
}

float PacketAide::readFloat()
{
	float ret;
	memcpy(&ret, &mData[mPos], 4);
	mPos+=4;
	return ret;
}
	
long long PacketAide::read8Byte()
{
	long long ret = (
		((long long)(mData[mPos+7]&0xff)<<56) | 
		((long long)(mData[mPos+6]&0xff)<<48) | 
		((long long)(mData[mPos+5]&0xff)<<40) | 
		((long long)(mData[mPos+4]&0xff)<<32) | 
		((long long)(mData[mPos+3]&0xff)<<24) | 
		((long long)(mData[mPos+2]&0xff)<<16) |
		((long long)(mData[mPos+1]&0xff)<< 8) |
		((long long)(mData[mPos+0]&0xff)));

	mPos +=8;
	return ret;
}

void PacketAide::writeByte(unsigned int val)
{
	mData[mPos+0] = (unsigned char)(val&0xff);
	mPos +=1;
}

void PacketAide::write2Byte(unsigned int val)
{
	mData[mPos+1] = (unsigned char)((val>>8)&0xff);
	mData[mPos+0] = (unsigned char)((val)&0xff);
	mPos +=2;
}

void PacketAide::write4Byte(unsigned int val)
{
	mData[mPos+3] = (unsigned char)((val>>24) & 0xff);
	mData[mPos+2] = (unsigned char)((val>>16) & 0xff);
	mData[mPos+1] = (unsigned char)((val>> 8) & 0xff);
	mData[mPos+0] = (unsigned char)((val)     & 0xff);
	//ipone 会报错 ARM EXC_ARM_DA_ALIGN
	//unsigned int* pVal = (unsigned int*)&mData[mPos];
	//*pVal = val;
	mPos +=4;
}

void PacketAide::write4Bytes(unsigned int* ary, int count)
{
	int size = count*sizeof(unsigned int);
	memcpy(&mData[mPos], ary, size);
	mPos+=size;
}

void PacketAide::writeFloat(float val)
{
	memcpy(&mData[mPos], &val, 4);
	mPos += 4;
}

byte PacketAide::rbyte() const
{
	byte ret = mData[mPos + 0];
	mPos += 1;
	return ret;
}

word PacketAide::rword() const
{
	word ret = ((word)((mData[mPos + 1] & 0xff) << 8) | (mData[mPos + 0] & 0xff));
	mPos += 2;
	return ret;
}

char PacketAide::rchar() const
{
	char ret;
	memcpy(&ret, &mData[mPos], sizeof(char));
	mPos += sizeof(char);

	return ret;
}

dword PacketAide::rdword() const
{
	dword ret = (
		((unsigned int)(mData[mPos + 3] & 0xff) << 24) |
		((unsigned int)(mData[mPos + 2] & 0xff) << 16) |
		((unsigned int)(mData[mPos + 1] & 0xff) << 8) |
		((unsigned int)(mData[mPos + 0] & 0xff)));
	//ipone 会报错 ARM EXC_ARM_DA_ALIGN
	//unsigned int ret = *(unsigned int*)&mData[mPos];
	mPos += 4;
	return ret;
}

long PacketAide::rlong() const
{
	long ret = (
		((long)(mData[mPos + 3] & 0xff) << 24) |
		((long)(mData[mPos + 2] & 0xff) << 16) |
		((long)(mData[mPos + 1] & 0xff) << 8) |
		((long)(mData[mPos + 0] & 0xff)));
	mPos += 4;
	return ret;
}

SCORE PacketAide::rSCORE() const
{
	SCORE ret = (
		((long long)(mData[mPos + 7] & 0xff) << 56) |
		((long long)(mData[mPos + 6] & 0xff) << 48) |
		((long long)(mData[mPos + 5] & 0xff) << 40) |
		((long long)(mData[mPos + 4] & 0xff) << 32) |
		((long long)(mData[mPos + 3] & 0xff) << 24) |
		((long long)(mData[mPos + 2] & 0xff) << 16) |
		((long long)(mData[mPos + 1] & 0xff) << 8) |
		((long long)(mData[mPos + 0] & 0xff)));

	mPos += 8;
	return ret;
}

const char* PacketAide::rchars(char* const pArray, int count) const
{
	u2string str;
	str.resize(count + 1, '\0');
	readUTF16(&str[0], count);
	//int nLen = countarray(pArray);
	strncpy(pArray, u2_8(&str[0]), count);

	//memcpy(pArray, readString(count).c_str(), count * 2);

	return pArray;
}

const char* PacketAide::rchars_for_single(char* const pArray, int count) const
{
	memcpy(pArray, &mData[mPos], count);
	mPos += count;

	return pArray;
}


void PacketAide::wbyte(byte val)
{
	mData[mPos + 0] = val;
	mPos += 1;
}

void PacketAide::wword(word val)
{
	mData[mPos + 1] = (unsigned char)((val >> 8) & 0xff);
	mData[mPos + 0] = (unsigned char)((val)& 0xff);
	mPos += 2;
}

void PacketAide::wchar(char val)
{
	memcpy(&mData[mPos], &val, sizeof(char));
	mPos += 2;
}

void PacketAide::wdword(dword val)
{
	write4Byte(val);
}

void PacketAide::wlong(long val)
{
	write4Byte(val);
}

void PacketAide::wSCORE(SCORE val)
{
	write8Byte(val);
}

void PacketAide::wchars(char* const pArray, int count)
{
	//writeString(pArray, count);
	writeUTF16(u8_2(pArray), count);
}

void PacketAide::wchars_for_single(char* const pArray, int count)
{
	memcpy(&mData[mPos], pArray, count);
	mPos += count;
}

void PacketAide::write8Byte(long long val)
{
	mData[mPos+7] = (unsigned char)((val>>56) & 0xff);
	mData[mPos+6] = (unsigned char)((val>>48) & 0xff);
	mData[mPos+5] = (unsigned char)((val>>40) & 0xff);
	mData[mPos+4] = (unsigned char)((val>>32) & 0xff);
	mData[mPos+3] = (unsigned char)((val>>24) & 0xff);
	mData[mPos+2] = (unsigned char)((val>>16) & 0xff);
	mData[mPos+1] = (unsigned char)((val>> 8) & 0xff);
	mData[mPos+0] = (unsigned char)( val      & 0xff);
	mPos +=8;
}

void PacketAide::writeString(const std::string& utf8, int len)
{
	std::string ucs2 = u2l(utf8);
	ucs2.resize(len*2, '\0');
	memcpy(&mData[mPos], &ucs2[0], ucs2.size());
	mPos += ucs2.size();
}

std::string PacketAide::readString(int len) const
{
	std::string ret;
	ret.resize(len*2+2, '\0');
	memcpy(&ret[0], &mData[mPos], len*2);
	mPos += len * 2;
	return l2u(ret);
}

void PacketAide::writeUTF16(const unsigned short* data, int len)
{
	int ucs2len = ucs2_len(data);
	if (ucs2len > len)ucs2len = len;

	for (int i = 0; i < ucs2len; ++i)
	{
		mData[mPos + i * 2 + 1] = (data[i] >> 8) & 0xff;
		mData[mPos + i * 2 + 0] = (data[i]) & 0xff;
	}

	for (int i = ucs2len; i < len; ++i)
	{
		mData[mPos + i * 2 + 1] = '\0';
		mData[mPos + i * 2 + 0] = '\0';
	}

	mPos += len * 2;
}

void PacketAide::readUTF16(unsigned short* data, int len) const
{
	for (int i = 0; i < len; ++i)
	{
		data[i] = (mData[mPos + i * 2 + 1] << 8) | mData[mPos + i * 2 + 0];
	}

	mPos += len * 2;
}

u2string PacketAide::readUTF16(int len) const
{
	static u2string ret;
	ret.resize(len + 1, '\0');
	for (int i = 0; i < len; ++i)
	{
		ret[i] = (mData[mPos + i * 2 + 1] << 8) | mData[mPos + i * 2 + 0];
	}

	mPos += len * 2;

	return ret;
}

void PacketAide::writeUTF16(const u2string& u2, int len)
{
	const unsigned short* data = &u2[0];
	int ucs2len = ucs2_len(data);
	if (ucs2len > len)ucs2len = len;

	for (int i = 0; i < ucs2len; ++i)
	{
		mData[mPos + i * 2 + 1] = (data[i] >> 8) & 0xff;
		mData[mPos + i * 2 + 0] = (data[i]) & 0xff;
	}

	for (int i = ucs2len; i < len; ++i)
	{
		mData[mPos + i * 2 + 1] = '\0';
		mData[mPos + i * 2 + 0] = '\0';
	}

	mPos += len * 2;
}