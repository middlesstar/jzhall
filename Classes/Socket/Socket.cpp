#include "cocos2d.h"
#include "Socket.h"
#include "MacroDefineCommon.h"
#include "df/Timer.h"

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	#include <WinSock2.h>  
	#include <WS2tcpip.h>
	#pragma comment (lib,"WS2_32.Lib")  
	#define LHS_EAGAIN          WSAEWOULDBLOCK    
	#define LHS_EWOULDBLOCK     WSAEWOULDBLOCK  
#else
	#include <signal.h>  
	#include <sys/socket.h>  
	#include <netinet/in.h>  
	#include<netinet/tcp.h>  
	#include <netdb.h>  
	#include <errno.h>  
	#include <fcntl.h>  
	#include <unistd.h>  
	#include <sys/stat.h>  
	#include <sys/types.h>  
	#include <arpa/inet.h> 
	typedef int					SOCKET;  
	#define INVALID_SOCKET		(SOCKET)(~0)  
	#define SOCKET_ERROR		(-1)  
	#define LHS_EAGAIN          EAGAIN              //此操作会令线程阻塞,但socket为不可阻塞的  
	#define LHS_EWOULDBLOCK     EWOULDBLOCK         //资源暂时不可用  
#endif

USING_NS_CC;
using namespace std;   

#define SOCKET_PRINTF log

//////////////////////////////////////////////////////////////////////////
static bool					__isSetSig		= true; 
static int					__SocketIndex	= 1;
static map<int, CSocket*>	__SocketMap;
SocketDetector* SocketDetector::m_pDetector = nullptr;
//////////////////////////////////////////////////////////////////////////
void socket_close(int s)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)  
	shutdown(s, SD_BOTH);
	closesocket(s);
#else  
	close(s);  
#endif 
}

int socket_send(int s, const char* data, int size)
{
	unsigned int flags = 0;
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)  
	flags=MSG_NOSIGNAL;  
#endif  
	return send(s, data, size, flags);
}

int socket_recv(int s, char* data, int size)
{
	unsigned int flags=0;  
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)  
	flags=MSG_NOSIGNAL;  
#endif  
	return recv(s, data, size, flags);
}

int socket_error()
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	return WSAGetLastError();
#else
	return errno;
#endif
}

void socket_sleep(unsigned int delay)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	Sleep(delay);
#else
	usleep(delay*1000); 
#endif
}

//////////////////////////////////////////////////////////////////////////
class CInitSock
{
public:
	CInitSock()
	{
		#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
			//WSADATA对象，用于存储Socket动态库的信息
			WSADATA wsaData;
			//调用Windows Sockets DLL
			if(WSAStartup(0x0110,&wsaData))
			{
				//初始化DLL错误，显示错误提示，程序退出
				SOCKET_PRINTF("Initialize socket failed.\n");
			}
		#endif

		#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)  
			if (__isSetSig)  
			{  
				__isSetSig=false;  
				struct sigaction Sa;  
				Sa.sa_handler = SIG_IGN;  
				sigaction(SIGPIPE,&Sa,0);
			}  
		#endif 
	}

	~CInitSock()
	{
		#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
			WSACleanup();
		#endif
	}
};

CInitSock __InitSock;

class MTSocketQueueLocker  
{  
	std::mutex* _mutex;  

public:  
	MTSocketQueueLocker(std::mutex* aMutex) : _mutex(aMutex)  
	{  
		_mutex->lock();
	}  
	~MTSocketQueueLocker()
	{  
		_mutex->unlock();
	}  
};
#define MTSocketQueueLock(mutex)  MTSocketQueueLocker __locker__(mutex) 

class MTSocketQueue
{
public:
	static MTSocketQueue* shared();
	static void purge();

public:
	void push(unsigned int onwer, int cmd, char* data, int dataSize);
	void post();

private:
	MTSocketQueue();
	~MTSocketQueue();

private:
	struct MTData
	{
		unsigned int	onwer;		//数据拥有者
		int				cmd;		//命令:0关闭连接 1:错误码(dataSize) 2:消息
		char*			data;
		int				dataSize;
	};

	std::list<MTData>	mDataQueues;
	std::mutex			mDataMutex;

