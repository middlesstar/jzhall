#include "CServerItem_SHZ.h"

//用户处理
bool SHZCServerItem::OnSocketMainUser(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//请求坐下失败
	case SUB_GR_REQUEST_FAILURE_SHZ:	return OnSocketSubRequestFailure(data,dataSize);
		//邀请玩家
	case SUB_GR_USER_INVITE_SHZ:		return OnSocketSubUserInvite(data,dataSize);
		//等待分配
	case SUB_GR_USER_WAIT_DISTRIBUTE_SHZ: return OnSocketSubWaitDistribute(data,dataSize);

		//用户进入
	case SUB_GR_USER_ENTER_SHZ:			return OnSocketSubUserEnter(data,dataSize);
		//用户积分
	case SUB_GR_USER_SCORE_SHZ:			return OnSocketSubUserScore(data,dataSize);
		//用户状态
	case SUB_GR_USER_STATUS_SHZ:		return OnSocketSubUserStatus(data,dataSize);
	
		//用户聊天
	case SUB_GR_USER_CHAT_SHZ:			return OnSocketSubUserChat(data,dataSize);
		//用户表情
	case SUB_GR_USER_EXPRESSION_SHZ:	return OnSocketSubExpression(data,dataSize);
		//用户私聊
	case SUB_GR_WISPER_CHAT_SHZ:		return OnSocketSubWisperUserChat(data,dataSize);
		//私聊表情
	case SUB_GR_WISPER_EXPRESSION_SHZ:	return OnSocketSubWisperExpression(data,dataSize);
	
		//道具成功
	case SUB_GR_PROPERTY_SUCCESS_SHZ:	return OnSocketSubPropertySuccess(data,dataSize);
		//道具失败
	case SUB_GR_PROPERTY_FAILURE_SHZ:	return OnSocketSubPropertyFailure(data,dataSize);
		//道具效应
	case SUB_GR_PROPERTY_EFFECT_SHZ:	return OnSocketSubPropertyEffect(data,dataSize);
		//礼物消息
	case SUB_GR_PROPERTY_MESSAGE_SHZ:   return OnSocketSubPropertyMessage(data,dataSize);
		//喇叭消息
	case SUB_GR_PROPERTY_TRUMPET_SHZ:	return OnSocketSubPropertyTrumpet(data,dataSize);

		//喜报消息
	case SUB_GR_GLAD_MESSAGE_SHZ:		return OnSocketSubGladMessage(data,dataSize);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 桌子状态
//请求失败
bool SHZCServerItem::OnSocketSubRequestFailure(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	int lErrorCode = packet.read4Byte();

	int len=(dataSize-4)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0],len);
	std::string sString = u2_8(&str[0]);

	//变量定义
	SHZIClientUserItem * pITableUserItem=0;
	word wMeTableID=mMeUserItem->GetTableID();
	word wMeChairID=mMeUserItem->GetChairID();


	//界面还原
	if ((mReqTableID!=INVALID_TABLE)&&(mReqChairID!=INVALID_TABLE)&&(mReqChairID!=MAX_CHAIR))
	{
		SHZIClientUserItem * pITableUserItem=mTableViewFrame.GetClientUserItem(mReqTableID,mReqChairID);
		if (pITableUserItem==mMeUserItem) 
			mTableViewFrame.SetClientUserItem(mReqTableID,mReqChairID,0);
	}

	//设置变量
	mReqTableID=INVALID_TABLE;
	mReqChairID=INVALID_CHAIR;

	PLAZZ_PRINTF("OnSocketSubRequestFailure %d : %d\n", mServiceStatus, SHZ_ServiceStatus_ServiceIng);
	if (mIsQuickSitDown)
	{
		mIsQuickSitDown = false;
		if (SHZCServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
		{//快速坐下,防作弊模式
			IntermitConnect(true);
			if (mIServerItemSink)
				mIServerItemSink->OnGFServerClose(sString.c_str());
		}
	}
	else 
	{
		//提示消息
		if (mIServerItemSink)
			mIServerItemSink->onGRRequestFailure(sString.c_str());
	}

	return true;

	////变量定义
	//CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)data;

	////效验参数
	//ASSERT(dataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	//if (dataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	////变量定义
	//IClientUserItem * pITableUserItem=0;
	//word wMeTableID=mMeUserItem->GetTableID();
	//word wMeChairID=mMeUserItem->GetChairID();

	//
	////界面还原
	//if ((mReqTableID!=INVALID_TABLE)&&(mReqChairID!=INVALID_TABLE)&&(mReqChairID!=MAX_CHAIR))
	//{
	//	IClientUserItem * pITableUserItem=mTableViewFrame.GetClientUserItem(mReqTableID,mReqChairID);
	//	if (pITableUserItem==mMeUserItem) 
	//		mTableViewFrame.SetClientUserItem(mReqTableID,mReqChairID,0);
	//}

	////设置变量
	//mReqTableID=INVALID_TABLE;
	//mReqChairID=INVALID_CHAIR;

	//PLAZZ_PRINTF("OnSocketSubRequestFailure %d : %d\n", mServiceStatus, ServiceStatus_ServiceIng);
	//if (mIsQuickSitDown)
	//{
	//	mIsQuickSitDown = false;
	//	if (CServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
	//	{//快速坐下,防作弊模式
	//		IntermitConnect(true);
	//		if (mIServerItemSink)
	//			mIServerItemSink->OnGFServerClose(pRequestFailure->szDescribeString);
	//	}
	//}
	//else 
	//{
	//	//提示消息
	//	if (mIServerItemSink)
	//		mIServerItemSink->onGRRequestFailure(pRequestFailure->szDescribeString);
	//}
	//
	//return true;
}

//等待分配
bool SHZCServerItem::OnSocketSubWaitDistribute(void *data, int dataSize)
{
	//更新桌面
	mTableViewFrame.SetTableStatus(true);

	//游戏通知
	OnSocketMainGameFrame(MDM_GF_FRAME_SHZ,SUB_GR_USER_WAIT_DISTRIBUTE_SHZ,data,dataSize);
	return true;
}

