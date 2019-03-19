#include "CServerItem_SHZ.h"
//////////////////////////////////////////////////////////////////////////
//
int __gServerItemRefCount_SHZ = 0;
SHZIServerItem* __gServerItem_SHZ = 0;

SHZIServerItem* SHZIServerItem::create()
{
	if (__gServerItemRefCount_SHZ == 0)
	{
		__gServerItem_SHZ = new SHZCServerItem();
	}   

	__gServerItemRefCount_SHZ++;
	PLAZZ_PRINTF("IServerItem::create ref:%d\n", __gServerItemRefCount_SHZ);

	return __gServerItem_SHZ;
}

void SHZIServerItem::destory()
{
	if (__gServerItemRefCount_SHZ > 0)
	{
		__gServerItemRefCount_SHZ--;
		if (__gServerItemRefCount_SHZ <= 0)
		{
			__gServerItemRefCount_SHZ = 0;
			delete __gServerItem_SHZ;
			__gServerItem_SHZ = 0;
		}
	}
	PLAZZ_PRINTF("IServerItem::destory ref:%d\n", __gServerItemRefCount_SHZ);

}

SHZIServerItem* SHZIServerItem::get()
{
	return __gServerItem_SHZ;
}

//////////////////////////////////////////////////////////////////////////
SHZCServerItem::SHZCServerItem() 
	: mReqTableID(INVALID_TABLE)
	, mReqChairID(INVALID_CHAIR)
	, mFindTableID(INVALID_TABLE)
	, mIsGameReady(false)
	, mMeUserItem(0)
	, mIServerItemSink(0)
	, mIChatSink(0)
	, mIStringMessageSink(0)
	, mSocketEngine(0)
	, mServiceStatus(SHZ_ServiceStatus_Unknow)
{
	mIsQuickSitDown=false;
	memset(&mUserAttribute,   0, sizeof(mUserAttribute));
	memset(&mServerAttribute, 0, sizeof(mServerAttribute));

	mSocketEngine = ISocketEngine::create();
	mUserManager  = new SHZCPlazaUserManager();
	mUserManager->SetUserManagerSink(this);
	mSocketEngine->setSocketEngineSink(this);
}

SHZCServerItem::~SHZCServerItem()
{
	IntermitConnect(true);
	mIServerItemSink=0;
	mUserManager->SetUserManagerSink(0);
	mSocketEngine->setSocketEngineSink(0);
	ISocketEngine::destory(mSocketEngine);
	mSocketEngine = 0;
	SafeDelete(mUserManager);
	mUserManager  = 0;
}

//////////////////////////////////////////////////////////////////////////
//���ýӿ�

//���ýӿ�
bool SHZCServerItem::SetServerItemSink(SHZIServerItemSink* pIServerItemSink)
{
	mIServerItemSink = pIServerItemSink;
	return true;
}

//���ýӿ�
bool SHZCServerItem::SetChatSink(SHZIChatSink* pIChatSink)
{
	PLAZZ_PRINTF("CServerItem::SetChatSink\n");
	mIChatSink = pIChatSink;
	return true;
}

