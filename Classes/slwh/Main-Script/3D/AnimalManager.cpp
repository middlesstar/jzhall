#include "AnimalManager.h"


//����λ������
#define		_R					108.f						//�뾶
#define		_H					50.f						//�߶�
#define		_A					15.f						//ƫ�� ÿ���������15��

//��ֵϵ��
//#define		_Slerp				0.05f


#define	ANIMAL_MOVE_V_Y		10
#define	CENTER_POS			Vec3(_ZERO, 65.f, _ZERO)
#define	Running_TIME		3.f
#define	UPDATE_TIME			0.01f

#define Path_Tex_Step_Normal		"slwh/Model/seatBlue.png"
#define Path_Tex_Step_Red			"slwh/Model/seatRed.png"
#define Path_Tex_Step_Green			"slwh/Model/seatGreen.png"
#define Path_Tex_Step_Yellow		"slwh/Model/seatYellow.png"


AnimalManager::AnimalManager() :
m_speed(_ZERO),
m_runTime(_ZERO),
m_targetAngle(_ZERO),
m_bIsLerp(false),
m_animalState(AnimalNormal)
{
	memset(m_step, NULL, sizeof(m_step));
}

AnimalManager::~AnimalManager()
{
}

void AnimalManager::Start()
{


	//�ڷ�24�����λ�úͶ���
	float a = _PI;                        //�Ƕ� ������ ����߿�ʼ����
	float add = _A / 180.0f * _PI;        //���㻡������
	float x = _ZERO;
	float y = _ZERO;

	for (size_t i = _ZERO; i < Animal_Max; i++)
	{
		//����Ĭ�ϲ���
		m_step[i]->setTexture(Path_Tex_Step_Normal);
		m_animal[i].addChild(m_step[i]);
	
		x = _R * cos(a);
		y = _R * sin(a);


		m_animal[i].setScale(35.f);					//�����������õĴ�С
		m_animal[i].setPosition3D(Vec3(x, _H, y));
		m_animal[i].setRotation3D(Vec3(_ZERO, -a / _PI * 180.0f - 90, _ZERO));

		a -= add;
	}
	//��������
	OpenAniNorAc();
	

	//����
	//StartPlay(1);
	//ResPlay(1,1);
}

void AnimalManager::ResDance(int ptIndex)
{
	m_centerAnimal = ptIndex - m_targetAngle / _A;

	if (m_centerAnimal < _ZERO)
	{
		m_centerAnimal += Animal_Max;
	}

	//ֱ�ӿ�ʼ���� �� ����
	CallBackMoveToCenterEnd();
}

void AnimalManager::ResPlay(int ptIndex, enumAnimalColor e1)
{
	ResPlay(ptIndex, (int)e1);
}

void AnimalManager::ResPlay(int ptIndex, int color) //color��ɫ0~2
{

	m_centerAnimal = ptIndex - m_targetAngle / _A;

	if (m_centerAnimal < _ZERO)
	{
		m_centerAnimal += Animal_Max;
	}
	m_oldPos = m_animal[m_centerAnimal].getPosition3D();
	m_oldRot = m_animal[m_centerAnimal].getRotation3D();


	m_step[m_centerAnimal]->setTexture(getStepTex(color));

	//m_animal[m_centerAnimal]->setRotation3D(Vec3(_ZERO, _ZERO, _ZERO));
	auto ac0 = EaseExponentialInOut::create(MoveTo::create(Running_TIME, CENTER_POS));
	auto ac1 = EaseExponentialInOut::create(RotateTo::create(Running_TIME, Vec3(_ZERO, -getRotation3D().y, _ZERO)));
	
	auto ac2 = MoveBy::create(Running_TIME / 2 ,Vec3(_ZERO,15,_ZERO));
	auto ac3 = ac2->reverse();
	auto seq0 = Sequence::create(ac2, ac3, nullptr);

	auto spa0 = Spawn::create(ac0, ac1,seq0,nullptr);

	auto seq1 = Sequence::create(spa0, CallFunc::create(this, callfunc_selector(AnimalManager::CallBackMoveToCenterEnd)), nullptr);
	m_animal[m_centerAnimal].runAction(seq1);

	//schedule(schedule_selector(AnimalManager::AnimalToCenterUpdate), UPDATE_TIME);
}

