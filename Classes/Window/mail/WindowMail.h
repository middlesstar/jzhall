#ifndef  _WindowMail_H_
#define  _WindowMail_H_

#include "Window/WindowBase.h"

struct CMD_ST_GetEmailList_rsp;
struct EmailInfoSt;

enum eWindowMailState
{
	EWMS_List,		//邮件列表	
	EWMS_Content	//邮件内容
};

/* 窗口基类 */
class  WindowMail : public WindowBase
{
public:
	WindowMail();
	virtual ~WindowMail();

	CREATE_FUNC(WindowMail);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;
	virtual void updateWindow(int nCode, void* pData) override;

protected:
	void _setUIState(int nState);

	void _setMailList(CMD_ST_GetEmailList_rsp* pMailData);
	void _setMailItem(ImageView* pItem, EmailInfoSt* pMail);
	void _adjustScrollHeight(ui::ScrollView* pScroll, float fIncreaseHeight);		//将pScroll的innersize增加fIncreaseHeight
	void _setMailContent(EmailInfoSt* pMail);

	void _MailItemClick(Ref* pRef);
	void _getAttachGoodsClick(Ref* pRef);

	std::string _getAttachGoodsImgPath(int nType);
protected:
	ImageView* m_pImgItem;
	ui::ScrollView* m_pScroll;

	Node* m_pNodeContent;
};

#endif // _WindowMail_H_

