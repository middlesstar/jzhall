#include "CClientKernel_SHZ.h"

SHZIGameFrameSink* SHZIGameFrameSink::getInterface()
{
	return (SHZIGameFrameSink*)SHZIClientKernel::get();
}

//��������
void SHZCClientKernel::OnGFConfigServer(const tagUserAttribute& UserAttribute, const tagServerAttribute& ServerAttribute)
{
	//PLAZZ_PRINTF("flow->CClientKernel::OnGFConfigServer\n");

	memcpy(&mUserAttribute, &UserAttribute, sizeof(mUserAttribute));
	memcpy(&mServerAttribute, &ServerAttribute, sizeof(mServerAttribute));
}

//�������
void SHZCClientKernel::OnGFConfigFinish()
{
	//PLAZZ_PRINTF("flow->CClientKernel::OnGFConfigFinish\n");

	if (mIClientKernelSink && !mIClientKernelSink->SetupGameClient())
		return;
	SendGameOption();
}

bool SHZCClientKernel::OnGFEventSocket(int main, int sub, void* data, int dataSize)
{
	//PLAZZ_PRINTF("flow->CClientKernel::OnGFEventSocket\n");
	
	//��Ϸ��Ϣ
	if (main==MDM_GF_GAME_SHZ)
	{
		//Ч��״̬
		ASSERT(mIClientKernelSink!=0);
		if (mIClientKernelSink==0) 
			return false;
		return mIClientKernelSink->OnEventGameMessage(sub,data,dataSize);
	}

	//��Ϸ��Ϣ
	if (main==MDM_GR_INSURE_SHZ)
	{
		PLAZZ_PRINTF("flow->CClientKernel::OnGFEventSocket MDM_GR_INSURE_SHZ\n");
		//if(mIClientKernelSink)
		//	mIClientKernelSink->OnEventInsureMessage(sub,data,dataSize);

		return true;
	}

	//�ں˴���
	if (main==MDM_GF_FRAME_SHZ)
	{
		switch (sub)
		{
		case SUB_GF_USER_CHAT_SHZ:			//�û�����
			{
				return OnSocketSubUserChat(data,dataSize);
			}
		case SUB_GF_USER_EXPRESSION_SHZ:	//�û�����
			{
				return OnSocketSubExpression(data,dataSize);
			}
		case SUB_GF_GAME_STATUS_SHZ:		//��Ϸ״̬
			{
				return OnSocketSubGameStatus(data,dataSize);
			}
		case SUB_GF_GAME_SCENE_SHZ:			//��Ϸ����
			{
				return OnSocketSubGameScene(data,dataSize);
			}
		case SUB_GF_LOOKON_STATUS_SHZ:		//�Թ�״̬
			{
				return OnSocketSubLookonStatus(data,dataSize);
			}
		case SUB_GF_SYSTEM_MESSAGE_SHZ:		//ϵͳ��Ϣ
			{
				return OnSocketSubSystemMessage(data,dataSize);
			}
		case SUB_GF_ACTION_MESSAGE_SHZ:		//������Ϣ
			{
				return OnSocketSubActionMessage(data,dataSize);
			}
		case SUB_GF_USER_READY_SHZ:			//�û�׼��
			{
				if(mMeUserItem ==0 || mMeUserItem->GetUserStatus()>=US_READY)
					return true;
				SendUserReady(0,0);
				if (mIClientKernelSink)
					mIClientKernelSink->OnGFMatchWaitTips(0);
				return true;
			}
		case SUB_GR_USER_WAIT_DISTRIBUTE_SHZ:   //�ȴ���ʾ
			{
				if (mIClientKernelSink)
					mIClientKernelSink->OnGFWaitTips(true);
				return true;
			}
		case SUB_GR_MATCH_INFO_SHZ:				//������Ϣ
			{
				if (!mIClientKernelSink)
					return true;

				PACKET_AIDE_DATA(data);
					
				tagMatchInfo MatchInfo;
				u2string sTitle;

				for (int i = 0; i < 4; ++i)
				{
					sTitle = packet.readUTF16(64);
					strncpy(MatchInfo.szTitle[i], u2_8(&sTitle[0]), countarray(MatchInfo.szTitle[i]));
				}
				
				MatchInfo.wGameCount = packet.read2Byte();
				mIClientKernelSink->OnGFMatchInfo(&MatchInfo);

				////����Ч��
				//ASSERT(dataSize==sizeof(CMD_GR_Match_Info));
				//if(dataSize!=sizeof(CMD_GR_Match_Info)) return false;

				////��������
				//CMD_GR_Match_Info *pMatchInfo=(CMD_GR_Match_Info*)data;
				//if (mIClientKernelSink)
				//	mIClientKernelSink->OnGFMatchInfo(pMatchInfo);
				return true;
			}
		case SUB_GR_MATCH_WAIT_TIP_SHZ:			//�ȴ���ʾ
			{
				if (!mIClientKernelSink)
					return true;

				//���ò���
				if(dataSize!=0)
				{
					PACKET_AIDE_DATA(data);
					tagMatchWaitTip MatchWaitTip;
					MatchWaitTip.lScore = packet.read8Byte();
					MatchWaitTip.wRank = packet.read2Byte();
					MatchWaitTip.wCurTableRank = packet.read2Byte();
					MatchWaitTip.wUserCount = packet.read2Byte();
					MatchWaitTip.wPlayingTable = packet.read2Byte();
					u2string str;
					str.resize(LEN_SERVER+1,'\0');
					packet.readUTF16(&str[0], LEN_SERVER);
					strncpy(MatchWaitTip.szMatchName, u2_8(&str[0]), countarray(MatchWaitTip.szMatchName));
					mIClientKernelSink->OnGFMatchWaitTips(&MatchWaitTip);
				}
				else
				{
					mIClientKernelSink->OnGFMatchWaitTips(0);
				}

				////Ч�����
				//ASSERT(dataSize==sizeof(CMD_GR_Match_Wait_Tip) || dataSize==0);
				//if(dataSize!=sizeof(CMD_GR_Match_Wait_Tip) && dataSize!=0) return false;

				//if (mIClientKernelSink)
				//{
				//	//���ò���
				//	if(dataSize!=0)
				//		mIClientKernelSink->OnGFMatchWaitTips((CMD_GR_Match_Wait_Tip*)data);
				//	else
				//		mIClientKernelSink->OnGFMatchWaitTips(0);
				//}
				return true;
			}
		case SUB_GR_MATCH_RESULT_SHZ:			//�������
			{
				//���ò���
				if (!mIClientKernelSink)
					return true;

				PACKET_AIDE_DATA(data);

				tagMatchResult MatchResult;
				u2string str;
				str.resize(256+1, '\0');
				packet.readUTF16(&str[0], 256);
				strncpy(MatchResult.szDescribe, u2_8(&str[0]), countarray(MatchResult.szDescribe));
				
				MatchResult.dwGold = packet.read4Byte();
				MatchResult.dwMedal = packet.read4Byte();
				MatchResult.dwExperience = packet.read4Byte();

				mIClientKernelSink->OnGFMatchWaitTips(0);
				mIClientKernelSink->OnGFMatchResult(&MatchResult);
				
				////Ч�����
				//ASSERT(dataSize==sizeof(CMD_GR_MatchResult));
				//if(dataSize!=sizeof(CMD_GR_MatchResult)) return false;

				////���ò���
				//if (mIClientKernelSink)
				//{
				//	mIClientKernelSink->OnGFMatchWaitTips(0);
				//	mIClientKernelSink->OnGFMatchResult((CMD_GR_MatchResult*)data);
				//}

				return true;
			}
		}

		return true;
	}

	return false;
}

