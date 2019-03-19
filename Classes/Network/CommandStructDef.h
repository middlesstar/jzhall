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
网狐协议：错误 和成功 返回 消息
*************************************************/
//操作成功
struct CMD_GP_OperateSuccess
{
	long							lResultCode;						//操作代码
	char							szDescribeString[128];				//成功消息

	void fromPack(const PacketAide& packData);
};

//操作失败
struct CMD_GP_OperateFailure
{
	long							lResultCode;						//错误代码
	char							szDescribeString[128];				//描述消息

	void fromPack(const PacketAide& packData);
};

//游客登录
struct tagHallGPVisitorLogon : public sStructBase
{
	tagHallGPVisitorLogon()
	{
		memset(this, 0, sizeof(tagHallGPVisitorLogon));
	}

	//word wFaceID;           //  
	//byte cbValidateFlags;   //

	//系统信息
	byte                            cbDeviceType;                       //设备类型
	word							wModuleID;							//模块标识(KindID)
	dword							dwPlazaVersion;						//广场版本
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
	byte							cbValidateFlags;			        //校验标识(手机没有密保)
	char							szPassword[LEN_MD5];				//登录密码

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//注册帐号
struct CMD_GP_RegisterAccounts : public sStructBase
{
	CMD_GP_RegisterAccounts()
	{
		memset(this, 0, sizeof(CMD_GP_RegisterAccounts));
	}

	//系统信息
	byte                            cbDeviceType;                       //设备类型
	word							wModuleID;							//模块标识(KindID)
	dword							dwPlazaVersion;						//广场版本
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
	char							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码(手机用)

	//密码变量
	char							szLogonPass[LEN_MD5];				//登录密码
	char							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	word							wFaceID;							//头像标识
	byte							cbGender;							//用户性别
	char							szAccounts[LEN_ACCOUNTS];			//登录帐号
	char							szNickName[LEN_NICKNAME];			//用户昵称
	char							szSpreader[LEN_ACCOUNTS];			//推荐帐号(手机目前没有)
	char							szPassPortID[LEN_PASS_PORT_ID];		//证件号码(手机目前没有)
	char							szCompellation[LEN_COMPELLATION];	//真实名字(手机目前没有)
	byte							cbValidateFlags;			        //校验标识(手机目前没有)

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//登录成功
struct CMD_GP_LogonSuccess : public sStructBase
{
	CMD_GP_LogonSuccess()
	{
		memset(this, 0, sizeof(CMD_GP_LogonSuccess));
	}

	//属性资料
	word							wFaceID;							//头像标识
	dword							dwUserID;							//用户 I D
	dword							dwGameID;							//游戏 I D
	dword							dwGroupID;							//社团标识
	dword							dwCustomID;							//自定标识
	dword							dwUserMedal;						//用户奖牌
	dword							dwExperience;						//经验数值
	dword							dwLoveLiness;						//用户魅力

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行

	//用户信息
	byte							cbGender;							//用户性别
	byte							cbMoorMachine;						//锁定机器
	char							szAccounts[LEN_ACCOUNTS];			//登录帐号
	char							szNickName[LEN_ACCOUNTS];			//用户昵称
	char							szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	byte                            cbShowServerStatus;                 //显示服务器状态

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//大厅登录成功补充字段,
struct CMD_GP_LogonSuccess_Hall : public sStructBase
{
	CMD_GP_LogonSuccess_Hall()
	{
		memset(this, 0, sizeof(CMD_GP_LogonSuccess_Hall));
	}

	dword							userid;								//玩家id
	longlong						llYuanBao;							//玩家元宝
	dword							dwVipLevel;							//VIP等级
	dword							dwExpLevel;							//经验等级
	dword							dwTicket;							//船票
	dword  							fudaitime;							//福袋的剩余时间
	dword 							ritems[6];							//商店首冲列表,0-首冲，1-已充过

	dword							sumRecharge;						//充值总金额
	dword							istrourist;							//0-游客,1-非游客
	dword 							iscomment;							//0-没有评论 1-评论

