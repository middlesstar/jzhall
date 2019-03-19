#ifndef __NetManager_H__
#define __NetManager_H__

/* 网络管理 */
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

//连接超时时间(秒)
#define SOCK_CONNECT_TIMEOUT 6
//接收器缓冲区大小
#define SOCK_RECVBUFFERSIZE 1024 * 32

/******************************************************************************
类    名: NetManager
功能描述: 客户端网络对象，一个网络对象对应一个SOCKET
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
	///构造函数，初始化默认值
	NetManager();
	///析构函数，释放所有资源
	virtual ~NetManager();

	static NetManager* getInstance()
	{
		static NetManager m_NetManager;
		return &m_NetManager;
	}

public:
	//设置于服务器连接的地址与端口
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
	///开始无阻塞方式连接
	virtual bool connect();
	///是否处于连接状态
	virtual bool isConnected();
	///关闭连接
	virtual void close();
	///帧循环，读写数据
	virtual void drive();
	///往服务端写数据，无阻塞
	virtual void write(const char* buff, int length);
	///释放本网络对象
	virtual void release();

public:
	///当创建成功 开始连接时调用
	void onCreate();
	///当连接成功时调用
	void onConnected();
	///当断开连接时调用
	void onDisconnected();
	///当连接错误时调用
	void onConnectError();
	///当连接超时时调用
	void onConnectTimeout();
	///当读到消息时调用
	void onMessage(string& strbuff);

protected:
	///枚举连接状态
	enum {
		ENULL = 1,				//无状态
		EConnecting = 2,		//正在进行连接
		EConnected = 3,			//连接成功
		EConnectTimeout = 4,	//连接超时
		EConnectError = 5,		//连接异常
		EDisconnected = 6		//连接中断
	}
	m_nConnectStatus;

	///超时时间记录
	struct timeval m_ccConnectTime;

protected:
	///接收缓冲器
	char m_RecvBuffer[SOCK_RECVBUFFERSIZE];
	string m_strRecvBuffer;

	std::queue<sNetMessage> m_queueMessageCache;
	std::map<uint, std::function<void(const PacketAide& packet, int nLen)> > m_mapPacketHandler;	//注册的收包回调方法
protected:
	///服务器IP地址
	string m_nnAddress;
	///服务器端口号
	unsigned short m_nnPort;

	byte mBufPack[SIZE_TCP_BUFFER];
	byte mReadBufPack[SIZE_TCP_BUFFER];
	byte mBufUnPack[SIZE_TCP_BUFFER];
protected:
	///IO操作
	CSocket	m_Sock;

	bool m_bDisconnected;
	int m_nReconnectTimes;
	int m_nLostPacketTimes;
};


#endif