	static MTSocketQueue*	msInstance;
};

//////////////////////////////////////////////////////////////////////////
MTSocketQueue* MTSocketQueue::msInstance = 0;

MTSocketQueue* MTSocketQueue::shared()
{
	if (msInstance == 0)
		msInstance = new MTSocketQueue();

	return msInstance;
}

void MTSocketQueue::purge()
{
	if (msInstance)
		delete msInstance;
	msInstance = 0;
}

//////////////////////////////////////////////////////////////////////////

MTSocketQueue::MTSocketQueue()
{
}

MTSocketQueue::~MTSocketQueue()
{
	{
		MTSocketQueueLock(&mDataMutex);

		while (!mDataQueues.empty())
		{
			MTData& mtData = mDataQueues.front();
			if (mtData.data)
				delete []mtData.data;
			mDataQueues.pop_front();
		}
	}
}

void MTSocketQueue::push(unsigned int owner, int cmd, char* data, int dataSize)
{
	if (__SocketMap[owner] == 0)
		return;

	MTData mtData;
	memset(&mtData, 0, sizeof(mtData));
	mtData.onwer	= owner;
	mtData.cmd		= cmd;
	mtData.dataSize = dataSize;

	if (data != 0)
	{
		mtData.data = new char[dataSize];
		if (mtData.data == 0)
			return;
		memcpy(mtData.data, data, dataSize);
	}
	
	MTSocketQueueLock(&mDataMutex);
	mDataQueues.push_back(mtData);
}

void MTSocketQueue::post()
{
	MTSocketQueueLock(&mDataMutex);
	while (!mDataQueues.empty())
	{
		const MTData& mtData = mDataQueues.front();
		CSocket* owner = __SocketMap[mtData.onwer];
		if (owner)
		{
			switch (mtData.cmd)
			{
			case 0:
				owner->disconnect();
				break;
			case 1:
				owner->onSocketError(mtData.dataSize);
				break;
			case 2:
				owner->onSocketData(mtData.data, mtData.dataSize);
				break;
			}
		}

		if (mtData.data)
			delete []mtData.data;
		mDataQueues.pop_front();
	}
}

/*----------------------------------水浒传消息队列-------------------------------------*/
//////////////////////////////////////////////////////////////////////////
// 队列锁
class SHZMTSocketQueueLocker
{
	std::mutex* _mutex;

public:
	SHZMTSocketQueueLocker(std::mutex* aMutex) : _mutex(aMutex)
	{
		_mutex->lock();
	}
	~SHZMTSocketQueueLocker()
	{
		_mutex->unlock();
	}
};
#define SHZMTSocketQueueLock(mutex)  SHZMTSocketQueueLocker __locker__(mutex) 

//////////////////////////////////////////////////////////////////////////
// 循环消息处理
class SHZMTSocketQueue
{
public:
	static SHZMTSocketQueue* shared();
	static void purge();

public:
	void push(unsigned int onwer, int cmd, char* data, int dataSize);
	void post();

private:
	SHZMTSocketQueue();
	~SHZMTSocketQueue();

private:
	struct MTData
	{
		unsigned int	onwer;		//数据拥有者
		int				cmd;		//命令:0关闭连接 1:错误码(dataSize) 2:消息
		char*			data;
		int				dataSize;
	}; // MTData

	std::list<MTData>	mDataQueues;
	//pthread_mutex_t		mMutex;
	std::mutex			mDataMutex;

	static SHZMTSocketQueue*	msInstance;
}; // MTSocketQueue


//////////////////////////////////////////////////////////////////////////
SHZMTSocketQueue* SHZMTSocketQueue::msInstance = 0;


SHZMTSocketQueue* SHZMTSocketQueue::shared()
{
	if (msInstance == 0)
		msInstance = new SHZMTSocketQueue();

	return msInstance;
}


void SHZMTSocketQueue::purge()
{
	if (msInstance)
		delete msInstance;
	msInstance = 0;
}


//////////////////////////////////////////////////////////////////////////
SHZMTSocketQueue::SHZMTSocketQueue()
{
	//pthread_mutex_init(&mMutex, 0);  
}

