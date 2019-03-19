#include "CommandStructDef_SHZ.h"
#include "libs/PacketAide.h"
void CMD_C_LotteryExp_SHZ::toPack(PacketAide& packData)
{
	packData.wword(wType);
	packData.wword(wLotteryIndex);

}

void CMD_C_LotteryExp_SHZ::fromPack(const PacketAide& packData)
{
	wType = packData.rword();
	wLotteryIndex = packData.rword();
}

void CMD_S_LotteryExp_SHZ::toPack(PacketAide& packData)
{
	packData.wword(wType);
	packData.wword(wIndex);
	packData.wSCORE(dwExp);
	packData.wSCORE(lYuanBaoScore);
	packData.wSCORE(lScore);
}
void CMD_S_LotteryExp_SHZ::fromPack(const PacketAide& packData)
{
	wType = packData.rword();
	wIndex = packData.rword();
	dwExp = packData.rSCORE();
	lYuanBaoScore = packData.rSCORE();
	lScore = packData.rSCORE();
}
