#include "SharkNetworkKernel.h"
#include "shark/UI/GameScene/SharkGameScene.h"
#include "shark/Module/SharkNotice.h"
#include "LanguageConfig.h"
SharkNetWorkKernel* SharkNetWorkKernel::m_instance = nullptr;
SharkNetWorkKernel::SharkNetWorkKernel() : CSocketMission()
{
}

SharkNetWorkKernel::SharkNetWorkKernel(const std::string& url, int port) : CSocketMission(url, port)
{
	
}

SharkNetWorkKernel::~SharkNetWorkKernel()
{

}

void SharkNetWorkKernel::relese()
{
	if (nullptr == m_instance) return;

	stop();
	delete m_instance;
	m_instance = nullptr;

}

SharkNetWorkKernel* SharkNetWorkKernel::getInstance()
{
	if (m_instance == nullptr)
	{
		//m_instance = new NetworkKernel("120.26.6.159", 9501);
		m_instance = new SharkNetWorkKernel();
	}
	return m_instance;
}

//�����Ϸ�����
void SharkNetWorkKernel::onEventTCPSocketLink()
{
}

//�����˷���
void SharkNetWorkKernel::onEventTCPSocketShut()
{
}

//���ӳ���
void SharkNetWorkKernel::onEventTCPSocketError(int errorCode)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��Ϣ�ܻ�
bool SharkNetWorkKernel::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	log("--------NetworkKernel:Main:%d,Sub:%d,dataSize:%d--------", main, sub, dataSize);
	switch (main)
	{
		//��½����1
	case MDM_GR_LOGON_SHARK: return onSubCmdLogon(sub, data, dataSize);
		//��������2
	case MDM_GR_CONFIG_SHARK: return onSubCmdConfig(sub, data, dataSize);
		//�û���Ϣ3
	case MDM_GR_USER_SHARK: return onSunCmdUserInfo(sub, data, dataSize);
		//��Ϸ����200
	case MDM_GF_GAME_SHARK: return onSubCmdGame(sub, data, dataSize);
		//״̬����4
	case MDM_GR_STATUS_SHARK: return onSubCmdState(sub, data, dataSize);
		//ϵͳ����
	case MDM_CM_SYSTEM_SHARK: return onSubCmdSystem(sub, data, dataSize);
	default:
		break;
	}

	return true;
}


//---------------------------��½���----------------------------//

//���͵�¼
bool SharkNetWorkKernel::SendLogonPacket()
{

	auto pInfo = Player::getInstance();

	PACKET_AIDE_SIZE(512);
	packet.write4Byte(DF::shared()->GetPlazaVersion());
	packet.write4Byte(DF::shared()->GetFrameVersion());
	packet.write4Byte(DF::shared()->GetGameVersion());
	packet.write4Byte(pInfo->getLogonData().dwUserID);
	packet.writeUTF16(u8_2(""), LEN_PASSWORD);									//��½��Ϸ������ ���� �� ���ַ���
	packet.writeUTF16(u8_2(DF::shared()->GetMachineID()), LEN_MACHINE_ID);
	packet.write2Byte(DF::shared()->GetGameKindID());

	//��������
	SharkNetWorkKernel::getInstance()->send(MDM_GR_LOGON_SHARK, SUB_GR_LOGON_USERID_SHARK, packet.getBuffer(), packet.getPosition());

	return true;
}

//��½���������ȡ
bool SharkNetWorkKernel::onSubCmdLogon(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//��¼�ɹ�
	case SUB_GR_LOGON_SUCCESS_SHARK:	return onSocketSubLogonSuccess(data, dataSize);
		//��¼ʧ��
	case SUB_GR_LOGON_FAILURE_SHARK:	return onSocketSubLogonFailure(data, dataSize);
		//��¼���
	case SUB_GR_LOGON_FINISH_SHARK:	return onSocketSubLogonFinish(data, dataSize);
		//������ʾ
	case SUB_GR_UPDATE_NOTIFY_SHARK:	return onSocketSubUpdateNotify(data, dataSize);
	}

	return true;
}