void AnimalManager::ResStop()
{

	auto ac0 = EaseExponentialInOut::create(MoveTo::create(Running_TIME, m_oldPos));
	auto ac1 = EaseExponentialInOut::create(RotateTo::create(Running_TIME, m_oldRot));

	auto ac2 = MoveBy::create(Running_TIME / 2, Vec3(_ZERO, 15, _ZERO));
	auto ac3 = ac2->reverse();
	auto seq0 = Sequence::create(ac2, ac3, nullptr);

	auto spa0 = Spawn::create(ac0, ac1, seq0, nullptr);
	
	auto callfunc = CallFunc::create([this](){m_step[m_centerAnimal]->setTexture(Path_Tex_Step_Normal); });

	auto seq1 = Sequence::create(spa0, callfunc, nullptr);
	m_animal[m_centerAnimal].runAction(seq1);

	//schedule(schedule_selector(AnimalManager::AnimalToCenterUpdate), UPDATE_TIME);
}

void AnimalManager::CallBackMoveToCenterEnd()
{	
	int animal[] = _AnimalArray;
	//�������
	switch (static_cast<enumAnimalType>(animal[m_centerAnimal]))
	{
	case enumAnimalType::Animal0:
		_PLAY_MUSCI_EFF(RABBIT_CALL);
		break;
	case enumAnimalType::Animal1:
		_PLAY_MUSCI_EFF(MONKEY_CALL);
		break;
	case enumAnimalType::Animal2:
		_PLAY_MUSCI_EFF(PANDA_CALL);
		break;
	case enumAnimalType::Animal3:
		_PLAY_MUSCI_EFF(LION_CALL);
		break;
	default:
		break;

	}
	//�ƶ��������ﲥ�Ŷ���
	m_animal[m_centerAnimal].PlayHappyAc();

}

//void AnimalManager::AnimalToCenterUpdate(float t)
//{
//
//	static float time = _ZERO;
//	Vec3 v3 = m_animal[m_centerAnimal].getPosition3D();
//	
//	if (time < Running_TIME / 2)
//	{
//		v3 += Vec3(_ZERO, t * ANIMAL_MOVE_V_Y, _ZERO);
//		m_animal[m_centerAnimal].setPosition3D(v3);
//	}
//	else if (time < Running_TIME)
//	{
//		v3 -= Vec3(_ZERO, t * ANIMAL_MOVE_V_Y, _ZERO);
//		m_animal[m_centerAnimal].setPosition3D(v3);
//	}
//	else
//	{
//		time = _ZERO;
//		unschedule(schedule_selector(AnimalManager::AnimalToCenterUpdate));
//	}
//	
//	time += t;
//}

bool AnimalManager::AttachSprite3D(
	std::string const &Ani0Path,
	std::string const &Ani1Path,
	std::string const &Ani2Path,
	std::string const &Ani3Path,
	std::string const &stepPath)
{

	bool b = LoadAnimal(Ani0Path, Ani1Path, Ani2Path, Ani3Path) & LoadStep(stepPath);

	Start();

	return b;

}