//邀请玩家
bool SHZCServerItem::OnSocketSubUserInvite(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	word wTableID = packet.read2Byte();
	dword dwUserID = packet.read4Byte();

	ASSERT(wTableID<mTableViewFrame.GetTableCount());
	if (mTableViewFrame.GetPlayFlag(wTableID)==true) 
		return true;

	//寻找用户
	SHZIClientUserItem * pIUserItem=mUserManager->SearchUserByUserID(dwUserID);
	if (pIUserItem==0) return true;
	if (pIUserItem->GetTableID()==INVALID_TABLE) return true;

	//变量定义
	ASSERT(SHZCParameterGlobal::shared()!=0);
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//用户过虑
	if (pIUserItem->GetUserCompanion()==CP_DETEST) return true;
	if (pParameterGlobal->m_cbInviteMode==INVITE_MODE_NONE_SHZ) return true;
	if ((pParameterGlobal->m_cbInviteMode==INVITE_MODE_FRIEND_SHZ)&&(pIUserItem->GetUserCompanion()!=CP_FRIEND)) return true;

	//提示消息
	char szMessage[256]={0};
	sprintf(szMessage, a_u8("[ %s ] 邀请您加入第 [ %ld ] 游戏桌进行游戏，是否同意？"),pIUserItem->GetNickName(),wTableID+1);

	//提示消息
	if (mIServerItemSink)
	{
		PLAZZ_PRINTF("%s",a_u8("//TODO:未实现\n"));
		//if (mIServerItemSink->OnGFUserInvite(szMessage))
		//{
		//	word wChairID=INVALID_CHAIR;
		//	word wNullCount=mTableViewFrame.GetNullChairCount(wTableID,wChairID);
		//	if (wNullCount==0) 
		//	{
		//		mIServerItemSink->OnGFUserInviteFailure("此游戏桌已经没有空位置了！");
		//		return true;
		//	}
		//	PerformSitDownAction(wTableID,wChairID,false);
		//}
	}

	return true;

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_UserInvite));
	//if (dataSize!=sizeof(CMD_GR_UserInvite)) return false;

	////消息处理
	//CMD_GR_UserInvite * pUserInvite=(CMD_GR_UserInvite *)data;
	//ASSERT(pUserInvite->wTableID<mTableViewFrame.GetTableCount());
	//if (mTableViewFrame.GetPlayFlag(pUserInvite->wTableID)==true) 
	//	return true;

	////寻找用户
	//IClientUserItem * pIUserItem=mUserManager->SearchUserByUserID(pUserInvite->dwUserID);
	//if (pIUserItem==0) return true;
	//if (pIUserItem->GetTableID()==INVALID_TABLE) return true;

	////变量定义
	//ASSERT(CParameterGlobal::shared()!=0);
	//CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	////用户过虑
	//if (pIUserItem->GetUserCompanion()==CP_DETEST) return true;
	//if (pParameterGlobal->m_cbInviteMode==INVITE_MODE_NONE_SHZ) return true;
	//if ((pParameterGlobal->m_cbInviteMode==INVITE_MODE_FRIEND_SHZ)&&(pIUserItem->GetUserCompanion()!=CP_FRIEND)) return true;

	////提示消息
	//char szMessage[256]=T_T("");
	//_sntprintf(szMessage,countarray(szMessage),T_T("[ %s ] 邀请您加入第 [ %ld ] 游戏桌进行游戏，是否同意？"),pIUserItem->GetNickName(),pUserInvite->wTableID+1);

	////提示消息
	//if (mIServerItemSink)
	//{
	//	if (mIServerItemSink->OnGFUserInvite(szMessage))
	//	{
	//		word wChairID=INVALID_CHAIR;
	//		word w0Count=mTableViewFrame.Get0ChairCount(pUserInvite->wTableID,wChairID);
	//		if (w0Count==0) 
	//		{
	//			mIServerItemSink->OnGFUserInviteFailure(T_T("此游戏桌已经没有空位置了！"));
	//			return true;
	//		}
	//		PerformSitDownAction(pUserInvite->wTableID,wChairID,false);
	//	}
	//}

	//return true;
}

//////////////////////////////////////////////////////////////////////////
//用户信息
#define READ_PACKET(szOut)														\
int len=wDataSize/2;															\
u2string str;																	\
str.resize(len+1,'\0');															\
packet.readUTF16(&str[0], len);													\
strncpy(szOut, u2_8(&str[0]), countarray(szOut))

