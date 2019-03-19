#ifndef __CommandDef_H__
#define __CommandDef_H__

#define MB_VALIDATE_FLAGS                   0x01        //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS              0x02        //Ч��Ͱ汾

//��Э���
enum eCommandType
{
	HALL_MDM_GP_LOGON		=		1,
	HALL_MDM_GP_SERVER_LIST =		2,

	HALL_MDM_GP_SERVICE		=		3,

	HALL_MDM_GP_WEBOP		=		300,

	COMMAND_MAX
};

//СЭ���
enum eSubCommandType
{
	HALL_SUB_GP_SERVERINFO = 1,				   //����������б�type��kind
	HALL_SUB_GP_GAMESERVER = 2,				   //���󷿼���Ϣgameserver

	HALL_DBR_GP_LOGON_ACCOUNTS = 2,
	HALL_SUB_GP_REGISTER_ACCOUNTS = 3,
	HALL_SUB_GP_VISITOR_ACCOUNT = 5,           //

	HALL_SUB_GR_LOGON_QUICK = 7,				//�ο͵�¼

	HALL_SUB_GP_LOGON_SUCCESS = 100,
	HALL_SUB_GP_LIST_TYPE = 100,
	HALL_SUB_GP_LIST_KIND = 101,
	HALL_SUB_GP_LOGON_FAILURE = 101,
	HALL_SUB_GP_LIST_NODE = 102,
	HALL_SUB_GP_LOGON_FINISH = 102,
	HALL_SUB_GP_LIST_PAGE = 103,
	HALL_SUB_GP_LIST_SERVER = 104,

	HALL_SUB_GP_USER_FACE_INFO = 200,	//�������ظ��޸�ͷ��
	HALL_SUB_GP_SYSTEM_FACE_INFO = 201,		//�����޸�ͷ��

	HALL_SUB_WEB_RECHARGERSP = 253,			//��ֵ�ɹ��󷵻صĻظ�

	HALL_SUB_GP_GET_RANK = 267,				//�������а�
	HALL_SUB_GP_GET_RANK_RESULT = 268,

	HALL_SUB_GP_HALL_INFO = 300,		//��¼ʱ�򷵻ش����Ķ�����Ϣ

	HALL_SUB_GP_QUERY_INSURE_INFO = 404,	//��ѯ����
	HALL_SUB_GR_USER_INSURE_INFO = 100,		//��������

	HALL_SUB_GP_USER_SAVE_SCORE = 400,			//������
	HALL_SUB_GP_USER_TAKE_SCORE = 401,			//ȡ�����
	HALL_SUB_GR_USER_INSURE_SUCCESS = 405,		//���ɹ�
	HALL_SUB_GP_USER_INSURE_FAILURE = 406,		//���ʧ��

	HALL_SUB_GP_MONEYTREE_REQ = 736,		//ҡǮ������
	HALL_SUB_GP_MONEYTREE_RSP = 737,		//ҡǮ���ظ�

	HALL_SUB_GP_HALL_MODIFLYUSER_REQ = 746,		//�����ǳ�����
	HALL_SUB_GP_HALL_MODIFLYUSER_RSP = 747,		//�����ǳƻظ�

	HALL_SUB_GP_DAILYLOTTERY_GET = 738,         //����ÿ�ճ齱
	HALL_SUB_GP_DAILYLOTTERY_GET_RESULT = 739,	//���س齱���

	HALL_SUB_GP_HALLGOODS_GET = 742,			//����һ�
	HALL_SUB_GP_HALLGOODS_GET_RESULT = 743,		//���ضһ���Ϣ

	HALL_SUB_GP_HALL_JIUJIJIN_REQ = 748,		//�ȼý�����
	HALL_SUB_GP_HALL_JIUJIJIN_RSP = 749,		//�ȼý𷵻�

	HALL_SUB_GP_HALL_SIGNDAY_REQ = 750,        //ÿ��ǩ������
	HALL_SUB_GP_HALL_SIGNDAY_RSP = 751,        //ÿ��ǩ������

