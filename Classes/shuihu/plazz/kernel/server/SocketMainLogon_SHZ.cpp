#include "CServerItem_SHZ.h"

//登录消息
bool SHZCServerItem::OnSocketMainLogon(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//登录成功
	case SUB_GR_LOGON_SUCCESS_SHZ:	return OnSocketSubLogonSuccess(data,dataSize);
		//登录失败
	case SUB_GR_LOGON_FAILURE_SHZ:	return OnSocketSubLogonFailure(data,dataSize);
		//登录完成
	case SUB_GR_LOGON_FINISH_SHZ:	return OnSocketSubLogonFinish(data,dataSize);
		//更新提示
	case SUB_GR_UPDATE_NOTIFY_SHZ:	return OnSocketSubUpdateNotify(data,dataSize);
	}

	return true;
}


//登录成功
bool SHZCServerItem::OnSocketSubLogonSuccess(void* data, int dataSize)
{
	mIsQuickSitDown = false;

	PACKET_AIDE_DATA(data);

	//玩家属性
	mUserAttribute.dwUserRight=packet.read4Byte();
	mUserAttribute.dwMasterRight=packet.read4Byte();

	

	if (SHZCUserRight::IsGameCheatUser(mUserAttribute.dwUserRight))
	{

		PLAZZ_PRINTF("%s=%d",a_u8("用户权限"),mUserAttribute.dwUserRight) ;
	}else
	{
		PLAZZ_PRINTF("%s=%d",a_u8("用户没有权限"),mUserAttribute.dwUserRight) ;
	}
	

	//设置状态
	SetServiceStatus(SHZ_ServiceStatus_RecvInfo);

	if (mIServerItemSink)
		mIServerItemSink->OnGRLogonSuccess();

	return true;

	//mIsQuickSitDown = false;
	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_LogonSuccess));
	//if (dataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	////消息处理
	//CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)data;

	////玩家属性
	//mUserAttribute.dwUserRight=pLogonSuccess->dwUserRight;
	//mUserAttribute.dwMasterRight=pLogonSuccess->dwMasterRight;
	//
	////设置状态
	//SetServiceStatus(ServiceStatus_RecvInfo);

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRLogonSuccess();

	//return true;
}

//登录失败
bool SHZCServerItem::OnSocketSubLogonFailure(void* data, int dataSize)
{
	//效验数据
	ASSERT(dataSize>=4);
	if (dataSize<4) return false;

	PACKET_AIDE_DATA(data);
	int lErrorCode = packet.read4Byte();
	int len=(dataSize-4)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0], len);
	std::string sString = u2_8(&str[0]);

	PLAZZ_PRINTF("%s", a_u8("OnSocketSubLogonFailure 登录失败:%s\n"), sString.c_str());
	//关闭连接
	IntermitConnect(true);

	if (mIServerItemSink)
		mIServerItemSink->OnGRLogonFailure(lErrorCode, sString);

	if (mIServerItemSink)
		mIServerItemSink->OnGFServerClose(sString);
	return true;

	////效验参数
	//CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)data;
	//ASSERT(dataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	//if (dataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//IntermitConnect(true);

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRLogonFailure(pLogonFailure->lErrorCode, pLogonFailure->szDescribeString);

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGFServerClose(pLogonFailure->szDescribeString);
	//return true;
}

//登录完成
bool SHZCServerItem::OnSocketSubLogonFinish(void* data, int dataSize)
{
	//设置状态
	SetServiceStatus(SHZ_ServiceStatus_ServiceIng);
		
	mUserAttribute.dwUserID=mMeUserItem->GetUserID();
	mUserAttribute.wChairID=INVALID_CHAIR;
	mUserAttribute.wTableID=INVALID_TABLE;

	//规则判断
	if (SHZCServerRule::IsForfendGameRule(mServerAttribute.dwServerRule)==false)
	{
		//发送规则
		SendUserRulePacket();
	}

	if (mIServerItemSink)
		mIServerItemSink->OnGRLogonFinish();
	return true;
}


//更新提示
bool SHZCServerItem::OnSocketSubUpdateNotify(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	//升级标志
	byte cbMustUpdatePlaza = packet.readByte();
	byte cbMustUpdateClient = packet.readByte();
	byte cbAdviceUpdateClient = packet.readByte();

	//当前版本
	dword dwCurrentPlazaVersion = packet.read4Byte();
	dword dwCurrentFrameVersion = packet.read4Byte();
	dword dwCurrentClientVersion = packet.read4Byte();

	//大厅更新
	if (cbMustUpdatePlaza==TRUE)
	{
		IntermitConnect(true);

		if (mIServerItemSink)
			mIServerItemSink->OnGRUpdateNotify(1, a_u8("游戏广场已更新"));
		return true;
	}

	//游戏更新
	if (cbMustUpdateClient==TRUE)
	{
		IntermitConnect(true);

		if (mIServerItemSink)
			mIServerItemSink->OnGRUpdateNotify(1, a_u8("游戏已更新"));
		return true;
	}

	//构造提示
	if (cbAdviceUpdateClient!=TRUE)
		return true;

	//构造提示
	char szDescribe[512]={0};
	sprintf(szDescribe,a_u8("“%s”已经更新了，我们强烈建议您进行更新，现在进行更新吗？"),mGameKind.szKindName);

	IntermitConnect(true);

	if (mIServerItemSink)
		mIServerItemSink->OnGRUpdateNotify(0, szDescribe);

	return true;

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_UpdateNotify));
	//if (dataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	////变量定义
	//CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)data;

	////大厅更新
	//if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	//{
	//	IntermitConnect(true);

	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGRUpdateNotify(1, T_T("游戏广场已更新"));
	//	return true;
	//}

	////游戏更新
	//if (pUpdateNotify->cbMustUpdateClient==TRUE)
	//{
	//	IntermitConnect(true);

	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGRUpdateNotify(1, T_T("游戏已更新"));
	//	return true;
	//}

	////构造提示
	//if (pUpdateNotify->cbAdviceUpdateClient!=TRUE)
	//	return true;

	////构造提示
	//char szDescribe[512]=T_T("");
	//_sntprintf(szDescribe,countarray(szDescribe),T_T("“%s”已经更新了，我们强烈建议您进行更新，现在进行更新吗？"),mGameKind.szKindName);

	//if (mIServerItemSink && mIServerItemSink->OnGRUpdateNotify(0, szDescribe))
	//	IntermitConnect(true);
	//return true;
}

