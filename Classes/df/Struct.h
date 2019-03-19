#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE


#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//游戏列表

//游戏类型
struct tagGameType
{
	word							wJoinID;							//挂接索引
	word							wSortID;							//排序索引
	word							wTypeID;							//类型索引
	char							szTypeName[LEN_TYPE];				//种类名字
};

//游戏种类
struct tagGameKind
{
	word							wTypeID;							//类型索引
	word							wJoinID;							//挂接索引
	word							wSortID;							//排序索引
	word							wKindID;							//类型索引
	word							wGameID;							//模块索引
	dword							dwOnLineCount;						//在线人数
	dword							dwFullCount;						//满员人数
	char							szKindName[LEN_KIND];				//游戏名字
	char							szProcessName[LEN_PROCESS];			//进程名字
};

//游戏节点
struct tagGameNode
{
	word							wKindID;							//名称索引
	word							wJoinID;							//挂接索引
	word							wSortID;							//排序索引
	word							wNodeID;							//节点索引
	char							szNodeName[LEN_NODE];				//节点名称
};

//定制类型
struct tagGamePage
{
	word							wPageID;							//页面索引
	word							wKindID;							//名称索引
	word							wNodeID;							//节点索引
	word							wSortID;							//排序索引
	word							wOperateType;						//控制类型
	char							szDisplayName[LEN_PAGE];			//显示名称
};

//游戏房间
struct tagGameServer
{
	word							wServerID;							//房间索引
	word							wServerPort;						//房间端口
	char							szServerAddr[LEN_SERVERADDR];		//房间地址
};

//水浒传游戏房间
struct tagSHZ_GameServer
{
	word							wKindID;							//名称索引
	word							wNodeID;							//节点索引
	word							wSortID;							//排序索引
	word							wServerID;							//房间索引
	word							wServerPort;						//房间端口
	dword							dwOnLineCount;						//在线人数
	dword							dwFullCount;						//满员人数
	char							szServerAddr[LEN_SERVERADDR];		//房间地址
	char							szServerName[LEN_SERVER];			//房间名称
};

//森林舞会游戏房间扩展数据
struct tagSLWH_GameServer
{
	dword							dwScoreLimit;						//房间分数限制
	dword							dwVip;								//房间VIP限制	
};

//金鲨银鲨游戏房间扩展数据
struct tagShark_GameServer
{
	dword							roomScoreLimit;						//房间分数限制
	dword							roomVip;								//房间VIP限制	
};

//视频配置
struct tagAVServerOption
{
	word							wAVServerPort;						//视频端口
	dword							dwAVServerAddr;						//视频地址
};

//在线信息
struct tagOnLineInfoKind
{
	word							wKindID;							//类型标识
	dword							dwOnLineCount;						//在线人数
};

//在线信息
struct tagOnLineInfoServer
{
	word							wServerID;							//房间标识
	dword							dwOnLineCount;						//在线人数
};

//////////////////////////////////////////////////////////////////////////////////
//用户信息

//桌子状态
struct tagTableStatus
{
	byte							cbTableLock;						//锁定标志
	byte							cbPlayStatus;						//游戏标志
};

//用户状态
struct tagUserStatus
{
	word							wTableID;							//桌子索引
	word							wChairID;							//椅子位置
	byte							cbUserStatus;						//用户状态
};

//用户属性
struct tagUserAttrib
{
	byte							cbCompanion;						//用户关系
};

//用户积分
struct tagUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行

	//输赢信息
	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数
	dword							dwFleeCount;						//逃跑盘数

	//全局信息
	dword							dwUserMedal;						//用户奖牌
	dword							dwExperience;						//用户经验
	dword							lLoveLiness;						//用户魅力
};

//用户积分
struct tagMobileUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数

	//输赢信息
	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数
	dword							dwFleeCount;						//逃跑盘数

	//全局信息
	dword							dwExperience;						//用户经验
};


//道具使用
struct tagUsePropertyInfo
{
	word                            wPropertyCount;                     //道具数目
	word                            dwValidNum;						    //有效数字
	dword                           dwEffectTime;                       //生效时间
};


//用户道具
struct tagUserProperty
{
	word                            wPropertyUseMark;                   //道具标示
	tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//使用信息   
};

//道具包裹
struct tagPropertyPackage
{
	word                            wTrumpetCount;                     //小喇叭数
	word                            wTyphonCount;                      //大喇叭数
};

//时间信息
struct tagTimeInfo
{
	dword						dwEnterTableTimer;						//进出桌子时间
	dword						dwLeaveTableTimer;						//离开桌子时间
	dword						dwStartGameTimer;						//开始游戏时间
	dword						dwEndGameTimer;							//离开游戏时间
};

//用户信息
struct tagUserInfo
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

	//ToDoSL
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
	tagTimeInfo						TimerInfo;							//时间信息

	//比赛信息
	byte							cbEnlistStatus;						//报名状态

	//扩展标识
	dword							lExpand;
	dword							dwExpand;

	//新添加的信息
	dword                            wVipState;                          //Vip状态
	dword                            wCanonType;                         //用户炮台类型
    
};

