#include "AnimalCamera.h"


#ifndef _ZERO
#define _ZERO 0
#endif // !_ZERO




AnimalCamera::AnimalCamera() :
m_camera(nullptr)
{
}


AnimalCamera::~AnimalCamera()
{
}


bool AnimalCamera::AttachCamera(Camera* const &ca)
{
	if (nullptr == ca || m_camera == ca)
	{
		return false;
	}
	m_camera = ca;
	addChild(m_camera);

	return true;

}


