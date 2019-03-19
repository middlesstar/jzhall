#ifndef __CWater_H__
#define __CWater_H__

#include "cocos2d.h"

USING_NS_CC;


class CWater : public Layer
{
public:
	CWater();
	~CWater();

	bool init();

	CREATE_FUNC(CWater);

	//清除所有的阳光
	void removeAllLight();

protected:
	Animate* createSunAni(const char* name);
	bool cacheSunAni();

protected:
	Sprite*     m_spWater;
	Action*     m_pWater;
	Sprite*     m_spWaterClone[4][6];
	Action*     m_pWaterAniClone[4][6];
};

#endif