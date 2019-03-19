/******************************************************************************
文件名: NetManager.cpp
编写日期: 2012-5-27
功能描述: 客户端网络对象，一个网络对象对应一个SOCKET
******************************************************************************/
#include "NetManager.h"
#include "QPCipher.h"
#include "CommandDef.h"
#include "CommandStructDef.h"
#include "Player/Player.h"
#include "HallGameScene.h"
#include "LanguageConfig.h"

#define MAX_OFFLINE_HEARTBEAT_TIME		3			//判断断线的心跳包次数

///构造函数，初始化默认值
NetManager::NetManager()
	: m_nnPort(0)
	, m_nConnectStatus(ENULL)
	, m_bDisconnected(false)
	, m_nReconnectTimes(0)
	, m_nLostPacketTimes(0)
{
	registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_HEART_RSP, bind(&NetManager::onHeartBeatResp, this, placeholders::_1, placeholders::_2));
}

///析构函数，释放所有资源
NetManager::~NetManager()
{
}

///设置于服务器连接的地址与端口
void NetManager::setAddress(const char* ip, unsigned short port)
{
	this->m_nnPort = port;
	this->m_nnAddress = ip;
}

const string& NetManager::getLoginServerIp()
{
	return m_nnAddress;
}

unsigned short NetManager::getLoginServerPort()
{
	return m_nnPort;
}

void NetManager::sendHeartBeat()
{
	if (!isConnected())
	{
		return;
	}

	//log("-------------sendHeartBeat-----------------");

	if (m_nLostPacketTimes >= MAX_OFFLINE_HEARTBEAT_TIME)
	{
		HallGameScene::getInstance()->openOkDialog(GETSTRING(2), nullptr, ECCNT_DISCONNECT);		//断线了,弹出提示框
		this->close();

		return;
	}

 	CMD_ST_HeartReq req;
 	req.userid = Player::getInstance()->getLogonData().dwUserID;
 
 	PACKET_AIDE_SIZE(4);
 	req.toPack(packet);
 
 	memset(mBufPack, 0, sizeof(mBufPack));
 
 	int nPacketSize = SIZE_PACK_HEAD + packet.getPosition();
 	QPCipher::setPackInfo(mBufPack, nPacketSize, HALL_MDM_GP_SERVICE, HALL_SUB_HEART_REQ);
 
 	// 赋值
 	if (nPacketSize > SIZE_PACK_HEAD) {
 		memcpy(&mBufPack[SIZE_PACK_HEAD], packet.getBuffer(), nPacketSize - SIZE_PACK_HEAD);
 	}
 	// 加密数据
 	QPCipher::encryptBuffer(mBufPack, nPacketSize);
 	write((const char*)mBufPack, nPacketSize);

	m_nLostPacketTimes++;
}

void NetManager::onHeartBeatResp(const PacketAide& packet, int nLen)
{
	//log("-------------onHeartBeatResp-----------------");
	m_nLostPacketTimes--;
}

void NetManager::sendMsg(int main, int sub, const PacketAide& packet)
{
	if (!isConnected())
	{
		m_queueMessageCache.push(sNetMessage(main, sub, packet));

		if (!connect())
		{
			HallGameScene::getInstance()->openMessageTip(1);
		}

		return;
	}

	log("-------------NetManager::sendMsg[main = %d,sub = %d,len = %d]---------------", main, sub, packet.getPosition());

	memset(mBufPack, 0, sizeof(mBufPack));

	int nPacketSize = SIZE_PACK_HEAD + packet.getPosition();
	QPCipher::setPackInfo(mBufPack, nPacketSize, main, sub);

	// 赋值
	if (nPacketSize > SIZE_PACK_HEAD) {
		memcpy(&mBufPack[SIZE_PACK_HEAD], packet.getBuffer(), nPacketSize - SIZE_PACK_HEAD);
	}
	// 加密数据
	QPCipher::encryptBuffer(mBufPack, nPacketSize);
	write((const char*)mBufPack, nPacketSize);
}

