#ifndef __Regular_H__
#define __Regular_H__

#include "slwh/Base/UIBase.h"

/*
所有管理类钧在UILayerMain 层初始化
而不在UILayerMain调用 采用单例模式
*/

class Regular :
	public UIBase
{
protected:
	Button*						m_close;
	LayerColor*					m_layerColor;

	bool						m_bIsInit;
protected:
	bool						m_bIsCloseRunning;
	//动画
	Action*						m_fadein;
	Action*						m_fadeout;

protected:
	static Regular*				m_instance;

public:
	~Regular();

	void Show();
	void Hide();


protected:
	//禁止定义变量
	Regular();
	void Start();

	void CloseEvent(Ref* pSender, TouchEventType type);

	//点击关闭按钮后，动画播放完成的回调
	void CallBackClose();

	bool onTouchBegan(Touch* touch, Event* event);

	CREATE_FUNC(Regular)
public:
	static Regular* GetInstance();
};

#endif