//�û�����
bool SHZCClientKernel::OnSocketSubUserChat(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubUserChat\n");

	int baseSize=2+4+4+4;
	ASSERT(dataSize>=baseSize);
	if(dataSize<baseSize) return false;

	//��ʾ��Ϣ
	if (mIChatSink==0) return true;

	//��������
	PACKET_AIDE_DATA(data);
	word wChatLength = packet.read2Byte();
	dword dwChatColor = packet.read4Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	int len = (dataSize-baseSize)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0],len);	
	
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

		//����ģʽ
		if ((gGlobalUnits_SHZ.m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
			return true;

		//���ģʽ
		if ((gGlobalUnits_SHZ.m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
			return true;
	}

	//������Ϣ
	if (dwTargetUserID!=0L)
	{
		//��ȡ�û�
		SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);

		if (pIRecvUserItem!=0)
		{
			//������Ϣ
			mIChatSink->InsertUserChat(
				pISendUserItem->GetNickName(), pIRecvUserItem->GetNickName(),
				u2_8(&str[0]), dwChatColor);
		}
	}
	else 
	{
		//������Ϣ
		mIChatSink->InsertUserChat(pISendUserItem->GetNickName(),u2_8(&str[0]), dwChatColor);
	}

	return true;

	////��������
	//CMD_GF_S_UserChat * pUserChat=(CMD_GF_S_UserChat *)data;

	////Ч�����
	//ASSERT(dataSize>=(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)));
	//ASSERT(dataSize==(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	////Ч�����
	//if (dataSize<(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	//if (dataSize!=(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	////��ʾ��Ϣ
	//if (mIChatSink==0) return true;

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
	//		
	//	//����ģʽ
	//	if ((gGlobalUnits.m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//���ģʽ
	//	if ((gGlobalUnits.m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}
	//}

	////������Ϣ
	//if (pUserChat->dwTargetUserID!=0L)
	//{
	//	//��ȡ�û�
	//	IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pUserChat->dwTargetUserID);

	//	if (pIRecvUserItem!=0)
	//	{
	//		//������Ϣ
	//		const tchar* pszSendUser=pISendUserItem->GetNickName();
	//		const tchar* pszRecvUser=pIRecvUserItem->GetNickName();
	//		dword dwColor = ((pUserChat->dwChatColor & 0xFF) << 16) | (pUserChat->dwChatColor >> 8);
	//		mIChatSink->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,dwColor);
	//	}
	//}
	//else 
	//{
	//	//������Ϣ
	//	const tchar* pszSendUser=pISendUserItem->GetNickName();
	//	dword dwColor = ((pUserChat->dwChatColor & 0xFF) << 16) | (pUserChat->dwChatColor >> 8);
	//	mIChatSink->InsertUserChat(pszSendUser,pUserChat->szChatString,dwColor);
	//}

	//return true;
}


//�û�����
bool SHZCClientKernel::OnSocketSubExpression(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubExpression\n");

	//Ч�����
	ASSERT(dataSize==9);
	if (dataSize!=9) return false;

	//��ʾ��Ϣ
	if (mIChatSink==0) return true;

	PACKET_AIDE_DATA(data);
	word wItemIndex=packet.read2Byte();
	dword dwSendUserID=packet.read4Byte();
	dword dwTargetUserID=packet.read4Byte();

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

		//����ģʽ
		if ((gGlobalUnits_SHZ.m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//���ģʽ
		if ((gGlobalUnits_SHZ.m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//������Ϣ
	if (dwTargetUserID!=0L)
	{
		//��ȡ�û�
		SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);

		//������Ϣ
		if (pIRecvUserItem!=0)
		{
			mIChatSink->InsertExpression(pISendUserItem->GetNickName(), pIRecvUserItem->GetNickName(),wItemIndex);
		}
	}
	else
	{
		//������Ϣ
		mIChatSink->InsertExpression(pISendUserItem->GetNickName(),wItemIndex);
	}

	return true;

	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GF_S_UserExpression));
	//if (dataSize!=sizeof(CMD_GF_S_UserExpression)) return false;

	////��ʾ��Ϣ
	//if (mIChatSink==0) return true;

	////��������
	//CMD_GF_S_UserExpression * pUserExpression=(CMD_GF_S_UserExpression *)data;

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
	//	
	//	//����ģʽ
	//	if ((gGlobalUnits.m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//���ģʽ
	//	if ((gGlobalUnits.m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}
	//}

	////������Ϣ
	//if (pUserExpression->dwTargetUserID!=0L)
	//{
	//	//��ȡ�û�
	//	IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pUserExpression->dwTargetUserID);

	//	//������Ϣ
	//	if (pIRecvUserItem!=0)
	//	{
	//		const tchar* pszSendUser=pISendUserItem->GetNickName();
	//		const tchar* pszRecvUser=pIRecvUserItem->GetNickName();
	//		mIChatSink->InsertExpression(pszSendUser,pszRecvUser,pUserExpression->wItemIndex);
	//	}
	//}
	//else
	//{
	//	//������Ϣ
	//	const tchar* pszSendUser=pISendUserItem->GetNickName();
	//	mIChatSink->InsertExpression(pszSendUser,pUserExpression->wItemIndex);
	//}

	//return true;
}

//��Ϸ״̬
bool SHZCClientKernel::OnSocketSubGameStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubGameStatus\n");

	PACKET_AIDE_DATA(data);

	//���ñ���
	mGameStatus=packet.readByte();
	mAllowLookon=packet.readByte()!=0;

	return true;

	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GF_GameStatus));
	//if (dataSize!=sizeof(CMD_GF_GameStatus)) return false;

	////��Ϣ����
	//CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)data;

	////���ñ���
	//mGameStatus=pGameStatus->cbGameStatus;
	//mAllowLookon=pGameStatus->cbAllowLookon?true:false;

	//return true;
}

