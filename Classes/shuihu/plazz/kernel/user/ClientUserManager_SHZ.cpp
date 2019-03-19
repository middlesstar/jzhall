#include "ClientUserManager_SHZ.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
SHZCClientUserItem::SHZCClientUserItem()
{
	//���ñ���
	m_cbCompanion=CP_NORMAL;
	zeromemory(&m_UserInfo,sizeof(m_UserInfo));
	zeromemory(m_szUserNote,sizeof(m_szUserNote));
	zeromemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));
	zeromemory(&m_PropertyPackage,sizeof(m_PropertyPackage));
	m_UserInfo.wTableID=INVALID_TABLE;
	m_UserInfo.wChairID=INVALID_CHAIR;

	return;
}

//��������
SHZCClientUserItem::~SHZCClientUserItem()
{
}

//��Ҫ�ȼ�
long SHZCClientUserItem::GetImportOrder()
{
	//����ȼ�
	int nOrder=0;
	if (m_cbCompanion==CP_FRIEND) nOrder+=1000;
	if (m_UserInfo.cbMemberOrder!=0) nOrder+=m_UserInfo.cbMemberOrder*100L;
	if (m_UserInfo.cbMasterOrder!=0) nOrder+=m_UserInfo.cbMasterOrder*10000L;

	return nOrder;
}

