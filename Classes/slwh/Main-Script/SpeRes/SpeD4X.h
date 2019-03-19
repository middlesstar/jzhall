#ifndef __SpeD4X_H__
#define __SpeD4X_H__

#include "slwh/Base/ModelBase.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

#define ANIMAL_TYPE					4


class SpeD4X :
	public ModelBase
{
protected:
	Sprite3D*				m_seat;
	Sprite3D*				m_animal[ANIMAL_TYPE];
	PUParticleSystem3D*		m_ps;
protected:
	void CreatObj();

	void DeleteObj();

	void EffDelay(float t);
public:
	void StartPlay();

	void StopPlay();
public:
	SpeD4X();

	~SpeD4X();


};

#endif