void NetManager::unpack(byte* data, int start, int length)
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

	log("---------NetManager::unpack[main = %d,sub = %d,length = %d]--------", main, sub, length);

	memset(mBufUnPack, 0, sizeof(mBufUnPack));

	// 附加数据
	if (length > 8)
	{
		memcpy(mBufUnPack, &data[start + 8], length - 8);
	}

	length -= 8;

	if (main == 400 && sub == 1)
	{
		// 回复心跳
	}
	else
	{
		std::map<uint, std::function<void(const PacketAide& packet, int nLen)> >::iterator it = m_mapPacketHandler.find(compositionCommand(main, sub));
		if (it != m_mapPacketHandler.end())
		{
			PacketAide packet(mBufUnPack);
			it->second(packet, length);
		}
	}

}

void NetManager::onReconnectWithServer()
{
	purgeMsgCache();
}

void NetManager::purgeMsgCache()
{
	/* 连接成功,将待发送的消息全部发送 */
	while (!m_queueMessageCache.empty())
	{
		sNetMessage sendmsg = m_queueMessageCache.front();
		m_queueMessageCache.pop();

		sendMsg(sendmsg.m_nMain, sendmsg.m_nSub, sendmsg.m_packetData);
	}
}

void NetManager::registerPakcetHandler(int nMain, int nSub, const std::function<void(const PacketAide& packet, int nLen)>& func)
{
	registerPakcetHandler(compositionCommand(nMain, nSub), func);
}

void NetManager::registerPakcetHandler(uint uMesType, const std::function<void(const PacketAide& packet, int nLen)>& func)
{
	std::map<uint, std::function<void(const PacketAide& packet, int nLen)> >::iterator it = m_mapPacketHandler.find(uMesType);
	if (it != m_mapPacketHandler.end())
	{
		m_mapPacketHandler.erase(it);
	}

	m_mapPacketHandler.insert(make_pair(uMesType, func));
}

int NetManager::compositionCommand(int main, int sub)
{
	return (int)(main << 16) + sub;
}

void NetManager::decompositionCommand(int Command, int* main, int* sub)
{
	int nBase = pow(2, 16);
	*main = Command / nBase;
	*sub = Command % nBase;
}

