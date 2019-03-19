#ifndef SHZ_LotteryMission_H_
#define SHZ_LotteryMission_H_

#include "cocos2d.h"
#include "Network/CommandStructDef_SHZ.h"
struct  lotteryInfo
{
	SCORE							lYuanBaoScore;						//�û�Ԫ��
	SCORE							lShExpLottery;						//ˮ䰾���齱
};
class LotteryMission 
{
private:
	LotteryMission();
	static LotteryMission *m_Instance;

public:
	~LotteryMission();
	static LotteryMission* getInstance();

public:
	//����齱����
	void reqLottery(int type, int degree);
	//���ճ齱����
	void recLottery(void * pData);
protected:
	CC_SYNTHESIZE(CMD_S_LotteryExp_SHZ, m_objLotteryExp, LotteryExp);
	CC_SYNTHESIZE(lotteryInfo, m_objLotteryInfo, LotteryInfo);
};
#endif