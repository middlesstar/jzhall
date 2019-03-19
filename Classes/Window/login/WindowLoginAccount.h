#ifndef  _WindowLoginAccount_H_
#define  _WindowLoginAccount_H_

#include "Window/WindowBase.h"
#include "Window/ConfirmDialog/ConfirmDialogDelegate.h"

/* 登录界面 */
enum eWindowLoginAccountState
{
	EWLS_LOGIN,			//昵称登入
	EWSS_NEW,			//新建账号
	EWSS_CHANGE			//选择账号
};

class  WindowLoginAccount : public WindowBase, public ConfirmDialogDelegate
{
public:
	WindowLoginAccount();
	virtual ~WindowLoginAccount();

	CREATE_FUNC(WindowLoginAccount);
	virtual void setContent() override;				        //设置UI布局
	virtual void updateWindow(int nCode) override;	        //刷新窗口

	virtual void onOkClick(int nData, void* pData) override;
protected:
	void _setUIState(int nState);

	void _initLoginNode();
	void _initNewNode();
	void _initChangeNode();
	void _initChangeScroll(bool bFirst = true);
	void _deleteAccountCache(int nUserid);

	void _setAccountItem(Layout* pItem, const std::string& strAccount, const std::string& strPassword,int nUserID);
	void _setItemSelect(Layout* pItem);
	void _ItemClickCallback(Ref* pRef);
	void _ItemDeleteCallback(Ref* pRef);
protected:
	Node* m_pNodeLogin;
	Node* m_pNodeNew;
	Node* m_pNodeChange;

	ui::ScrollView* m_pScroll;
	Layout* m_pItem;
	ImageView* m_pImgSelect;
	Layout* m_pSelectItem;
};

#endif // _WindowLoginAccount_H_