//用户进入
bool SHZCServerItem::OnSocketSubUserEnter(void* data, int dataSize)
{
	//变量定义
	tagSHZ_UserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	lotteryInfo lotteryInfo;
	zeromemory(&UserInfo,sizeof(UserInfo));
	zeromemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	SHZCGlobalUserInfo * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	PACKET_AIDE_DATA(data);
	//用户属性
	UserInfo.dwGameID=packet.read4Byte();
	UserInfo.dwUserID=packet.read4Byte();
	UserInfo.dwGroupID=packet.read4Byte();
	UserInfo.wFaceID=packet.read2Byte();
	UserInfo.dwCustomID=packet.read4Byte();

	//用户属性
	UserInfo.cbGender=packet.readByte();
	UserInfo.cbMemberOrder=packet.readByte();
	UserInfo.cbMasterOrder=packet.readByte();

	//用户状态
	UserInfo.wTableID=packet.read2Byte();
	UserInfo.wChairID=packet.read2Byte();
	UserInfo.cbUserStatus=packet.readByte();
	
	//用户积分
	UserInfo.lScore=packet.read8Byte();
	lotteryInfo.lYuanBaoScore = packet.rSCORE();
	lotteryInfo.lShExpLottery = packet.rSCORE();
	UserInfo.lGrade=packet.read8Byte();
	UserInfo.lInsure=packet.read8Byte();

	UserInfo.dwWinCount=packet.read4Byte();
	UserInfo.dwLostCount=packet.read4Byte();
	UserInfo.dwDrawCount=packet.read4Byte();
	UserInfo.dwFleeCount=packet.read4Byte();
	UserInfo.dwUserMedal=packet.read4Byte();
	UserInfo.dwExperience=packet.read4Byte();
	UserInfo.lLoveLiness=packet.read4Byte();

	PLAZZ_PRINTF("--------------lotteryInfo.lYuanBaoScore = %d----------",lotteryInfo.lYuanBaoScore);
	PLAZZ_PRINTF("--------------lotteryInfo.lShExpLottery = %d----------",lotteryInfo.lShExpLottery);

	PLAZZ_PRINTF("--------------UserInfo.lScore = %ld----------", UserInfo.lScore);
	PLAZZ_PRINTF("--------------UserInfo.lInsure = %ld----------", UserInfo.lInsure);
	//TODOSL
	if (pGlobalUserData->dwUserID == UserInfo.dwUserID)
	{
		LotteryMission::getInstance()->setLotteryInfo(lotteryInfo);
	}
	//扩展信息
	while (dataSize>packet.getPosition())
	{
		word wDataSize = packet.read2Byte();
		word wDataDescribe = packet.read2Byte();

		if (wDataDescribe==0) break;
		switch (wDataDescribe)
		{
		case DTP_GR_NICK_NAME_SHZ:		//用户昵称
			{
				READ_PACKET(UserInfo.szNickName);
				break;
			}
		case DTP_GR_GROUP_NAME_SHZ:		//用户社团
			{
				READ_PACKET(UserInfo.szGroupName);
				break;
			}
		case DTP_GR_UNDER_WRITE_SHZ:	//个性签名
			{
				READ_PACKET(UserInfo.szUnderWrite);
				break;
			}
		}
	
	}

	//自定头像
	if (UserInfo.dwCustomID!=0L)
	{
		////加载头像
		//CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
		//bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

		////下载头像
		//if (bSuccess==false)
		//{
		//	pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
		//}
	}

	//变量定义
	bool bHideUserInfo=SHZCServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==UserInfo.dwUserID;
	bool bMasterUserOrder=((UserInfo.cbMasterOrder>0)||((mMeUserItem!=0)&&(mMeUserItem->GetMasterOrder()>0)));

	//隐藏信息
	if (!((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true)))
	{
		//用户信息
		strncpy(UserInfo.szNickName,a_u8("游戏玩家"),countarray(UserInfo.szNickName));
		//..
	}

	//激活用户
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==0) pIClientUserItem=mUserManager->ActiveUserItem(UserInfo,CustomFaceInfo);

	//获取对象
	HallServerListData * pServerListData=HallServerListData::shared();

	//人数更新
	if (pServerListData)
		pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());

	//变量定义
	ASSERT(SHZCParameterGlobal::shared()!=0);
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//好友提示
	if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	{
		if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
		{
			char szMessage[256]={0};
			sprintf(szMessage, a_u8("您的好友 [%s] 进来了！"), pIClientUserItem->GetNickName());
			//提示消息
			if (mIChatSink)
				mIChatSink->InsertSystemChat(szMessage);
		}
	}

	return true;

	////效验参数
	ASSERT(dataSize>=sizeof(tagUserInfoHead));
	//if (dataSize<sizeof(tagUserInfoHead)) return false;

	////变量定义
	//tagUserInfo UserInfo;
	//tagCustomFaceInfo CustomFaceInfo;
	//zeromemory(&UserInfo,sizeof(UserInfo));
	//zeromemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	////消息处理
	//tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)data;

	////变量定义
	//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	////变量定义
	//bool bHideUserInfo=CServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule);
	//bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	//bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((mMeUserItem!=0)&&(mMeUserItem->GetMasterOrder()>0)));

	////读取信息
	//if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	//{
	//	//用户属性
	//	UserInfo.wFaceID=pUserInfoHead->wFaceID;
	//	UserInfo.dwGameID=pUserInfoHead->dwGameID;
	//	UserInfo.dwUserID=pUserInfoHead->dwUserID;
	//	UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
	//	UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

	//	//用户状态
	//	UserInfo.wTableID=pUserInfoHead->wTableID;
	//	UserInfo.wChairID=pUserInfoHead->wChairID;
	//	UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

	//	//用户属性
	//	UserInfo.cbGender=pUserInfoHead->cbGender;
	//	UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	//	UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

	//	//用户积分
	//	UserInfo.lScore=pUserInfoHead->lScore;
	//	UserInfo.lGrade=pUserInfoHead->lGrade;
	//	UserInfo.lInsure=pUserInfoHead->lInsure;
	//	UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
	//	UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
	//	UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
	//	UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;
	//	UserInfo.dwUserMedal=pUserInfoHead->dwUserMedal;
	//	UserInfo.dwExperience=pUserInfoHead->dwExperience;
	//	UserInfo.lLoveLiness=pUserInfoHead->lLoveLiness;

	//	//变量定义
	//	void * dataBuffer=0;
	//	tagDataDescribe DataDescribe;
	//	CRecvPacketHelper RecvPacket(pUserInfoHead+1,dataSize-sizeof(tagUserInfoHead));

	//	//扩展信息
	//	while (true)
	//	{
	//		dataBuffer=RecvPacket.GetData(DataDescribe);
	//		if (DataDescribe.wDataDescribe==DTP_NULL) break;
	//		switch (DataDescribe.wDataDescribe)
	//		{
	//		case DTP_GR_NICK_NAME_SHZ:		//用户昵称
	//			{
	//				ASSERT(dataBuffer!=0);
	//				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
	//				if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
	//				{
	//					memcpy(&UserInfo.szNickName,dataBuffer,DataDescribe.wDataSize);
	//					UserInfo.szNickName[countarray(UserInfo.szNickName)-1]=0;
	//				}
	//				break;
	//			}
	//		case DTP_GR_GROUP_NAME_SHZ:		//用户社团
	//			{
	//				ASSERT(dataBuffer!=0);
	//				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
	//				if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
	//				{
	//					memcpy(&UserInfo.szGroupName,dataBuffer,DataDescribe.wDataSize);
	//					UserInfo.szGroupName[countarray(UserInfo.szGroupName)-1]=0;
	//				}
	//				break;
	//			}
	//		case DTP_GR_UNDER_WRITE_SHZ:	//个性签名
	//			{
	//				ASSERT(dataBuffer!=0);
	//				ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
	//				if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
	//				{
	//					memcpy(UserInfo.szUnderWrite,dataBuffer,DataDescribe.wDataSize);
	//					UserInfo.szUnderWrite[countarray(UserInfo.szUnderWrite)-1]=0;
	//				}
	//				break;
	//			}
	//		}
	//	}

	//	//自定头像
	//	if (pUserInfoHead->dwCustomID!=0L)
	//	{
	//		////加载头像
	//		//CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	//		//bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

	//		////下载头像
	//		//if (bSuccess==false)
	//		//{
	//		//	pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
	//		//}
	//	}
	//}
	//else
	//{
	//	//用户信息
	//	UserInfo.dwUserID=pUserInfoHead->dwUserID;
	//	tstrcpyn(UserInfo.szNickName,T_T("游戏玩家"),countarray(UserInfo.szNickName));

	//	//用户状态
	//	UserInfo.wTableID=pUserInfoHead->wTableID;
	//	UserInfo.wChairID=pUserInfoHead->wChairID;
	//	UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

	//	//用户属性
	//	UserInfo.cbGender=pUserInfoHead->cbGender;
	//	UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	//	UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	//}

	////激活用户
	//IClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(UserInfo.dwUserID);
	//if (pIClientUserItem==0) pIClientUserItem=mUserManager->ActiveUserItem(UserInfo,CustomFaceInfo);

	////获取对象
	//CServerListData * pServerListData=CServerListData::shared();

	////人数更新
	//if (pServerListData)
	//	pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());

	////变量定义
	//ASSERT(CParameterGlobal::shared()!=0);
	//CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	////好友提示
	//if (((bHideUserInfo==false)&&(bMySelfUserItem==false))||(bMasterUserOrder==true))
	//{
	//	if(pParameterGlobal->m_bNotifyFriendCome && pIClientUserItem->GetUserCompanion()==CP_FRIEND)
	//	{
	//		char szMessage[256]={0};
	//		_sntprintf(szMessage, 256, T_T("您的好友 [%s] 进来了！"), pIClientUserItem->GetNickName());
	//		//提示消息
	//		if (mIChatSink)
	//			mIChatSink->InsertSystemChat(szMessage);
	//	}
	//}

	//return true;
}

