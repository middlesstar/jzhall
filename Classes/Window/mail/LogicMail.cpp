#include "LogicMail.h"
#include "Player/Player.h"

LogicMail::LogicMail()
{
	_registerMessageHandler();
}

LogicMail::~LogicMail()
{

}

void LogicMail::_registerMessageHandler()
{
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GETEAMILL_RSP, bind(&LogicMail::onMailsResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GETEMAILATT_RSP, bind(&LogicMail::onMailsOperateResp, this, placeholders::_1, placeholders::_2));
}

void LogicMail::_addAttachGoods(int nType, int nCount)
{
	switch (nType)
	{
	case 1:			//金币
		Player::getInstance()->addCoin(nCount);
		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
		break;
	case 2:			//元宝
		Player::getInstance()->addGold(nCount);
		HALL_NOTIFICATION(Hall_Message_Refresh_Gold, nullptr);
		break;
	case 3:			//船票
		Player::getInstance()->addTicket(nCount);
		HALL_NOTIFICATION(Hall_Message_Refresh_Ticket, nullptr);
		break;
	default:
		break;
	}
}

void LogicMail::requestMails()
{
	CMD_ST_GetEmailList_req req;
	req.userId = Player::getInstance()->getLogonData().dwUserID;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GETEAMILL_REQ, packet);
}

void LogicMail::onMailsResp(const PacketAide& packet, int nLen)
{
	CMD_ST_GetEmailList_rsp resp;
	resp.fromPack(packet);

	memcpy(&m_stMailList, &resp, sizeof(m_stMailList));
	HallGameScene::getInstance()->updateWindow(EWT_Mail, 0, &m_stMailList);
}

void LogicMail::requestMailsOperate(int nType,int eid)
{
	CMD_ST_GetEmailAttReq req;
	req.euid = eid;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.optye = nType;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GETEMAILATT_REQ, packet);
}

void LogicMail::onMailsOperateResp(const PacketAide& packet, int nLen)
{
	CMD_ST_GetEmailAttRsp resp;
	resp.fromPack(packet);

	//成功
	if (resp.ret == 0)
	{
		HallGameScene::getInstance()->updateWindow(EWT_Mail, 1, nullptr);

		if (resp.optye == EEOT_GET_ATTACH)
		{
			//要增加金币元宝船票并刷新界面
			if (!m_stCurClickMail.isEmpty())
			{
				//附件1
				if (m_stCurClickMail.attachtype != 0 && m_stCurClickMail.attachnums > 0)
				{
					_addAttachGoods(m_stCurClickMail.attachtype, m_stCurClickMail.attachnums);
				}

				//附件2
				if (m_stCurClickMail.attachtype2 != 0 && m_stCurClickMail.attachnums2 > 0)
				{
					_addAttachGoods(m_stCurClickMail.attachtype2, m_stCurClickMail.attachnums2);
				}

				m_stCurClickMail.clear();
			}
		}
	}
}

void LogicMail::deleteEmailByEid(int nEid)
{
	for (int i = 0; i < countarray(m_stMailList.emaillist); i++)
	{
		if (m_stMailList.emaillist[i].eid == nEid)
		{
			m_stMailList.emaillist[i].clear();
			break;
		}
	}
}

CMD_ST_GetEmailList_rsp* LogicMail::getEmailListData()
{
	return &m_stMailList;
}