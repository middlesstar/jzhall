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
	//��ʱ��������
	int               timeIndex;
	//��ؼ�ʱ����λ���Ƿ�ı�
	bool flag;


	static const float	UPDATE_TIME;
public:
	SharkTimeCount();
	~SharkTimeCount();

	//��������ʱ
	void StartPlay(int time,int index);
	//ֹͣ����ʱ
	void StopPlay();
	//�Ƿ��ڵ���ʱ
	//bool GetRunningState(){ return m_bIsRun; }
	//�л�����ʱ��ʾ���λ��
	void changeTimeCount();

	//���õ���ʱ����ʾ
	void showTime();
	//���õ���ʱ������
	void HideTime();
protected:

	void setContent();
	void TimeUpdate(float t);

};

#endif