const byte* NetManager::_encrypt(int main, int sub, byte* data, int size)
{
	memset(mBufPack, 0, sizeof(mBufPack));
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

void NetManager::_decrypt(byte* data, int start, int length)
{
	// 解密
	if ((data[start] & QPCipher::getCipherMode()) > 0)
	{
		QPCipher::decryptBuffer(data, start, length);
	}
}

///开始无阻塞方式连接
bool NetManager::connect()
{
	//there is connected or connecting 
	if (this->m_nConnectStatus == EConnected || this->m_nConnectStatus == EConnecting){
		return false;
	}
	//validate value
	if (this->m_nnAddress == "" || this->m_nnPort == 0) {
		return false;
	}
	if (!m_Sock.connect_extra(this->m_nnAddress.c_str(), this->m_nnPort)) {
		return false;
	}
	//set the connecting status
	this->m_nConnectStatus = EConnecting;
	//get the connect time of started.
	gettimeofday(&m_ccConnectTime, NULL);
	//call back to virtual function 
	this->onCreate();

	return true;
}

///是否处于连接状态
bool NetManager::isConnected()
{
	if (this->m_nConnectStatus == EConnected)
	{
		return true;
	}
	return false;
}

///关闭连接
void NetManager::close()
{
	this->m_Sock.close_extra();
	this->m_nConnectStatus = EDisconnected;
}

///释放本网络对象
void NetManager::release()
{
	this->close();
	//delete this;
}

///往服务端写数据，无阻塞
void NetManager::write(const char* buff, int length)
{
	//check status
	if (this->m_nConnectStatus != EConnected) {
		return;
	}
	//check io is alive
	if (m_Sock.IsWritable()) {
		//pack length
		if (length > 0) {
			int nLen = m_Sock.Write(buff, length);
			if (nLen == SOCKET_ERROR) {
				//set the connecting status
				this->m_nConnectStatus = EDisconnected;
				//call back to virtual function
				this->onDisconnected();
				//release socket
				this->close();
				//log
				CCLOG("## [DEBUG] Write Disconnected if(nLen == SOCKET_ERROR)");
			}
		}
	}
	else {
		//log
		CCLOG("## [DEBUG] Write Disconnected if( m_Sock.IsWritable() )");
	}
}

///帧循环，读写数据
void NetManager::drive()
{
	//connect successed
	if (this->m_nConnectStatus == EConnected){
		//Read Buffer
		while (m_Sock.IsReadable())
		{
			memset(m_RecvBuffer, 0, sizeof(m_RecvBuffer));
			int nLen = m_Sock.Read(m_RecvBuffer, SOCK_RECVBUFFERSIZE);
			//check error
			if (nLen == 0 || nLen == SOCKET_ERROR) {
				//change connecting status
				this->m_nConnectStatus = EDisconnected;
				//call back to virtual function
				this->onDisconnected();
				//release socket
				this->close();
				//stop while and loop;
				break;
			}
			else {
				//read data
				m_strRecvBuffer.append(m_RecvBuffer, nLen);
				this->onMessage(m_strRecvBuffer);
			}
		}
	}
	//sock was keeping connecting status
	else if (this->m_nConnectStatus == EConnecting) {
		//check connected status of unblock socket
		int nRet = this->m_Sock.IsConnected();
		if (nRet == 1) {
			//set the connecting status
			this->m_nConnectStatus = EConnected;
			//call back to virtual function
			this->onConnected();
		}
		else if (nRet < 0) {	//error
			//set the connecting status
			this->m_nConnectStatus = EConnectError;
			//call back to virtual function
			this->onConnectError();
			//close socket
			this->close();
		}
		else {
			//if it didn't finish the work at time
			struct timeval ccTimeNow;
			gettimeofday(&ccTimeNow, NULL);
			if (ccTimeNow.tv_sec - m_ccConnectTime.tv_sec >= SOCK_CONNECT_TIMEOUT){
				//set the connecting status
				this->m_nConnectStatus = EConnectTimeout;
				//call back to virtual function
				this->onConnectTimeout();
				//release socket
				this->close();
			}
		}
	}
}


void NetManager::onCreate()
{
	log("NetManager::onCreate");
}

void NetManager::onConnected()
{
	log("------------NetManager::onConnected------------");

	m_nLostPacketTimes = 0;

	//如果刚刚断开连接，请求一下刷新信息
	if (m_bDisconnected)
	{
		Player::getInstance()->requsetFlushServerInfo();
		m_bDisconnected = false;
	}
	else
	{
		purgeMsgCache();
	}
}

void NetManager::onDisconnected()
{
	log("NetManager::onDisconnected");
	m_bDisconnected = true;
}

void NetManager::onConnectError()
{
	log("NetManager::onConnectError");
}

void NetManager::onConnectTimeout()
{
	log("NetManager::onConnectTimeout");
}

void NetManager::onMessage(string& strbuff)
{
	log("NetManager::onMessage");

	int iDstLength = 0;
	while (strbuff.length() >= SIZE_PACK_INFO)
	{
		iDstLength = QPCipher::getPackSize((unsigned char*)strbuff.c_str(), 0);

		//取出来的包长度有问题，把这些数据放弃
		if (iDstLength > SIZE_TCP_BUFFER)
		{
			strbuff.clear();
			break;
		}

		if (strbuff.length() < iDstLength)
		{
			break;
		}

		memset(mReadBufPack, 0, sizeof(mReadBufPack));
		memcpy(mReadBufPack, strbuff.c_str(), iDstLength);
		strbuff.erase(0, iDstLength);

		unpack(mReadBufPack, 0, iDstLength);
	}


}