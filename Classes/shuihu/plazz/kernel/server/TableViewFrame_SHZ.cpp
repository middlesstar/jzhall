#include "TableViewFrame_SHZ.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
SHZCTableView::SHZCTableView()
{
	//����ӿ�
	mITableViewFrame=0;

	//���ӱ�־
	mIsLocker		= false;
	mIsPlaying		= false;
	mIsFocusFrame	= false;

	//����״̬
	mWatchCount		= 0;
	mTableOwnerID	= 0;

	//���Ա���
	mTableID		= 0;
	mChairCount		= 0;
	
	zeromemory(mIClientUserItem, sizeof(mIClientUserItem));
	return;
}

//��������
SHZCTableView::~SHZCTableView()
{
}

//��������
word SHZCTableView::GetNullChairCount(word & wNullChairID)
{
	//���ñ���
	wNullChairID=INVALID_CHAIR;

	//Ѱ��λ��
	word wNullCount=0;
	for (word i=0;i<mChairCount;i++)
	{
		if (mIClientUserItem[i]==0)
		{
			//������Ŀ
			wNullCount++;

			//���ý��
			if (wNullChairID==INVALID_CHAIR) wNullChairID=i;
		}
	}

	return wNullCount;
}

//���ú���
void SHZCTableView::InitTableView(word wTableID, word wChairCount, SHZITableViewFrame * pITableViewFrame)
{
	//��������
	mTableID=wTableID;
	mChairCount=wChairCount;

	//���ýӿ�
	mITableViewFrame=pITableViewFrame;
	return;
}

//��ȡ�û�
SHZIClientUserItem * SHZCTableView::GetClientUserItem(word wChairID)
{
	//Ч�����
	ASSERT(wChairID<mChairCount);
	if (wChairID>=mChairCount) return 0;

	//��ȡ�û�
	return mIClientUserItem[wChairID];
}

//������Ϣ
bool SHZCTableView::SetClientUserItem(word wChairID, SHZIClientUserItem * pIClientUserItem)
{
	//Ч�����
	ASSERT(wChairID<mChairCount);
	if (wChairID>=mChairCount) return false;

	//�����û�
	mIClientUserItem[wChairID]=pIClientUserItem;

	//���½���
	mITableViewFrame->UpdateTableView(mTableID);

	return true;
}

//����״̬ 
void SHZCTableView::SetTableStatus(bool bPlaying, bool bLocker)
{
	//���ñ�־
	if ((mIsLocker!=bLocker)||(mIsPlaying!=bPlaying))
	{
		//���ñ���
		mIsLocker=bLocker; 
		mIsPlaying=bPlaying;

		//���½���
		mITableViewFrame->UpdateTableView(mTableID);
	}

	return;
}