//用户积分
bool SHZCServerItem::OnSocketSubUserScore(void* data, int dataSize)
{
	tagUserScore UserScore;

  	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	UserScore.lScore = packet.read8Byte();
	UserScore.lGrade = packet.read8Byte();
	UserScore.lInsure = packet.read8Byte();
	UserScore.dwWinCount = packet.read4Byte();
	UserScore.dwLostCount = packet.read4Byte();
	UserScore.dwDrawCount = packet.read4Byte();
	UserScore.dwFleeCount = packet.read4Byte();
	UserScore.dwUserMedal = packet.read4Byte();
	UserScore.dwExperience = packet.read4Byte();
	UserScore.lLoveLiness = packet.read4Byte();

	//寻找用户
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//用户判断
	//ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return true;

	//变量定义
	SHZCGlobalUserInfo * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bMySelfUserItem=pGlobalUserData->dwUserID==dwUserID;
	bool bAvertCheatMode=SHZCServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule);
	bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(mMeUserItem->GetMasterOrder()>0));

	//更新用户
	if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		mUserManager->UpdateUserItemScore(pIClientUserItem,&UserScore);
	}

	return true;

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_UserScore));
	//if (dataSize<sizeof(CMD_GR_UserScore)) return false;

	////寻找用户
	//CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)data;
	//IClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(pUserScore->dwUserID);

	////用户判断
	//ASSERT(pIClientUserItem!=0);
	//if (pIClientUserItem==0) return true;

	////变量定义
	//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	////变量定义
	//bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserScore->dwUserID;
	//bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule);
	//bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(mMeUserItem->GetMasterOrder()>0));

	////更新用户
	//if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	//{
	//	mUserManager->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);
	//}

	//return true;
}

//用户状态
bool SHZCServerItem::OnSocketSubUserStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubUserStatus\n");
	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	tagUserStatus UserStatus;
	UserStatus.wTableID = packet.read2Byte();
	UserStatus.wChairID = packet.read2Byte();
	UserStatus.cbUserStatus = packet.readByte();

	//寻找用户
	SHZIClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(dwUserID);

	//用户判断
	//ASSERT(pIClientUserItem!=0);
	if (pIClientUserItem==0) return true;

	//设置状态
	if (UserStatus.cbUserStatus==US_NULL) 
	{
		//删除用户
		mUserManager->DeleteUserItem(pIClientUserItem);

		//获取对象
		HallServerListData * pServerListData=HallServerListData::shared();

		//人数更新
		pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());
	}
	else
	{
		//更新用户
		mUserManager->UpdateUserItemStatus(pIClientUserItem,&UserStatus);
	}

	return true;

	//_tprintf(T_T("CServerItem::OnSocketSubUserStatus\n"));
	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_UserStatus));
	//if (dataSize<sizeof(CMD_GR_UserStatus)) return false;

	////寻找用户
	//CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)data;
	//IClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(pUserStatus->dwUserID);

	////用户判断
	//ASSERT(pIClientUserItem!=0);
	//if (pIClientUserItem==0) return true;

	////设置状态
	//if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	//{
	//	//删除用户
	//	mUserManager->DeleteUserItem(pIClientUserItem);

	//	//获取对象
	//	CServerListData * pServerListData=CServerListData::shared();

	//	//人数更新
	//	pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());
	//}
	//else
	//{
	//	//更新用户
	//	mUserManager->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	//}

	//return true;
}

//////////////////////////////////////////////////////////////////////////
//聊天信息
//用户聊天
bool SHZCServerItem::OnSocketSubUserChat(void* data, int dataSize)
{
	if (mIChatSink==0)
		return true;

	PACKET_AIDE_DATA(data);
	word wChatLength = packet.read2Byte();
	dword dwChatColor = packet.read4Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	u2string str;
	str.resize(wChatLength+1,'\0');
	packet.readUTF16(&str[0], wChatLength);
	const std::string sString = u2_8(&str[0]);

	dwChatColor = ((dwChatColor & 0xFF) << 16) | (dwChatColor >> 8);

	//获取用户
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSendUserID);
	if (pISendUserItem==0) return true;

	//消息过虑
	if (pISendUserItem!=mMeUserItem)
	{
		//社团变量
		dword dwUserGroupID=pISendUserItem->GetGroupID();
		dword dwMySelfGroupID=mMeUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		byte cbUserCompanion=pISendUserItem->GetUserCompanion();
		SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//显示消息
	if (dwTargetUserID!=0L)
	{
		//获取用户
		SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
		if (pIRecvUserItem==0) return true;

		mIChatSink->InsertUserChat(
			pISendUserItem->GetNickName(),
			pIRecvUserItem->GetNickName(),sString.c_str(),dwChatColor);
	}
	else 
	{
		//显示消息
		
		mIChatSink->InsertUserChat(pISendUserItem->GetNickName(), sString.c_str(),dwChatColor);
	}

	return true;

	////变量定义
	//CMD_GR_S_UserChat * pUserChat=(CMD_GR_S_UserChat *)data;

	////效验参数
	//ASSERT(dataSize>=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)));
	//ASSERT(dataSize==(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	////效验参数
	//if (dataSize<(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	//if (dataSize!=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	////获取用户
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pUserChat->dwSendUserID);
	//if (pISendUserItem==0) return true;

	////消息过虑
	//if (pISendUserItem!=mMeUserItem)
	//{
	//	//社团变量
	//	dword dwUserGroupID=pISendUserItem->GetGroupID();
	//	dword dwMySelfGroupID=mMeUserItem->GetGroupID();

	//	//社团判断
	//	bool bSameGroup=false;
	//	if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

	//	//变量定义
	//	byte cbUserCompanion=pISendUserItem->GetUserCompanion();
	//	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//	//好友模式
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//厌恶模式
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}
	//}

	////显示消息
	//if (pUserChat->dwTargetUserID!=0L)
	//{
	//	//获取用户
	//	IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pUserChat->dwTargetUserID);
	//	if (pIRecvUserItem==0) return true;

	//	//显示消息
	//	const char* pszSendUser=pISendUserItem->GetNickName();
	//	const char* pszRecvUser=pIRecvUserItem->GetNickName();
	//	dword dwColor = pUserChat->dwChatColor;
	//	// 颜色调换
	//	dwColor = ((dwColor & 0xFF) << 16) | (dwColor >> 8);

	//	if (mIChatSink)
	//		mIChatSink->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,dwColor);
	//}
	//else 
	//{
	//	//显示消息
	//	const char* pszSendUser=pISendUserItem->GetNickName();
	//	dword dwColor = pUserChat->dwChatColor;
	//	// 颜色调换
	//	dwColor = ((dwColor & 0xFF) << 16) | (dwColor >> 8);

	//	if (mIChatSink)
	//		mIChatSink->InsertUserChat(pszSendUser,pUserChat->szChatString,dwColor);
	//}

	//return true;
}

