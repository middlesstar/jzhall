#include "MyCamera.h"


#ifndef _ZERO
#define _ZERO 0
#endif // !_ZERO

//#define VEC3_START				Vec3(0.f, 300.f, 180.0f)

#define RUN_TIME				3.f
//#define VEC3_START				Vec3(0.f, 500.f, 150.0f)
#define VEC3_START				Vec3(0.f, 250.f, 145.0f)
#define VEC3_END				Vec3(0.f ,100.f, 80.f)
#define VEC3_MID				Vec3(0.f ,200.f, 180.f)
#define VEC3_FOCUS				Vec3(0.f ,80.f, 40.f)


MyCamera::MyCamera():
m_camera(nullptr)
{
}

MyCamera::~MyCamera()
{
}

void MyCamera::Start()
{

	m_camera->setCameraFlag(CameraFlag::USER1);

	m_camera->setPosition3D(VEC3_START);

	Camera2Center();


	//²âÊÔ
	//Close2AnimalMid();
}

void MyCamera::Camera2Center()
{
	m_camera->lookAt(VEC3_FOCUS);

}

void MyCamera::Close2Animal()
{
	m_camera->runAction(CCSequence::create(
		CCEaseExponentialInOut::create(MoveTo::create(RUN_TIME, VEC3_END)), 
		CCCallFunc::create(this, callfunc_selector(MyCamera::CallBack)),
		NULL));

	scheduleUpdate();
}

void MyCamera::Close2AnimalMid()
{
	m_camera->runAction(CCSequence::create(
		CCEaseExponentialInOut::create(MoveTo::create(RUN_TIME, VEC3_MID)),
		CCCallFunc::create(this, callfunc_selector(MyCamera::CallBack)),
		NULL));

	scheduleUpdate();
}

void MyCamera::FarAway()
{
	m_camera->runAction(CCSequence::create(
		CCEaseExponentialInOut::create(MoveTo::create(RUN_TIME, VEC3_START)),
		CCCallFunc::create(this, callfunc_selector(MyCamera::CallBack)),
		NULL));

	scheduleUpdate();
}

bool MyCamera::AttachCamera(Camera* const &ca)
{
	if (nullptr == ca || m_camera == ca)
	{
		return false;
	}
	m_camera = ca;
	addChild(m_camera);

	Start();
	return true;

}

void MyCamera::update(float deltaTime)
{

	Camera2Center();
}

void MyCamera::CallBack()
{
	unscheduleUpdate();
}



