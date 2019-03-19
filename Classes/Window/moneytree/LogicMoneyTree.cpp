#include "LogicMoneyTree.h"
#include "Player/Player.h"
#include "df/DF.h"
#include "libs/PacketAide.h"
#include "Util/GameUtil.h"

LogicMoneyTree::LogicMoneyTree()
	: m_uMoneyGetTime(0xffffffff)
{
	_registerMessageHandler();
}

LogicMoneyTree::~LogicMoneyTree()
{

}

void LogicMoneyTree::_registerMessageHandler()
{
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_MONEYTREE_RSP, bind(&LogicMoneyTree::onMoneyTreeResp, this, placeholders::_1, placeholders::_2));
}

void LogicMoneyTree::requestMoneyTree(int nType)
{
	CMD_GP_YaoQianShu_USERINFO req;
	req.nType = nType;
	req.dwGameID = Player::getInstance()->getLogonData().dwGameID;

	PACKET_AIDE_SIZE_NEW(32);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_MONEYTREE_REQ, packet);
}

void LogicMoneyTree::onMoneyTreeResp(const PacketAide& packet, int nLen)
{
	m_objMoneyTreeData.fromPack(packet);
	log("-------------onMoneyTreeResp[ntype = %d,time = %d,GetGold = %d,TotelGold = %d]-------------", m_objMoneyTreeData.nType, m_objMoneyTreeData.nTime, m_objMoneyTreeData.GetGold, m_objMoneyTreeData.TotelGold);

	if (m_objMoneyTreeData.nType == EMTT_GET_DATA)
	{
		m_uMoneyGetTime = m_objMoneyTreeData.nTime + GameUtil::getMillisecond() / 1000;
	}
	else
	{
		//如果是摇动摇钱树得到金币，则重新请求一次摇钱树数据
		requestMoneyTree(EMTT_GET_DATA);
		m_vecCoin.clear();

		Player::getInstance()->addCoin(m_objMoneyTreeData.GetGold);
		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
	}
	
	HallGameScene::getInstance()->updateWindow(EWT_MoneyTree, m_objMoneyTreeData.nType);

}