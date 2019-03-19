#ifndef  _LogicPromote_H_
#define  _LogicPromote_H_

#include "Window/LogicBase.h"

class  LogicPromote : public LogicBase
{
public:
	LogicPromote();
	virtual ~LogicPromote();

	SINGLE_INSTANCE_FUNC(LogicPromote);

protected:
	void _registerMessageHandler();

public:
	void requestShareSucess(int nType);								//请求分享成功后调用
	void onShareSucessResp(const PacketAide& packet, int nLen);		//分享后返回

	void requestGetSharePrize(const std::string& strCode);			//请求通过分享码领取奖励
	void onGetSharePrizeResp(const PacketAide& packet, int nLen);	//通过分享码领取奖励返回

	void requestShareInfo(int nOpType);								//请求红包收益操作 //1-查询收益 2-领取收益 3-查询排行榜
	void onShareInfoResp(const PacketAide& packet, int nLen);		//红包收益操作返回
	void onShareRankResp(const PacketAide& packet, int nLen);		//红包收益排行返回

	void requestShareDays();										//请求分享天数
	void requestMySharePrize();										//请求领取分享天数的奖励
	void onShareDaysResp(const PacketAide& packet, int nLen);		//返回分享天数

	int getShareDayLevel(int nShareDay);
protected:
	CC_SYNTHESIZE(int, m_nGetCoinNum, GetCoinNum);					//可以领取的金币
	CC_SYNTHESIZE(int, m_nGetGoldNum, GoldNum);						//可以领取的元宝

	CC_SYNTHESIZE(int, m_nShareDay, ShareDay);						//分享的天数
	CC_SYNTHESIZE(int, m_nGetIndex, GetIndex);						//领取的宝箱index，0-4
};

#endif // _LogicPromote_H_

