#include "CServerItem_SHZ.h"

//�û�����
bool SHZCServerItem::OnSocketMainUser(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//��������ʧ��
	case SUB_GR_REQUEST_FAILURE_SHZ:	return OnSocketSubRequestFailure(data,dataSize);
		//�������
	case SUB_GR_USER_INVITE_SHZ:		return OnSocketSubUserInvite(data,dataSize);
		//�ȴ�����
	case SUB_GR_USER_WAIT_DISTRIBUTE_SHZ: return OnSocketSubWaitDistribute(data,dataSize);

		//�û�����
	case SUB_GR_USER_ENTER_SHZ:			return OnSocketSubUserEnter(data,dataSize);
		//�û�����
	case SUB_GR_USER_SCORE_SHZ:			return OnSocketSubUserScore(data,dataSize);
		//�û�״̬
	case SUB_GR_USER_STATUS_SHZ:		return OnSocketSubUserStatus(data,dataSize);
	
		//�û�����
	case SUB_GR_USER_CHAT_SHZ:			return OnSocketSubUserChat(data,dataSize);
		//�û�����
	case SUB_GR_USER_EXPRESSION_SHZ:	return OnSocketSubExpression(data,dataSize);
		//�û�˽��
	case SUB_GR_WISPER_CHAT_SHZ:		return OnSocketSubWisperUserChat(data,dataSize);
		//˽�ı���
	case SUB_GR_WISPER_EXPRESSION_SHZ:	return OnSocketSubWisperExpression(data,dataSize);
	
		//���߳ɹ�
	case SUB_GR_PROPERTY_SUCCESS_SHZ:	return OnSocketSubPropertySuccess(data,dataSize);
		//����ʧ��
	case SUB_GR_PROPERTY_FAILURE_SHZ:	return OnSocketSubPropertyFailure(data,dataSize);
		//����ЧӦ
	case SUB_GR_PROPERTY_EFFECT_SHZ:	return OnSocketSubPropertyEffect(data,dataSize);
		//������Ϣ
	case SUB_GR_PROPERTY_MESSAGE_SHZ:   return OnSocketSubPropertyMessage(data,dataSize);
		//������Ϣ
	case SUB_GR_PROPERTY_TRUMPET_SHZ:	return OnSocketSubPropertyTrumpet(data,dataSize);

		//ϲ����Ϣ
	case SUB_GR_GLAD_MESSAGE_SHZ:		return OnSocketSubGladMessage(data,dataSize);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ����״̬
//����ʧ��
bool SHZCServerItem::OnSocketSubRequestFailure(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	int lErrorCode = packet.read4Byte();

	int len=(dataSize-4)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0],len);
	std::string sString = u2_8(&str[0]);

	//��������
	SHZIClientUserItem * pITableUserItem=0;
	word wMeTableID=mMeUserItem->GetTableID();
	word wMeChairID=mMeUserItem->GetChairID();


	//���滹ԭ
	if ((mReqTableID!=INVALID_TABLE)&&(mReqChairID!=INVALID_TABLE)&&(mReqChairID!=MAX_CHAIR))
	{
		SHZIClientUserItem * pITableUserItem=mTableViewFrame.GetClientUserItem(mReqTableID,mReqChairID);
		if (pITableUserItem==mMeUserItem) 
			mTableViewFrame.SetClientUserItem(mReqTableID,mReqChairID,0);
	}

	//���ñ���
	mReqTableID=INVALID_TABLE;
	mReqChairID=INVALID_CHAIR;

	PLAZZ_PRINTF("OnSocketSubRequestFailure %d : %d\n", mServiceStatus, SHZ_ServiceStatus_ServiceIng);
	if (mIsQuickSitDown)
	{
		mIsQuickSitDown = false;
		if (SHZCServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
		{//��������,������ģʽ
			IntermitConnect(true);
			if (mIServerItemSink)
				mIServerItemSink->OnGFServerClose(sString.c_str());
		}
	}
	else 
	{
		//��ʾ��Ϣ
		if (mIServerItemSink)
			mIServerItemSink->onGRRequestFailure(sString.c_str());
	}

	return true;

	////��������
	//CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)data;

	////Ч�����
	//ASSERT(dataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	//if (dataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	////��������
	//IClientUserItem * pITableUserItem=0;
	//word wMeTableID=mMeUserItem->GetTableID();
	//word wMeChairID=mMeUserItem->GetChairID();

	//
	////���滹ԭ
	//if ((mReqTableID!=INVALID_TABLE)&&(mReqChairID!=INVALID_TABLE)&&(mReqChairID!=MAX_CHAIR))
	//{
	//	IClientUserItem * pITableUserItem=mTableViewFrame.GetClientUserItem(mReqTableID,mReqChairID);
	//	if (pITableUserItem==mMeUserItem) 
	//		mTableViewFrame.SetClientUserItem(mReqTableID,mReqChairID,0);
	//}

	////���ñ���
	//mReqTableID=INVALID_TABLE;
	//mReqChairID=INVALID_CHAIR;

	//PLAZZ_PRINTF("OnSocketSubRequestFailure %d : %d\n", mServiceStatus, ServiceStatus_ServiceIng);
	//if (mIsQuickSitDown)
	//{
	//	mIsQuickSitDown = false;
	//	if (CServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
	//	{//��������,������ģʽ
	//		IntermitConnect(true);
	//		if (mIServerItemSink)
	//			mIServerItemSink->OnGFServerClose(pRequestFailure->szDescribeString);
	//	}
	//}
	//else 
	//{
	//	//��ʾ��Ϣ
	//	if (mIServerItemSink)
	//		mIServerItemSink->onGRRequestFailure(pRequestFailure->szDescribeString);
	//}
	//
	//return true;
}

//�ȴ�����
bool SHZCServerItem::OnSocketSubWaitDistribute(void *data, int dataSize)
{
	//��������
	mTableViewFrame.SetTableStatus(true);

	//��Ϸ֪ͨ
	OnSocketMainGameFrame(MDM_GF_FRAME_SHZ,SUB_GR_USER_WAIT_DISTRIBUTE_SHZ,data,dataSize);
	return true;
}