	HALL_SUB_GP_HALL_DXREARD_REQ = 752,			//�����ѯ�ҽ���¼
	HALL_SUB_GP_HALL_DXREARD_RSP = 753,			//��ѯ�ҽ���¼����

	HALL_SUB_GP_HALL_GETEAMILL_REQ = 754,		//�����ѯ�ʼ��б�
	HALL_SUB_GP_HALL_GETEAMILL_RSP = 755,		//�����ʼ��б�

	HALL_SUB_GP_HALL_GIVECHUANPIAO_REQ = 758,	//�������ʹ�Ʊ
	HALL_SUB_GP_HALL_GIVECHUANPIAO_RSP = 759,	//�������ʹ�Ʊ���

	HALL_SUB_GP_HALL_GETLIPIN_REQ = 760,		//������Ʒ���ŵõ���Ʒ
	HALL_SUB_GP_HALL_GETLIPIN_RSP = 761,

	HALL_SUB_GP_HALL_SHARESUCESS_REQ = 762,		//�������ɹ�����ã�
	HALL_SUB_GP_HALL_SHARESUCESS_RSP = 763,		//����󷵻�

	HALL_SUB_GP_HALL_GETSHARE_REQ = 764,		//��ȡ�������
	HALL_SUB_GP_HALL_GETSHARE_RSP = 765,		//����󷵻�

	HALL_SUB_GP_GRABRED_REQ = 766,			//� ����������
	HALL_SUB_GP_GRABRED_rsp = 767,			//� ������ ����

	HALL_SUB_GP_HALL_SHOUYI_REQ = 768,			//�������������
	HALL_SUB_GP_HALL_SHOUYI_RSP = 769,			//��������������
	HALL_SUB_GP_HALL_SHOUYIRANK_RSP = 770,		//�������а񷵻�

	HALL_SUB_GP_HALL_BINDACCOUNT_REQ = 771,		//������˺�
	HALL_SUB_GP_HALL_BINDACCOUNT_RSP = 772,		//�����˺Ű󶨷���

	HALL_SUB_GP_HALL_HAOPING_REQ = 773,			//�������
	HALL_SUB_GP_HALL_HAOPING_RSP = 774,			//�����������

	HALL_SUB_GP_HALL_SHAREREDCOUNT_REQ = 775,	//�����ѯ�������
	HALL_SUB_GP_HALL_SHAREREDCOUNT_RSP = 776,	//���ط������

	HALL_SUB_GP_HALL_GETEMAILATT_REQ = 777,		//��ȡ�ʼ��и�������
	HALL_SUB_GP_HALL_GETEMAILATT_RSP = 778,		//��ȡ�ʼ��и�������

	HALL_SUB_CHATMSG_REQ = 779,					//��������
	HALL_SUB_CHATMSG_RSP = 780,					//����ظ�

	HALL_SUB_CHATHONGBAO_REQ = 781,				//�����ͺ��,����������780

	HALL_SUB_CHATGETHONGBAO_REQ = 782,			//������ȡ���
	HALL_SUB_CHATGETHONGBAO_RSP = 783,			//��ȡ�������

	HALL_SUB_REPLACEROOM_REQ = 784,				//�л�����,�������޷���

	HALL_SUB_NETFLUSHUSER_REQ = 785,			//����ˢ���û���ϢЭ��
	HALL_SUB_NETFLUSHUSER_RSP = 786,			//����

	HALL_SUB_HEART_REQ = 787,				//����Э��
	HALL_SUB_HEART_RSP = 788,				//����Э�鷵��

	HALL_SUB_GP_OPERATE_SUCCESS = 900,		//�����ɹ�
	HALL_SUB_GP_OPERATE_FAILURE = 901,		//����ʧ��
	

	HALL_SUB_COMMAND_MAX
};



//todosl

#define      SUB_GP_UPDATE_NOTIFY		200									//������ʾ









/*-------------------------ˮ䰴�---------------------*/

//СЭ���
enum eSubCommandType_SHZ{
	SUB_S_LOTTERYEXP_SHZ = 121,
	SUB_C_LOTTERYEXP_SHZ = 219,
};
#endif

