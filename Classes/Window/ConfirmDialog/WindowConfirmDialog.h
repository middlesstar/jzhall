#ifndef  _WindowConfirmDialog_H_
#define  _WindowConfirmDialog_H_

#include "Window/WindowBase.h"

/* ȷ�Ͽ� */
enum eWindowDialogType
{
	EWDT_OK,				//ȷ����
	EWDT_OK_CANCEL,			//ȷ��ȡ����
	EWDT_OK_VIP				//ȷ��VIP��
};

class ConfirmDialogDelegate;

class  WindowConfirmDialog : public WindowBase
{
public:
	WindowConfirmDialog();
	virtual ~WindowConfirmDialog();

	static WindowConfirmDialog* create(const std::string& strText);
	bool init(const std::string& strText);

	virtual void setContent(const std::string& strText);				                //����UI����
	virtual void onEnter() override;

	void setDialogType(int nType);
protected:
	Node* m_pNodeOk;
	Node* m_pNodeOkCancel;
	Node* m_pNodeVip;
	CC_SYNTHESIZE(ConfirmDialogDelegate*, m_pDelegate, Delegate);
};

#endif // _WindowConfirmDialog_H_