bool AnimalManager::LoadAnimal(
	std::string const &Ani0Path,
	std::string const &Ani1Path,
	std::string const &Ani2Path,
	std::string const &Ani3Path)
{

	//�Զ���������򣨹̶��������ݣ�
	/*{
		1, 0, 1, 3,
		0, 1, 0, 2,
		0, 2, 1, 3,
		2, 0, 1, 0,
		3, 0, 1, 0,
		2, 0, 1, 3,
	};*/

	int x[] = _AnimalArray;

	Sprite3D* spr = nullptr;
	Animation3D* animation = nullptr;
	for (size_t i = _ZERO; i < Animal_Max; i++)
	{
		switch (x[i])
		{
		case 0:
			spr = Sprite3D::create(Ani0Path);
			if (nullptr == spr)
				return false;
			//������Ҫ�Ŵ�
			spr->setScale(0.016f);
			spr->setRotation3D(Vec3(_ZERO, _ZERO, _ZERO));
			spr->setPosition3D(Vec3(_ZERO,_ZERO,_ZERO));
			m_animal[i].AttachSprite3D(spr);
			animation = Animation3D::create(Ani0Path);
			if (animation)
			{
				//�°汾����3������  ����0��0 ~ 3   �赸��3.33 ~ 7.166(3.836)    ����1��9.66 ~ 11(1.34)
				m_animal[i].LoadAction(Animate3D::create(animation, 0.f, 3.f), Animate3D::create(animation, 3.3333f, 3.836f), Animate3D::create(animation, 9.66667f, 1.34f));
			}
			break;
		case 1:
			spr = Sprite3D::create(Ani1Path);
			if (nullptr == spr)
				return false;
			spr->setScale(0.012f);
			spr->setRotation3D(Vec3(_ZERO, _ZERO, _ZERO));
			spr->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
			m_animal[i].AttachSprite3D(spr);
			animation = Animation3D::create(Ani1Path);
			if (animation)
			{
				//�°汾����3������  ����0��0 ~ 3   ����1��3.33 ~ 5.33(2)    �赸��5.66 ~ 9.5(3.84)
				m_animal[i].LoadAction(Animate3D::create(animation, 0.f, 3.f), Animate3D::create(animation, 5.66667f,3.84f), Animate3D::create(animation, 3.33333f, 2.f));
			}
			break;
		case 2:
			spr = Sprite3D::create(Ani2Path);
			if (nullptr == spr)
				return false;
			spr->setScale(0.012f);
			spr->setRotation3D(Vec3(_ZERO, _ZERO, _ZERO));
			spr->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
			m_animal[i].AttachSprite3D(spr);
			animation = Animation3D::create(Ani2Path);
			if (animation)
			{
				//�°汾��è3������  ����0��0 ~ 2(2)   ����1��2.33 ~ 3.6(1.27)    �赸��3.9 ~ 7.733(3.833)
				m_animal[i].LoadAction(Animate3D::create(animation, 0.f, 2.f), Animate3D::create(animation, 3.9f, 3.833f), Animate3D::create(animation, 2.333333f, 1.27f));
			}
			break;
		case 3:
			spr = Sprite3D::create(Ani3Path);
			if (nullptr == spr)
				return false;
			spr->setScale(0.012f);
			spr->setRotation3D(Vec3(_ZERO, _ZERO, _ZERO));
			spr->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
			m_animal[i].AttachSprite3D(spr);
			animation = Animation3D::create(Ani3Path);
			if (animation)
			{
				//�°汾ʨ��3������  ����0��0 ~ 2 (2)  ����1��2 ~ 4(2)    �赸��4.3333 ~ 8.33333(4)
				m_animal[i].LoadAction(Animate3D::create(animation, 0.f, 2.f), Animate3D::create(animation, 4.3333f,4.f), Animate3D::create(animation, 2.f, 2.f));
			}
			break;
		default:
			//������ͣ�
			break;
		}
		m_animal[i].setCameraMask((int)CameraFlag::USER1);
		addChild(&m_animal[i]);
	}

	return true;
}

//����
std::string AnimalManager::getStepTex(int color)
{
	switch (color)
	{
	case 0: return Path_Tex_Step_Red;
	case 1: return Path_Tex_Step_Green;
	case 2: return Path_Tex_Step_Yellow;
	default: return Path_Tex_Step_Normal;
	}
}

