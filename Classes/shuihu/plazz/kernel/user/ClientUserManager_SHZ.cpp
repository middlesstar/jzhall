#include "ClientUserManager_SHZ.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCClientUserItem::SHZCClientUserItem()
{
	//设置变量
	m_cbCompanion=CP_NORMAL;
	zeromemory(&m_UserInfo,sizeof(m_UserInfo));
	zeromemory(m_szUserNote,sizeof(m_szUserNote));
	zeromemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));
	zeromemory(&m_PropertyPackage,sizeof(m_PropertyPackage));
	m_UserInfo.wTableID=INVALID_TABLE;
	m_UserInfo.wChairID=INVALID_CHAIR;

	return;
}

//析构函数
SHZCClientUserItem::~SHZCClientUserItem()
{
}

//重要等级
long SHZCClientUserItem::GetImportOrder()
{
	//构造等级
	int nOrder=0;
	if (m_cbCompanion==CP_FRIEND) nOrder+=1000;
	if (m_UserInfo.cbMemberOrder!=0) nOrder+=m_UserInfo.cbMemberOrder*100L;
	if (m_UserInfo.cbMasterOrder!=0) nOrder+=m_UserInfo.cbMasterOrder*10000L;

	return nOrder;
}

//用户胜率
float SHZCClientUserItem::GetUserWinRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwWinCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//用户输率
float SHZCClientUserItem::GetUserLostRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwLostCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//用户和率
float SHZCClientUserItem::GetUserDrawRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwDrawCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//用户逃率
float SHZCClientUserItem::GetUserFleeRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwFleeCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//设置备注
void SHZCClientUserItem::SetUserNoteInfo(const char* pszUserNote)
{
	//效验参数
	ASSERT(pszUserNote!=0);
	if (pszUserNote==0) return;

	//设置备注
	strncpy(m_szUserNote,pszUserNote,countarray(m_szUserNote));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCPlazaUserManager::SHZCPlazaUserManager()
{
	//组件接口
	m_pIUserManagerSink=0;
	//m_pIUserInformation=0;

	return;
}

//析构函数
SHZCPlazaUserManager::~SHZCPlazaUserManager()
{
	for (int i=0,l=(int)m_UserItemStorage.size();i<l;++i)
	{
		delete m_UserItemStorage[i];
	}
	m_UserItemStorage.clear();

	for (int i=0,l=(int)m_UserItemActive.size();i<l;++i)
	{
		delete m_UserItemActive[i];
	}
	m_UserItemActive.clear();

	return;
}


////设置接口
//bool CPlazaUserManager::SetUserInformation(IUserInformation * pIUserInformation)
//{
//	//设置接口
//	m_pIUserInformation=pIUserInformation;
//	return true;
//}

//设置接口
bool SHZCPlazaUserManager::SetUserManagerSink(SHZIUserManagerSink * pIUserManagerSink)
{
	m_pIUserManagerSink=pIUserManagerSink;
	return true;
}

//增加用户
SHZIClientUserItem * SHZCPlazaUserManager::ActiveUserItem(const tagSHZ_UserInfo & UserInfo, const tagCustomFaceInfo & CustomFaceInfo)
{
	//变量定义
	SHZCClientUserItem * pClientUserItem=0;
	int iStorageCount = m_UserItemStorage.size();
	//获取用户
	if (iStorageCount>0)
	{
		pClientUserItem=m_UserItemStorage[iStorageCount-1];
		m_UserItemStorage.pop_back();
	}
	else
	{
		pClientUserItem=new SHZCClientUserItem;
		if (pClientUserItem==0) return 0;
	}

	//插入用户
	ASSERT(pClientUserItem!=0);
	m_UserItemActive.push_back(pClientUserItem);

	////信息查找
	//ASSERT(m_pIUserInformation!=0);
	//tagCompanionInfo * pCompanionInfo=m_pIUserInformation->SearchCompanionInfo(UserInfo.dwUserID);

	////关系数据
	//if (pCompanionInfo!=0)
	//{
	//	pClientUserItem->m_cbCompanion=pCompanionInfo->cbCompanion;
	//	tstrcpyn(pClientUserItem->m_szUserNote,pCompanionInfo->szUserNote,CountArray(pClientUserItem->m_szUserNote));
	//}
	//else 
	{
		pClientUserItem->m_szUserNote[0]=0;
		pClientUserItem->m_cbCompanion=CP_NORMAL; 
	}

	//设置数据
	memcpy(&pClientUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	memcpy(&pClientUserItem->m_CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//更新通知
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink!=0) m_pIUserManagerSink->OnUserItemAcitve(pClientUserItem);

	////信息更新
	//if (pCompanionInfo!=0)
	//{
	//	//一致判断
	//	bool bSameData=(UserInfo.dwGameID==pCompanionInfo->dwGameID);
	//	if ((bSameData==true)&&(tstrcmp(UserInfo.szNickName,pCompanionInfo->szNickName)!=0)) bSameData=false;

	//	//更新处理
	//	if (bSameData==false) m_pIUserInformation->InsertCompanionInfo(pClientUserItem,pCompanionInfo->cbCompanion);
	//}

	return pClientUserItem;
}


//重置用户
bool SHZCPlazaUserManager::ResetUserItem()
{
	//存储用户
	m_UserItemStorage.insert(
		m_UserItemStorage.end(), 
		m_UserItemActive.begin(), 
		m_UserItemActive.end());

	//设置变量
	m_UserItemActive.clear();
	
	return true;
}

//删除用户
bool SHZCPlazaUserManager::DeleteUserItem(SHZIClientUserItem * pIClientUserItem)
{
	//查找用户
	SHZCClientUserItem * pUserItemActive=0;
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		pUserItemActive=m_UserItemActive[i];
		if (pIClientUserItem==pUserItemActive)
		{
			//删除用户
			m_UserItemActive.erase(m_UserItemActive.begin()+i);
			m_UserItemStorage.push_back(pUserItemActive);

			//删除通知
			ASSERT(m_pIUserManagerSink!=0);
			if (m_pIUserManagerSink)
				m_pIUserManagerSink->OnUserItemDelete(pUserItemActive);

			//设置数据
			pUserItemActive->m_cbCompanion=CP_NORMAL;
			zeromemory(&pUserItemActive->m_UserInfo,sizeof(tagSHZ_UserInfo));

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//更新积分
bool SHZCPlazaUserManager::UpdateUserItemScore(SHZIClientUserItem * pIClientUserItem, const tagUserScore * pUserScore)
{
	//效验参数
	ASSERT(pUserScore!=0);
	ASSERT(pIClientUserItem!=0);

	//获取用户
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//以往数据
	tagUserScore UserScore;
	UserScore.lScore=pUserInfo->lScore;
	UserScore.lGrade=pUserInfo->lGrade;
	UserScore.lInsure=pUserInfo->lInsure;
	UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	UserScore.dwUserMedal=pUserInfo->dwUserMedal;
	UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	std::cout << UserScore.lScore;
	std::cout << pUserInfo->lScore;
	//PLAZZ_PRINTF("------------UserScore.lScore1 = %d--------------", UserScore.lScore);
	//PLAZZ_PRINTF("------------pUserInfo->lScore1 = %d-------------", pUserInfo->lScore);

	//设置数据
	pUserInfo->lScore=pUserScore->lScore;
	pUserInfo->lGrade=pUserScore->lGrade;
	pUserInfo->lInsure=pUserScore->lInsure;
	pUserInfo->dwWinCount=pUserScore->dwWinCount;
	pUserInfo->dwLostCount=pUserScore->dwLostCount;
	pUserInfo->dwDrawCount=pUserScore->dwDrawCount;
	pUserInfo->dwFleeCount=pUserScore->dwFleeCount;
	pUserInfo->dwUserMedal=pUserScore->dwUserMedal;
	pUserInfo->dwExperience=pUserScore->dwExperience;
	pUserInfo->lLoveLiness=pUserScore->lLoveLiness;
	//PLAZZ_PRINTF("------------pUserInfo->lScore1 = %d-------------", pUserInfo->lScore);
	std::cout << pUserInfo->lScore;
	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserScore);

	return true;
}

//更新状态
bool SHZCPlazaUserManager::UpdateUserItemStatus(SHZIClientUserItem * pIClientUserItem, const tagUserStatus * pUserStatus)
{
	//效验参数
	ASSERT(pUserStatus!=0);
	ASSERT(pIClientUserItem!=0);

	//获取用户
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//以往数据
	tagUserStatus UserStatus;
	UserStatus.wTableID=pUserInfo->wTableID;
	UserStatus.wChairID=pUserInfo->wChairID;
	UserStatus.cbUserStatus=pUserInfo->cbUserStatus;

	//设置数据
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserStatus);

	return true;
}

//更新属性
bool SHZCPlazaUserManager::UpdateUserItemAttrib(SHZIClientUserItem * pIClientUserItem, const tagUserAttrib * pUserAttrib)
{
	//效验参数
	ASSERT(pUserAttrib!=0);
	ASSERT(pIClientUserItem!=0);

	//以往数据
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

	//设置数据
	pIClientUserItem->SetUserCompanion(pUserAttrib->cbCompanion);

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserAttrib);

	return true;
}

//更新头像
bool SHZCPlazaUserManager::UpdateUserCustomFace(SHZIClientUserItem * pIClientUserItem, dword dwCustomID, const tagCustomFaceInfo & CustomFaceInfo)
{
	//获取用户
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//设置变量
	pUserInfo->dwCustomID=dwCustomID;
	memcpy(pCustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserFaceUpdate(pIClientUserItem);

	return true;
}

//枚举用户
SHZIClientUserItem * SHZCPlazaUserManager::EnumUserItem(word wEnumIndex)
{
	if (wEnumIndex>=m_UserItemActive.size()) return 0;
	return m_UserItemActive[wEnumIndex];
}

//查找用户
SHZIClientUserItem * SHZCPlazaUserManager::SearchUserByUserID(dword dwUserID)
{
	//用户搜索
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemActive[i];
		if (pClientUserItem->m_UserInfo.dwUserID==dwUserID) return pClientUserItem;
	}

	return 0;
}

//查找用户
SHZIClientUserItem * SHZCPlazaUserManager::SearchUserByGameID(dword dwGameID)
{
	//用户搜索
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemActive[i];
		if (pClientUserItem->m_UserInfo.dwGameID==dwGameID) return pClientUserItem;
	}

	return 0;
}

