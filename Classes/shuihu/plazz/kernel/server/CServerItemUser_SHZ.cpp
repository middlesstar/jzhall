#include "CServerItem_SHZ.h"

//////////////////////////////////////////////////////////////////////////
//�û��ӿ�

//�Լ�λ��
word SHZCServerItem::GetMeChairID()
{
	if (mMeUserItem == 0) return INVALID_CHAIR;
	return mMeUserItem->GetChairID();
}

//�Լ�λ��
SHZIClientUserItem * SHZCServerItem::GetMeUserItem()
{
	return mMeUserItem;
}

//��Ϸ�û�
SHZIClientUserItem * SHZCServerItem::GetTableUserItem(word wChairID)
{
	return mUserManager->EnumUserItem(wChairID);
}

//�����û�
SHZIClientUserItem * SHZCServerItem::SearchUserByUserID(dword dwUserID)
{
	return mUserManager->SearchUserByUserID(dwUserID);
}

//�����û�
SHZIClientUserItem * SHZCServerItem::SearchUserByGameID(dword dwGameID)
{
	return mUserManager->SearchUserByGameID(dwGameID);
}

//�����û�
SHZIClientUserItem * SHZCServerItem::SearchUserByNickName(const char* szNickName)
{
	return mUserManager->SearchUserByNickName(szNickName);
}

//////////////////////////////////////////////////////////////////////////
// IUserManagerSink
//////////////////////////////////////////////////////////////////////////
void SHZCServerItem::OnUserItemAcitve(SHZIClientUserItem* pIClientUserItem)
{
	//�ж��Լ�
	if (mMeUserItem==0)
	{
		mMeUserItem=pIClientUserItem;
	}

	////�����б�
	//m_UserListControl.InsertDataItem(pIClientUserItem);

	//��������
	byte cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		word wTableID=pIClientUserItem->GetTableID();
		word wChairID=pIClientUserItem->GetChairID();
		mTableViewFrame.SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	//��ʾ��Ϣ
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();
	if ((pParameterGlobal->m_bNotifyUserInOut == true) && (mServiceStatus == SHZ_ServiceStatus_ServiceIng))
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertUserEnter(pIClientUserItem->GetNickName());
	}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserEnter(pIClientUserItem);
}

void SHZCServerItem::OnUserItemDelete(SHZIClientUserItem* pIClientUserItem)
{
	//��������
	word wLastTableID=pIClientUserItem->GetTableID();
	word wLastChairID=pIClientUserItem->GetChairID();
	dword dwLeaveUserID=pIClientUserItem->GetUserID();
	tagSHZ_UserInfo * pMeUserInfo=mMeUserItem->GetUserInfo();

	//��������
	ASSERT(SHZCParameterGlobal::shared());
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//�뿪����
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//��ȡ�û�
		SHZIClientUserItem * pITableUserItem=mTableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) mTableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,0);

		//�뿪֪ͨ
		if ((pIClientUserItem==mMeUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.cbUserStatus=US_NULL;
			UserStatus.wTableID=INVALID_TABLE;
			UserStatus.wChairID=INVALID_CHAIR;

			if (SHZIClientKernel::get())
				SHZIClientKernel::get()->OnGFUserStatus(pIClientUserItem->GetUserID(), UserStatus);
		}
	}

	////�������
	//m_ChatControl.DeleteUserItem(pIClientUserItem);

	////ɾ���б�
	//m_UserListControl.DeleteDataItem(pIClientUserItem);

	////���Ҵ���
	//CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	//if (pDlgSearchUser!=0) pDlgSearchUser->OnUserItemDelete(pIClientUserItem,this);

	////˽�Ĵ���
	//for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	//{
	//	CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
	//	if (pDlgWhisper->m_hWnd!=0) pDlgWhisper->OnEventUserLeave(pIClientUserItem);
	//}

	////�˵�����
	//for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
	//{
	//	//��ȡ�û�
	//	IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
	//	if (pIChatUserItem->GetUserID()==dwLeaveUserID) m_MenuUserItemArray.RemoveAt(i);
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserDelete(pIClientUserItem);

	//��ʾ��Ϣ
	if ((pParameterGlobal->m_bNotifyUserInOut == true) && (mServiceStatus == SHZ_ServiceStatus_ServiceIng))
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertUserLeave(pIClientUserItem->GetNickName());
	}

}

void SHZCServerItem::OnUserFaceUpdate(SHZIClientUserItem * pIClientUserItem)
{
	//��������
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	////�б����
	//m_UserListControl.UpdateDataItem(pIClientUserItem);

	////���Ҵ���
	//CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	//if (pDlgSearchUser!=0) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	////˽�Ĵ���
	//for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	//{
	//	CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
	//	if (pDlgWhisper->m_hWnd!=0) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//��������
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->cbUserStatus!=US_LOOKON))
	{
		mTableViewFrame.UpdateTableView(pUserInfo->wTableID);
	}

	//������Ϸ
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(mMeUserItem->GetTableID()==pUserInfo->wTableID))
	{
		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->OnGFUserCustomFace(pUserInfo->dwUserID,pUserInfo->dwCustomID,*pCustomFaceInfo);
	}
}

