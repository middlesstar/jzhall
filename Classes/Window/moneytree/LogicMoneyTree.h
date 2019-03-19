#ifndef  _LogicMoneyTree_H_
#define  _LogicMoneyTree_H_

#include "Window/LogicBase.h"

enum eMoneyTreeType
{
	EMTT_GET_DATA	=	0,
	EMTT_REQUEST_MONEY
};

class  LogicMoneyTree : public LogicBase
{
public:
	LogicMoneyTree();
	virtual ~LogicMoneyTree();

	SINGLE_INSTANCE_FUNC(LogicMoneyTree);

protected:
	void _registerMessageHandler();

public:
	void requestMoneyTree(int nType);								//请求摇钱树0-请求信息，1-请求获取
	void onMoneyTreeResp(const PacketAide& packet, int nLen);		//摇钱树返回

	vector<int>& getCoinVection() { return m_vecCoin; };
protected:
	CC_SYNTHESIZE(CMD_GP_YaoQianShu_Get, m_objMoneyTreeData, MoneyTreeData);		//摇钱树数据
	CC_SYNTHESIZE(unsigned int, m_uMoneyGetTime, MoneyGetTime);						//下次摇钱树可以摇的时间

	vector<int> m_vecCoin;
};

#endif // _LogicMoneyTree_H_

