#include "CGPGameNoticeMission_SHZ.h"


SHZCGPGameNoticeMission::SHZCGPGameNoticeMission(const char* url, int port)
: CSocketMission(url, port)
{
	mIGPGameNoticeMissionSink = 0;
}

// ���ûص��ӿ�
void SHZCGPGameNoticeMission::setMissionSink(SHZIGPGameNoticeMissionSink* pIGPGameNoticeMissionSink)
{
	mIGPGameNoticeMissionSink = pIGPGameNoticeMissionSink;
}

void SHZCGPGameNoticeMission::StartGameNotice()
{
	start();
}

void SHZCGPGameNoticeMission::onEventTCPSocketLink()
{
	//��������
	send(MDM_GR_LOGON_SHZ,SUB_GP_LINE_GAMENOTICE_SHZ, 0, 0);
}

void SHZCGPGameNoticeMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPGameNoticeMission::onEventTCPSocketShut\n");
}

void SHZCGPGameNoticeMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPGameNoticeMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool SHZCGPGameNoticeMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPGameNoticeMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_REMOTE_SERVICE_SHZ)
	{
		return false;
	}

	switch (sub)
	{
		
	case SUB_GP_GAME_NOTICE_SHZ:	return onSubGameNoticeResult(data, size);
	}

	return false;
}

// �һ����
bool SHZCGPGameNoticeMission::onSubGameNoticeResult(void* data, int size)
{
	//�ر�����
	//stop();
	
	PACKET_AIDE_DATA(data);

	u2string sMsg;
	sMsg.resize(size+1, '\0');
	packet.readUTF16(&sMsg[0], size/2);
	std::string str=u2_8(&sMsg[0]);
	//��ʾ��Ϣ
	if (mIGPGameNoticeMissionSink)
		mIGPGameNoticeMissionSink->onGPGameNoticeResult(str.c_str());
	return true;
}
