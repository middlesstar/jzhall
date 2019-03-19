#include "CServerItem_SHZ.h"

SHZIRoomFrameSink* SHZIRoomFrameSink::getInterface()
{
	return (SHZIRoomFrameSink*)SHZIServerItem::get();
}

//////////////////////////////////////////////////////////////////////////
// �����Ϣ

//��Ϸ��׼����
void SHZCServerItem::OnGFGameReady()
{
	PLAZZ_PRINTF("CServerItem::OnGFGameReady\n");
	//��������
	word wTableID=mMeUserItem->GetTableID();
	word wChairID=mMeUserItem->GetChairID();

	////��������
	//if (bAlreadyExist==false)
	//{
	//	//��ȡ�ȼ�
	//	tagLevelItem LevelItem[64];
	//	byte cbLevelCount=(byte)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

	//	//��������
	//	m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
	//	m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
	//	m_ProcessManager.SendPropertyInfo(m_PropertyInfo,m_cbPropertyCount);		
	//}

	mUserAttribute.wChairID = wChairID;
	mUserAttribute.wTableID = wTableID;

	//������Ϣ
	SHZIClientKernel::get()->OnGFConfigServer(mUserAttribute, mServerAttribute);

	//�����û�
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//��������
		word wEnumIndex=0;
		
		//�������
		while (true)
		{
			//��ȡ�û�
			SHZIClientUserItem * pIClientUserItem=mUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==0) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//�����û�
			SHZIClientKernel::get()->OnGFUserEnter(pIClientUserItem);
		};

		//�Թ��û�
		wEnumIndex=0;
		while (true)
		{
			//��ȡ�û�
			SHZIClientUserItem * pIClientUserItem=mUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==0) break;

			//�����ж�
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//�����û�
			SHZIClientKernel::get()->OnGFUserEnter(pIClientUserItem);
		};
	}

	//�������
	SHZIClientKernel::get()->OnGFConfigFinish();
	
	mIsGameReady = true;
}

//��Ϸ�ر�
void SHZCServerItem::OnGFGameClose(int iExitCode)
{
	PLAZZ_PRINTF("flow->OnGFGameClose %d\n", iExitCode);
	//Ч��״̬

	ASSERT(mMeUserItem!=0);
	if (mMeUserItem==0) return ;

	mIsGameReady = false;

	//��������
	word wTableID=mMeUserItem->GetTableID();
	word wChairID=mMeUserItem->GetChairID();
	byte cbUserStatus=mMeUserItem->GetUserStatus();
	mUserAttribute.wChairID = INVALID_CHAIR;
	mUserAttribute.wTableID = INVALID_TABLE;

	//�˳���Ϸ
	if ((wTableID != INVALID_TABLE) && (mServiceStatus == SHZ_ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
	}

	mTableViewFrame.SetTableStatus(false);
}

//////////////////////////////////////////////////////////////////////////
// Socket��Ϣ

// ��������
bool SHZCServerItem::GFSendData(int main, int sub, void* data, int size)
{
	return SendSocketData(main, sub, data, size);
}