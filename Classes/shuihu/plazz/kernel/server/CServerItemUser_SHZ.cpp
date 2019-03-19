#include "CServerItem_SHZ.h"

//////////////////////////////////////////////////////////////////////////
//用户接口

//自己位置
word SHZCServerItem::GetMeChairID()
{
	if (mMeUserItem == 0) return INVALID_CHAIR;
	return mMeUserItem->GetChairID();
}

//自己位置
SHZIClientUserItem * SHZCServerItem::GetMeUserItem()
{
	return mMeUserItem;
}

//游戏用户
SHZIClientUserItem * SHZCServerItem::GetTableUserItem(word wChairID)
{
	return mUserManager->EnumUserItem(wChairID);
}

//查找用户
SHZIClientUserItem * SHZCServerItem::SearchUserByUserID(dword dwUserID)
{
	return mUserManager->SearchUserByUserID(dwUserID);
}

//查找用户
SHZIClientUserItem * SHZCServerItem::SearchUserByGameID(dword dwGameID)
{
	return mUserManager->SearchUserByGameID(dwGameID);
}

//查找用户
SHZIClientUserItem * SHZCServerItem::SearchUserByNickName(const char* szNickName)
{
	return mUserManager->SearchUserByNickName(szNickName);
}

//////////////////////////////////////////////////////////////////////////
// IUserManagerSink
//////////////////////////////////////////////////////////////////////////
void SHZCServerItem::OnUserItemAcitve(SHZIClientUserItem* pIClientUserItem)
{
	//判断自己
	if (mMeUserItem==0)
	{
		mMeUserItem=pIClientUserItem;
	}

	////插入列表
	//m_UserListControl.InsertDataItem(pIClientUserItem);

	//设置桌子
	byte cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		word wTableID=pIClientUserItem->GetTableID();
		word wChairID=pIClientUserItem->GetChairID();
		mTableViewFrame.SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	//提示信息
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
	//变量定义
	word wLastTableID=pIClientUserItem->GetTableID();
	word wLastChairID=pIClientUserItem->GetChairID();
	dword dwLeaveUserID=pIClientUserItem->GetUserID();
	tagSHZ_UserInfo * pMeUserInfo=mMeUserItem->GetUserInfo();

	//变量定义
	ASSERT(SHZCParameterGlobal::shared());
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//离开处理
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//获取用户
		SHZIClientUserItem * pITableUserItem=mTableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) mTableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,0);

		//离开通知
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

	////聊天对象
	//m_ChatControl.DeleteUserItem(pIClientUserItem);

	////删除列表
	//m_UserListControl.DeleteDataItem(pIClientUserItem);

	////查找窗口
	//CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	//if (pDlgSearchUser!=0) pDlgSearchUser->OnUserItemDelete(pIClientUserItem,this);

	////私聊窗口
	//for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	//{
	//	CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
	//	if (pDlgWhisper->m_hWnd!=0) pDlgWhisper->OnEventUserLeave(pIClientUserItem);
	//}

	////菜单对象
	//for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
	//{
	//	//获取用户
	//	IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
	//	if (pIChatUserItem->GetUserID()==dwLeaveUserID) m_MenuUserItemArray.RemoveAt(i);
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserDelete(pIClientUserItem);

	//提示信息
	if ((pParameterGlobal->m_bNotifyUserInOut == true) && (mServiceStatus == SHZ_ServiceStatus_ServiceIng))
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertUserLeave(pIClientUserItem->GetNickName());
	}

}

void SHZCServerItem::OnUserFaceUpdate(SHZIClientUserItem * pIClientUserItem)
{
	//变量定义
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	////列表更新
	//m_UserListControl.UpdateDataItem(pIClientUserItem);

	////查找窗口
	//CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	//if (pDlgSearchUser!=0) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	////私聊窗口
	//for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	//{
	//	CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
	//	if (pDlgWhisper->m_hWnd!=0) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//更新桌子
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->cbUserStatus!=US_LOOKON))
	{
		mTableViewFrame.UpdateTableView(pUserInfo->wTableID);
	}

	//更新游戏
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(mMeUserItem->GetTableID()==pUserInfo->wTableID))
	{
		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->OnGFUserCustomFace(pUserInfo->dwUserID,pUserInfo->dwCustomID,*pCustomFaceInfo);
	}
}

