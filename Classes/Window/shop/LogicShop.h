#ifndef  _LogicShop_H_
#define  _LogicShop_H_

#include "Window/LogicBase.h"
#include "network/HttpClient.h"

typedef enum _ChannelId
{
	kChannel_iOS = 1,
	kChannel_android
} ChannelId;

class  LogicShop : public LogicBase
{
public:
	LogicShop();
	virtual ~LogicShop();

	SINGLE_INSTANCE_FUNC(LogicShop);

protected:
	void _registerMessageHandler();

public:
	void requestOrderId(int productId);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void onBuySuccessResp(const PacketAide& packet, int nLen);	//充值成功返回

protected:
	CMD_ST_GetEmailList_rsp m_stMailList;
};

#endif // _LogicShop_H_

