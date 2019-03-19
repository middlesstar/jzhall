#ifndef SHZ_CMD_LONGON_HEAD_FILE
#define SHZ_CMD_LONGON_HEAD_FILE

#pragma pack(1)
#include "df/types.h"
//////////////////////////////////////////////////////////////////////////

struct SHZsystemtime {
	word wYear;
	word wMonth;
	word wDayOfWeek;
	word wDay;
	word wHour;
	word wMinute;
	word wSecond;
	word wMilliseconds;
};

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON_SHZ				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID_SHZ			1									//I D 登录
#define SUB_GP_LOGON_ACCOUNTS_SHZ		2									//帐号登录

//登录结果
#define SUB_GP_LOGON_SUCCESS_SHZ		100									//登录成功
#define SUB_GP_LOGON_FAILURE_SHZ		101									//登录失败
#define SUB_GP_LOGON_FINISH_SHZ			102									//登录完成
#define SUB_GP_VALIDATE_MBCARD_SHZ      103                                 //登录失败

//升级提示
#define SUB_GP_UPDATE_NOTIFY_SHZ		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS_SHZ           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS_SHZ      0x02                                //效验低版本


//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO_SHZ			1									//社团信息
#define DTP_GP_MEMBER_INFO_SHZ			2									//会员信息
#define	DTP_GP_UNDER_WRITE_SHZ			3									//个性签名
#define DTP_GP_STATION_URL_SHZ			4									//主页信息



//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST_SHZ			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST_SHZ				1									//获取列表
#define SUB_GP_GET_SERVER_SHZ			2									//获取房间
#define SUB_GP_GET_ONLINE_SHZ			3									//获取在线
#define SUB_GP_GET_COLLECTION_SHZ		4									//获取收藏

//列表信息
#define SUB_GP_LIST_TYPE_SHZ			100									//类型列表
#define SUB_GP_LIST_KIND_SHZ			101									//种类列表
#define SUB_GP_LIST_NODE_SHZ			102									//节点列表
#define SUB_GP_LIST_PAGE_SHZ			103									//定制列表
#define SUB_GP_LIST_SERVER_SHZ			104									//房间列表
#define SUB_GP_VIDEO_OPTION_SHZ			105									//视频配置

//完成信息
#define SUB_GP_LIST_FINISH_SHZ			200									//发送完成
#define SUB_GP_SERVER_FINISH_SHZ		201									//房间完成

//在线信息
#define SUB_GR_KINE_ONLINE_SHZ			300									//类型在线
#define SUB_GR_SERVER_ONLINE_SHZ		301									//房间在线
#define SUB_GR_ONLINE_FINISH_SHZ		302									//在线完成


//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE_SHZ			3									//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE_SHZ		100									//修改机器
#define SUB_GP_MODIFY_LOGON_PASS_SHZ	101									//修改密码
#define SUB_GP_MODIFY_INSURE_PASS_SHZ	102									//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE_SHZ	103									//修改签名

//修改头像
#define SUB_GP_USER_FACE_INFO_SHZ		200									//头像信息
#define SUB_GP_SYSTEM_FACE_INFO_SHZ		201									//系统头像
#define SUB_GP_CUSTOM_FACE_INFO_SHZ		202									//自定头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL_SHZ		301									//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL_SHZ		302									//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL_SHZ	303									//修改资料

//银行服务
#define SUB_GP_USER_SAVE_SCORE_SHZ		400									//存款操作
#define SUB_GP_USER_TAKE_SCORE_SHZ		401									//取款操作
#define SUB_GP_USER_TRANSFER_SCORE_SHZ	402									//转账操作
#define SUB_GP_USER_INSURE_INFO_SHZ		403									//银行资料
#define SUB_GP_QUERY_INSURE_INFO_SHZ	404									//查询银行
#define SUB_GP_USER_INSURE_SUCCESS_SHZ	405									//银行成功
#define SUB_GP_USER_INSURE_FAILURE_SHZ	406									//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST_SHZ	407								//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT_SHZ	408								//用户信息

//兑换道具
#define SUB_GP_EXCHANGE_SHZ				500									//兑换道具
#define SUB_GP_EXCHANGE_RESULT_SHZ		501									//兑换结果

//客服信息
#define SUB_GP_KEFU_SHZ					600									//客服信息
#define SUB_GP_KEFU_RESULT_SHZ			601									//客服信息结果

//公告信息
#define SUB_GP_LINE_GAMENOTICE_SHZ		4		
#define SUB_GP_GAME_NOTICE_SHZ			300

