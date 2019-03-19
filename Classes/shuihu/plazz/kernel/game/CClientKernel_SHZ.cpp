#include "shuihu/plazz/kernel/server/CServerItem_SHZ.h"
#include "CClientKernel_SHZ.h"
#include "shuihu/Game/CMD_CatchBird_SHZ.h"
#include "shuihu/gPlatform_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
//////////////////////////////////////////////////////////////////////////
//

#define MSG_SCENE 100
#define MSG_GAME  200


int __gClentKernelRefCount_SHZ = 0;
SHZIClientKernel* __gClientKernel_SHZ = 0;

SHZIClientKernel* SHZIClientKernel::create()
{
	if (__gClentKernelRefCount_SHZ == 0)
	{
		__gClientKernel_SHZ = new SHZCClientKernel();
	}
	__gClentKernelRefCount_SHZ++;

	PLAZZ_PRINTF("IClientKernel::create ref:%d\n", __gClentKernelRefCount_SHZ);
	return __gClientKernel_SHZ;
}

void SHZIClientKernel::destory()
{
	if (__gClentKernelRefCount_SHZ > 0)
	{
		__gClentKernelRefCount_SHZ--;
		PLAZZ_PRINTF("IClientKernel::destory ref:%d\n", __gClentKernelRefCount_SHZ);
		if (__gClentKernelRefCount_SHZ <= 0)
		{
			__gClentKernelRefCount_SHZ = 0;
			delete __gClientKernel_SHZ;
			__gClientKernel_SHZ = 0;
		}
	}

	PLAZZ_PRINTF("IClientKernel::destory ref:%d\n", __gClentKernelRefCount_SHZ);

}

SHZIClientKernel* SHZIClientKernel::get()
{
	return __gClientKernel_SHZ;
}

//////////////////////////////////////////////////////////////////////////
SHZCClientKernel::SHZCClientKernel() 
: mAllowLookon(false)
, mGameStatus(GAME_STATUS_FREE)
, mMeUserItem(0)
, mIClientKernelSink(0)
, mIChatSink(0)
, mIStringMessageSink(0)
{
	memset(&mUserAttribute,   0, sizeof(mUserAttribute));
	memset(&mGameAttribute,   0, sizeof(mGameAttribute));
	memset(&mServerAttribute, 0, sizeof(mServerAttribute));

	mUserManager  = new SHZCGameUserManager();
	mUserManager->SetUserManagerSink(this);
	SHZIServerItem::create();
	PLAZZ_PRINTF("flow->CClientKernel::CClientKernel\n");
	gGlobalUnits_SHZ.InitGlobalUnits();
}

SHZCClientKernel::~SHZCClientKernel()
{
	if (mIClientKernelSink!=0)
		mIClientKernelSink->CloseGameClient();
	mUserManager->SetUserManagerSink(0);
	SafeDelete(mUserManager);
	mUserManager  = 0;
	mIClientKernelSink=0;
	mIChatSink=0;
	mIStringMessageSink=0;

	SHZIServerItem::destory();
	gGlobalUnits_SHZ.UnInitGlobalUnits();

	PLAZZ_PRINTF("flow->CClientKernel::~CClientKernel\n");
}

//////////////////////////////////////////////////////////////////////////
// IClientKernel
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//配置接口

//设置接口
bool SHZCClientKernel::SetClientKernelSink(SHZIClientKernelSink* pIClientKernelSink)
{
	PLAZZ_PRINTF("flow->CClientKernel::SetClientKernelSink\n");
	mIClientKernelSink = pIClientKernelSink;
	return true;
}

//设置接口
bool SHZCClientKernel::SetChatSink(SHZIChatSink* pIChatSink)
{
	PLAZZ_PRINTF("flow->CClientKernel::SetChatSink\n");
	mIChatSink = pIChatSink;
	return true;
}

//设置接口
bool SHZCClientKernel::SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink)
{
	PLAZZ_PRINTF("flow->CClientKernel::SetStringMessageSink\n");
	mIStringMessageSink = pIStringMessageSink;
	return true;
}

//内核配置
bool SHZCClientKernel::Init()
{
	PLAZZ_PRINTF("flow->CClientKernel::Init\n");
	//mSocketEngine->setTCPValidate(true);
	mGameAttribute.wKindID			= DF::shared()->GetGameKindID();
	mGameAttribute.dwClientVersion	= DF::shared()->GetGameVersion();
	mGameAttribute.wChairCount		= DF::shared()->GetGamePlayer();
	
	DF::shared()->GetGameName(mGameAttribute.szGameName);

	if (SHZIServerItem::get())
		SHZIServerItem::get()->OnGFGameReady();
	return true;
}

