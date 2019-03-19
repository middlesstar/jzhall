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

//��ʼ������
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
//������Ϣ

//�û�����
bool SHZChatLayer::InsertExpression(const char* pszSendUser, unsigned int index)
{
	//PLAZZ_PRINTF(t8("%s ���ͱ���:%d\n"), pszSendUser, index);

	////��������
	//CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
	//CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

	////�������
	//if (pExpressionItem!=0)
	//{
	//	//��ȡ·��
	//	char szImagePath[MAX_PATH]=TEXT("");
	//	pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

	//	//������Ϣ
	//	const char* pszSendUser=pISendUserItem->GetNickName();
	//	m_ChatMessage.InsertExpression(pszSendUser,szImagePath);
	//}
	return true;
}

//�û�����
bool SHZChatLayer::InsertExpression(const char* pszSendUser, unsigned int index, bool bMyselfString)
{
	//PLAZZ_PRINTF(t8("%s ���ͱ���:%d\n"), pszSendUser, index);
	return true;
}

//�û�����
bool SHZChatLayer::InsertExpression(const char* pszSendUser, const char* pszRecvUser, unsigned int index)
{
	//PLAZZ_PRINTF(t8("%s �� %s ���ͱ���:%d\n"), pszSendUser, pszRecvUser, index);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//������Ϣ

//�û�����
bool SHZChatLayer::InsertUserChat(const char* pszSendUser, const char* pszString, unsigned int crColor)
{
	//PLAZZ_PRINTF(t8("%s ˵:%s\n"), pszSendUser, pszString);
	return true;
}
//�û�����
bool SHZChatLayer::InsertUserChat(const char* pszSendUser, const char* pszRecvUser, const char* pszString, unsigned int crColor)
{
	//PLAZZ_PRINTF(t8("%s �� %s ˵:%s\n"), pszSendUser, pszRecvUser, pszString);
	return true;
}
//�û�˽��
bool SHZChatLayer::InsertWhisperChat(const char* pszSendUser, const char* pszString, unsigned int crColor, bool bMyselfString)
{
	//PLAZZ_PRINTF(t8("%s ����˵:%s\n"), pszSendUser, pszString);
	return true;
}
//�û�����
bool SHZChatLayer::InsertUserTrumpet(const char* pszSendUser,const char* pszString,unsigned int crColor)
{
	//PLAZZ_PRINTF(t8("%s ����Trumpet����:%s\n"), pszSendUser, pszString);
	return true;
}
//�û�����
bool SHZChatLayer::InsertUserTyphon(const char* pszSendUser,const char* pszString,unsigned int crColor)
{
	//PLAZZ_PRINTF(t8("%s ����Typhon����:%s\n"), pszSendUser, pszString);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//ϵͳ��Ϣ

//ϵͳ��Ϣ
bool SHZChatLayer::InsertSystemChat(const char* pszString)
{
	PLAZZ_PRINTF("ChatLayer::InsertSystemChat");
	//PLAZZ_PRINTF(t8("ϵͳ��Ϣ:%s \n"), pszString);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// IStringMessageSink

//////////////////////////////////////////////////////////////////////////
//�¼���Ϣ

//�����¼�
bool SHZChatLayer::InsertUserEnter(const char* pszUserName)
{
	PLAZZ_PRINTF("ChatLayer::InsertUserEnter");
	//PLAZZ_PRINTF(t8("%s ���뷿��\n"), pszUserName);
	return true;
}

//�뿪�¼�
bool SHZChatLayer::InsertUserLeave(const char* pszUserName)
{
	PLAZZ_PRINTF("ChatLayer::InsertUserLeave");
	//PLAZZ_PRINTF(t8("%s �뿪����\n"), pszUserName);
	return true;
}

//�����¼�
bool SHZChatLayer::InsertUserOffLine(const char* pszUserName)
{
	PLAZZ_PRINTF("ChatLayer::InsertUserOffLine");
	//PLAZZ_PRINTF(t8("%s ����\n"), pszUserName);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//�ַ���Ϣ

//��ͨ��Ϣ(�������)
bool SHZChatLayer::InsertNormalString(const char* pszString)
{
	PLAZZ_PRINTF("ChatLayer::InsertNormalString");
	//PLAZZ_PRINTF(t8("��ͨ��Ϣ:%s\n"), pszString);
	return true;
}
//ϵͳ��Ϣ(�������)
bool SHZChatLayer::InsertSystemString(const char* pszString)
{
	PLAZZ_PRINTF("ChatLayer::InsertSystemString");
	//PLAZZ_PRINTF(t8("ϵͳ��Ϣ:%s\n"), pszString);
	return true;
}
//��ʾ��Ϣ(�Ի���ʽ??)
int SHZChatLayer::InsertPromptString(const char* pszString, int iButtonType)
{
	PLAZZ_PRINTF("ChatLayer::InsertPromptString");
	//PLAZZ_PRINTF(t8("��ʾ��Ϣ:%s[%s]\n"), pszString, iButtonType == 0 ? t8("ȷ��"):t8("ȷ��,ȡ��"));
	return 0;
}
//������Ϣ(�������)
bool SHZChatLayer::InsertAfficheString(const char* pszString)
{
	PLAZZ_PRINTF("ChatLayer::InsertAfficheString");
	//PLAZZ_PRINTF(t8("������Ϣ:%s\n"), pszString);
	return true;
}


//ϲ����Ϣ
bool SHZChatLayer::InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum)
{
	G_NOTIFICATION_SHZ("GLAD_MSG", SHZMTData::create(colText, colName, colNum, pszContent, pszNickName, pszNum));
	return true;
}