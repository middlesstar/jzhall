#include "CServerItem_SHZ.h"

SHZIRoomFrameSink* SHZIRoomFrameSink::getInterface()
{
	return (SHZIRoomFrameSink*)SHZIServerItem::get();
}

//////////////////////////////////////////////////////////////////////////
// 框架消息

//游戏已准备好
void SHZCServerItem::OnGFGameReady()
{
	PLAZZ_PRINTF("CServerItem::OnGFGameReady\n");
	//变量定义
	word wTableID=mMeUserItem->GetTableID();
	word wChairID=mMeUserItem->GetChairID();

	////房间配置
	//if (bAlreadyExist==false)
	//{
	//	//获取等级
	//	tagLevelItem LevelItem[64];
	//	byte cbLevelCount=(byte)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

	//	//发送配置
	//	m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
	//	m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
	//	m_ProcessManager.SendPropertyInfo(m_PropertyInfo,m_cbPropertyCount);		
	//}

	mUserAttribute.wChairID = wChairID;
	mUserAttribute.wTableID = wTableID;

	//房间信息
	SHZIClientKernel::get()->OnGFConfigServer(mUserAttribute, mServerAttribute);

	//发送用户
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//变量定义
		word wEnumIndex=0;
		
		//发送玩家
		while (true)
		{
			//获取用户
			SHZIClientUserItem * pIClientUserItem=mUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==0) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//发送用户
			SHZIClientKernel::get()->OnGFUserEnter(pIClientUserItem);
		};

		//旁观用户
		wEnumIndex=0;
		while (true)
		{
			//获取用户
			SHZIClientUserItem * pIClientUserItem=mUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==0) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//发送用户
			SHZIClientKernel::get()->OnGFUserEnter(pIClientUserItem);
		};
	}

	//配置完成
	SHZIClientKernel::get()->OnGFConfigFinish();
	
	mIsGameReady = true;
}

//游戏关闭
void SHZCServerItem::OnGFGameClose(int iExitCode)
{
	PLAZZ_PRINTF("flow->OnGFGameClose %d\n", iExitCode);
	//效验状态

	ASSERT(mMeUserItem!=0);
	if (mMeUserItem==0) return ;

	mIsGameReady = false;

	//变量定义
	word wTableID=mMeUserItem->GetTableID();
	word wChairID=mMeUserItem->GetChairID();
	byte cbUserStatus=mMeUserItem->GetUserStatus();
	mUserAttribute.wChairID = INVALID_CHAIR;
	mUserAttribute.wTableID = INVALID_TABLE;

	//退出游戏
	if ((wTableID != INVALID_TABLE) && (mServiceStatus == SHZ_ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
	}

	mTableViewFrame.SetTableStatus(false);
}

//////////////////////////////////////////////////////////////////////////
// Socket消息

// 发送数据
bool SHZCServerItem::GFSendData(int main, int sub, void* data, int size)
{
	return SendSocketData(main, sub, data, size);
}