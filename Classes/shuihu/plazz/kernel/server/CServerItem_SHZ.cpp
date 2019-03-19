#include "CServerItem_SHZ.h"
//////////////////////////////////////////////////////////////////////////
//
int __gServerItemRefCount_SHZ = 0;
SHZIServerItem* __gServerItem_SHZ = 0;

SHZIServerItem* SHZIServerItem::create()
{
	if (__gServerItemRefCount_SHZ == 0)
	{
		__gServerItem_SHZ = new SHZCServerItem();
	}   

	__gServerItemRefCount_SHZ++;
	PLAZZ_PRINTF("IServerItem::create ref:%d\n", __gServerItemRefCount_SHZ);

	return __gServerItem_SHZ;
}

void SHZIServerItem::destory()
{
	if (__gServerItemRefCount_SHZ > 0)
	{
		__gServerItemRefCount_SHZ--;
		if (__gServerItemRefCount_SHZ <= 0)
		{
			__gServerItemRefCount_SHZ = 0;
			delete __gServerItem_SHZ;
			__gServerItem_SHZ = 0;
		}
	}
	PLAZZ_PRINTF("IServerItem::destory ref:%d\n", __gServerItemRefCount_SHZ);

}

SHZIServerItem* SHZIServerItem::get()
{
	return __gServerItem_SHZ;
}

//////////////////////////////////////////////////////////////////////////
SHZCServerItem::SHZCServerItem() 
	: mReqTableID(INVALID_TABLE)
	, mReqChairID(INVALID_CHAIR)
	, mFindTableID(INVALID_TABLE)
	, mIsGameReady(false)
	, mMeUserItem(0)
	, mIServerItemSink(0)
	, mIChatSink(0)
	, mIStringMessageSink(0)
	, mSocketEngine(0)
	, mServiceStatus(SHZ_ServiceStatus_Unknow)
{
	mIsQuickSitDown=false;
	memset(&mUserAttribute,   0, sizeof(mUserAttribute));
	memset(&mServerAttribute, 0, sizeof(mServerAttribute));

	mSocketEngine = ISocketEngine::create();
	mUserManager  = new SHZCPlazaUserManager();
	mUserManager->SetUserManagerSink(this);
	mSocketEngine->setSocketEngineSink(this);
}

SHZCServerItem::~SHZCServerItem()
{
	IntermitConnect(true);
	mIServerItemSink=0;
	mUserManager->SetUserManagerSink(0);
	mSocketEngine->setSocketEngineSink(0);
	ISocketEngine::destory(mSocketEngine);
	mSocketEngine = 0;
	SafeDelete(mUserManager);
	mUserManager  = 0;
}

//////////////////////////////////////////////////////////////////////////
//配置接口

//设置接口
bool SHZCServerItem::SetServerItemSink(SHZIServerItemSink* pIServerItemSink)
{
	mIServerItemSink = pIServerItemSink;
	return true;
}

//设置接口
bool SHZCServerItem::SetChatSink(SHZIChatSink* pIChatSink)
{
	PLAZZ_PRINTF("CServerItem::SetChatSink\n");
	mIChatSink = pIChatSink;
	return true;
}

