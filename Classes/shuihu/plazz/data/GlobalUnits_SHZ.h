#ifndef SHZ_GLOBAL_UNITS_HEAD_FILE
#define SHZ_GLOBAL_UNITS_HEAD_FILE

#include "shuihu/plazz/Plazz_SHZ.h"

//////////////////////////////////////////////////////////////////////////////////

//ȫ�ֵ�Ԫ
class SHZ_CGlobalUnits
{
	//��������
public:
	bool                            m_bMuteStatuts;                     //����״̬
	bool							m_bAllowSound;						//��������
	bool							m_bAllowBackGroundSound;			//����������
	bool							m_bNotifyUserInOut;					//������Ϣ
	bool							m_bDebarDetestChat;					//������Ϣ

	//�Թ�����
public:
	bool							m_bAllowLookon;						//�����Թ�
	bool							m_bAllowFriendLookon;				//�����Թ�

	//ģʽ����
public:
	byte							m_cbMessageMode;					//��Ϣģʽ

	//��������
public:
	//���캯��
	SHZ_CGlobalUnits();
	//��������
	virtual ~SHZ_CGlobalUnits();

	//�������
public:
	//�������
	bool InitGlobalUnits();
	//ע�����
	bool UnInitGlobalUnits();

	//���ú���
public:
	//���ز���
	void LoadParameter();
	//�������
	void SaveParameter();
	//Ĭ�ϲ���
	void DefaultParameter();

public:
	//������Ϸ��Ч
	bool IsAllowGameSound(){return m_bAllowSound;}
	//��������Ч
	bool IsAllowBackGroundSound(){return m_bAllowBackGroundSound;}
};

//////////////////////////////////////////////////////////////////////////////////
//����˵��
extern SHZ_CGlobalUnits					gGlobalUnits_SHZ;						//ȫ�ֵ�Ԫ

//////////////////////////////////////////////////////////////////////////

#endif