//�������
bool SHZCServerItem::OnSocketSubUserInvite(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	word wTableID = packet.read2Byte();
	dword dwUserID = packet.read4Byte();

	ASSERT(wTableID<mTableViewFrame.GetTableCount());
	if (mTableViewFrame.GetPlayFlag(wTableID)==true) 
		return true;

	//Ѱ���û�
	SHZIClientUserItem * pIUserItem=mUserManager->SearchUserByUserID(dwUserID);
	if (pIUserItem==0) return true;
	if (pIUserItem->GetTableID()==INVALID_TABLE) return true;

	//��������
	ASSERT(SHZCParameterGlobal::shared()!=0);
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//�û�����
	if (pIUserItem->GetUserCompanion()==CP_DETEST) return true;
	if (pParameterGlobal->m_cbInviteMode==INVITE_MODE_NONE_SHZ) return true;
	if ((pParameterGlobal->m_cbInviteMode==INVITE_MODE_FRIEND_SHZ)&&(pIUserItem->GetUserCompanion()!=CP_FRIEND)) return true;

	//��ʾ��Ϣ
	char szMessage[256]={0};
	sprintf(szMessage, a_u8("[ %s ] ����������� [ %ld ] ��Ϸ��������Ϸ���Ƿ�ͬ�⣿"),pIUserItem->GetNickName(),wTableID+1);

	//��ʾ��Ϣ
	if (mIServerItemSink)
	{
		PLAZZ_PRINTF("%s",a_u8("//TODO:δʵ��\n"));
		//if (mIServerItemSink->OnGFUserInvite(szMessage))
		//{
		//	word wChairID=INVALID_CHAIR;
		//	word wNullCount=mTableViewFrame.GetNullChairCount(wTableID,wChairID);
		//	if (wNullCount==0) 
		//	{
		//		mIServerItemSink->OnGFUserInviteFailure("����Ϸ���Ѿ�û�п�λ���ˣ�");
		//		return true;
		//	}
		//	PerformSitDownAction(wTableID,wChairID,false);
		//}
	}

	return true;

	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_UserInvite));
	//if (dataSize!=sizeof(CMD_GR_UserInvite)) return false;

	////��Ϣ����
	//CMD_GR_UserInvite * pUserInvite=(CMD_GR_UserInvite *)data;
	//ASSERT(pUserInvite->wTableID<mTableViewFrame.GetTableCount());
	//if (mTableViewFrame.GetPlayFlag(pUserInvite->wTableID)==true) 
	//	return true;

	////Ѱ���û�
	//IClientUserItem * pIUserItem=mUserManager->SearchUserByUserID(pUserInvite->dwUserID);
	//if (pIUserItem==0) return true;
	//if (pIUserItem->GetTableID()==INVALID_TABLE) return true;

	////��������
	//ASSERT(CParameterGlobal::shared()!=0);
	//CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	////�û�����
	//if (pIUserItem->GetUserCompanion()==CP_DETEST) return true;
	//if (pParameterGlobal->m_cbInviteMode==INVITE_MODE_NONE_SHZ) return true;
	//if ((pParameterGlobal->m_cbInviteMode==INVITE_MODE_FRIEND_SHZ)&&(pIUserItem->GetUserCompanion()!=CP_FRIEND)) return true;

	////��ʾ��Ϣ
	//char szMessage[256]=T_T("");
	//_sntprintf(szMessage,countarray(szMessage),T_T("[ %s ] ����������� [ %ld ] ��Ϸ��������Ϸ���Ƿ�ͬ�⣿"),pIUserItem->GetNickName(),pUserInvite->wTableID+1);

	////��ʾ��Ϣ
	//if (mIServerItemSink)
	//{
	//	if (mIServerItemSink->OnGFUserInvite(szMessage))
	//	{
	//		word wChairID=INVALID_CHAIR;
	//		word w0Count=mTableViewFrame.Get0ChairCount(pUserInvite->wTableID,wChairID);
	//		if (w0Count==0) 
	//		{
	//			mIServerItemSink->OnGFUserInviteFailure(T_T("����Ϸ���Ѿ�û�п�λ���ˣ�"));
	//			return true;
	//		}
	//		PerformSitDownAction(pUserInvite->wTableID,wChairID,false);
	//	}
	//}

	//return true;
}

//////////////////////////////////////////////////////////////////////////
//�û���Ϣ
#define READ_PACKET(szOut)														\
int len=wDataSize/2;															\
u2string str;																	\
str.resize(len+1,'\0');															\
packet.readUTF16(&str[0], len);													\
strncpy(szOut, u2_8(&str[0]), countarray(szOut))

