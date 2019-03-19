#ifndef _SLWH_LOGON
#define _SLWH_LOGON

#include "Network/CSocketMission.h"

class SLWHLogon : public CSocketMission
{
public:
	SLWHLogon();
	virtual ~SLWHLogon();
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);

private:

};



#endif // !_SLWH_LOGON