//查找用户
SHZIClientUserItem * SHZCPlazaUserManager::SearchUserByNickName(const char* pszNickName)
{
	//用户搜索
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemActive[i];
		const char* pszTempNickName=pClientUserItem->GetNickName();
		if (strcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCGameUserManager::SHZCGameUserManager()
{
	//组件接口
	m_pIUserManagerSink=0;

	//用户数据
	zeromemory(m_pTableUserItem,sizeof(m_pTableUserItem));

	return;
}

//析构函数
SHZCGameUserManager::~SHZCGameUserManager()
{
	for (int i=0,l=(int)m_UserItemStorage.size();i<l;++i)
	{
		delete m_UserItemStorage[i];
	}
	m_UserItemStorage.clear();

	for (int i=0,l=(int)m_UserItemLookon.size();i<l;++i)
	{
		delete m_UserItemLookon[i];
	}

	m_UserItemLookon.clear();

	//游戏用户
	for (word i=0;i<countarray(m_pTableUserItem);i++)
	{
		if (m_pTableUserItem[i]!=0) 
			delete m_pTableUserItem[i];
	}

	//用户数据
	zeromemory(m_pTableUserItem,sizeof(m_pTableUserItem));

	return;
}


//设置接口
bool SHZCGameUserManager::SetUserManagerSink(SHZIUserManagerSink* pIUserManangerSink)
{
	m_pIUserManagerSink=pIUserManangerSink;
	return true;
}

//重置用户
bool SHZCGameUserManager::ResetUserItem()
{
	//存储用户
	for (word i=0;i<MAX_CHAIR;i++)
	{
		if (m_pTableUserItem[i]!=0) m_UserItemStorage.push_back(m_pTableUserItem[i]);
	}
	m_UserItemStorage.insert(
		m_UserItemStorage.end(), 
		m_UserItemLookon.begin(), 
		m_UserItemLookon.end());

	//设置变量
	m_UserItemLookon.clear();
	zeromemory(m_pTableUserItem,sizeof(m_pTableUserItem));

	return true;
}

//删除用户
bool SHZCGameUserManager::DeleteUserItem(SHZIClientUserItem * pIClientUserItem)
{
	//效验状态
	ASSERT((pIClientUserItem!=0)&&(pIClientUserItem->GetChairID()<MAX_CHAIR));
	if ((pIClientUserItem==0)||(pIClientUserItem->GetChairID()>=MAX_CHAIR)) return false;

	//游戏用户
	if (pIClientUserItem->GetUserStatus()!=US_LOOKON)
	{
		//变量定义
		word wChairID=pIClientUserItem->GetChairID();
		SHZCClientUserItem * pUserItemRemove=(SHZCClientUserItem *)pIClientUserItem;

		//效验用户
		ASSERT(m_pTableUserItem[pIClientUserItem->GetChairID()]==pIClientUserItem);
		if (m_pTableUserItem[pIClientUserItem->GetChairID()]!=pIClientUserItem) return false;

		//删除用户
		m_pTableUserItem[wChairID]=0;
		m_UserItemStorage.push_back(pUserItemRemove);

		//删除通知
		ASSERT(m_pIUserManagerSink!=0);
		if (m_pIUserManagerSink)
			m_pIUserManagerSink->OnUserItemDelete(pUserItemRemove);

		//设置数据
		pUserItemRemove->m_cbCompanion=CP_NORMAL;
		zeromemory(&pUserItemRemove->m_UserInfo,sizeof(tagSHZ_UserInfo));

		return true;
	}

	//旁观用户
	SHZCClientUserItem * pUserItemActive=0;
	for (int i=0,l=(int)m_UserItemLookon.size();i<l;i++)
	{
		pUserItemActive=m_UserItemLookon[i];
		if (pIClientUserItem==pUserItemActive)
		{
			//删除用户
			m_UserItemLookon.erase(m_UserItemLookon.begin()+i);
			m_UserItemStorage.push_back(pUserItemActive);

			//删除通知
			ASSERT(m_pIUserManagerSink!=0);
			if (m_pIUserManagerSink)
				m_pIUserManagerSink->OnUserItemDelete(pUserItemActive);

			//设置数据
			pUserItemActive->m_cbCompanion=CP_NORMAL;
			zeromemory(&pUserItemActive->m_UserInfo,sizeof(tagSHZ_UserInfo));

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//增加用户
SHZIClientUserItem * SHZCGameUserManager::ActiveUserItem(const tagSHZ_UserInfo & UserInfo, const tagCustomFaceInfo & CustomFaceInfo, byte cbCompanion, const char* pszUserNote)
{
	//效验状态
	ASSERT(UserInfo.wChairID<MAX_CHAIR);
	if (UserInfo.wChairID>=MAX_CHAIR) return 0;

	//变量定义
	SHZCClientUserItem * pClientUserItem=0;
	int nStorageCount=m_UserItemStorage.size();

	//获取用户
	if (nStorageCount>0)
	{
		pClientUserItem=m_UserItemStorage[nStorageCount-1];
		ASSERT(pClientUserItem!=0);
		m_UserItemStorage.pop_back();
	}
	else
	{
		pClientUserItem=new SHZCClientUserItem;
		ASSERT(pClientUserItem!=0);
		if (pClientUserItem==0) return 0;
	}

	//设置用户
	if (UserInfo.cbUserStatus==US_LOOKON) m_UserItemLookon.push_back(pClientUserItem);
	else m_pTableUserItem[UserInfo.wChairID]=pClientUserItem;

	//用户信息
	memcpy(&pClientUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	memcpy(&pClientUserItem->m_CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//附加信息
	pClientUserItem->m_cbCompanion=cbCompanion;
	strncpy(pClientUserItem->m_szUserNote,pszUserNote,countarray(pClientUserItem->m_szUserNote));

	//更新通知
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink!=0) 
		m_pIUserManagerSink->OnUserItemAcitve(pClientUserItem);

	return pClientUserItem;
}

//更新积分
bool SHZCGameUserManager::UpdateUserItemScore(SHZIClientUserItem * pIClientUserItem, const tagUserScore * pUserScore)
{
	//效验参数
	ASSERT(pUserScore!=0);
	ASSERT(pIClientUserItem!=0);

	//获取用户
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//以往数据
	tagUserScore UserScore;
	UserScore.lScore=pUserInfo->lScore;
	UserScore.lGrade=pUserInfo->lGrade;
	UserScore.lInsure=pUserInfo->lInsure;
	UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	UserScore.dwUserMedal=pUserInfo->dwUserMedal;
	UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.lLoveLiness=pUserInfo->lLoveLiness;

	//设置数据
	pUserInfo->lScore=pUserScore->lScore;
	pUserInfo->lGrade=pUserScore->lGrade;
	pUserInfo->lInsure=pUserScore->lInsure;
	pUserInfo->dwWinCount=pUserScore->dwWinCount;
	pUserInfo->dwLostCount=pUserScore->dwLostCount;
	pUserInfo->dwDrawCount=pUserScore->dwDrawCount;
	pUserInfo->dwFleeCount=pUserScore->dwFleeCount;
	pUserInfo->dwUserMedal=pUserScore->dwUserMedal;
	pUserInfo->dwExperience=pUserScore->dwExperience;
	pUserInfo->lLoveLiness=pUserScore->lLoveLiness;

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserScore);

	return true;
}

//更新状态
bool SHZCGameUserManager::UpdateUserItemStatus(SHZIClientUserItem * pIClientUserItem, const tagUserStatus * pUserStatus)
{
	//效验参数
	ASSERT(pUserStatus!=0);
	ASSERT(pIClientUserItem!=0);

	//获取用户
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//以往数据
	tagUserStatus UserStatus;
	UserStatus.wTableID=pUserInfo->wTableID;
	UserStatus.wChairID=pUserInfo->wChairID;
	UserStatus.cbUserStatus=pUserInfo->cbUserStatus;

	//设置数据
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		if (m_pIUserManagerSink!=0) m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserStatus);

	return true;
}

//更新属性
bool SHZCGameUserManager::UpdateUserItemAttrib(SHZIClientUserItem * pIClientUserItem, const tagUserAttrib * pUserAttrib)
{
	//以往数据
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

	//设置变量
	pIClientUserItem->SetUserCompanion(pUserAttrib->cbCompanion);

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink!=0) 
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserAttrib);

	return true;
}

//更新头像
bool SHZCGameUserManager::UpdateUserCustomFace(SHZIClientUserItem * pIClientUserItem, dword dwCustomID, const tagCustomFaceInfo & CustomFaceInfo)
{
	//获取用户
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//设置变量
	pUserInfo->dwCustomID=dwCustomID;
	memcpy(&pCustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink!=0) 
		m_pIUserManagerSink->OnUserFaceUpdate(pIClientUserItem);

	return true;
}

//游戏用户
SHZIClientUserItem * SHZCGameUserManager::GetTableUserItem(word wChariID)
{
	//效验参数
	ASSERT(wChariID<countarray(m_pTableUserItem));
	if (wChariID>=countarray(m_pTableUserItem)) return 0;

	return m_pTableUserItem[wChariID];
}

//旁观用户
SHZIClientUserItem * SHZCGameUserManager::EnumLookonUserItem(word wEnumIndex)
{
	if (wEnumIndex>=m_UserItemLookon.size()) return 0;
	return m_UserItemLookon[wEnumIndex];
}

//查找用户
SHZIClientUserItem * SHZCGameUserManager::SearchUserByUserID(dword dwUserID)
{
	//游戏用户
	for (word i=0;i<MAX_CHAIR;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_pTableUserItem[i];
		if ((pClientUserItem!=0)&&(pClientUserItem->GetUserID()==dwUserID)) return pClientUserItem;
	}

	//旁观用户
	for (int i=0,l=(int)m_UserItemLookon.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemLookon[i];
		if (pClientUserItem->GetUserID()==dwUserID) return pClientUserItem;
	}

	return 0;
}

//查找用户
SHZIClientUserItem * SHZCGameUserManager::SearchUserByGameID(dword dwGameID)
{
	//游戏用户
	for (word i=0;i<MAX_CHAIR;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_pTableUserItem[i];
		if ((pClientUserItem!=0)&&(pClientUserItem->GetGameID()==dwGameID)) return pClientUserItem;
	}

	//旁观用户
	for (int i=0,l=(int)m_UserItemLookon.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemLookon[i];
		if (pClientUserItem->GetGameID()==dwGameID) return pClientUserItem;
	}

	return 0;
}

//查找用户
SHZIClientUserItem * SHZCGameUserManager::SearchUserByNickName(const char* pszNickName)
{
	//游戏用户
	for (word i=0;i<MAX_CHAIR;i++)
	{
		//获取用户
		SHZCClientUserItem * pClientUserItem=m_pTableUserItem[i];

		//用户判断
		if (pClientUserItem!=0)
		{
			const char* pszTempNickName=pClientUserItem->GetNickName();
			if (strcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
		}
	}

	//旁观用户
	for (int i=0,l=(int)m_UserItemLookon.size();i<l;i++)
	{
		//获取用户
		SHZCClientUserItem * pClientUserItem=m_UserItemLookon[i];

		//用户判断
		if (pClientUserItem!=0)
		{
			const char* pszTempNickName=pClientUserItem->GetNickName();
			if (strcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
		}
	}

	return 0;
}
