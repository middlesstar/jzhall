#include "LogicActivity.h"
#include "Player/Player.h"
#include "LanguageConfig.h"
#include "WindowActivity.h"


LogicActivity::LogicActivity()
{
	_registerMessageHandler();

	//注册抢福袋活动消息
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_GRABRED_rsp, bind(&LogicActivity::onGrabRed, this, placeholders::_1, placeholders::_2));
	
}

LogicActivity::~LogicActivity()
{
}

void LogicActivity::_registerMessageHandler()
{
}


//发送抢红包请求
void LogicActivity::requestGrabRed()
{
	CMD_ST_FuDaiReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.optype = 1;			//请求抢福袋
	req.money = 0;
	req.gold = 0;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_GRABRED_REQ, packet);
}

//接收抢红包同意请求
void LogicActivity::onGrabRed(const PacketAide& packet, int nLen)
{
	m_qfd.fromPack(packet);
	if (m_qfd.ret == 0)
	{
		HallGameScene::getInstance()->updateWindow(EWT_Hall, 1);	//通知开始抢福袋
	}
	else
	{
		HallGameScene::getInstance()->updateWindow(EWT_Hall, 2);	//通知不能抢福袋
	}
}

//发送抢红包数据
void LogicActivity::sendGrabRedData(int score, int yuanbao)
{
	
	CMD_ST_FuDaiReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.optype = 2;												//结束活动提交数据
	req.gold = score;
	req.money = yuanbao;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_GRABRED_REQ, packet);

}


