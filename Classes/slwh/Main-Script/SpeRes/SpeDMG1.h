#ifndef __SpeDMG1_H__
#define __SpeDMG1_H__

#include "slwh/Base/ModelBase.h"


#define TEX_MAX 12

//DMG第二套表现形式  跑火车

class SpeDMG1 :
	public ModelBase
{
protected:
	Sprite3D* m_rootPoint;
	Sprite3D* m_points[11];
protected:
	void DeleteObj();

public:
	void StartPlay(Sprite3D* point);

	void StopPlay();

	SpeDMG1();

	~SpeDMG1();
};

#endif