//��¼�ɹ�
bool SharkNetWorkKernel::onSocketSubLogonSuccess(void* data, int dataSize)
{

	//PACKET_AIDE_DATA(data);

	//�������
	//mUserAttribute.dwUserRight = packet.read4Byte();
	//mUserAttribute.dwMasterRight = packet.read4Byte();

	//if (SHZCUserRight::IsGameCheatUser(mUserAttribute.dwUserRight))
	//{

	//	PLAZZ_PRINTF("%s=%d", a_u8("�û�Ȩ��"), mUserAttribute.dwUserRight);
	//}
	//else
	//{
	//	PLAZZ_PRINTF("%s=%d", a_u8("�û�û��Ȩ��"), mUserAttribute.dwUserRight);
	//}

	//����״̬
	//SetServiceStatus(SHZ_ServiceStatus_RecvInfo);

	return true;

}

//��¼ʧ��
bool SharkNetWorkKernel::onSocketSubLogonFailure(void* data, int dataSize)
{
	//Ч������
	ASSERT(dataSize >= 4);
	if (dataSize < 4) return false;

	PACKET_AIDE_DATA(data);
	int lErrorCode = packet.read4Byte();
	int len = (dataSize - 4) / 2;
	u2string str;
	str.resize(len + 1, '\0');
	packet.readUTF16(&str[0], len);
	std::string sString = u2_8(&str[0]);

	log("%s:%s",a_u8("OnSocketSubLogonFailure ��¼ʧ��:"), sString.c_str());

	SharkNotice::openNotice(sString.c_str());
	//�ر�����
	SharkNetWorkKernel::getInstance()->stop();

	return true;
}

//��¼���
bool SharkNetWorkKernel::onSocketSubLogonFinish(void* data, int dataSize)
{
	//���Ϳ�������
	SendSitDownPacket();

	//���Ϳ������
	SendGameOption();

	//ͬ��������Ϣ
	SendGameRoomInfo();

	//����״̬
	//SetServiceStatus(SHZ_ServiceStatus_ServiceIng);

	//mUserAttribute.dwUserID = mMeUserItem->GetUserID();
	//mUserAttribute.wChairID = INVALID_CHAIR;
	//mUserAttribute.wTableID = INVALID_TABLE;

	////�����ж�
	//if (SHZCServerRule::IsForfendGameRule(mServerAttribute.dwServerRule) == false)
	//{
	//	//���͹���
	//	SendUserRulePacket();
	//}

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRLogonFinish();
	return true;
}

//�汾������ʾ
bool SharkNetWorkKernel::onSocketSubUpdateNotify(void* data, int dataSize)
{
	//PACKET_AIDE_DATA(data);
	////������־
	//byte cbMustUpdatePlaza = packet.readByte();
	//byte cbMustUpdateClient = packet.readByte();
	//byte cbAdviceUpdateClient = packet.readByte();

	////��ǰ�汾
	//dword dwCurrentPlazaVersion = packet.read4Byte();
	//dword dwCurrentFrameVersion = packet.read4Byte();
	//dword dwCurrentClientVersion = packet.read4Byte();

	////��������
	//if (cbMustUpdatePlaza == TRUE)
	//{
	//	IntermitConnect(true);

	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGRUpdateNotify(1, a_u8("��Ϸ�㳡�Ѹ���"));
	//	return true;
	//}

	////��Ϸ����
	//if (cbMustUpdateClient == TRUE)
	//{
	//	IntermitConnect(true);

	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGRUpdateNotify(1, a_u8("��Ϸ�Ѹ���"));
	//	return true;
	//}

	////������ʾ
	//if (cbAdviceUpdateClient != TRUE)
	//	return true;

	////������ʾ
	//char szDescribe[512] = { 0 };
	//sprintf(szDescribe, a_u8("��%s���Ѿ������ˣ�����ǿ�ҽ��������и��£����ڽ��и�����"), mGameKind.szKindName);

	//IntermitConnect(true);

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRUpdateNotify(0, szDescribe);

	return true;

}


//---------------------------������Ϣ���----------------------------//

