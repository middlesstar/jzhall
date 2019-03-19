#include "SpeDMG.h"



#define Path_DMG_Root			"slwh/Model/DMG/DMG%d.png"


SpeDMG::SpeDMG():
m_ps(nullptr),
m_dmgNode(nullptr),
m_dmgModel(nullptr)
{
}

SpeDMG::~SpeDMG()
{
}

void SpeDMG::setDMGIndex(int index)
{

	CC_ASSERT(index >= 0 && index < 12);
	char pathbuf[256];
	sprintf(pathbuf, Path_DMG_Root, index);
	m_dmgModel->setTexture(pathbuf);
}


void SpeDMG::StartPlay()
{
	//创建对象
	CreatObj();

	setDMGIndex(_ZERO);

	setScale(_ZERO);
	setPosition3D(Vec3(_ZERO, 56.f, _ZERO));
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

void SpeDMG::StopPlay()
{

	m_dmgNode->stopAllActions();
	runAction(
		Sequence::create(
		EaseBackIn::create(ScaleTo::create(0.5f, _ZERO)),
		CCCallFunc::create(this, callfunc_selector(SpeDMG::DeleteObj)),
		nullptr
		));
}

void SpeDMG::CreatObj()
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
	m_dmgNode->setPosition3D(Vec3(_ZERO,53.f,_ZERO));
	addChild(m_dmgNode);



	//创建特效
	m_ps = PUParticleSystem3D::create("slwh/ps3d/scripts/blackHole.pu");
	CC_ASSERT(m_ps != nullptr);
	m_ps->setPosition3D(Vec3(_ZERO, 55, _ZERO));
	m_ps->setScale(5.f);
	m_ps->setCameraMask((int)CameraFlag::USER1);
	scheduleOnce(schedule_selector(SpeDMG::EffDelay), 1.f);
	addChild(m_ps, 0);
}

void SpeDMG::DeleteObj()
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
	
}


void SpeDMG::EffDelay(float t)
{
	m_ps->startParticleSystem();
}