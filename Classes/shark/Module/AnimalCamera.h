#ifndef __AnimalCamera_H__
#define __AnimalCamera_H__

#include "shark/Base/AnimalBase.h"

/*
主相机脚本
*/


class AnimalCamera :
	public AnimalBase
{
protected:
	Camera*				m_camera;

public:
	AnimalCamera();
	~AnimalCamera();

	bool AttachCamera(Camera* const &spr);


};

#endif



