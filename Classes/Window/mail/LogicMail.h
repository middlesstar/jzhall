#ifndef  _LogicMail_H_
#define  _LogicMail_H_

#include "Window/LogicBase.h"

enum eEmailOperateType
{
	EEOT_GET_ATTACH		=	1,			//领取附件
	EEOT_DELETE			=	2,			//删除邮件
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
	void requestMails();									//请求邮件
	void onMailsResp(const PacketAide& packet, int nLen);	//返回邮件

	void requestMailsOperate(int nType,int eid);					//请求操作邮件
	void onMailsOperateResp(const PacketAide& packet, int nLen);	//返回邮件操作

public:
	void deleteEmailByEid(int nEid);
	CMD_ST_GetEmailList_rsp* getEmailListData();
protected:
	CMD_ST_GetEmailList_rsp m_stMailList;

	CC_SYNTHESIZE(EmailInfoSt, m_stCurClickMail, CurClickMail);		//当前点击查看的邮件
};

#endif // _LogicMail_H_

