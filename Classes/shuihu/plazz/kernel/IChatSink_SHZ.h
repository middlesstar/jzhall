#ifndef SHZ_IChatSink_H_
#define SHZ_IChatSink_H_

//////////////////////////////////////////////////////////////////////////
// ���칳��
//////////////////////////////////////////////////////////////////////////
class SHZIChatSink
{
public:
	virtual ~SHZIChatSink(){};

	//������Ϣ
public:
	//�û�����
	virtual bool InsertExpression(const char* pszSendUser, unsigned int index)=0;
	//�û�����
	virtual bool InsertExpression(const char* pszSendUser, unsigned int index, bool bMyselfString)=0;
	//�û�����
	virtual bool InsertExpression(const char* pszSendUser, const char* pszRecvUser, unsigned int index)=0;

	//������Ϣ
public:
	//�û�����
	virtual bool InsertUserChat(const char* pszSendUser, const char* pszString, unsigned int crColor)=0;
	//�û�����
	virtual bool InsertUserChat(const char* pszSendUser, const char* pszRecvUser, const char* pszString, unsigned int crColor)=0;
	//�û�˽��
	virtual bool InsertWhisperChat(const char* pszSendUser, const char* pszString, unsigned int crColor, bool bMyselfString)=0;
	//�û�����
	virtual bool InsertUserTrumpet(const char* pszSendUser,const char* pszString,unsigned int crColor)=0;
	//�û�����
	virtual bool InsertUserTyphon(const char* pszSendUser,const char* pszString,unsigned int crColor)=0;

	//ϵͳ��Ϣ
public:
	//ϵͳ��Ϣ
	virtual bool InsertSystemChat(const char* pszString)=0;

};

#endif // _IChatSink_H_