SHZMTSocketQueue::~SHZMTSocketQueue()
{
	{
		//MTSocketQueueLock(&mMutex);
		SHZMTSocketQueueLock(&mDataMutex);

		while (!mDataQueues.empty())
		{
			MTData& mtData = mDataQueues.front();
			if (mtData.data)
				delete[]mtData.data;
			mDataQueues.pop_front();
		}
	}

	//pthread_mutex_destroy(&mMutex);
}

void SHZMTSocketQueue::push(unsigned int owner, int cmd, char* data, int dataSize)
{
	if (__SocketMap[owner] == 0)
		return;

	MTData mtData;
	memset(&mtData, 0, sizeof(mtData));
	mtData.onwer = owner;
	mtData.cmd = cmd;
	mtData.dataSize = dataSize;

	if (data != 0)
	{
		mtData.data = new char[dataSize];
		if (mtData.data == 0)
			return;
		memcpy(mtData.data, data, dataSize);
	}

	//MTSocketQueueLock(&mMutex);
	SHZMTSocketQueueLock(&mDataMutex);
	mDataQueues.push_back(mtData);
}

void SHZMTSocketQueue::post()
{
	//MTSocketQueueLock(&mMutex);
	SHZMTSocketQueueLock(&mDataMutex);
	while (!mDataQueues.empty())
	{
		const MTData& mtData = mDataQueues.front();
		CSocket* owner = __SocketMap[mtData.onwer];
		if (owner)
		{
			switch (mtData.cmd)
			{
			case 0:
				owner->disconnect();
				break;
			case 1:
				owner->onSocketError(mtData.dataSize);
				break;
			case 2:
				owner->onSocketData(mtData.data, mtData.dataSize);
				break;
			}

		}

		if (mtData.data)
			delete[]mtData.data;
		mDataQueues.pop_front();
	}
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CSocket::CSocket(bool bDetectable, int iTimeOut)
	: mSocket(INVALID_SOCKET)
	, mSocketSink(0)
	, mbDetector(bDetectable)
	, miTimeOut(iTimeOut)
{
	mSocketIndex=-1;
}

CSocket::~CSocket()
{
	if (mSocketIndex >= 0)
		__SocketMap[mSocketIndex]=0;
	mSocketSink = 0;
	disconnect();
}

void CSocket::setSocketSink(ISocketSink* pISocketSink)
{
	mSocketSink = pISocketSink;
}

bool CSocket::isAlive() const
{
	return mSocket != INVALID_SOCKET;
}

int CSocket::send(const char* data, int size)
{
	if (!isAlive())
		return -1;

	return socket_send(mSocket, data, size);
}

int CSocket::connect(const char* url, int port)
{
	return connect_IPv6(url, port);
}

void CSocket::onBeginHeartBeat()
{
    mSocketSink->onHeartBeat();
    
    /// start Heart beat Detecting
    detectData data;
    data.s = mSocket;
    data.mlBeginTime = CoTimer::getCurrentTime();
    data.iTimeOut    = miTimeOut;
    data.pSocket     = this;
    data.type        = 0;  /// heart beat time out flag
    SocketDetector::getInstance()->push(data);
}

void CSocket::disconnect()
{
  	if (!isAlive())
		return;

	if (mSocketIndex > 0)
	{
  		if (mbDetector) SocketDetector::getInstance()->pop(mSocket);
		__SocketMap[mSocketIndex] = 0;
		mSocketIndex = -1;
	}

    socket_close(mSocket);
	mSocket = INVALID_SOCKET;	
	onSocketShut();
}

int CSocket::connect_IPv4(const char* url, int port)
{
	disconnect();

	hostent* hp = gethostbyname(url);

	if (!hp)
	{
		SOCKET_PRINTF("socket gethostbyname error!\n");
		onSocketError(1025);//socket_error());
		return -1;
	}

	//create Socket
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET)
	{
		SOCKET_PRINTF("create socket failed\n");
		onSocketError(1025);//socket_error());
		return -1;
	}

	sockaddr_in local;
	memset(&local, 0, sizeof(local));
	memcpy((char*)&local.sin_addr, hp->h_addr, hp->h_length);
	local.sin_family = hp->h_addrtype;
	local.sin_port = htons(port);

	/////Connecting
	if (::connect(s, (const sockaddr*)&local, sizeof(local)) < 0)
	{
		SocketDetector::getInstance()->pop(s);
		/// failed
		int errCode = socket_error();
		JZ_PRINTF("connect error code = %d, ***********", errCode);
		socket_close(s);
		onSocketError(1025);//errCode);
		return -1;
	}

	//// running in this way, not Time Out Removing the SocketDetector Data
	JZ_PRINTF("connect succeed! ***********************");
	SocketDetector::getInstance()->pop(s);

	if (mSocketIndex == -1)
	{
		mSocketIndex = __SocketIndex++;
		__SocketMap[mSocketIndex] = this;
	}

	mSocket = (int)s;

	// Create Receiver Thread
	thread t(recv_thread, &mSocketIndex);
	t.detach();

	onSocketLink();
	return 0;
}

