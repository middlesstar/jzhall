#ifndef __SpeDMG2_H__
#define __SpeDMG2_H__

#include "slwh/Base/ModelBase.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

#define TEX_MAX 12

//DMG第三套表现形式


class SpeDMG2 :
	public ModelBase
{
protected:
	Node*									m_dmgNode;
	Sprite3D*								m_dmgModel;
	Node*									m_points[23];
	PUParticleSystem3D*						m_ps;
protected:
	void CreatObj(int index);

	void DeleteObj();

	void EffDelay(float t);
public:
	void StartPlay(int index);

	void StopPlay();

	void setDMGIndex(int index);		//设置第几轮的纹理

	SpeDMG2();

	~SpeDMG2();
};

#endif


