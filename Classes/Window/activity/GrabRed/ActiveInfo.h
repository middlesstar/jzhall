#ifndef  _ActiveInfo_H_
#define  _ActiveInfo_H_


#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "ActiveConfig.h"


USING_NS_CC;
using namespace ui;

class ActiveInfo :
	public Node
{
protected:
	Label*						m_timeLabel;
	Label*						m_goldLabel;
	Label*						m_hfLabel;

	Text*						m_time;
	Text*						m_gold;
	Text*						m_hf;

	float						m_nowTime;
	int							m_goldNum;
	double						m_hfNum;

public:

	float GetTime(){ return m_nowTime - _RAND0; }
	int GetGold(){ return m_goldNum - _RAND1; }
	float GetHF(){ return m_hfNum - _RAND2; }

	void AddTime(float t);
	void AddGold(int t);
	void AddHF(float t);


	void InitData();

	ActiveInfo();
	~ActiveInfo();

	virtual void OnInit();
protected:

};

#endif // _ActiveInfo_H_