//赠送信息
#define SUB_GP_GIVEAWAY_SHZ				5									//赠送
#define SUB_GP_GIVEAWAY_RESULT_SHZ		800

//赠送下限
#define SUB_GP_LOWERLIMIT_SHZ			6


//邮件信息
#define SUB_GP_MESSAGE_LIST_SHZ			 700								//邮件列表
#define SUB_GP_MESSAGE_LIST_RESULT_SHZ	 701								//邮件列表返回
#define SUB_GP_MESSAGE_AWARD_SHZ		 702								//获取排行榜奖励
#define SUB_GP_MESSAGE_AWARD_RESULT_SHZ	 703								//获取排行榜结果
#define SUB_GP_MESSAGE_COUNT_SHZ         711                                //获得邮件的个数
#define SUB_GP_MESSAGE_COUNT_RESULT_SHZ  712                                //邮件的个数

#define SUB_GP_RANK_LIST_SHZ			704									//排行榜列表
#define SUB_GP_RANKITEM_LIST_RESULT_SHZ 705									//排行榜列表返回

#define	SUB_GP_QUANPAN_GET_SHZ			706									//获得全盘状态
#define SUB_GP_QUANPAN_SET_SHZ			707									//设置全盘状态
#define SUB_GP_QUANPAN_GET_RESULT_SHZ	708									//获得全盘状态的结果


#define SUB_GP_SHARE_POST_SHZ           709                                 //分享成功
#define SUB_GP_SHARE_RESULT_SHZ         710                                 //分享返回

#define SUB_GP_RECORD_CHARGE_SHZ        716									//记录充值信息

#define SUB_GP_QUERY_FIRST_CHARGE_SHZ   717									//查询第一次充值的记录
#define SUB_GP_QUERY_RESULT_SHZ         718									//查询结果
#define SUB_GP_ORDER_INQUERY_SHZ		719									//订单确认
#define SUB_GP_ORDER_INQUERY_RESULT_SHZ 720									//订单确认返回结果
#define SUB_GP_RECORD_RANK_DATA_SHZ     721									//记录比赛数据
#define SUB_GP_GET_NOTIFY_SHZ           722									//得到公告信息
#define SUB_GP_NOTIFY_RESULT_SHZ        723									//返回公告的信息
#define SUB_GP_MATCH_RANK_SHZ           724							        //得到比赛的排行榜
#define SUB_GP_MATCH_RANK_RESULT_SHZ    725									//得到比赛的排行榜结果

#define SUB_GP_VIP_GET_SHZ				730									//Vip领取
#define SUB_GP_VIP_GET_RESULT_SHZ		731									//Vip领取结果
#define SUB_GP_VIP_CHECK_SHZ			732									//Vip等级检查
#define SUB_GP_VIP_CHECK_RESULT_SHZ		733									//Vip等级检查结果
#define SUB_GP_VIP_BUY_SHZ				734									//Vip购买检查
#define SUB_GP_VIP_BUY_RESULT_SHZ		735									//Vip购买检查结果
//操作结果
#define SUB_GP_OPERATE_SUCCESS_SHZ		900									//操作成功
#define SUB_GP_OPERATE_FAILURE_SHZ		901									//操作失败

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME_SHZ			1									//用户昵称
#define DTP_GP_UI_USER_NOTE_SHZ			2									//用户说明
#define DTP_GP_UI_UNDER_WRITE_SHZ		3									//个性签名
#define DTP_GP_UI_QQ_SHZ				4									//Q Q 号码
#define DTP_GP_UI_EMAIL_SHZ				5									//电子邮件
#define DTP_GP_UI_SEAT_PHONE_SHZ		6									//固定电话
#define DTP_GP_UI_MOBILE_PHONE_SHZ		7									//移动电话
#define DTP_GP_UI_COMPELLATION_SHZ		8									//真实名字
#define DTP_GP_UI_DWELLING_PLACE_SHZ	9									//联系地址

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE_SHZ		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE_SHZ	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND_SHZ	101									//协调查找

//时间奖励
#define SUB_GP_C_TIME_AWARD_CHECK_SHZ	110									//时间奖励信息查询
#define SUB_GP_C_TIME_AWARD_GET_SHZ		111									//时间奖励领取

//查找服务
#define SUB_GP_S_SEARCH_DATABASE_SHZ	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND_SHZ	201									//协调查找


//时间奖励
#define SUB_GP_S_TIME_AWARD_CHECK_SHZ	210									//时间奖励信息查询结果
#define SUB_GP_S_TIME_AWARD_GET_SHZ		211									//时间奖励领取结果

#pragma pack()

#endif