int CSocket::connect_IPv6(const char* url, int port)
{
	disconnect();

	char strIP[100];
	sprintf(strIP, "%s", url);

	char strPort[100];
	sprintf(strPort, "%d", port);

	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	struct sockaddr_in *sinp;
	int sockfd;
	int err;
	char seraddr[INET_ADDRSTRLEN];
	short serport;

	hint.ai_family = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_CANONNAME;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_addr = NULL;
	hint.ai_canonname = NULL;
	hint.ai_next = NULL;

	if ((err = getaddrinfo(strIP, strPort, &hint, &ailist)) != 0)
	{
		SOCKET_PRINTF("getaddrinfo error: %s\n", gai_strerror(err));
		return -1;
	}

	bool isConnectOk = false;
	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		sinp = (struct sockaddr_in *)aip->ai_addr;
		if (inet_ntop(sinp->sin_family, &sinp->sin_addr, seraddr, INET_ADDRSTRLEN) != NULL)
		{
			SOCKET_PRINTF("server address is %s\n", seraddr);
		}
		else
		{
			continue;
		}

		serport = ntohs(sinp->sin_port);
		SOCKET_PRINTF("server port is %d\n", serport);

		if (serport == 0)
		{
			sinp->sin_port = htons(port);
		}

		if ((sockfd = socket(aip->ai_family, SOCK_STREAM, 0)) < 0)
		{
			SOCKET_PRINTF("create socket failed: %s\n", strerror(errno));
			isConnectOk = false;

			continue;
		}

		SOCKET_PRINTF("create socket ok\n");

		if (::connect(sockfd, aip->ai_addr, aip->ai_addrlen) < 0) 
		{
			SOCKET_PRINTF("connect error code = %d,can't connect to %s: %s\n", socket_error(), strIP, strerror(errno));

			socket_close(sockfd);
			onSocketError(1025);//errCode);

			isConnectOk = false;
			continue;
		}
		isConnectOk = true;

		JZ_PRINTF("connect succeed! ***********************");
		SocketDetector::getInstance()->pop(sockfd);

		if (mSocketIndex == -1)
		{
			mSocketIndex = __SocketIndex++;
			__SocketMap[mSocketIndex] = this;
		}

		mSocket = sockfd;

		// Create Receiver Thread
		thread t(recv_thread, &mSocketIndex);
		t.detach();

		onSocketLink();

		break;
	}

	freeaddrinfo(ailist);

	return isConnectOk ? 0 : -1;
}

//////////////////////////////////////////////////////////////////////////
// 辅助函数
void CSocket::onSocketLink()
{
	if (mSocketSink)
		mSocketSink->onSocketLink();
}

void CSocket::onSocketShut()
{
	if (mSocketSink)
		mSocketSink->onSocketShut();
}

void CSocket::onSocketError(int errorCode)
{
	if (mSocketSink)
		mSocketSink->onSocketError(errorCode);
}

void CSocket::onSocketData(void* data, int dataSize)
{
	if (mSocketSink)
		mSocketSink->onSocketData(data, dataSize);
}

void CSocket::onSocketTimeOut(int type)
{
	SOCKET_PRINTF("Time Out........");
    MTSocketQueue::shared()->push(this->mSocketIndex, 1, 0, 1024 + type);
}

