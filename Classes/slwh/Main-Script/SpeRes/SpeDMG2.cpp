#include "SpeDMG2.h"



#define Path_DMG_Root			"slwh/Model/DMG/DMG%d.png"


SpeDMG2::SpeDMG2() :
m_ps(nullptr),
m_dmgNode(nullptr),
m_dmgModel(nullptr)
{
}

SpeDMG2::~SpeDMG2()
{
}

void SpeDMG2::setDMGIndex(int index)
{

	CC_ASSERT(index >= 0 && index < 12);
	char pathbuf[256];
	sprintf(pathbuf, Path_DMG_Root, index);
	m_dmgModel->setTexture(pathbuf);
}


void SpeDMG2::StartPlay(int index)
{
	//创建对象
	CreatObj(index);

	setDMGIndex(11);

	setScale(_ZERO);
	setPosition3D(Vec3(_ZERO, 56.f, _ZERO));

	for (size_t i = 0; i < 23; i++)
	{
		m_points[i]->runAction(
			Sequence::create(
			DelayTime::create(i * 0.25f),
			EaseBackOut::create(ScaleTo::create(0.5f, 1.f)),
			nullptr)
			);
	}
	

	runAction(
		Sequence::create(
		DelayTime::create(0.5f),
		EaseBackOut::create(ScaleTo::create(0.88f, 0.78f)),
		nullptr)
		);

	auto ac1 = MoveBy::create(0.8f, Vec3(_ZERO, -15, _ZERO));
	auto ac2 = ac1->reverse();
	auto seq = RepeatForever::create(Sequence::create(ac1, ac2, nullptr));
	m_dmgNode->runAction(seq);
}

void SpeDMG2::StopPlay()
{

	for (size_t i = 0; i < 23; i++)
	{
		m_points[i]->runAction(
			Sequence::create(
			DelayTime::create(i * 0.25f),
			EaseBackOut::create(ScaleTo::create(0.5f,_ZERO)),
			nullptr)
			);
	}

	m_dmgNode->stopAllActions();
	runAction(
		Sequence::create(
		DelayTime::create(23 * 0.25f),
		EaseBackIn::create(ScaleTo::create(0.5f, _ZERO)),
		CCCallFunc::create(this, callfunc_selector(SpeDMG2::DeleteObj)),
		nullptr
		));
}

void SpeDMG2::CreatObj(int index)
{

	//模型的坐标轴不在模型中心，在模型外面套个壳子，
	m_dmgNode = Node::create();
	m_dmgModel = Sprite3D::create(Path_DMG);
	CC_ASSERT(m_dmgModel != nullptr);
	m_dmgModel->setCameraMask((int)CameraFlag::USER1);
	m_dmgModel->setScale(30.f);
	m_dmgModel->setPosition3D(Vec3(_ZERO, -580, _ZERO));
	m_dmgModel->setRotation3D(Vec3(_ZERO, _ZERO, 180));
	m_dmgNode->addChild(m_dmgModel);
	m_dmgNode->setRotation3D(Vec3(-115, _ZERO, _ZERO));
	m_dmgNode->setPosition3D(Vec3(_ZERO, 53.f, _ZERO));
	addChild(m_dmgNode);

	//创建指针
	int in = index;
	for (size_t i = 0; i < 23; i++)
	{
		m_points[i] = Node::create();
		auto spr = Sprite3D::create(Path_Point);
		CC_ASSERT(m_points[i] != nullptr);
		spr->setTexture("slwh/Model/Point.png");
		spr->setScale(40.f);
		spr->setPosition3D(Vec3(-65, -5, _ZERO));
		spr->setRotation3D(Vec3(_ZERO, _ZERO, 180));
		spr->setCameraMask((int)CameraFlag::USER1);
		m_points[i]->addChild(spr); 
		m_points[i]->setRotation3D(Vec3(_ZERO, ++in * 15, _ZERO));
		m_points[i]->setScale(_ZERO);
		addChild(m_points[i]);
	}
	

	//创建特效
	m_ps = PUParticleSystem3D::create("slwh/ps3d/scripts/blackHole.pu");
	CC_ASSERT(m_ps != nullptr);
	m_ps->setPosition3D(Vec3(_ZERO, 55, _ZERO));
	m_ps->setScale(5.f);
	m_ps->setCameraMask((int)CameraFlag::USER1);
	scheduleOnce(schedule_selector(SpeDMG2::EffDelay), 1.f);
	addChild(m_ps, 0);
}

void SpeDMG2::DeleteObj()
{
	if (m_dmgNode != nullptr)
	{
		m_dmgNode->removeFromParent();
		m_dmgNode = nullptr;
		m_dmgModel = nullptr;
	}

	if (m_ps != nullptr)
	{
		m_ps->removeFromParent();
		m_ps = nullptr;
	}

	for (size_t i = 0; i < 23; i++)
	{
		if (m_points[i]!= nullptr)
		{
			m_points[i]->removeFromParent();
			m_points[i] = nullptr;
		}
	}



}


void SpeDMG2::EffDelay(float t)
{
	m_ps->startParticleSystem();
}