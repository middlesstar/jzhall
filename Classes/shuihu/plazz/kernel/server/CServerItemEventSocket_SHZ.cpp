#include "CServerItem_SHZ.h"

//////////////////////////////////////////////////////////////////////////
// ISocketEngineSink
//////////////////////////////////////////////////////////////////////////
void SHZCServerItem::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CServerItem::onEventTCPSocketLink\n");

	SendLogonPacket();
}

void SHZCServerItem::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CServerItem::onEventTCPSocketShut\n");

	if (mServiceStatus != SHZ_ServiceStatus_NetworkDown)
	{
		IntermitConnect(true);
		if (mIServerItemSink)
			mIServerItemSink->OnGFServerClose(a_u8("�����ж�"));
	}
}

void SHZCServerItem::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CServerItem::onEventTCPSocketError err:%d\n", errorCode);

	switch (errorCode)
	{
	case 10054:
		{
			if (mIStringMessageSink)
				mIStringMessageSink->InsertSystemString(a_u8("�����쳣,���Ͽ�����!!!"));
		}
		break;
	}

}

bool SHZCServerItem::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	switch (main)
	{
		//��¼��Ϣ
	case MDM_GR_LOGON_SHZ:	return OnSocketMainLogon(sub,data,dataSize);
		//������Ϣ
	case MDM_GR_CONFIG_SHZ:	return OnSocketMainConfig(sub,data,dataSize);
		//�û���Ϣ
	case MDM_GR_USER_SHZ:	return OnSocketMainUser(sub,data,dataSize);
		//״̬��Ϣ
	case MDM_GR_STATUS_SHZ:	return OnSocketMainStatus(sub,data,dataSize);
		//������Ϣ
	case MDM_GR_INSURE_SHZ:	return OnSocketMainInsure(sub,data,dataSize);
		//������Ϣ
	case MDM_GR_MANAGE_SHZ:	return OnSocketMainManager(sub,data,dataSize);
		//������Ϣ
	case MDM_CM_SYSTEM_SHZ: return OnSocketMainSystem(sub,data,dataSize);
		//��Ϸ��Ϣ
	case MDM_GF_GAME_SHZ:
		//�����Ϣ
	case MDM_GF_FRAME_SHZ:	return OnSocketMainGameFrame(main,sub,data,dataSize);
		//������Ϣ
	case MDM_GR_MATCH_SHZ:	return OnSocketMainMatch(sub,data,dataSize);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