//�û�����
bool SHZCServerItem::OnSocketSubUserEnter(void* data, int dataSize)
{
	//��������
	tagSHZ_UserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	lotteryInfo lotteryInfo;
	zeromemory(&UserInfo,sizeof(UserInfo));
	zeromemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	SHZCGlobalUserInfo * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	PACKET_AIDE_DATA(data);
	//�û�����
	UserInfo.dwGameID=packet.read4Byte();
	UserInfo.dwUserID=packet.read4Byte();
	UserInfo.dwGroupID=packet.read4Byte();
	UserInfo.wFaceID=packet.read2Byte();
	UserInfo.dwCustomID=packet.read4Byte();

	//�û�����
	UserInfo.cbGender=packet.readByte();
	UserInfo.cbMemberOrder=packet.readByte();
	UserInfo.cbMasterOrder=packet.readByte();

	//�û�״̬
	UserInfo.wTableID=packet.read2Byte();
	UserInfo.wChairID=packet.read2Byte();
	UserInfo.cbUserStatus=packet.readByte();
	
	//�û�����
	UserInfo.lScore=packet.read8Byte();
	lotteryInfo.lYuanBaoScore = packet.rSCORE();
	lotteryInfo.lShExpLottery = packet.rSCORE();
	UserInfo.lGrade=packet.read8Byte();
	UserInfo.lInsure=packet.read8Byte();

	UserInfo.dwWinCount=packet.read4Byte();
	UserInfo.dwLostCount=packet.read4Byte();
	UserInfo.dwDrawCount=packet.read4Byte();
	UserInfo.dwFleeCount=packet.read4Byte();
	UserInfo.dwUserMedal=packet.read4Byte();
	UserInfo.dwExperience=packet.read4Byte();
	UserInfo.lLoveLiness=packet.read4Byte();

	PLAZZ_PRINTF("--------------lotteryInfo.lYuanBaoScore = %d----------",lotteryInfo.lYuanBaoScore);
	PLAZZ_PRINTF("--------------lotteryInfo.lShExpLottery = %d----------",lotteryInfo.lShExpLottery);

	PLAZZ_PRINTF("--------------UserInfo.lScore = %ld----------", UserInfo.lScore);
	PLAZZ_PRINTF("--------------UserInfo.lInsure = %ld----------", UserInfo.lInsure);
	//TODOSL
	if (pGlobalUserData->dwUserID == UserInfo.dwUserID)
	{
		LotteryMission::getInstance()->setLotteryInfo(lotteryInfo);
	}
	//��չ��Ϣ
	while (dataSize>packet.getPosition())
	{
		word wDataSize = packet.read2Byte();
		word wDataDescribe = packet.read2Byte();

		if (wDataDescribe==0) break;
		switch (wDataDescribe)
		{
		case DTP_GR_NICK_NAME_SHZ:		//�û��ǳ�
			{
				READ_PACKET(UserInfo.szNickName);
				break;
			}
		case DTP_GR_GROUP_NAME_SHZ:		//�û�����
			{
				READ_PACKET(UserInfo.szGroupName);
				break;
			}
		case DTP_GR_UNDER_WRITE_SHZ:	//����ǩ��
			{
				READ_PACKET(UserInfo.szUnderWrite);
				break;
			}
		}
	
	}

	//�Զ�ͷ��
	if (UserInfo.dwCustomID!=0L)
	{
		////����ͷ��
		//CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		//bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

		////����ͷ��
		//if (bSuccess==false)
		//{
		//	pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
		//}
	}

	//��������
	bool bHideUserInfo=SHZCServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==UserInfo.dwUserID;
	bool bMasterUserOrder=((UserInfo.cbMasterOrder>0)||((mMeUserItem!=0)&&(mMeUserItem->GetMasterOrder()>0)));

	//������Ϣ
	if (!((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true)))
	{
		//�û���Ϣ
		strncpy(UserInfo.szNickName,a_u8("��Ϸ���"),countarray(UserInfo.szNickName));
		//..
	}

	//�����û�
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==0) pIClientUserItem=mUserManager->ActiveUserItem(UserInfo,CustomFaceInfo);

	//��ȡ����
	HallServerListData * pServerListData=HallServerListData::shared();

	//��������
	if (pServerListData)
		pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());

	//��������
	ASSERT(SHZCParameterGlobal::shared()!=0);
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//������ʾ
	if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	{
		if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
		{
			char szMessage[256]={0};
			sprintf(szMessage, a_u8("���ĺ��� [%s] �����ˣ�"), pIClientUserItem->GetNickName());
			//��ʾ��Ϣ
			if (mIChatSink)
				mIChatSink->InsertSystemChat(szMessage);
		}
	}

	return true;

	////Ч�����
	ASSERT(dataSize>=sizeof(tagUserInfoHead));
	//if (dataSize<sizeof(tagUserInfoHead)) return false;

	////��������
	//tagUserInfo UserInfo;
	//tagCustomFaceInfo CustomFaceInfo;
	//zeromemory(&UserInfo,sizeof(UserInfo));
	//zeromemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	////��Ϣ����
	//tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)data;

	////��������
	//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	////��������
	//bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule);
	//bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	//bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((mMeUserItem!=0)&&(mMeUserItem->GetMasterOrder()>0)));

	////��ȡ��Ϣ
	//if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	//{
	//	//�û�����
	//	UserInfo.wFaceID=pUserInfoHead->wFaceID;
	//	UserInfo.dwGameID=pUserInfoHead->dwGameID;
	//	UserInfo.dwUserID=pUserInfoHead->dwUserID;
	//	UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
	//	UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

	//	//�û�״̬
	//	UserInfo.wTableID=pUserInfoHead->wTableID;
	//	UserInfo.wChairID=pUserInfoHead->wChairID;
	//	UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

	//	//�û�����
	//	UserInfo.cbGender=pUserInfoHead->cbGender;
	//	UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	//	UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

	//	//�û�����
	//	UserInfo.lScore=pUserInfoHead->lScore;
	//	UserInfo.lGrade=pUserInfoHead->lGrade;
	//	UserInfo.lInsure=pUserInfoHead->lInsure;
	//	UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
	//	UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
	//	UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
	//	UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;
	//	UserInfo.dwUserMedal=pUserInfoHead->dwUserMedal;
	//	UserInfo.dwExperience=pUserInfoHead->dwExperience;
	//	UserInfo.lLoveLiness=pUserInfoHead->lLoveLiness;

	//	//��������
	//	void * dataBuffer=0;
	//	tagDataDescribe DataDescribe;
	//	CRecvPacketHelper RecvPacket(pUserInfoHead+1,dataSize-sizeof(tagUserInfoHead));

	//	//��չ��Ϣ
	//	while (true)
	//	{
	//		dataBuffer=RecvPacket.GetData(DataDescribe);
	//		if (DataDescribe.wDataDescribe==DTP_NULL) break;
	//		switch (DataDescribe.wDataDescribe)
	//		{
	//		case DTP_GR_NICK_NAME_SHZ:		//�û��ǳ�
	//			{
	//				ASSERT(dataBuffer!=0);
	//				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
	//				if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
	//				{
	//					memcpy(&UserInfo.szNickName,dataBuffer,DataDescribe.wDataSize);
	//					UserInfo.szNickName[countarray(UserInfo.szNickName)-1]=0;
	//				}
	//				break;
	//			}
	//		case DTP_GR_GROUP_NAME_SHZ:		//�û�����
	//			{
	//				ASSERT(dataBuffer!=0);
	//				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
	//				if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
	//				{
	//					memcpy(&UserInfo.szGroupName,dataBuffer,DataDescribe.wDataSize);
	//					UserInfo.szGroupName[countarray(UserInfo.szGroupName)-1]=0;
	//				}
	//				break;
	//			}
	//		case DTP_GR_UNDER_WRITE_SHZ:	//����ǩ��
	//			{
	//				ASSERT(dataBuffer!=0);
	//				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
	//				if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
	//				{
	//					memcpy(UserInfo.szUnderWrite,dataBuffer,DataDescribe.wDataSize);
	//					UserInfo.szUnderWrite[countarray(UserInfo.szUnderWrite)-1]=0;
	//				}
	//				break;
	//			}
	//		}
	//	}

	//	//�Զ�ͷ��
	//	if (pUserInfoHead->dwCustomID!=0L)
	//	{
	//		////����ͷ��
	//		//CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	//		//bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

	//		////����ͷ��
	//		//if (bSuccess==false)
	//		//{
	//		//	pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
	//		//}
	//	}
	//}
	//else
	//{
	//	//�û���Ϣ
	//	UserInfo.dwUserID=pUserInfoHead->dwUserID;
	//	tstrcpyn(UserInfo.szNickName,T_T("��Ϸ���"),countarray(UserInfo.szNickName));

	//	//�û�״̬
	//	UserInfo.wTableID=pUserInfoHead->wTableID;
	//	UserInfo.wChairID=pUserInfoHead->wChairID;
	//	UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

	//	//�û�����
	//	UserInfo.cbGender=pUserInfoHead->cbGender;
	//	UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	//	UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	//}

	////�����û�
	//IClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(UserInfo.dwUserID);
	//if (pIClientUserItem==0) pIClientUserItem=mUserManager->ActiveUserItem(UserInfo,CustomFaceInfo);

	////��ȡ����
	//CServerListData * pServerListData=CServerListData::shared();

	////��������
	//if (pServerListData)
	//	pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());

	////��������
	//ASSERT(CParameterGlobal::shared()!=0);
	//CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	////������ʾ
	//if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	//{
	//	if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
	//	{
	//		char szMessage[256]={0};
	//		_sntprintf(szMessage, 256, T_T("���ĺ��� [%s] �����ˣ�"), pIClientUserItem->GetNickName());
	//		//��ʾ��Ϣ
	//		if (mIChatSink)
	//			mIChatSink->InsertSystemChat(szMessage);
	//	}
	//}

	//return true;
}

