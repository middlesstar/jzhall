#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "ChatLayer_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"

USING_NS_CC;
USING_NS_CC_EXT;

//////////////////////////////////////////////////////////////////////////
SHZChatLayer::SHZChatLayer()
{
}

SHZChatLayer::~SHZChatLayer()
{
}

//初始化方法
bool SHZChatLayer::init()
{
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		
		return true;
	} while (0);

	return false;
}

//////////////////////////////////////////////////////////////////////////
// IChatSink

//////////////////////////////////////////////////////////////////////////
//表情消息

//用户表情
bool SHZChatLayer::InsertExpression(const char* pszSendUser, unsigned int index)
{
	//PLAZZ_PRINTF(t8("%s 发送表情:%d\n"), pszSendUser, index);

	////变量定义
	//CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
	//CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

	////插入表情
	//if (pExpressionItem!=0)
	//{
	//	//获取路径
	//	char szImagePath[MAX_PATH]=TEXT("");
	//	pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

	//	//插入消息
	//	const char* pszSendUser=pISendUserItem->GetNickName();
	//	m_ChatMessage.InsertExpression(pszSendUser,szImagePath);
	//}
	return true;
}

//用户表情
bool SHZChatLayer::InsertExpression(const char* pszSendUser, unsigned int index, bool bMyselfString)
{
	//PLAZZ_PRINTF(t8("%s 发送表情:%d\n"), pszSendUser, index);
	return true;
}

//用户表情
bool SHZChatLayer::InsertExpression(const char* pszSendUser, const char* pszRecvUser, unsigned int index)
{
	//PLAZZ_PRINTF(t8("%s 对 %s 发送表情:%d\n"), pszSendUser, pszRecvUser, index);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//聊天消息

//用户聊天
bool SHZChatLayer::InsertUserChat(const char* pszSendUser, const char* pszString, unsigned int crColor)
{
	//PLAZZ_PRINTF(t8("%s 说:%s\n"), pszSendUser, pszString);
	return true;
}
//用户聊天
bool SHZChatLayer::InsertUserChat(const char* pszSendUser, const char* pszRecvUser, const char* pszString, unsigned int crColor)
{
	//PLAZZ_PRINTF(t8("%s 对 %s 说:%s\n"), pszSendUser, pszRecvUser, pszString);
	return true;
}
//用户私聊
bool SHZChatLayer::InsertWhisperChat(const char* pszSendUser, const char* pszString, unsigned int crColor, bool bMyselfString)
{
	//PLAZZ_PRINTF(t8("%s 对你说:%s\n"), pszSendUser, pszString);
	return true;
}
//用户喇叭
bool SHZChatLayer::InsertUserTrumpet(const char* pszSendUser,const char* pszString,unsigned int crColor)
{
	//PLAZZ_PRINTF(t8("%s 发送Trumpet喇叭:%s\n"), pszSendUser, pszString);
	return true;
}
//用户喇叭
bool SHZChatLayer::InsertUserTyphon(const char* pszSendUser,const char* pszString,unsigned int crColor)
{
	//PLAZZ_PRINTF(t8("%s 发送Typhon喇叭:%s\n"), pszSendUser, pszString);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//系统消息

//系统消息
bool SHZChatLayer::InsertSystemChat(const char* pszString)
{
	PLAZZ_PRINTF("ChatLayer::InsertSystemChat");
	//PLAZZ_PRINTF(t8("系统消息:%s \n"), pszString);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// IStringMessageSink

//////////////////////////////////////////////////////////////////////////
//事件消息

//进入事件
bool SHZChatLayer::InsertUserEnter(const char* pszUserName)
{
	PLAZZ_PRINTF("ChatLayer::InsertUserEnter");
	//PLAZZ_PRINTF(t8("%s 进入房间\n"), pszUserName);
	return true;
}

//离开事件
bool SHZChatLayer::InsertUserLeave(const char* pszUserName)
{
	PLAZZ_PRINTF("ChatLayer::InsertUserLeave");
	//PLAZZ_PRINTF(t8("%s 离开房间\n"), pszUserName);
	return true;
}

//断线事件
bool SHZChatLayer::InsertUserOffLine(const char* pszUserName)
{
	PLAZZ_PRINTF("ChatLayer::InsertUserOffLine");
	//PLAZZ_PRINTF(t8("%s 断线\n"), pszUserName);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//字符消息

//普通消息(窗口输出)
bool SHZChatLayer::InsertNormalString(const char* pszString)
{
	PLAZZ_PRINTF("ChatLayer::InsertNormalString");
	//PLAZZ_PRINTF(t8("普通消息:%s\n"), pszString);
	return true;
}
//系统消息(窗口输出)
bool SHZChatLayer::InsertSystemString(const char* pszString)
{
	PLAZZ_PRINTF("ChatLayer::InsertSystemString");
	//PLAZZ_PRINTF(t8("系统消息:%s\n"), pszString);
	return true;
}
//提示消息(对话框方式??)
int SHZChatLayer::InsertPromptString(const char* pszString, int iButtonType)
{
	PLAZZ_PRINTF("ChatLayer::InsertPromptString");
	//PLAZZ_PRINTF(t8("提示消息:%s[%s]\n"), pszString, iButtonType == 0 ? t8("确认"):t8("确认,取消"));
	return 0;
}
//公告消息(窗口输出)
bool SHZChatLayer::InsertAfficheString(const char* pszString)
{
	PLAZZ_PRINTF("ChatLayer::InsertAfficheString");
	//PLAZZ_PRINTF(t8("公告消息:%s\n"), pszString);
	return true;
}


//喜报消息
bool SHZChatLayer::InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum)
{
	G_NOTIFICATION_SHZ("GLAD_MSG", SHZMTData::create(colText, colName, colNum, pszContent, pszNickName, pszNum));
	return true;
}