//用户表情
bool SHZCServerItem::OnSocketSubExpression(void* data, int dataSize)
{
	if (mIChatSink==0)
		return true;
	PACKET_AIDE_DATA(data);
	word wItemIndex = packet.read2Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	//获取用户
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSendUserID);
	if (pISendUserItem==0) return true;

	//消息过虑
	if (pISendUserItem!=mMeUserItem)
	{
		//社团变量
		dword dwUserGroupID=pISendUserItem->GetGroupID();
		dword dwMySelfGroupID=mMeUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		byte cbUserCompanion=pISendUserItem->GetUserCompanion();
		SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//公聊消息
	if (dwTargetUserID==0L)
	{
		mIChatSink->InsertExpression(pISendUserItem->GetNickName(), wItemIndex);
		return true;
	}

	//私聊消息
	if (dwTargetUserID!=0L)
	{
		//获取用户
		SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
		if (pIRecvUserItem==0) return true;

		//显示消息
		mIChatSink->InsertExpression(pISendUserItem->GetNickName(),pIRecvUserItem->GetNickName(),wItemIndex);
	}

	return true;

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_S_UserExpression));
	//if (dataSize!=sizeof(CMD_GR_S_UserExpression)) return false;

	////变量定义
	//CMD_GR_S_UserExpression * pUserExpression=(CMD_GR_S_UserExpression *)data;

	////获取用户
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pUserExpression->dwSendUserID);
	//if (pISendUserItem==0) return true;

	////消息过虑
	//if (pISendUserItem!=mMeUserItem)
	//{
	//	//社团变量
	//	dword dwUserGroupID=pISendUserItem->GetGroupID();
	//	dword dwMySelfGroupID=mMeUserItem->GetGroupID();

	//	//社团判断
	//	bool bSameGroup=false;
	//	if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

	//	//变量定义
	//	byte cbUserCompanion=pISendUserItem->GetUserCompanion();
	//	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//	//好友模式
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//厌恶模式
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}
	//}

	////公聊消息
	//if (pUserExpression->dwTargetUserID==0L)
	//{
	//	if (mIChatSink)
	//		mIChatSink->InsertExpression(pISendUserItem->GetNickName(), pUserExpression->wItemIndex);
	//}

	////私聊消息
	//if (pUserExpression->dwTargetUserID!=0L)
	//{
	//	//获取用户
	//	IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pUserExpression->dwTargetUserID);
	//	if (pIRecvUserItem==0) return true;

	//	//显示消息
	//	const char* pszSendUser=pISendUserItem->GetNickName();
	//	const char* pszRecvUser=pIRecvUserItem->GetNickName();
	//	if (mIChatSink)
	//		mIChatSink->InsertExpression(pszSendUser,pszRecvUser,pUserExpression->wItemIndex);
	//}

	//return true;
}

//用户私聊
bool SHZCServerItem::OnSocketSubWisperUserChat(void* data, int dataSize)
{
	if (mIChatSink==0)
		return true;

	PACKET_AIDE_DATA(data);
	word wChatLength = packet.read2Byte();
	dword dwChatColor = packet.read4Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	u2string str;
	str.resize(wChatLength+1,'\0');
	packet.readUTF16(&str[0], wChatLength);
	const std::string sString = u2_8(&str[0]);

	dwChatColor = ((dwChatColor & 0xFF) << 16) | (dwChatColor >> 8);

	//获取用户
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSendUserID);
	if (pISendUserItem==0) return true;

	//接收用户
	SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
	if (pIRecvUserItem==0) return true;

	//消息过虑
	if (pISendUserItem!=mMeUserItem)
	{
		//社团变量
		dword dwUserGroupID=pISendUserItem->GetGroupID();
		dword dwMySelfGroupID=mMeUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		byte cbUserCompanion=pISendUserItem->GetUserCompanion();
		SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//屏蔽用户
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	mIChatSink->InsertWhisperChat(pISendUserItem->GetNickName(), sString.c_str(), dwChatColor, pISendUserItem == mMeUserItem);
	return true;

	////变量定义
	//CMD_GR_S_WisperChat * pWisperChat=(CMD_GR_S_WisperChat *)data;

	////效验参数
	//ASSERT(dataSize>=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)));
	//ASSERT(dataSize==(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	////效验参数
	//if (dataSize<(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	//if (dataSize!=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	////获取用户
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pWisperChat->dwSendUserID);
	//if (pISendUserItem==0) return true;

	////接收用户
	//IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pWisperChat->dwTargetUserID);
	//if (pIRecvUserItem==0) return true;

	////消息过虑
	//if (pISendUserItem!=mMeUserItem)
	//{
	//	//社团变量
	//	dword dwUserGroupID=pISendUserItem->GetGroupID();
	//	dword dwMySelfGroupID=mMeUserItem->GetGroupID();

	//	//社团判断
	//	bool bSameGroup=false;
	//	if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

	//	//变量定义
	//	byte cbUserCompanion=pISendUserItem->GetUserCompanion();
	//	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//	//好友模式
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//厌恶模式
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//屏蔽用户
	//	if(cbUserCompanion == CP_SHIELD) return true;
	//}

	//dword dwColor = pWisperChat->dwChatColor;
	//// 颜色调换
	//dwColor = ((dwColor & 0xFF) << 16) | (dwColor >> 8);
	//if (mIChatSink)
	//	mIChatSink->InsertWhisperChat(pISendUserItem->GetNickName(), pWisperChat->szChatString, dwColor, pISendUserItem == mMeUserItem);
	//return true;
}