	void fromPack(const PacketAide& packData);
};

//登录失败
struct CMD_GP_LogonFailure : public sStructBase
{
	CMD_GP_LogonFailure()
	{
		memset(this, 0, sizeof(CMD_GP_LogonFailure));
	}

	long							lResultCode;						//错误代码
	char							szDescribeString[128];				//描述消息

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//登陆完成
struct CMD_GP_LogonFinish : public sStructBase
{
	CMD_GP_LogonFinish()
	{
		memset(this, 0, sizeof(CMD_GP_LogonFinish));
	}

	word							wIntermitTime;						//中断时间
	word							wOnLineCountTime;					//更新时间

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//游戏列表

//游戏类型
struct tagHallGameType : public sStructBase
{
	tagHallGameType()
	{
		memset(this, 0, sizeof(tagHallGameType));
	}

	word							wJoinID;							//挂接索引
	word							wSortID;							//排序索引
	word							wTypeID;							//类型索引
	char							szTypeName[LEN_TYPE];				//种类名字

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//游戏种类
struct tagHallGameKind : public sStructBase
{
	tagHallGameKind()
	{
		memset(this, 0, sizeof(tagHallGameKind));
	}

	word							wTypeID;							//类型索引
	word							wJoinID;							//挂接索引
	word							wSortID;							//排序索引
	word							wKindID;							//类型索引
	word							wGameID;							//模块索引
	dword							dwOnLineCount;						//在线人数
	dword							dwFullCount;						//满员人数
	char							szKindName[LEN_KIND];				//游戏名字
	char							szProcessName[LEN_PROCESS];			//进程名字

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//游戏节点
struct tagHallGameNode : public sStructBase
{
	tagHallGameNode()
	{
		memset(this, 0, sizeof(tagHallGameNode));
	}

	word							wKindID;							//名称索引
	word							wJoinID;							//挂接索引
	word							wSortID;							//排序索引
	word							wNodeID;							//节点索引
	char							szNodeName[LEN_NODE];				//节点名称

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//定制类型
struct tagHallGamePage : public sStructBase
{
	tagHallGamePage()
	{
		memset(this, 0, sizeof(tagHallGamePage));
	}

	word							wPageID;							//页面索引
	word							wKindID;							//名称索引
	word							wNodeID;							//节点索引
	word							wSortID;							//排序索引
	word							wOperateType;						//控制类型
	char							szDisplayName[LEN_PAGE];			//显示名称

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//游戏房间
struct tagHallGameServer : public sStructBase
{
	tagHallGameServer()
	{
		memset(this, 0, sizeof(tagHallGameServer));
	}

	word							wKindID;							//名称索引
	word							wNodeID;							//节点索引
	word							wSortID;							//排序索引
	word							wServerID;							//房间索引
	word							wServerPort;						//房间端口
	dword							dwOnLineCount;						//在线人数
	dword							dwFullCount;						//满员人数
	char							szServerAddr[LEN_SERVERADDR];		//房间名称
	char							szServerName[LEN_SERVER];			//房间名称

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//I D 登录
struct CMD_GP_LogonGameID : public sStructBase
{
	CMD_GP_LogonGameID()
	{
		memset(this, 0, sizeof(CMD_GP_LogonGameID));
	}

	//系统信息
	byte                            cbDeviceType;                       //设备类型
	word							wModuleID;							//模块标识(KindID)
	dword							dwPlazaVersion;						//广场版本
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
	char							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码(手机用)

	//登录信息
	dword							dwGameID;							//游戏 I D
	char							szPassword[LEN_MD5];				//登录密码
	byte							cbValidateFlags;			        //校验标识(手机没有密保)

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//帐号登录
struct CMD_GP_LogonAccounts : public sStructBase
{
	CMD_GP_LogonAccounts()
	{
		memset(this, 0, sizeof(CMD_GP_LogonAccounts));
	}

	//系统信息
	byte                            cbDeviceType;                       //设备类型
	word							wModuleID;							//模块标识(KindID)
	dword							dwPlazaVersion;						//广场版本
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
	char							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码(手机用)

