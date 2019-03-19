#include "LogicChat.h"
#include "WindowChat.h"
#include "Player/Player.h"
#include "df/DF.h"
#include "LanguageConfig.h"

LogicChat::LogicChat()
	: m_vecLastPos(1000, 570)
	, m_pChatWindow(nullptr)
	, m_nCurChannel(ECC_WORLD)
	, m_nSendPacketCount(0)
{
	_registerMessageHandler();
}

LogicChat::~LogicChat()
{

}

void LogicChat::_registerMessageHandler()
{
	//NetworkChat* pNetChat = NetworkChat::getInstance();

	//pNetChat->registerPakcetHandler(ENCP_MSG_LINK, bind(&LogicChat::onChatServerLinked, this, placeholders::_1, placeholders::_2));
	//pNetChat->registerPakcetHandler(ENCP_MSG_LOGINCHAT_RSP, bind(&LogicChat::onLoginChatServerResp, this, placeholders::_1, placeholders::_2));
	//pNetChat->registerPakcetHandler(ENCP_MSG_CHARALLRSP, bind(&LogicChat::onSendChatMessageResp, this, placeholders::_1, placeholders::_2));
	//pNetChat->registerPakcetHandler(ENCP_MSG_GETRED_RSP, bind(&LogicChat::onGetRedPacketResp, this, placeholders::_1, placeholders::_2));

	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_CHATMSG_RSP, bind(&LogicChat::onSendChatMessageResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_CHATGETHONGBAO_RSP, bind(&LogicChat::onGetRedPacketResp, this, placeholders::_1, placeholders::_2));
}

int LogicChat::getChatChannelType(bool bWorldChannel)
{
	int nChannelType = ECC_WORLD;

	if (!bWorldChannel)
	{
		switch (HallGameScene::m_gCurSceneId)
		{
		case EHST_HALL:
			nChannelType = ECC_HALL;
			break;
		case EHST_SLWH:
			nChannelType = ECC_SLWH;
			break;
		case EHST_SHARK:
			nChannelType = ECC_SHARK;
			break;
		case EHST_SHUIHU:
			nChannelType = ECC_SHUIHU;
			break;
		default:
			break;
		}
	}

	return nChannelType;
}

// void LogicChat::onChatServerLinked(const PacketAide& packet, int nLen)
// {
// 	log("--------------LogicChat::onChatServerLinked----------------");
// 
// 	requestLoginChatServer();
// }
// 
// void LogicChat::requestLoginChatServer()
// {
// 	Msg_LoginChat_Req req;
// 	string strPwd = Player::getInstance()->getPassword();
// 
// 	req.dType = 1;
// 	req.setString(req.accounts, Player::getInstance()->getAccount().c_str());
// 	req.setString(req.accpwd, (strPwd.size() < 32) ? DF::MD5Encrypt(strPwd.c_str()) : strPwd.c_str());
// 
// 	PACKET_AIDE_SIZE(256);
// 	req.toPack(packet);
// 
// 	NetworkChat::getInstance()->sendMsg(ENCP_MSG_LOGINCHAT_REQ, packet);
// }
// 
// void LogicChat::onLoginChatServerResp(const PacketAide& packet, int nLen)
// {
// 	Msg_LoginChat_Rsp rsp;
// 	rsp.fromPack(packet);
// 
// 	log("----------------LogicChat::onLoginChatServerResp----------------", rsp.ret);
// }

void LogicChat::requestSendChatMessage(const std::string& strMsg)
{
	CMD_ST_ChatMsgReq req;

	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.msgtype = m_nCurChannel;
	req.setString(req.content, strMsg.c_str());

	PACKET_AIDE_SIZE_NEW(256);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_CHATMSG_REQ, packet);
}

void LogicChat::requestSendRedPacket(int nRid)
{
	//保存发送红包的数量
	int redCount[] = {200,1000,5000,10000};
	if (nRid > 0 && nRid <= countarray(redCount))
	{
		m_nSendPacketCount = redCount[nRid - 1];
	}

	CMD_ST_ChatHongBaoReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.msgtype = m_nCurChannel;
	req.hongbaoid = nRid;

	PACKET_AIDE_SIZE_NEW(32);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_CHATHONGBAO_REQ, packet);
}

void LogicChat::onSendChatMessageResp(const PacketAide& packet, int nLen)
{
	log("-------------LogicChat::onSendChatMessageResp[len = %d]--------------", nLen);

	CMD_ST_ChatMsgRsp rsp;
	rsp.fromPack(packet);
	std::queue<CMD_ST_ChatMsgRsp>* pQueue = rsp.msgtype == ECC_WORLD ? &m_queueWorldMessage : &m_queueChannelMessage;

	log("-------------LogicChat::onSendChatMessageResp[content = %s,uname = %s]--------------", rsp.content, rsp.uname);

	if (rsp.ntype == 2)
	{
		if (rsp.ret != 0)
		{
			HallGameScene::openSceneMessageTip(rsp.ret == 1 ? GETSTRING(35).c_str() : GETSTRING(102).c_str());
			return;
		}
		else
		{
			Player::getInstance()->addCoin(-m_nSendPacketCount);
			m_nSendPacketCount = 0;
			HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
		}
	}

	if (pQueue->size() >= m_gMaxMessageCount)
	{
		pQueue->pop();
	}
	pQueue->push(rsp);

	//刷新UI
	if (m_pChatWindow)
	{
		m_pChatWindow->updateWindow(rsp.msgtype, &rsp);
	}
}

void LogicChat::requestGetRedPacket(int nRedPacketId)
{
	CMD_ST_ChatGetHongBaoReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.hongbaoid = nRedPacketId;

	PACKET_AIDE_SIZE_NEW(32);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_CHATGETHONGBAO_REQ, packet);
	//NetworkChat::getInstance()->sendMsg(ENCP_MSG_GETRED_REQ, packet);
}

void LogicChat::onGetRedPacketResp(const PacketAide& packet, int nLen)
{
	CMD_ST_ChatGetHongBaoRsp resp;
	resp.fromPack(packet);

	if (resp.ret == 0 && resp.gold > 0)
	{
		Player::getInstance()->addCoin(resp.gold);
		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
	}
	else if (resp.ret == 1)
	{
		return;				//红包不存在
	}

	//刷新UI
	if (m_pChatWindow)
	{
		m_pChatWindow->updateWindow(ECS_GETPACKET, &resp);
	}
}

void LogicChat::requestChangeRoom(int nRoomId)
{
	CMD_ST_ReplaceReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.roomid = nRoomId;

	PACKET_AIDE_SIZE_NEW(32);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_REPLACEROOM_REQ, packet);
}