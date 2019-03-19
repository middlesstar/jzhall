#include "NetworkKernel.h"
#include "slwh/Main-Script/SLWHGameScene.h"
#include "slwh/Module/SLWHNotice.h"
#include "LanguageConfig.h"
NetworkKernel* NetworkKernel::m_instance = nullptr;
NetworkKernel::NetworkKernel() : CSocketMission()
{
}

NetworkKernel::NetworkKernel(const std::string& url, int port) : CSocketMission(url, port)
{
}

NetworkKernel::~NetworkKernel()
{
}

void NetworkKernel::relese()
{
	if (nullptr == m_instance) return;

	stop();
	delete m_instance;
	m_instance = nullptr;

}

NetworkKernel* NetworkKernel::getInstance()
{
	if (m_instance == nullptr)
	{
		//m_instance = new NetworkKernel("120.26.6.159", 9501);
		m_instance = new NetworkKernel();
	}
	return m_instance;
}

//�����Ϸ�����
void NetworkKernel::onEventTCPSocketLink()
{
	log("NetworkKernel::onEventTCPSocketLink");
}

//�����˷���
void NetworkKernel::onEventTCPSocketShut()
{
	log("NetworkKernel::onEventTCPSocketShut");
}

//���ӳ���
void NetworkKernel::onEventTCPSocketError(int errorCode)
{
	log("NetworkKernel::onEventTCPSocketError");

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��Ϣ�ܻ�
bool NetworkKernel::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	log("--------NetworkKernel:Main:%d,Sub:%d,dataSize:%d--------", main, sub, dataSize);
	switch (main)
	{
		//��½����1
	case MDM_GR_LOGON_SLWH: return onSubCmdLogon(sub, data, dataSize);
		//��������2
	case MDM_GR_CONFIG_SLWH: return onSubCmdConfig(sub, data, dataSize);
		//�û���Ϣ3
	case MDM_GR_USER_SLWH: return onSunCmdUserInfo(sub, data, dataSize);
		//��Ϸ����200
	case MDM_GF_GAME_SLWH: return onSubCmdGame(sub, data, dataSize);
		//״̬����4
	case MDM_GR_STATUS_SLWH: return onSubCmdState(sub, data, dataSize);
		//ϵͳ����1000
	case MDM_CM_SYSTEM_SLWH: return onSubCmdSystem(sub, data, dataSize);
	default:
		break;
	}

	return true;
}


//---------------------------��½���----------------------------//

//���͵�¼
bool NetworkKernel::SendLogonPacket()
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
	NetworkKernel::getInstance()->send(MDM_GR_LOGON_SLWH, SUB_GR_LOGON_USERID_SLWH, packet.getBuffer(), packet.getPosition());

	return true;
}

//��½���������ȡ
bool NetworkKernel::onSubCmdLogon(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//��¼�ɹ�
	case SUB_GR_LOGON_SUCCESS_SLWH:	return onSocketSubLogonSuccess(data, dataSize);
		//��¼ʧ��
	case SUB_GR_LOGON_FAILURE_SLWH:	return onSocketSubLogonFailure(data, dataSize);
		//��¼���
	case SUB_GR_LOGON_FINISH_SLWH:	return onSocketSubLogonFinish(data, dataSize);
		//������ʾ
	case SUB_GR_UPDATE_NOTIFY_SLWH:	return onSocketSubUpdateNotify(data, dataSize);
	}

	return true;
}

//��¼�ɹ�
bool NetworkKernel::onSocketSubLogonSuccess(void* data, int dataSize)
{

	//PACKET_AIDE_DATA(data);
	log("----------------dais:onSocketSubLogonSuccess----------------------");
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
bool NetworkKernel::onSocketSubLogonFailure(void* data, int dataSize)
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

	SLWHNotice::openNotice(sString.c_str());

	//�ر�����
	NetworkKernel::getInstance()->stop();

	return true;
}