	//登录信息
	char							szPassword[LEN_MD5];				//登录密码
	char							szAccounts[LEN_ACCOUNTS];			//登录帐号
	byte							cbValidateFlags;			        //校验标识(手机没有密保)
	word							wPlatform;							//平台0:原本平台 其它:其它平台 奇虎360,百度啊,什么的

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//摇钱树结构
struct CMD_GP_YaoQianShu_USERINFO
{
	word							nType;									//请求类型,0-请求数据,1-请求获取金币
	dword							dwGameID;								//用户ID

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//服务器返回摇钱树
struct CMD_GP_YaoQianShu_Get
{
	word							nType;									//回复类型
	dword							nTime;									//倒计时
	dword							GetGold;								//可到金币数量	
	dword							TotelGold;								//金币上限

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

/*************************************************
自定义系统头像(大协议号3,小协议号，请求：201，回复：200)
*************************************************/							 
struct CMD_GP_SystemFaceInfo : public sStructBase
{
	CMD_GP_SystemFaceInfo()
	{
		memset(this, 0, sizeof(CMD_GP_SystemFaceInfo));
	}

	word							wFaceID;							//头像标识
	dword							dwUserID;							//用户 I D
	char							szPassword[LEN_PASSWORD];			//用户密码
	char							szMachineID[LEN_MACHINE_ID];		//机器序列

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//返回包
struct CMD_GP_UserFaceInfo
{
	word							wFaceID;							//头像标识
	dword							dwCustomID;							//自定标识

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};


/*************************************************
自定义用户昵称(大协议号3,小协议号，请求：746，回复：747)
*************************************************/
struct CMD_ST_modiflyuser_req : public sStructBase
{
	CMD_ST_modiflyuser_req()
	{
		memset(this, 0, sizeof(CMD_ST_modiflyuser_req));
	}

	dword     					userId;				//用户ID；
	char						nickName[LEN_NICKNAME];		//用户昵称		如果为L"" 则不更新名字信息	 

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};
//返回 
struct CMD_ST_modiflyuser_rsp
{
	dword 						ret;			//0:成功 1:昵称不合法  

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};


/*************************************************
查询银行资料(大协议号3,小协议号，请求：404，回复：100)
*************************************************/
//查询银行
struct CMD_GP_QueryInsureInfo
{
	dword							dwUserID;							//用户 I D
};

//返回银行资料
struct CMD_GR_S_UserInsureInfo
{
	byte                            cbActivityGame;                     //游戏动作
	word							wRevenueTake;						//税收比例
	word							wRevenueTransfer;					//税收比例
	word							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};


/*************************************************
向银行存款(大协议号3,小协议号，请求：400，回复：成功-405，失败-406)
*************************************************/
//存入金币
struct CMD_GP_UserSaveScore : public sStructBase
{
	CMD_GP_UserSaveScore()
	{
		memset(this, 0, sizeof(CMD_GP_UserSaveScore));
	}

	dword							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	char							szMachineID[LEN_MACHINE_ID];		//机器序列

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//提取金币
struct CMD_GP_UserTakeScore : public sStructBase
{
	CMD_GP_UserTakeScore()
	{
		memset(this, 0, sizeof(CMD_GP_UserTakeScore));
	}

	dword							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	char							szPassword[LEN_MD5];				//银行密码
	char							szMachineID[LEN_MACHINE_ID];		//机器序列

	void toPack(PacketAide& packData);
	void fromPack(const PacketAide& packData);
};

//银行操作成功
struct CMD_GR_S_UserInsureSuccess : public sStructBase
{
	CMD_GR_S_UserInsureSuccess()
	{
		memset(this, 0, sizeof(CMD_GR_S_UserInsureSuccess));
	}

	dword							dwUserID;							//用户 I D
	SCORE							lUserScore;							//身上金币
	SCORE							lUserInsure;						//银行金币
	char							szDescribeString[128];				//描述消息

	void fromPack(const PacketAide& packData);
};

//银行操作失败
struct CMD_GP_UserInsureFailure : public sStructBase
{
	CMD_GP_UserInsureFailure()
	{
		memset(this, 0, sizeof(CMD_GP_UserInsureFailure));
	}

