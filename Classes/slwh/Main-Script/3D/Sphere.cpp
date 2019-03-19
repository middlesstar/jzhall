#include "Sphere.h"



#define Path_Tex_ptNormal		"slwh/Model/Stage.png"
#define Path_Tex_ptRed			"slwh/Model/SphereRed.png"
#define Path_Tex_ptGreen		"slwh/Model/SphereGreen.png"
#define Path_Tex_ptYellow		"slwh/Model/SphereYellow.png"


Sphere::Sphere():
m_bIsRunning(false)
{
}


Sphere::~Sphere()
{
}

void Sphere::Start()
{

	ChangeColor(enumAnimalColor::InvalidAniColor);
}


void Sphere::ChangeColor(enumAnimalColor color)
{
	if (nullptr == m_sprite3d)
		return;

	switch (color)
	{
	case InvalidAniColor:
		m_sprite3d->setTexture(Path_Tex_ptNormal);
		break;
	case Red:
		m_sprite3d->setTexture(Path_Tex_ptRed);
		m_color = color;
		break;
	case Green:
		m_sprite3d->setTexture(Path_Tex_ptGreen);
		m_color = color;
		break;
	case Yellow:
		m_sprite3d->setTexture(Path_Tex_ptYellow);
		m_color = color;
		break;
	case ColorMax:
		m_sprite3d->setTexture(Path_Tex_ptNormal);
		break;
	default:
		m_sprite3d->setTexture(Path_Tex_ptNormal);
		break;
	}

}

//改变第几个板子的颜色
void Sphere::ChangeColor(int index)
{

	CC_ASSERT(index >= _ZERO && index < Animal_Max);
	if (nullptr == m_colorIndex)
	{
		return;
	}

	ChangeColor(static_cast<enumAnimalColor>(m_colorIndex[index]));
}


void Sphere::OpenLight(enumAnimalColor color)
{


	if (m_bIsRunning)
	{
		return;
	}

	m_bIsRunning = true;
	m_color = color;
	schedule(schedule_selector(Sphere::Lightting), 0.3f);

}

enumAnimalColor Sphere::OpenLight()
{
	OpenLight(m_color);
	return m_color;
}

void Sphere::Lightting(float t)
{

	
	static int index = _ZERO;
	if (index % 2 == _ZERO)
	{
		ChangeColor(m_color);
	}
	else
	{
		ChangeColor(enumAnimalColor::InvalidAniColor);
	}

	if (++index >= 0xFFFFFFFF)	
	{
		index = _ZERO;
	}


}

//结束闪灯
void Sphere::CloseLight()
{
	if (m_bIsRunning)
	{
		unschedule(schedule_selector(Sphere::Lightting));
		m_bIsRunning = false;
	}

}

void Sphere::SetColorIndex(int* const &x)
{
	if (x == nullptr)
	{
		return;
	}

	memcpy(&m_colorIndex, x, sizeof(m_colorIndex));

}