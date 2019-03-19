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
	void requestExchange(int nGoodsIndex,longlong lTel = 0 ,longlong lQQ = 0,const string& strName = "",const string& strAddress = "");	//ÇëÇó¶Ò»»
	void onExchangeResp(const PacketAide& packet, int nLen);																			//¶Ò»»·µ»Ø

	void requestRecords();									//ÇëÇó¶Ò»»¼ÇÂ¼
	void onRecordsResp(const PacketAide& packet, int nLen);	//·µ»Ø¶Ò»»¼ÇÂ¼

protected:
	CC_SYNTHESIZE(int, m_nCostGold, CostGold);			//¶Ò»»ÏûºÄµÄÔª±¦
};

#endif // _LogicExchange_H_

