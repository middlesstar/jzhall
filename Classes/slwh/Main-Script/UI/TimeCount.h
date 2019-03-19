#ifndef __TimeCount_H__
#define __TimeCount_H__

#include "slwh/Base/UIBase.h"


class TimeCount :
	public UIBase
{

protected:
	TextAtlas*		m_num;
	int				m_nowTime;
	bool			m_bIsRun;


	static const float	UPDATE_TIME;
public:
	TimeCount();
	~TimeCount();

	//开启倒计时
	void StartPlay(int time);
	//停止倒计时
	void StopPlay();
	//是否在倒计时
	bool GetRunningState(){ return m_bIsRun; }
	//得到当前剩余时间
	int GetleftTime(){ return m_nowTime; }
protected:

	void Start();
	void TimeUpdate(float t);

};

#endif

