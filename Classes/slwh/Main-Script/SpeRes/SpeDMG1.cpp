#include "SpeDMG1.h"


SpeDMG1::SpeDMG1()
{
}

SpeDMG1::~SpeDMG1()
{
}

void SpeDMG1::StartPlay(Sprite3D* point)
{
	for (size_t i = 0; i < 11; i++)
	{
		m_points[i] = Sprite3D::create(Path_Point);
		CC_ASSERT(m_points[i] != nullptr);


		m_points[i]->setTexture("slwh/Model/Point.png");
		m_points[i]->setScale(30.f);
		m_points[i]->setPosition3D(Vec3(-55, 53.f, _ZERO));
		m_points[i]->setRotation3D(Vec3(_ZERO, _ZERO, 180));
		m_points[i]->setCameraMask((int)CameraFlag::USER1);
		addChild(m_points[i]);
	}



}

void SpeDMG1::StopPlay()
{
}

void SpeDMG1::DeleteObj()
{
}

