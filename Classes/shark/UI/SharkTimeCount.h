#ifndef __SharkTimeCount_H__
#define __SharkTimeCount_H__

#include "shark/Base/ViewBase.h"


#define      POSONE  Vec2(247.12,326.31)
#define      POSTWO  Vec2(267.48,137.78)

class SharkTimeCount : public ViewBase
{

protected:
	TextAtlas*          m_battleTime;
	TextAtlas*			m_betTime;
	Sprite*             battleTime;
	Sprite*             betTime;
	int				m_nowTime;
	//bool				m_bIsRun;
	//计时器的索引
	int               timeIndex;
	//监控计时器的位置是否改变
	bool flag;


	static const float	UPDATE_TIME;
public:
	SharkTimeCount();
	~SharkTimeCount();

	//开启倒计时
	void StartPlay(int time,int index);
	//停止倒计时
	void StopPlay();
	//是否在倒计时
	//bool GetRunningState(){ return m_bIsRun; }
	//切换倒计时显示框的位置
	void changeTimeCount();

	//设置倒计时框显示
	void showTime();
	//设置倒计时框隐藏
	void HideTime();
protected:

	void setContent();
	void TimeUpdate(float t);

};

#endif

