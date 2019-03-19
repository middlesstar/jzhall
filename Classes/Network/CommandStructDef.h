#ifndef __CommandStructDef_H__
#define __CommandStructDef_H__

#include "df/types.h"
#include "df/Define.h"

class PacketAide;

#pragma pack(1)

struct sStructBase
{
	void setString(char* pDes, const char* pSrc)
	{
		if (pDes && pSrc)
		{
			memcpy(pDes, pSrc, strlen(pSrc));
		}
	}
};


/*************************************************
����Э�飺���� �ͳɹ� ���� ��Ϣ
*************************************************/
//�����ɹ�
struct CMD_GP_OperateSuccess
{
	long							lResultCode;						//��������
	char							szDescribeString[128];				//�ɹ���Ϣ

	void fromPack(const PacketAide& packData);
};

//����ʧ��
struct CMD_GP_OperateFailure
{
	long							lResultCode;						//�������
	char							szDescribeString[128];				//������Ϣ

	void fromPack(const PacketAide& packData);
};

//�ο͵�¼
struct tagHallGPVisitorLogon : public sStructBase
{
	tagHallGPVisitorLogon()
	{
		memset(this, 0, sizeof(tagHallGPVisitorLogon));
	}

	//word wFaceID;           //  
	//byte cbValidateFlags;   //