//设置接口
bool SHZCServerItem::SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink)
{
	mIStringMessageSink = pIStringMessageSink;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//属性接口
//用户属性
const tagUserAttribute& SHZCServerItem::GetUserAttribute() const
{
	return mUserAttribute;
}

//房间属性
const tagServerAttribute& SHZCServerItem::GetServerAttribute() const 
{
	return mServerAttribute;
}

//服务状态
SHZ_enServiceStatus SHZCServerItem::GetServiceStatus()
{
	return mServiceStatus;
}

//是否服务状态
bool SHZCServerItem::IsService()
{
	return GetServiceStatus() == SHZ_ServiceStatus_ServiceIng;
}

//自己状态
bool SHZCServerItem::IsPlayingMySelf()
{
	return ((mMeUserItem!=0)&&(mMeUserItem->GetUserStatus()==US_PLAYING));
}

//设置状态
void SHZCServerItem::SetServiceStatus(SHZ_enServiceStatus ServiceStatus)
{
	//设置变量
	mServiceStatus=ServiceStatus;
	return;
}

//配置房间
bool SHZCServerItem::SetServerAttribute(HallGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)
{
	//房间属性
	SHZCGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;

	memcpy(&mGameKind,&pGameKindItem->m_GameKind,sizeof(mGameKind));
	memcpy(&mGameServer,&pGameServerItem->m_GameServer,sizeof(mGameServer));

	mServerAttribute.wKindID		= mGameServer.wKindID;
	mServerAttribute.wServerID		= mGameServer.wServerID;
	//mServerAttribute.wServerType	= mGameServer.wServerType;
	//mServerAttribute.dwServerRule	= mGameServer.dwServerRule;
	mServerAttribute.wAVServerPort	= wAVServerPort;
	mServerAttribute.dwAVServerAddr	= dwAVServerAddr;
	strncpy(mServerAttribute.szServerName, mGameServer.szServerName, LEN_SERVER);

	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//加载配置
	mParameterGame=pParameterGlobal->GetParameterGame(&mGameKind);
	mParameterServer=pParameterGlobal->GetParameterServer(&mGameServer);

	return true;
}

//////////////////////////////////////////////////////////////////////////
//连接接口

//配置房间
bool SHZCServerItem::ConnectServer(HallGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)
{
	if (mServiceStatus != SHZ_ServiceStatus_Unknow &&
		mServiceStatus != SHZ_ServiceStatus_NetworkDown)
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("很抱歉, 此游戏房间还未关闭,请先关闭!!"),DLG_MB_OK);
		return false;
	}

	//房间属性
	SetServerAttribute(pGameServerItem, wAVServerPort, dwAVServerAddr);

	//关闭判断
	ASSERT(mServerAttribute.wServerID!=0);
	if (mServerAttribute.wServerID==0)
	{
		SetServiceStatus(SHZ_ServiceStatus_Unknow);
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("很抱歉，此游戏房间已经关闭了，不允许继续进入！"),DLG_MB_OK);
		return false;
	}

	//创建组件
	cocos2d::log("-------------ConnectServer[ip = %s,port = %d]-------------", mGameServer.szServerAddr, mGameServer.wServerPort);

	if (!mSocketEngine->connect(mGameServer.szServerAddr, mGameServer.wServerPort))
	{
		cocos2d::log("-------------connect error-------------");
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(a_u8("游戏房间连接失败，请检查系统网络配置参数是否正确"),DLG_MB_OK);
		return false;
	}

	//设置状态
	SetServiceStatus(SHZ_ServiceStatus_Entering);


	return true;
}

//中断连接
bool SHZCServerItem::IntermitConnect(bool force)
{	
	if (mServiceStatus == SHZ_ServiceStatus_Unknow ||
		mServiceStatus == SHZ_ServiceStatus_NetworkDown)
		return false;

	if (IsService())
	{
		if(SHZIClientKernel::get())
		{
			SHZIClientKernel::get()->Intermit(0);
		}
	}

	//设置状态
	SetServiceStatus(SHZ_ServiceStatus_NetworkDown);

	mSocketEngine->disconnect();
	mReqTableID		= INVALID_TABLE;
	mReqChairID		= INVALID_CHAIR;
	mFindTableID	= INVALID_TABLE;
	mIsGameReady	= false;
	mMeUserItem		= 0;
	mUserManager->ResetUserItem();
	return true;
}

//////////////////////////////////////////////////////////////////////////
//网络接口

//发送函数
bool SHZCServerItem::SendSocketData(word wMainCmdID, word wSubCmdID)
{
	return SendSocketData(wMainCmdID, wSubCmdID, 0, 0);
}

//发送函数
bool SHZCServerItem::SendSocketData(word wMainCmdID, word wSubCmdID, void * data, word dataSize)
{
	return mSocketEngine->send(wMainCmdID, wSubCmdID, (unsigned char*)data, dataSize);
}



