#include "SpeD3Y.h"

#define CENTER_PT				Vec3(_ZERO, 100, _ZERO)
#define HIDE_PT					Vec3(-1000, -1000, -1000)


SpeD3Y::SpeD3Y() :
m_centerPt(CENTER_PT),
m_HidePt(HIDE_PT),
m_animalManager(nullptr),
m_stage(nullptr)
{
}

SpeD3Y::~SpeD3Y()
{
	if (nullptr != m_animalManager)
	{
		m_animalManager->release();
	}

	if (nullptr != m_stage)
	{
		m_stage->release();
	}

}


void SpeD3Y::Start()
{
	m_animalPath[0] = Path_Rabbit;
	m_animalPath[1] = Path_Monkey;
	m_animalPath[2] = Path_Panda;
	m_animalPath[3] = Path_Lion;

}


void SpeD3Y::StartPlay()
{
	if (nullptr == m_sprite3d)
	{
		return;
	}

	if (nullptr == m_animalManager || nullptr == m_stage)
	{
		return;
	}
	m_sprite3d->setPosition3D(m_centerPt);
	scheduleUpdate();

}

void SpeD3Y::StopPlay()
{
	m_sprite3d->setPosition3D(m_HidePt);
	unscheduleUpdate();
}

void SpeD3Y::update(float t)
{
	int animal = static_cast<int>(Angle0To360(m_animalManager->getRotation3D().y) / 15.f);
	int stage = static_cast<int>(Angle0To360(m_stage->getRotation3D().y) / 15.f);


	int x = stage - animal;
	if (x < _ZERO)
	{
		x += Animal_Max;
	}

	SetAnimal(x);
}

void SpeD3Y::AttachAnimalAndStage(Node* animal, Node* stage)
{
	if (nullptr == animal || nullptr == stage)
	{
		return;
	}

	m_animalManager = animal;
	m_animalManager->retain();
	m_stage = stage;
	m_stage->retain();
}


void SpeD3Y::SetAnimal(int index)
{
	if (nullptr == m_sprite3d)
	{
		return;
	}

	CC_ASSERT(index >= _ZERO && index < Animal_Max);

	static int oldindex = -1;

	if (oldindex == index )
	{
		return;
	}

	oldindex = index;

	//¶¯ÎïË³Ðò
	int animal[] = _AnimalArray;

	//removeChild(m_sprite3d);
	//m_sprite3d = Sprite3D::create(m_animalPath[animal[index]]);
	m_sprite3d->initWithFile(m_animalPath[animal[index]]);
	m_sprite3d->setScale(30.f);
	//m_sprite3d->setCameraMask((int)CameraFlag::USER1);



}

