#ifndef __WaitNext_H__
#define __WaitNext_H__

#include "shark/Base/ViewBase.h"

/*
进入游戏等待界面
*/


class WaitNext :
	public ViewBase
{
protected:
	TextAtlas*			m_timeCount;
	ImageView*			m_waitnextgame;
	Button*				m_okbt;
	Node*				m_waitnode;
	int					m_nowTime;					//当前剩余的时间
protected:
	void setContent();

	void BtEvent(Ref* pSender, TouchEventType type);

	void updatetime(float t);
	//定时器3秒关闭通知框
	void updateNoticeOut(float t);
	//文字动画
	void updateText(float t);
	//等待通知框退出
	void waitnodeOutFunc();
	
public:
	WaitNext();

	virtual ~WaitNext();

	void StartPlay(int time);
	//销毁(不再使用)
	void destory();
};

#endif