bool SHZCClientKernel::Intermit(int iExitCode)
{
	PLAZZ_PRINTF("flow->CClientKernel::Intermit\n");

	if (DF::shared()->GetGameType() == 0 &&  SHZIServerItem::get())
		SHZIServerItem::get()->OnGFGameClose(iExitCode);

	SHZIClientKernel::destory();

	
	return true;
}

//////////////////////////////////////////////////////////////////////////
//属性接口

//用户属性
tagUserAttribute * SHZCClientKernel::GetUserAttribute()
{
	return &mUserAttribute;
}

//游戏属性
tagGameAttribute * SHZCClientKernel::GetGameAttribute()
{
	return &mGameAttribute;
}

//房间属性
tagServerAttribute * SHZCClientKernel::GetServerAttribute()
{
	return &mServerAttribute;
}

//////////////////////////////////////////////////////////////////////////
//模式接口


//旁观状态
bool SHZCClientKernel::IsLookonMode()
{
	if (mMeUserItem == 0) return true;
	return mMeUserItem->GetUserStatus() == US_LOOKON;
}

//允许状态
bool SHZCClientKernel::IsAllowLookon()
{
	return mAllowLookon;
}

//////////////////////////////////////////////////////////////////////////
//状态接口

//获取状态
byte SHZCClientKernel::GetGameStatus()
{
	return mGameStatus;
}

//设置状态
void SHZCClientKernel::SetGameStatus(byte cbGameStatus)
{
	mGameStatus = cbGameStatus;
}


//////////////////////////////////////////////////////////////////////////
//用户接口

//自己位置
word SHZCClientKernel::GetMeChairID()
{
	if (mMeUserItem == 0) return INVALID_CHAIR;
	return mMeUserItem->GetChairID();
}

//自己位置
SHZIClientUserItem * SHZCClientKernel::GetMeUserItem()
{
	return mMeUserItem;
}

//游戏用户
SHZIClientUserItem * SHZCClientKernel::GetTableUserItem(word wChairID)
{
	return mUserManager->GetTableUserItem(wChairID);
}

//查找用户
SHZIClientUserItem * SHZCClientKernel::SearchUserByUserID(dword dwUserID)
{
	return mUserManager->SearchUserByUserID(dwUserID);
}

//查找用户
SHZIClientUserItem * SHZCClientKernel::SearchUserByGameID(dword dwGameID)
{
	return mUserManager->SearchUserByGameID(dwGameID);
}

//查找用户
SHZIClientUserItem * SHZCClientKernel::SearchUserByNickName(const char* szNickName)
{
	return mUserManager->SearchUserByNickName(szNickName);
}

//旁观用户
SHZIClientUserItem * SHZCClientKernel::EnumLookonUserItem(word wEnumIndex)
{
	return mUserManager->EnumLookonUserItem(wEnumIndex);
}


//////////////////////////////////////////////////////////////////////////
//网络接口

//发送函数
bool SHZCClientKernel::SendSocketData(word wMainCmdID, word wSubCmdID)
{
	return SendSocketData(wMainCmdID, wSubCmdID, 0, 0);
}

//发送函数
bool SHZCClientKernel::SendSocketData(word wMainCmdID, word wSubCmdID, void * data, word dataSize)
{

	if (0 == DF::shared()->GetGameType()){


		if (!platformIsConnectToInternet_SHZ())
		{
			popup_SHZ(a_u8("网络错误"), a_u8("无法连接到游戏服务器!"));
			return false;
		}

		if (!SHZIServerItem::get())
			return false;

		return SHZIServerItem::get()->GFSendData(wMainCmdID, wSubCmdID, (unsigned char*)data, dataSize);

	}

}

//////////////////////////////////////////////////////////////////////////
//功能接口
//发送进入场景
bool SHZCClientKernel::SendGameOption()
{
	PLAZZ_PRINTF("CClientKernel::SendGameOption ..");


	//发送场景消息
	//变量定义
	PACKET_AIDE_SIZE(512);
	packet.writeByte(gGlobalUnits_SHZ.m_bAllowLookon);
	packet.write4Byte(DF::shared()->GetFrameVersion());
	packet.write4Byte(mGameAttribute.dwClientVersion);

	PLAZZ_PRINTF("CClientKernel::SendGameOption ok");

	//发送数据
	return SendSocketData(MDM_GF_FRAME_SHZ,SUB_GF_GAME_OPTION_SHZ, packet.getBuffer(), packet.getPosition());

}

