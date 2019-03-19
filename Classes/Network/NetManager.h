#ifndef __NetManager_H__
#define __NetManager_H__

/* ������� */
#include "cocos2d.h"
#include <queue>
#include "Socket/Socket.h"
#include "ISocketEngine.h"
#include "libs/PacketAide.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//typedef int		SOCKET;
#else
	typedef void*	LPVOID;
	typedef int		SOCKET;
	typedef int		HANDLE;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#define INVALID_HANDLE	INVALID_HANDLE_VALUE
	#define	SD_BOTH	0x02
#else
	#define	INVALID_HANDLE			(-1)
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include <windows.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <errno.h>
	#include <sys/syscall.h>
	#include <fcntl.h>
	#include <netinet/tcp.h>
#endif

USING_NS_CC;
using namespace std;

//���ӳ�ʱʱ��(��)
#define SOCK_CONNECT_TIMEOUT 6
//��������������С
#define SOCK_RECVBUFFERSIZE 1024 * 32

/******************************************************************************
��    ��: NetManager
��������: �ͻ����������һ����������Ӧһ��SOCKET
******************************************************************************/

struct sNetMessage
{
	int m_nMain;
	int m_nSub;
	PacketAide m_packetData;

	sNetMessage() {};
	~sNetMessage() {};
	sNetMessage(int nMain, int nSub, PacketAide packetData)
		: m_nMain(nMain)
		, m_nSub(nSub)
		, m_packetData(packetData)
	{

	}
};

class NetManager
{
public:
	///���캯������ʼ��Ĭ��ֵ
	NetManager();
	///�����������ͷ�������Դ
	virtual ~NetManager();

	static NetManager* getInstance()
	{
		static NetManager m_NetManager;
		return &m_NetManager;
	}

public:
	//�����ڷ��������ӵĵ�ַ��˿�
	void setAddress(const char* ip, unsigned short port);
	const string& getLoginServerIp();
	unsigned short getLoginServerPort();

	void sendHeartBeat();
	void onHeartBeatResp(const PacketAide& packet, int nLen);

	void sendMsg(int main, int sub, const PacketAide& packet);
	void unpack(byte* data, int start, int length);

	void onReconnectWithServer();
	void purgeMsgCache();

	void registerPakcetHandler(int nMain,int nSub, const std::function<void(const PacketAide& packet, int nLen)>& func);
	void registerPakcetHandler(uint uMesType, const std::function<void(const PacketAide& packet, int nLen)>& func);

	int compositionCommand(int main, int sub);
	void decompositionCommand(int Command, int* main, int* sub);
protected:
	const byte* _encrypt(int main, int sub, byte* data, int size);
	void _decrypt(byte* data, int start, int length);
public:
	///��ʼ��������ʽ����
	virtual bool connect();
	///�Ƿ�������״̬
	virtual bool isConnected();
	///�ر�����
	virtual void close();
	///֡ѭ������д����
	virtual void drive();
	///�������д���ݣ�������
	virtual void write(const char* buff, int length);
	///�ͷű��������
	virtual void release();

public:
	///�������ɹ� ��ʼ����ʱ����
	void onCreate();
	///�����ӳɹ�ʱ����
	void onConnected();
	///���Ͽ�����ʱ����
	void onDisconnected();
	///�����Ӵ���ʱ����
	void onConnectError();
	///�����ӳ�ʱʱ����
	void onConnectTimeout();
	///��������Ϣʱ����
	void onMessage(string& strbuff);

protected:
	///ö������״̬
	enum {
		ENULL = 1,				//��״̬
		EConnecting = 2,		//���ڽ�������
		EConnected = 3,			//���ӳɹ�
		EConnectTimeout = 4,	//���ӳ�ʱ
		EConnectError = 5,		//�����쳣
		EDisconnected = 6		//�����ж�
	}
	m_nConnectStatus;

	///��ʱʱ���¼
	struct timeval m_ccConnectTime;

protected:
	///���ջ�����
	char m_RecvBuffer[SOCK_RECVBUFFERSIZE];
	string m_strRecvBuffer;

	std::queue<sNetMessage> m_queueMessageCache;
	std::map<uint, std::function<void(const PacketAide& packet, int nLen)> > m_mapPacketHandler;	//ע����հ��ص�����
protected:
	///������IP��ַ
	string m_nnAddress;
	///�������˿ں�
	unsigned short m_nnPort;

	byte mBufPack[SIZE_TCP_BUFFER];
	byte mReadBufPack[SIZE_TCP_BUFFER];
	byte mBufUnPack[SIZE_TCP_BUFFER];
protected:
	///IO����
	CSocket	m_Sock;

	bool m_bDisconnected;
	int m_nReconnectTimes;
	int m_nLostPacketTimes;
};


#endif