//�û�����
bool SHZCServerItem::OnSocketSubUserScore(void* data, int dataSize)
{
	tagUserScore UserScore;

  	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	UserScore.lScore = packet.read8Byte();
	UserScore.lGrade = packet.read8Byte();
	UserScore.lInsure = packet.read8Byte();
	UserScore.dwWinCount = packet.read4Byte();
	UserScore.dwLostCount = packet.read4Byte();
	UserScore.dwDrawCount = packet.read4Byte();
	UserScore.dwFleeCount = packet.read4Byte();
	UserScore.dwUserMedal = packet.read4Byte();
	UserScore.dwExperience = packet.read4Byte();
	UserScore.lLoveLiness = packet.read4Byte();

	//Ѱ���û�
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//�û��ж�
	//ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return true;

	//��������
	SHZCGlobalUserInfo * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	bool bMySelfUserItem=pGlobalUserData->dwUserID==dwUserID;
	bool bAvertCheatMode=SHZCServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule);
	bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(mMeUserItem->GetMasterOrder()>0));

	//�����û�
	if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		mUserManager->UpdateUserItemScore(pIClientUserItem,&UserScore);
	}

	return true;

	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_UserScore));
	//if (dataSize<sizeof(CMD_GR_UserScore)) return false;

	////Ѱ���û�
	//CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)data;
	//IClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(pUserScore->dwUserID);

	////�û��ж�
	//ASSERT(pIClientUserItem!=0);
	//if (pIClientUserItem==0) return true;

	////��������
	//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	////��������
	//bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserScore->dwUserID;
	//bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule);
	//bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(mMeUserItem->GetMasterOrder()>0));

	////�����û�
	//if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	//{
	//	mUserManager->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);
	//}

	//return true;
}

//�û�״̬
bool SHZCServerItem::OnSocketSubUserStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubUserStatus\n");
	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	tagUserStatus UserStatus;
	UserStatus.wTableID = packet.read2Byte();
	UserStatus.wChairID = packet.read2Byte();
	UserStatus.cbUserStatus = packet.readByte();

	//Ѱ���û�
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//�û��ж�
	//ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return true;

	//����״̬
	if (UserStatus.cbUserStatus==US_NULL) 
	{
		//ɾ���û�
		mUserManager->DeleteUserItem(pIClientUserItem);

		//��ȡ����
		HallServerListData * pServerListData=HallServerListData::shared();

		//��������
		pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());
	}
	else
	{
		//�����û�
		mUserManager->UpdateUserItemStatus(pIClientUserItem,&UserStatus);
	}

	return true;

	//_tprintf(T_T("CServerItem::OnSocketSubUserStatus\n"));
	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_UserStatus));
	//if (dataSize<sizeof(CMD_GR_UserStatus)) return false;

	////Ѱ���û�
	//CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)data;
	//IClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(pUserStatus->dwUserID);

	////�û��ж�
	//ASSERT(pIClientUserItem!=0);
	//if (pIClientUserItem==0) return true;

	////����״̬
	//if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	//{
	//	//ɾ���û�
	//	mUserManager->DeleteUserItem(pIClientUserItem);

	//	//��ȡ����
	//	CServerListData * pServerListData=CServerListData::shared();

	//	//��������
	//	pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());
	//}
	//else
	//{
	//	//�����û�
	//	mUserManager->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	//}

	//return true;
}

//////////////////////////////////////////////////////////////////////////
//������Ϣ
//�û�����
bool SHZCServerItem::OnSocketSubUserChat(void* data, int dataSize)
{
	if (mIChatSink==0)
		return true;

	PACKET_AIDE_DATA(data);
	word wChatLength = packet.read2Byte();
	dword dwChatColor = packet.read4Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	u2string str;
	str.resize(wChatLength+1,'\0');
	packet.readUTF16(&str[0], wChatLength);
	const std::string sString = u2_8(&str[0]);

	dwChatColor = ((dwChatColor & 0xFF) << 16) | (dwChatColor >> 8);

	//��ȡ�û�
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSendUserID);
	if (pISendUserItem==0) return true;

	//��Ϣ����
	if (pISendUserItem!=mMeUserItem)
	{
		//���ű���
		dword dwUserGroupID=pISendUserItem->GetGroupID();
		dword dwMySelfGroupID=mMeUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		byte cbUserCompanion=pISendUserItem->GetUserCompanion();
		SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//��ʾ��Ϣ
	if (dwTargetUserID!=0L)
	{
		//��ȡ�û�
		SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
		if (pIRecvUserItem==0) return true;

		mIChatSink->InsertUserChat(
			pISendUserItem->GetNickName(),
			pIRecvUserItem->GetNickName(),sString.c_str(),dwChatColor);
	}
	else 
	{
		//��ʾ��Ϣ
		
		mIChatSink->InsertUserChat(pISendUserItem->GetNickName(), sString.c_str(),dwChatColor);
	}

	return true;

	////��������
	//CMD_GR_S_UserChat * pUserChat=(CMD_GR_S_UserChat *)data;

	////Ч�����
	//ASSERT(dataSize>=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)));
	//ASSERT(dataSize==(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	////Ч�����
	//if (dataSize<(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	//if (dataSize!=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	////��ȡ�û�
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pUserChat->dwSendUserID);
	//if (pISendUserItem==0) return true;

	////��Ϣ����
	//if (pISendUserItem!=mMeUserItem)
	//{
	//	//���ű���
	//	dword dwUserGroupID=pISendUserItem->GetGroupID();
	//	dword dwMySelfGroupID=mMeUserItem->GetGroupID();

	//	//�����ж�
	//	bool bSameGroup=false;
	//	if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

	//	//��������
	//	byte cbUserCompanion=pISendUserItem->GetUserCompanion();
	//	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//	//����ģʽ
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//���ģʽ
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}
	//}

	////��ʾ��Ϣ
	//if (pUserChat->dwTargetUserID!=0L)
	//{
	//	//��ȡ�û�
	//	IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pUserChat->dwTargetUserID);
	//	if (pIRecvUserItem==0) return true;

	//	//��ʾ��Ϣ
	//	const char* pszSendUser=pISendUserItem->GetNickName();
	//	const char* pszRecvUser=pIRecvUserItem->GetNickName();
	//	dword dwColor = pUserChat->dwChatColor;
	//	// ��ɫ����
	//	dwColor = ((dwColor & 0xFF) << 16) | (dwColor >> 8);

	//	if (mIChatSink)
	//		mIChatSink->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,dwColor);
	//}
	//else 
	//{
	//	//��ʾ��Ϣ
	//	const char* pszSendUser=pISendUserItem->GetNickName();
	//	dword dwColor = pUserChat->dwChatColor;
	//	// ��ɫ����
	//	dwColor = ((dwColor & 0xFF) << 16) | (dwColor >> 8);

	//	if (mIChatSink)
	//		mIChatSink->InsertUserChat(pszSendUser,pUserChat->szChatString,dwColor);
	//}

	//return true;
}

