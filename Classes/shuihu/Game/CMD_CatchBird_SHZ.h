#ifndef SHZ_CMD_GAME_HEAD_FILE
#define SHZ_CMD_GAME_HEAD_FILE

#include "shuihu/plazz/Plazz_SHZ.h"
#pragma pack(1)
#define UR_GAME_CONTROL					0x20000000L				//��Ϸ�������

//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID_SHZ						203  								//��Ϸ I D
#define GAME_NAME_SHZ					TEXT("ˮ䰴�")	//��Ϸ����

//�������
#define GAME_PLAYER_SHZ					1									//��Ϸ����
#define VERSION_SERVER_SHZ				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT_SHZ				PROCESS_VERSION(6,0,3)				//����汾

#define GAME_SCENE_FREE_SHZ				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY_SHZ				GAME_STATUS_PLAY					//�з�״̬

const float kRevolutionWidth_SHZ  = 1074;
const float kRevolutionHeight_SHZ = 740;
//�����
enum SHZ_enServerSubCmd
{
	enServerSubCmd_Begin_SHZ=100,
	SUB_S_GAMESCENE_SHZ,									//������Ϣ
	SUB_S_GAMESTART_SHZ,									//��Ϸ��ʼ
	SUB_S_UPORDOWNESCORE_SHZ,								//��Ϸ���·�
	SUB_S_ADDSCORE_SHZ,									    //��Ϸ��ע
	SUB_S_TYPESCROLL_SHZ,									//��������
	SUB_S_GETSCORE_SHZ,									    //��û���
	SUB_S_COMEBIBEI_SHZ	,								    //����ȱ�
	SUB_S_BIBEISTART_SHZ,								    //�ȱ����
	SUB_S_MALISTART_SHZ,								    //������ʼ
	SUB_S_MESSAGEINFOR_SHZ,
	SUB_S_BIBEIDATAM_SHZ,                                   //�ȱ�����
	SUB_S_ADDBEI_SHZ,										//�ӱ�
	SUB_S_GAMEMSG_SHZ,										//��Ϸ����
	SUB_S_REWARD_SHZ,
	SUB_S_LEVEL_UP_SHZ,                                     //�û�����
	SUB_S_DISABLE_GAME_SHZ,                                 //������Ϸ
	SUB_S_YA_LINE_SHZ,                                      //����ѹ��
	   
	enServerSubCmd_End_SHZ
};
#define SUB_S_STORAGE_SHZ                   118
#define SUB_S_REFRESH_SHZ					119				//����ˢ��
//С������Ϸ
struct CMD_S_MaLi_SHZ
{
	//��������
	int                            m_GameCount;
	
	//4��ͼ
	int                            m_ThreeTypeId[4];
	
	//תתͼ
	int                            m_TypeIndex;
	
	//��ǰ��
	int                            m_AddScore;
	
	//�Ƿ����Ͽ�ʼС����
	bool                           m_NowBegin;
};
//��Ϣ���ݰ�
struct CMD_S_GameMessage_SHZ
{
	char						   szContent[128];				//��Ϣ����
};

//������Ϣ
struct CMD_S_GameScene_SHZ
{
	//��λ����
	int                            m_CellScore;
	//�Ѿ��ϵķ�
	int                            m_UserScore;

};
//���·���
struct CMD_S_UpOrDownScore_SHZ
{
	//��ǰ�Ϸ�
	int                            m_UserScore;
	
};
//��ע����
struct CMD_S_AddScore_SHZ
{
	//��ǰ��ע
	int                            m_AddScore;
	
};

struct  CMD_S_Addbei_SHZ
{
	int								iaddCount;
};

//Ӯȡ����
struct CMD_S_WinScore_SHZ
{
	//��ǰӮ��
	int                            m_WinScore;
	//��÷�ʽ
	int                            m_GameState;
	//
};
//������Ϣ
struct CMD_S_TypeScroll_SHZ
{
	//��ǰ�Ϸ�
	int                            m_UserScore;
	//��Ӯ����
	int                            m_WinScore;
	//ͼ������
	int                            m_TypeImgid[15];

	bool						   bquanpan;
	//
};
//�ȱ����
struct CMD_S_BiBeiRelult_SHZ
{
	//����һ
	int                            m_S1;
	//���Ӷ�
	int                            m_S2;
	//ͼ������
	int                            m_WinScore;

};
//�ȱ�����
struct CMD_S_BiBeiData_SHZ
{
	//�ȱ�����
	int                            m_BiBeiData[10];

};

//////////////////////////////////////////////////////////////////////////
//�����q
enum SHZ_enClientSubCmd
{
	enClientSubCmd_Begin_SHZ=200,
	SUB_C_GAMESCENE_SHZ,									//������Ϣ
	SUB_C_GAMESTART_SHZ,									//��Ϸ��ʼ
	SUB_C_UPORDOWNESCORE_SHZ,								//��Ϸ���·�
	SUB_C_ADDSCORE_SHZ,									    //��Ϸ��ע
	SUB_C_TYPESCROLL_SHZ,									//��������
	SUB_C_GETSCORE_SHZ,									    //��û���
	SUB_C_COMEBIBEI_SHZ	,								    //����ȱ�
	SUB_C_BIBEISTART_SHZ,								    //�ȱ�����
	SUB_C_MALISTART_SHZ,								    //������ʼ
	SUB_C_REQUESTBEIBEI_SHZ,								//Ҫ��ȱ�
	SUB_C_ADDMAXSCORE_SHZ,									//����ע
	SUB_C_CURSCORE_SHZ,										//��ע
	SUB_C_LINE_SHZ,											//����
	SUB_C_ADDBEI_SHZ,										//�ӱ�
	SUB_C_CURBEI_SHZ,										//����
	SUB_C_REWARD_SHZ,                                       //��ֵ
	enClientSubCmd_End_SHZ
};
 
#define SUB_C_USER_FILTER_SHZ               9  
#define SUB_C_STORAGE_SHZ                   10 
//��ұȱ�
struct SHZ_CMD_C_BiBeiStart_SHZ
{
	//�����С���,0,��,1,С,2,��
	int                             m_MaxOrOther;
	//�ȱ���ʽ,0,�ȱ�,1,��ȱ�,2,ȫ�ȱ�
	int                             m_BiBeiType;
	//
};
//������·�
struct CMD_C_UpOrDownScore_SHZ
{
	//�����ж� 
	bool                            m_bUpOrDown;
	//
};//

//��
struct CMD_C_Line_SHZ
{
	int								iCurLine;

};
//��ȡ�÷�
struct CMD_C_GetScore_SHZ
{
	int                             m_GetType;
	//
};

//BW

struct CMD_C_UserFilter_SHZ {
	dword user_id;
	unsigned char operate_code; // 0 ������ 1 ������ 2 ���
};

struct CMD_C_Stroage_SHZ {
	int iMode;	// 0:��ѯ 1:����
	longlong lValue;
};

struct CMD_S_Stroage_SHZ {
	longlong lValue;
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
