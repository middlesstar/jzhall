#ifndef __SpeD3Y_H__
#define __SpeD3Y_H__

#include "slwh/Base/ModelBase.h"


#define ANIMAL_TYPE					4

class SpeD3Y :
	public ModelBase
{
protected:
	const Vec3				m_centerPt;
	const Vec3				m_HidePt;

	Node*					m_animalManager;
	Node*					m_stage;

	std::string				m_animalPath[ANIMAL_TYPE];
public:
	SpeD3Y();
	~SpeD3Y();

	void StartPlay();
	void SetAnimal(int index);
	void AttachAnimalAndStage(Node* animal, Node* stage);
	void StopPlay();
protected:
	void update(float t);
	void Start();

};

#endif
