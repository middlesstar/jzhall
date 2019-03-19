#ifndef  _WindowLoginAccount_H_
#define  _WindowLoginAccount_H_

#include "Window/WindowBase.h"
#include "Window/ConfirmDialog/ConfirmDialogDelegate.h"

/* ��¼���� */
enum eWindowLoginAccountState
{
	EWLS_LOGIN,			//�ǳƵ���
	EWSS_NEW,			//�½��˺�
	EWSS_CHANGE			//ѡ���˺�
};

class  WindowLoginAccount : public WindowBase, public ConfirmDialogDelegate
{
public:
	WindowLoginAccount();
	virtual ~WindowLoginAccount();

	CREATE_FUNC(WindowLoginAccount);
	virtual void setContent() override;				        //����UI����
	virtual void updateWindow(int nCode) override;	        //ˢ�´���

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