//��Ϸ����
bool SHZCClientKernel::OnSocketSubGameScene(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubGameScene\n");

	//Ч��״̬
	ASSERT(mMeUserItem!=0);
	if (mMeUserItem==0) return true;

	if (mIClientKernelSink==0)
		return true;
	//��������
	bool bLookonUser=(mMeUserItem->GetUserStatus()==US_LOOKON);
	return mIClientKernelSink->OnEventSceneMessage(mGameStatus,bLookonUser,data,dataSize);
}

//�Թ�״̬
bool SHZCClientKernel::OnSocketSubLookonStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubLookonStatus\n");

	//��Ϣ����
	if (IsLookonMode()==true)
	{
		byte cbAllowLookon = ((unsigned char*)data)[0];
		
		//�¼�����
		mAllowLookon=cbAllowLookon!=0;

		//�¼�֪ͨ
		ASSERT(mMeUserItem!=0);

		if (mIClientKernelSink)
			mIClientKernelSink->OnEventLookonMode(data,dataSize);

		//��ʾ��Ϣ
		if (mAllowLookon==true)
		{
			if (mIStringMessageSink!=0)
				mIStringMessageSink->InsertNormalString(a_u8("��������ۿ������Ϸ"));
		}
		else
		{
			if (mIStringMessageSink!=0)
				mIStringMessageSink->InsertNormalString(a_u8("������ֹ�ۿ������Ϸ"));
		}
	}

	return true;

	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GF_LookonStatus));
	//if (dataSize!=sizeof(CMD_GF_LookonStatus)) return false;

	////��Ϣ����
	//if (IsLookonMode()==true)
	//{
	//	//��������
	//	CMD_GF_LookonStatus * pLookonStatus=(CMD_GF_LookonStatus *)data;

	//	//�¼�����
	//	mAllowLookon=(pLookonStatus->cbAllowLookon==TRUE)?true:false;

	//	//�¼�֪ͨ
	//	ASSERT(mMeUserItem!=0);

	//	if (mIClientKernelSink)
	//		mIClientKernelSink->OnEventLookonMode(data,dataSize);

	//	//��ʾ��Ϣ
	//	if (mAllowLookon==true)
	//	{
	//		if (mIStringMessageSink!=0)
	//			mIStringMessageSink->InsertNormalString(T_T("��������ۿ������Ϸ"));
	//	}
	//	else
	//	{
	//		if (mIStringMessageSink!=0)
	//			mIStringMessageSink->InsertNormalString(T_T("������ֹ�ۿ������Ϸ"));
	//	}
	//}

	//return true;
}

