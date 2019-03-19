#ifndef  _sharkStruct_H_
#define  _sharkStruct_H_
#pragma pack(1)
#include "df/types.h"
#include "df/Define.h"


//服务器状态
enum gameState
{
	STAKE = 1,     //押分状态
	BATTLE = 2,     //比赛状态
	GAME_COMMAND_MAX
};
 //开奖类型
enum resultKind
{
	Invalid = -1,

	GENERALRESULT     = 1,	     //普通开奖  
	SILVERRESULT = 2,                 //银鲨开奖
	SHARKRESULT = 3,		   //金鲨鱼开奖											
	COLORRESULT  = 4,		  //彩鲨鱼开奖						

    RESULT_KIND_MAX,       
};

//动物索引
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

//玩家结构
struct UserInfo_Shark{
	int			userid;
	char		nickname[LEN_NICKNAME];
	int			viplevel;
	int			gold;
	int			headimgid;
	SCORE		jet;						//玩家当前压分				
	SCORE		winscore;					//玩家输赢

	/*

	//排序规则 如果赢分一样VIP高排前面 (从大到小)
	bool operator > (const UserInfo_Shark &m)const
	{
		bool b = false;
		if (winscore > m.winscore) b = true;
		else if (winscore == m.winscore && viplevel > m.viplevel) b = true;
		return b;
	}

	//排序规则 如果vip一样 分数高的排前面 （从大到小排序）
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


//彩金开奖
struct SharkCJ
{
	int	 num;							//有几个玩家中奖
	dword userid[3];					//3个玩家的USERID
	char name[3][LEN_NICKNAME];			//3个玩家的名字
	longlong	score[3];				//3个玩家的获奖分数

	SharkCJ(){ memset(this, 0, sizeof(SharkCJ)); };
};

//压分信息
struct StakeInfo
{
	dword index;					//压分当前档
	dword dwOneLimit;				//每个按钮压分最高限制
	dword dwStake[5];					//压分档位


	StakeInfo(){ memset(this, 0, sizeof(StakeInfo)); };
};

//总压分信息
struct AllJetInfo
{
	SCORE lTotelBet[12];			//12个总压分信息
	SCORE lAllBet;					//总分

	AllJetInfo(){ memset(this, 0, sizeof(AllJetInfo)); };
};


//房间信息
struct RoomInfo_Shark
{
	word		wType;									//房间类型 0兔房 1猫房 2狮房
	dword		dwLimit;								//房间进入分限制
	dword		dwRateID;								//这一轮的倍率ID；
	longlong	lCJ;									//彩金
	dword		dwOnline;								//在线人数
	StakeInfo		stakeinfo;								//压分信息
	std::vector<UserInfo_Shark>	vec;						//玩家信息列表
	dword		 JsRate;						     	//当前的金鲨倍率
	AllJetInfo	alljetinfo;								//所有压分总信息


	RoomInfo_Shark()
	{
		dwRateID = 0;
		wType = 0;
		lCJ = 0;
		dwOnline = 0;
	}
};

//返回的结果
struct OpenResult
{
	dword resultIndex[3];
	dword aType;
	dword aValue;
	OpenResult(){ memset(this, 0, sizeof(OpenResult)); };
};


//自己的押注信息
struct ownStakeInfo
{
	int   ownStake[12];
	int   ownTotal;
	//int   radeIndex[12];
	ownStakeInfo(){ memset(this, 0, sizeof(ownStakeInfo)); };
};

#endif // _sharkStruct_H_

