#include "AnimalBase.h"


AnimalBase::AnimalBase() :
m_sprite3d(nullptr)
{
}


AnimalBase::~AnimalBase()
{
	/*if (nullptr != m_sprite3d)
		m_sprite3d->release();*/

}


bool AnimalBase::AttachSprite3D(Sprite3D* const &spr)
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
	//正确关联资源后 开始管理这个资源
	//setContent();
	return true;
}