//ϵͳ��Ϣ
bool SHZCClientKernel::OnSocketSubSystemMessage(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubSystemMessage\n");

	PACKET_AIDE_DATA(data);
	word wType = packet.read2Byte();
	word wLength = packet.read2Byte();

	int len=(dataSize-4)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0],len);

	const std::string sString = u2_8(&str[0]);

	//������Ϣ
	if((wType&SMT_TABLE_ROLL_SHZ)!=0)
	{
		//tstring str = pSystemMessage->szString;
		//if(str.find(T_T("ϵͳ����"))>=0)
		//	mIClientKernelSink->OnGFWaitTips(true);

		if (mIClientKernelSink)
			mIClientKernelSink->OnGFTableMessage(sString.c_str());

		if((wType|~SMT_TABLE_ROLL_SHZ)==0)
			return true;
	}

	//�رմ���
	if ((wType&SMT_CLOSE_GAME_SHZ)!=0)
	{
		//�ж�����
		Intermit(SHZ_GameExitCode_Normal);
	}

	//��ʾ��Ϣ
	if (wType&SMT_CHAT_SHZ)
	{
		if (mIStringMessageSink!=0)
			mIStringMessageSink->InsertSystemString(sString.c_str());
	}

	//������Ϣ
	if (wType&SMT_EJECT_SHZ)
	{
		if (mIStringMessageSink!=0)
			mIStringMessageSink->InsertPromptString(sString.c_str(), DLG_MB_OK);
	}

	return true;

	////��������
	//CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)data;
	//word wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	////Ч�����
	//ASSERT((dataSize>wHeadSize)&&(dataSize==(wHeadSize+pSystemMessage->wLength*sizeof(tchar))));
	//if ((dataSize<=wHeadSize)||(dataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(tchar)))) return false;

	////������Ϣ
	//if((pSystemMessage->wType&SMT_TABLE_ROLL_SHZ)!=0)
	//{
	//	tstring str = pSystemMessage->szString;
	//	if(str.find(T_T("ϵͳ����"))>=0)
	//		mIClientKernelSink->OnGFWaitTips(true);

	//	if (mIClientKernelSink)
	//		mIClientKernelSink->OnGFTableMessage(pSystemMessage->szString);

	//	if((pSystemMessage->wType|~SMT_TABLE_ROLL_SHZ)==0)
	//		return true;
	//}
	//	
	////�رմ���
	//if ((pSystemMessage->wType&SMT_CLOSE_GAME_SHZ)!=0)
	//{
	//	//�ж�����
	//	Intermit(0);
	//}

	////��ʾ��Ϣ
	//if (pSystemMessage->wType&SMT_CHAT_SHZ)
	//{
	//	if (mIStringMessageSink!=0)
	//		mIStringMessageSink->InsertSystemString(pSystemMessage->szString);
	//}

	////������Ϣ
	//if (pSystemMessage->wType&SMT_EJECT_SHZ)
	//{
	//	if (mIStringMessageSink!=0)
	//		mIStringMessageSink->InsertPromptString(pSystemMessage->szString, DLG_MB_OK);
	//}

	////�رշ���
	//if (pSystemMessage->wType&SMT_CLOSE_GAME_SHZ)
	//{
	//	Intermit(GameExitCode_Normal);
	//}

	//return true;
}