//水浒传用户信息
struct tagSHZ_UserInfo
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
	tagTimeInfo						TimerInfo;							//时间信息

	//比赛信息
	byte							cbEnlistStatus;						//报名状态

	//扩展标识
	dword							lExpand;
	dword							dwExpand;
};


//用户信息
//CMD_GR_UserInfo
struct tagUserInfoHead
{
	//用户属性
	dword							dwGameID;							//游戏 I D
	dword							dwUserID;							//用户 I D
	dword							dwGroupID;							//社团 I D

	//头像信息
	word							wFaceID;							//头像索引
	dword							dwCustomID;							//自定标识

	//用户属性
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
};

//头像信息
struct tagCustomFaceInfo
{
	dword							dwDataSize;							//数据大小
	dword							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//用户信息
struct tagUserRemoteInfo
{
	//用户信息
	dword							dwUserID;							//用户标识
	dword							dwGameID;							//游戏标识
	char							szNickName[LEN_NICKNAME];			//用户昵称

	//等级信息
	byte							cbGender;							//用户性别
	byte							cbMemberOrder;						//会员等级
	byte							cbMasterOrder;						//管理等级

	//位置信息
	word							wKindID;							//类型标识
	word							wServerID;							//房间标识
	char							szGameServer[LEN_SERVER];			//房间位置
};

//////////////////////////////////////////////////////////////////////////////////

//广场子项
struct tagGamePlaza
{
	word							wPlazaID;							//广场标识
	char							szServerAddr[32];					//服务地址
	char							szServerName[32];					//服务器名
};

//级别子项
struct tagLevelItem
{
	dword							lLevelScore;						//级别积分
	char							szLevelName[16];					//级别描述
};

//会员子项
struct tagMemberItem
{
	byte							cbMemberOrder;						//等级标识
	char							szMemberName[16];					//等级名字
};

//管理子项
struct tagMasterItem
{
	byte							cbMasterOrder;						//等级标识
	char							szMasterName[16];					//等级名字
};

//列表子项
struct tagColumnItem
{
	byte							cbColumnWidth;						//列表宽度
	byte							cbDataDescribe;						//字段类型
	char							szColumnName[16];					//列表名字
};

//地址信息
struct tagAddressInfo
{
	char							szAddress[32];						//服务地址
};

//数据信息
struct tagDataBaseParameter
{
	word							wDataBasePort;						//数据库端口
	char							szDataBaseAddr[32];					//数据库地址
	char							szDataBaseUser[32];					//数据库用户
	char							szDataBasePass[32];					//数据库密码
	char							szDataBaseName[32];					//数据库名字
};

//房间配置
struct tagServerOptionInfo
{
	//挂接属性
	word							wKindID;							//挂接类型
	word							wNodeID;							//挂接节点
	word							wSortID;							//排列标识

	//税收配置
	word							wRevenueRatio;						//税收比例
	SCORE							lServiceScore;						//服务费用

	//房间配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分

	//会员限制
	byte							cbMinEnterMember;					//最低会员
	byte							cbMaxEnterMember;					//最高会员

	//房间属性
	dword							dwServerRule;						//房间规则
	char							szServerName[LEN_SERVER];			//房间名称
};

//用户信息
struct tagMobileUserInfoHead
{
	//用户属性
	dword							dwGameID;							//游戏 I D
	dword							dwUserID;							//用户 I D

	//头像信息
	word							wFaceID;							//头像索引
	dword							dwCustomID;							//自定标识

	//用户属性
	byte							cbGender;							//用户性别
	byte							cbMemberOrder;						//会员等级

	//用户状态
	word							wTableID;							//桌子索引
	word							wChairID;							//椅子索引
	byte							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数

	//游戏信息
	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数
	dword							dwFleeCount;						//逃跑盘数
	dword							dwExperience;						//用户经验
};

//////////////////////////////////////////////////////////////////////////////////
//比赛信息

//赛事信息
struct tagMatchInfo
{
	char							szTitle[4][64];						//信息标题
	word							wGameCount;							//游戏局数
};

//提示信息
struct tagMatchWaitTip
{
	SCORE							lScore;								//当前积分
	word							wRank;								//当前名次
	word							wCurTableRank;						//本桌名次
	word							wUserCount;							//当前人数
	word							wPlayingTable;						//游戏桌数
	char							szMatchName[LEN_SERVER];			//比赛名称
};

//比赛结果
struct tagMatchResult
{
	char							szDescribe[256];					//得奖描述
	dword							dwGold;								//金币奖励
	dword							dwMedal;							//奖牌奖励
	dword							dwExperience;						//经验奖励
};

//比赛描述
struct tagMatchDesc
{
	char							szTitle[4][16];						//信息标题
	char							szDescribe[4][64];					//描述内容
	dword							crTitleColor;						//标题颜色
	dword							crDescribeColor;					//描述颜色
};

//////////////////////////////////////////////////////////////////////////////////
//排行榜
//排行榜项
struct tagRankItem
{
	dword							dwUserID;							//用户ID
	SCORE							lScore;								//用户积分
	unsigned short					szNickname[LEN_NICKNAME];			//用户昵称
};

#pragma pack()

#endif