//私聊表情
bool SHZCServerItem::OnSocketSubWisperExpression(void* data, int dataSize)
{
	if (mIChatSink==0)
		return true;
	PACKET_AIDE_DATA(data);
	word wItemIndex = packet.read2Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	//获取用户
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSendUserID);
	if (pISendUserItem==0) return true;

	//接收用户
	SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
	if (pIRecvUserItem==0) return true;

	//消息过虑
	if (pISendUserItem!=mMeUserItem)
	{
		//社团变量
		dword dwUserGroupID=pISendUserItem->GetGroupID();
		dword dwMySelfGroupID=mMeUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		byte cbUserCompanion=pISendUserItem->GetUserCompanion();
		SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}

		//屏蔽用户
		if(cbUserCompanion == CP_SHIELD) return true;
	}

	mIChatSink->InsertExpression(pISendUserItem->GetNickName(), pIRecvUserItem->GetNickName(), wItemIndex);

	return true;
	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_S_WisperExpression));
	//if (dataSize!=sizeof(CMD_GR_S_WisperExpression)) return false;

	////变量定义
	//CMD_GR_S_WisperExpression * pWisperExpression=(CMD_GR_S_WisperExpression *)data;

	////获取用户
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pWisperExpression->dwSendUserID);
	//if (pISendUserItem==0) return true;

	////接收用户
	//IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pWisperExpression->dwTargetUserID);
	//if (pIRecvUserItem==0) return true;

	////消息过虑
	//if (pISendUserItem!=mMeUserItem)
	//{
	//	//社团变量
	//	dword dwUserGroupID=pISendUserItem->GetGroupID();
	//	dword dwMySelfGroupID=mMeUserItem->GetGroupID();

	//	//社团判断
	//	bool bSameGroup=false;
	//	if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

	//	//变量定义
	//	byte cbUserCompanion=pISendUserItem->GetUserCompanion();
	//	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//	//好友模式
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//厌恶模式
	//	if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//屏蔽用户
	//	if(cbUserCompanion == CP_SHIELD) return true;
	//}

	//if (mIChatSink)
	//	mIChatSink->InsertExpression(pISendUserItem->GetNickName(), pIRecvUserItem->GetNickName(), pWisperExpression->wItemIndex);

	//return true;
}

//////////////////////////////////////////////////////////////////////////
//道具信息
//道具成功
bool SHZCServerItem::OnSocketSubPropertySuccess(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	byte cbRequestArea = packet.readByte();
	word wItemCount = packet.read2Byte();
	word wPropertyIndex = packet.read2Byte();
	dword dwSourceUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	//获取用户
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwSourceUserID);
	if (pISendUserItem==0) return true;

	//获取用户
	SHZIClientUserItem * pTargetUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);
	if (pTargetUserItem==0) return true;

	//使用范围
	if(cbRequestArea==PT_ISSUE_AREA_GAME)
	{
		PLAZZ_PRINTF("%s", a_u8("恭喜您,游戏道具购买成功!\n"));
		//if (mIPropertySink)
		//	mIPropertySink->onPropertyBuySuccess(
		//		pPropertySuccess->wPropertyIndex,
		//		pPropertySuccess->wItemCount, 
		//		pPropertySuccess->dwSourceUserID,
		//		pPropertySuccess->dwTargetUserID);
		return true;
	}

	PLAZZ_PRINTF("%s", a_u8("恭喜您,房间道具购买成功!\n"));

	////获取对象
	//ASSERT(CGamePropertyManager::GetInstance()!=0);
	//CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	////寻找道具
	//ASSERT(pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex)!=0);
	//CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex);

	////道具处理
	//if (pGamePropertyItem!=0 && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	//{
	//	//图片信息
	//	tagPropertyImage PropertyImage;
	//	pGamePropertyItem->GetImageInfo(PropertyImage);

	//	//播放动画
	//	if ((PropertyImage.pszFlash!=0)&&(PropertyImage.pszFlash[0]!=0))
	//	{
	//		//工作路径
	//		char szDirectory[MAX_PATH]=T_T("");
	//		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//		//构造路径
	//		char szFlashPath[MAX_PATH]=T_T("");
	//		_sntprintf(szFlashPath,CountArray(szFlashPath),T_T("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

	//		try
	//		{
	//			//播放动画
	//			if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
	//			{
	//				throw T_T("动画文件加载失败");
	//			}

	//			//动画控件
	//			CRect rcWindow;
	//			m_TableViewFrame.GetWindowRect(&rcWindow);
	//			m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

	//			//播放动画
	//			m_FlashControl.PlayMovie();
	//			m_FlashControl.ShowControl(SW_SHOW);
	//		}
	//		catch (...)
	//		{
	//			ASSERT(FALSE);
	//		}
	//	}
	//}

	////道具提示
	//if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	//{
	//	//变量定义
	//	char  szMessage[256]=T_T("");

	//	//获取提示
	//	if(pPropertySuccess->dwSourceUserID==mMeUserItem->GetUserID() && pPropertySuccess->dwSourceUserID!=pPropertySuccess->dwTargetUserID)
	//		_sntprintf(szMessage,CountArray(szMessage),T_T("恭喜您,道具购买成功!"));
	//	else
	//		pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),mMeUserItem->GetNickName(),pPropertySuccess->wItemCount,szMessage);

	//	//弹出消息
	//	CInformation Information;
	//	Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);

	//	//更新喇叭
	//	if(m_pTrumpetItem!=0)  m_pTrumpetItem->UpdateControlSurface();
	//}

	return true;

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_S_PropertySuccess));
	//if (dataSize!=sizeof(CMD_GR_S_PropertySuccess)) return false;

	////变量定义
	//CMD_GR_S_PropertySuccess * pPropertySuccess=(CMD_GR_S_PropertySuccess *)data;

	////获取用户
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pPropertySuccess->dwSourceUserID);
	//if (pISendUserItem==0) return true;

	////获取用户
	//IClientUserItem * pTargetUserItem=mUserManager->SearchUserByUserID(pPropertySuccess->dwTargetUserID);
	//if (pTargetUserItem==0) return true;

	////使用范围
	//if(pPropertySuccess->cbRequestArea==PT_ISSUE_AREA_GAME)
	//{
	//	_tprintf(T_T("恭喜您,游戏道具购买成功!\n"));
	//	//if (mIPropertySink)
	//	//	mIPropertySink->onPropertyBuySuccess(
	//	//		pPropertySuccess->wPropertyIndex,
	//	//		pPropertySuccess->wItemCount, 
	//	//		pPropertySuccess->dwSourceUserID,
	//	//		pPropertySuccess->dwTargetUserID);
	//	return true;
	//}

	//_tprintf(T_T("恭喜您,房间道具购买成功!\n"));

	//////获取对象
	////ASSERT(CGamePropertyManager::GetInstance()!=0);
	////CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//////寻找道具
	////ASSERT(pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex)!=0);
	////CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertySuccess->wPropertyIndex);

	//////道具处理
	////if (pGamePropertyItem!=0 && pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
	////{
	////	//图片信息
	////	tagPropertyImage PropertyImage;
	////	pGamePropertyItem->GetImageInfo(PropertyImage);

	////	//播放动画
	////	if ((PropertyImage.pszFlash!=0)&&(PropertyImage.pszFlash[0]!=0))
	////	{
	////		//工作路径
	////		char szDirectory[MAX_PATH]=T_T("");
	////		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	////		//构造路径
	////		char szFlashPath[MAX_PATH]=T_T("");
	////		_sntprintf(szFlashPath,CountArray(szFlashPath),T_T("%s\\Property\\%s"),szDirectory,PropertyImage.pszFlash);

	////		try
	////		{
	////			//播放动画
	////			if (m_FlashControl.LoadMovieFile(szFlashPath)==false)
	////			{
	////				throw T_T("动画文件加载失败");
	////			}

	////			//动画控件
	////			CRect rcWindow;
	////			m_TableViewFrame.GetWindowRect(&rcWindow);
	////			m_FlashControl.SetPosition(rcWindow.left+(rcWindow.Width()-FALSH_MOVIE_CX)/2,rcWindow.top+(rcWindow.Height()-FALSH_MOVIE_CY)*2/5,FALSH_MOVIE_CX,FALSH_MOVIE_CY,SWP_NOSIZE);

	////			//播放动画
	////			m_FlashControl.PlayMovie();
	////			m_FlashControl.ShowControl(SW_SHOW);
	////		}
	////		catch (...)
	////		{
	////			ASSERT(FALSE);
	////		}
	////	}
	////}

	//////道具提示
	////if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
	////{
	////	//变量定义
	////	char  szMessage[256]=T_T("");

	////	//获取提示
	////	if(pPropertySuccess->dwSourceUserID==mMeUserItem->GetUserID() && pPropertySuccess->dwSourceUserID!=pPropertySuccess->dwTargetUserID)
	////		_sntprintf(szMessage,CountArray(szMessage),T_T("恭喜您,道具购买成功!"));
	////	else
	////		pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),mMeUserItem->GetNickName(),pPropertySuccess->wItemCount,szMessage);

	////	//弹出消息
	////	CInformation Information;
	////	Information.ShowMessageBox(szMessage,MB_ICONINFORMATION);

	////	//更新喇叭
	////	if(m_pTrumpetItem!=0)  m_pTrumpetItem->UpdateControlSurface();
	////}

	//return true;
}

