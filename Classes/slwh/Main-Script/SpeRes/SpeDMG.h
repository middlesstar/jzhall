#ifndef __SpeDMG_H__
#define __SpeDMG_H__

#include "slwh/Base/ModelBase.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

#define TEX_MAX 12

//DMGԭʼ������ʽ

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

	void setDMGIndex(int index);		//���õڼ��ֵ�����

	SpeDMG();

	~SpeDMG();
};

#endif


