#include "CClientKernel_SHZ.h"

SHZIGameFrameSink* SHZIGameFrameSink::getInterface()
{
	return (SHZIGameFrameSink*)SHZIClientKernel::get();
}

//房间配置
void SHZCClientKernel::OnGFConfigServer(const tagUserAttribute& UserAttribute, const tagServerAttribute& ServerAttribute)
{
	//PLAZZ_PRINTF("flow->CClientKernel::OnGFConfigServer\n");

	memcpy(&mUserAttribute, &UserAttribute, sizeof(mUserAttribute));
	memcpy(&mServerAttribute, &ServerAttribute, sizeof(mServerAttribute));
}

//配置完成
void SHZCClientKernel::OnGFConfigFinish()
{
	//PLAZZ_PRINTF("flow->CClientKernel::OnGFConfigFinish\n");

	if (mIClientKernelSink && !mIClientKernelSink->SetupGameClient())
		return;
	SendGameOption();
}

bool SHZCClientKernel::OnGFEventSocket(int main, int sub, void* data, int dataSize)
{
	//PLAZZ_PRINTF("flow->CClientKernel::OnGFEventSocket\n");
	
	//游戏消息
	if (main==MDM_GF_GAME_SHZ)
	{
		//效验状态
		ASSERT(mIClientKernelSink!=0);
		if (mIClientKernelSink==0) 
			return false;
		return mIClientKernelSink->OnEventGameMessage(sub,data,dataSize);
	}

	//游戏消息
	if (main==MDM_GR_INSURE_SHZ)
	{
		PLAZZ_PRINTF("flow->CClientKernel::OnGFEventSocket MDM_GR_INSURE_SHZ\n");
		//if(mIClientKernelSink)
		//	mIClientKernelSink->OnEventInsureMessage(sub,data,dataSize);

		return true;
	}

	//内核处理
	if (main==MDM_GF_FRAME_SHZ)
	{
		switch (sub)
		{
		case SUB_GF_USER_CHAT_SHZ:			//用户聊天
			{
				return OnSocketSubUserChat(data,dataSize);
			}
		case SUB_GF_USER_EXPRESSION_SHZ:	//用户表情
			{
				return OnSocketSubExpression(data,dataSize);
			}
		case SUB_GF_GAME_STATUS_SHZ:		//游戏状态
			{
				return OnSocketSubGameStatus(data,dataSize);
			}
		case SUB_GF_GAME_SCENE_SHZ:			//游戏场景
			{
				return OnSocketSubGameScene(data,dataSize);
			}
		case SUB_GF_LOOKON_STATUS_SHZ:		//旁观状态
			{
				return OnSocketSubLookonStatus(data,dataSize);
			}
		case SUB_GF_SYSTEM_MESSAGE_SHZ:		//系统消息
			{
				return OnSocketSubSystemMessage(data,dataSize);
			}
		case SUB_GF_ACTION_MESSAGE_SHZ:		//动作消息
			{
				return OnSocketSubActionMessage(data,dataSize);
			}
		case SUB_GF_USER_READY_SHZ:			//用户准备
			{
				if(mMeUserItem ==0 || mMeUserItem->GetUserStatus()>=US_READY)
					return true;
				SendUserReady(0,0);
				if (mIClientKernelSink)
					mIClientKernelSink->OnGFMatchWaitTips(0);
				return true;
			}
		case SUB_GR_USER_WAIT_DISTRIBUTE_SHZ:   //等待提示
			{
				if (mIClientKernelSink)
					mIClientKernelSink->OnGFWaitTips(true);
				return true;
			}
		case SUB_GR_MATCH_INFO_SHZ:				//比赛信息
			{
				if (!mIClientKernelSink)
					return true;

				PACKET_AIDE_DATA(data);
					
				tagMatchInfo MatchInfo;
				u2string sTitle;

				for (int i = 0; i < 4; ++i)
				{
					sTitle = packet.readUTF16(64);
					strncpy(MatchInfo.szTitle[i], u2_8(&sTitle[0]), countarray(MatchInfo.szTitle[i]));
				}
				
				MatchInfo.wGameCount = packet.read2Byte();
				mIClientKernelSink->OnGFMatchInfo(&MatchInfo);

				////参数效验
				//ASSERT(dataSize==sizeof(CMD_GR_Match_Info));
				//if(dataSize!=sizeof(CMD_GR_Match_Info)) return false;

				////变量定义
				//CMD_GR_Match_Info *pMatchInfo=(CMD_GR_Match_Info*)data;
				//if (mIClientKernelSink)
				//	mIClientKernelSink->OnGFMatchInfo(pMatchInfo);
				return true;
			}
		case SUB_GR_MATCH_WAIT_TIP_SHZ:			//等待提示
			{
				if (!mIClientKernelSink)
					return true;

				//设置参数
				if(dataSize!=0)
				{
					PACKET_AIDE_DATA(data);
					tagMatchWaitTip MatchWaitTip;
					MatchWaitTip.lScore = packet.read8Byte();
					MatchWaitTip.wRank = packet.read2Byte();
					MatchWaitTip.wCurTableRank = packet.read2Byte();
					MatchWaitTip.wUserCount = packet.read2Byte();
					MatchWaitTip.wPlayingTable = packet.read2Byte();
					u2string str;
					str.resize(LEN_SERVER+1,'\0');
					packet.readUTF16(&str[0], LEN_SERVER);
					strncpy(MatchWaitTip.szMatchName, u2_8(&str[0]), countarray(MatchWaitTip.szMatchName));
					mIClientKernelSink->OnGFMatchWaitTips(&MatchWaitTip);
				}
				else
				{
					mIClientKernelSink->OnGFMatchWaitTips(0);
				}

				////效验参数
				//ASSERT(dataSize==sizeof(CMD_GR_Match_Wait_Tip) || dataSize==0);
				//if(dataSize!=sizeof(CMD_GR_Match_Wait_Tip) && dataSize!=0) return false;

				//if (mIClientKernelSink)
				//{
				//	//设置参数
				//	if(dataSize!=0)
				//		mIClientKernelSink->OnGFMatchWaitTips((CMD_GR_Match_Wait_Tip*)data);
				//	else
				//		mIClientKernelSink->OnGFMatchWaitTips(0);
				//}
				return true;
			}
		case SUB_GR_MATCH_RESULT_SHZ:			//比赛结果
			{
				//设置参数
				if (!mIClientKernelSink)
					return true;

				PACKET_AIDE_DATA(data);

				tagMatchResult MatchResult;
				u2string str;
				str.resize(256+1, '\0');
				packet.readUTF16(&str[0], 256);
				strncpy(MatchResult.szDescribe, u2_8(&str[0]), countarray(MatchResult.szDescribe));
				
				MatchResult.dwGold = packet.read4Byte();
				MatchResult.dwMedal = packet.read4Byte();
				MatchResult.dwExperience = packet.read4Byte();

				mIClientKernelSink->OnGFMatchWaitTips(0);
				mIClientKernelSink->OnGFMatchResult(&MatchResult);
				
				////效验参数
				//ASSERT(dataSize==sizeof(CMD_GR_MatchResult));
				//if(dataSize!=sizeof(CMD_GR_MatchResult)) return false;

				////设置参数
				//if (mIClientKernelSink)
				//{
				//	mIClientKernelSink->OnGFMatchWaitTips(0);
				//	mIClientKernelSink->OnGFMatchResult((CMD_GR_MatchResult*)data);
				//}

				return true;
			}
		}

		return true;
	}

	return false;
}

