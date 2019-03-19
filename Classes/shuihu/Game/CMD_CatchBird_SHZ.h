#ifndef SHZ_CMD_GAME_HEAD_FILE
#define SHZ_CMD_GAME_HEAD_FILE

#include "shuihu/plazz/Plazz_SHZ.h"
#pragma pack(1)
#define UR_GAME_CONTROL					0x20000000L				//游戏特殊控制

//////////////////////////////////////////////////////////////////////////////////
//服务定义

//游戏属性
#define KIND_ID_SHZ						203  								//游戏 I D
#define GAME_NAME_SHZ					TEXT("水浒传")	//游戏名字

//组件属性
#define GAME_PLAYER_SHZ					1									//游戏人数
#define VERSION_SERVER_SHZ				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT_SHZ				PROCESS_VERSION(6,0,3)				//程序版本

#define GAME_SCENE_FREE_SHZ				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_PLAY_SHZ				GAME_STATUS_PLAY					//叫分状态

const float kRevolutionWidth_SHZ  = 1074;
const float kRevolutionHeight_SHZ = 740;
//命令定义
enum SHZ_enServerSubCmd
{
	enServerSubCmd_Begin_SHZ=100,
	SUB_S_GAMESCENE_SHZ,									//场景消息
	SUB_S_GAMESTART_SHZ,									//游戏开始
	SUB_S_UPORDOWNESCORE_SHZ,								//游戏上下分
	SUB_S_ADDSCORE_SHZ,									    //游戏加注
	SUB_S_TYPESCROLL_SHZ,									//启动滚动
	SUB_S_GETSCORE_SHZ,									    //获得积分
	SUB_S_COMEBIBEI_SHZ	,								    //进入比倍
	SUB_S_BIBEISTART_SHZ,								    //比倍结果
	SUB_S_MALISTART_SHZ,								    //马力开始
	SUB_S_MESSAGEINFOR_SHZ,
	SUB_S_BIBEIDATAM_SHZ,                                   //比倍数据
	SUB_S_ADDBEI_SHZ,										//加倍
	SUB_S_GAMEMSG_SHZ,										//游戏公告
	SUB_S_REWARD_SHZ,
	SUB_S_LEVEL_UP_SHZ,                                     //用户升级
	SUB_S_DISABLE_GAME_SHZ,                                 //不能游戏
	SUB_S_YA_LINE_SHZ,                                      //设置压线
	   
	enServerSubCmd_End_SHZ
};
#define SUB_S_STORAGE_SHZ                   118
#define SUB_S_REFRESH_SHZ					119				//配置刷新
//小马力游戏
struct CMD_S_MaLi_SHZ
{
	//马力次数
	int                            m_GameCount;
	
	//4个图
	int                            m_ThreeTypeId[4];
	
	//转转图
	int                            m_TypeIndex;
	
	//当前分
	int                            m_AddScore;
	
	//是否马上开始小玛利
	bool                           m_NowBegin;
};
//消息数据包
struct CMD_S_GameMessage_SHZ
{
	char						   szContent[128];				//消息内容
};

//场景消息
struct CMD_S_GameScene_SHZ
{
	//单位分数
	int                            m_CellScore;
	//已经上的分
	int                            m_UserScore;

};
//上下分数
struct CMD_S_UpOrDownScore_SHZ
{
	//当前上分
	int                            m_UserScore;
	
};
//上注分数
struct CMD_S_AddScore_SHZ
{
	//当前上注
	int                            m_AddScore;
	
};

struct  CMD_S_Addbei_SHZ
{
	int								iaddCount;
};

//赢取分数
struct CMD_S_WinScore_SHZ
{
	//当前赢分
	int                            m_WinScore;
	//获得方式
	int                            m_GameState;
	//
};
//滚动信息
struct CMD_S_TypeScroll_SHZ
{
	//当前上分
	int                            m_UserScore;
	//输赢分数
	int                            m_WinScore;
	//图形数据
	int                            m_TypeImgid[15];

	bool						   bquanpan;
	//
};
//比倍结果
struct CMD_S_BiBeiRelult_SHZ
{
	//塞子一
	int                            m_S1;
	//塞子二
	int                            m_S2;
	//图形数据
	int                            m_WinScore;

};
//比倍数据
struct CMD_S_BiBeiData_SHZ
{
	//比倍数据
	int                            m_BiBeiData[10];

};

//////////////////////////////////////////////////////////////////////////
//命令定义q
enum SHZ_enClientSubCmd
{
	enClientSubCmd_Begin_SHZ=200,
	SUB_C_GAMESCENE_SHZ,									//场景消息
	SUB_C_GAMESTART_SHZ,									//游戏开始
	SUB_C_UPORDOWNESCORE_SHZ,								//游戏上下分
	SUB_C_ADDSCORE_SHZ,									    //游戏加注
	SUB_C_TYPESCROLL_SHZ,									//启动滚动
	SUB_C_GETSCORE_SHZ,									    //获得积分
	SUB_C_COMEBIBEI_SHZ	,								    //进入比倍
	SUB_C_BIBEISTART_SHZ,								    //比倍启动
	SUB_C_MALISTART_SHZ,								    //马力开始
	SUB_C_REQUESTBEIBEI_SHZ,								//要求比倍
	SUB_C_ADDMAXSCORE_SHZ,									//最大加注
	SUB_C_CURSCORE_SHZ,										//减注
	SUB_C_LINE_SHZ,											//减线
	SUB_C_ADDBEI_SHZ,										//加倍
	SUB_C_CURBEI_SHZ,										//减倍
	SUB_C_REWARD_SHZ,                                       //充值
	enClientSubCmd_End_SHZ
};
 
#define SUB_C_USER_FILTER_SHZ               9  
#define SUB_C_STORAGE_SHZ                   10 
//玩家比倍
struct SHZ_CMD_C_BiBeiStart_SHZ
{
	//买大买小买和,0,大,1,小,2,和
	int                             m_MaxOrOther;
	//比倍方式,0,比倍,1,半比倍,2,全比倍
	int                             m_BiBeiType;
	//
};
//玩家上下分
struct CMD_C_UpOrDownScore_SHZ
{
	//上下判定 
	bool                            m_bUpOrDown;
	//
};//

//线
struct CMD_C_Line_SHZ
{
	int								iCurLine;

};
//获取得分
struct CMD_C_GetScore_SHZ
{
	int                             m_GetType;
	//
};

//BW

struct CMD_C_UserFilter_SHZ {
	dword user_id;
	unsigned char operate_code; // 0 黑名单 1 白名单 2 清除
};

struct CMD_C_Stroage_SHZ {
	int iMode;	// 0:查询 1:设置
	longlong lValue;
};

struct CMD_S_Stroage_SHZ {
	longlong lValue;
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
