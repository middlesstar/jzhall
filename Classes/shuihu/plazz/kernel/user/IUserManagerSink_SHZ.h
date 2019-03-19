#ifndef SHZ_IUserManagerSink_H_
#define SHZ_IUserManagerSink_H_

#include "IClientUserItem_SHZ.h"
class SHZIUserManagerSink
{
public:
	virtual void OnUserItemAcitve(SHZIClientUserItem* pIClientUserItem) = 0;
	virtual void OnUserItemDelete(SHZIClientUserItem* pIClientUserItem) = 0;
	virtual void OnUserFaceUpdate(SHZIClientUserItem* pIClientUserItem) = 0;
	virtual void OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserScore& LastScore) = 0;
	virtual void OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserStatus& LastStatus) = 0;
	virtual void OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserAttrib & UserAttrib) = 0;
};

#endif // _IUserManagerSink_H_