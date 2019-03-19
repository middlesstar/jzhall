#ifndef _SLWH_NET_STRUCT
#define _SLWH_NET_STRUCT
#pragma pack(1)

#include "df/types.h"
#include "df/Define.h"


/////////////////////////////////////////////////////////////
//��¼����

#define MDM_GR_LOGON_SLWH				1									//��¼��Ϣ

//��¼ģʽ
#define SUB_GR_LOGON_USERID_SLWH		1									//I D ��¼
#define SUB_GR_LOGON_MOBILE_SLWH		2									//�ֻ���¼
#define SUB_GR_LOGON_ACCOUNTS_SLWH		3									//�ʻ���¼

//��¼���
#define SUB_GR_LOGON_SUCCESS_SLWH		100									//��¼�ɹ�
#define SUB_GR_LOGON_FAILURE_SLWH		101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH_SLWH		102									//��¼���

//������ʾ
#define SUB_GR_UPDATE_NOTIFY_SLWH		200									//�汾������ʾ


/////////////////////////////////////////////////////////////
//��������

#define MDM_GR_CONFIG_SLWH				2									//������Ϣ

#define SUB_GR_CONFIG_COLUMN_SLWH		100									//�б�����
#define SUB_GR_CONFIG_SERVER_SLWH		101									//��������
#define SUB_GR_CONFIG_PROPERTY_SLWH		102									//��������
#define SUB_GR_CONFIG_FINISH_SLWH		103									//�������
#define SUB_GR_CONFIG_USER_RIGHT_SLWH	104									//���Ȩ��


///////////////////////////////////////////////////////////
//�û�����
#define MDM_GR_USER_SLWH				3


#define SUB_GR_USER_CHAIR_REQ_SLWH		10									//��������
#define SUB_GR_USER_STANDUP_SLWH		4									//��������


#define SUB_GR_USER_ENTER_SLWH			100									//�û�����
#define SUB_GR_USER_STATUS_SLWH			102									//�û�״̬
#define SUB_GR_USER_ERROR_CODE			103									//������Ϣ
#define SUB_GR_GLAD_MESSAGE_SLWH		400									//ϲ����Ϣ


//////////////////////////////////////////////////////////
//״̬����
#define MDM_GR_STATUS_SLWH				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO_SLWH			100									//������Ϣ
#define SUB_GR_TABLE_STATUS_SLWH		101									//����״̬


////////////////////////////////////////////////////////
//�������
#define MDM_GF_FRAME_SLWH				100									//�������

#define SUB_GF_GAME_OPTION_SLWH			1									//��Ϸ����

/////////////////////////////////////////////////////////
//ϵͳ����
#define MDM_CM_SYSTEM_SLWH				1000								//ϵͳ����

#define SUB_CM_SYSTEM_MESSAGE_SLWH		1									//ϵͳ��Ϣ

//��������
#define SMT_CLOSE_ROOM_SLWH				0x0100								//�رշ���
#define SMT_CLOSE_GAME_SLWH				0x0200								//�ر���Ϸ
#define SMT_CLOSE_LINK_SLWH				0x0400								//�ж�����


////////////////////////////////////////////////////////
//��Ϸ����(��Ϸ�߼�)
#define MDM_GF_GAME_SLWH				200									//��Ϸ����


#define SUB_GR_INTOGAMEINGREQ			999									//�����ѯ������Ϣ
#define SUB_GR_INTOGAMEINGRES			1000								//������Ϸͬ��

//��������
//struct AnimalRecorde{
//	//1-�� 2-ׯ 3-�� 
//	int atype;
//	//����˵Ķ�����
//	int animaNumber;
//	//����
//	int animalindex;
//	//���⿪��
//	int tsid;		
//};

//������Ϸ������Ϣ
//struct SLWHGameTableInfo
//{
//	dword							dwGameState;							//��Ϸ״̬ 1:��ע��;2;��Ϸ��ʼ 
//	dword                           dwLeftTime;								//�����ע��;����ʣ�������
//	dword							dwColorIndex[24];						//��һ�ֵ�ɫ������;
//	AnimalRecorde					RecordAnimal[20];						//20���ѳ��¼��
//	dword							dwRateID;								//��һ�ֵı���ID��
//	longlong						lCj;									//�ʽ�
//	dword							onlineNum;								//��������
//	PlayerInfo						PlayerInfo[100];	
//};



#define SUB_GR_VIEWALLNOTES				1001								//��ѯ������ע���
//����ѹ�ֽṹ




#define SUB_GR_OPERATIONNOTES			1002								//������ע
//����ѹ�ֽṹ��
struct CMD_GP_PUTNOTESREQ{
	longlong							iAmimalIndex;							//0 - 14������ 12=ׯ 13=�� ��=14
	longlong							iAmimalGold;							//��ע����Ľ��
};


#define SUB_GR_GAMESENLINSTART			1003								//֪ͨ������Ϣ��
//���ؿ����������ͨ D3Y D4X ��
struct CMD_GP_StartGameRandRes
{
	dword							dwAnimalIndex;							//������������0-23 {1, 0, 1, 3, 0, 1, 0, 2, 0, 2, 1, 3, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 3} ����
	dword                           dwAnimalType;							//������������0-2; ׯ �� 
	dword							dwColorIndex;							//ɫ������
	dword							dwTsRewardId;							//���⿪���� ID 1:û�����⽱ 2:�ʽ� 3:����Ԫ 4:����Ԫ 5:������ 6:������ 7:���˳齱
	
};



