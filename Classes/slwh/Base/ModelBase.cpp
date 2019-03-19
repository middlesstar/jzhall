#include "ModelBase.h"

ModelBase::ModelBase():
m_sprite3d(nullptr)
{
}

ModelBase::~ModelBase()
{
	/*if (nullptr != m_sprite3d)
		m_sprite3d->release();*/

}

bool ModelBase::AttachSprite3D(Sprite3D* const &spr)
{
	if (nullptr == spr || m_sprite3d == spr)
	{
		return false;
	}
	m_sprite3d = spr;
	this->addChild(m_sprite3d);
	//m_sprite3d->retain();
	//this->onEnter();
	//this->onEnterTransitionDidFinish();
	//��ȷ������Դ�� ��ʼ���������Դ
	Start();
	return true;
}

void ModelBase::Rotate(const Vec3 &forward, const float &angle)
{
	Vec3 v3 = getRotation3D();
	v3 += forward * angle;
	setRotation3D(v3);
}

void ModelBase::Rotate(const Vec3 &eulerAngles)
{
	Vec3 v3 = getRotation3D();
	v3 += eulerAngles;
	setRotation3D(v3);
}

void ModelBase::Rotate(const float &xAngle, const float &yAngle, const float &zAngle)
{
	Vec3 v3 = getRotation3D();
	v3 += Vec3(xAngle, yAngle, zAngle);
	setRotation3D(v3);
}

float ModelBase::Angle0To360(float angle)
{

	if (angle < _ZERO)
	{
		angle = -angle;
		//ȡС��
		float f = angle - (int)angle;
		return 360 - ((int)angle % 360 + f);
	}
	else
	{
		//ȡС��
		float f = angle - (int)angle;
		return (int)angle % 360 + f;
	}
	
}