//��¼���
bool NetworkKernel::onSocketSubLogonFinish(void* data, int dataSize)
{
	log("----------------dais:onSocketSubLogonFinish----------------------");
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
bool NetworkKernel::onSocketSubUpdateNotify(void* data, int dataSize)
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
bool NetworkKernel::onSubCmdConfig(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//�б�����
	case SUB_GR_CONFIG_COLUMN_SLWH:	return onSocketSubConfigColumu(data, dataSize);
		//��������
	case SUB_GR_CONFIG_SERVER_SLWH:	return onSocketSubConfigRoom(data, dataSize);
		//��������
	case SUB_GR_CONFIG_PROPERTY_SLWH: return onSocketSubConfigProperty(data, dataSize);
		//�������
	case SUB_GR_CONFIG_FINISH_SLWH:	return onSocketSubConfigFinish(data, dataSize);
		//���Ȩ��
	case SUB_GR_CONFIG_USER_RIGHT_SLWH:	return onSocketSubConfigUserRight(data, dataSize);
	default:
		break;
	}
	return true;
}

//�б�����
bool NetworkKernel::onSocketSubConfigColumu(void* data, int dataSize)
{
	//��������
	PACKET_AIDE_DATA(data);
	byte cbColumnCount = packet.readByte();
	return true;
}

//��������
bool NetworkKernel::onSocketSubConfigRoom(void* data, int dataSize)
{
	log("----------------dais:onSocketSubConfigRoom----------------------");
	//Ч������
	ASSERT(dataSize == 10);
	if (dataSize < 10) return false;
	
	PACKET_AIDE_DATA(data);
	auto gameInfo = OneGameInfo::GetInstance();
	SLWHTableInfo info = gameInfo->getTableInfo();
	//��������
	info.wTableCount = packet.read2Byte();
	info.wChairCount = packet.read2Byte();
	info.wServerType = packet.read2Byte();
	info.dwServerRule = packet.read4Byte();
	gameInfo->setTableInfo(info);
	log("----------------dais:onSocketSubConfigRoom over----------------------");
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
bool NetworkKernel::onSocketSubConfigProperty(void* data, int dataSize)
{
	return true;
}

//�������
bool NetworkKernel::onSocketSubConfigFinish(void* data, int dataSize)
{
	return true;
}

//���Ȩ��
bool NetworkKernel::onSocketSubConfigUserRight(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	//���Ȩ��
	dword dwUserRight = packet.read4Byte();
	return true;
}


//---------------------------�û���Ϣ���3----------------------------//


//���Ϳ�������
bool NetworkKernel::SendSitDownPacket()
{
	log("-------dais:SendSitDownPacket---------");
	return send(MDM_GR_USER_SLWH, SUB_GR_USER_CHAIR_REQ_SLWH);
}

//��������
bool NetworkKernel::SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave)
{
	PACKET_AIDE_SIZE(10);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);
	packet.writeByte(cbForceLeave);					//1 ǿ���뿪

	//��������
	send(MDM_GR_USER_SLWH, SUB_GR_USER_STANDUP_SLWH, packet.getBuffer(), packet.getPosition());

	return true;
}

//�û���Ϣ�����ȡ
bool NetworkKernel::onSunCmdUserInfo(int sub, void* data, int dataSize)
{
	switch (sub)
	{
	case SUB_GR_USER_ENTER_SLWH: return onSocketSubUserEnter(data, dataSize);
	case SUB_GR_USER_STATUS_SLWH: return onSocketSubUserStatus(data, dataSize);
	case SUB_GR_USER_ERROR_CODE: return onSocketSubErrorCode(data, dataSize);
	default:
		break;
	}
	return true;
}

//�û�״̬
bool NetworkKernel::onSocketSubUserStatus(void* data, int dataSize)
{


	SLWHUserInfo info = OneGameInfo::GetInstance()->getUserInfo();

	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	if (info.dwUserID != dwUserID) return true;

	info.wTableID = packet.read2Byte();
	info.wChairID = packet.read2Byte();
	info.cbUserStatus = packet.readByte();

	OneGameInfo::GetInstance()->setUserInfo(info);

	return true;
}

//�û�����
bool NetworkKernel::onSocketSubUserEnter(void* data, int dataSize)
{
	//�Ѿ���ȡ���Լ�����Ϣ�ˣ����ٶ�ȡ������ҽ������Ϣ��������ܣ�
	
	if (OneGameInfo::GetInstance()->getUserInfo().dwUserID != _ZERO) return true;

	//��������
	SLWHUserInfo UserInfo;
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

	OneGameInfo::GetInstance()->setUserInfo(UserInfo);

	return true;
}

