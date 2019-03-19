#ifndef SHZ_CGPGameNoticeMission_H_
#define SHZ_CGPGameNoticeMission_H_
#include "shuihu/plazz/Plazz_SHZ.h"

//////////////////////////////////////////////////////////////////////////
// �������޸���ʾ
//////////////////////////////////////////////////////////////////////////
class SHZIGPGameNoticeMissionSink
{
public:
	virtual ~SHZIGPGameNoticeMissionSink(){}
	virtual void onGPGameNoticeResult(const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// �һ�����
//////////////////////////////////////////////////////////////////////////
class SHZCGPGameNoticeMission
	: public CSocketMission
{
public:
	SHZCGPGameNoticeMission(const char* url, int port);
	// ���ûص��ӿ�
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
	// ��Ϣ
	bool onSubGameNoticeResult(void* data, int size);
	
private:
	// �ص�
	SHZIGPGameNoticeMissionSink* mIGPGameNoticeMissionSink;
}; 

#endif // _CGPKefuMission_H_