//������
void SHZCTableView::SetFocusFrame(bool bFocusFrame)
{
	//���ñ�־
	if (mIsFocusFrame!=bFocusFrame)
	{
		//���ñ���
		mIsFocusFrame=bFocusFrame;

		//���½���
		mITableViewFrame->UpdateTableView(mTableID);
	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////

//���캯��
SHZCTableViewFrame::SHZCTableViewFrame()
{
	//���Ա���
	mTableCount=0;
	mChairCount=0;
	mServerRule=0;
	mServerType=0;

	//��������
	mMatchTotalUser=0;
	mMatchWaittingCount=0;
	mMatchStatus=MS_NULL;
	return;
}

//��������
SHZCTableViewFrame::~SHZCTableViewFrame()
{
	//ɾ������
	for (int i=0, l = (int)mTableViewArray.size(); i<l ;i++)
	{
		SHZCTableView * pTableView=mTableViewArray[i];
		if (pTableView!=0) 
			SafeDelete(pTableView);
	}

	//ɾ������
	mTableViewArray.clear();

	return;
}


//���ú���
bool SHZCTableViewFrame::ConfigTableFrame(word wTableCount, word wChairCount, dword dwServerRule, word wServerType, word wServerID)
{
	//Ч�����
	ASSERT(wChairCount<=MAX_CHAIR);
	
	//���ñ���
	mTableCount=wTableCount;
	mChairCount=wChairCount;
	mServerRule=dwServerRule;
	mServerType=wServerType;
	mTableViewArray.resize(mTableCount);
	zeromemory(&mTableViewArray[0],mTableCount*sizeof(SHZCTableView *));

	//��������
	for (word i=0;i<mTableCount;i++)
	{
		mTableViewArray[i]=new SHZCTableView;
		mTableViewArray[i]->InitTableView(i,wChairCount,this);
	}


	return true;
}

//��ȡ�û�
SHZIClientUserItem * SHZCTableViewFrame::GetClientUserItem(word wTableID, word wChairID)
{
	//��ȡ����
	ASSERT(GetTableViewItem(wTableID)!=0);
	SHZITableView * pITableView=GetTableViewItem(wTableID);

	//��ȡ�û�
	if (pITableView!=0)
	{
		return pITableView->GetClientUserItem(wChairID);
	}

	return 0;
}

//������Ϣ
bool SHZCTableViewFrame::SetClientUserItem(word wTableID, word wChairID, SHZIClientUserItem * pIClientUserItem)
{
	SHZITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView!=0) pITableView->SetClientUserItem(wChairID,pIClientUserItem);
	return true;
}

//��Ϸ��־
bool SHZCTableViewFrame::GetPlayFlag(word wTableID)
{
	//��ȡ����
	ASSERT(GetTableViewItem(wTableID)!=0);
	SHZITableView * pITableView=GetTableViewItem(wTableID);

	//��ȡ��־
	if (pITableView!=0)
	{
		return pITableView->GetPlayFlag();
	}

	return false;
}

//�����־
bool SHZCTableViewFrame::GetLockerFlag(word wTableID)
{
	//��ȡ����
	ASSERT(GetTableViewItem(wTableID)!=0);
	SHZITableView * pITableView=GetTableViewItem(wTableID);

	//��ȡ��־
	if (pITableView!=0)
	{
		return pITableView->GetLockerFlag();
	}

	return false;
}

//������
void SHZCTableViewFrame::SetFocusFrame(word wTableID, bool bFocusFrame)
{
	//��ȡ����
	ASSERT(GetTableViewItem(wTableID)!=0);
	SHZITableView * pITableView=GetTableViewItem(wTableID);

	//���ñ�־
	if (pITableView!=0) pITableView->SetFocusFrame(bFocusFrame);

	return;
}


//����״̬ 
void SHZCTableViewFrame::SetTableStatus(word wTableID, bool bPlaying, bool bLocker)
{
	//��ȡ����
	ASSERT(GetTableViewItem(wTableID)!=0);
	SHZITableView * pITableView=GetTableViewItem(wTableID);

	//���ñ�־
	if (pITableView!=0) pITableView->SetTableStatus(bPlaying,bLocker);

	return;
}

//����״̬ 
void SHZCTableViewFrame::SetTableStatus(bool bWaitDistribute)
{
	//m_bWaitDistribute=bWaitDistribute;
	return;
}

//���ӿ���
bool SHZCTableViewFrame::VisibleTable(word wTableID)
{
	//Ч�����
	ASSERT(wTableID<mTableCount);
	if (wTableID>=mTableCount) return false;

	return true;
}

//��������
bool SHZCTableViewFrame::FlashGameTable(word wTableID)
{
	//��ȡ����
	SHZITableView * pITableView=GetTableViewItem(wTableID);

	//�����ж�
	if (pITableView==0)
	{
		ASSERT(FALSE);
		return false;
	}


	return true;
}

//��������
bool SHZCTableViewFrame::FlashGameChair(word wTableID, word wChairID)
{
	//��ȡ����
	SHZITableView * pITableView=GetTableViewItem(wTableID);

	//�����ж�
	if (pITableView==0)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//��������
bool SHZCTableViewFrame::UpdateTableView(word wTableID)
{
	//��ȡ����
	SHZITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView==0) return false;

	
	return true;
}

//��ȡ����
SHZITableView * SHZCTableViewFrame::GetTableViewItem(word wTableID)
{
	//��ȡ����
	if (wTableID!=INVALID_TABLE)
	{
		//Ч�����
		ASSERT(wTableID<(int)mTableViewArray.size());
		if (wTableID>=(int)mTableViewArray.size()) return 0;

		//��ȡ����
		SHZITableView * pITableView=mTableViewArray[wTableID];

		return pITableView;
	}

	return 0;
}

//��������
word SHZCTableViewFrame::GetNullChairCount(word wTableID, word & wNullChairID)
{
	//��ȡ����
	ASSERT(GetTableViewItem(wTableID)!=0);
	SHZITableView * pITableView=GetTableViewItem(wTableID);

	//��ȡ״̬
	if (pITableView!=0)
	{
		return pITableView->GetNullChairCount(wNullChairID);
	}

	return 0;
}

//����״̬
void SHZCTableViewFrame::SetMatchStatus(byte cbMatchStatus)
{
	mMatchStatus=cbMatchStatus;
	//if(cbMatchStatus==MS_MATCHING)
	//{
	//	m_tStartTime=CTime::GetCurrentTime();
	//	SetTimer(IDI_TICK_TIMER,1000, 0);
	//	InvalidateRect(0);
	//}
	//else
	//{
	//	KillTimer(IDI_TICK_TIMER);
	//}
}

//////////////////////////////////////////////////////////////////////////////////