//用户聊天
bool SHZCClientKernel::OnSocketSubUserChat(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubUserChat\n");

	int baseSize=2+4+4+4;
	ASSERT(dataSize>=baseSize);
	if(dataSize<baseSize) return false;

	//显示消息
	if (mIChatSink==0) return true;

	//变量定义
	PACKET_AIDE_DATA(data);
	word wChatLength = packet.read2Byte();
	dword dwChatColor = packet.read4Byte();
	dword dwSendUserID = packet.read4Byte();
	dword dwTargetUserID = packet.read4Byte();

	int len = (dataSize-baseSize)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0],len);	
	
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

		//好友模式
		if ((gGlobalUnits_SHZ.m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
			return true;

		//厌恶模式
		if ((gGlobalUnits_SHZ.m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
			return true;
	}

	//插入消息
	if (dwTargetUserID!=0L)
	{
		//获取用户
		SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);

		if (pIRecvUserItem!=0)
		{
			//插入消息
			mIChatSink->InsertUserChat(
				pISendUserItem->GetNickName(), pIRecvUserItem->GetNickName(),
				u2_8(&str[0]), dwChatColor);
		}
	}
	else 
	{
		//插入消息
		mIChatSink->InsertUserChat(pISendUserItem->GetNickName(),u2_8(&str[0]), dwChatColor);
	}

	return true;

	////变量定义
	//CMD_GF_S_UserChat * pUserChat=(CMD_GF_S_UserChat *)data;

	////效验参数
	//ASSERT(dataSize>=(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)));
	//ASSERT(dataSize==(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	////效验参数
	//if (dataSize<(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	//if (dataSize!=(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	////显示消息
	//if (mIChatSink==0) return true;

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
	//		
	//	//好友模式
	//	if ((gGlobalUnits.m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//厌恶模式
	//	if ((gGlobalUnits.m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}
	//}

	////插入消息
	//if (pUserChat->dwTargetUserID!=0L)
	//{
	//	//获取用户
	//	IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pUserChat->dwTargetUserID);

	//	if (pIRecvUserItem!=0)
	//	{
	//		//插入消息
	//		const tchar* pszSendUser=pISendUserItem->GetNickName();
	//		const tchar* pszRecvUser=pIRecvUserItem->GetNickName();
	//		dword dwColor = ((pUserChat->dwChatColor & 0xFF) << 16) | (pUserChat->dwChatColor >> 8);
	//		mIChatSink->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,dwColor);
	//	}
	//}
	//else 
	//{
	//	//插入消息
	//	const tchar* pszSendUser=pISendUserItem->GetNickName();
	//	dword dwColor = ((pUserChat->dwChatColor & 0xFF) << 16) | (pUserChat->dwChatColor >> 8);
	//	mIChatSink->InsertUserChat(pszSendUser,pUserChat->szChatString,dwColor);
	//}

	//return true;
}


//用户表情
bool SHZCClientKernel::OnSocketSubExpression(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubExpression\n");

	//效验参数
	ASSERT(dataSize==9);
	if (dataSize!=9) return false;

	//显示消息
	if (mIChatSink==0) return true;

	PACKET_AIDE_DATA(data);
	word wItemIndex=packet.read2Byte();
	dword dwSendUserID=packet.read4Byte();
	dword dwTargetUserID=packet.read4Byte();

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

		//好友模式
		if ((gGlobalUnits_SHZ.m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((gGlobalUnits_SHZ.m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//插入消息
	if (dwTargetUserID!=0L)
	{
		//获取用户
		SHZIClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(dwTargetUserID);

		//插入消息
		if (pIRecvUserItem!=0)
		{
			mIChatSink->InsertExpression(pISendUserItem->GetNickName(), pIRecvUserItem->GetNickName(),wItemIndex);
		}
	}
	else
	{
		//插入消息
		mIChatSink->InsertExpression(pISendUserItem->GetNickName(),wItemIndex);
	}

	return true;

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GF_S_UserExpression));
	//if (dataSize!=sizeof(CMD_GF_S_UserExpression)) return false;

	////显示消息
	//if (mIChatSink==0) return true;

	////变量定义
	//CMD_GF_S_UserExpression * pUserExpression=(CMD_GF_S_UserExpression *)data;

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
	//	
	//	//好友模式
	//	if ((gGlobalUnits.m_cbMessageMode==MESSAGE_MODE_FRIEND_SHZ)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}

	//	//厌恶模式
	//	if ((gGlobalUnits.m_cbMessageMode==MESSAGE_MODE_DETEST_SHZ)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
	//	{
	//		return true;
	//	}
	//}

	////插入消息
	//if (pUserExpression->dwTargetUserID!=0L)
	//{
	//	//获取用户
	//	IClientUserItem * pIRecvUserItem=mUserManager->SearchUserByUserID(pUserExpression->dwTargetUserID);

	//	//插入消息
	//	if (pIRecvUserItem!=0)
	//	{
	//		const tchar* pszSendUser=pISendUserItem->GetNickName();
	//		const tchar* pszRecvUser=pIRecvUserItem->GetNickName();
	//		mIChatSink->InsertExpression(pszSendUser,pszRecvUser,pUserExpression->wItemIndex);
	//	}
	//}
	//else
	//{
	//	//插入消息
	//	const tchar* pszSendUser=pISendUserItem->GetNickName();
	//	mIChatSink->InsertExpression(pszSendUser,pUserExpression->wItemIndex);
	//}

	//return true;
}

//游戏状态
bool SHZCClientKernel::OnSocketSubGameStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubGameStatus\n");

	PACKET_AIDE_DATA(data);

	//设置变量
	mGameStatus=packet.readByte();
	mAllowLookon=packet.readByte()!=0;

	return true;

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GF_GameStatus));
	//if (dataSize!=sizeof(CMD_GF_GameStatus)) return false;

	////消息处理
	//CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)data;

	////设置变量
	//mGameStatus=pGameStatus->cbGameStatus;
	//mAllowLookon=pGameStatus->cbAllowLookon?true:false;

	//return true;
}

