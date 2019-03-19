#ifndef  _LogicExchange_H_
#define  _LogicExchange_H_

#include "Window/LogicBase.h"

class  LogicExchange : public LogicBase
{
public:
	LogicExchange();
	virtual ~LogicExchange();

	SINGLE_INSTANCE_FUNC(LogicExchange);

protected:
	void _registerMessageHandler();

public:
	void requestExchange(int nGoodsIndex,longlong lTel = 0 ,longlong lQQ = 0,const string& strName = "",const string& strAddress = "");	//����һ�
	void onExchangeResp(const PacketAide& packet, int nLen);																			//�һ�����

	void requestRecords();									//����һ���¼
	void onRecordsResp(const PacketAide& packet, int nLen);	//���ضһ���¼

protected:
	CC_SYNTHESIZE(int, m_nCostGold, CostGold);			//�һ����ĵ�Ԫ��
};

#endif // _LogicExchange_H_

