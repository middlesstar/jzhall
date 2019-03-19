#ifndef SHZ_ChatLayer_H_
#define SHZ_ChatLayer_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class SHZChatLayer
	: public cocos2d::CCLayer
	, public SHZIChatSink
	, public SHZIStringMessageSink
{
public:
	CREATE_FUNC(SHZChatLayer);

public:
	SHZChatLayer();
	~SHZChatLayer();
	bool init();

	//////////////////////////////////////////////////////////////////////////
	// IChatSink
	//������Ϣ
public:
	//�û�����
	virtual bool InsertExpression(const char* pszSendUser, unsigned int index);
	//�û�����
	virtual bool InsertExpression(const char* pszSendUser, unsigned int index, bool bMyselfString);
	//�û�����
	virtual bool InsertExpression(const char* pszSendUser, const char* pszRecvUser, unsigned int index);

	//������Ϣ
public:
	//�û�����
	virtual bool InsertUserChat(const char* pszSendUser, const char* pszString, unsigned int crColor);
	//�û�����
	virtual bool InsertUserChat(const char* pszSendUser, const char* pszRecvUser, const char* pszString, unsigned int crColor);
	//�û�˽��
	virtual bool InsertWhisperChat(const char* pszSendUser, const char* pszString, unsigned int crColor, bool bMyselfString);
	//�û�����
	virtual bool InsertUserTrumpet(const char* pszSendUser,const char* pszString,unsigned int crColor);
	//�û�����
	virtual bool InsertUserTyphon(const char* pszSendUser,const char* pszString,unsigned int crColor);

	//ϵͳ��Ϣ
public:
	//ϵͳ��Ϣ
	virtual bool InsertSystemChat(const char* pszString);

	//////////////////////////////////////////////////////////////////////////
	// IStringMessageSink
	//�¼���Ϣ
public:
	//�����¼�
	virtual bool InsertUserEnter(const char* pszUserName);

	//�뿪�¼�
	virtual bool InsertUserLeave(const char* pszUserName);
	//�����¼�
	virtual bool InsertUserOffLine(const char* pszUserName);
	//�ַ���Ϣ
public:
	//��ͨ��Ϣ(�������)
	virtual bool InsertNormalString(const char* pszString);
	//ϵͳ��Ϣ(�������)
	virtual bool InsertSystemString(const char* pszString);
	//��ʾ��Ϣ(�Ի���ʽ??)
	virtual int InsertPromptString(const char* pszString, int iButtonType);
	//������Ϣ(�������)
	virtual bool InsertAfficheString(const char* pszString);

	//������Ϣ
public:
	//ϲ����Ϣ
	virtual bool InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum);

};

#endif // _ChatLayer_H_