#include "CServerItem_SHZ.h"

//系统消息
bool SHZCServerItem::OnSocketMainSystem(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//系统消息
	case SUB_CM_SYSTEM_MESSAGE_SHZ:	return OnSocketSubSystemMessage(data,dataSize);
		//动作消息
	case SUB_CM_ACTION_MESSAGE_SHZ:	return OnSocketSubActionMessage(data,dataSize);
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}


//系统消息
bool SHZCServerItem::OnSocketSubSystemMessage(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	word wType = packet.read2Byte();
	word wLength = packet.read2Byte();

	int len=(dataSize-4)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0],len);

	const std::string sString = u2_8(&str[0]);

	PLAZZ_PRINTF("CServerItem::OnSocketSubSystemMessage %s\n", sString.c_str());

	//关闭游戏
	if (wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ|SMT_CLOSE_GAME_SHZ))
	{
		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->Intermit(SHZ_GameExitCode_Shutdown);
	}

	//关闭游戏
	if (wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))
	{
		IntermitConnect(true);
	}

	//显示消息
	if (wType&SMT_CHAT_SHZ) 
	{
		if (mIChatSink)
			mIChatSink->InsertSystemChat(sString.c_str());
	}

	//弹出消息
	if (wType&SMT_EJECT_SHZ)
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(sString.c_str(), DLG_MB_OK);
	}

	//关闭处理
	if ((wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))!=0)
	{
		//时间延迟
		if (mIServerItemSink)
			mIServerItemSink->OnGFServerClose(sString);
	}
	else if (wType&SMT_CLOSE_GAME_SHZ)
	{//只是关闭游戏时判断是否快速模式
		if (SHZCServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
		{
			//时间延迟
			IntermitConnect(true);
			if (mIServerItemSink)
				mIServerItemSink->OnGFServerClose(sString);
		}
	}

	return true;

	////变量定义
	//CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)data;
	//word wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	////效验参数
	//ASSERT((dataSize>wHeadSize)&&(dataSize==(wHeadSize+pSystemMessage->wLength*sizeof(tchar))));
	//if ((dataSize<=wHeadSize)||(dataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(tchar)))) return false;

	//word wType = pSystemMessage->wType;

	//int len = tstrlen(pSystemMessage->szString);
	//if (len > 0)
	//{
	//	pSystemMessage->szString[len-1]=T_T('\0');
	//}
	////关闭游戏
	//if (wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ|SMT_CLOSE_GAME_SHZ))
	//{
	//	if (IClientKernel::get())
	//		IClientKernel::get()->Intermit(GameExitCode_Shutdown);
	//}

	////关闭游戏
	//if (wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))
	//{
	//	IntermitConnect(true);
	//}
	//
	////显示消息
	//if (wType&SMT_CHAT_SHZ) 
	//{
	//	if (mIChatSink)
	//		mIChatSink->InsertSystemChat(pSystemMessage->szString);
	//}

	////弹出消息
	//if (wType&SMT_EJECT_SHZ)
	//{
	//	if (mIStringMessageSink)
	//		mIStringMessageSink->InsertPromptString(pSystemMessage->szString, DLG_MB_OK);
	//}

	////关闭处理
	//if ((wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))!=0)
	//{
	//	//时间延迟
	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGFServerClose(pSystemMessage->szString);
	//}
	//else if (wType&SMT_CLOSE_GAME_SHZ)
	//{//只是关闭游戏时判断是否快速模式
	//	if (CServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
	//	{
	//		//时间延迟
	//		IntermitConnect(true);
	//		if (mIServerItemSink)
	//			mIServerItemSink->OnGFServerClose(pSystemMessage->szString);
	//	}
	//}

	//return true;
}

//动作消息
bool SHZCServerItem::OnSocketSubActionMessage(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	word wType = packet.read2Byte();
	word wLength = packet.read2Byte();
	uint nButtonType = packet.read2Byte();

	u2string str;
	str.resize(wLength+1,'\0');
	packet.readUTF16(&str[0],wLength);

	const std::string sString = u2_8(&str[0]);

	//关闭处理
	if ((wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))!=0)
	{
		IntermitConnect(true);
	}

	return true;
	////变量定义
	//CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)data;
	//word wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	////效验参数
	//ASSERT((dataSize>wHeadSize)&&(dataSize>=(wHeadSize+pActionMessage->wLength*sizeof(tchar))));
	//if ((dataSize<=wHeadSize)||(dataSize<(wHeadSize+pActionMessage->wLength*sizeof(tchar)))) return false;

	////关闭处理
	//if ((pActionMessage->wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))!=0)
	//{
	//	IntermitConnect(true);

	//	////关闭提示
	//	//m_DlgStatus.HideStatusWindow();
	//}

	////弹出消息
	//CInformation Information;
	//int nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

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
	//					ShellExecute(0,T_T("OPEN"),pActionBrowse->szBrowseUrl,0,0,SW_NORMAL);
	//				}

	//				//大厅浏览
	//				if (pActionBrowse->cbBrowseType&BRT_PLAZA_SHZ)
	//				{
	//					CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true);
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
	//					ShellExecute(0,T_T("OPEN"),pActionDownLoad->szDownLoadUrl,0,0,SW_NORMAL);
	//				}

	//				//下载模块
	//				if (pActionDownLoad->cbDownLoadMode&DLT_MODULE_SHZ)
	//				{
	//					ShellExecute(0,T_T("OPEN"),pActionDownLoad->szDownLoadUrl,0,0,SW_NORMAL);
	//				}

	//				break;
	//			}
	//		}
	//	}

	//	//增加偏移
	//	wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	//}

	////关闭房间
	//if (pActionMessage->wType&SMT_CLOSE_ROOM_SHZ) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	//return true;
}