//////////////////////////////////////////////////////////////////////////
// 客户端处理线程
void CSocket::recv_thread(void* p)
{
	const int MAX_BUF = 16384;
	int socketIndex = *(int*)p;
	char dataBuffer[MAX_BUF]={0};

	while (__SocketMap[socketIndex] && __SocketMap[socketIndex]->isAlive())
	{
        //SOCKET_PRINTF("socket fd: %d---------------------------------------------------------------", __SocketMap[socketIndex]->getFd());
		int dataSize = socket_recv(__SocketMap[socketIndex]->mSocket, dataBuffer, MAX_BUF);
		if(dataSize==SOCKET_ERROR)  
		{  
			int errCode =socket_error();
            //SOCKET_PRINTF("errno: %d", errCode);
			if(errCode==LHS_EWOULDBLOCK)  
			{  
				socket_sleep(20);
				continue; 
			} 
			else if(errCode==LHS_EAGAIN)
			{
                socket_sleep(20);
				continue;
			}
			if (errCode == 10004)
            {
                break;
            }

			if (__SocketMap.size() > socketIndex && __SocketMap[socketIndex] && MTSocketQueue::shared())
            {
                MTSocketQueue::shared()->push(socketIndex, 1, 0, errCode);
                socket_sleep(20);
                continue;
            }

			break;
		}
		if (dataSize == 0)
		{
            //SOCKET_PRINTF("socket data size is 0...");
            break;
		}

		if (__SocketMap[socketIndex] && MTSocketQueue::shared())
			MTSocketQueue::shared()->push(socketIndex, 2, dataBuffer, dataSize);
	}
	if (__SocketMap.size() > socketIndex && __SocketMap[socketIndex] && MTSocketQueue::shared())
    {
        //SOCKET_PRINTF("socket break...socket fd: %d, *****************************", __SocketMap[socketIndex]->getFd());
        MTSocketQueue::shared()->push(socketIndex, 0, 0, 0);
    }
}

// distributing Data
void CSocket::post()
{
	if (MTSocketQueue::shared())
		MTSocketQueue::shared()->post();
}

void CSocket::purge()
{
	MTSocketQueue::purge();
}

bool CSocket::IsReadable()
{
	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(mSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (select((int)(mSocket + 1), &fd, NULL, NULL, &tv) > 0)
	{
		if (FD_ISSET(mSocket, &fd))
		{
			return true;
		}
	}

	return false;
}

///检测是否可以写入数据
bool CSocket::IsWritable()
{
	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(mSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (select((int)(mSocket + 1), NULL, &fd, NULL, &tv) > 0)
	{
		if (FD_ISSET(mSocket, &fd))
		{
			return true;
		}
	}

	return false;
}

///检测是否已经建立连接
int	CSocket::IsConnected()
{
	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(mSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (select((int)(mSocket + 1), NULL, &fd, NULL, &tv) > 0)
	{
		if (FD_ISSET(mSocket, &fd))
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			return 1;
#else
			int error;
			socklen_t len = sizeof(error);
			if (getsockopt(mSocket, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			{
				return -1;
			}
			//LINUX ECONNREFUSED 111
			//UNIX ECONNREFUSED 61
			if (error == ECONNREFUSED)
			{
				return -1;
			}
			return 1;
#endif
		}
	}
	return 0;
}

int	CSocket::Read(char* pBuffer, int nLen)
{
	if (mSocket == INVALID_SOCKET)
	{
		return SOCKET_ERROR;
	}

	return recv(mSocket, pBuffer, nLen, 0);
}

int	CSocket::Write(const char* pBuffer, int nLen)
{
	if (mSocket == INVALID_SOCKET)
	{
		return SOCKET_ERROR;
	}
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) )
	return ::send(mSocket, pBuffer, nLen, 0);
#else
	return ::send(mSocket, pBuffer, nLen, SO_NOSIGPIPE);
#endif
}

bool CSocket::connect_extra(const char* url, int port)
{
	return connect_extra_IPv6(url, port);
}

void CSocket::disconnect_extra()
{
	if (mSocket != INVALID_SOCKET)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		shutdown(mSocket, SD_BOTH);
#else
		shutdown(mSocket, SHUT_RDWR);
#endif
	}
}

void CSocket::close_extra()
{
	if (mSocket != INVALID_SOCKET)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		closesocket(mSocket);
#else
		close(mSocket);
#endif
		mSocket = INVALID_SOCKET;
	}
}

