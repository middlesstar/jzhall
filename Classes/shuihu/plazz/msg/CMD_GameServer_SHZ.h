#ifndef SHZ_CMD_GAME_SERVER_HEAD_FILE
#define SHZ_CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GR_LOGON_SHZ				1									//登录信息

//登录模式
#define SUB_GR_LOGON_USERID_SHZ			1									//I D 登录
#define SUB_GR_LOGON_MOBILE_SHZ			2									//手机登录
#define SUB_GR_LOGON_ACCOUNTS_SHZ		3									//帐户登录

//登录结果
#define SUB_GR_LOGON_SUCCESS_SHZ		100									//登录成功
#define SUB_GR_LOGON_FAILURE_SHZ		101									//登录失败
#define SUB_GR_LOGON_FINISH_SHZ			102									//登录完成

//升级提示
#define SUB_GR_UPDATE_NOTIFY_SHZ		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//配置命令

#define MDM_GR_CONFIG_SHZ				2									//配置信息

#define SUB_GR_CONFIG_COLUMN_SHZ		100									//列表配置
#define SUB_GR_CONFIG_SERVER_SHZ		101									//房间配置
#define SUB_GR_CONFIG_PROPERTY_SHZ		102									//道具配置
#define SUB_GR_CONFIG_FINISH_SHZ		103									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT_SHZ	104									//玩家权限

//////////////////////////////////////////////////////////////////////////////////

#define MDM_GR_USER_SHZ					3									//用户信息

//用户动作
#define SUB_GR_USER_RULE_SHZ			1									//用户规则
#define SUB_GR_USER_LOOKON_SHZ			2									//旁观请求
#define SUB_GR_USER_SITDOWN_SHZ			3									//坐下请求
#define SUB_GR_USER_STANDUP_SHZ			4									//起立请求
#define SUB_GR_USER_INVITE_SHZ			5									//用户邀请
#define SUB_GR_USER_INVITE_REQ_SHZ		6									//邀请请求
#define SUB_GR_USER_REPULSE_SIT_SHZ  	7									//拒绝玩家坐下
#define SUB_GR_USER_KICK_USER_SHZ       8                                   //踢出用户
#define SUB_GR_USER_INFO_REQ_SHZ        9                                   //请求用户信息
#define SUB_GR_USER_CHAIR_REQ_SHZ       10                                  //请求更换位置
#define SUB_GR_USER_CHAIR_INFO_REQ_SHZ  11                                  //请求椅子用户信息
#define SUB_GR_USER_WAIT_DISTRIBUTE_SHZ 12									//等待分配

//用户状态
#define SUB_GR_USER_ENTER_SHZ			100									//用户进入
#define SUB_GR_USER_SCORE_SHZ			101									//用户分数
#define SUB_GR_USER_STATUS_SHZ			102									//用户状态
#define SUB_GR_REQUEST_FAILURE_SHZ		103									//请求失败

//聊天命令
#define SUB_GR_USER_CHAT_SHZ			201									//聊天消息
#define SUB_GR_USER_EXPRESSION_SHZ		202									//表情消息
#define SUB_GR_WISPER_CHAT_SHZ			203									//私聊消息
#define SUB_GR_WISPER_EXPRESSION_SHZ	204									//私聊表情
#define SUB_GR_COLLOQUY_CHAT_SHZ		205									//会话消息
#define SUB_GR_COLLOQUY_EXPRESSION_SHZ	206									//会话表情

//道具命令
#define SUB_GR_PROPERTY_BUY_SHZ			300									//购买道具
#define SUB_GR_PROPERTY_SUCCESS_SHZ		301									//道具成功
#define SUB_GR_PROPERTY_FAILURE_SHZ		302									//道具失败
#define SUB_GR_PROPERTY_MESSAGE_SHZ     303                                 //道具消息
#define SUB_GR_PROPERTY_EFFECT_SHZ      304                                 //道具效应
#define SUB_GR_PROPERTY_TRUMPET_SHZ		305                                 //喇叭消息

#define SUB_GR_GLAD_MESSAGE_SHZ			400                                 //喜报消息


//////////////////////////////////////////////////////////////////////////////////

//规则标志
#define UR_LIMIT_SAME_IP_SHZ			0x01								//限制地址
#define UR_LIMIT_WIN_RATE_SHZ			0x02								//限制胜率
#define UR_LIMIT_FLEE_RATE_SHZ			0x04								//限制逃率
#define UR_LIMIT_GAME_SCORE_SHZ			0x08								//限制积分

//////////////////////////////////////////////////////////////////////////////////
//状态命令

#define MDM_GR_STATUS_SHZ				4									//状态信息

#define SUB_GR_TABLE_INFO_SHZ			100									//桌子信息
#define SUB_GR_TABLE_STATUS_SHZ			101									//桌子状态

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//银行命令

#define MDM_GR_INSURE_SHZ				5									//用户信息

//银行命令
#define SUB_GR_QUERY_INSURE_INFO_SHZ	1									//查询银行
#define SUB_GR_SAVE_SCORE_REQUEST_SHZ	2									//存款操作
#define SUB_GR_TAKE_SCORE_REQUEST_SHZ	3									//取款操作
#define SUB_GR_TRANSFER_SCORE_REQUEST_SHZ	4								//取款操作
#define SUB_GR_QUERY_USER_INFO_REQUEST_SHZ	5								//查询用户

