#ifndef  _sharkNetStruct_H_
#define  _sharkNetStruct_H_
#pragma pack(1)
#include "df/types.h"
#include "df/Define.h"

/////////////////////////////////////////////////////////////
//��¼����

#define MDM_GR_LOGON_SHARK				1									//��¼��Ϣ

//��¼ģʽ
#define SUB_GR_LOGON_USERID_SHARK		1									//I D ��¼
#define SUB_GR_LOGON_MOBILE_SHARK		2									//�ֻ���¼
#define SUB_GR_LOGON_ACCOUNTS_SHARK		3									//�ʻ���¼

//��¼���
#define SUB_GR_LOGON_SUCCESS_SHARK		100									//��¼�ɹ�
#define SUB_GR_LOGON_FAILURE_SHARK		101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH_SHARK		102									//��¼���

//������ʾ
#define SUB_GR_UPDATE_NOTIFY_SHARK		200									//�汾������ʾ


/////////////////////////////////////////////////////////////
//��������

#define MDM_GR_CONFIG_SHARK				2									//������Ϣ

#define SUB_GR_CONFIG_COLUMN_SHARK		100									//�б�����
#define SUB_GR_CONFIG_SERVER_SHARK		101									//��������
#define SUB_GR_CONFIG_PROPERTY_SHARK		102									//��������
#define SUB_GR_CONFIG_FINISH_SHARK		103									//�������
#define SUB_GR_CONFIG_USER_RIGHT_SHARK	104									//���Ȩ��


///////////////////////////////////////////////////////////
//�û�����
#define MDM_GR_USER_SHARK				3


#define SUB_GR_USER_CHAIR_REQ_SHARK		10									//��������
#define SUB_GR_USER_STANDUP_SHARK		4									//��������


#define SUB_GR_USER_ENTER_SHARK			100									//�û�����
#define SUB_GR_USER_STATUS_SHARK			102									//�û�״̬
#define SUB_GR_USER_ERROR_CODE			103									//������Ϣ
#define SUB_GR_GLAD_MESSAGE_SHARK		400									//ϲ����Ϣ


//////////////////////////////////////////////////////////
//״̬����
#define MDM_GR_STATUS_SHARK				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO_SHARK			100									//������Ϣ
#define SUB_GR_TABLE_STATUS_SHARK		101									//����״̬


////////////////////////////////////////////////////////
//�������
#define MDM_GF_FRAME_SHARK				100									//�������

#define SUB_GF_GAME_OPTION_SHARK			1									//��Ϸ����

/////////////////////////////////////////////////////////
//ϵͳ����
#define MDM_CM_SYSTEM_SHARK				1000								//ϵͳ����

#define SUB_CM_SYSTEM_MESSAGE_SHARK		1									//ϵͳ��Ϣ

//��������
#define SMT_CLOSE_ROOM_SHARK				0x0100								//�رշ���
#define SMT_CLOSE_GAME_SHARK				0x0200								//�ر���Ϸ
#define SMT_CLOSE_LINK_SHARK				0x0400								//�ж�����


////////////////////////////////////////////////////////
//��Ϸ����(��Ϸ�߼�)
#define MDM_GF_GAME_SHARK				200									//��Ϸ����




//�������������Ϣ
struct SharkTableInfo
{
	word							wTableID;							//����ID
	byte							bTableLock;							//������
	byte							bPlayStatus;						//���״̬

	word							wTableCount;						//������Ŀ
	word							wChairCount;						//������Ŀ

	word							wServerType;						//��Ϸ����
	dword							dwServerRule;						//�������
};


//Shark�û���Ϣ
struct SharkUserInfo
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

	SharkUserInfo(){ memset(this, 0, sizeof(SharkUserInfo)); };
};

#define  SUB_G_INTOGAMEINGREQ			999									//�����ѯ������Ϣ
#define  SUB_G_INTOGAMEINGRES			1000								//������Ϸͬ��

#define  SUB_G_OPERATIONNOTES             1002				 //������ע
//������עЭ��
struct CMD_GP_PUTNOTESREQU{
	int							AmimalIndex;                //0 - 12������ 
	longlong				    AmimalGold;                 //��ע����Ľ��
};

#define  SUB_G_GAMESENLINSTART          1003
struct CMD_GP_StartGameRandRsp
{
	dword                           AnimalIndex[3];            //֪ͨ������������0����ͨ�� ����ע��� 
	dword                           Atype;      //A�������� 1-Ԫ�� 2-��Ʊ 3-���
	dword                           Avalue;     //A�����ֵ
};
#define SUB_G_GAMESNEXTSTART           1004                    //֪ͨ������һ����Ϸ���㲥����
struct CMD_GP_StartNextGame{ 

	dword				JsRate;				//��һ�ֵĽ�ɳ���ʣ�
	dword				RateID;				//��һ�ֵı���ID
	longlong			CaiJinValue;		//��һ�ֵĲʽ�ֵ
};

#define SUB_G_GAMECAIJINGRES			1005									//�ʽ������Ϣ

#define SUB_G_GAMEUSERCHANGE			1009							//��Ҽ�����뿪

#define    SUB_G_GAMECHEARNOTES            1010				 //�����������Ѻע��Ϣ

#define SUB_G_GAMESENLINSTARTBORD          1011			 //֪ͨ����:��ע������Ϣ
//������ע��Ϣ;֪ͨ�������������
struct CMD_GP_PUTNOTESREC{
	longlong						gold;                     //���ӽ����ֵ
	dword							userid;					  //��ע���ID
	longlong						currgold;				  //������Ͻ��
	dword					        AnimalIndex;									//��ѹ����
};

#define SUB_G_GAMESTARTGAMELIST			1012								//֪ͨ����:�����û��б�ˢ��

#define SUB_G_GAMECHEARNOTEALL			1013								//֪ͨ����:���ĳ�����������ע��Ϣ
struct St_UserNote{
	word noteid;															//ѹ�ְ�ťid 0~11
	longlong gold;															//ѹ�ֽ����
};

//�������ע������
struct CMD_GP_CharNotesInfor{
	dword								userid;								//���id
	dword								size;								//��С
	St_UserNote						notes[15];							//����ע������
};


#define SUB_G_GAMETICKTIME				9999								//����������
#define SUB_G_GAMETICKTIMERSP			8888								//�յ�������


#endif // _sharkNetStruct_H_

