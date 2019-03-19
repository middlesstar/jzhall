#ifndef  _sharkNetStruct_H_
#define  _sharkNetStruct_H_
#pragma pack(1)
#include "df/types.h"
#include "df/Define.h"

/////////////////////////////////////////////////////////////
//登录命令

#define MDM_GR_LOGON_SHARK				1									//登录信息

//登录模式
#define SUB_GR_LOGON_USERID_SHARK		1									//I D 登录
#define SUB_GR_LOGON_MOBILE_SHARK		2									//手机登录
#define SUB_GR_LOGON_ACCOUNTS_SHARK		3									//帐户登录

//登录结果
#define SUB_GR_LOGON_SUCCESS_SHARK		100									//登录成功
#define SUB_GR_LOGON_FAILURE_SHARK		101									//登录失败
#define SUB_GR_LOGON_FINISH_SHARK		102									//登录完成

//升级提示
#define SUB_GR_UPDATE_NOTIFY_SHARK		200									//版本升级提示


/////////////////////////////////////////////////////////////
//配置命令

#define MDM_GR_CONFIG_SHARK				2									//配置信息

#define SUB_GR_CONFIG_COLUMN_SHARK		100									//列表配置
#define SUB_GR_CONFIG_SERVER_SHARK		101									//房间配置
#define SUB_GR_CONFIG_PROPERTY_SHARK		102									//道具配置
#define SUB_GR_CONFIG_FINISH_SHARK		103									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT_SHARK	104									//玩家权限


///////////////////////////////////////////////////////////
//用户命令
#define MDM_GR_USER_SHARK				3


#define SUB_GR_USER_CHAIR_REQ_SHARK		10									//快速坐下
#define SUB_GR_USER_STANDUP_SHARK		4									//请求起立


#define SUB_GR_USER_ENTER_SHARK			100									//用户进入
#define SUB_GR_USER_STATUS_SHARK			102									//用户状态
#define SUB_GR_USER_ERROR_CODE			103									//错误信息
#define SUB_GR_GLAD_MESSAGE_SHARK		400									//喜报消息


//////////////////////////////////////////////////////////
//状态命令
#define MDM_GR_STATUS_SHARK				4									//状态信息

#define SUB_GR_TABLE_INFO_SHARK			100									//桌子信息
#define SUB_GR_TABLE_STATUS_SHARK		101									//桌子状态


////////////////////////////////////////////////////////
//框架命令
#define MDM_GF_FRAME_SHARK				100									//框架命令

#define SUB_GF_GAME_OPTION_SHARK			1									//游戏配置

/////////////////////////////////////////////////////////
//系统命令
#define MDM_CM_SYSTEM_SHARK				1000								//系统命令

#define SUB_CM_SYSTEM_MESSAGE_SHARK		1									//系统消息

//控制掩码
#define SMT_CLOSE_ROOM_SHARK				0x0100								//关闭房间
#define SMT_CLOSE_GAME_SHARK				0x0200								//关闭游戏
#define SMT_CLOSE_LINK_SHARK				0x0400								//中断连接


////////////////////////////////////////////////////////
//游戏命令(游戏逻辑)
#define MDM_GF_GAME_SHARK				200									//游戏命令




//网狐框架桌子信息
struct SharkTableInfo
{
	word							wTableID;							//桌子ID
	byte							bTableLock;							//桌子锁
	byte							bPlayStatus;						//玩家状态

	word							wTableCount;						//桌子数目
	word							wChairCount;						//椅子数目

	word							wServerType;						//游戏类型
	dword							dwServerRule;						//房间规则
};


//Shark用户信息
struct SharkUserInfo
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

	SharkUserInfo(){ memset(this, 0, sizeof(SharkUserInfo)); };
};

#define  SUB_G_INTOGAMEINGREQ			999									//请求查询桌子信息
#define  SUB_G_INTOGAMEINGRES			1000								//进入游戏同步

#define  SUB_G_OPERATIONNOTES             1002				 //请求下注
//请求下注协议
struct CMD_GP_PUTNOTESREQU{
	int							AmimalIndex;                //0 - 12个索引 
	longlong				    AmimalGold;                 //下注动物的金币
};

#define  SUB_G_GAMESENLINSTART          1003
struct CMD_GP_StartGameRandRsp
{
	dword                           AnimalIndex[3];            //通知开奖的索引；0：普通奖 不关注后边 
	dword                           Atype;      //A区间类型 1-元宝 2-船票 3-金币
	dword                           Avalue;     //A区间的值
};
#define SUB_G_GAMESNEXTSTART           1004                    //通知开启下一轮游戏：广播房间
struct CMD_GP_StartNextGame{ 

	dword				JsRate;				//这一轮的金沙倍率；
	dword				RateID;				//这一轮的倍率ID
	longlong			CaiJinValue;		//这一轮的彩金值
};

#define SUB_G_GAMECAIJINGRES			1005									//彩金玩家信息

#define SUB_G_GAMEUSERCHANGE			1009							//玩家加入或离开

#define    SUB_G_GAMECHEARNOTES            1010				 //请求清除个人押注信息

#define SUB_G_GAMESENLINSTARTBORD          1011			 //通知所有:下注返回信息
//返回下注信息;通知桌子上所有玩家
struct CMD_GP_PUTNOTESREC{
	longlong						gold;                     //增加金币总值
	dword							userid;					  //下注玩家ID
	longlong						currgold;				  //玩家身上金币
	dword					        AnimalIndex;									//所压动物
};

#define SUB_G_GAMESTARTGAMELIST			1012								//通知所有:开奖用户列表刷新

#define SUB_G_GAMECHEARNOTEALL			1013								//通知所有:清除某个玩家所有下注信息
struct St_UserNote{
	word noteid;															//压分按钮id 0~11
	longlong gold;															//压分金币数
};

//玩家已下注的数据
struct CMD_GP_CharNotesInfor{
	dword								userid;								//玩家id
	dword								size;								//大小
	St_UserNote						notes[15];							//已下注的数据
};


#define SUB_G_GAMETICKTIME				9999								//发送心跳包
#define SUB_G_GAMETICKTIMERSP			8888								//收到心跳包


#endif // _sharkNetStruct_H_