//������Ϣ
bool NetworkKernel::onSocketSubErrorCode(void* data, int dataSize)
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
bool NetworkKernel::onSubCmdSystem(int sub, void* data, int dataSize)
{
	switch (sub)
	{

	case SUB_CM_SYSTEM_MESSAGE_SLWH: return onSocketSubSystemMessage(data, dataSize);
	default:
		break;
	}

	return true;
}


bool NetworkKernel::onSocketSubSystemMessage(void* data, int dataSize)
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
	if (wType & (SMT_CLOSE_ROOM_SLWH | SMT_CLOSE_LINK_SLWH | SMT_CLOSE_GAME_SLWH))
	{
		log("%s",a_u8("----------------�ر���Ϸ----------------"));

		SLWHNotice::openNotice(GETSTRING(1400), [](Ref*)
		{
			SLWHGameScene::m_uiLayer->m_uiMenu.exit(1);
		});
	}
	


	
	return true;
}


//---------------------------״̬�������----------------------------//

//���ͽ��볡��
bool NetworkKernel::SendGameOption()
{

	log("---------dais:SendGameOption-------------");

	//��������
	PACKET_AIDE_SIZE(512);
	packet.writeByte(FALSE);									//�Ƿ������Թ� (FALSEΪ������)
	packet.write4Byte(DF::shared()->GetFrameVersion());
	packet.write4Byte(DF::shared()->GetGameVersion());

	//��������
	return send(MDM_GF_FRAME_SLWH, SUB_GF_GAME_OPTION_SLWH, packet.getBuffer(), packet.getPosition());
}

//״̬���������ȡ
bool NetworkKernel::onSubCmdState(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//������Ϣ
	case SUB_GR_TABLE_INFO_SLWH: return onSocketSubStatusTableInfo(data, dataSize);
		//����״̬
	case SUB_GR_TABLE_STATUS_SLWH: return onSocketSubStatusTableStatus(data, dataSize);
	default:
		break;
	}

	return true;
}

//������Ϣ���ᷢ��������ҵ���Ϣ������
bool NetworkKernel::onSocketSubStatusTableInfo(void* data, int dataSize)
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
bool NetworkKernel::onSocketSubStatusTableStatus(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	SLWHTableInfo info;
	info.wTableID = packet.read2Byte();
	info.bTableLock = packet.readByte();
	info.bPlayStatus = packet.readByte();
	OneGameInfo::GetInstance()->setTableInfo(info);


	return true;

}

//---------------------------��Ϸ�߼����----------------------------//

//����������
bool NetworkKernel::SendHeartPak()
{
	log("SLWH:NetworkKernel::SendHeartPak()");
	send(MDM_GF_GAME_SLWH, SUB_G_GAMETICKTIME);
	return true;
}

//���󷿼���Ϣ
bool NetworkKernel::SendGameRoomInfo()
{

	send(MDM_GF_GAME_SLWH, SUB_GR_INTOGAMEINGREQ);
	return true;
}

//�����Լ���ѹ����Ϣ
bool NetworkKernel::SendJetInfo(word index, longlong jet)
{
	//У�����
	CC_ASSERT(index >= _ZERO && index < 15);
	if (index < _ZERO || index >= 15) return false;

	
	CMD_GP_PUTNOTESREQ jetInfo;
	jetInfo.iAmimalIndex = index;
	jetInfo.iAmimalGold = jet;


	send(MDM_GF_GAME_SLWH, SUB_GR_OPERATIONNOTES, &jetInfo, sizeof(CMD_GP_PUTNOTESREQ));
	return true;
}

//��Ϸ���������ȡ
bool NetworkKernel::onSubCmdGame(int sub, void* data, int dataSize)
{

	switch (sub)
	{
	case SUB_GR_INTOGAMEINGRES:		return onSocketSubTableInfo(data, dataSize);
	case SUB_GR_GAMESENLINSTART:	return onSocketSubOpenRlt(data, dataSize);
	case SUB_G_GAMECAIJINGRES:		return onSocketSubOpenCJRlt(data, dataSize);
	case SUB_GR_GAMESNEXTSTART:		return onSocketSubStartGame(data, dataSize);
	case SUB_G_GAMESILIANFRES:		return onSocketSubOpen4LFRlt(data, dataSize);
	case SUB_G_GAMEDAMANGUANRES:	return onSocketSubOpenDMGRlt(data, dataSize);
	case SUB_G_GAMELUCHDRAEM:		return onSocketSubOpenXYCJRlt(data, dataSize);
	case SUB_G_GAMEUSERCHANGE:		return onSocketSubJoinOrExit(data, dataSize);
	case SUB_G_GAMESENLINSTARTBORD:	return onSocketSubOtherUserJet(data, dataSize);
	case SUB_G_GAMESTARTGAMELIST:	return onSocketSubUpdateUserList(data, dataSize);
	case SUB_G_GAMECHEARNOTEALL:	return onSocketSubOtherCancelJet(data, dataSize);
	case SUB_G_GAMETICKTIMERSP:		return onSocketSubHeartPak(data, dataSize);
	default: break;
	}
	return true;
}

