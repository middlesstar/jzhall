#ifndef SHZ_CMD_COMMOM_HEAD_FILE
#define SHZ_CMD_COMMOM_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

#define MDM_CM_SYSTEM_SHZ				1000								//系统命令

#define SUB_CM_SYSTEM_MESSAGE_SHZ		1									//系统消息
#define SUB_CM_ACTION_MESSAGE_SHZ		2									//动作消息
#define SUB_CM_DOWN_LOAD_MODULE_SHZ		3									//下载消息

//////////////////////////////////////////////////////////////////////////////////

//类型掩码
#define SMT_CHAT_SHZ					0x0001								//聊天消息
#define SMT_EJECT_SHZ					0x0002								//弹出消息
#define SMT_GLOBAL_SHZ					0x0004								//全局消息
#define SMT_PROMPT_SHZ					0x0008								//提示消息
#define SMT_TABLE_ROLL_SHZ				0x0010								//滚动消息

//控制掩码
#define SMT_CLOSE_ROOM_SHZ				0x0100								//关闭房间
#define SMT_CLOSE_GAME_SHZ				0x0200								//关闭游戏
#define SMT_CLOSE_LINK_SHZ				0x0400								//中断连接
//
////系统消息
//struct CMD_CM_SystemMessage
//{
//	word							wType;								//消息类型
//	word							wLength;							//消息长度
//	tchar							szString[1024];						//消息内容
//};

//////////////////////////////////////////////////////////////////////////////////

//动作类型
#define ACT_BROWSE_SHZ				1									//浏览动作
#define ACT_BROWSE_SHZ				2									//下载动作
//
////动作信息
//struct tagActionHead
//{
//	uint							uResponseID;						//响应标识
//	word							wAppendSize;						//附加大小
//	byte							cbActionType;						//动作类型
//};

//浏览类型
#define BRT_IE_SHZ						0x01								//I E 浏览
#define BRT_PLAZA_SHZ					0x02								//大厅浏览
#define BRT_WINDOWS_SHZ					0x04								//窗口浏览

////浏览动作
//struct tagActionBrowse
//{
//	byte							cbBrowseType;						//浏览类型
//	tchar							szBrowseUrl[256];					//浏览地址
//};

//下载类型
#define DLT_IE_SHZ						1									//I E 下载
#define DLT_MODULE_SHZ					2									//下载模块

////下载动作
//struct tagActionDownLoad
//{
//	byte							cbDownLoadMode;						//下载方式
//	tchar							szDownLoadUrl[256];					//下载地址
//};

////动作消息
//struct CMD_CM_ActionMessage
//{
//	word							wType;								//消息类型
//	word							wLength;							//消息长度
//	uint							nButtonType;						//按钮类型
//	tchar							szString[1024];						//消息内容
//};

//////////////////////////////////////////////////////////////////////////////////

////下载信息
//struct CMD_CM_DownLoadModule
//{
//	byte							cbShowUI;							//显示界面
//	byte							cbAutoInstall;						//自动安装
//	word							wFileNameSize;						//名字长度
//	word							wDescribeSize;						//描述长度
//	word							wDownLoadUrlSize;					//地址长度
//};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif
