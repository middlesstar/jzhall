#include "LightAll.h"

//����������ɫ -> ������λ��
#define Color_Bule		0
#define Color_Red		1
#define Color_Green		2
#define Color_Yellow	3

#define Path_Tex_SceneLight_Root	"slwh/Model/SceneLight%d.png"


LightAll::LightAll():
m_color(enumAnimalColor::InvalidAniColor),
m_bIsRunning(false)
{
	//memset(m_lightTex , NULL , sizeof(m_lightTex));
	
}


LightAll::~LightAll()
{
	
}


void LightAll::Start()
{
	//���س����������
	char path[256];
	for (size_t i = _ZERO; i < 7; i++)
	{
		sprintf(path, Path_Tex_SceneLight_Root,i);
		m_lightTex[i] = path;
	}
	ChangeLight(0);
	//OpenLight(enumAnimalColor::ColorMax);
}


void LightAll::ChangeLight(int index)
{
	if (nullptr == m_sprite3d)
		return;

	if (index < 0 || index > 6)
		return;
	


	m_sprite3d->setTexture(m_lightTex[index]);

	///�����и���~~~~~~~~~
	///����һ�����ʣ�Materail���� �� �ı���ʵ����� �ò�����Ҫ��Ч�� ģ���ޱ仯 Sprite3D ���и�������� 
	///ʹ��Sprite3D�еķ��� ��ͬ�¸ı���ʵ����� �� ������������ ���ܴﵽ�ı��Ч��
	/*Vector<cocos2d::Node*> child = m_sprite3d->getChildren();

	for (Vector<cocos2d::Node*>::iterator ite = child.begin(); ite != child.end(); ++ite)
	{
		((Sprite3D*)*ite)->setTexture();
	}*/
}

void LightAll::OpenLight(enumAnimalColor color)
{

	if (m_bIsRunning)
	{
		return;
	}

	m_bIsRunning = true;
	m_color = color;
	schedule(schedule_selector(LightAll::Lightting), 0.5f);

}

//��������
void LightAll::CloseLight()
{
	if (m_bIsRunning)
	{
		this->unschedule(schedule_selector(LightAll::Lightting));
		ChangeLight(Color_Bule);
		m_bIsRunning = false;
	}
}

void LightAll::Lightting(float t)
{
	static uint index = 0;
	static int oldlight = -1;


	//ȫ���� 
	if (m_color == enumAnimalColor::ColorMax)
	{
		//ÿ���������ϴβ�ͬ
		int rand;
		rand = random(0, 6);
		if (oldlight == rand)
		{
			if (++rand > 6)
			{
				rand = 0;
			}
		}
		ChangeLight(rand);
		oldlight = rand;
	}
	else
	{
		if (index % 2 == 0)
		{
			switch (m_color)
			{
			case InvalidAniColor:
				//�����Ǵ��͵�~
				CCLOG("���������������δ��ֵ\n");
				break;
			case Red:
				ChangeLight(Color_Red);
				break;
			case Green:
				ChangeLight(Color_Green);
				break;
			case Yellow:
				ChangeLight(Color_Yellow);
				break;
			case ColorMax:
				//����
				break;
			default:
				CCLOG("��������������ݴ���\n");
				break;
			}
		}
		else
		{
			ChangeLight(Color_Bule);
		}
		++index;
	}
}

