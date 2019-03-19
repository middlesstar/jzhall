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
	int getChatChannelType(bool bWorldChannel);							//��ȡ����Ƶ�������࣬	bWorldChannel��ʾ�Ƿ���������				

	//void onChatServerLinked(const PacketAide& packet, int nLen);

	//void requestLoginChatServer();
	//void onLoginChatServerResp(const PacketAide& packet, int nLen);


	void requestSendChatMessage(const std::string& strMsg);				//��������Ƶ������Ϣ
	void requestSendRedPacket(int nRid);								//���ͺ��
	void onSendChatMessageResp(const PacketAide& packet, int nLen);
	
	void requestGetRedPacket(int nRedPacketId);									//������ȡ���
	void onGetRedPacketResp(const PacketAide& packet, int nLen);

	void requestChangeRoom(int nRoomId);

protected:
	CC_SYNTHESIZE(Vec2,m_vecLastPos,LastPos);
	CC_SYNTHESIZE(WindowBase*, m_pChatWindow, ChatWindow);
	CC_SYNTHESIZE(int, m_nCurChannel, CurChannel);			//��ǰƵ��
	CC_SYNTHESIZE(int, m_nSendPacketCount, SendPacketCount);			//���ͺ��������
	CC_SYNTHESIZE(std::string, m_strRedPacketName, RedPacketName);		//������˵�����

	//��ౣ��50����Ϣ
	static const int m_gMaxMessageCount = 50;
	std::queue<CMD_ST_ChatMsgRsp> m_queueWorldMessage;		//����Ƶ������Ϣ
	std::queue<CMD_ST_ChatMsgRsp> m_queueChannelMessage;		//��ǰƵ������Ϣ
};

#endif // _LogicChat_H_