//�������������ȡ
bool SharkNetWorkKernel::onSubCmdConfig(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//�б�����
	case SUB_GR_CONFIG_COLUMN_SHARK:	return onSocketSubConfigColumu(data, dataSize);
		//��������
	case SUB_GR_CONFIG_SERVER_SHARK:	return onSocketSubConfigRoom(data, dataSize);
		//��������
	case SUB_GR_CONFIG_PROPERTY_SHARK: return onSocketSubConfigProperty(data, dataSize);
		//�������
	case SUB_GR_CONFIG_FINISH_SHARK:	return onSocketSubConfigFinish(data, dataSize);
		//���Ȩ��
	case SUB_GR_CONFIG_USER_RIGHT_SHARK:	return onSocketSubConfigUserRight(data, dataSize);
	default:
		break;
	}
	return true;
}

//�б�����
bool SharkNetWorkKernel::onSocketSubConfigColumu(void* data, int dataSize)
{
	//��������
	PACKET_AIDE_DATA(data);
	byte cbColumnCount = packet.readByte();
	return true;
}

//��������
bool SharkNetWorkKernel::onSocketSubConfigRoom(void* data, int dataSize)
{
	//Ч������
	ASSERT(dataSize == 10);
	if (dataSize < 10) return false;

	PACKET_AIDE_DATA(data);

	auto gameInfo = GameInfo::GetInstance();
	SharkTableInfo info = gameInfo->gettableInfor();

	//��������
	info.wTableCount = packet.read2Byte();
	info.wChairCount = packet.read2Byte();
	info.wServerType = packet.read2Byte();
	info.dwServerRule = packet.read4Byte();

	gameInfo->settableInfor(info);

	//m_UserListControl.SetServerRule(m_dwServerRule);

	/*if (!mTableViewFrame.ConfigTableFrame(
		mServerAttribute.wTableCount,
		mServerAttribute.wChairCount,
		mServerAttribute.dwServerRule,
		mServerAttribute.wServerType,
		mServerAttribute.wServerID))
		{
		IntermitConnect(false);
		return false;
		}*/

	return true;
}

//��������
bool SharkNetWorkKernel::onSocketSubConfigProperty(void* data, int dataSize)
{
	return true;
}

//�������
bool SharkNetWorkKernel::onSocketSubConfigFinish(void* data, int dataSize)
{
	return true;
}

//���Ȩ��
bool SharkNetWorkKernel::onSocketSubConfigUserRight(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	//���Ȩ��
	dword dwUserRight = packet.read4Byte();
	return true;
}


//---------------------------�û���Ϣ���3----------------------------//


//���Ϳ�������
bool SharkNetWorkKernel::SendSitDownPacket()
{
	return send(MDM_GR_USER_SHARK, SUB_GR_USER_CHAIR_REQ_SHARK);
}

//��������
bool SharkNetWorkKernel::SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave)
{
	PACKET_AIDE_SIZE(10);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);
	packet.writeByte(cbForceLeave);					//1 ǿ���뿪

	//��������
	send(MDM_GR_USER_SHARK, SUB_GR_USER_STANDUP_SHARK, packet.getBuffer(), packet.getPosition());

	return true;
}

//�û� ��Ϣ�����ȡ
bool SharkNetWorkKernel::onSunCmdUserInfo(int sub, void* data, int dataSize)
{
	switch (sub)
	{
	case SUB_GR_USER_ENTER_SHARK: return onSocketSubUserEnter(data, dataSize);
	case SUB_GR_USER_STATUS_SHARK: return onSocketSubUserStatus(data, dataSize);
	case SUB_GR_USER_ERROR_CODE: return onSocketSubErrorCode(data, dataSize);
	default:
		break;
	}
	return true;
}

//�û�״̬
bool SharkNetWorkKernel::onSocketSubUserStatus(void* data, int dataSize)
{


	SharkUserInfo info = GameInfo::GetInstance()->getuserInfor();

	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	if (info.dwUserID != dwUserID) return true;

	info.wTableID = packet.read2Byte();
	info.wChairID = packet.read2Byte();
	info.cbUserStatus = packet.readByte();

	GameInfo::GetInstance()->setuserInfor(info);

	return true;
}

