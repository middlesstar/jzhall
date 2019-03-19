#ifndef SHZ_CGameUserManager_H_
#define SHZ_CGameUserManager_H_

#include <vector>

#include "IUserManagerSink_SHZ.h"

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class SHZCClientUserItem : public SHZIClientUserItem
{
	//��Ԫ����
	friend class SHZCGameUserManager;
	friend class SHZCPlazaUserManager;

	//���Ա���
protected:
	tagSHZ_UserInfo						m_UserInfo;							//�û���Ϣ
	tagCustomFaceInfo				m_CustomFaceInfo;					//�Զ�ͷ��
	tagPropertyPackage              m_PropertyPackage;                  //���߰���

	//��չ����
protected:
	byte							m_cbCompanion;						//�û���ϵ
	char							m_szUserNote[LEN_USERNOTE];			//�û���ע

	//��������
public:
	//���캯��
	SHZCClientUserItem();
	//��������
	virtual ~SHZCClientUserItem();

	//���Խӿ�
public:
	//��Ҫ�ȼ�
	virtual long GetImportOrder();
	//�û���Ϣ
	virtual tagSHZ_UserInfo * GetUserInfo() { return &m_UserInfo; }
	//�Զ�ͷ��
	virtual tagCustomFaceInfo * GetCustomFaceInfo() { return &m_CustomFaceInfo; }
	//���߰���
	virtual tagPropertyPackage * GetPropertyPackage() { return &m_PropertyPackage; };

	//ͷ����Ϣ
public:
	//ͷ������
	virtual word GetFaceID() { return m_UserInfo.wFaceID; }
	//�Զ�����
	virtual dword GetCustomID() { return m_UserInfo.dwCustomID; }

	//������Ϣ
public:
	//�û��Ա�
	virtual byte GetGender() { return m_UserInfo.cbGender; }
	//�û���ʶ
	virtual dword GetUserID() { return m_UserInfo.dwUserID; }
	//��Ϸ��ʶ
	virtual dword GetGameID() { return m_UserInfo.dwGameID; }
	//���ű�ʶ
	virtual dword GetGroupID() { return m_UserInfo.dwGroupID; }
	//�û��ǳ�
	virtual const char* GetNickName() { return m_UserInfo.szNickName; }
	//��������
	virtual const char* GetGroupName() { return m_UserInfo.szGroupName; }
	//����ǩ��
	virtual const char* GetUnderWrite() { return m_UserInfo.szUnderWrite; }

	//��������
public:
	//�û�����
	virtual dword GetUserMedal() { return m_UserInfo.dwUserMedal; }
	//������ֵ
	virtual dword GetUserExperience() { return m_UserInfo.dwExperience; }
	//������ֵ
	virtual long GetUserLoveLiness() { return m_UserInfo.lLoveLiness; }

	//�ȼ���Ϣ
public:
	//��Ա�ȼ�
	virtual byte GetMemberOrder() { return m_UserInfo.cbMemberOrder; }
	//����ȼ�
	virtual byte GetMasterOrder() { return m_UserInfo.cbMasterOrder; }

	//�û�״̬
public:
	//�û�����
	virtual word GetTableID() { return m_UserInfo.wTableID; }
	//�û�����
	virtual word GetChairID() { return m_UserInfo.wChairID; }
	//�û�״̬
	virtual byte GetUserStatus() { return m_UserInfo.cbUserStatus; }

	//������Ϣ
public:
	//������ֵ
	virtual SCORE GetUserScore() { return m_UserInfo.lScore; }
	//�ɼ���ֵ
	virtual SCORE GetUserGrade() { return m_UserInfo.lGrade; }
	//������ֵ
	virtual SCORE GetUserInsure() { return m_UserInfo.lInsure; }

	//��Ϸ��Ϣ
public:
	//ʤ������
	virtual dword GetUserWinCount() { return m_UserInfo.dwWinCount; }
	//ʧ������
	virtual dword GetUserLostCount() { return m_UserInfo.dwLostCount; }
	//�;�����
	virtual dword GetUserDrawCount() { return m_UserInfo.dwDrawCount; }
	//��������
	virtual dword GetUserFleeCount() { return m_UserInfo.dwFleeCount; }
	//��Ϸ����
	virtual dword GetUserPlayCount() { return m_UserInfo.dwWinCount+m_UserInfo.dwLostCount+m_UserInfo.dwDrawCount+m_UserInfo.dwFleeCount; }

	//������Ϣ
public:
	//�û�ʤ��
	virtual float GetUserWinRate();
	//�û�����
	virtual float GetUserLostRate();
	//�û�����
	virtual float GetUserDrawRate();
	//�û�����
	virtual float GetUserFleeRate();

	//�û���ϵ
public:
	//��ȡ��ϵ
	virtual byte GetUserCompanion() { return m_cbCompanion; }
	//���ù�ϵ
	virtual void SetUserCompanion(byte cbCompanion) { m_cbCompanion=cbCompanion; }

	//�û���ע
public:
	//���ñ�ע
	virtual void SetUserNoteInfo(const char* pszUserNote);
	//��ȡ��ע
	virtual const char* GetUserNoteInfo() { return m_szUserNote; }
};