//����������
bool NetworkKernel::onSocketSubHeartPak(void* data, int dataSize)
{
	if (SLWHGameScene::This != nullptr)
	{
		log("SLWH:NetworkKernel::onSocketSubHeartPak()");
		SLWHGameScene::This->OnHreatPak();
	}
	return true;
}

//SLWH��Ϸ��������ͬ��
bool NetworkKernel::onSocketSubTableInfo(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	auto gameInfo = OneGameInfo::GetInstance();

	gameInfo->setServerState((enumServerState)packet.read4Byte());		
	//gameInfo->getServerState() == _Jet ? gameInfo->setbCountScore(false) : gameInfo->setbCountScore(true);
	
	//���õ�һ����Ϸ��Ϣ
	gameInfo->setLeftTime(packet.read4Byte());

	log("-------------%s:%d,LeftTime:%d-----------", a_u8("ͬ��״̬:"), gameInfo->getServerState(), gameInfo->getLeftTime());


	//���õ�ǰ������Ϣ
	slwhRoomInfo* roomInfo = gameInfo->getroomInfo();
	for (int i = 0; i < 24; i++)
	{
		roomInfo->dwColorIndex[i] = packet.read4Byte();
	}
	//����˷���20����¼
	int res[20]{0};
	for (size_t i = 0; i < 20; i++)
	{
		int zhx = packet.read4Byte();
		packet.read4Byte();				//�ն�4�ֽ�����
		int animalindex = packet.read4Byte();
		int openSpe = packet.read4Byte();

		int color = animalindex / 4;
		int animal = animalindex % 4;
		res[i] = _RES_KEY(openSpe, color, animal) * 10 + zhx;
	}
	gameInfo->setResRecord(res);
	roomInfo->dwRateID = packet.read4Byte();
	//��ȡ�ʽ�
	roomInfo->lCj = packet.read8Byte();
	log("-------CJ:%d--------------", roomInfo->lCj);

	//��ȡ�����������
	roomInfo->dwOnline = packet.read4Byte();
	log("-------online:%d--------------", roomInfo->dwOnline);

	//��ȡ���������Ϣ
	for (size_t i = 0; i < roomInfo->dwOnline; i++)
	{
		slwhPlayerInfo info;
		info.userid = packet.read4Byte();
		
		u2string str;
		str.resize(LEN_NICKNAME + 1, '\0');
		packet.readUTF16(&str[0], LEN_NICKNAME);
		memcpy(info.nickname, u2_8(&str[0]), LEN_NICKNAME);
		
		info.viplevel = packet.read4Byte();
		info.gold = packet.read4Byte();
		info.headimgid = packet.read4Byte();
		info.winscore = packet.read8Byte();
		roomInfo->vec.push_back(info);

		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{
			int coinScore = gameInfo->getscoreChange();
			//int yuanBaoScore = gameinfo->getchangeYuanBao();
			coinScore = info.gold;
			gameInfo->setscoreChange(coinScore);
		}
		log("-------ID:%d ,name:%s, gold:%d ,vip:%d , head:%d, win:%ld---------------", info.userid, info.nickname, info.gold, info.viplevel, info.headimgid, info.winscore);
	}

	//gameInfo->setroomInfo(roomInfo);

	//��Ϸ׼�����
	OneGameInfo::GetInstance()->setStartGameFinish(true);

	return true;
}

