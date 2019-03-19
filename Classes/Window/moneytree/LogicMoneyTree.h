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
	void requestMoneyTree(int nType);								//����ҡǮ��0-������Ϣ��1-�����ȡ
	void onMoneyTreeResp(const PacketAide& packet, int nLen);		//ҡǮ������

	vector<int>& getCoinVection() { return m_vecCoin; };
protected:
	CC_SYNTHESIZE(CMD_GP_YaoQianShu_Get, m_objMoneyTreeData, MoneyTreeData);		//ҡǮ������
	CC_SYNTHESIZE(unsigned int, m_uMoneyGetTime, MoneyGetTime);						//�´�ҡǮ������ҡ��ʱ��

	vector<int> m_vecCoin;
};

#endif // _LogicMoneyTree_H_