//�û�����
bool SHZCServerItem::OnSocketSubExpression(void* data, int dataSize)
{
	if (mIChatSink==0)
		return true;
	PACKET_AIDE_DATA(data);
	word wItemIndex = packet.read2Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	//��ȡ�û�
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSendUserID);
	if (pISendUserItem==0) return true;

	//��Ϣ����
	if (pISendUserItem!=mMeUserItem)
	{
		//���ű���
		dword dwUserGroupID=pISendUserItem->GetGroupID();
		dword dwMySelfGroupID=mMeUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		byte cbUserCompanion=pISendUserItem->GetUserCompanion();
		SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//������Ϣ
	if (dwTargetUserID==0L)
	{
		mIChatSink->InsertExpression(pISendUserItem->GetNickName(), wItemIndex);
		return true;
	}

	//˽����Ϣ
	if (dwTargetUserID!=0L)
	{
		//��ȡ�û�
		SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
		if (pIRecvUserItem==0) return true;

		//��ʾ��Ϣ
		mIChatSink->InsertExpression(pISendUserItem->GetNickName(),pIRecvUserItem->GetNickName(),wItemIndex);
	}

	return true;

	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_S_UserExpression));
	//if (dataSize!=sizeof(CMD_GR_S_UserExpression)) return false;

	////��������
	//CMD_GR_S_UserExpression * pUserExpression=(CMD_GR_S_UserExpression *)data;

	////��ȡ�û�
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pUserExpression->dwSendUserID);
	//if (pISendUserItem==0) return true;

	////��Ϣ����
	//if (pISendUserItem!=mMeUserItem)
	//{
	//	//���ű���
	//	dword dwUserGroupID=pISendUserItem->GetGroupID();
	//	dword dwMySelfGroupID=mMeUserItem->GetGroupID();

	//	//�����ж�
	//	bool bSameGroup=false;
	//	if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

	//	//��������
	//	byte cbUserCompanion=pISendUserItem->GetUserCompanion();
	//	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//	//����ģʽ
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//���ģʽ
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}
	//}

	////������Ϣ
	//if (pUserExpression->dwTargetUserID==0L)
	//{
	//	if (mIChatSink)
	//		mIChatSink->InsertExpression(pISendUserItem->GetNickName(), pUserExpression->wItemIndex);
	//}

	////˽����Ϣ
	//if (pUserExpression->dwTargetUserID!=0L)
	//{
	//	//��ȡ�û�
	//	IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pUserExpression->dwTargetUserID);
	//	if (pIRecvUserItem==0) return true;

	//	//��ʾ��Ϣ
	//	const char* pszSendUser=pISendUserItem->GetNickName();
	//	const char* pszRecvUser=pIRecvUserItem->GetNickName();
	//	if (mIChatSink)
	//		mIChatSink->InsertExpression(pszSendUser,pszRecvUser,pUserExpression->wItemIndex);
	//}

	//return true;
}

//�û�˽��
bool SHZCServerItem::OnSocketSubWisperUserChat(void* data, int dataSize)
{
	if (mIChatSink==0)
		return true;

	PACKET_AIDE_DATA(data);
	word wChatLength = packet.read2Byte();
	dword dwChatColor = packet.read4Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	u2string str;
	str.resize(wChatLength+1,'\0');
	packet.readUTF16(&str[0], wChatLength);
	const std::string sString = u2_8(&str[0]);

	dwChatColor = ((dwChatColor & 0xFF) << 16) | (dwChatColor >> 8);

	//��ȡ�û�
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSendUserID);
	if (pISendUserItem==0) return true;

	//�����û�
	SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
	if (pIRecvUserItem==0) return true;

	//��Ϣ����
	if (pISendUserItem!=mMeUserItem)
	{
		//���ű���
		dword dwUserGroupID=pISendUserItem->GetGroupID();
		dword dwMySelfGroupID=mMeUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		byte cbUserCompanion=pISendUserItem->GetUserCompanion();
		SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//�����û�
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	mIChatSink->InsertWhisperChat(pISendUserItem->GetNickName(), sString.c_str(), dwChatColor, pISendUserItem == mMeUserItem);
	return true;

	////��������
	//CMD_GR_S_WisperChat * pWisperChat=(CMD_GR_S_WisperChat *)data;

	////Ч�����
	//ASSERT(dataSize>=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)));
	//ASSERT(dataSize==(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	////Ч�����
	//if (dataSize<(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	//if (dataSize!=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	////��ȡ�û�
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pWisperChat->dwSendUserID);
	//if (pISendUserItem==0) return true;

	////�����û�
	//IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pWisperChat->dwTargetUserID);
	//if (pIRecvUserItem==0) return true;

	////��Ϣ����
	//if (pISendUserItem!=mMeUserItem)
	//{
	//	//���ű���
	//	dword dwUserGroupID=pISendUserItem->GetGroupID();
	//	dword dwMySelfGroupID=mMeUserItem->GetGroupID();

	//	//�����ж�
	//	bool bSameGroup=false;
	//	if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

	//	//��������
	//	byte cbUserCompanion=pISendUserItem->GetUserCompanion();
	//	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//	//����ģʽ
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//���ģʽ
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//�����û�
	//	if(cbUserCompanion == CP_SHIELD) return true;
	//}

	//dword dwColor = pWisperChat->dwChatColor;
	//// ��ɫ����
	//dwColor = ((dwColor & 0xFF) << 16) | (dwColor >> 8);
	//if (mIChatSink)
	//	mIChatSink->InsertWhisperChat(pISendUserItem->GetNickName(), pWisperChat->szChatString, dwColor, pISendUserItem == mMeUserItem);
	//return true;
}