//发送准备
bool SHZCClientKernel::SendUserReady(void * data, word dataSize)
{
	if (mMeUserItem == 0) return false;
	return SendSocketData(MDM_GF_FRAME_SHZ, SUB_GF_USER_READY_SHZ, 0, 0);
}

//旁观控制
bool SHZCClientKernel::SendUserLookon(dword dwUserID, bool bAllowLookon)
{
	if (!IsLookonMode()) {
		PACKET_AIDE_SIZE(4);
		packet.write4Byte(dwUserID);
		packet.writeByte(bAllowLookon?1:0);
		return SendSocketData(MDM_GF_FRAME_SHZ, SUB_GF_LOOKON_CONFIG_SHZ, packet.getBuffer(), packet.getPosition());

	}
	return true;
}

//发送表情
bool SHZCClientKernel::SendUserExpression(dword dwTargetUserID, word wItemIndex)
{
	//变量定义
	PACKET_AIDE_SIZE(6);
	packet.write2Byte(wItemIndex);
	packet.write4Byte(dwTargetUserID);
	//发送命令
	SendSocketData(MDM_GF_FRAME_SHZ,SUB_GF_USER_EXPRESSION_SHZ, packet.getBuffer(), packet.getPosition());


	return true;
}

//发送聊天
bool SHZCClientKernel::SendUserChatMessage(dword dwTargetUserID, const char* pszChatString, dword crColor)
{
	if (mMeUserItem == 0) return false;

	const unsigned short* ucs2 = u8_2(pszChatString);
	int ucs2len = ucs2_len(ucs2);
	PACKET_AIDE_SIZE(512);
	packet.writeByte(ucs2len);
	packet.write4Byte((crColor<<8)|(crColor>>24&0xff));
	packet.write4Byte(dwTargetUserID);
	packet.writeUTF16(ucs2, ucs2len);
	return SendSocketData(MDM_GF_FRAME_SHZ, SUB_GF_USER_CHAT_SHZ, packet.getBuffer(), packet.getPosition());


}


//////////////////////////////////////////////////////////////////////////
//辅助接口

//激活框架
void SHZCClientKernel::ActiveGameFrame()
{
	PLAZZ_PRINTF("flow->CClientKernel::ActiveGameFrame\n");

}

//切换椅子
word SHZCClientKernel::SwitchViewChairID(word wChairID)
{
	return wChairID;
}

//////////////////////////////////////////////////////////////////////////
// IUserManagerSink
//////////////////////////////////////////////////////////////////////////
void SHZCClientKernel::OnUserItemAcitve(SHZIClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnUserItemAcitve\n");

	if (pIClientUserItem == 0) 
		return;
	if (mMeUserItem == 0 && mUserAttribute.dwUserID == pIClientUserItem->GetUserID()) 
	{
		mMeUserItem = pIClientUserItem;
		if (mIClientKernelSink)
			mIClientKernelSink->OnGFWaitTips(false);
	}

	if (mIClientKernelSink)
		mIClientKernelSink->OnEventUserEnter(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);
}

void SHZCClientKernel::OnUserItemDelete(SHZIClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnUserItemDelete\n");

	if (pIClientUserItem == 0) 
		return;

	if (mIClientKernelSink)
		mIClientKernelSink->OnEventUserLeave(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);
}


//用户更新
void SHZCClientKernel::OnUserFaceUpdate(SHZIClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnUserFaceUpdate\n");



	if (pIClientUserItem == 0) 
		return;

	if (mIClientKernelSink)
		mIClientKernelSink->OnEventCustomFace(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);

	return;
}

void SHZCClientKernel::OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserScore& LastUserScore)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnUserItemUpdate tagUserScore\n");

	if (pIClientUserItem == 0) 
		return;

	if (mIClientKernelSink)
		mIClientKernelSink->OnEventUserScore(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);
}

void SHZCClientKernel::OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserStatus& LastUserStatus)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnUserItemUpdate tagUserStatus\n");

	if (pIClientUserItem == 0) 
		return;

	if (mIClientKernelSink)
		mIClientKernelSink->OnEventUserStatus(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);
}

//用户更新
void SHZCClientKernel::OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserAttrib & UserAttrib)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnUserItemUpdate tagUserAttrib\n");

	if (pIClientUserItem == 0) 
		return;

	if (mIClientKernelSink)
		mIClientKernelSink->OnEventUserAttrib(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);
	return;
}
