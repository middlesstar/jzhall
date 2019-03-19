#ifndef  _LogicFreeMoney_H_
#define  _LogicFreeMoney_H_

#include "Window/LogicBase.h"

class  LogicFreeMoney : public LogicBase
{
public:
	LogicFreeMoney();
	virtual ~LogicFreeMoney();

	SINGLE_INSTANCE_FUNC(LogicFreeMoney);

protected:
	void _registerMessageHandler();

public:
	void requestDialyPrize();										//请求每日抽奖
	void onDialyPrizeResp(const PacketAide& packet, int nLen);		//每日抽奖返回

	void requestDole();												//请求领取救济金
	void onDoleResp(const PacketAide& packet, int nLen);			//领取救济金返回

	void requestDailySin(int type);									//请求每日签到
	void onDailySinResp(const PacketAide& packet, int nLen);		//领取每日签到金币返回

	void requestWechatPresent(const std::string& strExCode);		//请求微信礼品兑换
	void onWechatPresentResp(const PacketAide& packet, int nLen);	//返回微信礼品兑换

	void requestComment();											//请求好评有礼
	void onCommentResp(const PacketAide& packet, int nLen);			//返回好评有礼

protected:
	CC_SYNTHESIZE(CMD_GP_DailyLottery_Get, m_objDailyPrize, DailyPrize);
	CC_SYNTHESIZE(CMD_ST_SignDay_rsp, m_objSignDay, SignDay);
};

#endif // _LogicFreeMoney_H_

