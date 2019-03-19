#ifndef SHZ_LotteryMission_H_
#define SHZ_LotteryMission_H_

#include "cocos2d.h"
#include "Network/CommandStructDef_SHZ.h"
struct  lotteryInfo
{
	SCORE							lYuanBaoScore;						//用户元宝
	SCORE							lShExpLottery;						//水浒经验抽奖
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
	//请求抽奖数据
	void reqLottery(int type, int degree);
	//接收抽奖数据
	void recLottery(void * pData);
protected:
	CC_SYNTHESIZE(CMD_S_LotteryExp_SHZ, m_objLotteryExp, LotteryExp);
	CC_SYNTHESIZE(lotteryInfo, m_objLotteryInfo, LotteryInfo);
};
#endif