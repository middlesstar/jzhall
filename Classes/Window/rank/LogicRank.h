#ifndef  _LogicRank_H_
#define  _LogicRank_H_

#include "Window/LogicBase.h"

//排行榜类型
enum RankMissionType
{
	RANKSTART = 0,		//! [占位]收益排行列表
	DayMatchRank,		//! 日
	WeekMatchRank,		//! 周
	MouthMatchRank,		//! 月
	QuarterMatchRank,	//! 季度

	LastDayMatchRank,	//! 上日
	LastWeekMatchRank,	//! 上周
	LastMouthMatchRank,	//! 上月
	LastQuarterMatchRank,//! 上季度

	DRANKSCORESTART = 10,	//! [占位]各种资源排行
	DRechargeRank,		//! 充值积分
	DScoreRank,			//! 金币
	DMoneyRank,			//! 元宝
	DPraiseRank,		//! 赞

	RANKEND				//! [占位]
};

class  LogicRank : public LogicBase
{
public:
	LogicRank();
	virtual ~LogicRank();

	SINGLE_INSTANCE_FUNC(LogicRank);

protected:
	void _registerMessageHandler();

public:
	void requestRank(int nType);								//请求排行榜
	void onRankResp(const PacketAide& packet, int nLen);		//返回排行榜
};

#endif // _LogicRank_H_