	long							lResultCode;						//错误代码
	char							szDescribeString[128];				//描述消息

	void fromPack(const PacketAide& packData);
};


/*************************************************
救济金请求(大协议号3,小协议号，请求：748，回复：749)
*************************************************/
//救济金请求
struct CMD_ST_JIUJIJIN_req
{
	dword						userId;          //用户ID；		

	void toPack(PacketAide& packData);
};

//救济金返回
struct CMD_ST_JIUJIJIN_rsp
{
	dword						gold;          //0:领取失败，>0 领取救济金 成功并返回钱
	dword						code;		   //0 成功,1 玩家不存在 2 今日领取达到上限 3 高于1000不能领取 

	void fromPack(const PacketAide& packData);
};



/*************************************************
每日抽奖(大协议号3,小协议号，请求：738，回复：739)
*************************************************/
//请求每日抽奖数据结构
struct CMD_GP_DailyLottery_USERINFO
{
	dword							dwGameID;								//用户ID

	void toPack(PacketAide& packData);
};

//返回每日抽奖数据结构
struct CMD_GP_DailyLottery_Get
{
	dword							dwIndex;								//中奖索引 按照策划案顺序 10000为不能抽奖（今天已抽过奖）
	word							wType;									//中奖类型 0为金币 1位元宝
	dword							dwGetGold;								//得到金币数量	
	dword							dwGetYuanbao;							//得到元宝数量

	void fromPack(const PacketAide& packData);
};
/*************************************************
每日签到(大协议号3,小协议号，请求：750，回复：751)
*************************************************/
//请求每日签到
struct CMD_ST_SignDay_req
{
	dword     					userId;          //用户ID；		
	dword     					opType;          //1:签到 2:领取连续奖励
	void toPack(PacketAide& packData);
};

//返回签到信息
struct CMD_ST_SignDay_rsp
{
	dword     					gold;            //获得到的金币数量	； 
	dword     					opType;          //操作类型
	dword                       ret;			//0-成功 1：玩家不存在 2：今日已签到 3：连续天数不足
	dword                       signindex;      //签到索引 1- 30
	dword                       lxcount;        //连续签到的次数

	void fromPack(const PacketAide& packData);
};


/*************************************************
请求兑换(大协议号3,小协议号，请求：742，回复：743)
*************************************************/
struct CMD_GP_Hall_Goods_USERINFO : public sStructBase
{
	CMD_GP_Hall_Goods_USERINFO()
	{
		memset(this, 0, sizeof(CMD_GP_Hall_Goods_USERINFO));
	}

	dword							userId;									//用户ID
	word							wIndex;									//购买的商品索引
	char							tName[25];								//用户姓名		如果为"" 则不更新名字信息
	longlong						llTelNum;								//用户手机号    如果为-1  则不更新电话信息
	char							tAddress[128];							//收货地址		如果为"" 则不更新地址信息
	longlong						uqq;								    //用户QQ号码

	void toPack(PacketAide& packData);
};

//购买商品和提交（更新）用户信息回应  
//商城消息回应结构体
struct CMD_GP_Hall_Goods_Get
{
	dword							wgold;									//如果是兑换金币；就兑换金币
	dword							dwErrorCode;							//0=成功 1=玩家不存在 2-不存在物品 3-元宝不足 4-资料不完整

