#ifndef __CommandStructDef_SHZ_H__
#define __CommandStructDef_SHZ_H__


#include "CommandStructDef.h"
//��������
struct  CMD_C_LotteryExp_SHZ : public sStructBase
{
	CMD_C_LotteryExp_SHZ()
	{
		memset(this, 0, sizeof(CMD_C_LotteryExp_SHZ));
	}

	word							wType;			//��������		0:���龭�����		1:����齱		
	word							wLotteryIndex;		//��һ����ĳ齱	0:��ʾ��ͨ�齱			

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

struct CMD_S_LotteryExp_SHZ :public sStructBase
{
	CMD_S_LotteryExp_SHZ()
	{
		memset(this, 0, sizeof(CMD_S_LotteryExp_SHZ));
	}

	word							wType;			//�������� ����������ֵ��ͬ
	word							wIndex;			//���س齱����  -1Ϊ���ܳ齱
	SCORE						    dwExp;			//��ǰ����ֵ
	SCORE						    lYuanBaoScore;		//��ǰԪ�� 
	SCORE							lScore;			//��ǰ���

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

#endif