	//ϵͳ��Ϣ
	byte                            cbDeviceType;                       //�豸����
	word							wModuleID;							//ģ���ʶ(KindID)
	dword							dwPlazaVersion;						//�㳡�汾
	char							szMachineID[LEN_MACHINE_ID];		//��������
	byte							cbValidateFlags;			        //У���ʶ(�ֻ�û���ܱ�)
	char							szPassword[LEN_MD5];				//��¼����

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts : public sStructBase
{
	CMD_GP_RegisterAccounts()
	{
		memset(this, 0, sizeof(CMD_GP_RegisterAccounts));
	}

	//ϵͳ��Ϣ
	byte                            cbDeviceType;                       //�豸����
	word							wModuleID;							//ģ���ʶ(KindID)
	dword							dwPlazaVersion;						//�㳡�汾
	char							szMachineID[LEN_MACHINE_ID];		//��������
	char							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����(�ֻ���)

	//�������
	char							szLogonPass[LEN_MD5];				//��¼����
	char							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	word							wFaceID;							//ͷ���ʶ
	byte							cbGender;							//�û��Ա�
	char							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	char							szNickName[LEN_NICKNAME];			//�û��ǳ�
	char							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�(�ֻ�Ŀǰû��)
	char							szPassPortID[LEN_PASS_PORT_ID];		//֤������(�ֻ�Ŀǰû��)
	char							szCompellation[LEN_COMPELLATION];	//��ʵ����(�ֻ�Ŀǰû��)
	byte							cbValidateFlags;			        //У���ʶ(�ֻ�Ŀǰû��)

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess : public sStructBase
{
	CMD_GP_LogonSuccess()
	{
		memset(this, 0, sizeof(CMD_GP_LogonSuccess));
	}

	//��������
	word							wFaceID;							//ͷ���ʶ
	dword							dwUserID;							//�û� I D
	dword							dwGameID;							//��Ϸ I D
	dword							dwGroupID;							//���ű�ʶ
	dword							dwCustomID;							//�Զ���ʶ
	dword							dwUserMedal;						//�û�����
	dword							dwExperience;						//������ֵ
	dword							dwLoveLiness;						//�û�����

	//�û��ɼ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����

	//�û���Ϣ
	byte							cbGender;							//�û��Ա�
	byte							cbMoorMachine;						//��������
	char							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	char							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	char							szGroupName[LEN_GROUP_NAME];		//��������

	//������Ϣ
	byte                            cbShowServerStatus;                 //��ʾ������״̬

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//������¼�ɹ������ֶ�,
struct CMD_GP_LogonSuccess_Hall : public sStructBase
{
	CMD_GP_LogonSuccess_Hall()
	{
		memset(this, 0, sizeof(CMD_GP_LogonSuccess_Hall));
	}

	dword							userid;								//���id
	longlong						llYuanBao;							//���Ԫ��
	dword							dwVipLevel;							//VIP�ȼ�
	dword							dwExpLevel;							//����ȼ�
	dword							dwTicket;							//��Ʊ
	dword  							fudaitime;							//������ʣ��ʱ��
	dword 							ritems[6];							//�̵��׳��б�,0-�׳壬1-�ѳ��

	dword							sumRecharge;						//��ֵ�ܽ��
	dword							istrourist;							//0-�ο�,1-���ο�
	dword 							iscomment;							//0-û������ 1-����

	void fromPack(const PacketAide& packData);
};

//��¼ʧ��
struct CMD_GP_LogonFailure : public sStructBase
{
	CMD_GP_LogonFailure()
	{
		memset(this, 0, sizeof(CMD_GP_LogonFailure));
	}

	long							lResultCode;						//�������
	char							szDescribeString[128];				//������Ϣ

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//��½���
struct CMD_GP_LogonFinish : public sStructBase
{
	CMD_GP_LogonFinish()
	{
		memset(this, 0, sizeof(CMD_GP_LogonFinish));
	}

	word							wIntermitTime;						//�ж�ʱ��
	word							wOnLineCountTime;					//����ʱ��

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//��Ϸ�б�

//��Ϸ����
struct tagHallGameType : public sStructBase
{
	tagHallGameType()
	{
		memset(this, 0, sizeof(tagHallGameType));
	}

	word							wJoinID;							//�ҽ�����
	word							wSortID;							//��������
	word							wTypeID;							//��������
	char							szTypeName[LEN_TYPE];				//��������

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//��Ϸ����
struct tagHallGameKind : public sStructBase
{
	tagHallGameKind()
	{
		memset(this, 0, sizeof(tagHallGameKind));
	}

	word							wTypeID;							//��������
	word							wJoinID;							//�ҽ�����
	word							wSortID;							//��������
	word							wKindID;							//��������
	word							wGameID;							//ģ������
	dword							dwOnLineCount;						//��������
	dword							dwFullCount;						//��Ա����
	char							szKindName[LEN_KIND];				//��Ϸ����
	char							szProcessName[LEN_PROCESS];			//��������

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//��Ϸ�ڵ�
struct tagHallGameNode : public sStructBase
{
	tagHallGameNode()
	{
		memset(this, 0, sizeof(tagHallGameNode));
	}

	word							wKindID;							//��������
	word							wJoinID;							//�ҽ�����
	word							wSortID;							//��������
	word							wNodeID;							//�ڵ�����
	char							szNodeName[LEN_NODE];				//�ڵ�����

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//��������
struct tagHallGamePage : public sStructBase
{
	tagHallGamePage()
	{
		memset(this, 0, sizeof(tagHallGamePage));
	}

	word							wPageID;							//ҳ������
	word							wKindID;							//��������
	word							wNodeID;							//�ڵ�����
	word							wSortID;							//��������
	word							wOperateType;						//��������
	char							szDisplayName[LEN_PAGE];			//��ʾ����

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//��Ϸ����
struct tagHallGameServer : public sStructBase
{
	tagHallGameServer()
	{
		memset(this, 0, sizeof(tagHallGameServer));
	}

	word							wKindID;							//��������
	word							wNodeID;							//�ڵ�����
	word							wSortID;							//��������
	word							wServerID;							//��������
	word							wServerPort;						//����˿�
	dword							dwOnLineCount;						//��������
	dword							dwFullCount;						//��Ա����
	char							szServerAddr[LEN_SERVERADDR];		//��������
	char							szServerName[LEN_SERVER];			//��������

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//I D ��¼
struct CMD_GP_LogonGameID : public sStructBase
{
	CMD_GP_LogonGameID()
	{
		memset(this, 0, sizeof(CMD_GP_LogonGameID));
	}

	//ϵͳ��Ϣ
	byte                            cbDeviceType;                       //�豸����
	word							wModuleID;							//ģ���ʶ(KindID)
	dword							dwPlazaVersion;						//�㳡�汾
	char							szMachineID[LEN_MACHINE_ID];		//��������
	char							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����(�ֻ���)

	//��¼��Ϣ
	dword							dwGameID;							//��Ϸ I D
	char							szPassword[LEN_MD5];				//��¼����
	byte							cbValidateFlags;			        //У���ʶ(�ֻ�û���ܱ�)

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts : public sStructBase
{
	CMD_GP_LogonAccounts()
	{
		memset(this, 0, sizeof(CMD_GP_LogonAccounts));
	}

	//ϵͳ��Ϣ
	byte                            cbDeviceType;                       //�豸����
	word							wModuleID;							//ģ���ʶ(KindID)
	dword							dwPlazaVersion;						//�㳡�汾
	char							szMachineID[LEN_MACHINE_ID];		//��������
	char							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����(�ֻ���)

	//��¼��Ϣ
	char							szPassword[LEN_MD5];				//��¼����
	char							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	byte							cbValidateFlags;			        //У���ʶ(�ֻ�û���ܱ�)
	word							wPlatform;							//ƽ̨0:ԭ��ƽ̨ ����:����ƽ̨ �滢360,�ٶȰ�,ʲô��

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//ҡǮ���ṹ
struct CMD_GP_YaoQianShu_USERINFO
{
	word							nType;									//��������,0-��������,1-�����ȡ���
	dword							dwGameID;								//�û�ID

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//����������ҡǮ��
struct CMD_GP_YaoQianShu_Get
{
	word							nType;									//�ظ�����
	dword							nTime;									//����ʱ
	dword							GetGold;								//�ɵ��������	
	dword							TotelGold;								//�������

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

/*************************************************
�Զ���ϵͳͷ��(��Э���3,СЭ��ţ�����201���ظ���200)
*************************************************/							 
struct CMD_GP_SystemFaceInfo : public sStructBase
{
	CMD_GP_SystemFaceInfo()
	{
		memset(this, 0, sizeof(CMD_GP_SystemFaceInfo));
	}

	word							wFaceID;							//ͷ���ʶ
	dword							dwUserID;							//�û� I D
	char							szPassword[LEN_PASSWORD];			//�û�����
	char							szMachineID[LEN_MACHINE_ID];		//��������

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//���ذ�
struct CMD_GP_UserFaceInfo
{
	word							wFaceID;							//ͷ���ʶ
	dword							dwCustomID;							//�Զ���ʶ

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};


/*************************************************
�Զ����û��ǳ�(��Э���3,СЭ��ţ�����746���ظ���747)
*************************************************/
struct CMD_ST_modiflyuser_req : public sStructBase
{
	CMD_ST_modiflyuser_req()
	{
		memset(this, 0, sizeof(CMD_ST_modiflyuser_req));
	}

	dword     					userId;				//�û�ID��
	char						nickName[LEN_NICKNAME];		//�û��ǳ�		���ΪL"" �򲻸���������Ϣ	 

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};
//���� 
struct CMD_ST_modiflyuser_rsp
{
	dword 						ret;			//0:�ɹ� 1:�ǳƲ��Ϸ�  

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};


/*************************************************
��ѯ��������(��Э���3,СЭ��ţ�����404���ظ���100)
*************************************************/
//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	dword							dwUserID;							//�û� I D
};

//������������
struct CMD_GR_S_UserInsureInfo
{
	byte                            cbActivityGame;                     //��Ϸ����
	word							wRevenueTake;						//˰�ձ���
	word							wRevenueTransfer;					//˰�ձ���
	word							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};


/*************************************************
�����д��(��Э���3,СЭ��ţ�����400���ظ����ɹ�-405��ʧ��-406)
*************************************************/
//������
struct CMD_GP_UserSaveScore : public sStructBase
{
	CMD_GP_UserSaveScore()
	{
		memset(this, 0, sizeof(CMD_GP_UserSaveScore));
	}

	dword							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	char							szMachineID[LEN_MACHINE_ID];		//��������

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//��ȡ���
struct CMD_GP_UserTakeScore : public sStructBase
{
	CMD_GP_UserTakeScore()
	{
		memset(this, 0, sizeof(CMD_GP_UserTakeScore));
	}

	dword							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	char							szPassword[LEN_MD5];				//��������
	char							szMachineID[LEN_MACHINE_ID];		//��������

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//���в����ɹ�
struct CMD_GR_S_UserInsureSuccess : public sStructBase
{
	CMD_GR_S_UserInsureSuccess()
	{
		memset(this, 0, sizeof(CMD_GR_S_UserInsureSuccess));
	}

	dword							dwUserID;							//�û� I D
	SCORE							lUserScore;							//���Ͻ��
	SCORE							lUserInsure;						//���н��
	char							szDescribeString[128];				//������Ϣ

	void fromPack(const PacketAide& packData);
};

//���в���ʧ��
struct CMD_GP_UserInsureFailure : public sStructBase
{
	CMD_GP_UserInsureFailure()
	{
		memset(this, 0, sizeof(CMD_GP_UserInsureFailure));
	}

	long							lResultCode;						//�������
	char							szDescribeString[128];				//������Ϣ

	void fromPack(const PacketAide& packData);
};


/*************************************************
�ȼý�����(��Э���3,СЭ��ţ�����748���ظ���749)
*************************************************/
//�ȼý�����
struct CMD_ST_JIUJIJIN_req
{
	dword						userId;          //�û�ID��		

	void toPack(PacketAide& packData);
};

//�ȼý𷵻�
struct CMD_ST_JIUJIJIN_rsp
{
	dword						gold;          //0:��ȡʧ�ܣ�>0 ��ȡ�ȼý� �ɹ�������Ǯ
	dword						code;		   //0 �ɹ�,1 ��Ҳ����� 2 ������ȡ�ﵽ���� 3 ����1000������ȡ 

	void fromPack(const PacketAide& packData);
};



/*************************************************
ÿ�ճ齱(��Э���3,СЭ��ţ�����738���ظ���739)
*************************************************/
//����ÿ�ճ齱���ݽṹ
struct CMD_GP_DailyLottery_USERINFO
{
	dword							dwGameID;								//�û�ID

	void toPack(PacketAide& packData);
};

//����ÿ�ճ齱���ݽṹ
struct CMD_GP_DailyLottery_Get
{
	dword							dwIndex;								//�н����� ���ղ߻���˳�� 10000Ϊ���ܳ齱�������ѳ������
	word							wType;									//�н����� 0Ϊ��� 1λԪ��
	dword							dwGetGold;								//�õ��������	
	dword							dwGetYuanbao;							//�õ�Ԫ������

	void fromPack(const PacketAide& packData);
};
/*************************************************
ÿ��ǩ��(��Э���3,СЭ��ţ�����750���ظ���751)
*************************************************/
//����ÿ��ǩ��
struct CMD_ST_SignDay_req
{
	dword     					userId;          //�û�ID��		
	dword     					opType;          //1:ǩ�� 2:��ȡ��������
	void toPack(PacketAide& packData);
};

//����ǩ����Ϣ
struct CMD_ST_SignDay_rsp
{
	dword     					gold;            //��õ��Ľ������	�� 
	dword     					opType;          //��������
	dword                       ret;			//0-�ɹ� 1����Ҳ����� 2��������ǩ�� 3��������������
	dword                       signindex;      //ǩ������ 1- 30
	dword                       lxcount;        //����ǩ���Ĵ���

	void fromPack(const PacketAide& packData);
};


/*************************************************
����һ�(��Э���3,СЭ��ţ�����742���ظ���743)
*************************************************/
struct CMD_GP_Hall_Goods_USERINFO : public sStructBase
{
	CMD_GP_Hall_Goods_USERINFO()
	{
		memset(this, 0, sizeof(CMD_GP_Hall_Goods_USERINFO));
	}

	dword							userId;									//�û�ID
	word							wIndex;									//�������Ʒ����
	char							tName[25];								//�û�����		���Ϊ"" �򲻸���������Ϣ
	longlong						llTelNum;								//�û��ֻ���    ���Ϊ-1  �򲻸��µ绰��Ϣ
	char							tAddress[128];							//�ջ���ַ		���Ϊ"" �򲻸��µ�ַ��Ϣ
	longlong						uqq;								    //�û�QQ����

	void toPack(PacketAide& packData);
};

//������Ʒ���ύ�����£��û���Ϣ��Ӧ  
//�̳���Ϣ��Ӧ�ṹ��
struct CMD_GP_Hall_Goods_Get
{
	dword							wgold;									//����Ƕһ���ң��Ͷһ����
	dword							dwErrorCode;							//0=�ɹ� 1=��Ҳ����� 2-��������Ʒ 3-Ԫ������ 4-���ϲ�����

	void fromPack(const PacketAide& packData);
};

/*************************************************
�����ѯ�ҽ���¼(��Э���3,СЭ��ţ�����752���ظ���753)
*************************************************/
typedef struct ItemGoodsDx
{
	ItemGoodsDx()
	{
		memset(this, 0, sizeof(ItemGoodsDx));
	}

	static int getLength()
	{
		ItemGoodsDx item;
		return countarray(item.dtime) * 2 + countarray(item.itemName) * 2 + countarray(item.orderNo) * 2 + sizeof(dword);
	}

	bool isEmpty()
	{
		return strlen(dtime) == 0 && strlen(itemName) == 0 && strlen(orderNo) == 0;
	}

	char							dtime[25];								//�һ�ʱ��
	char							itemName[25];							//��Ʒ����
	char							orderNo[25];							 //�һ�����
	dword							dstate;									//�һ�״̬ 0-����� 1-���ͨ��

	void fromPack(const PacketAide& packData);
}DxRecord;

struct CMD_ST_DxRewardList_req
{
	dword     					userId;            //�û�ID

	void toPack(PacketAide& packData);
};

struct CMD_ST_DxRewardList_rsp
{
	DxRecord						aGoodsList[100];							//�һ���¼

	void fromPack(const PacketAide& packData);
};


/*************************************************
�����ѯ�ҽ���¼(��Э���3,СЭ��ţ�����754���ظ���755)
*************************************************/
struct EmailInfoSt
{
	EmailInfoSt()
	{
		memset(this, 0, sizeof(EmailInfoSt));
	}

	bool isEmpty()
	{
		return strlen(sendusername) == 0 && strlen(sendtime) == 0;
	}

	bool hasAttach()
	{
		return attachtype != 0 || attachtype2 != 0;
	}

	void clear()
	{
		memset(this, 0, sizeof(EmailInfoSt));
	}

	dword                           eid;									 //�ʼ�ID 
	char							sendusername[25];						 //���͵��������
	char							emailtitle[30];							 //�ʼ�����
	char							emialcontent[500];						 //�ʼ�����
	char							sendtime[20];							//����ʱ��
	dword                           isread; 								 //�Ƿ��Ķ� 1-�Ķ� 0-δ�Ķ�
	dword                           attachtype;                              //�������� 1:��� 2:Ԫ�� 3����Ʊ
	dword                           attachnums;                               //��������
	dword                           attachtype2;                              //����2���� 1:��� 2:Ԫ�� 3����Ʊ
	dword                           attachnums2;                               //����2����

	void fromPack(const PacketAide& packData);

};

//�����ѯ�ʼ��б�
struct CMD_ST_GetEmailList_req
{
	dword     					userId;					  //�û�ID

	void toPack(PacketAide& packData);
};

struct CMD_ST_GetEmailList_rsp
{
	EmailInfoSt     				emaillist[20];            //�ʼ��б�

	void fromPack(const PacketAide& packData);
};

/*************************************************
�������ʹ�Ʊ(��Э���3,СЭ��ţ�����758���ظ���759)
*************************************************/
struct CMD_ST_GiveChuanPiaoReq
{
	dword   				userid;              //�Լ����û�ID
	dword   				gameid;              //�Է��ĵ���ϷID
	dword 					nums;				 //��������

	void toPack(PacketAide& packData);
};

struct CMD_ST_GiveChuanPiaoRsp
{
	dword   				ret;              //0;���ͳɹ� 1-��Ҳ�����

	void fromPack(const PacketAide& packData);
};


/*************************************************
����һ�΢����Ʒ(��Э���3,СЭ��ţ�����760���ظ���761)
*************************************************/
struct CMD_ST_GetLiPinReq : public sStructBase
{
	CMD_ST_GetLiPinReq()
	{
		memset(this, 0, sizeof(CMD_ST_GetLiPinReq));
	}

	dword   				userid;              //�Լ����û�ID
	char 					cardnum[9];			 //����

	void toPack(PacketAide& packData);
};

struct CMD_ST_GetLiPinRsp
{
	dword   				ret;              //0:�ɹ� 1-����ȡ 2-�����ڿ���
	dword 					gold;			  //���Ϊret 0 -���ӵĽ��
	dword 					money;			  //���Ϊret  0 -���ӵ�Ԫ��

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_ShareSucessReq
{
	dword   				userid;              //0:�ɹ���������ID
	dword 					sharetype;			 //1-΢������Ȧ 2-QQ����Ȧ 3-΢�ź���

	void toPack(PacketAide& packData);
};

struct CMD_ST_ShareSucessRsp
{
	long   					lGold;              	//����ɹ����ý�ң�����Ϊ0
	dword 					dwCount;			  	//�����ͷ���Ĵ���
	dword 					dwSharetype;			//1-΢������Ȧ 2-QQ����Ȧ 3-΢�ź���

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_GetShareReq : public sStructBase
{
	CMD_ST_GetShareReq()
	{
		memset(this, 0, sizeof(CMD_ST_GetShareReq));
	}

	dword   				userid;              //���ID
	char 					cardnum[9];			 //����

	void toPack(PacketAide& packData);
};

struct CMD_ST_GetShareRsp
{
	dword   				ret;              //0:��ȡ�ɹ� 1-�����벻���� 2-���豸����ȡ 3-���Ѿ���ȡ 4-�Լ�������ȡ�Լ��ķ�����

	dword  					money;            //500Ԫ��
	dword 					gold;             //10000

	void fromPack(const PacketAide& packData);
};


/*************************************************
�����������(��Э���3,СЭ��ţ�����766���ظ���767)
*************************************************/
//����������
struct CMD_ST_FuDaiReq{
	dword 					userid;				//���ID
	dword   				optype;              //1:���������� 2:����������
	dword 					gold;			 	//������� 29999
	dword 					money;				//Ԫ��������50

	void toPack(PacketAide& packData);
};


//�����������������
struct CMD_ST_FuDaiRsp{
	dword   				optype;              //1:���������� 2:����������
	dword 					ret;			 	 //0:�ɹ�  1-ʧ��
	long 					lefttime;			 //ʣ�����

	void fromPack(const PacketAide& packData);
};

//��ֵ�ɹ����������������,HALL_SUB_WEB_RECHARGERSP
struct CMD_Web_RechargeRsp
{
	dword 							userid;	//�û�ID
	dword							productid;//��ƷID
	dword							currgold; //���½��
	dword 							currvip;  //��ǰVIP�ȼ�
	dword 							leftexp;  //��һVIP��Ҫ�ﵽ�Ľ��
	dword 							sumrecharge; //����ܳ�ֵ


	void fromPack(const PacketAide& packData);
};

//������������� HALL_SUB_GP_HALL_SHOUYI_REQ
struct CMD_ST_ShouYiReq
{
	dword   				userid;              //
	dword 					optype;			 //1-��ѯ���� 2-��ȡ���� 3-��ѯ���а�

	void toPack(PacketAide& packData);
};

struct CMD_ST_ShouYiRsp
{
	dword 					optype;			//��������
	dword  					hongbao;        //���ĺ������
	dword 					gold;           //����Ľ��
	dword 					money;          //�����Ԫ��

	void fromPack(const PacketAide& packData);
};


//���нṹ��
typedef struct SyRank : public sStructBase
{
	SyRank()
	{
		memset(this, 0, sizeof(SyRank));
	}

	dword 							userid;
	char							account[30];					//����
	dword                           nums;                           //�������
	dword                           moneys;                         //�����Ԫ������

}SyRankInfo;

//
//�������а񷵻�   HALL_SUB_GP_HALL_SHOUYIRANK_RSP			769
struct CMD_ST_ShouRankYiRsp
{
	SyRankInfo 	 ranks[10];						//��ʾ10������	

	void fromPack(const PacketAide& packData);
};


//������˺�
struct CMD_ST_BindAccountReq : public sStructBase
{
	CMD_ST_BindAccountReq()
	{
		memset(this, 0, sizeof(CMD_ST_BindAccountReq));
	}

	dword   				userid;
	char					szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	char					szPassword[LEN_MD5];				//��¼����
	char					szMachineID[LEN_MACHINE_ID];		//��������

	void toPack(PacketAide& packData);
};

//�����˺Ű󶨷���
struct CMD_ST_BindAccountRsp
{
	dword   				ret;         //0-�󶨳ɹ� 1-��ʧ��  

	void fromPack(const PacketAide& packData);
};

//�������
struct CMD_ST_HaoPingReq
{
	dword   				userid;         //�û�ID

	void toPack(PacketAide& packData);
};

struct CMD_ST_HaoPingRsp
{
	dword   				ret;        //0�ɹ� 1-ʧ��
	dword   				gold;       //��õĽ��

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_ShareCountReq
{
	dword   				userid;         //�û�ID
	dword 					optype;			//1-��ѯ���� 2-��ȡ��������

	void toPack(PacketAide& packData);
};

struct CMD_ST_ShareCountRsp
{
	dword   				count;			//����
	dword 					optype;			//1-��ѯ���� 2-��ȡ��������
	dword 					getcount;		//����ȡ�ı���

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_GetEmailAttReq
{
	dword   				userid;         //���ID
	dword   				euid;			//�ʼ�ID
	dword 					optye;			// 1-��ȡ���� 2-ɾ���ʼ�	

	void toPack(PacketAide& packData);
};

struct CMD_ST_GetEmailAttRsp
{
	dword 					optye;		  // 1-��ȡ���� 2-ɾ���ʼ�	
	dword					ret;		  // 0-�ɹ� 1- ����������

	void fromPack(const PacketAide& packData);
};


//----------------����------------------------
//[SUB_CHATMSG_REQ]
struct CMD_ST_ChatMsgReq : public sStructBase
{
	CMD_ST_ChatMsgReq()
	{
		memset(this, 0, sizeof(CMD_ST_ChatMsgReq));
	}

	dword 					userid; 
	dword 					msgtype;			// 0-���� 1-ˮ䰴� 2-ɭ����� 3-��ɳ��ɳ 4-���� 100:��������
	char					content[100];		//50������

	void toPack(PacketAide& packData);
};

//[SUB_CHATMSG_RSP]
enum eChatChannel
{
	ECC_HALL	=		0,
	ECC_SHUIHU	=		1,
	ECC_SLWH	=		2,
	ECC_SHARK	=		3,
	ECC_BUYU	=		4,

	ECC_WORLD	=		100
};

struct CMD_ST_ChatMsgRsp : public sStructBase
{
	CMD_ST_ChatMsgRsp()
	{
		memset(this, 0, sizeof(CMD_ST_ChatMsgRsp));
	}

	int		ntype;			//1-��Ϣ 2-���
	int	    hid;			//���ID
	int		ret;			//0���ɹ�: 1����Ҳ��㣻2�����췢������ﵽ����
	int		msgtype;		// 0-���� 1-ˮ䰴� 2-ɭ����� 3-��ɳ��ɳ 4-���� 100:��������
	int		userid;			//���ID
	int		viplevel;		//VIP�ȼ�
	int		faceid;			//ͷ��ID
	SCORE	sendtime;		//����ʱ��
	char	uname[32];		//������� 32
	char	content[100];	//50������

	void fromPack(const PacketAide& packData);
};


//�����ͺ��,����Msg_SendMsg_Rsp
struct CMD_ST_ChatHongBaoReq
{
	dword 					userid;
	dword					hongbaoid;			//�����ID��1��200 2��1000 3��5000 4��10000
	dword					msgtype;			// 0-���� 1-ˮ䰴� 2-ɭ����� 3-��ɳ��ɳ 4-���� 100:��������

	void toPack(PacketAide& packData);
};

/// ��ȡ���
struct CMD_ST_ChatGetHongBaoReq
{
	dword 					userid;
	dword					hongbaoid;   //�����ID�� 

	void toPack(PacketAide& packData);
};

/// ��ȡ�˽ṹ��
struct StHong
{
	StHong()
	{
		memset(this, 0, sizeof(StHong));
	}

	int userid;			//0�����ID
	int gold;			//��ȡ�Ľ��
	int viplevel;		//vip�ȼ�
	char uname[32];		//������� 32

	void fromPack(const PacketAide& packData);
};


/// ��ȡ�������
struct CMD_ST_ChatGetHongBaoRsp
{
	int ret;			//0���ɹ� 1-�����ڵĺ�� 2-�Ѿ���ȡ�� 3-�������ȡ��
	int gold;			//��ȡ��ҵ����� 0���ѱ���ȡ����
	StHong hlist[8];	//��ȡ�ĺ���б�

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_ReplaceReq
{
	dword 					userid;
	dword					roomid;   //����ID 0-���� 1-ˮ䰴� 2-ɭ����� 3-��ɳ��ɳ 4-���� 

	void toPack(PacketAide& packData);
};

struct CMD_ST_NetFlushServerReq
{
	dword 					userid;	 //0:���ID
	dword 					roomid;   // 0-���� 1-ˮ䰴� 2-ɭ����� 3-��ɳ��ɳ 4-���� 100:��������

	void toPack(PacketAide& packData);
};

struct CMD_ST_NetFlushUserRsp
{
	dword 					money;		//���Ԫ��
	dword 					cpvalue;	//��Ʊֵ

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_HeartReq
{
	dword 					userid;

	void toPack(PacketAide& packData);
};


//! ���а�����
struct CMD_GP_GET_RANK
{
	dword		dwUserID;	//! �û�ID
	byte		dwType;		//! ��Ӧ���а�����

	void toPack(PacketAide& packData);
};

//���а���
typedef struct RankInfoSt
{
	dword		dwRank;						//����
	dword		dwUserID;					//�û�ID  [�յ�-1��ʾ�б��ͽ���]
	char		szTitle[LEN_NICKNAME];		//���н���
	longlong	lScore;						//��ֵ[��Դ����]
	char		szUserName[LEN_NICKNAME];	//�û�����
	dword		dwVip;						//�û�VIP�ȼ�

	void fromPack(const PacketAide& packData);

}RankInfo;

//! ���а�������
struct CMD_GP_GET_RANK_RESULT
{
	RankInfo	ranklist[31];					//���а��б�

	void fromPack(const PacketAide& packData);
};

/*------------todosl��Ϸǿ�Ƹ���------------------*/
struct CMD_GP_UpdateNotify
{
	byte                    cbMustUpdate;                   // ǿ������
	byte                    cbAdviceUpdate;                 //��������
	dword                   dwCurrentVersion;	            //��ǰ�汾
	char                    szUpdateUrl[128];			    //���µ�ַ

	void  fromPack(const PacketAide& packData);
};

//struct CMD_ST_HeartRsp
//{
//
//}

#pragma pack()


#endif