bool CSocket::connect_extra_IPv4(const char* url, int port)
{
	close_extra();
	hostent* hp = gethostbyname(url);
	
	if (!hp)
	{
		SOCKET_PRINTF("socket gethostbyname error!\n");
		return false;
	}
	
	//create Socket
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	
	if (s == INVALID_SOCKET)
	{
		SOCKET_PRINTF("create socket failed\n");
		return false;
	}
	
	if (!_setSocketOption(s))
	{
		SOCKET_PRINTF("setSocketOption failed\n");
		return false;
	}
	
	mSocket = (int)s;
	
	sockaddr_in local;
	memset(&local, 0, sizeof(local));
	memcpy((char*)&local.sin_addr, hp->h_addr, hp->h_length);
	local.sin_family = hp->h_addrtype;
	local.sin_port = htons(port);
	
	/////Connecting
	int nRet = ::connect(s, (const sockaddr*)&local, sizeof(local));
	if (nRet != 0)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		int nError = WSAGetLastError();
		if (nError == WSAEWOULDBLOCK)
		{
			return true;
		}
		else
		{
			return false;
		}
#else
		if (nRet == SOCKET_ERROR && errno == EINPROGRESS)
		{
			return true;
		}
		else
		{
			return false;
		}
#endif
	}
	
	//// running in this way, not Time Out Removing the SocketDetector Data
	JZ_PRINTF("connect succeed! ***********************");
	
	return true;
}

bool CSocket::connect_extra_IPv6(const char* url, int port)
{
	close_extra();

	char strIP[100];
	sprintf(strIP, "%s", url);

	char strPort[100];
	sprintf(strPort, "%d", port);

	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	struct sockaddr_in *sinp;
	int sockfd;
	int err;
	char seraddr[INET_ADDRSTRLEN];
	short serport;

	hint.ai_family = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_CANONNAME;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_addr = NULL;
	hint.ai_canonname = NULL;
	hint.ai_next = NULL;

	if ((err = getaddrinfo(strIP, strPort, &hint, &ailist)) != 0) 
	{
		SOCKET_PRINTF("getaddrinfo error: %s\n", gai_strerror(err));
		return false;
	}

	bool isConnectOk = false;
	for (aip = ailist; aip != NULL; aip = aip->ai_next) 
	{
		sinp = (struct sockaddr_in *)aip->ai_addr;
		if (inet_ntop(sinp->sin_family, &sinp->sin_addr, seraddr, INET_ADDRSTRLEN) != NULL)
		{
			SOCKET_PRINTF("server address is %s\n", seraddr);
		}
		else
		{
			continue;
		}

		serport = ntohs(sinp->sin_port);
		SOCKET_PRINTF("server port is %d\n", serport);

		if (serport == 0)
		{
			sinp->sin_port = htons(port);
		}

		if ((sockfd = socket(aip->ai_family, SOCK_STREAM, 0)) < 0) 
		{
			SOCKET_PRINTF("create socket failed: %s\n", strerror(errno));
			isConnectOk = false;

			continue;
		}

		SOCKET_PRINTF("create socket ok\n");

		if (!_setSocketOption(sockfd))
		{
			SOCKET_PRINTF("setSocketOption failed\n");
			return false;
		}

		mSocket = sockfd;
		
		/////Connecting
		int nRet = ::connect(sockfd, aip->ai_addr, aip->ai_addrlen);
		if (nRet != 0)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			int nError = WSAGetLastError();
			if (nError != WSAEWOULDBLOCK)
			{
				SOCKET_PRINTF("can't connect to %s: %s\n", strIP, strerror(errno));
				isConnectOk = false;

				continue;
			}
#else
			if (nRet != SOCKET_ERROR || errno != EINPROGRESS)
			{
				SOCKET_PRINTF("can't connect to %s: %s\n", strIP, strerror(errno));
				isConnectOk = false;
				continue;
			}
#endif
		}

		isConnectOk = true;

		break;
	}

	freeaddrinfo(ailist);

	return isConnectOk;
}