//���ýӿ�
bool SHZCServerItem::SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink)
{
	mIStringMessageSink = pIStringMessageSink;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//���Խӿ�
//�û�����
const tagUserAttribute& SHZCServerItem::GetUserAttribute() const
{
	return mUserAttribute;
}

//��������
const tagServerAttribute& SHZCServerItem::GetServerAttribute() const 
{
	return mServerAttribute;
}

//����״̬
SHZ_enServiceStatus SHZCServerItem::GetServiceStatus()
{
	return mServiceStatus;
}

//�Ƿ����״̬
bool SHZCServerItem::IsService()
{
	return GetServiceStatus() == SHZ_ServiceStatus_ServiceIng;
}

//�Լ�״̬
bool SHZCServerItem::IsPlayingMySelf()
{
	return ((mMeUserItem!=0)&&(mMeUserItem->GetUserStatus()==US_PLAYING));
}

//����״̬
void SHZCServerItem::SetServiceStatus(SHZ_enServiceStatus ServiceStatus)
{
	//���ñ���
	mServiceStatus=ServiceStatus;
	return;
}

//���÷���
bool SHZCServerItem::SetServerAttribute(HallGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)
{
	//��������
	SHZCGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;

	memcpy(&mGameKind,&pGameKindItem->m_GameKind,sizeof(mGameKind));
	memcpy(&mGameServer,&pGameServerItem->m_GameServer,sizeof(mGameServer));

	mServerAttribute.wKindID		= mGameServer.wKindID;
	mServerAttribute.wServerID		= mGameServer.wServerID;
	//mServerAttribute.wServerType	= mGameServer.wServerType;
	//mServerAttribute.dwServerRule	= mGameServer.dwServerRule;
	mServerAttribute.wAVServerPort	= wAVServerPort;
	mServerAttribute.dwAVServerAddr	= dwAVServerAddr;
	strncpy(mServerAttribute.szServerName, mGameServer.szServerName, LEN_SERVER);

	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//��������
	mParameterGame=pParameterGlobal->GetParameterGame(&mGameKind);
	mParameterServer=pParameterGlobal->GetParameterServer(&mGameServer);

	return true;
}

//////////////////////////////////////////////////////////////////////////
//���ӽӿ�

//���÷���
bool SHZCServerItem::ConnectServer(HallGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)
{
	if (mServiceStatus != SHZ_ServiceStatus_Unknow &&
		mServiceStatus != SHZ_ServiceStatus_NetworkDown)
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("�ܱ�Ǹ, ����Ϸ���仹δ�ر�,���ȹر�!!"),DLG_MB_OK);
		return false;
	}

	//��������
	SetServerAttribute(pGameServerItem, wAVServerPort, dwAVServerAddr);

	//�ر��ж�
	ASSERT(mServerAttribute.wServerID!=0);
	if (mServerAttribute.wServerID==0)
	{
		SetServiceStatus(SHZ_ServiceStatus_Unknow);
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("�ܱ�Ǹ������Ϸ�����Ѿ��ر��ˣ�������������룡"),DLG_MB_OK);
		return false;
	}

	//�������
	cocos2d::log("-------------ConnectServer[ip = %s,port = %d]-------------", mGameServer.szServerAddr, mGameServer.wServerPort);

	if (!mSocketEngine->connect(mGameServer.szServerAddr, mGameServer.wServerPort))
	{
		cocos2d::log("-------------connect error-------------");
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("��Ϸ��������ʧ�ܣ�����ϵͳ�������ò����Ƿ���ȷ"),DLG_MB_OK);
		return false;
	}

	//����״̬
	SetServiceStatus(SHZ_ServiceStatus_Entering);


	return true;
}

//�ж�����
bool SHZCServerItem::IntermitConnect(bool force)
{	
	if (mServiceStatus == SHZ_ServiceStatus_Unknow ||
		mServiceStatus == SHZ_ServiceStatus_NetworkDown)
		return false;

	if (IsService())
	{
		if(SHZIClientKernel::get())
		{
			SHZIClientKernel::get()->Intermit(0);
		}
	}

	//����״̬
	SetServiceStatus(SHZ_ServiceStatus_NetworkDown);

	mSocketEngine->disconnect();
	mReqTableID		= INVALID_TABLE;
	mReqChairID		= INVALID_CHAIR;
	mFindTableID	= INVALID_TABLE;
	mIsGameReady	= false;
	mMeUserItem		= 0;
	mUserManager->ResetUserItem();
	return true;
}

//////////////////////////////////////////////////////////////////////////
//����ӿ�

//���ͺ���
bool SHZCServerItem::SendSocketData(word wMainCmdID, word wSubCmdID)
{
	return SendSocketData(wMainCmdID, wSubCmdID, 0, 0);
}

//���ͺ���
bool SHZCServerItem::SendSocketData(word wMainCmdID, word wSubCmdID, void * data, word dataSize)
{
	return mSocketEngine->send(wMainCmdID, wSubCmdID, (unsigned char*)data, dataSize);
}



