#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//定义文件
#include "Macro.h"
#include "Define.h"

//结构文件
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//模板文件
//#include "Array.h"
#include "Module.h"
//#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//程序版本
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(6,0,3)				//大厅版本
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(6,0,3)				//手机版本
#define VERSION_MOBILE_IOS			PROCESS_VERSION(6,0,3)				//手机版本

//版本定义
#define VERSION_EFFICACY			1									//效验版本
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//框架版本

#endif