#define SUB_G_GAMECAIJINGRES			1005									//�ʽ������Ϣ


#define SUB_GR_GAMESNEXTSTART			1004								//֪ͨ������һ����Ϸ���㲥����
//���ؿ���һ���µ���Ϸ
struct CMD_GP_StartGame{
	dword							dwColorIndex[24];						//��һ�ֵ�ɫ������;
	dword							dwRateID;								//��һ�ֵı���ID��
	longlong						lCj;									//�ʽ�
};


#define SUB_G_GAMESILIANFRES			1006								//������
struct CMD_GP_SiLianFa
{
	dword     				Notes[4];										//�ĸ���������
	dword     				Clors[4];										//�ĸ���ɫ����
	dword                   AnimalType;										//����ׯ����0-2;
};


//DMG������
#define SUB_G_GAMEDAMANGUANRES			1007							//������
struct CMD_GP_DaManGuan
{
	dword     				Notes[12];            //12����������
	dword     				Clors[12];            //12����ɫ����
	dword                   AnimalType;           //����ׯ����0-2;
};

#define SUB_G_GAMELUCHDRAEM				1008							//���˳齱
//XYCJ���˳齱
struct CMD_GP_LuckDraw{
	dword     				Notes[4];            //Oλ��Ϊ1:��ȡ����3λ��
	dword					LuckType;			 //A���䣺 1-Ԫ�� 2-��Ʊ 
	dword					LuckValue;			 //A�����ֵ ����100
};


#define SUB_G_GAMEUSERCHANGE			1009							//��Ҽ�����뿪
//�յ������ݰ�
//struct CMD_GP_UserChange
//{
//	PlayerInfo						pl;									//�����Ϣ	
//	DWORD							type;								//0-�˳� 1-���� ����˳�ֻ��userid��Ч��
//};



#define SUB_G_GAMECHEARNOTES			1010								//�����������Ѻע��Ϣ

#define SUB_G_GAMESENLINSTARTBORD		1011								//֪ͨ����:��ע������Ϣ
struct CMD_GP_PUTNOTESRSP
{
	longlong				gold;											//ѹ��
	dword					userid;											//���ID
	longlong				score;											//����
	dword					animalIndex;									//��ѹ����
};

#define SUB_G_GAMESTARTGAMELIST			1012								//֪ͨ����:�����û��б�ˢ��
//struct CMD_GP_LotterList
//{
//	dword							onlineNum;								//��������
//	PlayerInfo						PlayerInfo[100];	
//};


#define SUB_G_GAMECHEARNOTEALL			1013								//֪ͨ����:���ĳ�����������ע��Ϣ
struct St_UserNotes{
	word noteid;															//ѹ�ְ�ťid 0~14
	longlong gold;															//ѹ�ֽ����
};

//�������ע������
struct CMD_GP_CharNotesInfo{
	dword								userid;								//���id
	dword								size;								//��С
	St_UserNotes						notes[15];							//����ע������
};


#define SUB_G_GAMETICKTIME				9999								//����������
#define SUB_G_GAMETICKTIMERSP			8888								//�յ�������



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//struct



//����ʧ�� ������Ϣ
//struct CMD_GR_RequestFailure
//{
//	LONG							lErrorCode;							//�������
//	TCHAR							szDescribeString[256];				//������Ϣ
//};



//�������������Ϣ
struct SLWHTableInfo
{
	word							wTableID;							//����ID
	byte							bTableLock;							//������
	byte							bPlayStatus;						//���״̬

	word							wTableCount;						//������Ŀ
	word							wChairCount;						//������Ŀ

	word							wServerType;						//��Ϸ����
	dword							dwServerRule;						//�������
};


//SLWH�û���Ϣ
struct SLWHUserInfo
{
	//��������
	dword							dwUserID;							//�û� I D
	dword							dwGameID;							//��Ϸ I D
	dword							dwGroupID;							//���� I D
	char							szNickName[LEN_NICKNAME];			//�û��ǳ�
	char							szGroupName[LEN_GROUP_NAME];		//��������
	char							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//ͷ����Ϣ
	word							wFaceID;							//ͷ������
	dword							dwCustomID;							//�Զ���ʶ

	//�û�����
	byte							cbGender;							//�û��Ա�
	byte							cbMemberOrder;						//��Ա�ȼ�
	byte							cbMasterOrder;						//����ȼ�

	//�û�״̬
	word							wTableID;							//��������
	word							wChairID;							//��������
	byte							cbUserStatus;						//�û�״̬

	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����

	//��Ϸ��Ϣ
	dword							dwWinCount;							//ʤ������
	dword							dwLostCount;						//ʧ������
	dword							dwDrawCount;						//�;�����
	dword							dwFleeCount;						//��������
	dword							dwUserMedal;						//�û�����
	dword							dwExperience;						//�û�����
	dword							lLoveLiness;						//�û�����

	//ʱ����Ϣ
	//tagTimeInfo						TimerInfo;						//ʱ����Ϣ

	//������Ϣ
	byte							cbEnlistStatus;						//����״̬

	//��չ��ʶ
	dword							lExpand;
	dword							dwExpand;

	SLWHUserInfo(){ memset(this, 0, sizeof(SLWHUserInfo)); };
};



#endif