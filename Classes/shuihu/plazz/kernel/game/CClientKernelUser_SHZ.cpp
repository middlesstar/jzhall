#include "CClientKernel_SHZ.h"

//用户进入
void SHZCClientKernel::OnGFUserEnter(SHZIClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserEnter\n");

	//用户判断
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return;

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->ActiveUserItem(
		*pIClientUserItem->GetUserInfo(),
		*pIClientUserItem->GetCustomFaceInfo(),
		pIClientUserItem->GetUserCompanion(),
		pIClientUserItem->GetUserNoteInfo());
}

//用户积分
void SHZCClientKernel::OnGFUserScore(dword dwUserID, const tagUserScore& UserScore)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserScore\n");

	//寻找用户
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return;

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->UpdateUserItemScore(pIClientUserItem,&UserScore);
}

//用户状态
void SHZCClientKernel::OnGFUserStatus(dword dwUserID, const tagUserStatus& UserStatus)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserStatus\n");

	SHZIClientUserItem* pIClientUserItem = mUserManager->SearchUserByUserID(dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return;

	//状态定义
	byte cbUserStatus=UserStatus.cbUserStatus;

	//离开判断
	if ((cbUserStatus==US_NULL)||(cbUserStatus==US_FREE))
	{
		if (mMeUserItem==pIClientUserItem)
		{
			//设置变量
			mAllowLookon=false;
			mMeUserItem=0;
			mGameStatus=GAME_STATUS_FREE;
			zeromemory(&mUserAttribute,sizeof(mUserAttribute));

			//重置游戏
			if (mIClientKernelSink)
				mIClientKernelSink->ResetGameClient();

			//删除用户
			mUserManager->ResetUserItem();
		}
		else
		{
			////删除时间
			//if (pIClientUserItem->GetUserStatus()!=US_LOOKON)
			//{
			//	word wChairID=pIClientUserItem->GetChairID();
			//	if (m_wClockChairID==wChairID) KillGameClock(m_wClockID);
			//}

			//删除用户
			mUserManager->DeleteUserItem(pIClientUserItem);
		}

		return;
	}

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->UpdateUserItemStatus(pIClientUserItem, &UserStatus);
}

//用户属性
void SHZCClientKernel::OnGFUserAttrib(dword dwUserID, const tagUserAttrib& UserAttrib)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserAttrib\n");

	//寻找用户
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return ;

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);
}

//用户自定义头像
void SHZCClientKernel::OnGFUserCustomFace(dword dwUserID, dword dwCustomID, const tagCustomFaceInfo& CustomFaceInfo)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserCustomFace\n");

	//寻找用户
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return;

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->UpdateUserCustomFace(pIClientUserItem,dwCustomID,CustomFaceInfo);
}
