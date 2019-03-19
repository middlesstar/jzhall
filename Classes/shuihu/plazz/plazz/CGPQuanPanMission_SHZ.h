#ifndef SHZ_CGPQuanPanMission_H_
#define SHZ_CGPQuanPanMission_H_
#include "shuihu/plazz/Plazz_SHZ.h"

class SHZICGPQuanPanMissionSink
{
public:
	virtual void onGPQuanPanStatue(uint8 statue){}
	~SHZICGPQuanPanMissionSink(){}
};

///////////////////////////////////////////////////////////////////
// 8��ȫ������
//////////////////////////////////////////////////////////////////

class SHZCGPQuanPanMission
	: public CSocketMission
{
public:
	SHZCGPQuanPanMission(const char* url, int port);
	void setMissionSink(SHZICGPQuanPanMissionSink *pICGPQuanPanMissionSink);
	// ��ȡȫ��״̬
	void getQuanPanStatue();
	// ����ȫ��״̬
	void setQuanPanStatue(uint8 statue);
//////////////////////////////////////////////////////////////////////////
// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	bool onSubQuanPanStatue(void *data, int size);

private:
	SHZICGPQuanPanMissionSink *mICGPQuanPanMissionSink;
	// ��������
	uint8 mMissionType;
};

#endif // _CGPQuanPanMission_H_