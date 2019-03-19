#include "Animal.h"


//const float Animal::Max_CenterTime = 1.5f;
//const Vec3 Animal::m_centerPt = Vec3(0, 04.2f, 0);
//const float Animal::m_animalMoveSpeed = 2.3f;                           //动物移动速度
//const float Animal::m_animalMoveSpeedF = 0.5f;                          //动物移动抛物运动分速度
//const float Animal::m_animalRotateSpeed = 2.0f;							//动物旋转速度

Animal::Animal() :
m_ac1(nullptr),
m_ac2(nullptr),
m_ac3(nullptr),
//m_centerTime(_ZERO),
//m_childAnimal(Invalidate),
m_bIsAcRun(false)
//m_ps(nullptr)
{

}

Animal::~Animal()
{
	if (nullptr != m_ac1)
		m_ac1->release();

	if (nullptr != m_ac2)
		m_ac2->release();

	if (nullptr != m_ac3)
		m_ac3->release();
}

bool Animal::LoadAction(Animate3D* const &one, Animate3D* const &two, Animate3D* const &three)
{
	if (nullptr == one || nullptr == two)
		return false;



	//m_ps = PUParticleSystem3D::create("slwh/ps3d/scripts/atomicity.pu");
	//m_ps->setPosition3D(Vec3(_ZERO, 0.5, _ZERO));
	//m_ps->setScale(0.05f);
	//m_ps->setCameraMask((int)CameraFlag::USER1);
	//addChild(m_ps, 0);


	m_ac1 = one;
	m_ac1->retain();
	m_ac2 = two;
	m_ac2->retain();
	m_ac3 = three;
	m_ac3->retain();
	return true;

}

void Animal::PlayNormalAc()
{
	if (nullptr == m_sprite3d || nullptr == m_ac1 )
		return;

	//随便选个动画播放 1 3 为普通动作
	if (random(0,1))
	{
		m_sprite3d->runAction(m_ac1);
	}
	else
	{
		m_sprite3d->runAction(m_ac3);
	}
}

void Animal::PlayHappyAc()
{
	if (nullptr == m_sprite3d || nullptr == m_ac2)
		return;

	//太卡 暂时注释掉
	//m_ps->startParticleSystem();

	m_sprite3d->runAction(m_ac2);
}

void Animal::updateAc(float t)
{
	if (m_bIsAcRun)
	{
		PlayNormalAc();
		//等个3到5秒开始播动画
		scheduleOnce(schedule_selector(Animal::Bridge), random(3.5f,5.5f));
	}
}

void Animal::Bridge(float t)
{
	scheduleOnce(schedule_selector(Animal::updateAc), 0.01f);
}

void Animal::OpenNormalAc(bool b)
{
	if (m_bIsAcRun == b) return;

	m_bIsAcRun = b;
	if (m_bIsAcRun)
	{
		//开始播放动画
		scheduleOnce(schedule_selector(Animal::Bridge), 0.01f);
	}
}