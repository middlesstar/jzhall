#include "CServerItem_SHZ.h"


//查找桌子
bool SHZCServerItem::SearchGameTable(tagSearchTable_SHZ & SearchTable)
{
	//变量定义
	word wNullCount=0;
	word wChairUser=mTableViewFrame.GetChairCount();
	word wMeTableID=mMeUserItem->GetTableID();

	//百人游戏
	if (SHZCServerRule::IsAllowDynamicJoin(mServerAttribute.dwServerRule)==true)
	{
		if(mTableViewFrame.GetChairCount() >= MAX_CHAIR)
		{
			SearchTable.wResultTableID=0;
			SearchTable.wResultChairID=MAX_CHAIR;
			return true;
		}
	}

	//搜索桌子
	for (word i=0;i<mTableViewFrame.GetTableCount();i++)
	{
		//设置索引
		SearchTable.wResultTableID=(SearchTable.wStartTableID+i)%mTableViewFrame.GetTableCount();

		//搜索处理
		if (wMeTableID!=SearchTable.wResultTableID)
		{
			//获取桌子
			SHZITableView * pITableView=mTableViewFrame.GetTableViewItem(SearchTable.wResultTableID);

			//搜索过虑
			if (pITableView->GetPlayFlag()==true) continue;
			if ((SearchTable.bFilterPass==true)&&(pITableView->GetLockerFlag()==true)) continue;

			//寻找空位置
			wNullCount=pITableView->GetNullChairCount(SearchTable.wResultChairID);

			//判断数目
			if (wNullCount>0)
			{
				//效验规则
				char strDescribe[256];
				if (EfficacyTableRule(SearchTable.wResultTableID,SearchTable.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}

				//数目判断
				if ((SearchTable.bOneNull==true)&&(wNullCount==1)) return true;
				if ((SearchTable.bTwoNull==true)&&(wNullCount==2)) return true;
				if ((SearchTable.bNotFull==true)&&(wNullCount<wChairUser)) return true;
				if ((SearchTable.bAllNull==true)&&(wNullCount==wChairUser)) return true;
			}
		}
	}

	//设置数据
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;

	return false;
}

//快速加入
bool SHZCServerItem::PerformQuickSitDown()
{
	mIsQuickSitDown=true;
	return SendSocketData(MDM_GR_USER_SHZ, SUB_GR_USER_CHAIR_REQ_SHZ);
} 



//执行旁观
bool SHZCServerItem::PerformLookonAction(word wTableID, word wChairID)
{
	PLAZZ_PRINTF("CServerItem::PerformLookonAction...\n");

	if (!IsService())
		return false;

	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (mServiceStatus != SHZ_ServiceStatus_ServiceIng) return false;
	if ((mReqTableID==wTableID)&&(mReqChairID==wChairID)) return false;

	//自己状态
	if (mMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("您正在游戏中，暂时不能离开，请先结束当前游戏！"), DLG_MB_OK);
		return false;
	}

	//权限判断
	if (SHZCUserRight::CanLookon(mUserAttribute.dwUserRight)==false)
	{
		//提示消息
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("抱歉，您暂时没有旁观游戏的权限！"), DLG_MB_OK);
		return false;
	}

	//清理界面
	if ((mReqTableID!=INVALID_TABLE)&&(mReqChairID!=INVALID_CHAIR)&&(mReqChairID!=MAX_CHAIR))
	{
		SHZIClientUserItem * pIClientUserItem=mTableViewFrame.GetClientUserItem(mReqTableID,mReqChairID);
		if (pIClientUserItem==mMeUserItem) mTableViewFrame.SetClientUserItem(mReqTableID,mReqChairID,0);
	}

	//设置变量
	mReqTableID=wTableID;
	mReqChairID=wChairID;
	mFindTableID=INVALID_TABLE;

	//设置界面
	mTableViewFrame.VisibleTable(wTableID);

	PLAZZ_PRINTF("CServerItem::PerformLookonAction send\n");
	//发送命令
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//执行起立
bool SHZCServerItem::PerformStandUpAction(word wTableID, word wChairID)
{
	PLAZZ_PRINTF("CServerItem::PerformStandUpAction...\n");

	if (!IsService())
		return false;

	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (mServiceStatus != SHZ_ServiceStatus_ServiceIng) return false;
	if ((mReqTableID==wTableID)&&(mReqChairID==wChairID)) return false;

	//自己状态
	if (mMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("您正在游戏中，暂时不能离开，请先结束当前游戏！"), DLG_MB_OK);

		return false;
	}

	//设置变量
	mReqTableID=INVALID_TABLE;
	mReqChairID=INVALID_CHAIR;
	mFindTableID=INVALID_TABLE;

	//设置界面
	mTableViewFrame.VisibleTable(wTableID);

	PLAZZ_PRINTF("CServerItem::PerformStandUpAction send\n");
	//发送命令
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//执行坐下
bool SHZCServerItem::PerformSitDownAction(word wTableID, word wChairID, bool bEfficacyPass)
{
	PLAZZ_PRINTF("CServerItem::PerformSitDownAction...\n");

	if (!IsService())
		return false;

	//状态过滤
	if (mServiceStatus != SHZ_ServiceStatus_ServiceIng) return false;
	if ((mReqTableID!=INVALID_TABLE)&&(mReqTableID==wTableID)) return false;
	if ((mReqChairID!=INVALID_CHAIR)&&(mReqChairID==wChairID)) return false;

	//自己状态
	if (mMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("您正在游戏中，暂时不能离开，请先结束当前游戏！"), DLG_MB_OK);
		return false;
	}

	//权限判断
	if (SHZCUserRight::CanPlay(mUserAttribute.dwUserRight)==false)
	{
		//提示消息
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("抱歉，您暂时没有加入游戏的权限！"), DLG_MB_OK);

		return false;
	}
	
	

	//桌子效验
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		char strDescribe[256]={0};
		if ((wChairID!=MAX_CHAIR)&&(EfficacyTableRule(wTableID,wChairID,false,strDescribe)==false))
		{
			//提示消息
			if (mIStringMessageSink)
				mIStringMessageSink->InsertPromptString(strDescribe, DLG_MB_OK);
			return false;
		}
	}

	//密码判断
	char szPassword[LEN_PASSWORD] = {0};
	if ((mMeUserItem->GetMasterOrder()==0)&&(bEfficacyPass==true)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=MAX_CHAIR))
	{
		//变量定义
		bool bLocker=mTableViewFrame.GetLockerFlag(wTableID);

		//密码判断
		if(bLocker)
		{
			PLAZZ_PRINTF("%s",a_u8("//TODO:该桌已设置进入密码，请输入密码。 未实现!!!!\n"));
			return false;
		}
	}

	//清理界面
	if ((mReqTableID!=INVALID_TABLE)&&(mReqChairID!=INVALID_CHAIR)&&(mReqChairID!=MAX_CHAIR))
	{
		SHZIClientUserItem * pIClientUserItem=mTableViewFrame.GetClientUserItem(mReqTableID,mReqChairID);
		if (pIClientUserItem==mMeUserItem) mTableViewFrame.SetClientUserItem(mReqTableID,mReqChairID,0);
	}

	//设置界面
	if ((wChairID!=MAX_CHAIR)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		mTableViewFrame.VisibleTable(wTableID);
		mTableViewFrame.SetClientUserItem(wTableID,wChairID,mMeUserItem);
	}

	//设置变量
	mReqTableID=wTableID;
	mReqChairID=wChairID;
	mFindTableID=INVALID_TABLE;

	PLAZZ_PRINTF("CServerItem::PerformSitDownAction send...\n");
	//发送命令
	SendSitDownPacket(wTableID,wChairID,szPassword);

	return true;
}

//执行购买
bool SHZCServerItem::PerformBuyProperty(byte cbRequestArea,const char* pszNickName, word wItemCount, word wPropertyIndex)
{
	PLAZZ_PRINTF("CServerItem::PerformBuyProperty...\n");

	if (!IsService())
		return false;

	PLAZZ_PRINTF("%s", a_u8("//TODO:执行购买。 未实现!!!!\n"));
	return true;


}
