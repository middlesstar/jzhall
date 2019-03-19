#include "SpeD4X.h"


SpeD4X::SpeD4X():
m_seat(nullptr),
m_ps(nullptr)
{
	memset(m_animal, NULL, sizeof(m_animal));
}

SpeD4X::~SpeD4X()
{
}

void SpeD4X::StartPlay()
{
	//创建对象
	CreatObj();

	setScale(_ZERO);
	setPosition3D(Vec3(_ZERO,56.f,_ZERO));
	runAction(
		Sequence::create(
		DelayTime::create(0.5f),
		EaseBackOut::create(ScaleTo::create(0.88f,0.78f)),
		nullptr)
		);
}

void SpeD4X::StopPlay()
{
	//m_sprite3d->setPosition3D(m_HidePt);
	runAction(
		Sequence::create(
		EaseBackIn::create(ScaleTo::create(0.5f,_ZERO)),
		CCCallFunc::create(this,callfunc_selector(SpeD4X::DeleteObj)),
		nullptr
		));
}

void SpeD4X::CreatObj()
{
	m_seat = Sprite3D::create(Path_D4X);
	CC_ASSERT(m_seat != nullptr);
	m_seat->setCameraMask((int)CameraFlag::USER1);
	m_seat->setScale(30.f);
	m_seat->setPosition3D(Vec3(_ZERO,-535, _ZERO));
	m_seat->setRotation3D(Vec3(_ZERO, _ZERO, 180));
	addChild(m_seat);


	m_animal[0] = Sprite3D::create(Path_Rabbit);
	CC_ASSERT(m_animal[0] != nullptr);
	m_animal[0]->setCameraMask((int)CameraFlag::USER1);
	addChild(m_animal[0]);
	m_animal[0]->setScale(0.448f);
	m_animal[0]->setPosition3D(Vec3(15,20,_ZERO));
	m_animal[0]->setRotation3D(Vec3(_ZERO, 90, _ZERO));
	auto animation0 = Animation3D::create(Path_Rabbit);
	if (animation0)
	{
		m_animal[0]->runAction(RepeatForever::create(Animate3D::create(animation0, 3.333f, 3.836f)));
	}


	m_animal[1] = Sprite3D::create(Path_Monkey);
	CC_ASSERT(m_animal[1] != nullptr);
	m_animal[1]->setCameraMask((int)CameraFlag::USER1);
	addChild(m_animal[1]);
	m_animal[1]->setScale(0.368f);
	m_animal[1]->setPosition3D(Vec3(_ZERO, 20, 15));
	m_animal[1]->setRotation3D(Vec3(_ZERO, _ZERO, _ZERO));
	auto animation1 = Animation3D::create(Path_Monkey);
	if (animation1)
	{
		m_animal[1]->runAction(RepeatForever::create(Animate3D::create(animation1, 5.666, 3.84f)));
	}


	m_animal[2] = Sprite3D::create(Path_Panda);
	CC_ASSERT(m_animal[2] != nullptr);
	m_animal[2]->setCameraMask((int)CameraFlag::USER1);
	m_animal[2]->setScale(0.368f);
	addChild(m_animal[2]);
	m_animal[2]->setPosition3D(Vec3(-15, 20, _ZERO));
	m_animal[2]->setRotation3D(Vec3(_ZERO, -90, _ZERO));
	auto animation2 = Animation3D::create(Path_Panda);
	if (animation2)
	{
		m_animal[2]->runAction(RepeatForever::create(Animate3D::create(animation2, 3.9f, 3.8333f)));
	}


	m_animal[3] = Sprite3D::create(Path_Lion);
	CC_ASSERT(m_animal[3] != nullptr);
	m_animal[3]->setCameraMask((int)CameraFlag::USER1);
	m_animal[3]->setScale(0.368f);
	addChild(m_animal[3]);
	m_animal[3]->setPosition3D(Vec3(_ZERO, 20, -15));
	m_animal[3]->setRotation3D(Vec3(_ZERO, 180, _ZERO));
	auto animation3 = Animation3D::create(Path_Lion);
	if (animation3)
	{
		m_animal[3]->runAction(RepeatForever::create(Animate3D::create(animation3, 4.333f, 3.836f)));
	}

	//创建特效
	m_ps = PUParticleSystem3D::create("slwh/ps3d/scripts/flareShield.pu");
	CC_ASSERT(m_ps != nullptr);
	m_ps->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
	m_ps->setScale(8.f);
	m_ps->setCameraMask((int)CameraFlag::USER1);
	scheduleOnce(schedule_selector(SpeD4X::EffDelay),1.f);
	addChild(m_ps, 0);
}

void SpeD4X::DeleteObj()
{

	if (m_seat != nullptr)
	{
		m_seat->removeFromParent();
		m_seat = nullptr;
	}

	for (size_t i = _ZERO; i < ANIMAL_TYPE; i++)
	{
		if (m_animal[i] != nullptr)
		{
			m_animal[i]->removeFromParent();
			m_animal[i] = nullptr;
		}
	}

	if (m_ps != nullptr)
	{
		m_ps->removeFromParent();
		m_ps = nullptr;
	}
}


void SpeD4X::EffDelay(float t)
{
	m_ps->startParticleSystem();
}