//�û�����
bool SharkNetWorkKernel::onSocketSubUserEnter(void* data, int dataSize)
{
	//�Ѿ���ȡ���Լ�����Ϣ�ˣ����ٶ�ȡ������ҽ������Ϣ��������ܣ�

	log("---------GameInfo = %d----------", GameInfo::GetInstance()->getuserInfor().dwUserID);
	if (GameInfo::GetInstance()->getuserInfor().dwUserID != _ZERO) return true;
	//��������
	SharkUserInfo UserInfo;
	zeromemory(&UserInfo, sizeof(UserInfo));

	PACKET_AIDE_DATA(data);
	//�û�����
	UserInfo.dwGameID = packet.read4Byte();
	UserInfo.dwUserID = packet.read4Byte();
	UserInfo.dwGroupID = packet.read4Byte();
	UserInfo.wFaceID = packet.read2Byte();
	UserInfo.dwCustomID = packet.read4Byte();

	//�û�����
	UserInfo.cbGender = packet.readByte();
	UserInfo.cbMemberOrder = packet.readByte();
	UserInfo.cbMasterOrder = packet.readByte();

	//�û�״̬
	UserInfo.wTableID = packet.read2Byte();
	UserInfo.wChairID = packet.read2Byte();
	UserInfo.cbUserStatus = packet.readByte();

	//�û�����
	UserInfo.lScore = packet.read8Byte();
	packet.rSCORE();			//Ԫ����Ϣ
	packet.rSCORE();			//ˮ䰾���
	UserInfo.lGrade = packet.read8Byte();
	UserInfo.lInsure = packet.read8Byte();

	UserInfo.dwWinCount = packet.read4Byte();
	UserInfo.dwLostCount = packet.read4Byte();
	UserInfo.dwDrawCount = packet.read4Byte();
	UserInfo.dwFleeCount = packet.read4Byte();
	UserInfo.dwUserMedal = packet.read4Byte();
	UserInfo.dwExperience = packet.read4Byte();
	UserInfo.lLoveLiness = packet.read4Byte();

	GameInfo::GetInstance()->setuserInfor(UserInfo);

	return true;
}

//������Ϣ
bool SharkNetWorkKernel::onSocketSubErrorCode(void* data, int dataSize)
{

	PACKET_AIDE_DATA(data);
	int lErrorCode = packet.read4Byte();

	int len = (dataSize - 4) / 2;
	u2string str;
	str.resize(len + 1, '\0');
	packet.readUTF16(&str[0], len);
	std::string sString = u2_8(&str[0]);

	log("--------------error code:,%d,error info:%s--------------", lErrorCode, sString.c_str());

	return true;
}

//---------------------------ϵͳ�������1000----------------------------//

//ϵͳ���������ȡ
bool SharkNetWorkKernel::onSubCmdSystem(int sub, void* data, int dataSize)
{
	switch (sub)
	{

	case SUB_CM_SYSTEM_MESSAGE_SHARK: return onSocketSubSystemMessage(data, dataSize);
	default:
		break;
	}

	return true;
}


bool SharkNetWorkKernel::onSocketSubSystemMessage(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	word wType = packet.read2Byte();
	word wLength = packet.read2Byte();

	int len = (dataSize - 4) / 2;
	u2string str;
	str.resize(len + 1, '\0');
	packet.readUTF16(&str[0], len);

	const std::string sString = u2_8(&str[0]);

	log("--------------%s:%s--------------",a_u8("NetworkKernel::NetworkKernel\n"), sString.c_str());

	//�ر���Ϸ
	if (wType & (SMT_CLOSE_ROOM_SHARK | SMT_CLOSE_LINK_SHARK | SMT_CLOSE_GAME_SHARK))
	{
		log("%s",a_u8("----------------�ر���Ϸ----------------"));

		SharkNotice::openNotice(GETSTRING(1400), [](Ref*)
		{
			SharkGameScene::m_mainView->menuManager.goBack(1);
		});
	}

	return true;
}


//---------------------------״̬�������----------------------------//