//˽�ı���
bool SHZCServerItem::OnSocketSubWisperExpression(void* data, int dataSize)
{
	if (mIChatSink==0)
		return true;
	PACKET_AIDE_DATA(data);
	word wItemIndex = packet.read2Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	//��ȡ�û�
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSendUserID);
	if (pISendUserItem==0) return true;

	//�����û�
	SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
	if (pIRecvUserItem==0) return true;

	//��Ϣ����
	if (pISendUserItem!=mMeUserItem)
	{
		//���ű���
		dword dwUserGroupID=pISendUserItem->GetGroupID();
		dword dwMySelfGroupID=mMeUserItem->GetGroupID();

		//�����ж�
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//��������
		byte cbUserCompanion=pISendUserItem->GetUserCompanion();
		SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

		//����ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//�����û�
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	mIChatSink->InsertExpression(pISendUserItem->GetNickName(), pIRecvUserItem->GetNickName(), wItemIndex);

	return true;
	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_S_WisperExpression));
	//if (dataSize!=sizeof(CMD_GR_S_WisperExpression)) return false;

	////��������
	//CMD_GR_S_WisperExpression * pWisperExpression=(CMD_GR_S_WisperExpression *)data;

	////��ȡ�û�
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pWisperExpression->dwSendUserID);
	//if (pISendUserItem==0) return true;

	////�����û�
	//IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pWisperExpression->dwTargetUserID);
	//if (pIRecvUserItem==0) return true;

	////��Ϣ����
	//if (pISendUserItem!=mMeUserItem)
	//{
	//	//���ű���
	//	dword dwUserGroupID=pISendUserItem->GetGroupID();
	//	dword dwMySelfGroupID=mMeUserItem->GetGroupID();

	//	//�����ж�
	//	bool bSameGroup=false;
	//	if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

	//	//��������
	//	byte cbUserCompanion=pISendUserItem->GetUserCompanion();
	//	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//	//����ģʽ
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//���ģʽ
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//�����û�
	//	if(cbUserCompanion == CP_SHIELD) return true;
	//}

	//if (mIChatSink)
	//	mIChatSink->InsertExpression(pISendUserItem->GetNickName(), pIRecvUserItem->GetNickName(), pWisperExpression->wItemIndex);

	//return true;
}

