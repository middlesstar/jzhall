#ifndef _SLWH_NET_STRUCT
#define _SLWH_NET_STRUCT
#pragma pack(1)

#include "df/types.h"
#include "df/Define.h"


/////////////////////////////////////////////////////////////
//登录命令

#define MDM_GR_LOGON_SLWH				1									//登录信息

//登录模式
#define SUB_GR_LOGON_USERID_SLWH		1									//I D 登录
#define SUB_GR_LOGON_MOBILE_SLWH		2									//手机登录
#define SUB_GR_LOGON_ACCOUNTS_SLWH		3									//帐户登录

//登录结果
#define SUB_GR_LOGON_SUCCESS_SLWH		100									//登录成功
#define SUB_GR_LOGON_FAILURE_SLWH		101									//登录失败
#define SUB_GR_LOGON_FINISH_SLWH		102									//登录完成

//升级提示
#define SUB_GR_UPDATE_NOTIFY_SLWH		200									//版本升级提示


/////////////////////////////////////////////////////////////
//配置命令

#define MDM_GR_CONFIG_SLWH				2									//配置信息

#define SUB_GR_CONFIG_COLUMN_SLWH		100									//列表配置
#define SUB_GR_CONFIG_SERVER_SLWH		101									//房间配置
#define SUB_GR_CONFIG_PROPERTY_SLWH		102									//道具配置
#define SUB_GR_CONFIG_FINISH_SLWH		103									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT_SLWH	104									//玩家权限


///////////////////////////////////////////////////////////
//用户命令
#define MDM_GR_USER_SLWH				3


#define SUB_GR_USER_CHAIR_REQ_SLWH		10									//快速坐下
#define SUB_GR_USER_STANDUP_SLWH		4									//请求起立


#define SUB_GR_USER_ENTER_SLWH			100									//用户进入
#define SUB_GR_USER_STATUS_SLWH			102									//用户状态
#define SUB_GR_USER_ERROR_CODE			103									//错误信息
#define SUB_GR_GLAD_MESSAGE_SLWH		400									//喜报消息


//////////////////////////////////////////////////////////
//状态命令
#define MDM_GR_STATUS_SLWH				4									//状态信息

#define SUB_GR_TABLE_INFO_SLWH			100									//桌子信息
#define SUB_GR_TABLE_STATUS_SLWH		101									//桌子状态


////////////////////////////////////////////////////////
//框架命令
#define MDM_GF_FRAME_SLWH				100									//框架命令

#define SUB_GF_GAME_OPTION_SLWH			1									//游戏配置

/////////////////////////////////////////////////////////
//系统命令
#define MDM_CM_SYSTEM_SLWH				1000								//系统命令

#define SUB_CM_SYSTEM_MESSAGE_SLWH		1									//系统消息

//控制掩码
#define SMT_CLOSE_ROOM_SLWH				0x0100								//关闭房间
#define SMT_CLOSE_GAME_SLWH				0x0200								//关闭游戏
#define SMT_CLOSE_LINK_SLWH				0x0400								//中断连接


////////////////////////////////////////////////////////
//游戏命令(游戏逻辑)
#define MDM_GF_GAME_SLWH				200									//游戏命令


#define SUB_GR_INTOGAMEINGREQ			999									//请求查询桌子信息
#define SUB_GR_INTOGAMEINGRES			1000								//进入游戏同步

//动物数据
//struct AnimalRecorde{
//	//1-闲 2-庄 3-和 
//	int atype;
//	//服务端的动物编号
//	int animaNumber;
//	//动物
//	int animalindex;
//	//特殊开奖
//	int tsid;		
//};

//返回游戏桌子信息
//struct SLWHGameTableInfo
//{
//	dword							dwGameState;							//游戏状态 1:下注中;2;游戏开始 
//	dword                           dwLeftTime;								//如果下注中;给出剩余多少秒
//	dword							dwColorIndex[24];						//这一轮的色块排列;
//	AnimalRecorde					RecordAnimal[20];						//20条已抽记录；
//	dword							dwRateID;								//这一轮的倍率ID；
//	longlong						lCj;									//彩金
//	dword							onlineNum;								//在线人数
//	PlayerInfo						PlayerInfo[100];	
//};



#define SUB_GR_VIEWALLNOTES				1001								//查询所有下注情况
//返回压分结构




#define SUB_GR_OPERATIONNOTES			1002								//请求下注
//请求压分结构体
struct CMD_GP_PUTNOTESREQ{
	longlong							iAmimalIndex;							//0 - 14个索引 12=庄 13=和 闲=14
	longlong							iAmimalGold;							//下注动物的金币
};


#define SUB_GR_GAMESENLINSTART			1003								//通知开奖消息体
//返回开奖结果（普通 D3Y D4X ）
struct CMD_GP_StartGameRandRes
{
	dword							dwAnimalIndex;							//开出动物所有0-23 {1, 0, 1, 3, 0, 1, 0, 2, 0, 2, 1, 3, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 3} 索引
	dword                           dwAnimalType;							//开出动物类型0-2; 庄 和 
	dword							dwColorIndex;							//色块索引
	dword							dwTsRewardId;							//特殊开奖的 ID 1:没有特殊奖 2:彩金 3:大三元 4:大四元 5:四连发 6:大满贯 7:幸运抽奖
	
};



