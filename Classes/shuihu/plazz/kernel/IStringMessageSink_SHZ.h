#ifndef SHZ_IStringMessageSink_H_
#define SHZ_IStringMessageSink_H_

#include "df/DF.h"

//��Ϣ�ӿ�
class SHZIStringMessageSink
{
public:
	virtual ~SHZIStringMessageSink(){}

	//�¼���Ϣ
public:
	//�����¼�
	virtual bool InsertUserEnter(const char* pszUserName)=0;
	//�뿪�¼�
	virtual bool InsertUserLeave(const char* pszUserName)=0;
	//�����¼�
	virtual bool InsertUserOffLine(const char* pszUserName)=0;

	//�ַ���Ϣ
public:
	//��ͨ��Ϣ(�������)
	virtual bool InsertNormalString(const char* pszString)=0;
	//ϵͳ��Ϣ(�������)
	virtual bool InsertSystemString(const char* pszString)=0;
	//��ʾ��Ϣ(�Ի���ʽ??)0:ȷ�� 1:ȷ��,ȡ��
	virtual int InsertPromptString(const char* pszString, int iButtonType)=0;
	//������Ϣ(�������)
	virtual bool InsertAfficheString(const char* pszString)=0;

	//������Ϣ
public:
	//ϲ����Ϣ
	virtual bool InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum)=0;

//	//������Ϣ
//	virtual bool InsertCustomString(const char* pszString, dword crColor)=0;
//	//������Ϣ
//	virtual bool InsertCustomString(const char* pszString, dword crColor, dword crBackColor)=0;
};

#endif // _IStringMessageSink_H_