//���͵�¼
bool SHZCServerItem::SendLogonPacket()
{
	//��������
	SHZCGlobalUserInfo * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	
	//��������
	PACKET_AIDE_SIZE(512);
	packet.write4Byte(DF::shared()->GetPlazaVersion());
	packet.write4Byte(DF::shared()->GetFrameVersion());
	packet.write4Byte(DF::shared()->GetGameVersion());
	packet.write4Byte(pGlobalUserData->dwUserID);
	packet.writeUTF16(u8_2(pGlobalUserData->szPassword), LEN_PASSWORD);
	packet.writeUTF16(u8_2(DF::shared()->GetMachineID()), LEN_MACHINE_ID);
	packet.write2Byte(mServerAttribute.wKindID);

	//��������
	SendSocketData(MDM_GR_LOGON_SHZ,SUB_GR_LOGON_USERID_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//��������
bool SHZCServerItem::SendUserRulePacket()
{	
	//��������
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();
	//��������
	byte cbRuleMask=0x00;

	//���Ƶ�ַ
	if (pParameterGlobal->m_bLimitSameIP==true)
		cbRuleMask|=UR_LIMIT_SAME_IP_SHZ;

	//����ʤ��
	if (mParameterGame->m_bLimitWinRate==true)
		cbRuleMask|=UR_LIMIT_WIN_RATE_SHZ;
	

	//��������
	if (mParameterGame->m_bLimitFleeRate==true)
		cbRuleMask|=UR_LIMIT_FLEE_RATE_SHZ;
	
	//���ƻ���
	if (mParameterGame->m_bLimitGameScore==true)
		cbRuleMask|=UR_LIMIT_GAME_SCORE_SHZ;

	PACKET_AIDE_SIZE(SIZE_PACK_DATA);
	packet.writeByte(cbRuleMask);
	packet.write2Byte(mParameterGame->m_wMinWinRate);
	packet.write2Byte(mParameterGame->m_wMaxFleeRate);
	packet.write4Byte(mParameterGame->m_lMaxGameScore);
	packet.write4Byte(mParameterGame->m_lMinGameScore);

	//Я������
	if ((SHZCServerRule::IsForfendLockTable(mServerAttribute.dwServerRule)==false)&&(mParameterServer->m_bTakePassword==true))
	{
		const unsigned short* ucs2 = u8_2(mParameterServer->m_szPassword);
		int ucs2len = ucs2_len(ucs2);
		packet.write2Byte(ucs2len);
		packet.write2Byte(DTP_GR_TABLE_PASSWORD_SHZ);
		packet.writeUTF16(ucs2, ucs2len);
	}

	//��������
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_RULE_SHZ,packet.getBuffer(),packet.getPosition());
	return true;
}

//�����Թ�
bool SHZCServerItem::SendLookonPacket(word wTableID, word wChairID)
{
	PACKET_AIDE_SIZE(4);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);

	//��������
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_LOOKON_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//��������
bool SHZCServerItem::SendSitDownPacket(word wTableID, word wChairID, const char* lpszPassword)
{
	const unsigned short* ucs2 = u8_2(lpszPassword);

	//��������
	PACKET_AIDE_SIZE(128);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);
	packet.writeUTF16(ucs2, LEN_PASSWORD);
		
	//��������
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_SITDOWN_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//��������
bool SHZCServerItem::SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave)
{
	PACKET_AIDE_SIZE(10);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);
	packet.writeByte(cbForceLeave);

	//��������
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_STANDUP_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//��������
bool SHZCServerItem::SendUserChatPacket(dword dwTargetUserID, const char* pszChatString, dword dwColor)
{
	const unsigned short* ucs2 = u8_2(pszChatString);
	int ucs2len = ucs2_len(ucs2);

	//������Ϣ
	PACKET_AIDE_SIZE(512);
	packet.write2Byte(ucs2len);
	packet.write4Byte((dwColor&0xFFFF<<8)|(dwColor&0xFF0000>>16));
	packet.write4Byte(dwTargetUserID);
	packet.writeUTF16(ucs2, ucs2len);

	//��������
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_CHAT_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//���ͱ���
bool SHZCServerItem::SendExpressionPacket(dword dwTargetUserID, word wItemIndex)
{
	PACKET_AIDE_SIZE(6);
	packet.write2Byte(wItemIndex);
	packet.write4Byte(dwTargetUserID);

	//��������
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_EXPRESSION_SHZ,packet.getBuffer(),packet.getPosition());

	return true;
}

//////////////////////////////////////////////////////////////////////////
//�ڲ�����

