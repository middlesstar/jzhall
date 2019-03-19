#include "LogicFreeMoney.h"
#include "Player/Player.h"
#include "Window/hall/WindowHall.h"
#include "Util/GameUtil.h"

LogicFreeMoney::LogicFreeMoney()
{
	_registerMessageHandler();
}

LogicFreeMoney::~LogicFreeMoney()
{

}

void LogicFreeMoney::_registerMessageHandler()
{
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_DAILYLOTTERY_GET_RESULT, bind(&LogicFreeMoney::onDialyPrizeResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_JIUJIJIN_RSP, bind(&LogicFreeMoney::onDoleResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SIGNDAY_RSP , bind(&LogicFreeMoney::onDailySinResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GETLIPIN_RSP, bind(&LogicFreeMoney::onWechatPresentResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_HAOPING_RSP, bind(&LogicFreeMoney::onCommentResp, this, placeholders::_1, placeholders::_2));
}

void LogicFreeMoney::requestDialyPrize()
{
	CMD_GP_DailyLottery_USERINFO req;

	req.dwGameID = Player::getInstance()->getLogonData().dwGameID;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_DAILYLOTTERY_GET, packet);
}

void LogicFreeMoney::onDialyPrizeResp(const PacketAide& packet, int nLen)
{
	m_objDailyPrize.fromPack(packet);
	log("-------------LogicFreeMoney::onDialyPrizeResp[index = %d]------------", m_objDailyPrize.dwIndex);

	if (m_objDailyPrize.dwIndex > 11)
	{
		HallGameScene::getInstance()->openMessageTip(23);
		return;
	}

	HallGameScene::getInstance()->updateWindow(EWT_DialyPrize, m_objDailyPrize.dwIndex);
}

void LogicFreeMoney::requestDole()
{
	CMD_ST_JIUJIJIN_req req;

	req.userId = Player::getInstance()->getLogonData().dwUserID;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_JIUJIJIN_REQ, packet);
}

void LogicFreeMoney::onDoleResp(const PacketAide& packet, int nLen)
{
	CMD_ST_JIUJIJIN_rsp resp;
	resp.fromPack(packet);
	if (resp.code == 0 && resp.gold > 0)
	{
		Player::getInstance()->addCoin(resp.gold);
		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
	}
	else
	{
		//1玩家不存在 2 今日领取达到上限 3 低于1000不能领取 	
		switch (resp.code)
		{
		case 1:
			HallGameScene::getInstance()->openMessageTip(20);
			break;
		case 2:
			HallGameScene::getInstance()->openMessageTip(21);
			break;
		case 3:
			HallGameScene::getInstance()->openMessageTip(22);
			break;
		default:
			break;
		}
	}
}

//请求每日签到
void LogicFreeMoney::requestDailySin(int type)
{
	CMD_ST_SignDay_req req;
	req.userId = Player::getInstance()->getLogonData().dwUserID;
	if (1 == type)
	{
		req.opType = 1;
	}
	else if(2 == type)
	{
		req.opType = 2;
	}
	else
	{
		req.opType = 3;
	}

	PACKET_AIDE_SIZE_NEW(20);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_SIGNDAY_REQ, packet);
}

//领取每日签到金币返回
void LogicFreeMoney::onDailySinResp(const PacketAide& packet, int nLen)
{
	m_objSignDay.fromPack(packet); 
	
	if (0 == m_objSignDay.ret)
	{
		
		if (3 == m_objSignDay.opType)
		{
			HallGameScene::getInstance()->openWindow(EWT_Sign);
		}
		//判断是否连续七天的签到
		else if (2 == m_objSignDay.opType)
		{
			Player::getInstance()->addCoin(m_objSignDay.gold);
			HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
			HallGameScene::getInstance()->updateWindow(EWT_Sign, 2);
		}
		else if (1 == m_objSignDay.opType)
		{
			if (0 == m_objSignDay.ret)
			{
				Player::getInstance()->addCoin(m_objSignDay.gold);
				HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
				HallGameScene::getInstance()->updateWindow(EWT_Sign, 2);
			}
			else
			{
				//提示签到不成功
			}
			
		}
	}	
	else if (1 == m_objSignDay.ret)
	{
		//提示玩家不存在
	}
	else if (2 == m_objSignDay.ret)
	{
		//提示玩家已经签到
		HallGameScene::getInstance()->updateWindow(EWT_Sign, m_objSignDay.ret);
	}	
	
	
}

void LogicFreeMoney::requestWechatPresent(const std::string& strExCode)
{
	CMD_ST_GetLiPinReq req;

	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.setString(req.cardnum, strExCode.c_str());

	PACKET_AIDE_SIZE_NEW(64);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GETLIPIN_REQ, packet);
}

void LogicFreeMoney::onWechatPresentResp(const PacketAide& packet, int nLen)
{
	CMD_ST_GetLiPinRsp resp;
	resp.fromPack(packet);

	//0:成功 1-已领取 2-不存在卡号 3-此账号领取次数达到上限(每个号只能领取5次)
	switch (resp.ret)
	{
	case 0:			//获取到了金币和元宝
		Player::getInstance()->addCoin(resp.gold);
		Player::getInstance()->addGold(resp.money);
		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
		HALL_NOTIFICATION(Hall_Message_Refresh_Gold, nullptr);
		HallGameScene::getInstance()->openMessageTip(30);
		break;
	case 1:
		HallGameScene::getInstance()->openMessageTip(31);
		break;
	case 2:
		HallGameScene::getInstance()->openMessageTip(32);
		break;
	default:
		break;
	}

}

void LogicFreeMoney::requestComment()
{
	CMD_ST_HaoPingReq req;

	req.userid = Player::getInstance()->getLogonData().dwUserID;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_HAOPING_REQ, packet);
}

void LogicFreeMoney::onCommentResp(const PacketAide& packet, int nLen)
{
	CMD_ST_HaoPingRsp resp;
	resp.fromPack(packet);

	if (resp.ret == 0)
	{
		Player::getInstance()->setComment(true);
		Player::getInstance()->addCoin(resp.gold);
		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
		HallGameScene::getInstance()->updateWindow(EWT_Hall, ENUM_COMMENT_SUCCESS);
		HallGameScene::getInstance()->closeWindow(EWT_Comment);

		GameUtil::setData("UserComment", 1);
	}
}