//�û�����
typedef std::vector<SHZCClientUserItem *> ClientUserItemVector_SHZ;

//////////////////////////////////////////////////////////////////////////////////

//�û�����
class SHZCPlazaUserManager
{
	//��������
protected:
	ClientUserItemVector_SHZ		m_UserItemActive;					//�����
	ClientUserItemVector_SHZ		m_UserItemStorage;					//�洢����

	//����ӿ�
protected:
	SHZIUserManagerSink *				m_pIUserManagerSink;			//֪ͨ�ӿ�
	//IUserInformation *				m_pIUserInformation;		//�û���Ϣ

	//��������
public:
	//���캯��
	SHZCPlazaUserManager();
	//��������
	virtual ~SHZCPlazaUserManager();

	//���ýӿ�
public:
	////���ýӿ�
	//virtual bool SetUserInformation(IUserInformation * pIUserInformation);
	//���ýӿ�
	virtual bool SetUserManagerSink(SHZIUserManagerSink * pIUserManagerSink);

	//����ӿ�
public:
	//�����û�
	virtual bool ResetUserItem();
	//ɾ���û�
	virtual bool DeleteUserItem(SHZIClientUserItem * pIClientUserItem);
	//�����û�
	virtual SHZIClientUserItem * ActiveUserItem(const tagSHZ_UserInfo & UserInfo, const tagCustomFaceInfo & CustomFaceInfo);

	//���½ӿ�
public:
	//���»���
	virtual bool UpdateUserItemScore(SHZIClientUserItem * pIClientUserItem, const tagUserScore * pUserScore);
	//����״̬
	virtual bool UpdateUserItemStatus(SHZIClientUserItem * pIClientUserItem, const tagUserStatus * pUserStatus);
	//��������
	virtual bool UpdateUserItemAttrib(SHZIClientUserItem * pIClientUserItem, const tagUserAttrib * pUserAttrib);
	//����ͷ��
	virtual bool UpdateUserCustomFace(SHZIClientUserItem * pIClientUserItem, dword dwCustomID, const tagCustomFaceInfo & CustomFaceInfo);

	//���ҽӿ�
public:
	//ö���û�
	virtual SHZIClientUserItem * EnumUserItem(word wEnumIndex);
	//�����û�
	virtual SHZIClientUserItem * SearchUserByUserID(dword dwUserID);
	//�����û�
	virtual SHZIClientUserItem * SearchUserByGameID(dword dwGameID);
	//�����û�
	virtual SHZIClientUserItem * SearchUserByNickName(const char* pszNickName);

public:
	//�������
	virtual dword GetActiveUserCount(){return (dword)m_UserItemActive.size();}
};

//////////////////////////////////////////////////////////////////////////////////

//�û�����
class SHZCGameUserManager
{
	//�û�����
protected:
	ClientUserItemVector_SHZ			m_UserItemLookon;					//�Թ��б�
	ClientUserItemVector_SHZ			m_UserItemStorage;					//�洢�б�

	//��������
protected:
	SHZCClientUserItem *				m_pTableUserItem[MAX_CHAIR];		//��Ϸ�û�

	//����ӿ�
protected:
	SHZIUserManagerSink *				m_pIUserManagerSink;				//֪ͨ�ӿ�

	//��������
public:
	//���캯��
	SHZCGameUserManager();
	//��������
	virtual ~SHZCGameUserManager();

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetUserManagerSink(SHZIUserManagerSink * pIUserManagerSink);
	
	//����ӿ�
public:
	//�����û�
	virtual bool ResetUserItem();
	//ɾ���û�
	virtual bool DeleteUserItem(SHZIClientUserItem * pIClientUserItem);
	//�����û�
	virtual SHZIClientUserItem * ActiveUserItem(const tagSHZ_UserInfo & UserInfo, const tagCustomFaceInfo & CustomFaceInfo, byte cbCompanion, const char* pszUserNote);

	//���½ӿ�
public:
	//���»���
	virtual bool UpdateUserItemScore(SHZIClientUserItem * pIClientUserItem, const tagUserScore * pUserScore);
	//����״̬
	virtual bool UpdateUserItemStatus(SHZIClientUserItem * pIClientUserItem, const tagUserStatus * pUserStatus);
	//��������
	virtual bool UpdateUserItemAttrib(SHZIClientUserItem * pIClientUserItem, const tagUserAttrib * pUserAttrib);
	//����ͷ��
	virtual bool UpdateUserCustomFace(SHZIClientUserItem * pIClientUserItem, dword dwCustomID, const tagCustomFaceInfo & CustomFaceInfo);

	//��ѯ�ӿ�
public:
	//��Ϸ�û�
	virtual SHZIClientUserItem * GetTableUserItem(word wChariID);
	//�Թ��û�
	virtual SHZIClientUserItem * EnumLookonUserItem(word wEnumIndex);

	//���ҽӿ�
public:
	//�����û�
	virtual SHZIClientUserItem * SearchUserByUserID(dword dwUserID);
	//�����û�
	virtual SHZIClientUserItem * SearchUserByGameID(dword dwGameID);
	//�����û�
	virtual SHZIClientUserItem * SearchUserByNickName(const char* pszNickName);
};

//////////////////////////////////////////////////////////////////////////////////

#endif