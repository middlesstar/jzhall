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

	//��������ʱ
	void StartPlay(int time);
	//ֹͣ����ʱ
	void StopPlay();
	//�Ƿ��ڵ���ʱ
	bool GetRunningState(){ return m_bIsRun; }
	//�õ���ǰʣ��ʱ��
	int GetleftTime(){ return m_nowTime; }
protected:

	void Start();
	void TimeUpdate(float t);

};

#endif

