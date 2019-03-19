#ifndef _Socket_H_
#define _Socket_H_
#include <list>
#include <thread>
#include <mutex>
#include <iostream>   
#include <map>

struct ISocketSink
{
	virtual ~ISocketSink(){}
	virtual void onSocketLink() = 0;
	virtual void onSocketShut() = 0;
	virtual void onSocketError(int errorCode) = 0;
	virtual void onSocketData(void* data, int dataSize) = 0;
	virtual void onHeartBeat()=0;
};
#define TIMEOUT 10000
#define CONNECT_TIMEOUT 5000
class CSocket
{
public:
	 CSocket(bool bDetectable=false,int iTimeOut=TIMEOUT);
	~CSocket();
	void setSocketSink(ISocketSink* pISocketSink);
	bool isAlive() const;
	int  connect(const char* url, int port);
	void disconnect();
	int send(const char* data, int size);

	////setting Detectable
	inline void setDetectable(bool bDetectable){ mbDetector = bDetectable; }
	inline void setTimeOut(int iTimeOut){ miTimeOut = iTimeOut; }
    void onBeginHeartBeat();

private:
	int connect_IPv4(const char* url, int port);
	int connect_IPv6(const char* url, int port);

	void onSocketLink();
	void onSocketShut();
	void onSocketError(int errorCode);
	void onSocketData(void* data, int dataSize);

public:
	void onSocketTimeOut(int type);

    const int& getFd(){return mSocket;}
	//////////////////////////////////////////////////////////////////////////
	// 处理线程
private:
	// 接收数据线程
	static void recv_thread(void* p);
public:
	// 分发数据队列
	static void post();
	static void purge();

public:
	bool IsReadable();		//检测是有数据可读
	bool IsWritable();		//检测是否可以写入数据
	int	IsConnected();		//检测是否已经建立连接

	int	Read(char* pBuffer, int nLen);		//读取数据，无阻塞
	int	Write(const char* pBuffer, int nLen);		//写入数据，无阻塞

	bool connect_extra(const char* url, int port);
	void disconnect_extra();			//主动断开连接
	void close_extra();					//关闭IO对象

protected:
	bool connect_extra_IPv4(const char* url, int port);
	bool connect_extra_IPv6(const char* url, int port);

	bool _setSocketOption(int nSocket);
private:
	int				mSocket;
	ISocketSink*	mSocketSink;
	int				mSocketIndex;
	friend class MTSocketQueue;
	friend class SHZMTSocketQueue;
	friend class SocketDetector;
	bool   mbDetector;  //// whether heart beat pack
	int    miTimeOut;   //// the limitation for twice heart beat pack time internal
};

typedef struct detectData
{
	int                 s;
	long long int       mlBeginTime;
	int                 iTimeOut;
	CSocket*            pSocket;
	int                 type;
} detectData;

class SocketDetector
{
public:
	static SocketDetector* getInstance();

protected:
	SocketDetector();
	~SocketDetector();
public:
    inline void setExit(bool bExit){ mbExit = bExit; }
	inline bool isExit(){ return mbExit; }
	inline void lock(){ mMutex.lock();}
	inline void unlock(){ mMutex.unlock(); }
	inline void lock_f(){ mfMutex.lock(); }
	inline void unlock_f(){ mfMutex.unlock(); }
	inline void setCreate(bool bCreate){ mbCreate = bCreate; }
	inline bool getCreate(){ return mbCreate; }
public:
	static void detectThread();

public:
	void push(detectData data);
	void update(CSocket* socket);
	void pop(int socketIndex);
	void onStartDetector();

private:
	static SocketDetector* m_pDetector;
	bool   mbExit;
	bool   mbCreate;
public:
	std::list<detectData>* m_pData;
	std::list<int>* m_pClear;
	std::mutex mMutex;
	std::mutex mfMutex;
};

#endif // _Socket_H_