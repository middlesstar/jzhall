#ifndef  _WindowConfirmDialog_H_
#define  _WindowConfirmDialog_H_

#include "Window/WindowBase.h"

/* 确认框 */
enum eWindowDialogType
{
	EWDT_OK,				//确定框
	EWDT_OK_CANCEL,			//确定取消框
	EWDT_OK_VIP				//确定VIP框
};

class ConfirmDialogDelegate;

class  WindowConfirmDialog : public WindowBase
{
public:
	WindowConfirmDialog();
	virtual ~WindowConfirmDialog();

	static WindowConfirmDialog* create(const std::string& strText);
	bool init(const std::string& strText);

	virtual void setContent(const std::string& strText);				                //设置UI布局
	virtual void onEnter() override;

	void setDialogType(int nType);
protected:
	Node* m_pNodeOk;
	Node* m_pNodeOkCancel;
	Node* m_pNodeVip;
	CC_SYNTHESIZE(ConfirmDialogDelegate*, m_pDelegate, Delegate);
};

#endif // _WindowConfirmDialog_H_

