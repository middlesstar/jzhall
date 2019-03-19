#ifndef SHZ_OPTION_PARAMETER_HEAD_FILE
#define SHZ_OPTION_PARAMETER_HEAD_FILE

#include "shuihu/plazz/Plazz_SHZ.h"
#include <map>

//配置信息
#define REG_GOBAL_OPTION_SHZ			"GolbalOption"				//全局信息


//////////////////////////////////////////////////////////////////////////////////
//枚举定义

//邀请模式
#define INVITE_MODE_ALL_SHZ				0									//全部显示
#define INVITE_MODE_NONE_SHZ			1									//全不显示
#define INVITE_MODE_FRIEND_SHZ			2									//好友显示

//消息模式
#define MESSAGE_MODE_ALL_SHZ			0									//全部显示
#define MESSAGE_MODE_DETEST_SHZ			1									//屏蔽厌恶
#define MESSAGE_MODE_FRIEND_SHZ			2									//社团好友
#define MESSAGE_MODE_NONE_SHZ			3									//全不显示

//界面动作
#define ACTION_ORIENTATION_SHZ			0									//定位用户
#define ACTION_SEND_WHISPER_SHZ			1									//发送私聊
#define ACTION_SHOW_USER_INFO_SHZ		2									//用户信息
#define ACTION_SEARCH_TABLE_SHZ			3									//寻找位置
#define ACTION_SHOW_SEARCH_DLG_SHZ		4									//查找界面

//////////////////////////////////////////////////////////////////////////////////

//游戏参数
class SHZCParameterGame
{
	//友元定义
	friend class SHZCParameterGlobal;

	//胜率限制
public:
	word							m_wMinWinRate;						//最低胜率
	bool							m_bLimitWinRate;					//限制胜率

	//逃率限制
public:
	word							m_wMaxFleeRate;						//最高逃跑
	bool							m_bLimitFleeRate;					//限制断线

	//积分限制
public:
	long							m_lMaxGameScore;					//最高分数 
	long							m_lMinGameScore;					//最低分数
	bool							m_bLimitGameScore;					//限制分数

	//属性变量
protected:
	char							m_szRegKeyName[16];					//注册项名

	//函数定义
public:
	//构造函数
	SHZCParameterGame();
	//析构函数
	virtual ~SHZCParameterGame();

	//功能函数
public:
	//加载参数
	void LoadParameter();
	//保存参数
	void SaveParameter();
	//默认参数
	void DefaultParameter();

	//配置函数
protected:
	//配置参数
	bool InitParameter(const char* pszProcessName);
};

//////////////////////////////////////////////////////////////////////////////////

//房间参数
class SHZCParameterServer
{
	//配置变量
public:
	bool							m_bTakePassword;					//密码标志
	char							m_szPassword[LEN_PASSWORD];			//桌子密码

	//函数定义
public:
	//构造函数
	SHZCParameterServer();
	//析构函数
	virtual ~SHZCParameterServer();

	//配置函数
public:
	//默认参数
	void DefaultParameter();
};

//////////////////////////////////////////////////////////////////////////////////
typedef std::map<word,SHZCParameterGame *> CParameterGameMap_SHZ;
typedef std::map<word,SHZCParameterServer *> CParameterServerMap_SHZ;

//全局参数
class SHZCParameterGlobal
{
	//静态函数
public:
	//获取对象
	static SHZCParameterGlobal* shared();
	static void purge();

	//登录配置
public:
	bool							m_bLogonAuto;						//自动登录
	bool							m_bWriteCookie;						//写入甜饼
	bool							m_bRemberPassword;					//记住密码

	//模式配置
public:
	byte							m_cbInviteMode;						//邀请模式
	byte							m_cbMessageMode;					//消息模式
	byte							m_cbActionHitAutoJoin;				//自动加入
	byte							m_cbActionLeftDoubleList;			//双击列表

	//时间配置
public:
	word							m_wIntermitTime;					//中断时间
	word							m_wOnLineCountTime;					//人数时间

	//房间配置
public:
	bool							m_bLimitDetest;						//限制厌恶
	bool							m_bLimitSameIP;						//限制地址
	bool							m_bNotifyUserInOut;					//进出消息
	bool							m_bNotifyFriendCome;				//好友提示

	//系统配置
public:
	bool							m_bAllowSound;						//允许声音
	bool							m_bAllowBackMusic;					//允许背景音乐
	bool							m_bAutoSitDown;						//自动坐下
	bool							m_bAutoShowWhisper;					//显示私聊
	bool							m_bSaveWhisperChat;					//保存私聊
	bool							m_bSendWhisperByEnter;				//回车发送

	//配置组件
protected:
	CParameterGameMap_SHZ				m_ParameterGameMap;					//游戏配置
	CParameterServerMap_SHZ				m_ParameterServerMap;				//房间配置

	//显示配置
public:
	bool                            m_bShowServerStatus;                //显示服务器状态

	//函数定义
public:
	//构造函数
	SHZCParameterGlobal();
	//析构函数
	virtual ~SHZCParameterGlobal();

	//配置函数
public:
	//加载参数
	void LoadParameter();
	//保存参数
	void SaveParameter();
	//默认参数
	void DefaultParameter();

	//游戏配置
public:
	//游戏配置
	SHZCParameterGame * GetParameterGame(tagGameKind * pGameKind);
	//房间配置
	SHZCParameterServer * GetParameterServer(tagSHZ_GameServer * pGameServer);
};

//////////////////////////////////////////////////////////////////////////////////

#endif