//////////////////////////////////////////////////////////////////////////
//������Ϣ
//���߳ɹ�
bool SHZCServerItem::OnSocketSubPropertySuccess(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	byte cbRequestArea = packet.readByte();
	word wItemCount = packet.read2Byte();
	word wPropertyIndex = packet.read2Byte();
	dword dwSourceUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	//��ȡ�û�
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSourceUserID);
	if (pISendUserItem==0) return true;

	//��ȡ�û�
	SHZIClientUserItem * pTargetUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
	if (pTargetUserItem==0) return true;

	//ʹ�÷�Χ
	if(cbRequestArea==PT_ISSUE_AREA_GAME)
	{
		PLAZZ_PRINTF("%s", a_u8("��ϲ��,��Ϸ���߹���ɹ�!\n"));
		//if (mIPropertySink)
		//	mIPropertySink->onPropertyBuySuccess(
		//		pPropertySuccess->wPropertyIndex,
		//		pPropertySuccess->wItemCount, 
		//		pPropertySuccess->dwSourceUserID,
		//		pPropertySuccess->dwTargetUserID);
		return true;
	}

	PLAZZ_PRINTF("%s", a_u8("��ϲ��,������߹���ɹ�!\n"));

	////��ȡ����
	//ASSERT(CGamePropertyManager::GetInstance()!=0);
	//CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	////Ѱ�ҵ���
	//ASSERT(pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex)!=0);
	//CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex);

	////���ߴ���
	//if (pGamePropertyItem!=0 && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	//{
	//	//ͼƬ��Ϣ
	//	tagPropertyImage PropertyImage;
	//	pGamePropertyItem->GetImageInfo(PropertyImage);

	//	//���Ŷ���
	//	if ((PropertyImage.pszFlash!=0)&&(PropertyImage.pszFlash[0]!=0))
	//	{
	//		//����·��
	//		char szDirectory[MAX_PATH]=T_T("");
	//		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//		//����·��
	//		char szFlashPath[MAX_PATH]=T_T("");
	//		_sntprintf(szFlashPath,CountArray(szFlashPath),T_T("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

	//		try
	//		{
	//			//���Ŷ���
	//			if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
	//			{
	//				throw T_T("�����ļ�����ʧ��");
	//			}

	//			//�����ؼ�
	//			CRect rcWindow;
	//			m_TableViewFrame.GetWindowRect(&rcWindow);
	//			m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

	//			//���Ŷ���
	//			m_FlashControl.PlayMovie();
	//			m_FlashControl.ShowControl(SW_SHOW);
	//		}
	//		catch (...)
	//		{
	//			ASSERT(FALSE);
	//		}
	//	}
	//}

	////������ʾ
	//if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	//{
	//	//��������
	//	char  szMessage[256]=T_T("");

	//	//��ȡ��ʾ
	//	if(pPropertySuccess->dwSourceUserID==mMeUserItem->GetUserID() && pPropertySuccess->dwSourceUserID!=pPropertySuccess->dwTargetUserID)
	//		_sntprintf(szMessage,CountArray(szMessage),T_T("��ϲ��,���߹���ɹ�!"));
	//	else
	//		pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),mMeUserItem->GetNickName(),pPropertySuccess->wItemCount,szMessage);

	//	//������Ϣ
	//	CInformation Information;
	//	Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);

	//	//��������
	//	if(m_pTrumpetItem!=0)  m_pTrumpetItem->UpdateControlSurface();
	//}

	return true;

	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_S_PropertySuccess));
	//if (dataSize!=sizeof(CMD_GR_S_PropertySuccess)) return false;

	////��������
	//CMD_GR_S_PropertySuccess * pPropertySuccess=(CMD_GR_S_PropertySuccess *)data;

	////��ȡ�û�
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pPropertySuccess->dwSourceUserID);
	//if (pISendUserItem==0) return true;

	////��ȡ�û�
	//IClientUserItem * pTargetUserItem=mUserManager->SearchUserByUserID(pPropertySuccess->dwTargetUserID);
	//if (pTargetUserItem==0) return true;

	////ʹ�÷�Χ
	//if(pPropertySuccess->cbRequestArea==PT_ISSUE_AREA_GAME)
	//{
	//	_tprintf(T_T("��ϲ��,��Ϸ���߹���ɹ�!\n"));
	//	//if (mIPropertySink)
	//	//	mIPropertySink->onPropertyBuySuccess(
	//	//		pPropertySuccess->wPropertyIndex,
	//	//		pPropertySuccess->wItemCount, 
	//	//		pPropertySuccess->dwSourceUserID,
	//	//		pPropertySuccess->dwTargetUserID);
	//	return true;
	//}

	//_tprintf(T_T("��ϲ��,������߹���ɹ�!\n"));

	//////��ȡ����
	////ASSERT(CGamePropertyManager::GetInstance()!=0);
	////CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//////Ѱ�ҵ���
	////ASSERT(pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex)!=0);
	////CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex);

	//////���ߴ���
	////if (pGamePropertyItem!=0 && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	////{
	////	//ͼƬ��Ϣ
	////	tagPropertyImage PropertyImage;
	////	pGamePropertyItem->GetImageInfo(PropertyImage);

	////	//���Ŷ���
	////	if ((PropertyImage.pszFlash!=0)&&(PropertyImage.pszFlash[0]!=0))
	////	{
	////		//����·��
	////		char szDirectory[MAX_PATH]=T_T("");
	////		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	////		//����·��
	////		char szFlashPath[MAX_PATH]=T_T("");
	////		_sntprintf(szFlashPath,CountArray(szFlashPath),T_T("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

	////		try
	////		{
	////			//���Ŷ���
	////			if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
	////			{
	////				throw T_T("�����ļ�����ʧ��");
	////			}

	////			//�����ؼ�
	////			CRect rcWindow;
	////			m_TableViewFrame.GetWindowRect(&rcWindow);
	////			m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

	////			//���Ŷ���
	////			m_FlashControl.PlayMovie();
	////			m_FlashControl.ShowControl(SW_SHOW);
	////		}
	////		catch (...)
	////		{
	////			ASSERT(FALSE);
	////		}
	////	}
	////}

	//////������ʾ
	////if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	////{
	////	//��������
	////	char  szMessage[256]=T_T("");

	////	//��ȡ��ʾ
	////	if(pPropertySuccess->dwSourceUserID==mMeUserItem->GetUserID() && pPropertySuccess->dwSourceUserID!=pPropertySuccess->dwTargetUserID)
	////		_sntprintf(szMessage,CountArray(szMessage),T_T("��ϲ��,���߹���ɹ�!"));
	////	else
	////		pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),mMeUserItem->GetNickName(),pPropertySuccess->wItemCount,szMessage);

	////	//������Ϣ
	////	CInformation Information;
	////	Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);

	////	//��������
	////	if(m_pTrumpetItem!=0)  m_pTrumpetItem->UpdateControlSurface();
	////}

	//return true;
}

//����ʧ��
bool SHZCServerItem::OnSocketSubPropertyFailure(void* data, int dataSize)
{
	//��������
	PACKET_AIDE_DATA(data);
	word wRequestArea = packet.read2Byte();
	int lErrorCode = packet.read4Byte();

	int len = (dataSize-packet.getPosition())/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0], len);
	const std::string sString = u2_8(&str[0]);
	
	//ʹ�÷�Χ
	if(wRequestArea==PT_ISSUE_AREA_GAME)
	{
		PLAZZ_PRINTF("%s", a_u8("��Ϸ���߹���ʧ��! [%d] %s \n"), lErrorCode, sString.c_str());
		//if (mIPropertySink)
		//	mIPropertySink->onPropertyBuyFailure(
		//		pPropertyFailure->lErrorCode,
		//		pPropertyFailure->szDescribeString);
		return true;
	}

	PLAZZ_PRINTF("%s", a_u8("������߹���ʧ��! [%d] %s \n"), lErrorCode, sString.c_str());

	return true;
	////��������
	//CMD_GR_PropertyFailure * pPropertyFailure=(CMD_GR_PropertyFailure *)data;
	//if(pPropertyFailure==0) return false;

	////ʹ�÷�Χ
	//if(pPropertyFailure->wRequestArea==PT_ISSUE_AREA_GAME)
	//{
	//	_tprintf(T_T("��Ϸ���߹���ʧ��! [%d] %s \n"), pPropertyFailure->lErrorCode, pPropertyFailure->szDescribeString);
	//	//if (mIPropertySink)
	//	//	mIPropertySink->onPropertyBuyFailure(
	//	//		pPropertyFailure->lErrorCode,
	//	//		pPropertyFailure->szDescribeString);
	//	return true;
	//}

	//_tprintf(T_T("������߹���ʧ��! [%d] %s \n"), 
	//	pPropertyFailure->lErrorCode, 
	//	pPropertyFailure->szDescribeString);

	//return true;
}

//����ЧӦ
bool SHZCServerItem::OnSocketSubPropertyEffect(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	byte cbMemberOrder = packet.readByte();

	//�����û�
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwUserID);
	if(pISendUserItem==0) return true;

	//���ñ���
	tagSHZ_UserInfo * pUserInfo = pISendUserItem->GetUserInfo();
	pUserInfo->cbMemberOrder=cbMemberOrder;

	////������Ϣ
	//m_UserListControl.UpdateDataItem(pISendUserItem);

	return true;
	////����У��
	//ASSERT(dataSize==sizeof(CMD_GR_S_PropertyEffect));
	//if(dataSize!=sizeof(CMD_GR_S_PropertyEffect))  return false;

	////��������
	//CMD_GR_S_PropertyEffect * pPropertyEffect=(CMD_GR_S_PropertyEffect *)data;
	//if(pPropertyEffect==0) return false;

	////�����û�
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pPropertyEffect->wUserID);
	//if(pISendUserItem==0) return true;

	////���ñ���
	//tagUserInfo * pUserInfo = pISendUserItem->GetUserInfo();
	//pUserInfo->cbMemberOrder=pPropertyEffect->cbMemberOrder;

	//////������Ϣ
	////m_UserListControl.UpdateDataItem(pISendUserItem);

	//return true;
}

