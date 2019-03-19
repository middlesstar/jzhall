#ifndef  _sharkStruct_H_
#define  _sharkStruct_H_
#pragma pack(1)
#include "df/types.h"
#include "df/Define.h"


//������״̬
enum gameState
{
	STAKE = 1,     //Ѻ��״̬
	BATTLE = 2,     //����״̬
	GAME_COMMAND_MAX
};
 //��������
enum resultKind
{
	Invalid = -1,

	GENERALRESULT     = 1,	     //��ͨ����  
	SILVERRESULT = 2,                 //���迪��
	SHARKRESULT = 3,		   //�����㿪��											
	COLORRESULT  = 4,		  //�����㿪��						

    RESULT_KIND_MAX,       
};

//��������
enum animalKind
{
	ANIMAL_GOLD = 1,
	ANIMAL_SILVER = 2,
	ANIMAL_SWALLOW = 3,
	ANIMAL_DOVE = 4,
	ANIMAL_PEACOCK = 5,
	ANIMAL_HAWK = 6,
	ANIMAL_LION = 7,
	ANIMAL_PANDA = 8,
	ANIMAL_MONKEY = 9,
	ANIMAL_RABBIT = 10,

	ANIMAL_KIND_MAX,
};

//��ҽṹ
struct UserInfo_Shark{
	int			userid;
	char		nickname[LEN_NICKNAME];
	int			viplevel;
	int			gold;
	int			headimgid;
	SCORE		jet;						//��ҵ�ǰѹ��				
	SCORE		winscore;					//�����Ӯ

	/*

	//������� ���Ӯ��һ��VIP����ǰ�� (�Ӵ�С)
	bool operator > (const UserInfo_Shark &m)const
	{
		bool b = false;
		if (winscore > m.winscore) b = true;
		else if (winscore == m.winscore && viplevel > m.viplevel) b = true;
		return b;
	}

	//������� ���vipһ�� �����ߵ���ǰ�� ���Ӵ�С����
	bool operator < (const UserInfo_Shark &m)const
	{
		bool b = true;
		if (viplevel < m.viplevel) b = false;
		else if (viplevel == m.viplevel && gold < m.gold) b = false;
		return b;
	}
	*/

	static bool LessComp(const UserInfo_Shark& left, const UserInfo_Shark& right)
	{
		if (left.viplevel < right.viplevel)
		{
			return false;
		}
		else if (left.viplevel == right.viplevel && left.gold <= right.gold)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	static bool GreatComp(const UserInfo_Shark& left, const UserInfo_Shark& right)
	{
		if (left.winscore < right.winscore)
		{
			return false;
		}
		else if (left.winscore == right.winscore && left.viplevel <= right.viplevel)
		{
			return false;
		}
		else
		{
			return true;
		}
	}


	UserInfo_Shark(){ memset(this, 0, sizeof(UserInfo_Shark)); };
};


//�ʽ𿪽�
struct SharkCJ
{
	int	 num;							//�м�������н�
	dword userid[3];					//3����ҵ�USERID
	char name[3][LEN_NICKNAME];			//3����ҵ�����
	longlong	score[3];				//3����ҵĻ񽱷���

	SharkCJ(){ memset(this, 0, sizeof(SharkCJ)); };
};

//ѹ����Ϣ
struct StakeInfo
{
	dword index;					//ѹ�ֵ�ǰ��
	dword dwOneLimit;				//ÿ����ťѹ���������
	dword dwStake[5];					//ѹ�ֵ�λ


	StakeInfo(){ memset(this, 0, sizeof(StakeInfo)); };
};

//��ѹ����Ϣ
struct AllJetInfo
{
	SCORE lTotelBet[12];			//12����ѹ����Ϣ
	SCORE lAllBet;					//�ܷ�

	AllJetInfo(){ memset(this, 0, sizeof(AllJetInfo)); };
};


//������Ϣ
struct RoomInfo_Shark
{
	word		wType;									//�������� 0�÷� 1è�� 2ʨ��
	dword		dwLimit;								//������������
	dword		dwRateID;								//��һ�ֵı���ID��
	longlong	lCJ;									//�ʽ�
	dword		dwOnline;								//��������
	StakeInfo		stakeinfo;								//ѹ����Ϣ
	std::vector<UserInfo_Shark>	vec;						//�����Ϣ�б�
	dword		 JsRate;						     	//��ǰ�Ľ��豶��
	AllJetInfo	alljetinfo;								//����ѹ������Ϣ


	RoomInfo_Shark()
	{
		dwRateID = 0;
		wType = 0;
		lCJ = 0;
		dwOnline = 0;
	}
};

//���صĽ��
struct OpenResult
{
	dword resultIndex[3];
	dword aType;
	dword aValue;
	OpenResult(){ memset(this, 0, sizeof(OpenResult)); };
};


//�Լ���Ѻע��Ϣ
struct ownStakeInfo
{
	int   ownStake[12];
	int   ownTotal;
	//int   radeIndex[12];
	ownStakeInfo(){ memset(this, 0, sizeof(ownStakeInfo)); };
};

#endif // _sharkStruct_H_