bool AnimalManager::LoadStep(std::string const &stepPath)
{
	for (size_t i = _ZERO; i < Animal_Max; i++)
	{
		m_step[i] = Sprite3D::create(stepPath);
		if (nullptr ==  m_step[i])
		{
			return false;
		}
		m_step[i]->setPosition3D(Vec3(0, -0.1f, 0));
		m_step[i]->setRotation3D(Vec3(180, 90, 0));
		m_step[i]->setCameraMask((int)CameraFlag::USER1);
	}

	return true;
}

void AnimalManager::update(float deltaTime)
{

	//������ת�˶�

	//4Ԫ����ֵ����������ʱ�洢������
	//Quaternion quat;

	switch (m_animalState)
	{
	case AnimalNormal:
		//�����Ǵ��͵�
		break;
	case AnimalptSpeedAdd:
		//����
		m_speed += Speed_Add * deltaTime;

		if (m_runTime > (float)AnimalptSpeedAdd)
		{
			m_animalState = AnimalSpeedMax;                //�л�������ٶ�����
		}
		break;
	case AnimalSpeedMax:
		//�����˶�

		if (m_runTime > (float)AnimalSpeedMax)
		{
			m_animalState = AnimalSpeedSub;                //�л�������
		}

		break;
	case AnimalSpeedSub:
		//����             
		m_speed -= Speed_Sub * deltaTime;

		if (m_runTime > (float)AnimalSpeedSub)
		{
			//ָ���л�Ϊ��С�ٶ�
			m_animalState = AnimalSpeedMin;
		}

		break;
	case AnimalSpeedMin:
		if (!m_bIsLerp)
		{
			if (true == DetalSpeed(getRotation3D().y))
			{
				m_bIsLerp = true;
				runAction(CCEaseExponentialOut::create(RotateTo::create(static_cast<int>(AnimalSpeedMin - m_runTime), Vec3(_ZERO, -360 + m_targetAngle, _ZERO))));
			}
			if (m_runTime > (float)AnimalSpeedMin)
			{
				//���ﾲֹ
				m_animalState = AnimalNormal;
				//�ر�update
				unscheduleUpdate();

			}		
		}
		break;
	default:
		m_animalState = AnimalNormal;
		//�ر�update
		unscheduleUpdate();
		break;
	}

	if (!m_bIsLerp)
	{
		Rotate(Vec3::UNIT_Y, -m_speed * deltaTime);
	}
	m_runTime += deltaTime;
}

void AnimalManager::OpenAniNorAc(bool b /*= true*/)
{

	for (size_t i = _ZERO; i < Animal_Max; i++)
	{
		m_animal[i].OpenNormalAc(b);
		//m_animal[i]->PlayHappyAc();
	}
}

void AnimalManager::StartPlay(int index, int colorindex)
{
	CC_ASSERT(index >= _ZERO && index < Animal_Max);
	CC_ASSERT(colorindex >= _ZERO && colorindex < Animal_Max);

	//����ת��
	index = colorindex - index;
	if (index<_ZERO)
	{
		index += Animal_Max;
	}
	//�ٶȸ�λ
	m_speed = _ZERO;
	//ʱ�临λ
	m_runTime = _ZERO;
	//�رն���
	OpenAniNorAc(false);
	//����Ŀ��Ƕ�
	m_targetAngle = index * _A;
	//��ȷ���Ƹ�λ
	m_bIsLerp = false;
	//��ʼ����
	m_animalState = AnimalptSpeedAdd;
	//����update
	scheduleUpdate();

}

bool AnimalManager::DetalSpeed(float t)
{

	//��ǰ�Ƕ���Ŀ��ǶȲ��ʱ�洢������
	float angle = Angle0To360(getRotation3D().y) - m_targetAngle;
	if (angle < _ZERO)
	{
		angle += 360;
	}

	if (angle <= 120 && angle >= 45)
	{
		//log("--------------%f,%f----------------", Angle0To360(getRotation3D().y), m_targetAngle);
		return true;
	}

	return false;

}



