#ifndef  _LogicChat_H_
#define  _LogicChat_H_

#include "Window/LogicBase.h"

class WindowBase;

class  LogicChat : public LogicBase
{
public:
	LogicChat();
	virtual ~LogicChat();

	SINGLE_INSTANCE_FUNC(LogicChat);

protected:
	void _registerMessageHandler();

public:
	int getChatChannelType(bool bWorldChannel);							//获取聊天频道的种类，	bWorldChannel表示是否世界聊天				

	//void onChatServerLinked(const PacketAide& packet, int nLen);

	//void requestLoginChatServer();
	//void onLoginChatServerResp(const PacketAide& packet, int nLen);


	void requestSendChatMessage(const std::string& strMsg);				//发送世界频道的消息
	void requestSendRedPacket(int nRid);								//发送红包
	void onSendChatMessageResp(const PacketAide& packet, int nLen);
	
	void requestGetRedPacket(int nRedPacketId);									//请求领取红包
	void onGetRedPacketResp(const PacketAide& packet, int nLen);

	void requestChangeRoom(int nRoomId);

protected:
	CC_SYNTHESIZE(Vec2,m_vecLastPos,LastPos);
	CC_SYNTHESIZE(WindowBase*, m_pChatWindow, ChatWindow);
	CC_SYNTHESIZE(int, m_nCurChannel, CurChannel);			//当前频道
	CC_SYNTHESIZE(int, m_nSendPacketCount, SendPacketCount);			//发送红包的数量
	CC_SYNTHESIZE(std::string, m_strRedPacketName, RedPacketName);		//发红包人的名字

	//最多保留50条消息
	static const int m_gMaxMessageCount = 50;
	std::queue<CMD_ST_ChatMsgRsp> m_queueWorldMessage;		//世界频道的消息
	std::queue<CMD_ST_ChatMsgRsp> m_queueChannelMessage;		//当前频道的消息
};

#endif // _LogicChat_H_