//发送登录
bool SHZCServerItem::SendLogonPacket()
{
	//变量定义
	SHZCGlobalUserInfo * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	
	//变量定义
	PACKET_AIDE_SIZE(512);
	packet.write4Byte(DF::shared()->GetPlazaVersion());
	packet.write4Byte(DF::shared()->GetFrameVersion());
	packet.write4Byte(DF::shared()->GetGameVersion());
	packet.write4Byte(pGlobalUserData->dwUserID);
	packet.writeUTF16(u8_2(pGlobalUserData->szPassword), LEN_PASSWORD);
	packet.writeUTF16(u8_2(DF::shared()->GetMachineID()), LEN_MACHINE_ID);
	packet.write2Byte(mServerAttribute.wKindID);

	//发送数据
	SendSocketData(MDM_GR_LOGON_SHZ,SUB_GR_LOGON_USERID_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//发送配置
bool SHZCServerItem::SendUserRulePacket()
{	
	//变量定义
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();
	//变量定义
	byte cbRuleMask=0x00;

	//限制地址
	if (pParameterGlobal->m_bLimitSameIP==true)
		cbRuleMask|=UR_LIMIT_SAME_IP_SHZ;

	//限制胜率
	if (mParameterGame->m_bLimitWinRate==true)
		cbRuleMask|=UR_LIMIT_WIN_RATE_SHZ;
	

	//限制逃率
	if (mParameterGame->m_bLimitFleeRate==true)
		cbRuleMask|=UR_LIMIT_FLEE_RATE_SHZ;
	
	//限制积分
	if (mParameterGame->m_bLimitGameScore==true)
		cbRuleMask|=UR_LIMIT_GAME_SCORE_SHZ;

	PACKET_AIDE_SIZE(SIZE_PACK_DATA);
	packet.writeByte(cbRuleMask);
	packet.write2Byte(mParameterGame->m_wMinWinRate);
	packet.write2Byte(mParameterGame->m_wMaxFleeRate);
	packet.write4Byte(mParameterGame->m_lMaxGameScore);
	packet.write4Byte(mParameterGame->m_lMinGameScore);

	//携带密码
	if ((SHZCServerRule::IsForfendLockTable(mServerAttribute.dwServerRule)==false)&&(mParameterServer->m_bTakePassword==true))
	{
		const unsigned short* ucs2 = u8_2(mParameterServer->m_szPassword);
		int ucs2len = ucs2_len(ucs2);
		packet.write2Byte(ucs2len);
		packet.write2Byte(DTP_GR_TABLE_PASSWORD_SHZ);
		packet.writeUTF16(ucs2, ucs2len);
	}

	//发送数据
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_RULE_SHZ,packet.getBuffer(),packet.getPosition());
	return true;
}

//发送旁观
bool SHZCServerItem::SendLookonPacket(word wTableID, word wChairID)
{
	PACKET_AIDE_SIZE(4);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);

	//发送数据
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_LOOKON_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//发送坐下
bool SHZCServerItem::SendSitDownPacket(word wTableID, word wChairID, const char* lpszPassword)
{
	const unsigned short* ucs2 = u8_2(lpszPassword);

	//变量定义
	PACKET_AIDE_SIZE(128);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);
	packet.writeUTF16(ucs2, LEN_PASSWORD);
		
	//发送数据
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_SITDOWN_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//发送起立
bool SHZCServerItem::SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave)
{
	PACKET_AIDE_SIZE(10);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);
	packet.writeByte(cbForceLeave);

	//发送数据
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_STANDUP_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//发送聊天
bool SHZCServerItem::SendUserChatPacket(dword dwTargetUserID, const char* pszChatString, dword dwColor)
{
	const unsigned short* ucs2 = u8_2(pszChatString);
	int ucs2len = ucs2_len(ucs2);

	//构造信息
	PACKET_AIDE_SIZE(512);
	packet.write2Byte(ucs2len);
	packet.write4Byte((dwColor&0xFFFF<<8)|(dwColor&0xFF0000>>16));
	packet.write4Byte(dwTargetUserID);
	packet.writeUTF16(ucs2, ucs2len);

	//发送命令
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_CHAT_SHZ, packet.getBuffer(), packet.getPosition());

	return true;
}

//发送表情
bool SHZCServerItem::SendExpressionPacket(dword dwTargetUserID, word wItemIndex)
{
	PACKET_AIDE_SIZE(6);
	packet.write2Byte(wItemIndex);
	packet.write4Byte(dwTargetUserID);

	//发送命令
	SendSocketData(MDM_GR_USER_SHZ,SUB_GR_USER_EXPRESSION_SHZ,packet.getBuffer(),packet.getPosition());

	return true;
}

//////////////////////////////////////////////////////////////////////////
//内部函数

