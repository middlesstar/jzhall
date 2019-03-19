#include "LotteryMission_SHZ.h"
#include "shuihu/StaticData_SHZ.h"
#include "shuihu/GameHeader_SHZ.h"
#include "Network/CommandDef.h"
#include "shuihu/plazz/kernel/game/CClientKernel_SHZ.h"
LotteryMission* LotteryMission::m_Instance = nullptr;

LotteryMission::LotteryMission()
{
	
}
LotteryMission* LotteryMission::getInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new LotteryMission();
	return m_Instance;
}

LotteryMission::~LotteryMission()
{
	if (m_Instance != nullptr)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
		
}
void LotteryMission::reqLottery(int type, int degree)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	CMD_C_LotteryExp_SHZ lotterySend;
	lotterySend.wType = type;
	lotterySend.wLotteryIndex = degree;

	PACKET_AIDE_SIZE(12);
	lotterySend.toPack(packet);
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_LOTTERYEXP_SHZ, packet.getBuffer(), packet.getPosition());
}

void LotteryMission::recLottery(void * pData)
{
	PACKET_AIDE_DATA(pData);
	m_objLotteryExp.fromPack(packet);
	printf("---------");
}
