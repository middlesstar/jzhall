#ifndef _SLWH_STRUCT
#define _SLWH_STRUCT
#include "cocos2d.h"
USING_NS_CC;
#include "df/types.h"
#include "df/Define.h"

//服务器状态
enum enumServerState
{
	//压分界面
	_Jet = 1,
	//开奖
	_Open = 2
};


////压分档
//enum enumJetAdd{
//	_LOW = 100,															//100分档
//	_MID = 1000,														//1000分档
//	_HIGH = 10000,														//10000分档
//};


enum enumSpecialRs
{
	InvalidSpecial	= -1,

	Normal	= 1,								//普通开奖    
	CJ		= 2,								//彩金			1/5000
	D3Y		= 3,								//大三元			1%
	D4X		= 4,								//大四喜			1%
	_4LF	= 5,								//4连发			1%  
	DMG		= 6,								//大满贯			1/3000
	XYCJ	= 7,								//幸运抽奖		1/1000

	SpecialRsMax,
};


enum enumAnimalType
{
	InvalidAniType = -1,

	Animal0 = 0,						//动物1号
	Animal1,							//动物2号
	Animal2,							//动物3号
	Animal3,							//动物4号

	AnimalMax							//MAX
};

enum enumAnimalColor
{
	InvalidAniColor = -1,

	Red = 0,							//红色
	Green,								//绿色
	Yellow,								//黄色

	ColorMax
};


enum enumGameZXH
{
	InvalidZHX = -1,

	Zhuang = 0,							//庄
	He,									//和
	Xian,								//闲

	ZXHmax								//MAX
};


//玩家结构
struct slwhPlayerInfo{
	int			userid;
	char		nickname[LEN_NICKNAME];
	int			viplevel;
	int			gold;
	int			headimgid;
	SCORE		jet;						//玩家当前压分				
	SCORE		winscore;					//玩家输赢

	//排序规则 如果赢分一样VIP高排前面 (从大到小)

	//bool operator > (const slwhPlayerInfo &m)const
	//{
	//	bool b = false;
	//	if (winscore > m.winscore) b = true;
	//	else if (winscore == m.winscore && viplevel > m.viplevel) b = true;
	//	return b;
	//}

	////排序规则 如果vip一样 分数高的排前面 （从大到小排序）
	//bool operator < (const slwhPlayerInfo &m)const 
	//{ 
	//	bool b = true;
	//	if (viplevel < m.viplevel) b = false;
	//	else if (viplevel == m.viplevel && gold < m.gold) b = false;
	//	return b;
	//}


	static bool LessComp(const slwhPlayerInfo& left, const slwhPlayerInfo& right)
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

	static bool GreatComp(const slwhPlayerInfo& left, const slwhPlayerInfo& right)
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

	slwhPlayerInfo(){ memset(this, 0, sizeof(slwhPlayerInfo)); };
};


//压分信息
struct slwhJetInfo
{

	dword index;					//压分当前档
	dword dwOneLimit;				//每个按钮压分最高限制
	dword dwJet[5];					//压分档位

	slwhJetInfo(){ memset(this, 0, sizeof(slwhJetInfo)); };
};

//总压分信息
struct slwhAllJetInfo
{
	SCORE lTotelBet[15];			//15个总压分信息
	SCORE lAllBet;					//总总分

	slwhAllJetInfo(){ memset(this, 0, sizeof(slwhAllJetInfo)); };
};


struct slwhRoomInfo
{
	word			wType;									//房间类型 0兔房 1猫房 2狮房
	dword			dwRateID;								//这一轮的倍率ID；
	longlong		lCj;									//彩金
	dword			dwOnline;								//在线人数
	slwhJetInfo		jetinfo;								//压分信息
	slwhAllJetInfo	alljetinfo;								//所有压分总信息
	std::vector<slwhPlayerInfo>	vec;						//玩家信息列表
	dword			dwColorIndex[24];						//这一轮的色块排列;
	slwhRoomInfo()
	{
		dwRateID = 0;
		wType = 0;
		lCj = 0;
		dwOnline = 0;
		memset(dwColorIndex, 0, sizeof(dwColorIndex));
	}
};



struct AnimalResult
{
	enumSpecialRs e1;                    //开奖级别
	enumAnimalColor e2;                  //动物颜色
	enumAnimalType e3;                   //动物类型
	enumGameZXH e4;                      //庄闲和

	AnimalResult(
		enumSpecialRs e1 = InvalidSpecial,
		enumAnimalColor e2 = InvalidAniColor,
		enumAnimalType e3 = InvalidAniType,
		enumGameZXH e4 = InvalidZHX)
	{
		this->e1 = e1;
		this->e2 = e2;
		this->e3 = e3;
		this->e4 = e4;
	}

	AnimalResult(int a)
	{
		this->e1 = (enumSpecialRs)(a / 1000);
		this->e2 = (enumAnimalColor)(a / 100 % 10);
		this->e3 = (enumAnimalType)(a / 10 % 10);
		this->e4 = (enumGameZXH)(a % 10);
	}

	void InitWith(
		enumSpecialRs e1,
		enumAnimalColor e2,
		enumAnimalType e3,
		enumGameZXH e4)
	{
		this->e1 = e1;
		this->e2 = e2;
		this->e3 = e3;
		this->e4 = e4;
	}
};


//普通 D3Y  D4X开奖结果
struct OpenRes
{
	AnimalResult res;
	int colorindex;
	int animalindex;

	OpenRes(){ memset(this, 0, sizeof(OpenRes)); };
};


//彩金开奖
struct OpenCJ
{
	bool bresOk;						//开奖结果是否接收完毕

	bool binfoOk;						//玩家信息是否完毕

	int	 num;							//有几个玩家中奖
	dword userid[3];					//3个玩家的USERID
	char name[3][LEN_NICKNAME];			//3个玩家的名字
	longlong	score[3];				//3个玩家的获奖分数

	OpenCJ(){  memset(this, 0, sizeof(OpenCJ)); };
};


//DMG开奖
struct OpenDMG
{
	AnimalResult res[12];
	int colorindex[12];
	int animalindex[12];
	int leftIndex;						//当前开奖到还剩环节数

	OpenDMG(){ memset(this, 0, sizeof(OpenDMG)); };
};


//幸运抽奖结构体
struct OpenXYCJ
{
	bool bresOk;						//开奖结果是否接收完毕

	bool bxycjOk;						//幸运抽奖界面消息是否接收完毕

	int  iNotes[4];						//O位置为1:获取后面3位置

	int Atype;							//A区间

	float AValue;						//A区间的值

	OpenXYCJ(){ memset(this, 0, sizeof(OpenXYCJ)); };
};


//4连发
struct Open4LF
{
	AnimalResult res[4];
	int colorindex[4];
	int animalindex[4];
	int leftIndex;						//当前开奖到还剩环节数
	 
	Open4LF(){ memset(this, 0, sizeof(Open4LF)); };
};


#endif