//�����ѹ��
bool NetworkKernel::onSocketSubOtherUserJet(void* data, int dataSize)
{
	CC_ASSERT(sizeof(CMD_GP_PUTNOTESRSP) == dataSize);
	if (sizeof(CMD_GP_PUTNOTESRSP) != dataSize) return false;

	CMD_GP_PUTNOTESRSP* p = (CMD_GP_PUTNOTESRSP*)data;

	//long					gold;											//ѹ��
	//dword					userid;											//���ID
	//longlong				score;											//����
	//dword					animalIndex;									//��ѹ����
	log("------------dais:usersID:%d,jetgold:%d,noeindex:%d------------------",p->userid,p->gold,p->animalIndex);

	if (SLWHGameScene::m_uiLayer != nullptr)
	{
		auto gameInfo = OneGameInfo::GetInstance();
		slwhRoomInfo* roomInfo = gameInfo->getroomInfo();
		roomInfo->alljetinfo.lTotelBet[p->animalIndex] += p->gold;
		roomInfo->alljetinfo.lAllBet += p->gold;

		//���Լ�ѹ��
		if (Player::getInstance()->getLogonData().dwUserID == p->userid)
		{
			SLWHGameScene::m_uiLayer->m_myJetManager.successJet(p->gold, p->score, p->animalIndex);
		}
		//�������
		SLWHGameScene::m_uiLayer->m_userListManager.otherJet(p->userid,p->gold,p->score,p->animalIndex);
	}
	return true;
}

//����ˢ������б�
bool NetworkKernel::onSocketSubUpdateUserList(void* data, int dataSize)
{
	if (SLWHGameScene::m_uiLayer == nullptr) return true;
	PACKET_AIDE_DATA(data);
	dword online = packet.read4Byte();
	
	auto gameinfo = OneGameInfo::GetInstance();
	auto roomInfo = gameinfo->getroomInfo();
	vector<slwhPlayerInfo> vec;
	for (size_t i = 0; i < online; i++)
	{
		slwhPlayerInfo info;
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
			int coinScore = gameinfo->getscoreChange();
			//int yuanBaoScore = gameinfo->getchangeYuanBao();
			coinScore = info.gold;
			gameinfo->setscoreChange(coinScore);
		}
	}
	roomInfo->vec = vec;

	return true;
}