//游戏场景
bool SHZCClientKernel::OnSocketSubGameScene(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubGameScene\n");

	//效验状态
	ASSERT(mMeUserItem!=0);
	if (mMeUserItem==0) return true;

	if (mIClientKernelSink==0)
		return true;
	//场景处理
	bool bLookonUser=(mMeUserItem->GetUserStatus()==US_LOOKON);
	return mIClientKernelSink->OnEventSceneMessage(mGameStatus,bLookonUser,data,dataSize);
}

//旁观状态
bool SHZCClientKernel::OnSocketSubLookonStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubLookonStatus\n");

	//消息处理
	if (IsLookonMode()==true)
	{
		byte cbAllowLookon = ((unsigned char*)data)[0];
		
		//事件处理
		mAllowLookon=cbAllowLookon!=0;

		//事件通知
		ASSERT(mMeUserItem!=0);

		if (mIClientKernelSink)
			mIClientKernelSink->OnEventLookonMode(data,dataSize);

		//提示消息
		if (mAllowLookon==true)
		{
			if (mIStringMessageSink!=0)
				mIStringMessageSink->InsertNormalString(a_u8("您被允许观看玩家游戏"));
		}
		else
		{
			if (mIStringMessageSink!=0)
				mIStringMessageSink->InsertNormalString(a_u8("您被禁止观看玩家游戏"));
		}
	}

	return true;

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GF_LookonStatus));
	//if (dataSize!=sizeof(CMD_GF_LookonStatus)) return false;

	////消息处理
	//if (IsLookonMode()==true)
	//{
	//	//变量定义
	//	CMD_GF_LookonStatus * pLookonStatus=(CMD_GF_LookonStatus *)data;

	//	//事件处理
	//	mAllowLookon=(pLookonStatus->cbAllowLookon==TRUE)?true:false;

	//	//事件通知
	//	ASSERT(mMeUserItem!=0);

	//	if (mIClientKernelSink)
	//		mIClientKernelSink->OnEventLookonMode(data,dataSize);

	//	//提示消息
	//	if (mAllowLookon==true)
	//	{
	//		if (mIStringMessageSink!=0)
	//			mIStringMessageSink->InsertNormalString(T_T("您被允许观看玩家游戏"));
	//	}
	//	else
	//	{
	//		if (mIStringMessageSink!=0)
	//			mIStringMessageSink->InsertNormalString(T_T("您被禁止观看玩家游戏"));
	//	}
	//}

	//return true;
}