//���ͽ��볡��
bool SharkNetWorkKernel::SendGameOption()
{

	//��������
	PACKET_AIDE_SIZE(512);
	packet.writeByte(FALSE);									//�Ƿ������Թ� (FALSEΪ������)
	packet.write4Byte(DF::shared()->GetFrameVersion());
	packet.write4Byte(DF::shared()->GetGameVersion());

	//��������
	return send(MDM_GF_FRAME_SHARK, SUB_GF_GAME_OPTION_SHARK, packet.getBuffer(), packet.getPosition());
}

//״̬���������ȡ
bool SharkNetWorkKernel::onSubCmdState(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//������Ϣ
	case SUB_GR_TABLE_INFO_SHARK: return onSocketSubStatusTableInfo(data, dataSize);
		//����״̬
	case SUB_GR_TABLE_STATUS_SHARK: return onSocketSubStatusTableStatus(data, dataSize);
	default:
		break;
	}

	return true;
}

//������Ϣ���ᷢ��������ҵ���Ϣ������
bool SharkNetWorkKernel::onSocketSubStatusTableInfo(void* data, int dataSize)
{

	//��������
	PACKET_AIDE_DATA(data);
	word wTableCount = packet.read2Byte();

	//for (int i = 0; i < wTableCount; i++)
	//{
	//	byte cbTableLock = packet.readByte();
	//	byte cbPlayStatus = packet.readByte();
	//	//mTableViewFrame.SetTableStatus(i, (cbPlayStatus == TRUE), (cbTableLock == TRUE));
	//}

	return true;
}

//����״̬
bool SharkNetWorkKernel::onSocketSubStatusTableStatus(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	SharkTableInfo info;
	info.wTableID = packet.read2Byte();
	info.bTableLock = packet.readByte();
	info.bPlayStatus = packet.readByte();
	GameInfo::GetInstance()->settableInfor(info);

	return true;

}

//---------------------------��Ϸ�߼����----------------------------//

//����������
bool SharkNetWorkKernel::SendHeartPak()
{
	send(MDM_GF_GAME_SHARK, SUB_G_GAMETICKTIME);//����
	return true;
}

//����������
bool SharkNetWorkKernel::onSocketSubHeartPak(void* data, int dataSize)
{
	if (SharkGameScene::This != nullptr)
	{
		SharkGameScene::This->OnHreatPak();
	}
	return true;
}
//���󷿼���Ϣ
bool SharkNetWorkKernel::SendGameRoomInfo()
{
	send(MDM_GF_GAME_SHARK, SUB_G_INTOGAMEINGREQ);
	return true;
}

//�����Լ���ѹ����Ϣ
bool SharkNetWorkKernel::SendJetInfo(word index, SCORE jet)
{
	//У�����
	//CC_ASSERT(index >= _ZERO && index < 12);
	//if (index < _ZERO || index >= 12) return false;

	
	CMD_GP_PUTNOTESREQU jetInfo;
	jetInfo.AmimalIndex = index;
	jetInfo.AmimalGold = jet;



	send(MDM_GF_GAME_SHARK, SUB_G_OPERATIONNOTES, &jetInfo, sizeof(CMD_GP_PUTNOTESREQU));
	return true;
}

//�������������ѹ����
bool SharkNetWorkKernel::SendAllJetInfo()
{
	return true;
}

//��Ϸ���������ȡ
bool SharkNetWorkKernel::onSubCmdGame(int sub, void* data, int dataSize)
{

	switch (sub)
	{
	case SUB_G_INTOGAMEINGRES: return onSocketSubTableInfo(data, dataSize);
	case SUB_G_GAMESENLINSTART: return onSocketSubOpenRlt(data, dataSize);
	case SUB_G_GAMESNEXTSTART: return  onSocketSubStartGame(data, dataSize);
	case SUB_G_GAMECAIJINGRES: return onSocketSubOpenCJRlt(data, dataSize);
	case SUB_G_GAMEUSERCHANGE: return onSocketSubJoinOrExit(data, dataSize);
	case SUB_G_GAMESENLINSTARTBORD: return onSocketSubOtherUserJet(data, dataSize);
	case SUB_G_GAMESTARTGAMELIST: return onSocketSubUpdateUserList(data, dataSize);
	case SUB_G_GAMECHEARNOTEALL: return onSocketSubOtherCancelJet(data, dataSize);
	case SUB_G_GAMETICKTIMERSP: return onSocketSubHeartPak(data, dataSize);
	default:
		break;
	}
	return true;
}

