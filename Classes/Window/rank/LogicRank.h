#ifndef  _LogicRank_H_
#define  _LogicRank_H_

#include "Window/LogicBase.h"

//���а�����
enum RankMissionType
{
	RANKSTART = 0,		//! [ռλ]���������б�
	DayMatchRank,		//! ��
	WeekMatchRank,		//! ��
	MouthMatchRank,		//! ��
	QuarterMatchRank,	//! ����

	LastDayMatchRank,	//! ����
	LastWeekMatchRank,	//! ����
	LastMouthMatchRank,	//! ����
	LastQuarterMatchRank,//! �ϼ���

	DRANKSCORESTART = 10,	//! [ռλ]������Դ����
	DRechargeRank,		//! ��ֵ����
	DScoreRank,			//! ���
	DMoneyRank,			//! Ԫ��
	DPraiseRank,		//! ��

	RANKEND				//! [ռλ]
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
	void requestRank(int nType);								//�������а�
	void onRankResp(const PacketAide& packet, int nLen);		//�������а�
};

#endif // _LogicRank_H_