bool CSocket::_setSocketOption(int nSocket)
{
	//设置非阻塞模式
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	unsigned long ul = 1;
	int nRet = ioctlsocket(nSocket, FIONBIO, (unsigned long*)&ul);
	if (nRet == SOCKET_ERROR)
	{
		close_extra();
		//LOG_ERROR("设置非阻塞模式失败");
		return false;
	}
#else
	int nFlags = fcntl(nSocket, F_GETFL, 0);
	int nRet = fcntl(nSocket, F_SETFL, nFlags | O_NONBLOCK);
	if (nRet == SOCKET_ERROR)
	{
		close_extra();
		//LOG_ERROR("设置非阻塞模式失败");
		return false;
	}
#endif
	//设置套接字无延时
	int nNoDelay = 1;
	if (setsockopt(nSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&nNoDelay, sizeof(nNoDelay)) == SOCKET_ERROR)
	{
		close_extra();
		//LOG_ERROR("设置套接字无延时失败");
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////
/////Socket Time Out Detecting
SocketDetector* SocketDetector::getInstance()
{
	if (m_pDetector == nullptr){
		m_pDetector = new SocketDetector;
		m_pDetector->onStartDetector();
	}
	return m_pDetector;
}

SocketDetector::SocketDetector() :mbCreate(false)
{
	this->setExit(false);
	m_pData  = new std::list<detectData>();
	m_pClear = new std::list<int>();
}

void SocketDetector::onStartDetector()
{
	if (!this->getCreate())
	{
		thread dThread(detectThread);
		dThread.detach();
		this->setCreate(true);
	}
}

SocketDetector::~SocketDetector()
{
	this->setExit(true);
}

void SocketDetector::detectThread()
{
	SocketDetector* thiz = getInstance();
	SOCKET_PRINTF("running detect Thread......");
	while (!(thiz->isExit()))
	{
		if (!(thiz->m_pData->empty()))
		{
			thiz->lock();
			long long int time = CoTimer::getCurrentTime();
			std::list<detectData>::iterator it;

			for (it = thiz->m_pData->begin(); it != thiz->m_pData->end(); it++)
			{ 
				long long int beginTime = it->mlBeginTime;
				int iTimeOut = it->iTimeOut;
				CSocket* socket = it->pSocket;
				int s = it->s;
				int type = it->type;
				if (time - beginTime >= iTimeOut)
				{
					//it = thiz->m_pData->erase(it);   /// TimeOut Remove
					thiz->m_pClear->push_back(it->s);
					socket->onSocketTimeOut(type);   /// TimeOut
					SOCKET_PRINTF("socket TimeOut id = %d,time=%lld....", s, time - beginTime);
				}
			}
			thiz->unlock();
		}
		if (!thiz->m_pClear->empty())
		{
			thiz->lock();
			std::list<int>::iterator IIt = thiz->m_pClear->begin();
			std::list<detectData>::iterator it2;
			for (IIt = thiz->m_pClear->begin(); IIt != thiz->m_pClear->end();)
			{
				int s = *IIt;
				for (it2 = thiz->m_pData->begin(); it2 != thiz->m_pData->end();)
				{
					if (it2->s == s)
					{
						thiz->m_pData->erase(it2);
						SOCKET_PRINTF("socket detect pop id = %d", s);
						break;
					}
					it2++;
				}
				IIt++;
			}
			thiz->m_pClear->clear();
			thiz->unlock();
		}
	}
}

void SocketDetector::push(detectData data)
{
	m_pDetector->lock_f();
	m_pData->push_back(data);
	m_pDetector->unlock_f();
}

void SocketDetector::pop(int s)
{
	m_pDetector->lock_f();
	m_pClear->push_back(s);
	m_pDetector->unlock_f();
}

void SocketDetector::update(CSocket* socket)
{
	m_pDetector->lock_f();

	std::list<detectData>::iterator it = m_pData->begin();

	int s = socket->mSocket;

	while (it != m_pData->end())
	{
		if ((*it).s == s)
		{
			//SOCKET_PRINTF("update socket heart beat id = %d", s);
			(*it).mlBeginTime = (long long int)CoTimer::getCurrentTime();
			break;
		}
	   it++;
	}
	m_pDetector->unlock_f();
}