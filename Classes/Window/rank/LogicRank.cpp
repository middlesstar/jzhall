#include "LogicRank.h"
#include "Player/Player.h"

LogicRank::LogicRank()
{
	_registerMessageHandler();
}

LogicRank::~LogicRank()
{

}

void LogicRank::_registerMessageHandler()
{
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_GET_RANK_RESULT, bind(&LogicRank::onRankResp, this, placeholders::_1, placeholders::_2));
}

void LogicRank::requestRank(int nType)
{
	CMD_GP_GET_RANK req;
	
	req.dwUserID = Player::getInstance()->getLogonData().dwUserID;
	req.dwType = nType;

	PACKET_AIDE_SIZE_NEW(32);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_GET_RANK, packet);
}

void LogicRank::onRankResp(const PacketAide& packet, int nLen)
{
	log("------------LogicRank::onRankResp-------------");
}