//道具失败
bool SHZCServerItem::OnSocketSubPropertyFailure(void* data, int dataSize)
{
	//变量定义
	PACKET_AIDE_DATA(data);
	word wRequestArea = packet.read2Byte();
	int lErrorCode = packet.read4Byte();

	int len = (dataSize-packet.getPosition())/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0], len);
	const std::string sString = u2_8(&str[0]);
	
	//使用范围
	if(wRequestArea==PT_ISSUE_AREA_GAME)
	{
		PLAZZ_PRINTF("%s", a_u8("游戏道具购买失败! [%d] %s \n"), lErrorCode, sString.c_str());
		//if (mIPropertySink)
		//	mIPropertySink->onPropertyBuyFailure(
		//		pPropertyFailure->lErrorCode,
		//		pPropertyFailure->szDescribeString);
		return true;
	}

	PLAZZ_PRINTF("%s", a_u8("房间道具购买失败! [%d] %s \n"), lErrorCode, sString.c_str());

	return true;
	////变量定义
	//CMD_GR_PropertyFailure * pPropertyFailure=(CMD_GR_PropertyFailure *)data;
	//if(pPropertyFailure==0) return false;

	////使用范围
	//if(pPropertyFailure->wRequestArea==PT_ISSUE_AREA_GAME)
	//{
	//	_tprintf(T_T("游戏道具购买失败! [%d] %s \n"), pPropertyFailure->lErrorCode, pPropertyFailure->szDescribeString);
	//	//if (mIPropertySink)
	//	//	mIPropertySink->onPropertyBuyFailure(
	//	//		pPropertyFailure->lErrorCode,
	//	//		pPropertyFailure->szDescribeString);
	//	return true;
	//}

	//_tprintf(T_T("房间道具购买失败! [%d] %s \n"), 
	//	pPropertyFailure->lErrorCode, 
	//	pPropertyFailure->szDescribeString);

	//return true;
}

//道具效应
bool SHZCServerItem::OnSocketSubPropertyEffect(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	byte cbMemberOrder = packet.readByte();

	//查找用户
	SHZIClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(dwUserID);
	if(pISendUserItem==0) return true;

	//设置变量
	tagSHZ_UserInfo * pUserInfo = pISendUserItem->GetUserInfo();
	pUserInfo->cbMemberOrder=cbMemberOrder;

	////更新信息
	//m_UserListControl.UpdateDataItem(pISendUserItem);

	return true;
	////参数校验
	//ASSERT(dataSize==sizeof(CMD_GR_S_PropertyEffect));
	//if(dataSize!=sizeof(CMD_GR_S_PropertyEffect))  return false;

	////变量定义
	//CMD_GR_S_PropertyEffect * pPropertyEffect=(CMD_GR_S_PropertyEffect *)data;
	//if(pPropertyEffect==0) return false;

	////查找用户
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserByUserID(pPropertyEffect->wUserID);
	//if(pISendUserItem==0) return true;

	////设置变量
	//tagUserInfo * pUserInfo = pISendUserItem->GetUserInfo();
	//pUserInfo->cbMemberOrder=pPropertyEffect->cbMemberOrder;

	//////更新信息
	////m_UserListControl.UpdateDataItem(pISendUserItem);

	//return true;
}

