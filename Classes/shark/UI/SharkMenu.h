#ifndef __SharkMenu_H__
#define __SharkMenu_H__

#include "shark/Base/ViewBase.h"
//#include "Regular.h"

/*
右上角3个按钮管理

*/


class SharkMenu :
	public ViewBase
{

protected:
	Node*               menu;           //菜单显示
	Button*				m_recharge;		//充值按钮
	Button*				m_setting;		//设置按钮
	Button*				m_regular;		//规则按钮
	Button*				m_exit;			//退出
	Button*             m_left;         //菜单返回按钮
	Button*             m_right;        //菜单拉出按钮

	bool                flag;           //菜单返回拉出的标志

public:

	SharkMenu();
	~SharkMenu();
	//返回按钮事件 
	void goBack(int code = 0);

	//弹出充值按钮
	void outRecharge(Ref* ref);

protected:
	void BtEvent(Ref *pSender, TouchEventType type);
	void setContent();
	//菜单收回
	void moveIn();
	//菜单显示
	void moveOut();
	//设置按钮可见
	void setRight();


};

#endif

