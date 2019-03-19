#include "GlobalTickObject.h"

//#include "buyu/event/MTNotification.h"
#include "shuihu/MTNotification_SHZ.h"
#include "Network/NetManager.h"

static GlobalTickObject* pGlobelGlobalTickObject = NULL;

GlobalTickObject::GlobalTickObject()
{
}

GlobalTickObject::~GlobalTickObject()
{
	
}

GlobalTickObject* GlobalTickObject::getInstance()
{
	if (!pGlobelGlobalTickObject)
	{
		pGlobelGlobalTickObject = new GlobalTickObject;
	}

	return pGlobelGlobalTickObject;
}

void GlobalTickObject::gameTick(float dt)
{
	//MTNotification::shared()->post(dt);
	SHZMTNotification::shared()->post(dt);

	NetManager::getInstance()->drive();
}