//��Ҽ�����뿪
bool NetworkKernel::onSocketSubJoinOrExit(void* data, int dataSize)
{

	PACKET_AIDE_DATA(data);

	auto gameInfo = OneGameInfo::GetInstance();
	
	slwhPlayerInfo info;
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
	slwhRoomInfo* roomInfo = gameInfo->getroomInfo();
	
	if (state == 0)
	{
		//����˳�
		log("-------onSocketSubJoinOrExit:EXIT_id:%d,-----------", info.userid);
		//gameInfo->deleteUserElement(info.userid);
		//roomInfo->dwOnline--;
		//gameInfo->setroomInfo(roomInfo);
		if (SLWHGameScene::m_uiLayer != nullptr)
		{
			SLWHGameScene::m_uiLayer->m_userListManager.deleteUserElement(info.userid);
		}
		auto roominfo = gameInfo->getroomInfo();
		for (vector<slwhPlayerInfo>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
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
		log("-------onSocketSubJoinOrExit:Join_id:%d-------------", info.userid);
		//gameInfo->addUserElement(info);
		//if (info.userid != OneGameInfo::GetInstance()->getUserInfo().dwUserID)
		//{
		//	//��һ�ν�����Ϸ�ᴥ��
		//	roomInfo->dwOnline++;
		//}
		//gameInfo->setroomInfo(roomInfo);
		if (SLWHGameScene::m_uiLayer != nullptr)
		{
			gameInfo->getroomInfo()->vec.push_back(info);

			//�����ѹ�ֽ׶μ�����Ϸ�����������  �����׶μ�����Ϸ���е���� 
			if (OneGameInfo::GetInstance()->getServerState() == _Jet)
				SLWHGameScene::m_uiLayer->m_userListManager.InitUserList();
			else
				SLWHGameScene::m_uiLayer->m_userListManager.addUserElement(info);
		}
	}
	
	return true;
}

//���ؿ������
bool NetworkKernel::onSocketSubOpenRlt(void* data, int dataSize)
{

	CC_ASSERT(dataSize == sizeof(CMD_GP_StartGameRandRes));
	if (dataSize != sizeof(CMD_GP_StartGameRandRes)) return false;

	CMD_GP_StartGameRandRes* gameRlt = (CMD_GP_StartGameRandRes*)data;
	auto gameInfo = OneGameInfo::GetInstance();


	//���ÿ������
	OpenRes open;
	//��������
	int animal[24]_AnimalArray;
	switch (gameRlt->dwTsRewardId)
	{

	case enumSpecialRs::Normal:
	case enumSpecialRs::D3Y:
	case enumSpecialRs::D4X:
	case enumSpecialRs::CJ:
	case enumSpecialRs::XYCJ:
		open.res.e1 = (enumSpecialRs)gameRlt->dwTsRewardId;
		open.res.e2 = (enumAnimalColor)gameInfo->getroomInfo()->dwColorIndex[gameRlt->dwColorIndex];
		open.res.e3 = (enumAnimalType)animal[gameRlt->dwAnimalIndex];
		open.res.e4 = (enumGameZXH)gameRlt->dwAnimalType;
		open.colorindex = gameRlt->dwColorIndex;
		open.animalindex = gameRlt->dwAnimalIndex;
		break;
	case enumSpecialRs::_4LF:
		//4LF�������ﴦ��
	case enumSpecialRs::DMG:
		//DMG��Ϣ��������
		return true;
	default:
		break;
	}

	gameInfo->setopenRlt(open);


	log("--------------%s:%d,%s:%d,%s:%d,%s:%d,%s:%d,%s,%d----------------",
		a_u8("��������"), gameRlt->dwTsRewardId,
		a_u8("��������"), gameRlt->dwAnimalIndex,
		a_u8("��������"), animal[gameRlt->dwAnimalIndex],
		a_u8("��ɫ����"), gameRlt->dwColorIndex,
		a_u8("��ɫ����"), OneGameInfo::GetInstance()->getroomInfo()->dwColorIndex[gameRlt->dwColorIndex],
		a_u8("ׯ����"), gameRlt->dwAnimalType);


	if (open.res.e1 == enumSpecialRs::CJ)
	{
		OpenCJ openCJ = gameInfo->getopenCJRlt();
		//����һ�����ݰ��������(�ʽ�������ݰ�)
		if (openCJ.binfoOk)
		{
			openCJ.binfoOk = false;
			openCJ.bresOk = false;
			//��ʼ���ʽ�����
			gameInfo->setopenCJRlt(openCJ);
			//����״̬Ϊ����
			gameInfo->setServerState(enumServerState::_Open);
		}
		else
		{
			openCJ.bresOk = true;
			gameInfo->setopenCJRlt(openCJ);
		}
	}
	else if (open.res.e1 == enumSpecialRs::XYCJ)
	{
		OpenXYCJ openXycj = gameInfo->getopenXYCJRlt();
		//����һ�����ݰ��������(XYCJת�����ݰ�)
		if (openXycj.bxycjOk)
		{
			openXycj.bresOk = false;
			openXycj.bxycjOk = false;
			//��ʼ��
			gameInfo->setopenXYCJRlt(openXycj);
			//����״̬Ϊ����
			gameInfo->setServerState(enumServerState::_Open);
		}
		else
		{
			openXycj.bresOk = true;
			gameInfo->setopenXYCJRlt(openXycj);
		}
	}
	else
	{
		//����״̬Ϊ����
		gameInfo->setServerState(enumServerState::_Open);
	}

	return true;
}

//CJ�ʽ𿪽���Ϣ
bool NetworkKernel::onSocketSubOpenCJRlt(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	auto gameInfo = OneGameInfo::GetInstance();
	OpenRes open = gameInfo->getopenRlt();
	OpenCJ openCJ = gameInfo->getopenCJRlt();

	//�м�������н�
	openCJ.num = packet.read4Byte();
	//��ȡ3�������Ϣ
	for (int i = 0; i < openCJ.num; i++)
	{
		openCJ.userid[i]= packet.read4Byte();

		u2string str;
		str.resize(LEN_NICKNAME + 1, '\0');
		packet.readUTF16(&str[0], LEN_NICKNAME);
		memcpy(openCJ.name[i], u2_8(&str[0]), LEN_NICKNAME);

		int	viplevel = packet.read4Byte();
		openCJ.score[i] = packet.read4Byte();
		int	headimgid = packet.read4Byte();
		SCORE winscore = packet.read8Byte();

		log("--------CJ:---name:%s , gold:%d--------", openCJ.name[i], openCJ.score[i]);
	}
	//����һ�����ݰ��������(�������ݰ�)
	if (openCJ.bresOk)
	{
		openCJ.binfoOk = false;
		openCJ.bresOk = false;
		//��ʼ���ʽ�����
		gameInfo->setopenCJRlt(openCJ);
		//����״̬Ϊ����
		gameInfo->setServerState(enumServerState::_Open);
	}
	else
	{
		openCJ.binfoOk = true;
		gameInfo->setopenCJRlt(openCJ);
	}
	

	return true;
}

//XYCJ���˳齱
bool NetworkKernel::onSocketSubOpenXYCJRlt(void* data, int dataSize)
{


	CC_ASSERT(dataSize == sizeof(CMD_GP_LuckDraw));
	if (dataSize != sizeof(CMD_GP_LuckDraw)) return false;

	CMD_GP_LuckDraw* xycj = (CMD_GP_LuckDraw*)data;

	log("-----------XYCJ:%d,%d,%d,%d---A:%d----------", xycj->Notes[0], xycj->Notes[1], xycj->Notes[2], xycj->Notes[3],xycj->LuckType);


	auto gameInfo = OneGameInfo::GetInstance();
	OpenXYCJ openXycj = gameInfo->getopenXYCJRlt();
	memcpy(openXycj.iNotes, xycj->Notes, sizeof(openXycj.iNotes));
	openXycj.Atype = xycj->LuckType;
	openXycj.AValue = (float)xycj->LuckValue / 100;	//����100 ����ʵ�ʵı��� �ͷ����Э��
	//����һ�����ݰ��������(�������ݰ�)
	if (openXycj.bresOk)
	{
		openXycj.bxycjOk = false;
		openXycj.bresOk = false;
		//��ʼ���ʽ�����
		gameInfo->setopenXYCJRlt(openXycj);
		//����״̬Ϊ����
		gameInfo->setServerState(enumServerState::_Open);
	}
	else
	{
		openXycj.bxycjOk = true;
		gameInfo->setopenXYCJRlt(openXycj);
	}
	

	return true;
}

//DMG�����Ὺ����Ϣ
bool NetworkKernel::onSocketSubOpenDMGRlt(void* data, int dataSize)
{
	CC_ASSERT(dataSize == sizeof(CMD_GP_DaManGuan));
	if (dataSize != sizeof(CMD_GP_DaManGuan)) return false;

	auto gameInfo = OneGameInfo::GetInstance();
	//��������
	int animal[24]_AnimalArray;
	CMD_GP_DaManGuan* pDMG = (CMD_GP_DaManGuan*)data;
	OpenDMG dmg;
	dmg.leftIndex = 12;
	for (size_t i = _ZERO; i < 12; i++)
	{
		dmg.res[i].e1 = (enumSpecialRs)enumSpecialRs::DMG;
		dmg.res[i].e2 = (enumAnimalColor)gameInfo->getroomInfo()->dwColorIndex[pDMG->Clors[i]];
		dmg.res[i].e3 = (enumAnimalType)animal[pDMG->Notes[i]];
		dmg.res[i].e4 = (enumGameZXH)pDMG->AnimalType;
		dmg.colorindex[i] = (int)pDMG->Clors[i];
		dmg.animalindex[i] = (int)pDMG->Notes[i];

		log("--------------%s:%s:%d,%s:%d,%s:%d,%s:%d,%s,%d--------",
			a_u8("DMG"),
			a_u8("��������"), dmg.animalindex[i],
			a_u8("��������"), dmg.res[i].e3,
			a_u8("��ɫ����"), dmg.colorindex[i],
			a_u8("��ɫ����"), dmg.res[i].e2,
			a_u8("ׯ����"), dmg.res[i].e4);
	}

	//���ÿ�������
	OpenRes open;
	open.res = dmg.res[0];
	open.colorindex = dmg.colorindex[0];
	open.animalindex = dmg.animalindex[0];
	gameInfo->setopenRlt(open);


	gameInfo->setopenDMGRlt(dmg);
	//����״̬Ϊ����
	gameInfo->setServerState(enumServerState::_Open);
	return true;
}

//4����
bool NetworkKernel::onSocketSubOpen4LFRlt(void* data, int dataSize)
{

	CC_ASSERT(dataSize == sizeof(CMD_GP_SiLianFa));
	if (dataSize != sizeof(CMD_GP_SiLianFa)) return false;

	auto gameInfo = OneGameInfo::GetInstance();
	//��������
	int animal[24]_AnimalArray;
	CMD_GP_SiLianFa* p4lf = (CMD_GP_SiLianFa*)data;
	//���ÿ�������
	OpenRes open;
	open.res.e1 = enumSpecialRs::_4LF;
	open.res.e4 = (enumGameZXH)p4lf->AnimalType;

	gameInfo->setopenRlt(open);

	Open4LF open_4LF;
	open_4LF.leftIndex = 4;
	for (size_t i = _ZERO; i < 4; i++)
	{
		open_4LF.res[i].e1 = enumSpecialRs::_4LF;
		open_4LF.res[i].e2 = (enumAnimalColor)gameInfo->getroomInfo()->dwColorIndex[p4lf->Clors[i]];
		open_4LF.res[i].e3 = (enumAnimalType)animal[p4lf->Notes[i]];
		open_4LF.res[i].e4 = (enumGameZXH)p4lf->AnimalType;
		open_4LF.colorindex[i] = p4lf->Clors[i];
		open_4LF.animalindex[i] =p4lf->Notes[i];

		log("--------------%s:%s:%d,%s:%d,%s:%d,%s:%d,%s,%d-----",
			a_u8("4LF"),
			a_u8("��������"), open_4LF.animalindex[i],
			a_u8("��������"), open_4LF.res[i].e3,
			a_u8("��ɫ����"), open_4LF.colorindex[i],
			a_u8("��ɫ����"), open_4LF.res[i].e2,
			a_u8("ׯ����"), open_4LF.res[i].e4);
	}
	gameInfo->setopen4LFRlt(open_4LF);
	//����״̬Ϊ����
	gameInfo->setServerState(enumServerState::_Open);

	return true;
}

//��ʼ��Ϸ(ѹ��)
bool NetworkKernel::onSocketSubStartGame(void* data, int dataSize)
{

	CC_ASSERT(dataSize == sizeof(CMD_GP_StartGame));
	if (dataSize != sizeof(CMD_GP_StartGame)) return false;
	CMD_GP_StartGame* info = (CMD_GP_StartGame*)data;


	auto gameInfo = OneGameInfo::GetInstance();
	slwhRoomInfo* roomInfo = gameInfo->getroomInfo();
	roomInfo->dwRateID = info->dwRateID;
	//��ʼ����ѹ����Ϣs
	roomInfo->alljetinfo = slwhAllJetInfo();
	roomInfo->lCj = info->lCj;
	memcpy(roomInfo->dwColorIndex, info->dwColorIndex, sizeof(dword)* 24);
	//����״̬Ϊ��ʼ��Ϸ
	gameInfo->setServerState(enumServerState::_Jet);
	gameInfo->setbCountScore(false);

	return true;
}

//�����ȡ����ѹ��
bool NetworkKernel::onSocketSubOtherCancelJet(void* data, int dataSize)
{
	CC_ASSERT(dataSize == sizeof(CMD_GP_CharNotesInfo));
	if (dataSize != sizeof(CMD_GP_CharNotesInfo)) return false;

	CMD_GP_CharNotesInfo* info = (CMD_GP_CharNotesInfo*)data;
	//��������ѹ��
	if (SLWHGameScene::m_uiLayer != nullptr)
	{
		auto gameInfo = OneGameInfo::GetInstance();
		slwhRoomInfo* roomInfo = gameInfo->getroomInfo();
		SCORE sum = _ZERO;
		for (size_t i = _ZERO; i < info->size; i++)
		{
			roomInfo->alljetinfo.lTotelBet[info->notes[i].noteid] -= info->notes[i].gold;
			sum += info->notes[i].gold;
			log("-------------------cancel jet :%d----------------------", info->notes[i].noteid);
		}
		
		roomInfo->alljetinfo.lAllBet -= sum;
		//gameInfo->setroomInfo(roomInfo);
		//�Լ�ȡ��ѹ�ֳɹ� ֪ͨˢ��UI
		if (info->userid == Player::getInstance()->getLogonData().dwUserID)
		{
			SLWHGameScene::m_uiLayer->m_myJetManager.successCancelJet();
		}

		SLWHGameScene::m_uiLayer->m_userListManager.othercancelJet(info->userid, sum);
	
	}
	return true;
}