	void fromPack(const PacketAide& packData);
};

/*************************************************
请求查询兑奖记录(大协议号3,小协议号，请求：752，回复：753)
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

	char							dtime[25];								//兑换时间
	char							itemName[25];							//商品名称
	char							orderNo[25];							 //兑换单号
	dword							dstate;									//兑换状态 0-审核中 1-审核通过

	void fromPack(const PacketAide& packData);
}DxRecord;

struct CMD_ST_DxRewardList_req
{
	dword     					userId;            //用户ID

	void toPack(PacketAide& packData);
};

struct CMD_ST_DxRewardList_rsp
{
	DxRecord						aGoodsList[100];							//兑换记录

	void fromPack(const PacketAide& packData);
};


/*************************************************
请求查询兑奖记录(大协议号3,小协议号，请求：754，回复：755)
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

	dword                           eid;									 //邮件ID 
	char							sendusername[25];						 //发送的玩家名称
	char							emailtitle[30];							 //邮件标题
	char							emialcontent[500];						 //邮件内容
	char							sendtime[20];							//发送时间
	dword                           isread; 								 //是否阅读 1-阅读 0-未阅读
	dword                           attachtype;                              //附件类型 1:金币 2:元宝 3：船票
	dword                           attachnums;                               //附件数量
	dword                           attachtype2;                              //附件2类型 1:金币 2:元宝 3：船票
	dword                           attachnums2;                               //附件2数量

	void fromPack(const PacketAide& packData);

};

//请求查询邮件列表
struct CMD_ST_GetEmailList_req
{
	dword     					userId;					  //用户ID

	void toPack(PacketAide& packData);
};

struct CMD_ST_GetEmailList_rsp
{
	EmailInfoSt     				emaillist[20];            //邮件列表

	void fromPack(const PacketAide& packData);
};

/*************************************************
请求赠送船票(大协议号3,小协议号，请求：758，回复：759)
*************************************************/
struct CMD_ST_GiveChuanPiaoReq
{
	dword   				userid;              //自己的用户ID
	dword   				gameid;              //对方的的游戏ID
	dword 					nums;				 //赠送数量

	void toPack(PacketAide& packData);
};

struct CMD_ST_GiveChuanPiaoRsp
{
	dword   				ret;              //0;赠送成功 1-玩家不存在

	void fromPack(const PacketAide& packData);
};


/*************************************************
请求兑换微信礼品(大协议号3,小协议号，请求：760，回复：761)
*************************************************/
struct CMD_ST_GetLiPinReq : public sStructBase
{
	CMD_ST_GetLiPinReq()
	{
		memset(this, 0, sizeof(CMD_ST_GetLiPinReq));
	}

	dword   				userid;              //自己的用户ID
	char 					cardnum[9];			 //卡号

	void toPack(PacketAide& packData);
};

struct CMD_ST_GetLiPinRsp
{
	dword   				ret;              //0:成功 1-已领取 2-不存在卡号
	dword 					gold;			  //如果为ret 0 -增加的金币
	dword 					money;			  //如果为ret  0 -增加的元宝

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_ShareSucessReq
{
	dword   				userid;              //0:成功分享的玩家ID
	dword 					sharetype;			 //1-微信朋友圈 2-QQ朋友圈 3-微信好友

	void toPack(PacketAide& packData);
};

struct CMD_ST_ShareSucessRsp
{
	long   					lGold;              	//分享成功后获得金币，可能为0
	dword 					dwCount;			  	//此类型分享的次数
	dword 					dwSharetype;			//1-微信朋友圈 2-QQ朋友圈 3-微信好友

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_GetShareReq : public sStructBase
{
	CMD_ST_GetShareReq()
	{
		memset(this, 0, sizeof(CMD_ST_GetShareReq));
	}

	dword   				userid;              //玩家ID
	char 					cardnum[9];			 //卡号

	void toPack(PacketAide& packData);
};

struct CMD_ST_GetShareRsp
{
	dword   				ret;              //0:领取成功 1-分享码不存在 2-该设备已领取 3-你已经领取 4-自己不能领取自己的分享码

	dword  					money;            //500元宝
	dword 					gold;             //10000

	void fromPack(const PacketAide& packData);
};


/*************************************************
抢福袋活动请求(大协议号3,小协议号，请求：766，回复：767)
*************************************************/
//抢福袋请求
struct CMD_ST_FuDaiReq{
	dword 					userid;				//玩家ID
	dword   				optype;              //1:请求抢福袋 2:结束抢福袋
	dword 					gold;			 	//金币数量 29999
	dword 					money;				//元宝的数量50

	void toPack(PacketAide& packData);
};


//服务器返回抢福袋活动
struct CMD_ST_FuDaiRsp{
	dword   				optype;              //1:请求抢福袋 2:结束抢福袋
	dword 					ret;			 	 //0:成功  1-失败
	long 					lefttime;			 //剩余的秒

