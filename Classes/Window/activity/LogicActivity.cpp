#include "LogicActivity.h"
#include "Player/Player.h"
#include "LanguageConfig.h"
#include "WindowActivity.h"


LogicActivity::LogicActivity()
{
	_registerMessageHandler();

	//ע�����������Ϣ
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_GRABRED_rsp, bind(&LogicActivity::onGrabRed, this, placeholders::_1, placeholders::_2));
	
}

LogicActivity::~LogicActivity()
{
}

void LogicActivity::_registerMessageHandler()
{
}


//�������������
void LogicActivity::requestGrabRed()
{
	CMD_ST_FuDaiReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.optype = 1;			//����������
	req.money = 0;
	req.gold = 0;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_GRABRED_REQ, packet);
}

//���������ͬ������
void LogicActivity::onGrabRed(const PacketAide& packet, int nLen)
{
	m_qfd.fromPack(packet);
	if (m_qfd.ret == 0)
	{
		HallGameScene::getInstance()->updateWindow(EWT_Hall, 1);	//֪ͨ��ʼ������
	}
	else
	{
		HallGameScene::getInstance()->updateWindow(EWT_Hall, 2);	//֪ͨ����������
	}
}

//�������������
void LogicActivity::sendGrabRedData(int score, int yuanbao)
{
	
	CMD_ST_FuDaiReq req;
	req.userid = Player::getInstance()->getLogonData().dwUserID;
	req.optype = 2;												//������ύ����
	req.gold = score;
	req.money = yuanbao;

	PACKET_AIDE_SIZE_NEW(16);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_GRABRED_REQ, packet);

}


