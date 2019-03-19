#ifndef __MyCamera_H__
#define __MyCamera_H__

#include "slwh/Base/ModelBase.h"

/*
主相机脚本
*/


class MyCamera :
	public ModelBase
{
protected:
	Camera*				m_camera;

public:
	MyCamera();
	~MyCamera();

	bool AttachCamera(Camera* const &spr);
	void Close2Animal();
	void Close2AnimalMid();
	void FarAway();
protected:
	void Camera2Center();
	void Start();
	void update(float t);

	void CallBack();
};

#endif



