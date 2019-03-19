#include "LogicPromote.h"
#include "Player/Player.h"
#include "WindowPromote.h"

LogicPromote::LogicPromote()
	: m_nGetCoinNum(0)
	, m_nGetGoldNum(0)
	, m_nShareDay(0)
	, m_nGetIndex(0)
{
	_registerMessageHandler();
}

LogicPromote::~LogicPromote()
{

}

void LogicPromote::_registerMessageHandler()
{
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SHARESUCESS_RSP, bind(&LogicPromote::onShareSucessResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GETSHARE_RSP, bind(&LogicPromote::onGetSharePrizeResp, this, placeholders::_1, placeholders::_2));

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SHOUYI_RSP, bind(&LogicPromote::onShareInfoResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SHOUYIRANK_RSP, bind(&LogicPromote::onShareRankResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SHAREREDCOUNT_RSP, bind(&LogicPromote::onShareDaysResp, this, placeholders::_1, placeholders::_2));
}

void LogicPromote::requestShareSucess(int nType)
{
	CMD_ST_ShareSucessReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.sharetype = nType;

	PACKET_AIDE_SIZE_NEW(32);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SHARESUCESS_REQ, packet);
}

void LogicPromote::onShareSucessResp(const PacketAide& packet, int nLen)
{
	CMD_ST_ShareSucessRsp resp;
	resp.fromPack(packet);

	m_nShareDay = resp.dwCount;

	Player::getInstance()->addCoin(resp.lGold);
	HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
	HallGameScene::getInstance()->updateWindow(EWT_Promote, EWPS_SHAREDAYS, nullptr);
}

void LogicPromote::requestGetSharePrize(const std::string& strCode)
{
	CMD_ST_GetShareReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.setString(req.cardnum, strCode.c_str());

	PACKET_AIDE_SIZE_NEW(32);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GETSHARE_REQ, packet);
}

void LogicPromote::onGetSharePrizeResp(const PacketAide& packet, int nLen)
{
	CMD_ST_GetShareRsp resp;
	resp.fromPack(packet);

	//0:领取成功 1-分享码不存在 2-该设备已领取 3-你已经领取 4-自己不能领取自己的分享码
	HallGameScene::getInstance()->openMessageTip(60 + resp.ret);
	if (resp.ret == 0)
	{
		//增加元宝和金币
		Player::getInstance()->addGold(resp.money);
		Player::getInstance()->addCoin(resp.gold);

		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
		HALL_NOTIFICATION(Hall_Message_Refresh_Gold, nullptr);
	}
	
}

void LogicPromote::requestShareInfo(int nOpType)
{
	CMD_ST_ShouYiReq req;

	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.optype = nOpType;

	PACKET_AIDE_SIZE_NEW(8);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SHOUYI_REQ, packet);
}

void LogicPromote::onShareInfoResp(const PacketAide& packet, int nLen)
{
	CMD_ST_ShouYiRsp resp;
	resp.fromPack(packet);

	if (resp.optype == 1)
	{
		m_nGetCoinNum = resp.gold;
		m_nGetGoldNum = resp.money;
	}
	else if(resp.optype == 2)
	{
		Player::getInstance()->addCoin(m_nGetCoinNum);
		Player::getInstance()->addGold(m_nGetGoldNum);

		m_nGetCoinNum = 0;
		m_nGetGoldNum = 0;

		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
		HALL_NOTIFICATION(Hall_Message_Refresh_Gold, nullptr);
	}
	

	HallGameScene::getInstance()->updateWindow(EWT_Promote, EWPS_PRIZE, &resp);
}

void LogicPromote::onShareRankResp(const PacketAide& packet, int nLen)
{
	CMD_ST_ShouRankYiRsp resp;
	resp.fromPack(packet);

	HallGameScene::getInstance()->updateWindow(EWT_Promote, EWPS_RANK, &resp);
}

void LogicPromote::requestShareDays()
{
	CMD_ST_ShareCountReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.optype = 1;

	PACKET_AIDE_SIZE_NEW(8);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SHAREREDCOUNT_REQ, packet);
}

void LogicPromote::requestMySharePrize()
{
	CMD_ST_ShareCountReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.optype = 2;

	PACKET_AIDE_SIZE_NEW(8);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SHAREREDCOUNT_REQ, packet);
}

void LogicPromote::onShareDaysResp(const PacketAide& packet, int nLen)
{
	CMD_ST_ShareCountRsp resp;
	resp.fromPack(packet);

	if (resp.optype == 1)
	{
		m_nShareDay = resp.count;
		m_nGetIndex = resp.getcount;
		HallGameScene::getInstance()->updateWindow(EWT_Promote, EWPS_SHAREDAYS, nullptr);
	}
	else
	{
		Player::getInstance()->addCoin(resp.count);
		m_nGetIndex = getShareDayLevel(m_nShareDay);

		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
		HallGameScene::getInstance()->updateWindow(EWT_Promote, EWPS_SHAREDAYS, nullptr);
	}
}

int LogicPromote::getShareDayLevel(int nShareDay)
{
	int nShareDaysArray[] = { 1, 5, 10, 30 };
	int i = 0;

	for (; i < countarray(nShareDaysArray); i++)
	{
		if (nShareDay < nShareDaysArray[i])
		{
			break;
		}
	}

	return i;
}