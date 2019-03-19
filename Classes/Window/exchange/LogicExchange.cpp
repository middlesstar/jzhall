#include "LogicExchange.h"
#include "Player/Player.h"
#include "LanguageConfig.h"
#include "WindowExchange.h"

LogicExchange::LogicExchange()
	: m_nCostGold(0)
{
	_registerMessageHandler();
}

LogicExchange::~LogicExchange()
{

}

void LogicExchange::_registerMessageHandler()
{
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALLGOODS_GET_RESULT, bind(&LogicExchange::onExchangeResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_DXREARD_RSP, bind(&LogicExchange::onRecordsResp, this, placeholders::_1, placeholders::_2));
}

void LogicExchange::requestExchange(int nGoodsIndex, longlong lTel/* = 0*/, longlong lQQ/* = 0*/, const string& strName/* = ""*/, const string& strAddress/* = ""*/)
{
	CMD_GP_Hall_Goods_USERINFO req;

	req.userId = Player::getInstance()->getLogonData().dwUserID;
	req.wIndex = nGoodsIndex;
	req.llTelNum = lTel;
	req.uqq = lQQ;
	req.setString(req.tName, strName.c_str());
	req.setString(req.tAddress, strAddress.c_str());

	PACKET_AIDE_SIZE_NEW(512);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALLGOODS_GET, packet);
}

void LogicExchange::onExchangeResp(const PacketAide& packet, int nLen)
{
	CMD_GP_Hall_Goods_Get resp;
	resp.fromPack(packet);

	//0=成功 1=玩家不存在 2-不存在物品 3-元宝不足 4-资料不完整
	switch (resp.dwErrorCode)
	{
	case 0:
		Player::getInstance()->addGold(-m_nCostGold);
		m_nCostGold = 0;
		HALL_NOTIFICATION(Hall_Message_Refresh_Gold, nullptr);

		if (resp.wgold > 0)
		{
			Player::getInstance()->addCoin(resp.wgold);
			HALL_NOTIFICATION(Hall_Message_Refresh_Coin,nullptr);
			HallGameScene::getInstance()->openMessageTip(StringUtils::format(GETSTRING(11).c_str(), resp.wgold).c_str());
		}
		else
		{
			HallGameScene::getInstance()->openMessageTip(GETSTRING(27).c_str());
		}

		requestRecords();			//兑换完成后跳转到兑换记录界面

		break;
	case 1:
		HallGameScene::getInstance()->openMessageTip(GETSTRING(20).c_str());
		break;
	case 2:
		HallGameScene::getInstance()->openMessageTip(GETSTRING(24).c_str());
		break;
	case 3:
		HallGameScene::getInstance()->openMessageTip(GETSTRING(25).c_str());
		break;
	case 4:
		HallGameScene::getInstance()->openMessageTip(GETSTRING(26).c_str());
		break;
	default:
		break;
	}
}

void LogicExchange::requestRecords()
{
	CMD_ST_DxRewardList_req req;
	req.userId = Player::getInstance()->getLogonData().dwUserID;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_DXREARD_REQ, packet);
}

void LogicExchange::onRecordsResp(const PacketAide& packet, int nLen)
{
	CMD_ST_DxRewardList_rsp resp;
	resp.fromPack(packet);

	HallGameScene::getInstance()->updateWindow(EWT_Exchange, EWES_RECORD, &resp);
}