//道具消息
bool SHZCServerItem::OnSocketSubPropertyMessage(void* data, int dataSize)
{
	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_S_PropertyMessage));
	//if (dataSize!=sizeof(CMD_GR_S_PropertyMessage)) return false;

	////校验数据
	//CMD_GR_S_PropertyMessage * pPropertyMessage = (CMD_GR_S_PropertyMessage *)data;
	//if(pPropertyMessage==0) return false;

	////获取对象
	//ASSERT(CGamePropertyManager::GetInstance()!=0);
	//CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	////寻找道具
	//ASSERT(pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex)!=0);
	//CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(pPropertyMessage->wPropertyIndex);
	//if(pGamePropertyItem==0) return true;

	////类型判断
	//if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY) return true;

	////获取用户
	//IClientUserItem * pISendUserItem=mUserManager->SearchUserItemByUserID(pPropertyMessage->dwSourceUserID);
	//if (pISendUserItem==0) return true;

	////获取用户
	//IClientUserItem * pIRecvUserItem=mUserManager->SearchUserItemByUserID(pPropertyMessage->dwTargerUserID);
	//if (pIRecvUserItem==0) return true;

	////提取信息
	//char szNotifyMessage[128]=T_T("");	
	//pGamePropertyItem->GetNotifyMessage(pISendUserItem->GetNickName(),pIRecvUserItem->GetNickName(),pPropertyMessage->wPropertyCount,szNotifyMessage);

	////插入消息
	//m_ChatMessage.InsertPromptString(szNotifyMessage);

	//if(IsGameClientReady())
	//{
	//	//构造结构
	//	IPC_GR_PropertyMessage  PropertyMessage;
	//	PropertyMessage.wPropertyIndex=pPropertyMessage->wPropertyIndex;
	//	PropertyMessage.wPropertyCount=pPropertyMessage->wPropertyCount;
	//	memcpy(PropertyMessage.szSourceNickName,pISendUserItem->GetNickName(),sizeof(PropertyMessage.szSourceNickName));
	//	memcpy(PropertyMessage.szTargerNickName,pIRecvUserItem->GetNickName(),sizeof(PropertyMessage.szTargerNickName));

	//	//发送数据
	//	m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_MESSAGE,&PropertyMessage,sizeof(PropertyMessage));
	//}

	return true;
}

//道具喇叭
bool SHZCServerItem::OnSocketSubPropertyTrumpet(void* data, int dataSize)
{
	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_S_SendTrumpet));
	//if (dataSize!=sizeof(CMD_GR_S_SendTrumpet)) return false;

	////变量定义
	//CMD_GR_S_SendTrumpet * pSendTrumpet=(CMD_GR_S_SendTrumpet *)data;

	////插入消息
	//if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)
	//	m_ChatMessage.InsertUserTrumpet(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	////插入消息
	//if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON)
	//	m_ChatMessage.InsertUserTyphon(pSendTrumpet->szSendNickName,pSendTrumpet->szTrumpetContent,pSendTrumpet->TrumpetColor);

	////更新包裹
	//if(pSendTrumpet->dwSendUserID==mMeUserItem->GetUserID())
	//{
	//	//道具包裹
	//	tagPropertyPackage * pPropertyPackage=mMeUserItem->GetPropertyPackage();

	//	//更新包裹
	//	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET) --pPropertyPackage->wTrumpetCount;
	//	if(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TYPHON) --pPropertyPackage->wTyphonCount;
	//	if(m_pTrumpetItem!=0) m_pTrumpetItem->UpdateControlSurface();
	//}

	//if(IsGameClientReady())
	//{
	//	//构造结构
	//	IPC_GR_PropertyTrumpet  PropertyTrumpet;
	//	PropertyTrumpet.dwSendUserID=pSendTrumpet->dwSendUserID;
	//	PropertyTrumpet.wPropertyIndex=pSendTrumpet->wPropertyIndex;
	//	PropertyTrumpet.TrumpetColor=pSendTrumpet->TrumpetColor;
	//	memcpy(PropertyTrumpet.szSendNickName,pSendTrumpet->szSendNickName,sizeof(PropertyTrumpet.szSendNickName));
	//	memcpy(PropertyTrumpet.szTrumpetContent,pSendTrumpet->szTrumpetContent,sizeof(PropertyTrumpet.szTrumpetContent));

	//	//发送数据
	//	m_ProcessManager.SendProcessData(IPC_CMD_GF_PROPERTY_INFO,IPC_SUB_GR_PROPERTY_TRUMPET,&PropertyTrumpet,sizeof(PropertyTrumpet));
	//}

	////变量定义
	//ASSERT(CParameterGlobal::shared()!=0);
	//CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	////播放声音
	//if(pParameterGlobal->m_bAllowSound)
	//{
	//	//变量定义
	//	HINSTANCE hInstance = AfxGetInstanceHandle();
	//	bool bPlaying=false;

	//	////获取对象
	//	//CD3DSound * pD3DSound=CD3DSound::GetInstance();
	//	//if(pD3DSound)
	//	//{
	//	//	bPlaying = pD3DSound->PlaySound(hInstance, (pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?T_T("BUGLE"):T_T("BUGLE_EX"), T_T("WAVE"), false);
	//	//}

	//	//系统播放
	//	if(!bPlaying)
	//	{
	//		HRSRC hResour=FindResource(hInstance,(pSendTrumpet->wPropertyIndex==PROPERTY_ID_TRUMPET)?T_T("BUGLE"):T_T("BUGLE_EX"),T_T("WAVE"));
	//		if (hResour!=0)
	//		{
	//			HGLOBAL hGlobal=LoadResource(hInstance,hResour);
	//			if (hGlobal!=0) 
	//			{
	//				//播放声音
	//				const char* pszMemory=(const char*)LockResource(hGlobal);
	//				BOOL bResult = ::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_NODEFAULT);

	//				//清理资源
	//				UnlockResource(hGlobal);
	//			}
	//			FreeResource(hGlobal);
	//		}
	//	}
	//}

	return true;
}

//喜报消息
bool SHZCServerItem::OnSocketSubGladMessage(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	u2string str;

	str.resize(128+1,'\0');
	packet.readUTF16(&str[0], 128);
	std::string sContent = u2_8(&str[0]);

	str.resize(32+1,'\0');
	packet.readUTF16(&str[0], 32);
	std::string sNickName = u2_8(&str[0]);

	str.resize(16+1,'\0');
	packet.readUTF16(&str[0], 16);
	std::string sNum = u2_8(&str[0]);

	dword colText = COL_N2G(packet.read4Byte());
	dword colName = COL_N2G(packet.read4Byte());
	dword colNum  = COL_N2G(packet.read4Byte());
	

	if (mIStringMessageSink)
		mIStringMessageSink->InsertGladString(sContent.c_str(), sNickName.c_str(), sNum.c_str(), colText, colName, colNum);
	return true;
}