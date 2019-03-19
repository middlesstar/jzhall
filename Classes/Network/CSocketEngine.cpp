#include <sstream>
#include "QPCipher.h"
#include "CSocketEngine.h"

#define SOCKETENGIN_LOG         cocos2d::log
using namespace std;
//////////////////////////////////////////////////////////////////////////

ISocketEngine* ISocketEngine::create()
{
	return new CSocketEngine();
}

void ISocketEngine::destory(ISocketEngine* pISocketEngine)
{
	if (pISocketEngine)
	{
		delete pISocketEngine;
	}
}

//////////////////////////////////////////////////////////////////////////
CSocketEngine::CSocketEngine()
{
	mISocketEngineSink = 0;
	mIsSendTCPValidate = false;
	mSocket.setSocketSink(this);
}

CSocketEngine::~CSocketEngine()
{
	mISocketEngineSink=0;
	mSocket.setSocketSink(0);
	disconnect();
}

//////////////////////////////////////////////////////////////////////////
// ISocketEngine 接口实现

/** 设置Socket接收器 */
void CSocketEngine::setSocketEngineSink(ISocketEngineSink* pISocketEngineSink)
{
	mISocketEngineSink = pISocketEngineSink;
}

/** 链接网络 **/
bool CSocketEngine::connect(const char* url, int port)
{
	int nRet = mSocket.connect(url, port);
	if (nRet != 0)
		return false;
	// 接收长度
	mBufRevLength=0;
	sendTCPValidate();
	return isAlive();
}

/** 关闭网络 **/
bool CSocketEngine::disconnect()
{
	//// 接收长度
	//mBufRevLength=0;
	mSocket.disconnect();
	return true;
}

/** 发送数据 **/
bool CSocketEngine::send(int main, int sub, void* data, int dataSize)
{
	if (!isAlive())
		return false;
	// 打包数据
	const byte* dataSend = pack(main, sub, (byte*)data, dataSize);
	mSocket.send((const char*)dataSend, SIZE_PACK_HEAD+dataSize);
	return true;
}

bool CSocketEngine::send(int nCommandId, void* data, int dataSize)
{
	if (!isAlive())
		return false;
	// 打包数据(【4个字节0 + 4消息号+4包体长度】 +  包体)

	int nPos = 4;
	memset(mBufPack, 0, sizeof(mBufPack));

	memcpy(&mBufPack[nPos], &nCommandId, sizeof(nCommandId));
	nPos += sizeof(nCommandId);

	memcpy(&mBufPack[nPos], &dataSize, sizeof(dataSize));
	nPos += sizeof(dataSize);

	memcpy(&mBufPack[nPos], data, dataSize);
	nPos += dataSize;

	mSocket.send((const char*)mBufPack, nPos);
	return true;
}

/** 状态判断 **/
bool CSocketEngine::isAlive()
{
	return mSocket.isAlive();
}

/** 发送校验 **/
void CSocketEngine::setTCPValidate(bool send)
{
	mIsSendTCPValidate = send;
}

/** 设置可探测性 **/
void CSocketEngine::setDetectable(bool bDetectable)
{
	mSocket.setDetectable(bDetectable);
}

/** 设置超时时间 **/
void CSocketEngine::setTimeOut(int iTimeOut)
{
	mSocket.setTimeOut(iTimeOut);
}

//////////////////////////////////////////////////////////////////////////
// 接口ISocketSink
void CSocketEngine::onSocketLink()
{
	if (mISocketEngineSink != 0)
		mISocketEngineSink->onEventTCPSocketLink();
}

void CSocketEngine::onSocketShut()
{
	if (mISocketEngineSink != 0)
		mISocketEngineSink->onEventTCPSocketShut();
}

void CSocketEngine::onSocketError(int errorCode)
{
	if (mISocketEngineSink != 0)
		mISocketEngineSink->onEventTCPSocketError(errorCode);
}

