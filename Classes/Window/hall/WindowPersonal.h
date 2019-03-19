#ifndef  _WindowModifyIcon_H_
#define  _WindowModifyIcon_H_

#include "Window/WindowBase.h"

enum eWindowPersonalState
{
	EWPS_PERSONAL,			//�������ý���
	EWPS_MODIFY,		//�޸�ͼ�����
	EWPS_BIND			//�󶨽���
};

class  WindowPersonal : public WindowBase
{
public:
	WindowPersonal();
	virtual ~WindowPersonal();

	CREATE_FUNC(WindowPersonal);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;

	void refreshNickName(Ref* ref);
	void refreshTicket(Ref* ref);
protected:
	void _setPersonalInfo();
	void _setUIState(int nState);

	void _setPersonalNode();
	void _setModifyNode();
	void _setBindNode();
protected:
	Node* m_pNodePersonal;
	Node* m_pNodeModify;
	Node* m_pNodeBind;

	TextField* m_pTextAccount;
	TextField* m_pTextPassword;
	TextField* m_pTextPasswordConfirm;

	Sprite* m_pSpritePersonal;
	Sprite* m_pSpriteModify;

	Sprite* m_pSelectSprite;
};

#endif // _WindowModifyIcon_H_

