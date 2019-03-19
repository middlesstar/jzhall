#include "CClientKernel_SHZ.h"

//�û�����
void SHZCClientKernel::OnGFUserEnter(SHZIClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserEnter\n");

	//�û��ж�
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

//�û�����
void SHZCClientKernel::OnGFUserScore(dword dwUserID, const tagUserScore& UserScore)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserScore\n");

	//Ѱ���û�
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return;

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->UpdateUserItemScore(pIClientUserItem,&UserScore);
}

//�û�״̬
void SHZCClientKernel::OnGFUserStatus(dword dwUserID, const tagUserStatus& UserStatus)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserStatus\n");

	SHZIClientUserItem* pIClientUserItem = mUserManager->SearchUserByUserID(dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return;

	//״̬����
	byte cbUserStatus=UserStatus.cbUserStatus;

	//�뿪�ж�
	if ((cbUserStatus==US_NULL)||(cbUserStatus==US_FREE))
	{
		if (mMeUserItem==pIClientUserItem)
		{
			//���ñ���
			mAllowLookon=false;
			mMeUserItem=0;
			mGameStatus=GAME_STATUS_FREE;
			zeromemory(&mUserAttribute,sizeof(mUserAttribute));

			//������Ϸ
			if (mIClientKernelSink)
				mIClientKernelSink->ResetGameClient();

			//ɾ���û�
			mUserManager->ResetUserItem();
		}
		else
		{
			////ɾ��ʱ��
			//if (pIClientUserItem->GetUserStatus()!=US_LOOKON)
			//{
			//	word wChairID=pIClientUserItem->GetChairID();
			//	if (m_wClockChairID==wChairID) KillGameClock(m_wClockID);
			//}

			//ɾ���û�
			mUserManager->DeleteUserItem(pIClientUserItem);
		}

		return;
	}

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->UpdateUserItemStatus(pIClientUserItem, &UserStatus);
}

//�û�����
void SHZCClientKernel::OnGFUserAttrib(dword dwUserID, const tagUserAttrib& UserAttrib)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserAttrib\n");

	//Ѱ���û�
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return ;

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->UpdateUserItemAttrib(pIClientUserItem,&UserAttrib);
}

//�û��Զ���ͷ��
void SHZCClientKernel::OnGFUserCustomFace(dword dwUserID, dword dwCustomID, const tagCustomFaceInfo& CustomFaceInfo)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnGFUserCustomFace\n");

	//Ѱ���û�
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//�û��ж�
	ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return;

	ASSERT(mUserManager!=0);
	if (mUserManager)
		mUserManager->UpdateUserCustomFace(pIClientUserItem,dwCustomID,CustomFaceInfo);
}
