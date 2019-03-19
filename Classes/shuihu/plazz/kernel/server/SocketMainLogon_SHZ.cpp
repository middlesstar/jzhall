#include "CServerItem_SHZ.h"

//��¼��Ϣ
bool SHZCServerItem::OnSocketMainLogon(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//��¼�ɹ�
	case SUB_GR_LOGON_SUCCESS_SHZ:	return OnSocketSubLogonSuccess(data,dataSize);
		//��¼ʧ��
	case SUB_GR_LOGON_FAILURE_SHZ:	return OnSocketSubLogonFailure(data,dataSize);
		//��¼���
	case SUB_GR_LOGON_FINISH_SHZ:	return OnSocketSubLogonFinish(data,dataSize);
		//������ʾ
	case SUB_GR_UPDATE_NOTIFY_SHZ:	return OnSocketSubUpdateNotify(data,dataSize);
	}

	return true;
}


//��¼�ɹ�
bool SHZCServerItem::OnSocketSubLogonSuccess(void* data, int dataSize)
{
	mIsQuickSitDown = false;

	PACKET_AIDE_DATA(data);

	//�������
	mUserAttribute.dwUserRight=packet.read4Byte();
	mUserAttribute.dwMasterRight=packet.read4Byte();

	

	if (SHZCUserRight::IsGameCheatUser(mUserAttribute.dwUserRight))
	{

		PLAZZ_PRINTF("%s=%d",a_u8("�û�Ȩ��"),mUserAttribute.dwUserRight) ;
	}else
	{
		PLAZZ_PRINTF("%s=%d",a_u8("�û�û��Ȩ��"),mUserAttribute.dwUserRight) ;
	}
	

	//����״̬
	SetServiceStatus(SHZ_ServiceStatus_RecvInfo);

	if (mIServerItemSink)
		mIServerItemSink->OnGRLogonSuccess();

	return true;

	//mIsQuickSitDown = false;
	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_LogonSuccess));
	//if (dataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	////��Ϣ����
	//CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)data;

	////�������
	//mUserAttribute.dwUserRight=pLogonSuccess->dwUserRight;
	//mUserAttribute.dwMasterRight=pLogonSuccess->dwMasterRight;
	//
	////����״̬
	//SetServiceStatus(ServiceStatus_RecvInfo);

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRLogonSuccess();

	//return true;
}

//��¼ʧ��
bool SHZCServerItem::OnSocketSubLogonFailure(void* data, int dataSize)
{
	//Ч������
	ASSERT(dataSize>=4);
	if (dataSize<4) return false;

	PACKET_AIDE_DATA(data);
	int lErrorCode = packet.read4Byte();
	int len=(dataSize-4)/2;
	u2string str;
	str.resize(len+1,'\0');
	packet.readUTF16(&str[0], len);
	std::string sString = u2_8(&str[0]);

	PLAZZ_PRINTF("%s", a_u8("OnSocketSubLogonFailure ��¼ʧ��:%s\n"), sString.c_str());
	//�ر�����
	IntermitConnect(true);

	if (mIServerItemSink)
		mIServerItemSink->OnGRLogonFailure(lErrorCode, sString);

	if (mIServerItemSink)
		mIServerItemSink->OnGFServerClose(sString);
	return true;

	////Ч�����
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

//��¼���
bool SHZCServerItem::OnSocketSubLogonFinish(void* data, int dataSize)
{
	//����״̬
	SetServiceStatus(SHZ_ServiceStatus_ServiceIng);
		
	mUserAttribute.dwUserID=mMeUserItem->GetUserID();
	mUserAttribute.wChairID=INVALID_CHAIR;
	mUserAttribute.wTableID=INVALID_TABLE;

	//�����ж�
	if (SHZCServerRule::IsForfendGameRule(mServerAttribute.dwServerRule)==false)
	{
		//���͹���
		SendUserRulePacket();
	}

	if (mIServerItemSink)
		mIServerItemSink->OnGRLogonFinish();
	return true;
}


//������ʾ
bool SHZCServerItem::OnSocketSubUpdateNotify(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	//������־
	byte cbMustUpdatePlaza = packet.readByte();
	byte cbMustUpdateClient = packet.readByte();
	byte cbAdviceUpdateClient = packet.readByte();

	//��ǰ�汾
	dword dwCurrentPlazaVersion = packet.read4Byte();
	dword dwCurrentFrameVersion = packet.read4Byte();
	dword dwCurrentClientVersion = packet.read4Byte();

	//��������
	if (cbMustUpdatePlaza==TRUE)
	{
		IntermitConnect(true);

		if (mIServerItemSink)
			mIServerItemSink->OnGRUpdateNotify(1, a_u8("��Ϸ�㳡�Ѹ���"));
		return true;
	}

	//��Ϸ����
	if (cbMustUpdateClient==TRUE)
	{
		IntermitConnect(true);

		if (mIServerItemSink)
			mIServerItemSink->OnGRUpdateNotify(1, a_u8("��Ϸ�Ѹ���"));
		return true;
	}

	//������ʾ
	if (cbAdviceUpdateClient!=TRUE)
		return true;

	//������ʾ
	char szDescribe[512]={0};
	sprintf(szDescribe,a_u8("��%s���Ѿ������ˣ�����ǿ�ҽ��������и��£����ڽ��и�����"),mGameKind.szKindName);

	IntermitConnect(true);

	if (mIServerItemSink)
		mIServerItemSink->OnGRUpdateNotify(0, szDescribe);

	return true;

	////Ч�����
	//ASSERT(dataSize==sizeof(CMD_GR_UpdateNotify));
	//if (dataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	////��������
	//CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)data;

	////��������
	//if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	//{
	//	IntermitConnect(true);

	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGRUpdateNotify(1, T_T("��Ϸ�㳡�Ѹ���"));
	//	return true;
	//}

	////��Ϸ����
	//if (pUpdateNotify->cbMustUpdateClient==TRUE)
	//{
	//	IntermitConnect(true);

	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGRUpdateNotify(1, T_T("��Ϸ�Ѹ���"));
	//	return true;
	//}

	////������ʾ
	//if (pUpdateNotify->cbAdviceUpdateClient!=TRUE)
	//	return true;

	////������ʾ
	//char szDescribe[512]=T_T("");
	//_sntprintf(szDescribe,countarray(szDescribe),T_T("��%s���Ѿ������ˣ�����ǿ�ҽ��������и��£����ڽ��и�����"),mGameKind.szKindName);

	//if (mIServerItemSink && mIServerItemSink->OnGRUpdateNotify(0, szDescribe))
	//	IntermitConnect(true);
	//return true;
}

