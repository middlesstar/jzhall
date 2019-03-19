#ifndef __CommandStructDef_SHZ_H__
#define __CommandStructDef_SHZ_H__


#include "CommandStructDef.h"
//定制类型
struct  CMD_C_LotteryExp_SHZ : public sStructBase
{
	CMD_C_LotteryExp_SHZ()
	{
		memset(this, 0, sizeof(CMD_C_LotteryExp_SHZ));
	}

	word							wType;			//请求类型		0:求情经验分数		1:请求抽奖		
	word							wLotteryIndex;		//哪一级别的抽奖	0:表示普通抽奖			

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

struct CMD_S_LotteryExp_SHZ :public sStructBase
{
	CMD_S_LotteryExp_SHZ()
	{
		memset(this, 0, sizeof(CMD_S_LotteryExp_SHZ));
	}

	word							wType;			//返回类型 与请求类型值相同
	word							wIndex;			//返回抽奖索引  -1为不能抽奖
	SCORE						    dwExp;			//当前经验值
	SCORE						    lYuanBaoScore;		//当前元宝 
	SCORE							lScore;			//当前金币

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

#endif