void SHZCServerItem::OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserScore& LastScore)
{
	//变量定义
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagSHZ_UserInfo * pMeUserInfo=mMeUserItem->GetUserInfo();

	////列表更新
	//m_UserListControl.UpdateDataItem(pIClientUserItem);

	////查找窗口
	//CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	//if (pDlgSearchUser!=0) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	////私聊窗口
	//for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	//{
	//	CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
	//	if (pDlgWhisper->m_hWnd!=0) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	//}
		
	//房间界面通知
	if (pIClientUserItem==mMeUserItem)
	{
		//变量定义
		SHZCGlobalUserInfo * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
		tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		tagSHZUserInsureInfo * pUserInsureData=pGlobalUserInfo->GetUserInsureInfo();

		//设置变量
		pUserInsureData->lUserScore+=pIClientUserItem->GetUserScore()-LastScore.lScore;
		pUserInsureData->lUserInsure+=pIClientUserItem->GetUserInsure()-LastScore.lInsure;
		pGlobalUserData->dwUserMedal+=pIClientUserItem->GetUserMedal()-LastScore.dwUserMedal;
	}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//游戏通知
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//变量定义
		tagUserScore UserScore;
		zeromemory(&UserScore,sizeof(UserScore));

		//设置变量
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

		//发送数据
		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->OnGFUserScore(pUserInfo->dwUserID,UserScore);
		return;
	}
}

void SHZCServerItem::OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserStatus& LastStatus)
{
	//变量定义
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagSHZ_UserInfo * pMeUserInfo=mMeUserItem->GetUserInfo();
	word wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	word wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	byte cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;



	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//桌子离开
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		SHZIClientUserItem * pITableUserItem=mTableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem)
		{
			mTableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,0);

		}
	}

	//桌子加入
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//厌恶判断（黑名单）
		if(pUserInfo->dwUserID != pMeUserInfo->dwUserID && cbNowStatus == US_SIT && pMeUserInfo->wTableID == wNowTableID)
		{
			//变量定义
			ASSERT(SHZCParameterGlobal::shared()!=0);
			SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

			//厌恶玩家
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

	//桌子状态
	if ((mTableViewFrame.GetChairCount() < MAX_CHAIR)&&(wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		mTableViewFrame.UpdateTableView(wNowTableID);
	}

	//离开通知
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
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
			//设置变量
			mReqTableID=INVALID_TABLE;
			mReqChairID=INVALID_CHAIR;

			if (SHZCServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
			{//快速模式的
				IntermitConnect(true);
				if (mIServerItemSink)
					mIServerItemSink->OnGFServerClose("");
			}
		}
		return;
	}

	//加入处理
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//自己判断
		if (mMeUserItem==pIClientUserItem)
		{
			//设置变量
			mReqTableID=INVALID_TABLE;
			mReqChairID=INVALID_CHAIR;

			if (SHZIClientKernel::get()==0)
			{
				//启动进程
				if (!mIServerItemSink || !mIServerItemSink->CreateKernel())
				{
					OnGFGameClose(SHZ_GameExitCode_CreateFailed);
					return;
				}
			}
		}

		//游戏通知
		if ((mMeUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			if (SHZIClientKernel::get())
				SHZIClientKernel::get()->OnGFUserEnter(pIClientUserItem);
		}

		return;
	}

	//状态改变
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//游戏通知
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

//用户更新
void SHZCServerItem::OnUserItemUpdate(SHZIClientUserItem * pIClientUserItem, const tagUserAttrib & UserAttrib)
{
	//变量定义
	word wMeTableID=mMeUserItem->GetTableID();
	word wUserTableID=pIClientUserItem->GetTableID();

	////列表更新
	//m_UserListControl.UpdateDataItem(pIClientUserItem);

	////查找窗口
	//CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	//if (pDlgSearchUser!=0) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);
	
	////私聊窗口
	//for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	//{
	//	CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
	//	if (pDlgWhisper->m_hWnd!=0) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//通知游戏
	if ((wMeTableID!=INVALID_TABLE)&&(wMeTableID==wUserTableID))
	{
		//变量定义
		tagUserAttrib UserAttrib;
		UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

		//发送通知

		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->OnGFUserAttrib(pIClientUserItem->GetUserID(),UserAttrib);
	}
	return;
}