//������Ϣ
bool SHZCServerItem::OnSocketSubPropertyMessage(void* data, int dataSize)
{
	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_S_PropertyMessage));
	//if (dataSize!=sizeof(CMD_GR_S_PropertyMessage)) return false;

	////У������
	//CMD_GR_S_PropertyMessage * pPropertyMessage = (CMD_GR_S_PropertyMessage *)data;
	//if(pPropertyMessage==0) return false;

	////��ȡ����
	//ASSERT(CGamePropertyManager::GetInstance()!=0);
	//CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	////Ѱ�ҵ���
	//ASSERT(pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex)!=0);
	//CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex);
	//if(pGamePropertyItem==0) return true;

	////�����ж�
	//if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY) return true;

	////��ȡ�û�
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserItemByUserID(pPropertyMessage->dwSourceUserID);
	//if (pISendUserItem==0) return true;

	////��ȡ�û�
	//IClientUserItem * pIRecvUserItem=mUserManager->SearchUserItemByUserID(pPropertyMessage->dwTargerUserID);
	//if (pIRecvUserItem==0) return true;

	////��ȡ��Ϣ
	//char szNotifyMessage[128]=T_T("");	
	//pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),pIRecvUserItem->GetNickName(),pPropertyMessage->wPropertyCount,szNotifyMessage);

	////������Ϣ
	//m_ChatMessage.InsertPromptString(szNotifyMessage);

	//if(IsGameClientReady())
	//{
	//	//����ṹ
	//	IPC_GR_PropertyMessage  PropertyMessage;
	//	PropertyMessage.wPropertyIndex=pPropertyMessage->wPropertyIndex;
	//	PropertyMessage.wPropertyCount=pPropertyMessage->wPropertyCount;
	//	memcpy(PropertyMessage.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertyMessage.szSourceNickName));
	//	memcpy(PropertyMessage.szTargerNickName,pIRecvUserItem->GetNickName(),sizeof(PropertyMessage.szTargerNickName));

	//	//��������
	//	m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	//}

	return true;
}

//��������
bool SHZCServerItem::OnSocketSubPropertyTrumpet(void* data, int dataSize)
{
	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_S_SendTrumpet));
	//if (dataSize!=sizeof(CMD_GR_S_SendTrumpet)) return false;

	////��������
	//CMD_GR_S_SendTrumpet * pSendTrumpet=(CMD_GR_S_SendTrumpet *)data;

	////������Ϣ
	//if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)
	//	m_ChatMessage.InsertUserTrumpet(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	////������Ϣ
	//if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON)
	//	m_ChatMessage.InsertUserTyphon(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	////���°���
	//if(pSendTrumpet->dwSendUserID==mMeUserItem->GetUserID())
	//{
	//	//���߰���
	//	tagPropertyPackage * pPropertyPackage=mMeUserItem->GetPropertyPackage();

	//	//���°���
	//	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET) --pPropertyPackage->wTrumpetCount;
	//	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON) --pPropertyPackage->wTyphonCount;
	//	if(m_pTrumpetItem!=0) m_pTrumpetItem->UpdateControlSurface();
	//}

	//if(IsGameClientReady())
	//{
	//	//����ṹ
	//	IPC_GR_PropertyTrumpet  PropertyTrumpet;
	//	PropertyTrumpet.dwSendUserID=pSendTrumpet->dwSendUserID;
	//	PropertyTrumpet.wPropertyIndex=pSendTrumpet->wPropertyIndex;
	//	PropertyTrumpet.TrumpetColor=pSendTrumpet->TrumpetColor;
	//	memcpy(PropertyTrumpet.szSendNickName,pSendTrumpet->szSendNickName,sizeof(PropertyTrumpet.szSendNickName));
	//	memcpy(PropertyTrumpet.szTrumpetContent,pSendTrumpet->szTrumpetContent,sizeof(PropertyTrumpet.szTrumpetContent));

	//	//��������
	//	m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET,&PropertyTrumpet,sizeof(PropertyTrumpet));
	//}

	////��������
	//ASSERT(CParameterGlobal::shared()!=0);
	//CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	////��������
	//if(pParameterGlobal->m_bAllowSound)
	//{
	//	//��������
	//	HINSTANCE hInstance = AfxGetInstanceHandle();
	//	bool bPlaying=false;

	//	////��ȡ����
	//	//CD3DSound * pD3DSound=CD3DSound::GetInstance();
	//	//if(pD3DSound)
	//	//{
	//	//	bPlaying = pD3DSound->PlaySound(hInstance, (pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?T_T("BUGLE"):T_T("BUGLE_EX"), T_T("WAVE"), false);
	//	//}

	//	//ϵͳ����
	//	if(!bPlaying)
	//	{
	//		HRSRC hResour=FindResource(hInstance,(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?T_T("BUGLE"):T_T("BUGLE_EX"),T_T("WAVE"));
	//		if (hResour!=0)
	//		{
	//			HGLOBAL hGlobal=LoadResource(hInstance,hResour);
	//			if (hGlobal!=0) 
	//			{
	//				//��������
	//				const char* pszMemory=(const char*)LockResource(hGlobal);
	//				BOOL bResult = ::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_NODEFAULT);

	//				//������Դ
	//				UnlockResource(hGlobal);
	//			}
	//			FreeResource(hGlobal);
	//		}
	//	}
	//}

	return true;
}

//ϲ����Ϣ
bool SHZCServerItem::OnSocketSubGladMessage(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	u2string str;

	str.resize(128+1,'\0');
	packet.readUTF16(&str[0], 128);
	std::string sContent = u2_8(&str[0]);

	str.resize(32+1,'\0');
	packet.readUTF16(&str[0], 32);
	std::string sNickName = u2_8(&str[0]);

	str.resize(16+1,'\0');
	packet.readUTF16(&str[0], 16);
	std::string sNum = u2_8(&str[0]);

	dword colText = COL_N2G(packet.read4Byte());
	dword colName = COL_N2G(packet.read4Byte());
	dword colNum  = COL_N2G(packet.read4Byte());
	

	if (mIStringMessageSink)
		mIStringMessageSink->InsertGladString(sContent.c_str(), sNickName.c_str(), sNum.c_str(), colText, colName, colNum);
	return true;
}