//����Ч��
bool SHZCServerItem::EfficacyUserChat(const char* pszChatString, word wExpressionIndex)
{
	//��������
	ASSERT(mMeUserItem!=0);
	byte cbMemberOrder=mMeUserItem->GetMemberOrder();
	byte cbMasterOrder=mMeUserItem->GetMasterOrder();

	//���ȱȽ�
	if(pszChatString != 0)
	{
		if(utf8_len(pszChatString) >= (LEN_USER_CHAT/2))
		{
			if (mIChatSink)
				mIChatSink->InsertSystemChat(a_u8("��Ǹ����������������ݹ����������̺��ٷ��ͣ�"));
			return false;
		}
	}

	//��������
	if (SHZCServerRule::IsForfendRoomChat(mServerAttribute.dwServerRule)&&(cbMasterOrder==0))
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			char szChatString[256]={0};
			sprintf(szChatString, a_u8("\n��%s������ʧ��"), pszChatString);

			if (mIChatSink)
				mIChatSink->InsertSystemChat(szChatString);
		}

		if (mIChatSink)
			mIChatSink->InsertSystemChat(a_u8("��Ǹ����ǰ����Ϸ�����ֹ�û��������죡"));

		return false;
	}

	//Ȩ���ж�
	if (SHZCUserRight::CanRoomChat(mUserAttribute.dwUserRight)==false)
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			char szChatString[256]={0};
			sprintf(szChatString, a_u8("\n��%s������ʧ��"), pszChatString);

			if (mIChatSink)
				mIChatSink->InsertSystemChat(szChatString);
		}

		//������Ϣ
		if (mIChatSink)
			mIChatSink->InsertSystemChat(a_u8("��Ǹ����û�д������Ե�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"));

		return false;
	}

	//�ٶ��ж�
	static dword dwChatTime=0;
	dword dwCurrentTime=(dword)time(0);
	if ((cbMasterOrder==0)&&((dwCurrentTime-dwChatTime)<=TIME_USER_CHAT))
	{
		//ԭʼ��Ϣ
		if (wExpressionIndex==INVALID_WORD)
		{
			char szChatString[256]={0};
			sprintf(szChatString, a_u8("\n��%s������ʧ��"), pszChatString);

			if (mIChatSink)
				mIChatSink->InsertSystemChat(szChatString);
		}

		//������Ϣ
		if (mIChatSink)
			mIChatSink->InsertSystemChat(a_u8("����˵���ٶ�̫���ˣ����������ȱ�����Ϣ�°ɣ�"));

		return false;
	}

	//���ñ���
	dwChatTime=dwCurrentTime;
	return true;
}

//����Ч��
bool SHZCServerItem::EfficacyTableRule(word wTableID, word wChairID, bool bReqLookon, char strDescribe[256])
{
	//Ч�����
	ASSERT(wTableID<mTableViewFrame.GetTableCount());
	ASSERT(wChairID<mTableViewFrame.GetChairCount());

	//״̬����
	if (mServiceStatus != SHZ_ServiceStatus_ServiceIng) return false;
	if (wTableID>=mTableViewFrame.GetTableCount()) return false;
	if (wChairID>=mTableViewFrame.GetChairCount()) return false;

	//��������
	ASSERT(SHZCParameterGlobal::shared()!=0);
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//��������
	SHZITableView * pITableView=mTableViewFrame.GetTableViewItem(wTableID);
	SHZIClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//��������
	bool bGameStart=pITableView->GetPlayFlag();
	bool bDynamicJoin=SHZCServerRule::IsAllowDynamicJoin(mServerAttribute.dwServerRule);

	//��Ϸ״̬
	if ((bGameStart==true)&&(bDynamicJoin==false)&&(bReqLookon==false))
	{
		sprintf(strDescribe, "%s", a_u8("����Ϸ���Ѿ���ʼ��Ϸ�ˣ���ʱ���ܼ��룡"));
		return false;
	}

	//�����ж�
	if ((bReqLookon==false)&&(mMeUserItem->GetMasterOrder()==0))
	{
		//�����ж�
		for (word i=0;i<mTableViewFrame.GetChairCount();i++)
		{
			//��ȡ�û�
			SHZIClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==0)||(pITableUserItem==mMeUserItem)) continue;

			//������
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pITableUserItem->GetUserCompanion()==CP_DETEST)
				{
					//������ʾ
					sprintf(strDescribe, a_u8("�������˲��벻�ܻ�ӭ�������Ϸ��������������ӭ����� [ %s ] ��"), pITableUserItem->GetNickName());
					return false;
				}
			}

			//ʤ��Ч��
			if (mParameterGame->m_bLimitWinRate==true)
			{
				if (((word)(pITableUserItem->GetUserWinRate()*1000L))<mParameterGame->m_wMinWinRate)
				{
					sprintf(strDescribe, a_u8("[ %s ] ��ʤ��̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//����Ч��
			if (mParameterGame->m_bLimitFleeRate)
			{
				if (((word)(pITableUserItem->GetUserFleeRate()*1000L))<mParameterGame->m_wMaxFleeRate)
				{
					sprintf(strDescribe, a_u8("[ %s ] ������̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//����Ч��
			if (mParameterGame->m_bLimitGameScore)
			{
				//��߻���
				if (pITableUserItem->GetUserScore()>mParameterGame->m_lMaxGameScore)
				{
					sprintf(strDescribe, a_u8("[ %s ] �Ļ���̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}

				//��ͻ���
				if (pITableUserItem->GetUserScore()<mParameterGame->m_lMinGameScore)
				{
					sprintf(strDescribe, a_u8("[ %s ] �Ļ���̫���ˣ����������ò�����"),pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}

	return true;
}