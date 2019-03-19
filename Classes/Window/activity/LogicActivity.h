#ifndef  _LogicActivity_H_
#define  _LogicActivity_H_

#include "Window/LogicBase.h"
class LogicActivity : public LogicBase
{
public:
	LogicActivity();
	virtual ~LogicActivity();

	SINGLE_INSTANCE_FUNC(LogicActivity);

	//发送抢红包请求
	void requestGrabRed();
	//接收抢红包同意请求
	void onGrabRed(const PacketAide& packet, int nLen);
	//发送抢红包数据
	void sendGrabRedData(int score, int yuanbao);
protected:
	void _registerMessageHandler();

public:
	CMD_ST_FuDaiRsp	m_qfd;			//抢福袋数据

};



#endif // _LogicActivity_H_