//系统消息
bool SHZCClientKernel::OnSocketSubSystemMessage(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubSystemMessage\n");

	PACKET_AIDE_DATA(data);
	word wType = packet.read2Byte();
	word wLength = packet.read2Byte();

	int len=(dataSize-4)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0],len);

	const std::string sString = u2_8(&str[0]);

	//滚动消息
	if((wType&SMT_TABLE_ROLL_SHZ)!=0)
	{
		//tstring str = pSystemMessage->szString;
		//if(str.find(T_T("系统配桌"))>=0)
		//	mIClientKernelSink->OnGFWaitTips(true);

		if (mIClientKernelSink)
			mIClientKernelSink->OnGFTableMessage(sString.c_str());

		if((wType|~SMT_TABLE_ROLL_SHZ)==0)
			return true;
	}

	//关闭处理
	if ((wType&SMT_CLOSE_GAME_SHZ)!=0)
	{
		//中断连接
		Intermit(SHZ_GameExitCode_Normal);
	}

	//显示消息
	if (wType&SMT_CHAT_SHZ)
	{
		if (mIStringMessageSink!=0)
			mIStringMessageSink->InsertSystemString(sString.c_str());
	}

	//弹出消息
	if (wType&SMT_EJECT_SHZ)
	{
		if (mIStringMessageSink!=0)
			mIStringMessageSink->InsertPromptString(sString.c_str(), DLG_MB_OK);
	}

	return true;

	////变量定义
	//CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)data;
	//word wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	////效验参数
	//ASSERT((dataSize>wHeadSize)&&(dataSize==(wHeadSize+pSystemMessage->wLength*sizeof(tchar))));
	//if ((dataSize<=wHeadSize)||(dataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(tchar)))) return false;

	////滚动消息
	//if((pSystemMessage->wType&SMT_TABLE_ROLL_SHZ)!=0)
	//{
	//	tstring str = pSystemMessage->szString;
	//	if(str.find(T_T("系统配桌"))>=0)
	//		mIClientKernelSink->OnGFWaitTips(true);

	//	if (mIClientKernelSink)
	//		mIClientKernelSink->OnGFTableMessage(pSystemMessage->szString);

	//	if((pSystemMessage->wType|~SMT_TABLE_ROLL_SHZ)==0)
	//		return true;
	//}
	//	
	////关闭处理
	//if ((pSystemMessage->wType&SMT_CLOSE_GAME_SHZ)!=0)
	//{
	//	//中断连接
	//	Intermit(0);
	//}

	////显示消息
	//if (pSystemMessage->wType&SMT_CHAT_SHZ)
	//{
	//	if (mIStringMessageSink!=0)
	//		mIStringMessageSink->InsertSystemString(pSystemMessage->szString);
	//}

	////弹出消息
	//if (pSystemMessage->wType&SMT_EJECT_SHZ)
	//{
	//	if (mIStringMessageSink!=0)
	//		mIStringMessageSink->InsertPromptString(pSystemMessage->szString, DLG_MB_OK);
	//}

	////关闭房间
	//if (pSystemMessage->wType&SMT_CLOSE_GAME_SHZ)
	//{
	//	Intermit(GameExitCode_Normal);
	//}

	//return true;
}

