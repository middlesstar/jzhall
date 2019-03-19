#ifndef _SocketEngine_H_
#define _SocketEngine_H_

#include "ISocketEngine.h"
#include "../Socket/Socket.h"

typedef unsigned char byte;

class CSocketEngine : public ISocketEngine, public ISocketSink
{
public:
	CSocketEngine();
	virtual ~CSocketEngine();

public:
    // 接口ISocketEngine
	/** 设置Socket接收器 */
	virtual void setSocketEngineSink(ISocketEngineSink* pISocketEngineSink);
	/** 链接网络 **/
	virtual bool connect(const char* url, int port);
	/** 关闭网络 **/
	virtual bool disconnect();
	/** 发送数据 **/
	virtual bool send(int main, int sub, void* data, int dataSize);

	/** 发送数据 **/
	virtual bool send(int nCommandId, void* data, int dataSize);

	/** 状态判断 **/
	virtual bool isAlive();
	/** 发送校验 **/
	virtual void setTCPValidate(bool send);

	/** 设置可探测性 **/
	virtual void setDetectable(bool bDetectable);

	/** 设置超时时间 **/
	virtual void setTimeOut(int iTimeOut);
public:
    // 接口ISocketSink
	virtual void onSocketLink();
	virtual void onSocketShut();
	virtual void onSocketError(int errorCode);
	virtual void onSocketData(void* data, int dataSize);
	virtual void onHeartBeat();
    virtual void onBeginHeartBeat();
private:
    // 辅助函数
	void sendTCPValidate();
	const byte* pack(int main, int sub, byte* data, int size);
	bool unpack(byte* data, int start, int length);

	bool isChatPacket(byte* data, int start);			//判断是否聊天的数据包,判断依据，前4个字节为0(即大消息号为0)
	int getChatPacketLength(byte* data, int start);		//获取聊天包的大小(打包数据(【4个字节0 + 4消息号+4包体长度】 + 包体))
	bool unpackChat(byte* data, int start, int length);
private:
	// 接收处理模块
	ISocketEngineSink* mISocketEngineSink;
	// 连接SOCKET
	CSocket mSocket;
	// 发送校验
	bool mIsSendTCPValidate;
private:
	// 临时缓冲
	byte mBufPack[SIZE_TCP_BUFFER];
	byte mBufUnPack[SIZE_TCP_BUFFER];
	// 接收缓冲
	byte mBufRecieve[SIZE_TCP_BUFFER];
	// 接收长度
	int mBufRevLength;
};

#endif // _SocketEngine_H_