//�û�ʤ��
float SHZCClientUserItem::GetUserWinRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwWinCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//�û�����
float SHZCClientUserItem::GetUserLostRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwLostCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//�û�����
float SHZCClientUserItem::GetUserDrawRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwDrawCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//�û�����
float SHZCClientUserItem::GetUserFleeRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwFleeCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//���ñ�ע
void SHZCClientUserItem::SetUserNoteInfo(const char* pszUserNote)
{
	//Ч�����
	ASSERT(pszUserNote!=0);
	if (pszUserNote==0) return;

	//���ñ�ע
	strncpy(m_szUserNote,pszUserNote,countarray(m_szUserNote));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
SHZCPlazaUserManager::SHZCPlazaUserManager()
{
	//����ӿ�
	m_pIUserManagerSink=0;
	//m_pIUserInformation=0;

	return;
}

//��������
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


////���ýӿ�
//bool CPlazaUserManager::SetUserInformation(IUserInformation * pIUserInformation)
//{
//	//���ýӿ�
//	m_pIUserInformation=pIUserInformation;
//	return true;
//}

//���ýӿ�
bool SHZCPlazaUserManager::SetUserManagerSink(SHZIUserManagerSink * pIUserManagerSink)
{
	m_pIUserManagerSink=pIUserManagerSink;
	return true;
}

//�����û�
SHZIClientUserItem * SHZCPlazaUserManager::ActiveUserItem(const tagSHZ_UserInfo & UserInfo, const tagCustomFaceInfo & CustomFaceInfo)
{
	//��������
	SHZCClientUserItem * pClientUserItem=0;
	int iStorageCount = m_UserItemStorage.size();
	//��ȡ�û�
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

	//�����û�
	ASSERT(pClientUserItem!=0);
	m_UserItemActive.push_back(pClientUserItem);

	////��Ϣ����
	//ASSERT(m_pIUserInformation!=0);
	//tagCompanionInfo * pCompanionInfo=m_pIUserInformation->SearchCompanionInfo(UserInfo.dwUserID);

	////��ϵ����
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

	//��������
	memcpy(&pClientUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	memcpy(&pClientUserItem->m_CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//����֪ͨ
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink!=0) m_pIUserManagerSink->OnUserItemAcitve(pClientUserItem);

	////��Ϣ����
	//if (pCompanionInfo!=0)
	//{
	//	//һ���ж�
	//	bool bSameData=(UserInfo.dwGameID==pCompanionInfo->dwGameID);
	//	if ((bSameData==true)&&(tstrcmp(UserInfo.szNickName,pCompanionInfo->szNickName)!=0)) bSameData=false;

	//	//���´���
	//	if (bSameData==false) m_pIUserInformation->InsertCompanionInfo(pClientUserItem,pCompanionInfo->cbCompanion);
	//}

	return pClientUserItem;
}


//�����û�
bool SHZCPlazaUserManager::ResetUserItem()
{
	//�洢�û�
	m_UserItemStorage.insert(
		m_UserItemStorage.end(), 
		m_UserItemActive.begin(), 
		m_UserItemActive.end());

	//���ñ���
	m_UserItemActive.clear();
	
	return true;
}

//ɾ���û�
bool SHZCPlazaUserManager::DeleteUserItem(SHZIClientUserItem * pIClientUserItem)
{
	//�����û�
	SHZCClientUserItem * pUserItemActive=0;
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		pUserItemActive=m_UserItemActive[i];
		if (pIClientUserItem==pUserItemActive)
		{
			//ɾ���û�
			m_UserItemActive.erase(m_UserItemActive.begin()+i);
			m_UserItemStorage.push_back(pUserItemActive);

			//ɾ��֪ͨ
			ASSERT(m_pIUserManagerSink!=0);
			if (m_pIUserManagerSink)
				m_pIUserManagerSink->OnUserItemDelete(pUserItemActive);

			//��������
			pUserItemActive->m_cbCompanion=CP_NORMAL;
			zeromemory(&pUserItemActive->m_UserInfo,sizeof(tagSHZ_UserInfo));

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//���»���
bool SHZCPlazaUserManager::UpdateUserItemScore(SHZIClientUserItem * pIClientUserItem, const tagUserScore * pUserScore)
{
	//Ч�����
	ASSERT(pUserScore!=0);
	ASSERT(pIClientUserItem!=0);

	//��ȡ�û�
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//��������
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

	//��������
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
	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserScore);

	return true;
}

//����״̬
bool SHZCPlazaUserManager::UpdateUserItemStatus(SHZIClientUserItem * pIClientUserItem, const tagUserStatus * pUserStatus)
{
	//Ч�����
	ASSERT(pUserStatus!=0);
	ASSERT(pIClientUserItem!=0);

	//��ȡ�û�
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//��������
	tagUserStatus UserStatus;
	UserStatus.wTableID=pUserInfo->wTableID;
	UserStatus.wChairID=pUserInfo->wChairID;
	UserStatus.cbUserStatus=pUserInfo->cbUserStatus;

	//��������
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserStatus);

	return true;
}

//��������
bool SHZCPlazaUserManager::UpdateUserItemAttrib(SHZIClientUserItem * pIClientUserItem, const tagUserAttrib * pUserAttrib)
{
	//Ч�����
	ASSERT(pUserAttrib!=0);
	ASSERT(pIClientUserItem!=0);

	//��������
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

	//��������
	pIClientUserItem->SetUserCompanion(pUserAttrib->cbCompanion);

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserAttrib);

	return true;
}

//����ͷ��
bool SHZCPlazaUserManager::UpdateUserCustomFace(SHZIClientUserItem * pIClientUserItem, dword dwCustomID, const tagCustomFaceInfo & CustomFaceInfo)
{
	//��ȡ�û�
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//���ñ���
	pUserInfo->dwCustomID=dwCustomID;
	memcpy(pCustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserFaceUpdate(pIClientUserItem);

	return true;
}

//ö���û�
SHZIClientUserItem * SHZCPlazaUserManager::EnumUserItem(word wEnumIndex)
{
	if (wEnumIndex>=m_UserItemActive.size()) return 0;
	return m_UserItemActive[wEnumIndex];
}

//�����û�
SHZIClientUserItem * SHZCPlazaUserManager::SearchUserByUserID(dword dwUserID)
{
	//�û�����
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemActive[i];
		if (pClientUserItem->m_UserInfo.dwUserID==dwUserID) return pClientUserItem;
	}

	return 0;
}

//�����û�
SHZIClientUserItem * SHZCPlazaUserManager::SearchUserByGameID(dword dwGameID)
{
	//�û�����
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemActive[i];
		if (pClientUserItem->m_UserInfo.dwGameID==dwGameID) return pClientUserItem;
	}

	return 0;
}