//������Ϣ
bool SHZCClientKernel::OnSocketSubActionMessage(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubActionMessage\n");

	PACKET_AIDE_DATA(data);
	word wType = packet.read2Byte();
	word wLength = packet.read2Byte();
	uint nButtonType = packet.read2Byte();

	u2string str;
	str.resize(wLength+1,'\0');
	packet.readUTF16(&str[0],wLength);

	const std::string sString = u2_8(&str[0]);

	//�رմ���
	if ((wType&SMT_CLOSE_GAME_SHZ)!=0)
	{
		//�ж�����
		Intermit(SHZ_GameExitCode_Normal);
	}

	return true;
	////��������
	//CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)data;
	//word wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	////Ч�����
	//ASSERT((dataSize>wHeadSize)&&(dataSize>=(wHeadSize+pActionMessage->wLength*sizeof(tchar))));
	//if ((dataSize<=wHeadSize)||(dataSize<(wHeadSize+pActionMessage->wLength*sizeof(tchar)))) return false;

	////�رմ���
	//if ((pActionMessage->wType&SMT_CLOSE_GAME_SHZ)!=0)
	//{
	//	//�ж�����
	//	IntermitConnect();
	//}

	////������Ϣ
	//int nResultCode=ShowInformation(pActionMessage->szString,pActionMessage->nButtonType,0);

	////��������
	//word wExcursion=wHeadSize+pActionMessage->wLength*sizeof(tchar);

	////��ȡ����
	//while (wExcursion<dataSize)
	//{
	//	//��������
	//	tagActionHead * pActionHead=(tagActionHead *)((byte *)data+wExcursion);

	//	//Ч�����
	//	ASSERT((wExcursion+sizeof(tagActionHead))<=dataSize);
	//	ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=dataSize);

	//	//Ч�����
	//	if ((wExcursion+sizeof(tagActionHead))>dataSize) return false;
	//	if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>dataSize) return false;

	//	//��������
	//	if (nResultCode==pActionHead->uResponseID)
	//	{
	//		switch (pActionHead->cbActionType)
	//		{
	//		case ACT_BROWSE_SHZ:	//�������
	//			{
	//				//��������
	//				word wDataPos=wExcursion+sizeof(tagActionHead);
	//				tagActionBrowse * pActionBrowse=(tagActionBrowse *)((byte *)data+wDataPos);

	//				//I E ���
	//				if (pActionBrowse->cbBrowseType&BRT_IE_SHZ)
	//				{
	//					ShellExecute(0,T_T("open"),pActionBrowse->szBrowseUrl,0,0,SW_NORMAL);
	//				}

	//				//�������
	//				if (pActionBrowse->cbBrowseType&BRT_PLAZA_SHZ)
	//				{
	//					//						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true,true);
	//				}

	//				break;
	//			}
	//		case ACT_BROWSE_SHZ:	//���ض���
	//			{
	//				//��������
	//				word wDataPos=wExcursion+sizeof(tagActionHead);
	//				tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((byte *)data+wDataPos);

	//				//I E ����
	//				if (pActionDownLoad->cbDownLoadMode&DLT_IE_SHZ)
	//				{
	//					ShellExecute(0,T_T("open"),pActionDownLoad->szDownLoadUrl,0,0,SW_NORMAL);
	//				}

	//				//����ģ��
	//				if (pActionDownLoad->cbDownLoadMode&DLT_MODULE_SHZ)
	//				{
	//					ShellExecute(0,T_T("open"),pActionDownLoad->szDownLoadUrl,0,0,SW_NORMAL);
	//				}

	//				break;
	//			}
	//		}
	//	}

	//	//����ƫ��
	//	wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	//}

	////�رշ���
	//if (pActionMessage->wType&SMT_CLOSE_GAME_SHZ) m_pIClientKernelSink->CloseGameClient();

	//return true;
}