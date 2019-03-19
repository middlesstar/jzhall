#include "DownTo.h"



//板子半径
#define		_R			80
#define		_H			45.f


#define Path_Tex_ptRed			"slwh/Model/ptRed.png"
#define Path_Tex_ptGreen		"slwh/Model/ptGreen.png"
#define Path_Tex_ptYellow		"slwh/Model/ptYellow.png"
DownTo::DownTo() :
m_bIsRunning(false),
m_index(_ZERO)
{
	memset(m_downTo, NULL, sizeof(m_downTo));
}


DownTo::~DownTo()
{
}

void DownTo::Start()
{

	//摆放24块板子位置
	float a = _PI;							//角度 弧度数
	float add = 15.0f / 180.0f * _PI;			//计算弧度增量
	float x = _ZERO;
	float y = _ZERO;


	for (size_t i = _ZERO; i < Animal_Max; i++)
	{
		//设置默认材质
		m_downTo[i]->setTexture(Path_Tex_ptGreen);

		x = _R * cos(a);
		y = _R * sin(a);

		m_downTo[i]->setScale(23.f);
		m_downTo[i]->setPosition3D(Vec3(x, _H, y));
		m_downTo[i]->setRotation3D(Vec3(180, - a / _PI * 180.0f - 90, 0));
		m_downTo[i]->setCameraMask((int)CameraFlag::USER1);

		a -= add;
	}

	//scheduleUpdate();

	/*enumAnimalColor e[24]
	{
			Red, Green, Red, Yellow,
			Red, Green, Red, Yellow,
			Red, Green, Red, Yellow,
			Red, Green, Red, Yellow,
			Red, Green, Red, Yellow,
			Red, Green, Red, Yellow
	};
	ChangeColor(e);*/
}

bool DownTo::LoadModel(std::string const &path)
{

	for (size_t i = 0; i < Animal_Max; i++)
	{
		m_downTo[i] = Sprite3D::create(path);
		addChild(m_downTo[i]);
	}

	//检查最后一个是否创建成功 如果成功则认为全部创建成功 否则创建失败
	if (NULL == m_downTo[Animal_Max - 1])
	{
		return false;
	}
	return true;
}

bool DownTo::AttachSprite3D(std::string const &path)
{

	bool b = LoadModel(path);

	Start();

	return b;
}


void DownTo::ChangeColor(enumAnimalColor* const &arr)
{
	if (m_bIsRunning)
	{
		return;
	}
	m_index = _ZERO;
	m_bIsRunning = true;
	memcpy(m_color, arr, sizeof(int)* Animal_Max);
	this->schedule(schedule_selector(DownTo::ChgeColorRun), 0.05f);

}


void DownTo::ChangeColor(int* const &arr)
{
	if (m_bIsRunning)
	{
		return;
	}
	m_index = _ZERO;
	m_bIsRunning = true;
	memcpy(m_color, arr, sizeof(int)* Animal_Max);
	this->schedule(schedule_selector(DownTo::ChgeColorRun), 0.05f);
}


void DownTo::ChgeColorRun(float t)
{

	switch (m_color[m_index])
	{
	case InvalidAniColor:
		//打酱油
		log("%s",a_u8("设置板子颜色出错"));
		break;
	case Red:
		m_downTo[m_index]->setTexture(Path_Tex_ptRed);
		break;
	case Green:
		m_downTo[m_index]->setTexture(Path_Tex_ptGreen);
		break;
	case Yellow:
		m_downTo[m_index]->setTexture(Path_Tex_ptYellow);
		break;
	case ColorMax:
		//打酱油
		log("%s", a_u8("设置板子颜色出错"));
		break;
	default:
		//打酱油
		log("%s", a_u8("设置板子颜色出错"));
		break;
	}
	log("%d:%d", m_index, (int)m_color[m_index]);
	if (++m_index >= Animal_Max)
	{
		this->unschedule(schedule_selector(DownTo::ChgeColorRun));
		m_index = _ZERO;
		m_bIsRunning = false;
	}

}

//void DownTo::update(float t)
//{
//	//static int x = 0;
//	//
//	//Vec3 v3 = getPosition3D();		
//	//v3 += Vec3(0,1,0);
//	//setPosition3D(v3);
//	//	
//	//
//
//
//}