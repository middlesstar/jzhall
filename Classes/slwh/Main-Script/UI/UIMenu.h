#ifndef __UIMenu_H__
#define __UIMenu_H__

#include "slwh/Base/UIBase.h"
#include "Regular.h"

/*
右上角3个按钮管理

*/


class UIMenu :
	public UIBase
{

protected:
	Button*				m_recharge;		//充值按钮
	Button*				m_setting;		//设置按钮
	Button*				m_regular;		//规则按钮
	Button*				m_exit;			//退出

public:

	UIMenu();
	~UIMenu();

	void exit(int code = 0);	//0退出房间到选房间界面，1退出房间到大厅

protected:
	void BtEvent(Ref *pSender, TouchEventType type);
	void Start();

};

#endif

