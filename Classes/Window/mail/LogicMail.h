#ifndef  _LogicMail_H_
#define  _LogicMail_H_

#include "Window/LogicBase.h"

enum eEmailOperateType
{
	EEOT_GET_ATTACH		=	1,			//��ȡ����
	EEOT_DELETE			=	2,			//ɾ���ʼ�
};

class  LogicMail : public LogicBase
{
public:
	LogicMail();
	virtual ~LogicMail();

	SINGLE_INSTANCE_FUNC(LogicMail);

protected:
	void _registerMessageHandler();
	void _addAttachGoods(int nType,int nCount);
public:
	void requestMails();									//�����ʼ�
	void onMailsResp(const PacketAide& packet, int nLen);	//�����ʼ�

	void requestMailsOperate(int nType,int eid);					//��������ʼ�
	void onMailsOperateResp(const PacketAide& packet, int nLen);	//�����ʼ�����

public:
	void deleteEmailByEid(int nEid);
	CMD_ST_GetEmailList_rsp* getEmailListData();
protected:
	CMD_ST_GetEmailList_rsp m_stMailList;

	CC_SYNTHESIZE(EmailInfoSt, m_stCurClickMail, CurClickMail);		//��ǰ����鿴���ʼ�
};

#endif // _LogicMail_H_

