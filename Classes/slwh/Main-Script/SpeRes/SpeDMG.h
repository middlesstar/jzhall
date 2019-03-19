#ifndef __SpeDMG_H__
#define __SpeDMG_H__

#include "slwh/Base/ModelBase.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

#define TEX_MAX 12

//DMG原始表现形式

class SpeDMG :
	public ModelBase
{
protected:
	Node*									m_dmgNode;
	Sprite3D*								m_dmgModel;
	PUParticleSystem3D*						m_ps;
protected:
	void CreatObj();

	void DeleteObj();

	void EffDelay(float t);
public:
	void StartPlay();

	void StopPlay();

	void setDMGIndex(int index);		//设置第几轮的纹理

	SpeDMG();

	~SpeDMG();
};

#endif


