#include "CServerItem_SHZ.h"

//ϵͳ��Ϣ
bool SHZCServerItem::OnSocketMainSystem(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//ϵͳ��Ϣ
	case SUB_CM_SYSTEM_MESSAGE_SHZ:	return OnSocketSubSystemMessage(data,dataSize);
		//������Ϣ
	case SUB_CM_ACTION_MESSAGE_SHZ:	return OnSocketSubActionMessage(data,dataSize);
	}

	//�������
	ASSERT(FALSE);

	return true;
}


//ϵͳ��Ϣ
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

	//�ر���Ϸ
	if (wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ|SMT_CLOSE_GAME_SHZ))
	{
		if (SHZIClientKernel::get())
			SHZIClientKernel::get()->Intermit(SHZ_GameExitCode_Shutdown);
	}

	//�ر���Ϸ
	if (wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))
	{
		IntermitConnect(true);
	}

	//��ʾ��Ϣ
	if (wType&SMT_CHAT_SHZ) 
	{
		if (mIChatSink)
			mIChatSink->InsertSystemChat(sString.c_str());
	}

	//������Ϣ
	if (wType&SMT_EJECT_SHZ)
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(sString.c_str(), DLG_MB_OK);
	}

	//�رմ���
	if ((wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))!=0)
	{
		//ʱ���ӳ�
		if (mIServerItemSink)
			mIServerItemSink->OnGFServerClose(sString);
	}
	else if (wType&SMT_CLOSE_GAME_SHZ)
	{//ֻ�ǹر���Ϸʱ�ж��Ƿ����ģʽ
		if (SHZCServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
		{
			//ʱ���ӳ�
			IntermitConnect(true);
			if (mIServerItemSink)
				mIServerItemSink->OnGFServerClose(sString);
		}
	}

	return true;

	////��������
	//CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)data;
	//word wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	////Ч�����
	//ASSERT((dataSize>wHeadSize)&&(dataSize==(wHeadSize+pSystemMessage->wLength*sizeof(tchar))));
	//if ((dataSize<=wHeadSize)||(dataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(tchar)))) return false;

	//word wType = pSystemMessage->wType;

	//int len = tstrlen(pSystemMessage->szString);
	//if (len > 0)
	//{
	//	pSystemMessage->szString[len-1]=T_T('\0');
	//}
	////�ر���Ϸ
	//if (wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ|SMT_CLOSE_GAME_SHZ))
	//{
	//	if (IClientKernel::get())
	//		IClientKernel::get()->Intermit(GameExitCode_Shutdown);
	//}

	////�ر���Ϸ
	//if (wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))
	//{
	//	IntermitConnect(true);
	//}
	//
	////��ʾ��Ϣ
	//if (wType&SMT_CHAT_SHZ) 
	//{
	//	if (mIChatSink)
	//		mIChatSink->InsertSystemChat(pSystemMessage->szString);
	//}

	////������Ϣ
	//if (wType&SMT_EJECT_SHZ)
	//{
	//	if (mIStringMessageSink)
	//		mIStringMessageSink->InsertPromptString(pSystemMessage->szString, DLG_MB_OK);
	//}

	////�رմ���
	//if ((wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))!=0)
	//{
	//	//ʱ���ӳ�
	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGFServerClose(pSystemMessage->szString);
	//}
	//else if (wType&SMT_CLOSE_GAME_SHZ)
	//{//ֻ�ǹر���Ϸʱ�ж��Ƿ����ģʽ
	//	if (CServerRule::IsAllowQuickMode(mServerAttribute.dwServerRule))
	//	{
	//		//ʱ���ӳ�
	//		IntermitConnect(true);
	//		if (mIServerItemSink)
	//			mIServerItemSink->OnGFServerClose(pSystemMessage->szString);
	//	}
	//}

	//return true;
}

//������Ϣ
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

	//�رմ���
	if ((wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))!=0)
	{
		IntermitConnect(true);
	}

	return true;
	////��������
	//CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)data;
	//word wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	////Ч�����
	//ASSERT((dataSize>wHeadSize)&&(dataSize>=(wHeadSize+pActionMessage->wLength*sizeof(tchar))));
	//if ((dataSize<=wHeadSize)||(dataSize<(wHeadSize+pActionMessage->wLength*sizeof(tchar)))) return false;

	////�رմ���
	//if ((pActionMessage->wType&(SMT_CLOSE_ROOM_SHZ|SMT_CLOSE_LINK_SHZ))!=0)
	//{
	//	IntermitConnect(true);

	//	////�ر���ʾ
	//	//m_DlgStatus.HideStatusWindow();
	//}

	////������Ϣ
	//CInformation Information;
	//int nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	////��������
	//word wExcursion=wHeadSize+pActionMessage->wLength*sizeof(tchar);

	////��ȡ����
	//while (wExcursion<dataSize)
	//{
	//	//��������
	//	tagActionHead * pActionHead=(tagActionHead *)((byte *)data+wExcursion);

	//	//Ч�����
	//	ASSERT((wExcursion+sizeof(tagActionHead))<=dataSize);
	//	ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=dataSize);

	//	//Ч�����
	//	if ((wExcursion+sizeof(tagActionHead))>dataSize) return false;
	//	if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>dataSize) return false;

	//	//��������
	//	if (nResultCode==pActionHead->uResponseID)
	//	{
	//		switch (pActionHead->cbActionType)
	//		{
	//		case ACT_BROWSE_SHZ:	//�������
	//			{
	//				//��������
	//				word wDataPos=wExcursion+sizeof(tagActionHead);
	//				tagActionBrowse * pActionBrowse=(tagActionBrowse *)((byte *)data+wDataPos);

	//				//I E ���
	//				if (pActionBrowse->cbBrowseType&BRT_IE_SHZ)
	//				{
	//					ShellExecute(0,T_T("OPEN"),pActionBrowse->szBrowseUrl,0,0,SW_NORMAL);
	//				}

	//				//�������
	//				if (pActionBrowse->cbBrowseType&BRT_PLAZA_SHZ)
	//				{
	//					CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true);
	//				}

	//				break;
	//			}
	//		case ACT_BROWSE_SHZ:	//���ض���
	//			{
	//				//��������
	//				word wDataPos=wExcursion+sizeof(tagActionHead);
	//				tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((byte *)data+wDataPos);

	//				//I E ����
	//				if (pActionDownLoad->cbDownLoadMode&DLT_IE_SHZ)
	//				{
	//					ShellExecute(0,T_T("OPEN"),pActionDownLoad->szDownLoadUrl,0,0,SW_NORMAL);
	//				}

	//				//����ģ��
	//				if (pActionDownLoad->cbDownLoadMode&DLT_MODULE_SHZ)
	//				{
	//					ShellExecute(0,T_T("OPEN"),pActionDownLoad->szDownLoadUrl,0,0,SW_NORMAL);
	//				}

	//				break;
	//			}
	//		}
	//	}

	//	//����ƫ��
	//	wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	//}

	////�رշ���
	//if (pActionMessage->wType&SMT_CLOSE_ROOM_SHZ) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	//return true;
}