//�����û�
SHZIClientUserItem * SHZCPlazaUserManager::SearchUserByNickName(const char* pszNickName)
{
	//�û�����
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemActive[i];
		const char* pszTempNickName=pClientUserItem->GetNickName();
		if (strcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
SHZCGameUserManager::SHZCGameUserManager()
{
	//����ӿ�
	m_pIUserManagerSink=0;

	//�û�����
	zeromemory(m_pTableUserItem,sizeof(m_pTableUserItem));

	return;
}

//��������
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

	//��Ϸ�û�
	for (word i=0;i<countarray(m_pTableUserItem);i++)
	{
		if (m_pTableUserItem[i]!=0) 
			delete m_pTableUserItem[i];
	}

	//�û�����
	zeromemory(m_pTableUserItem,sizeof(m_pTableUserItem));

	return;
}


//���ýӿ�
bool SHZCGameUserManager::SetUserManagerSink(SHZIUserManagerSink* pIUserManangerSink)
{
	m_pIUserManagerSink=pIUserManangerSink;
	return true;
}

//�����û�
bool SHZCGameUserManager::ResetUserItem()
{
	//�洢�û�
	for (word i=0;i<MAX_CHAIR;i++)
	{
		if (m_pTableUserItem[i]!=0) m_UserItemStorage.push_back(m_pTableUserItem[i]);
	}
	m_UserItemStorage.insert(
		m_UserItemStorage.end(), 
		m_UserItemLookon.begin(), 
		m_UserItemLookon.end());

	//���ñ���
	m_UserItemLookon.clear();
	zeromemory(m_pTableUserItem,sizeof(m_pTableUserItem));

	return true;
}

//ɾ���û�
bool SHZCGameUserManager::DeleteUserItem(SHZIClientUserItem * pIClientUserItem)
{
	//Ч��״̬
	ASSERT((pIClientUserItem!=0)&&(pIClientUserItem->GetChairID()<MAX_CHAIR));
	if ((pIClientUserItem==0)||(pIClientUserItem->GetChairID()>=MAX_CHAIR)) return false;

	//��Ϸ�û�
	if (pIClientUserItem->GetUserStatus()!=US_LOOKON)
	{
		//��������
		word wChairID=pIClientUserItem->GetChairID();
		SHZCClientUserItem * pUserItemRemove=(SHZCClientUserItem *)pIClientUserItem;

		//Ч���û�
		ASSERT(m_pTableUserItem[pIClientUserItem->GetChairID()]==pIClientUserItem);
		if (m_pTableUserItem[pIClientUserItem->GetChairID()]!=pIClientUserItem) return false;

		//ɾ���û�
		m_pTableUserItem[wChairID]=0;
		m_UserItemStorage.push_back(pUserItemRemove);

		//ɾ��֪ͨ
		ASSERT(m_pIUserManagerSink!=0);
		if (m_pIUserManagerSink)
			m_pIUserManagerSink->OnUserItemDelete(pUserItemRemove);

		//��������
		pUserItemRemove->m_cbCompanion=CP_NORMAL;
		zeromemory(&pUserItemRemove->m_UserInfo,sizeof(tagSHZ_UserInfo));

		return true;
	}

	//�Թ��û�
	SHZCClientUserItem * pUserItemActive=0;
	for (int i=0,l=(int)m_UserItemLookon.size();i<l;i++)
	{
		pUserItemActive=m_UserItemLookon[i];
		if (pIClientUserItem==pUserItemActive)
		{
			//ɾ���û�
			m_UserItemLookon.erase(m_UserItemLookon.begin()+i);
			m_UserItemStorage.push_back(pUserItemActive);

			//ɾ��֪ͨ
			ASSERT(m_pIUserManagerSink!=0);
			if (m_pIUserManagerSink)
				m_pIUserManagerSink->OnUserItemDelete(pUserItemActive);

			//��������
			pUserItemActive->m_cbCompanion=CP_NORMAL;
			zeromemory(&pUserItemActive->m_UserInfo,sizeof(tagSHZ_UserInfo));

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�����û�
SHZIClientUserItem * SHZCGameUserManager::ActiveUserItem(const tagSHZ_UserInfo & UserInfo, const tagCustomFaceInfo & CustomFaceInfo, byte cbCompanion, const char* pszUserNote)
{
	//Ч��״̬
	ASSERT(UserInfo.wChairID<MAX_CHAIR);
	if (UserInfo.wChairID>=MAX_CHAIR) return 0;

	//��������
	SHZCClientUserItem * pClientUserItem=0;
	int nStorageCount=m_UserItemStorage.size();

	//��ȡ�û�
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

	//�����û�
	if (UserInfo.cbUserStatus==US_LOOKON) m_UserItemLookon.push_back(pClientUserItem);
	else m_pTableUserItem[UserInfo.wChairID]=pClientUserItem;

	//�û���Ϣ
	memcpy(&pClientUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	memcpy(&pClientUserItem->m_CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//������Ϣ
	pClientUserItem->m_cbCompanion=cbCompanion;
	strncpy(pClientUserItem->m_szUserNote,pszUserNote,countarray(pClientUserItem->m_szUserNote));

	//����֪ͨ
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink!=0) 
		m_pIUserManagerSink->OnUserItemAcitve(pClientUserItem);

	return pClientUserItem;
}

//���»���
bool SHZCGameUserManager::UpdateUserItemScore(SHZIClientUserItem * pIClientUserItem, const tagUserScore * pUserScore)
{
	//Ч�����
	ASSERT(pUserScore!=0);
	ASSERT(pIClientUserItem!=0);

	//��ȡ�û�
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//��������
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

	//��������
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

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserScore);

	return true;
}

//����״̬
bool SHZCGameUserManager::UpdateUserItemStatus(SHZIClientUserItem * pIClientUserItem, const tagUserStatus * pUserStatus)
{
	//Ч�����
	ASSERT(pUserStatus!=0);
	ASSERT(pIClientUserItem!=0);

	//��ȡ�û�
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//��������
	tagUserStatus UserStatus;
	UserStatus.wTableID=pUserInfo->wTableID;
	UserStatus.wChairID=pUserInfo->wChairID;
	UserStatus.cbUserStatus=pUserInfo->cbUserStatus;

	//��������
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		if (m_pIUserManagerSink!=0) m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserStatus);

	return true;
}

//��������
bool SHZCGameUserManager::UpdateUserItemAttrib(SHZIClientUserItem * pIClientUserItem, const tagUserAttrib * pUserAttrib)
{
	//��������
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

	//���ñ���
	pIClientUserItem->SetUserCompanion(pUserAttrib->cbCompanion);

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink!=0) 
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserAttrib);

	return true;
}

//����ͷ��
bool SHZCGameUserManager::UpdateUserCustomFace(SHZIClientUserItem * pIClientUserItem, dword dwCustomID, const tagCustomFaceInfo & CustomFaceInfo)
{
	//��ȡ�û�
	tagSHZ_UserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//���ñ���
	pUserInfo->dwCustomID=dwCustomID;
	memcpy(&pCustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink!=0) 
		m_pIUserManagerSink->OnUserFaceUpdate(pIClientUserItem);

	return true;
}

//��Ϸ�û�
SHZIClientUserItem * SHZCGameUserManager::GetTableUserItem(word wChariID)
{
	//Ч�����
	ASSERT(wChariID<countarray(m_pTableUserItem));
	if (wChariID>=countarray(m_pTableUserItem)) return 0;

	return m_pTableUserItem[wChariID];
}

//�Թ��û�
SHZIClientUserItem * SHZCGameUserManager::EnumLookonUserItem(word wEnumIndex)
{
	if (wEnumIndex>=m_UserItemLookon.size()) return 0;
	return m_UserItemLookon[wEnumIndex];
}

//�����û�
SHZIClientUserItem * SHZCGameUserManager::SearchUserByUserID(dword dwUserID)
{
	//��Ϸ�û�
	for (word i=0;i<MAX_CHAIR;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_pTableUserItem[i];
		if ((pClientUserItem!=0)&&(pClientUserItem->GetUserID()==dwUserID)) return pClientUserItem;
	}

	//�Թ��û�
	for (int i=0,l=(int)m_UserItemLookon.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemLookon[i];
		if (pClientUserItem->GetUserID()==dwUserID) return pClientUserItem;
	}

	return 0;
}

//�����û�
SHZIClientUserItem * SHZCGameUserManager::SearchUserByGameID(dword dwGameID)
{
	//��Ϸ�û�
	for (word i=0;i<MAX_CHAIR;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_pTableUserItem[i];
		if ((pClientUserItem!=0)&&(pClientUserItem->GetGameID()==dwGameID)) return pClientUserItem;
	}

	//�Թ��û�
	for (int i=0,l=(int)m_UserItemLookon.size();i<l;i++)
	{
		SHZCClientUserItem * pClientUserItem=m_UserItemLookon[i];
		if (pClientUserItem->GetGameID()==dwGameID) return pClientUserItem;
	}

	return 0;
}

//�����û�
SHZIClientUserItem * SHZCGameUserManager::SearchUserByNickName(const char* pszNickName)
{
	//��Ϸ�û�
	for (word i=0;i<MAX_CHAIR;i++)
	{
		//��ȡ�û�
		SHZCClientUserItem * pClientUserItem=m_pTableUserItem[i];

		//�û��ж�
		if (pClientUserItem!=0)
		{
			const char* pszTempNickName=pClientUserItem->GetNickName();
			if (strcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
		}
	}

	//�Թ��û�
	for (int i=0,l=(int)m_UserItemLookon.size();i<l;i++)
	{
		//��ȡ�û�
		SHZCClientUserItem * pClientUserItem=m_UserItemLookon[i];

		//�û��ж�
		if (pClientUserItem!=0)
		{
			const char* pszTempNickName=pClientUserItem->GetNickName();
			if (strcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
		}
	}

	return 0;
}