//�����������ѹ����Ϣ
bool SharkNetWorkKernel::onSocketSubAllJetInfoRlt(void* data, int dataSize)
{
	return true;
}

//SHARK��Ϸ��������ͬ��
bool SharkNetWorkKernel::onSocketSubTableInfo(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	//���õ�ǰ������Ϣ

	
	auto gameInfo = GameInfo::GetInstance();
	RoomInfo_Shark* roominfo = gameInfo->getroomInfor();

	//����˷���12����¼
	int res[12];
	for (size_t i = 0; i <12; i++)
	{
		int animalNumber = packet.read4Byte();
		int animalIndex = packet.read4Byte();
		res[i] = animalNumber;
		
	}
	gameInfo->setResRecord(res);
	roominfo->JsRate = packet.read4Byte();              //���豶��
	gameInfo->setserverState((gameState)packet.read4Byte());		//��Ϸ״̬

	log("-------------%s:%d-----------", a_u8("ͬ��״̬:"), gameInfo->getserverState());

	//���õ�һ����Ϸ��Ϣ
	gameInfo->setLeftTime(packet.read4Byte());	

	
	roominfo->dwRateID = packet.read4Byte();          //��������

	//��ȡ�ʽ�
	roominfo->lCJ = packet.read8Byte();
	log("-------CJ:%d--------------", roominfo->lCJ);

	//��ȡ�����������
	roominfo->dwOnline = packet.read4Byte();
	log("-------online:%d--------------", roominfo->dwOnline);

	//��ȡ���������Ϣ
	for (size_t i = 0; i < roominfo->dwOnline; i++)
	{
		UserInfo_Shark info;
		info.userid = packet.read4Byte();


		u2string str;
		str.resize(LEN_NICKNAME + 1, '\0');
		packet.readUTF16(&str[0], LEN_NICKNAME);
		memcpy(info.nickname, u2_8(&str[0]), LEN_NICKNAME);

		info.viplevel = packet.read4Byte();
		info.gold = packet.read4Byte();
		info.headimgid = packet.read4Byte();
		info.winscore = packet.read8Byte();
		//info.jet = packet.read8Byte();
		//info.jet = _ZERO;
		roominfo->vec.push_back(info);

		log("-------ID:%d ,name:%s, gold:%d ,vip:%d , head:%d, win:%ld---------------", info.userid, info.nickname, info.gold, info.viplevel, info.headimgid, info.winscore);

		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{
			int coinScore = gameInfo->getchangeScore();
			//int yuanBaoScore = gameinfo->getchangeYuanBao();
			coinScore = info.gold;
			gameInfo->setchangeScore(coinScore);
		}
	}	


	//��Ϸ׼�����
	GameInfo::GetInstance()->setstartGameFinish(true);
	return true;
	
}

//�������ѹ��
bool SharkNetWorkKernel::onSocketSubOtherUserJet(void* data, int dataSize)
{
	CC_ASSERT(sizeof(CMD_GP_PUTNOTESREC) == dataSize);
	if (sizeof(CMD_GP_PUTNOTESREC) != dataSize) return false;

	CMD_GP_PUTNOTESREC* p = (CMD_GP_PUTNOTESREC*)data;
	if (SharkGameScene::m_mainView != nullptr)
	{
		log("-----------------%s----------------", a_u8("�������������Ϣ"));
		log("-------------totalCoin = %d------------------",p->gold);
		log("-------------playerCoin = %ld------------------", p->currgold);
		log("-------------playerID = %d-----------",p->userid);
		auto gameInfo = GameInfo::GetInstance();
		RoomInfo_Shark* roominfo = gameInfo->getroomInfor();
		int index = p->AnimalIndex;
		if (index == 1000)
		{
			index = 0;
		}
		else if (index == 2000)
		{
			index = 3;
		}
		roominfo->alljetinfo.lTotelBet[index] += p->gold;
		roominfo->alljetinfo.lAllBet += p->gold;

		if (Player::getInstance()->getLogonData().dwUserID == p->userid)
		{
			log("----------index = %d---------------", index);
			SharkGameScene::m_mainView->stakeManager.successStake(p->gold, p->currgold, index);
		}

		SharkGameScene::m_mainView->m_playerListManager.otherJet(p->userid, p->gold, p->currgold, index);
	}
	return true;

}