//聊天效验
bool SHZCServerItem::EfficacyUserChat(const char* pszChatString, word wExpressionIndex)
{
	//变量定义
	ASSERT(mMeUserItem!=0);
	byte cbMemberOrder=mMeUserItem->GetMemberOrder();
	byte cbMasterOrder=mMeUserItem->GetMasterOrder();

	//长度比较
	if(pszChatString != 0)
	{
		if(utf8_len(pszChatString) >= (LEN_USER_CHAT/2))
		{
			if (mIChatSink)
				mIChatSink->InsertSystemChat(a_u8("抱歉，您输入的聊天内容过长，请缩短后再发送！"));
			return false;
		}
	}

	//房间配置
	if (SHZCServerRule::IsForfendRoomChat(mServerAttribute.dwServerRule)&&(cbMasterOrder==0))
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			char szChatString[256]={0};
			sprintf(szChatString, a_u8("\n“%s”发送失败"), pszChatString);

			if (mIChatSink)
				mIChatSink->InsertSystemChat(szChatString);
		}

		if (mIChatSink)
			mIChatSink->InsertSystemChat(a_u8("抱歉，当前此游戏房间禁止用户大厅聊天！"));

		return false;
	}

	//权限判断
	if (SHZCUserRight::CanRoomChat(mUserAttribute.dwUserRight)==false)
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			char szChatString[256]={0};
			sprintf(szChatString, a_u8("\n“%s”发送失败"), pszChatString);

			if (mIChatSink)
				mIChatSink->InsertSystemChat(szChatString);
		}

		//插入消息
		if (mIChatSink)
			mIChatSink->InsertSystemChat(a_u8("抱歉，您没有大厅发言的权限，若需要帮助，请联系游戏客服咨询！"));

		return false;
	}

	//速度判断
	static dword dwChatTime=0;
	dword dwCurrentTime=(dword)time(0);
	if ((cbMasterOrder==0)&&((dwCurrentTime-dwChatTime)<=TIME_USER_CHAT))
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			char szChatString[256]={0};
			sprintf(szChatString, a_u8("\n“%s”发送失败"), pszChatString);

			if (mIChatSink)
				mIChatSink->InsertSystemChat(szChatString);
		}

		//插入消息
		if (mIChatSink)
			mIChatSink->InsertSystemChat(a_u8("您的说话速度太快了，请坐下来喝杯茶休息下吧！"));

		return false;
	}

	//设置变量
	dwChatTime=dwCurrentTime;
	return true;
}

//桌子效验
bool SHZCServerItem::EfficacyTableRule(word wTableID, word wChairID, bool bReqLookon, char strDescribe[256])
{
	//效验参数
	ASSERT(wTableID<mTableViewFrame.GetTableCount());
	ASSERT(wChairID<mTableViewFrame.GetChairCount());

	//状态过滤
	if (mServiceStatus != SHZ_ServiceStatus_ServiceIng) return false;
	if (wTableID>=mTableViewFrame.GetTableCount()) return false;
	if (wChairID>=mTableViewFrame.GetChairCount()) return false;

	//变量定义
	ASSERT(SHZCParameterGlobal::shared()!=0);
	SHZCParameterGlobal * pParameterGlobal=SHZCParameterGlobal::shared();

	//变量定义
	SHZITableView * pITableView=mTableViewFrame.GetTableViewItem(wTableID);
	SHZIClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//变量定义
	bool bGameStart=pITableView->GetPlayFlag();
	bool bDynamicJoin=SHZCServerRule::IsAllowDynamicJoin(mServerAttribute.dwServerRule);

	//游戏状态
	if ((bGameStart==true)&&(bDynamicJoin==false)&&(bReqLookon==false))
	{
		sprintf(strDescribe, "%s", a_u8("此游戏桌已经开始游戏了，暂时不能加入！"));
		return false;
	}

	//其他判断
	if ((bReqLookon==false)&&(mMeUserItem->GetMasterOrder()==0))
	{
		//规则判断
		for (word i=0;i<mTableViewFrame.GetChairCount();i++)
		{
			//获取用户
			SHZIClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==0)||(pITableUserItem==mMeUserItem)) continue;

			//厌恶玩家
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pITableUserItem->GetUserCompanion()==CP_DETEST)
				{
					//设置提示
					sprintf(strDescribe, a_u8("您设置了不与不受欢迎的玩家游戏，此桌有您不欢迎的玩家 [ %s ] ！"), pITableUserItem->GetNickName());
					return false;
				}
			}

			//胜率效验
			if (mParameterGame->m_bLimitWinRate==true)
			{
				if (((word)(pITableUserItem->GetUserWinRate()*1000L))<mParameterGame->m_wMinWinRate)
				{
					sprintf(strDescribe, a_u8("[ %s ] 的胜率太低了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//逃率效验
			if (mParameterGame->m_bLimitFleeRate)
			{
				if (((word)(pITableUserItem->GetUserFleeRate()*1000L))<mParameterGame->m_wMaxFleeRate)
				{
					sprintf(strDescribe, a_u8("[ %s ] 的逃率太高了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//积分效验
			if (mParameterGame->m_bLimitGameScore)
			{
				//最高积分
				if (pITableUserItem->GetUserScore()>mParameterGame->m_lMaxGameScore)
				{
					sprintf(strDescribe, a_u8("[ %s ] 的积分太高了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}

				//最低积分
				if (pITableUserItem->GetUserScore()<mParameterGame->m_lMinGameScore)
				{
					sprintf(strDescribe, a_u8("[ %s ] 的积分太低了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}

	return true;
}