void SHZCServerItem::OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserScore& LastScore)
{
	//��������
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagSHZ_UserInfo * pMeUserInfo=mMeUserItem->GetUserInfo();

	////�б����
	//m_UserListControl.UpdateDataItem(pIClientUserItem);

	////���Ҵ���
	//CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	//if (pDlgSearchUser!=0) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	////˽�Ĵ���
	//for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	//{
	//	CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
	//	if (pDlgWhisper->m_hWnd!=0) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	//}
		
	//�������֪ͨ
	if (pIClientUserItem==mMeUserItem)
	{
		//��������
		SHZCGlobalUserInfo * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
		tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		tagSHZUserInsureInfo * pUserInsureData=pGlobalUserInfo->GetUserInsureInfo();

		//���ñ���
		pUserInsureData->lUserScore+=pIClientUserItem->GetUserScore()-LastScore.lScore;
		pUserInsureData->lUserInsure+=pIClientUserItem->GetUserInsure()-LastScore.lInsure;
		pGlobalUserData->dwUserMedal+=pIClientUserItem->GetUserMedal()-LastScore.dwUserMedal;
	}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//��Ϸ֪ͨ
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//��������
		tagUserScore UserScore;
		zeromemory(&UserScore,sizeof(UserScore));

		//���ñ���
		UserScore.lScore=pUserInfo->lScore;
		UserScore.lGrade=pUserInfo->lGrade;
		UserScore.lInsure=pUserInfo->lInsure;
		UserScore.dwWinCount=pUserInfo->dwWinCount;
		UserScore.dwLostCount=pUserInfo->dwLostCount;
		UserScore.dwDrawCount=pUserInfo->dwDrawCount;
		UserScore.dwFleeCount=pUserInfo->dwFleeCount;
		UserScore.dwUserMedal=pUserInfo->dwUserMedal;
		UserScore.dwExperience=pUserInfo->dwExperience;
		UserScore.lLoveLiness=pUserInfo->lLoveLiness;

		//��������
		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->OnGFUserScore(pUserInfo->dwUserID,UserScore);
		return;
	}
}

void SHZCServerItem::OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserStatus& LastStatus)
{
	//��������
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagSHZ_UserInfo * pMeUserInfo=mMeUserItem->GetUserInfo();
	word wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	word wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	byte cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;



	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//�����뿪
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		SHZIClientUserItem * pITableUserItem=mTableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem)
		{
			mTableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,0);

		}
	}

	//���Ӽ���
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//����жϣ���������
		if(pUserInfo->dwUserID != pMeUserInfo->dwUserID && cbNowStatus == US_SIT && pMeUserInfo->wTableID == wNowTableID)
		{
			//��������
			ASSERT(SHZCParameterGlobal::shared()!=0);
			SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

			//������
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pIClientUserItem->GetUserCompanion()==CP_DETEST)
				{
					PACKET_AIDE_SIZE(64);
					packet.write2Byte(wNowTableID);
					packet.write2Byte(wNowChairID);
					packet.write4Byte(pMeUserInfo->dwUserID);
					packet.write4Byte(pUserInfo->dwUserID);
					SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_REPULSE_SIT_SHZ,packet.getBuffer(),packet.getPosition());

				}
			}
		}
		mTableViewFrame.SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}

	//����״̬
	if ((mTableViewFrame.GetChairCount() < MAX_CHAIR)&&(wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		mTableViewFrame.UpdateTableView(wNowTableID);
	}

	//�뿪֪ͨ
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��Ϸ֪ͨ
		if ((pIClientUserItem==mMeUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			if (SHZIClientKernel::get())
				SHZIClientKernel::get()->OnGFUserStatus(pUserInfo->dwUserID,UserStatus);
		}

		if (pIClientUserItem==mMeUserItem)
		{
			//���ñ���
			mReqTableID=INVALID_TABLE;
			mReqChairID=INVALID_CHAIR;

			if (SHZCServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
			{//����ģʽ��
				IntermitConnect(true);
				if (mIServerItemSink)
					mIServerItemSink->OnGFServerClose("");
			}
		}
		return;
	}

	//���봦��
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//�Լ��ж�
		if (mMeUserItem==pIClientUserItem)
		{
			//���ñ���
			mReqTableID=INVALID_TABLE;
			mReqChairID=INVALID_CHAIR;

			if (SHZIClientKernel::get()==0)
			{
				//��������
				if (!mIServerItemSink || !mIServerItemSink->CreateKernel())
				{
					OnGFGameClose(SHZ_GameExitCode_CreateFailed);
					return;
				}
			}
		}

		//��Ϸ֪ͨ
		if ((mMeUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			if (SHZIClientKernel::get())
				SHZIClientKernel::get()->OnGFUserEnter(pIClientUserItem);
		}

		return;
	}

	//״̬�ı�
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//��Ϸ֪ͨ
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;

		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->OnGFUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}
	
	return;
}

//�û�����
void SHZCServerItem::OnUserItemUpdate(SHZIClientUserItem * pIClientUserItem, const tagUserAttrib & UserAttrib)
{
	//��������
	word wMeTableID=mMeUserItem->GetTableID();
	word wUserTableID=pIClientUserItem->GetTableID();

	////�б����
	//m_UserListControl.UpdateDataItem(pIClientUserItem);

	////���Ҵ���
	//CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	//if (pDlgSearchUser!=0) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);
	
	////˽�Ĵ���
	//for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	//{
	//	CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
	//	if (pDlgWhisper->m_hWnd!=0) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//֪ͨ��Ϸ
	if ((wMeTableID!=INVALID_TABLE)&&(wMeTableID==wUserTableID))
	{
		//��������
		tagUserAttrib UserAttrib;
		UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

		//����֪ͨ

		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->OnGFUserAttrib(pIClientUserItem->GetUserID(),UserAttrib);
	}
	return;
}
