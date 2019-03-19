#include "CGPQuanPanMission_SHZ.h"
#include "df/LocalStore.h"

#define MISSION_QUANPAN_NULL 0
#define MISSION_QUANPAN_GET	 1
#define MISSION_QUANPAN_SET  2

SHZCGPQuanPanMission::SHZCGPQuanPanMission(const char* url, int port)
: CSocketMission(url, port)
{
	mMissionType = MISSION_QUANPAN_NULL;
	mICGPQuanPanMissionSink = 0;
}

// 设置回调接口
void SHZCGPQuanPanMission::setMissionSink(SHZICGPQuanPanMissionSink *pICGPQuanPanMissionSink)
{
	this->mICGPQuanPanMissionSink = pICGPQuanPanMissionSink;
}

void SHZCGPQuanPanMission::getQuanPanStatue()
{
	mMissionType = MISSION_QUANPAN_GET;
	start();
}

void SHZCGPQuanPanMission::setQuanPanStatue(uint8 statue)
{
	if (DF::shared()->GetGameType() == 0)
	{
		mMissionType = MISSION_QUANPAN_SET;
		start();
	}
		

}

///////////////////////////////////////////////////////////////////////////
// ISocketEngineSink
void SHZCGPQuanPanMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPQuanPanMission::onEventTCPSocketLink\n");

	//变量定义
	SHZCGlobalUserInfo * pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	switch (mMissionType)
	{
	case MISSION_QUANPAN_GET:
		{
			//变量定义
			PACKET_AIDE_SIZE(64);
			packet.write4Byte(pGlobalUserData->dwUserID);
			//发送数据
			send(MDM_GP_USER_SERVICE_SHZ, SUB_GP_QUANPAN_GET_SHZ, packet.getBuffer(), packet.getPosition());
		}
		break;
	case MISSION_QUANPAN_SET:
		{
			//变量定义
			PACKET_AIDE_SIZE(64);
			packet.write4Byte(pGlobalUserData->dwUserID);
			//packet.write4Byte(0);
			//发送数据
			send(MDM_GP_USER_SERVICE_SHZ, SUB_GP_QUANPAN_SET_SHZ, packet.getBuffer(), packet.getPosition());
		}
		break;
	default:
		break;
	}
}

void SHZCGPQuanPanMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPQuanPanMission::onEventTCPSocketShut");
}

void SHZCGPQuanPanMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPQuanPanMission::onEventTCPSocketError");
}

bool SHZCGPQuanPanMission::onEventTCPSocketRead(int main, int sub, void* data, int size)
{
	PLAZZ_PRINTF("CGPQuanPanMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE_SHZ)
	{
		return false;
	}

	switch (sub)
	{
	case SUB_GP_QUANPAN_GET_RESULT_SHZ:
		return onSubQuanPanStatue(data, size);
	default:
		break;
	}
	return false;
}

bool SHZCGPQuanPanMission::onSubQuanPanStatue(void *data, int size)
{
	if (0 == data || 0 == size)
	{
		stop();
		if (mICGPQuanPanMissionSink)
			mICGPQuanPanMissionSink->onGPQuanPanStatue(0);

		return false;
	}

	if (0 == mICGPQuanPanMissionSink)
		return true;

	PACKET_AIDE_DATA(data);

	uint8 statue = packet.readByte();
	mICGPQuanPanMissionSink->onGPQuanPanStatue(statue);
}