	void fromPack(const PacketAide& packData);
};

//充值成功后服务器返回数据,HALL_SUB_WEB_RECHARGERSP
struct CMD_Web_RechargeRsp
{
	dword 							userid;	//用户ID
	dword							productid;//产品ID
	dword							currgold; //最新金币
	dword 							currvip;  //当前VIP等级
	dword 							leftexp;  //下一VIP需要达到的金额
	dword 							sumrecharge; //玩家总充值


	void fromPack(const PacketAide& packData);
};

//请求红包收益操作 HALL_SUB_GP_HALL_SHOUYI_REQ
struct CMD_ST_ShouYiReq
{
	dword   				userid;              //
	dword 					optype;			 //1-查询收益 2-领取收益 3-查询排行榜

	void toPack(PacketAide& packData);
};

struct CMD_ST_ShouYiRsp
{
	dword 					optype;			//操作类型
	dword  					hongbao;        //发的红包数量
	dword 					gold;           //收益的金币
	dword 					money;          //收益的元宝

	void fromPack(const PacketAide& packData);
};


//排行结构体
typedef struct SyRank : public sStructBase
{
	SyRank()
	{
		memset(this, 0, sizeof(SyRank));
	}

	dword 							userid;
	char							account[30];					//名称
	dword                           nums;                           //红包数量
	dword                           moneys;                         //收益的元宝数量

}SyRankInfo;

//
//收益排行榜返回   HALL_SUB_GP_HALL_SHOUYIRANK_RSP			769
struct CMD_ST_ShouRankYiRsp
{
	SyRankInfo 	 ranks[10];						//显示10个排行	

	void fromPack(const PacketAide& packData);
};


//请求绑定账号
struct CMD_ST_BindAccountReq : public sStructBase
{
	CMD_ST_BindAccountReq()
	{
		memset(this, 0, sizeof(CMD_ST_BindAccountReq));
	}

	dword   				userid;
	char					szAccounts[LEN_ACCOUNTS];			//登录帐号
	char					szPassword[LEN_MD5];				//登录密码
	char					szMachineID[LEN_MACHINE_ID];		//机器序列

	void toPack(PacketAide& packData);
};

//请求账号绑定返回
struct CMD_ST_BindAccountRsp
{
	dword   				ret;         //0-绑定成功 1-绑定失败  

	void fromPack(const PacketAide& packData);
};

//请求好评
struct CMD_ST_HaoPingReq
{
	dword   				userid;         //用户ID

	void toPack(PacketAide& packData);
};

struct CMD_ST_HaoPingRsp
{
	dword   				ret;        //0成功 1-失败
	dword   				gold;       //获得的金币

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_ShareCountReq
{
	dword   				userid;         //用户ID
	dword 					optype;			//1-查询天数 2-领取天数奖励

	void toPack(PacketAide& packData);
};

struct CMD_ST_ShareCountRsp
{
	dword   				count;			//数量
	dword 					optype;			//1-查询天数 2-领取天数奖励
	dword 					getcount;		//已领取的宝箱

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_GetEmailAttReq
{
	dword   				userid;         //玩家ID
	dword   				euid;			//邮件ID
	dword 					optye;			// 1-领取附件 2-删除邮件	

	void toPack(PacketAide& packData);
};

struct CMD_ST_GetEmailAttRsp
{
	dword 					optye;		  // 1-领取附件 2-删除邮件	
	dword					ret;		  // 0-成功 1- 附件不存在

	void fromPack(const PacketAide& packData);
};


//----------------聊天------------------------
//[SUB_CHATMSG_REQ]
struct CMD_ST_ChatMsgReq : public sStructBase
{
	CMD_ST_ChatMsgReq()
	{
		memset(this, 0, sizeof(CMD_ST_ChatMsgReq));
	}

	dword 					userid; 
	dword 					msgtype;			// 0-大厅 1-水浒传 2-森林舞会 3-金沙银沙 4-捕鱼 100:世界聊天
	char					content[100];		//50个长度

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

