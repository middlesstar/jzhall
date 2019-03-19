#include "Line.h"


#define PATH_TEX_ROOT		"slwh/Model/10/%d.png"

Line::Line():
m_time(_ZERO)
{
}


Line::~Line()
{
}

void Line::Start()
{
	//m_tex = m_sprite3d->getMaterial(0);
	char buf[256];
	for (size_t i = _ZERO; i < 10; i++)
	{
		sprintf(buf, PATH_TEX_ROOT,i + 1);
		m_tex[i] = buf;
	}
	this->schedule(schedule_selector(Line::update),0.1f);
}

void Line::update(float delta)
{
	static int index = _ZERO;
	static bool b = true;
	if (b)
	{
		if (++index <= 9)
		{
			m_sprite3d->setTexture(m_tex[index]);

		}
		else
		{
			b = false;
		}
	}
	else
	{
		if (--index >= 0)
		{
			m_sprite3d->setTexture(m_tex[index]);

		}
		else
		{
			b = true;
		}
	}

	

	//呼吸灯
	//m_time += delta / 2.5f;
	//if ((int)m_time >= 100)
	//	m_time = 0.0f;

	//int scale = (m_time - (int)m_time) * 0.5f * 256;

	////放慢速度
	////scale /= 2;
	//Vector<cocos2d::Node*> child = m_sprite3d->getChildren();
	//
	//if ((int)(m_time) % 2 == 0)
	//{
	//	//m_sprite3d->setColor(Color3B(0, scale, 0));
	//	//这是一个大坑~~~~~改颜色对模型没有作用  问题待解决


	///*	for (Vector<cocos2d::Node*>::iterator ite = child.begin(); ite != child.end(); ++ite)
	//	{
	//		((Sprite3D*)*ite)->setColor(Color3B(0 , scale , 0));
	//	}*/
	//}
	//else
	//{
	//	/*for (Vector<cocos2d::Node*>::iterator ite = child.begin(); ite != child.end(); ++ite)
	//	{
	//		((Sprite3D*)*ite)->setColor(Color3B(0, 256 - scale, 0));
	//	}*/
	//}


}