#define SUB_G_GAMECAIJINGRES			1005									//彩金玩家信息


#define SUB_GR_GAMESNEXTSTART			1004								//通知开启下一轮游戏：广播房间
//返回开启一局新的游戏
struct CMD_GP_StartGame{
	dword							dwColorIndex[24];						//这一轮的色块排列;
	dword							dwRateID;								//这一轮的倍率ID；
	longlong						lCj;									//彩金
};


#define SUB_G_GAMESILIANFRES			1006								//四连发
struct CMD_GP_SiLianFa
{
	dword     				Notes[4];										//四个动物索引
	dword     				Clors[4];										//四个颜色索引
	dword                   AnimalType;										//开出庄和贤0-2;
};


//DMG大满贯
#define SUB_G_GAMEDAMANGUANRES			1007							//大满贯
struct CMD_GP_DaManGuan
{
	dword     				Notes[12];            //12个动物索引
	dword     				Clors[12];            //12个颜色索引
	dword                   AnimalType;           //开出庄和贤0-2;
};

#define SUB_G_GAMELUCHDRAEM				1008							//幸运抽奖
//XYCJ幸运抽奖
struct CMD_GP_LuckDraw{
	dword     				Notes[4];            //O位置为1:获取后面3位置
	dword					LuckType;			 //A区间： 1-元宝 2-船票 
	dword					LuckValue;			 //A区间的值 除以100
};


#define SUB_G_GAMEUSERCHANGE			1009							//玩家加入或离开
//收到的数据包
//struct CMD_GP_UserChange
//{
//	PlayerInfo						pl;									//玩家信息	
//	DWORD							type;								//0-退出 1-增加 玩家退出只有userid有效项
//};



#define SUB_G_GAMECHEARNOTES			1010								//请求清除个人押注信息

#define SUB_G_GAMESENLINSTARTBORD		1011								//通知所有:下注返回信息
struct CMD_GP_PUTNOTESRSP
{
	longlong				gold;											//压分
	dword					userid;											//玩家ID
	longlong				score;											//分数
	dword					animalIndex;									//所压动物
};

#define SUB_G_GAMESTARTGAMELIST			1012								//通知所有:开奖用户列表刷新
//struct CMD_GP_LotterList
//{
//	dword							onlineNum;								//在线人数
//	PlayerInfo						PlayerInfo[100];	
//};


#define SUB_G_GAMECHEARNOTEALL			1013								//通知所有:清除某个玩家所有下注信息
struct St_UserNotes{
	word noteid;															//压分按钮id 0~14
	longlong gold;															//压分金币数
};

//玩家已下注的数据
struct CMD_GP_CharNotesInfo{
	dword								userid;								//玩家id
	dword								size;								//大小
	St_UserNotes						notes[15];							//已下注的数据
};


#define SUB_G_GAMETICKTIME				9999								//发送心跳包
#define SUB_G_GAMETICKTIMERSP			8888								//收到心跳包



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//struct



//请求失败 错误信息
//struct CMD_GR_RequestFailure
//{
//	LONG							lErrorCode;							//错误代码
//	TCHAR							szDescribeString[256];				//描述信息
//};



//网狐框架桌子信息
struct SLWHTableInfo
{
	word							wTableID;							//桌子ID
	byte							bTableLock;							//桌子锁
	byte							bPlayStatus;						//玩家状态

	word							wTableCount;						//桌子数目
	word							wChairCount;						//椅子数目

	word							wServerType;						//游戏类型
	dword							dwServerRule;						//房间规则
};


//SLWH用户信息
struct SLWHUserInfo
{
	//基本属性
	dword							dwUserID;							//用户 I D
	dword							dwGameID;							//游戏 I D
	dword							dwGroupID;							//社团 I D
	char							szNickName[LEN_NICKNAME];			//用户昵称
	char							szGroupName[LEN_GROUP_NAME];		//社团名字
	char							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//头像信息
	word							wFaceID;							//头像索引
	dword							dwCustomID;							//自定标识

	//用户资料
	byte							cbGender;							//用户性别
	byte							cbMemberOrder;						//会员等级
	byte							cbMasterOrder;						//管理等级

	//用户状态
	word							wTableID;							//桌子索引
	word							wChairID;							//椅子索引
	byte							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行

	//游戏信息
	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数
	dword							dwFleeCount;						//逃跑盘数
	dword							dwUserMedal;						//用户奖牌
	dword							dwExperience;						//用户经验
	dword							lLoveLiness;						//用户魅力

	//时间信息
	//tagTimeInfo						TimerInfo;						//时间信息

	//比赛信息
	byte							cbEnlistStatus;						//报名状态

	//扩展标识
	dword							lExpand;
	dword							dwExpand;

	SLWHUserInfo(){ memset(this, 0, sizeof(SLWHUserInfo)); };
};



#endif