//����ˢ������б�
bool SharkNetWorkKernel::onSocketSubUpdateUserList(void* data, int datasize)
{
 	if (SharkGameScene::m_mainView == nullptr) return true;
	PACKET_AIDE_DATA(data);
	dword online = packet.read4Byte();

	auto gameinfo = GameInfo::GetInstance();
	auto roomInfo = gameinfo->getroomInfor();
	vector<UserInfo_Shark> vec;
	for (size_t i = 0; i < online; i++)
	{
		UserInfo_Shark info;
		info.userid = packet.read4Byte();

		u2string str;
		str.resize(LEN_NICKNAME + 1, '\0');
		packet.readUTF16(&str[0], LEN_NICKNAME);
		memcpy(info.nickname, u2_8(&str[0]), LEN_NICKNAME);

		info.viplevel = packet.read4Byte();
		info.gold = packet.read4Byte();
		info.headimgid = packet.read4Byte();
		info.winscore = packet.read8Byte();
		info.jet = _ZERO;
		vec.push_back(info);
		
		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{
			int coinScore = gameinfo->getchangeScore();

			coinScore = info.gold;
			gameinfo->setchangeScore(coinScore);
		}
	}

	roomInfo->vec = vec;
	return true;

}


//��Ҽ�����뿪
bool SharkNetWorkKernel::onSocketSubJoinOrExit(void* data, int dataSize)
{

	PACKET_AIDE_DATA(data);

	auto gameInfo = GameInfo::GetInstance();

	UserInfo_Shark info;
	info.userid = packet.read4Byte();

	u2string str;
	str.resize(LEN_NICKNAME + 1, '\0');
	packet.readUTF16(&str[0], LEN_NICKNAME);
	memcpy(info.nickname, u2_8(&str[0]), LEN_NICKNAME);

	info.viplevel = packet.read4Byte();
	info.gold = packet.read4Byte();
	info.headimgid = packet.read4Byte();
	info.winscore = packet.read8Byte();

	int state = packet.read4Byte();

	RoomInfo_Shark *roomInfo = gameInfo->getroomInfor();
	if (state == 0)
	{
		//����뿪
		roomInfo->dwOnline--;

		auto roominfo = gameInfo->getroomInfor();
		
		if (SharkGameScene::m_mainView != nullptr)
		{
			SharkGameScene::m_mainView->m_playerListManager.deleteUserElement(info.userid);
		}
		for (vector<UserInfo_Shark>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
		{
			if ((*itor).userid == info.userid)
			{
				roominfo->vec.erase(itor);
				break;
			}
		}

		
	}
	else if (state == 1)
	{
		//��ҽ���
		if (info.userid != GameInfo::GetInstance()->getuserInfor().dwUserID)
		{
			//��һ�ν�����Ϸ�ᴥ��
			roomInfo->dwOnline++;
		}
		if (SharkGameScene::m_mainView != nullptr)
		{
			gameInfo->getroomInfor()->vec.push_back(info);
			//if (GameInfo::GetInstance()->getserverState() == STAKE)
				SharkGameScene::m_mainView->m_playerListManager.InitPlayerList();
			//else
				//SharkGameScene::m_mainView->m_playerListManager.addPlayerElement(info);
		}

	}

	return true;

}

//���ؿ������
bool SharkNetWorkKernel::onSocketSubOpenRlt(void* data, int dataSize)
{

	CC_ASSERT(dataSize == sizeof(CMD_GP_StartGameRandRsp));
	if (dataSize != sizeof(CMD_GP_StartGameRandRsp)) return false;

	CMD_GP_StartGameRandRsp* gameRlt = (CMD_GP_StartGameRandRsp*)data;
	auto gameInfo = GameInfo::GetInstance();

	OpenResult result;
	for (int i = 0; i < 3; i++)
	{
		result.resultIndex[i] = gameRlt->AnimalIndex[i];
	}
	result.aType = gameRlt->Atype;
	result.aValue = gameRlt->Avalue;

	gameInfo->setopenRelt(result);	
	
	gameInfo->setserverState(gameState::BATTLE);
	return true;
}

//��ʼ��һ����Ϸ
//��ʼ��Ϸ(ѹ��)
bool SharkNetWorkKernel::onSocketSubStartGame(void* data, int dataSize)
{

	CC_ASSERT(dataSize == sizeof(CMD_GP_StartNextGame));
	if (dataSize != sizeof(CMD_GP_StartNextGame)) return false;
	CMD_GP_StartNextGame* info = (CMD_GP_StartNextGame*)data;	


	auto gameInfo = GameInfo::GetInstance();
	RoomInfo_Shark* roominfo = gameInfo->getroomInfor();
	roominfo->JsRate = info->JsRate;
	roominfo->dwRateID = info->RateID;
	roominfo->lCJ = info->CaiJinValue;
	//����״̬Ϊ��ʼ��Ϸ
	gameInfo->setserverState(gameState::STAKE);

	
	//����һ�ֵ�Ѻ������Ϣ����
	roominfo->alljetinfo.lAllBet = 0;
	for (int i = 0; i < 12; i++)
	{
		roominfo->alljetinfo.lTotelBet[i] = 0;
	}



	return true;
}

//�����ȡ����ѹ��
bool SharkNetWorkKernel::onSocketSubOtherCancelJet(void* data, int dataSize)
{
	CC_ASSERT(dataSize == sizeof(CMD_GP_CharNotesInfor));
	if (dataSize != sizeof(CMD_GP_CharNotesInfor)) return false;

	CMD_GP_CharNotesInfor* info = (CMD_GP_CharNotesInfor*)data;
	//��������ѹ��
	if (SharkGameScene::m_mainView != nullptr)
	{
		auto gameInfo = GameInfo::GetInstance();
		RoomInfo_Shark* roominfo = gameInfo->getroomInfor();
		SCORE sum = _ZERO;
		for (size_t i = _ZERO; i < info->size; i++)
		{
			int index = info->notes[i].noteid;
			if (index == 1000)
			{
				index = 0;
			}
			else if(index == 2000)
			{
				index = 3;
			}
			roominfo->alljetinfo.lTotelBet[index] -= info->notes[i].gold;
			sum += info->notes[i].gold;
			log("-------------------cancel jet :%d----------------------", info->notes[i].noteid);
		}

		roominfo->alljetinfo.lAllBet -= sum;

		//�Լ�ȡ��ѹ�ֳɹ� ֪ͨˢ��UI
		if (info->userid == Player::getInstance()->getLogonData().dwUserID)
		{
			SharkGameScene::m_mainView->stakeManager.successCancelStake();
		}

		SharkGameScene::m_mainView->m_playerListManager.othercancelJet(info->userid, sum);
	}
	return true;

}

//CJ�ʽ𿪽���Ϣ
bool SharkNetWorkKernel::onSocketSubOpenCJRlt(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	auto gameInfo = GameInfo::GetInstance();
	SharkCJ sharkCaiJin = gameInfo->getsharkCj();

	//�м�������н�
	sharkCaiJin.num = packet.read4Byte();
	//��ȡ3�������Ϣ
	for (int i = 0; i < sharkCaiJin.num; i++)
	{
		sharkCaiJin.userid[i] = packet.read4Byte();

		u2string str;
		str.resize(LEN_NICKNAME + 1, '\0');
		packet.readUTF16(&str[0], LEN_NICKNAME);
		memcpy(sharkCaiJin.name[i], u2_8(&str[0]), LEN_NICKNAME);

		int	viplevel = packet.read4Byte();
		sharkCaiJin.score[i] = packet.read4Byte();
		int	headimgid = packet.read4Byte();
		SCORE winscore = packet.read8Byte();

		log("--------CJ:---name:%s , gold:%d--------", sharkCaiJin.name[i], sharkCaiJin.score[i]);
	}

	 gameInfo->setsharkCj(sharkCaiJin);

		//����״̬Ϊ����
	gameInfo->setserverState(gameState::BATTLE);


	return true;
}