	int		ntype;			//1-消息 2-红包
	int	    hid;			//红包ID
	int		ret;			//0：成功: 1：金币不足；2：今天发红包金额达到上限
	int		msgtype;		// 0-大厅 1-水浒传 2-森林舞会 3-金沙银沙 4-捕鱼 100:世界聊天
	int		userid;			//玩家ID
	int		viplevel;		//VIP等级
	int		faceid;			//头像ID
	SCORE	sendtime;		//发送时间
	char	uname[32];		//玩家名称 32
	char	content[100];	//50个长度

	void fromPack(const PacketAide& packData);
};


//请求发送红包,返回Msg_SendMsg_Rsp
struct CMD_ST_ChatHongBaoReq
{
	dword 					userid;
	dword					hongbaoid;			//红包的ID；1：200 2：1000 3：5000 4：10000
	dword					msgtype;			// 0-大厅 1-水浒传 2-森林舞会 3-金沙银沙 4-捕鱼 100:世界聊天

	void toPack(PacketAide& packData);
};

/// 领取红包
struct CMD_ST_ChatGetHongBaoReq
{
	dword 					userid;
	dword					hongbaoid;   //红包的ID； 

	void toPack(PacketAide& packData);
};

/// 领取人结构体
struct StHong
{
	StHong()
	{
		memset(this, 0, sizeof(StHong));
	}

	int userid;			//0：玩家ID
	int gold;			//领取的金币
	int viplevel;		//vip等级
	char uname[32];		//玩家名称 32

	void fromPack(const PacketAide& packData);
};


/// 领取红包返回
struct CMD_ST_ChatGetHongBaoRsp
{
	int ret;			//0：成功 1-不存在的红包 2-已经领取过 3-红包已领取完
	int gold;			//领取金币的数量 0：已被领取完了
	StHong hlist[8];	//领取的红包列表

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_ReplaceReq
{
	dword 					userid;
	dword					roomid;   //房间ID 0-大厅 1-水浒传 2-森林舞会 3-金沙银沙 4-捕鱼 

	void toPack(PacketAide& packData);
};

struct CMD_ST_NetFlushServerReq
{
	dword 					userid;	 //0:玩家ID
	dword 					roomid;   // 0-大厅 1-水浒传 2-森林舞会 3-金沙银沙 4-捕鱼 100:世界聊天

	void toPack(PacketAide& packData);
};

struct CMD_ST_NetFlushUserRsp
{
	dword 					money;		//玩家元宝
	dword 					cpvalue;	//船票值

	void fromPack(const PacketAide& packData);
};

struct CMD_ST_HeartReq
{
	dword 					userid;

	void toPack(PacketAide& packData);
};


//! 排行榜请求
struct CMD_GP_GET_RANK
{
	dword		dwUserID;	//! 用户ID
	byte		dwType;		//! 对应排行榜类型

	void toPack(PacketAide& packData);
};

//排行榜构体
typedef struct RankInfoSt
{
	dword		dwRank;						//排名
	dword		dwUserID;					//用户ID  [收到-1表示列表发送结束]
	char		szTitle[LEN_NICKNAME];		//排行奖励
	longlong	lScore;						//数值[资源数量]
	char		szUserName[LEN_NICKNAME];	//用户名字
	dword		dwVip;						//用户VIP等级

	void fromPack(const PacketAide& packData);

}RankInfo;

//! 排行榜结果返回
struct CMD_GP_GET_RANK_RESULT
{
	RankInfo	ranklist[31];					//排行榜列表

	void fromPack(const PacketAide& packData);
};

/*------------todosl游戏强制更新------------------*/
struct CMD_GP_UpdateNotify
{
	byte                    cbMustUpdate;                   // 强行升级
	byte                    cbAdviceUpdate;                 //建议升级
	dword                   dwCurrentVersion;	            //当前版本
	char                    szUpdateUrl[128];			    //更新地址

	void  fromPack(const PacketAide& packData);
};

//struct CMD_ST_HeartRsp
//{
//
//}

#pragma pack()


#endif









