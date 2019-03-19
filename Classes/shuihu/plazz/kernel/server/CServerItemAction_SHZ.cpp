#include "CServerItem_SHZ.h"


//��������
bool SHZCServerItem::SearchGameTable(tagSearchTable_SHZ & SearchTable)
{
	//��������
	word wNullCount=0;
	word wChairUser=mTableViewFrame.GetChairCount();
	word wMeTableID=mMeUserItem->GetTableID();

	//������Ϸ
	if (SHZCServerRule::IsAllowDynamicJoin(mServerAttribute.dwServerRule)==true)
	{
		if(mTableViewFrame.GetChairCount() >= MAX_CHAIR)
		{
			SearchTable.wResultTableID=0;
			SearchTable.wResultChairID=MAX_CHAIR;
			return true;
		}
	}

	//��������
	for (word i=0;i<mTableViewFrame.GetTableCount();i++)
	{
		//��������
		SearchTable.wResultTableID=(SearchTable.wStartTableID+i)%mTableViewFrame.GetTableCount();

		//��������
		if (wMeTableID!=SearchTable.wResultTableID)
		{
			//��ȡ����
			SHZITableView * pITableView=mTableViewFrame.GetTableViewItem(SearchTable.wResultTableID);

			//��������
			if (pITableView->GetPlayFlag()==true) continue;
			if ((SearchTable.bFilterPass==true)&&(pITableView->GetLockerFlag()==true)) continue;

			//Ѱ�ҿ�λ��
			wNullCount=pITableView->GetNullChairCount(SearchTable.wResultChairID);

			//�ж���Ŀ
			if (wNullCount>0)
			{
				//Ч�����
				char strDescribe[256];
				if (EfficacyTableRule(SearchTable.wResultTableID,SearchTable.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}

				//��Ŀ�ж�
				if ((SearchTable.bOneNull==true)&&(wNullCount==1)) return true;
				if ((SearchTable.bTwoNull==true)&&(wNullCount==2)) return true;
				if ((SearchTable.bNotFull==true)&&(wNullCount<wChairUser)) return true;
				if ((SearchTable.bAllNull==true)&&(wNullCount==wChairUser)) return true;
			}
		}
	}

	//��������
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;

	return false;
}

//���ټ���
bool SHZCServerItem::PerformQuickSitDown()
{
	mIsQuickSitDown=true;
	return SendSocketData(MDM_GR_USER_SHZ, SUB_GR_USER_CHAIR_REQ_SHZ);
} 



//ִ���Թ�
bool SHZCServerItem::PerformLookonAction(word wTableID, word wChairID)
{
	PLAZZ_PRINTF("CServerItem::PerformLookonAction...\n");

	if (!IsService())
		return false;

	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (mServiceStatus != SHZ_ServiceStatus_ServiceIng) return false;
	if ((mReqTableID==wTableID)&&(mReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (mMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"), DLG_MB_OK);
		return false;
	}

	//Ȩ���ж�
	if (SHZCUserRight::CanLookon(mUserAttribute.dwUserRight)==false)
	{
		//��ʾ��Ϣ
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("��Ǹ������ʱû���Թ���Ϸ��Ȩ�ޣ�"), DLG_MB_OK);
		return false;
	}

	//�������
	if ((mReqTableID!=INVALID_TABLE)&&(mReqChairID!=INVALID_CHAIR)&&(mReqChairID!=MAX_CHAIR))
	{
		SHZIClientUserItem * pIClientUserItem=mTableViewFrame.GetClientUserItem(mReqTableID,mReqChairID);
		if (pIClientUserItem==mMeUserItem) mTableViewFrame.SetClientUserItem(mReqTableID,mReqChairID,0);
	}

	//���ñ���
	mReqTableID=wTableID;
	mReqChairID=wChairID;
	mFindTableID=INVALID_TABLE;

	//���ý���
	mTableViewFrame.VisibleTable(wTableID);

	PLAZZ_PRINTF("CServerItem::PerformLookonAction send\n");
	//��������
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//ִ������
bool SHZCServerItem::PerformStandUpAction(word wTableID, word wChairID)
{
	PLAZZ_PRINTF("CServerItem::PerformStandUpAction...\n");

	if (!IsService())
		return false;

	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//״̬����
	if (mServiceStatus != SHZ_ServiceStatus_ServiceIng) return false;
	if ((mReqTableID==wTableID)&&(mReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (mMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"), DLG_MB_OK);

		return false;
	}

	//���ñ���
	mReqTableID=INVALID_TABLE;
	mReqChairID=INVALID_CHAIR;
	mFindTableID=INVALID_TABLE;

	//���ý���
	mTableViewFrame.VisibleTable(wTableID);

	PLAZZ_PRINTF("CServerItem::PerformStandUpAction send\n");
	//��������
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//ִ������
bool SHZCServerItem::PerformSitDownAction(word wTableID, word wChairID, bool bEfficacyPass)
{
	PLAZZ_PRINTF("CServerItem::PerformSitDownAction...\n");

	if (!IsService())
		return false;

	//״̬����
	if (mServiceStatus != SHZ_ServiceStatus_ServiceIng) return false;
	if ((mReqTableID!=INVALID_TABLE)&&(mReqTableID==wTableID)) return false;
	if ((mReqChairID!=INVALID_CHAIR)&&(mReqChairID==wChairID)) return false;

	//�Լ�״̬
	if (mMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		//��ʾ��Ϣ
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("��������Ϸ�У���ʱ�����뿪�����Ƚ�����ǰ��Ϸ��"), DLG_MB_OK);
		return false;
	}

	//Ȩ���ж�
	if (SHZCUserRight::CanPlay(mUserAttribute.dwUserRight)==false)
	{
		//��ʾ��Ϣ
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("��Ǹ������ʱû�м�����Ϸ��Ȩ�ޣ�"), DLG_MB_OK);

		return false;
	}
	
	

	//����Ч��
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		char strDescribe[256]={0};
		if ((wChairID!=MAX_CHAIR)&&(EfficacyTableRule(wTableID,wChairID,false,strDescribe)==false))
		{
			//��ʾ��Ϣ
			if (mIStringMessageSink)
				mIStringMessageSink->InsertPromptString(strDescribe, DLG_MB_OK);
			return false;
		}
	}

	//�����ж�
	char szPassword[LEN_PASSWORD] = {0};
	if ((mMeUserItem->GetMasterOrder()==0)&&(bEfficacyPass==true)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=MAX_CHAIR))
	{
		//��������
		bool bLocker=mTableViewFrame.GetLockerFlag(wTableID);

		//�����ж�
		if(bLocker)
		{
			PLAZZ_PRINTF("%s",a_u8("//TODO:���������ý������룬���������롣 δʵ��!!!!\n"));
			return false;
		}
	}

	//�������
	if ((mReqTableID!=INVALID_TABLE)&&(mReqChairID!=INVALID_CHAIR)&&(mReqChairID!=MAX_CHAIR))
	{
		SHZIClientUserItem * pIClientUserItem=mTableViewFrame.GetClientUserItem(mReqTableID,mReqChairID);
		if (pIClientUserItem==mMeUserItem) mTableViewFrame.SetClientUserItem(mReqTableID,mReqChairID,0);
	}

	//���ý���
	if ((wChairID!=MAX_CHAIR)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		mTableViewFrame.VisibleTable(wTableID);
		mTableViewFrame.SetClientUserItem(wTableID,wChairID,mMeUserItem);
	}

	//���ñ���
	mReqTableID=wTableID;
	mReqChairID=wChairID;
	mFindTableID=INVALID_TABLE;

	PLAZZ_PRINTF("CServerItem::PerformSitDownAction send...\n");
	//��������
	SendSitDownPacket(wTableID,wChairID,szPassword);

	return true;
}

//ִ�й���
bool SHZCServerItem::PerformBuyProperty(byte cbRequestArea,const char* pszNickName, word wItemCount, word wPropertyIndex)
{
	PLAZZ_PRINTF("CServerItem::PerformBuyProperty...\n");

	if (!IsService())
		return false;

	PLAZZ_PRINTF("%s", a_u8("//TODO:ִ�й��� δʵ��!!!!\n"));
	return true;


}