//动作消息
bool SHZCClientKernel::OnSocketSubActionMessage(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubActionMessage\n");

	PACKET_AIDE_DATA(data);
	word wType = packet.read2Byte();
	word wLength = packet.read2Byte();
	uint nButtonType = packet.read2Byte();

	u2string str;
	str.resize(wLength+1,'\0');
	packet.readUTF16(&str[0],wLength);

	const std::string sString = u2_8(&str[0]);

	//关闭处理
	if ((wType&SMT_CLOSE_GAME_SHZ)!=0)
	{
		//中断连接
		Intermit(SHZ_GameExitCode_Normal);
	}

	return true;
	////变量定义
	//CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)data;
	//word wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	////效验参数
	//ASSERT((dataSize>wHeadSize)&&(dataSize>=(wHeadSize+pActionMessage->wLength*sizeof(tchar))));
	//if ((dataSize<=wHeadSize)||(dataSize<(wHeadSize+pActionMessage->wLength*sizeof(tchar)))) return false;

	////关闭处理
	//if ((pActionMessage->wType&SMT_CLOSE_GAME_SHZ)!=0)
	//{
	//	//中断连接
	//	IntermitConnect();
	//}

	////弹出消息
	//int nResultCode=ShowInformation(pActionMessage->szString,pActionMessage->nButtonType,0);

	////变量定义
	//word wExcursion=wHeadSize+pActionMessage->wLength*sizeof(tchar);

	////提取动作
	//while (wExcursion<dataSize)
	//{
	//	//变量定义
	//	tagActionHead * pActionHead=(tagActionHead *)((byte *)data+wExcursion);

	//	//效验参数
	//	ASSERT((wExcursion+sizeof(tagActionHead))<=dataSize);
	//	ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=dataSize);

	//	//效验参数
	//	if ((wExcursion+sizeof(tagActionHead))>dataSize) return false;
	//	if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>dataSize) return false;

	//	//动作出来
	//	if (nResultCode==pActionHead->uResponseID)
	//	{
	//		switch (pActionHead->cbActionType)
	//		{
	//		case ACT_BROWSE_SHZ:	//浏览动作
	//			{
	//				//变量定义
	//				word wDataPos=wExcursion+sizeof(tagActionHead);
	//				tagActionBrowse * pActionBrowse=(tagActionBrowse *)((byte *)data+wDataPos);

	//				//I E 浏览
	//				if (pActionBrowse->cbBrowseType&BRT_IE_SHZ)
	//				{
	//					ShellExecute(0,T_T("open"),pActionBrowse->szBrowseUrl,0,0,SW_NORMAL);
	//				}

	//				//大厅浏览
	//				if (pActionBrowse->cbBrowseType&BRT_PLAZA_SHZ)
	//				{
	//					//						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true,true);
	//				}

	//				break;
	//			}
	//		case ACT_BROWSE_SHZ:	//下载动作
	//			{
	//				//变量定义
	//				word wDataPos=wExcursion+sizeof(tagActionHead);
	//				tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((byte *)data+wDataPos);

	//				//I E 下载
	//				if (pActionDownLoad->cbDownLoadMode&DLT_IE_SHZ)
	//				{
	//					ShellExecute(0,T_T("open"),pActionDownLoad->szDownLoadUrl,0,0,SW_NORMAL);
	//				}

	//				//下载模块
	//				if (pActionDownLoad->cbDownLoadMode&DLT_MODULE_SHZ)
	//				{
	//					ShellExecute(0,T_T("open"),pActionDownLoad->szDownLoadUrl,0,0,SW_NORMAL);
	//				}

	//				break;
	//			}
	//		}
	//	}

	//	//增加偏移
	//	wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	//}

	////关闭房间
	//if (pActionMessage->wType&SMT_CLOSE_GAME_SHZ) m_pIClientKernelSink->CloseGameClient();

	//return true;
}