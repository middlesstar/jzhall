#ifndef __SpeDMG2_H__
#define __SpeDMG2_H__

#include "slwh/Base/ModelBase.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

#define TEX_MAX 12

//DMG�����ױ�����ʽ


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

	void setDMGIndex(int index);		//���õڼ��ֵ�����

	SpeDMG2();

	~SpeDMG2();
};

#endif