void CSocketEngine::onSocketData(void* data, int dataSize)
{
	memcpy(&mBufRecieve[mBufRevLength], data, dataSize);
	// 接收长度增加
	mBufRevLength += dataSize;
	
	// 尝试解包
	int iUnpackIndex = 0;
	bool isReaded = false;	
	int iDstLength = SIZE_PACK_INFO;
	int nChatPacketHeadLength = 12;			//大消息号4+小消息号4+包长4

	bool isChat = false;

	// 接收长度 - 已经解包的数据长度 
	while ((mBufRevLength - iUnpackIndex) >= iDstLength)
	{
		if (isReaded == false)
		{
			isReaded   = true;
			isChat = isChatPacket((unsigned char*)mBufRecieve, iUnpackIndex);
			if (isChat)			//如果是聊天信息包
			{
				iDstLength = getChatPacketLength((unsigned char*)mBufRecieve, iUnpackIndex);
			}
			else
			{
				iDstLength = QPCipher::getPackSize((unsigned char*)mBufRecieve, iUnpackIndex);
			}

			if (iDstLength > mBufRevLength && SOCKET_CHECK)
			{
				break;
			}
		} 
		else
		{
			if (isChat)
			{
				if (!unpackChat(mBufRecieve, iUnpackIndex, iDstLength))
				{
					disconnect();
					return;
				}
				iUnpackIndex += nChatPacketHeadLength;
			}
			else
			{
				// 解包数据并通知调用
				if (!unpack(mBufRecieve, iUnpackIndex, iDstLength))
				{
					disconnect();
					return;
				}
			}

			// 解包数据游标移动
			iUnpackIndex += iDstLength;
			// 重置接收长度为包信息头长度
			iDstLength = SIZE_PACK_INFO;
			// 重置接收为未读取包信息头状态
			isReaded = false;
			
		}
	}
	// 剩余数据长度
	mBufRevLength = mBufRevLength - iUnpackIndex;

	// 移动剩余数据
	if (mBufRevLength > 0)
	{
		memmove(mBufRecieve, &mBufRecieve[iUnpackIndex], mBufRevLength);
	}
}

void CSocketEngine::onHeartBeat()
{
	//// 发送心跳请求
	send(400, 1, 0, 0);
}

void CSocketEngine::onBeginHeartBeat()
{
    mSocket.onBeginHeartBeat();
}

//////////////////////////////////////////////////////////////////////////
// 辅助函数
void CSocketEngine::sendTCPValidate()
{
	if (mIsSendTCPValidate == false)
		return;
	// 获取验证信息
	QPCipher::tcpValidate(&mBufPack[0], 0);
	// 发送验证
	ostringstream ss;
	ss.str("");
	for (int i = 0; i < SIZE_VALIDATE; i++)
	{
		ss << mBufPack[i] << ",";
	}
	mSocket.send(ss.str().c_str(), (int)ss.str().size());
}

const byte* CSocketEngine::pack(int main, int sub, byte* data, int size)
{
	mBufPack[0]=0;
	int packsize = SIZE_PACK_HEAD + size;
	QPCipher::setPackInfo(mBufPack, packsize, main, sub);

	// 赋值
	if (size > 0) {
		memcpy(&mBufPack[SIZE_PACK_HEAD], data, size);
	}
	// 加密数据
	QPCipher::encryptBuffer(mBufPack, packsize);
	return mBufPack;
}

bool CSocketEngine::unpack(byte* data, int start, int length)
{
	// 解密
	if ((data[start] & QPCipher::getCipherMode()) > 0)
	{
		QPCipher::decryptBuffer(data, start, length);
	}
	// 主命令码
	int main = QPCipher::getMainCommand(data, start);
	// 次命令码
	int sub = QPCipher::getSubConmmand(data, start);

    // 附加数据
	if (length > 8) 
	{
		memcpy(mBufUnPack, &data[start+8], length - 8);
	}
	
	length -= 8;
	
	if (main == 400 && sub == 1)
	{
		SocketDetector::getInstance()->update(&mSocket);
		onHeartBeat(); /// 回复心跳
	}
	else
	{
		if (mISocketEngineSink != 0)
		{
 			bool bHandle = mISocketEngineSink->onEventTCPSocketRead(main, sub, mBufUnPack, length);
            //收到捕鱼消息，重置消息时间
            if(main == 200)
            {
                SocketDetector::getInstance()->update(&mSocket);
            }
            
			return bHandle;
		}
	}
	
	return true;
}

bool CSocketEngine::isChatPacket(byte* data, int start)
{
	return data[start] == 0 && data[start + 1] == 0 && data[start + 2] == 0 && data[start + 3] == 0;
}

int CSocketEngine::getChatPacketLength(byte* data, int start)
{
	int nLen = 0;
	memcpy(&nLen, &data[start + 8], sizeof(nLen));

	return nLen;
}

bool CSocketEngine::unpackChat(byte* data, int start, int length)
{
	// 主命令码
	int main = -1;
	// 次命令码
	int sub = -1;
	int nPos = 0;
	memcpy(&main, &data[start + nPos], sizeof(main));
	nPos += sizeof(main);

	memcpy(&sub, &data[start + nPos], sizeof(sub));
	nPos += sizeof(sub);

	if (main == 0 && sub > 0)
	{
		int nLen = 0;
		memcpy(&nLen, &data[start + nPos], sizeof(nLen));
		nPos += sizeof(nLen);

		if (nLen > 0 && nLen < SIZE_TCP_BUFFER)
		{
			memset(mBufUnPack, 0, sizeof(mBufUnPack));
			memcpy(mBufUnPack, &data[start + nPos], nLen);

			if (mISocketEngineSink)
			{
				return mISocketEngineSink->onEventTCPSocketRead(main, sub, mBufUnPack, length);
			}
		}
		
	}

	return true;
}