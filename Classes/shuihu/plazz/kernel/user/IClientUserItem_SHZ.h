#ifndef SHZ_IClientUserItem_H_
#define SHZ_IClientUserItem_H_

#include "df/DF.h"

//�û��ӿ�
class SHZIClientUserItem
{
public:
	virtual ~SHZIClientUserItem(){};

	//���Խӿ�
public:
	//��Ҫ�ȼ�
	virtual long GetImportOrder()=0;
	//�û���Ϣ
	virtual tagSHZ_UserInfo * GetUserInfo()=0;
	//�Զ�ͷ��
	virtual tagCustomFaceInfo * GetCustomFaceInfo()=0;
	//���߰���
	virtual tagPropertyPackage * GetPropertyPackage()=0;

	//ͷ����Ϣ
public:
	//ͷ������
	virtual word GetFaceID()=0;
	//�Զ�����
	virtual dword GetCustomID()=0;

	//������Ϣ
public:
	//�û��Ա�
	virtual byte GetGender()=0;
	//�û���ʶ
	virtual dword GetUserID()=0;
	//��Ϸ��ʶ
	virtual dword GetGameID()=0;
	//���ű�ʶ
	virtual dword GetGroupID()=0;
	//�û��ǳ�
	virtual const char* GetNickName()=0;
	//��������
	virtual const char* GetGroupName()=0;
	//����ǩ��
	virtual const char* GetUnderWrite()=0;

	//��������
public:
	//�û�����
	virtual dword GetUserMedal()=0;
	//������ֵ
	virtual dword GetUserExperience()=0;
	//������ֵ
	virtual long GetUserLoveLiness()=0;

	//�ȼ���Ϣ
public:
	//��Ա�ȼ�
	virtual byte GetMemberOrder()=0;
	//����ȼ�
	virtual byte GetMasterOrder()=0;

	//�û�״̬
public:
	//�û�����
	virtual word GetTableID()=0;
	//�û�����
	virtual word GetChairID()=0;
	//�û�״̬
	virtual byte GetUserStatus()=0;

	//��Ϸ��Ϣ
public:
	//������ֵ
	virtual SCORE GetUserScore()=0;
	//�ɼ���ֵ
	virtual SCORE GetUserGrade()=0;
	//������ֵ
	virtual SCORE GetUserInsure()=0;

	//��Ϸ��Ϣ
public:
	//ʤ������
	virtual dword GetUserWinCount()=0;
	//ʧ������
	virtual dword GetUserLostCount()=0;
	//�;�����
	virtual dword GetUserDrawCount()=0;
	//��������
	virtual dword GetUserFleeCount()=0;
	//��Ϸ����
	virtual dword GetUserPlayCount()=0;

	//������Ϣ
public:
	//�û�ʤ��
	virtual float GetUserWinRate()=0;
	//�û�����
	virtual float GetUserLostRate()=0;
	//�û�����
	virtual float GetUserDrawRate()=0;
	//�û�����
	virtual float GetUserFleeRate()=0;

	//�û���ϵ
public:
	//��ȡ��ϵ
	virtual byte GetUserCompanion()=0;
	//���ù�ϵ
	virtual void SetUserCompanion(byte cbCompanion)=0;

	//�û���ע
public:
	//��ȡ��ע
	virtual const char* GetUserNoteInfo()=0;
	//���ñ�ע
	virtual void SetUserNoteInfo(const char* pszUserNote)=0;
};

#endif //_IClientUserItem_H_