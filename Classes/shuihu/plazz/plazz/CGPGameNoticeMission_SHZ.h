#ifndef SHZ_CGPGameNoticeMission_H_
#define SHZ_CGPGameNoticeMission_H_
#include "shuihu/plazz/Plazz_SHZ.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class SHZIGPGameNoticeMissionSink
{
public:
	virtual ~SHZIGPGameNoticeMissionSink(){}
	virtual void onGPGameNoticeResult(const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class SHZCGPGameNoticeMission
	: public CSocketMission
{
public:
	SHZCGPGameNoticeMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(SHZIGPGameNoticeMissionSink* pIGPKefuMissionSink);
	
	void StartGameNotice();
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	// 消息
	bool onSubGameNoticeResult(void* data, int size);
	
private:
	// 回调
	SHZIGPGameNoticeMissionSink* mIGPGameNoticeMissionSink;
}; 

#endif // _CGPKefuMission_H_