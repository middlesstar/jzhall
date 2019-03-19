#ifndef SHZ_OPTION_PARAMETER_HEAD_FILE
#define SHZ_OPTION_PARAMETER_HEAD_FILE

#include "shuihu/plazz/Plazz_SHZ.h"
#include <map>

//������Ϣ
#define REG_GOBAL_OPTION_SHZ			"GolbalOption"				//ȫ����Ϣ


//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����ģʽ
#define INVITE_MODE_ALL_SHZ				0									//ȫ����ʾ
#define INVITE_MODE_NONE_SHZ			1									//ȫ����ʾ
#define INVITE_MODE_FRIEND_SHZ			2									//������ʾ

//��Ϣģʽ
#define MESSAGE_MODE_ALL_SHZ			0									//ȫ����ʾ
#define MESSAGE_MODE_DETEST_SHZ			1									//�������
#define MESSAGE_MODE_FRIEND_SHZ			2									//���ź���
#define MESSAGE_MODE_NONE_SHZ			3									//ȫ����ʾ

//���涯��
#define ACTION_ORIENTATION_SHZ			0									//��λ�û�
#define ACTION_SEND_WHISPER_SHZ			1									//����˽��
#define ACTION_SHOW_USER_INFO_SHZ		2									//�û���Ϣ
#define ACTION_SEARCH_TABLE_SHZ			3									//Ѱ��λ��
#define ACTION_SHOW_SEARCH_DLG_SHZ		4									//���ҽ���

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class SHZCParameterGame
{
	//��Ԫ����
	friend class SHZCParameterGlobal;

	//ʤ������
public:
	word							m_wMinWinRate;						//���ʤ��
	bool							m_bLimitWinRate;					//����ʤ��

	//��������
public:
	word							m_wMaxFleeRate;						//�������
	bool							m_bLimitFleeRate;					//���ƶ���

	//��������
public:
	long							m_lMaxGameScore;					//��߷��� 
	long							m_lMinGameScore;					//��ͷ���
	bool							m_bLimitGameScore;					//���Ʒ���

	//���Ա���
protected:
	char							m_szRegKeyName[16];					//ע������

	//��������
public:
	//���캯��
	SHZCParameterGame();
	//��������
	virtual ~SHZCParameterGame();

	//���ܺ���
public:
	//���ز���
	void LoadParameter();
	//�������
	void SaveParameter();
	//Ĭ�ϲ���
	void DefaultParameter();

	//���ú���
protected:
	//���ò���
	bool InitParameter(const char* pszProcessName);
};

//////////////////////////////////////////////////////////////////////////////////

//�������
class SHZCParameterServer
{
	//���ñ���
public:
	bool							m_bTakePassword;					//�����־
	char							m_szPassword[LEN_PASSWORD];			//��������

	//��������
public:
	//���캯��
	SHZCParameterServer();
	//��������
	virtual ~SHZCParameterServer();

	//���ú���
public:
	//Ĭ�ϲ���
	void DefaultParameter();
};

//////////////////////////////////////////////////////////////////////////////////
typedef std::map<word,SHZCParameterGame *> CParameterGameMap_SHZ;
typedef std::map<word,SHZCParameterServer *> CParameterServerMap_SHZ;

//ȫ�ֲ���
class SHZCParameterGlobal
{
	//��̬����
public:
	//��ȡ����
	static SHZCParameterGlobal* shared();
	static void purge();

	//��¼����
public:
	bool							m_bLogonAuto;						//�Զ���¼
	bool							m_bWriteCookie;						//д�����
	bool							m_bRemberPassword;					//��ס����

	//ģʽ����
public:
	byte							m_cbInviteMode;						//����ģʽ
	byte							m_cbMessageMode;					//��Ϣģʽ
	byte							m_cbActionHitAutoJoin;				//�Զ�����
	byte							m_cbActionLeftDoubleList;			//˫���б�

	//ʱ������
public:
	word							m_wIntermitTime;					//�ж�ʱ��
	word							m_wOnLineCountTime;					//����ʱ��

	//��������
public:
	bool							m_bLimitDetest;						//�������
	bool							m_bLimitSameIP;						//���Ƶ�ַ
	bool							m_bNotifyUserInOut;					//������Ϣ
	bool							m_bNotifyFriendCome;				//������ʾ

	//ϵͳ����
public:
	bool							m_bAllowSound;						//��������
	bool							m_bAllowBackMusic;					//����������
	bool							m_bAutoSitDown;						//�Զ�����
	bool							m_bAutoShowWhisper;					//��ʾ˽��
	bool							m_bSaveWhisperChat;					//����˽��
	bool							m_bSendWhisperByEnter;				//�س�����

	//�������
protected:
	CParameterGameMap_SHZ				m_ParameterGameMap;					//��Ϸ����
	CParameterServerMap_SHZ				m_ParameterServerMap;				//��������

	//��ʾ����
public:
	bool                            m_bShowServerStatus;                //��ʾ������״̬

	//��������
public:
	//���캯��
	SHZCParameterGlobal();
	//��������
	virtual ~SHZCParameterGlobal();

	//���ú���
public:
	//���ز���
	void LoadParameter();
	//�������
	void SaveParameter();
	//Ĭ�ϲ���
	void DefaultParameter();

	//��Ϸ����
public:
	//��Ϸ����
	SHZCParameterGame * GetParameterGame(tagGameKind * pGameKind);
	//��������
	SHZCParameterServer * GetParameterServer(tagSHZ_GameServer * pGameServer);
};

//////////////////////////////////////////////////////////////////////////////////

#endif