#define SUB_GR_USER_INSURE_INFO_SHZ		100									//银行资料
#define SUB_GR_USER_INSURE_SUCCESS_SHZ	101									//银行成功
#define SUB_GR_USER_INSURE_FAILURE_SHZ	102									//银行失败
#define SUB_GR_USER_TRANSFER_USER_INFO_SHZ	103								//用户资料

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//管理命令

#define MDM_GR_MANAGE_SHZ				6									//管理命令

#define SUB_GR_SEND_WARNING_SHZ			1									//发送警告
#define SUB_GR_SEND_MESSAGE_SHZ			2									//发送消息
#define SUB_GR_LOOK_USER_IP_SHZ			3									//查看地址
#define SUB_GR_KILL_USER_SHZ			4									//踢出用户
#define SUB_GR_LIMIT_ACCOUNS_SHZ		5									//禁用帐户
#define SUB_GR_SET_USER_RIGHT_SHZ		6									//权限设置

//房间设置
#define SUB_GR_QUERY_OPTION_SHZ			7									//查询设置
#define SUB_GR_OPTION_SERVER_SHZ		8									//房间设置
#define SUB_GR_OPTION_CURRENT_SHZ		9									//当前设置

#define SUB_GR_LIMIT_USER_CHAT_SHZ		10									//限制聊天

#define SUB_GR_KICK_ALL_USER_SHZ		11									//踢出用户
#define SUB_GR_DISMISSGAME_SHZ		    12									//解散游戏

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//设置标志
#define OSF_ROOM_CHAT_SHZ				1									//大厅聊天
#define OSF_GAME_CHAT_SHZ				2									//游戏聊天
#define OSF_ROOM_WISPER_SHZ				3									//大厅私聊
#define OSF_ENTER_TABLE_SHZ				4									//进入游戏
#define OSF_ENTER_SERVER_SHZ			5									//进入房间
#define OSF_SEND_BUGLE_SHZ				12									//发送喇叭

//////////////////////////////////////////////////////////////////////////////////
//比赛命令

#define MDM_GR_MATCH_SHZ				7									//比赛命令

#define SUB_GR_MATCH_FEE_SHZ			400									//报名费用
#define SUB_GR_MATCH_NUM_SHZ			401									//等待人数
#define SUB_GR_LEAVE_MATCH_SHZ			402									//退出比赛
#define SUB_GR_MATCH_INFO_SHZ			403									//比赛信息
#define SUB_GR_MATCH_WAIT_TIP_SHZ		404									//等待提示
#define SUB_GR_MATCH_RESULT_SHZ			405									//比赛结果
#define SUB_GR_MATCH_STATUS_SHZ			406									//比赛状态
#define SUB_GR_MATCH_DESC_SHZ			408									//比赛描述

////////////////改动以下时 请将游戏里面CMD_GAME.H的同时改动////////////////////////////
#define SUB_GR_MATCH_INFO_ER_SPARROWS_SHZ	410									//比赛信息(2人麻将)

//////////////////////////////////////////////////////////////////////////////////
//框架命令

#define MDM_GF_FRAME_SHZ				100									//框架命令

//////////////////////////////////////////////////////////////////////////////////
//框架命令

//用户命令
#define SUB_GF_GAME_OPTION_SHZ			1									//游戏配置
#define SUB_GF_USER_READY_SHZ			2									//用户准备
#define SUB_GF_LOOKON_CONFIG_SHZ		3									//旁观配置

//聊天命令
#define SUB_GF_USER_CHAT_SHZ			10									//用户聊天
#define SUB_GF_USER_EXPRESSION_SHZ		11									//用户表情

//游戏信息
#define SUB_GF_GAME_STATUS_SHZ			100									//游戏状态
#define SUB_GF_GAME_SCENE_SHZ			101									//游戏场景
#define SUB_GF_LOOKON_STATUS_SHZ		102									//旁观状态

//系统消息
#define SUB_GF_SYSTEM_MESSAGE_SHZ		200									//系统消息
#define SUB_GF_ACTION_MESSAGE_SHZ		201									//动作消息

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//游戏命令

#define MDM_GF_GAME_SHZ					200									//游戏命令

//////////////////////////////////////////////////////////////////////////////////
//携带信息

//其他信息
#define DTP_GR_TABLE_PASSWORD_SHZ		1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME_SHZ			10									//用户昵称
#define DTP_GR_GROUP_NAME_SHZ			11									//社团名字
#define DTP_GR_UNDER_WRITE_SHZ			12									//个性签名

//附加信息
#define DTP_GR_USER_NOTE_SHZ			20									//用户备注
#define DTP_GR_CUSTOM_FACE_SHZ			21									//自定头像

//////////////////////////////////////////////////////////////////////////////////

//请求错误
#define REQUEST_FAILURE_NORMAL_SHZ		0									//常规原因
#define REQUEST_FAILURE_NOGOLD_SHZ		1									//金币不足
#define REQUEST_FAILURE_NOSCORE_SHZ		2									//积分不足
#define REQUEST_FAILURE_PASSWORD_SHZ	3									//密码错误

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif
