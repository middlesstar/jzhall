#include <iostream>
#include "CSocketMission.h"

CSocketMission::CSocketMission(const std::string& url, int port)
{
	mSocketEngine = ISocketEngine::create();
	mSocketEngine->setTCPValidate(false);
	mSocketEngine->setSocketEngineSink(this);

	setUrl(url, port);
}

CSocketMission::CSocketMission()
{
	mSocketEngine = ISocketEngine::create();
	mSocketEngine->setTCPValidate(false);
	mSocketEngine->setSocketEngineSink(this);
}

CSocketMission::~CSocketMission()
{
	mSocketEngine->setSocketEngineSink(0);
	ISocketEngine::destory(mSocketEngine);
	mSocketEngine = 0;
}

void CSocketMission::setUrl(const std::string& url, int port)
{
	mUrl = url;
	mPort = port;
}

void CSocketMission::start()
{
	if (isAlive())
	{
		onEventTCPSocketLink();
	}
	else
	{
	    mSocketEngine->connect(mUrl.c_str(), mPort);
	
	}
}

void CSocketMission::stop()
{
	mSocketEngine->disconnect();
}

bool CSocketMission::isAlive() const
{
	return mSocketEngine->isAlive();
}

bool CSocketMission::send(int main, int sub)
{
	return send(main, sub, 0, 0);
}

bool CSocketMission::send(int main, int sub, void* data, int size)
{
	return mSocketEngine->send(main, sub, data, size);
}

bool CSocketMission::send(int nCommandId, void* data, int dataSize)
{
	return mSocketEngine->send(nCommandId, data, dataSize);
}

void CSocketMission::setDetectable(bool bDetectable)
{
	mSocketEngine->setDetectable(bDetectable);
}

void CSocketMission::setTimeOut(int iTimeOut)
{
	mSocketEngine->setTimeOut(iTimeOut);
}