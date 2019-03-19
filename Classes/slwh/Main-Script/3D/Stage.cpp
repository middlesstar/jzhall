 #include "Stage.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"


#define SPEED_ADD  100.0f                        //���ٶ�
#define SPEED_SUB  60.0f                         //���ٶ�
#define SPEED_MIN  1.5f                          //��С�ٶ�

//��ֵϵ��
//#define	_Slerp		0.05f
#define	_A			15				

Stage::Stage():
m_speed(_ZERO),                               
m_runTime(_ZERO),                                                              
m_bIsLerp(false),                    
m_targetAngle(_ZERO),
m_ptState(ptNormal),
m_sphere(nullptr),
m_open(nullptr),
m_close(nullptr),
m_stage(nullptr),
m_point(nullptr),
m_ps(nullptr),
m_ps1(nullptr)
{
}

Stage::~Stage()
{
	if (nullptr != m_open)
	{
		m_open->release();
	}
	
	if (nullptr != m_close)
	{
		m_close->release();
	}

}

void Stage::Start()
{

	//cocos��3d����֧�ֵĲ�̫�ã��˾���������ã�Ҫ��Ȼģ���ڿ�ʼ��ʱ�����ʧ
	OpenSphere(false);


	//FileUtils::getInstance()->addSearchPath("materials");
	m_ps = PUParticleSystem3D::create("slwh/ps3d/scripts/woosh.pu");
	m_ps->setPosition3D(Vec3(_ZERO, 40, _ZERO));
	m_ps->setScale(7.f);
	m_ps->setCameraMask((int)CameraFlag::USER1);
	schedule(schedule_selector(Stage::updatePs),1.f);
	addChild(m_ps, 0);


	m_ps1 = PUParticleSystem3D::create("slwh/ps3d/scripts/woosh.pu");
	m_ps1->setPosition3D(Vec3(_ZERO, 40, _ZERO));
	m_ps1->setScale(7.f);
	m_ps1->setCameraMask((int)CameraFlag::USER1);
	scheduleOnce(schedule_selector(Stage::updatePs1Once), 0.5f);
	addChild(m_ps1, 0);

	//����
	//StartPlay(8);

}

bool Stage::AttachSprite3D(std::string const &stage, std::string const &point, std::string const &sphere)
{
	int angle = _ZERO;
	float add = 360.f / STAGE_SPHERE_MAX;
	for (size_t i = _ZERO; i < STAGE_SPHERE_MAX; i++)
	{
		m_spr[i] = Sprite3D::create(sphere);
		if (nullptr == m_spr[i])
		{
			return false;
		}

		addChild(m_spr[i]);
		m_spr[i]->setScale(32);
		m_spr[i]->setPosition3D(Vec3(_ZERO, 50.f, _ZERO));
		m_spr[i]->setRotation3D(Vec3(_ZERO, angle, _ZERO));
		m_spr[i]->setCameraMask((int)CameraFlag::USER1);
		m_spr[i]->setTexture("slwh/Model/S.png");
		angle += add;
	}


	auto animation = Animation3D::create(sphere);
	if (animation)
	{
		m_open = Animate3D::create(animation, 0.f, 1.f);
		m_close = Animate3D::create(animation, 1.f, 2.f);
		m_open->retain();
		m_close->retain();
	}
	else
		return false;

	m_stage = Sprite3D::create(stage);
	if (nullptr != m_stage)
	{
		m_stage->setTexture("slwh/Model/Stage.png");
		m_stage->setScale(55.f);
		m_stage->setPosition3D(Vec3(_ZERO, 53.f, _ZERO));
		m_stage->setRotation3D(Vec3(_ZERO, _ZERO, 180));
		m_stage->setCameraMask((int)CameraFlag::USER1);
		addChild(m_stage);
	}
	else
		return false;

	m_point = Sprite3D::create(point);
	if (nullptr != m_point)
	{
		m_point->setTexture("slwh/Model/Point.png");
		m_point->setScale(30.f);
		m_point->setPosition3D(Vec3(-55, 53.f, _ZERO));
		m_point->setRotation3D(Vec3(_ZERO, _ZERO, 180));
		m_point->setCameraMask((int)CameraFlag::USER1);
		addChild(m_point);
	}
	else
		return false;
	

	//������Դ�� ��ʼ��ʼ����һ���߼�
	Start();
	return true;
}

void Stage::OpenSphere(bool b)
{
	if (nullptr == m_open || nullptr == m_close)
	{
		return;
	}

	if (b)
	{
		for (size_t i = _ZERO; i < STAGE_SPHERE_MAX; i++)
		{
			m_spr[i]->runAction(m_open->clone());
		}
	}
	else
	{
		for (size_t i = _ZERO; i < STAGE_SPHERE_MAX; i++)
		{
			m_spr[i]->runAction(m_close->clone());
		}
	}
}

void Stage::update(float delta)
{
	//4Ԫ����ֵ����������ʱ�洢������
	//Quaternion quat;
	if (m_sphere != nullptr)
	{
		m_sphere->ChangeColor(static_cast<int>((int)(GetRotate() + 7.5f) % 360 / 15.0f));

	}
	
	switch (m_ptState)
	{
	case ptNormal:
		//�����Ǵ��͵�
		break;
	case ptSpeedAdd:
		//����
		m_speed += SPEED_ADD * delta;

		if (m_runTime > (float)ptSpeedAdd)
		{
			m_ptState = ptSpeedMax;                //�л�������ٶ�����
		}
		break;
	case ptSpeedMax:
		if (m_runTime > (float)ptSpeedMax)
		{
			m_ptState = ptSpeedSub;                //�л�������
		}
		break;
	case ptSpeedSub:
		//����             
		m_speed -= SPEED_SUB * delta;
		if (m_runTime > (float)ptSpeedSub)
		{
			//ָ���л�Ϊ��С�ٶ�
			m_ptState = ptSpeedMin;
		}
		break;
	case ptSpeedMin:
		if (!m_bIsLerp)
		{
			if (true == DetalSpeed(getRotation3D().y))
			{
				m_bIsLerp = true;
				runAction(CCEaseExponentialOut::create(RotateTo::create(static_cast<int>(ptSpeedMin - m_runTime), Vec3(_ZERO, m_targetAngle, _ZERO))));
			}	
		}
		if (m_runTime > (float)ptSpeedMin)
		{
			//ָ�뾲ֹ
			m_ptState = ptNormal;
			//�ر�update
			unscheduleUpdate();
		}
		break;
	default:
		m_ptState = ptNormal;
		//�ر�update
		unscheduleUpdate();
		break;
	}

	if (!m_bIsLerp)
	{
		Rotate(Vec3::UNIT_Y, m_speed * delta);	
	}
	m_runTime += delta;
}

void Stage::AddSphere(Sphere* const &s)
{
	if (nullptr == s || m_sphere == s)
	{
		return;
	}
	m_sphere = s;
}

float Stage::GetRotate()
{
	return  getRotation3D().y;
}

bool Stage::DetalSpeed(float t)
{
	//��ǰ�Ƕ���Ŀ��ǶȲ��ʱ�洢������
	float angle = m_targetAngle - Angle0To360(getRotation3D().y);
	if (angle < _ZERO)
	{
		angle += 360;
	}
	
	if (angle <= 120 && angle >= 45)
	{
		return true;
	}
	return false;

}

void Stage::StartPlay(int index)
{

	CC_ASSERT(index >= _ZERO && index < Animal_Max);

	StartPlay((float)(index* _A));
}

void Stage::StartPlay(float angle)
{
	CC_ASSERT(angle >= _ZERO && angle <= 360);
	if (angle < _ZERO)  angle = _ZERO;
	if (angle > 360) angle = 360.f;

	//��ȷ���Ƹ�λ
	m_bIsLerp = false;

	//ָ�뿪ʼ����
	m_ptState = ptSpeedAdd;

	//�ٶȸ�λ
	m_speed = _ZERO;

	//ʱ�临λ
	m_runTime = _ZERO;

	//���ýǶ�
	m_targetAngle = angle;

	//����update
	scheduleUpdate();
}

void Stage::updatePs(float t)
{
	m_ps->startParticleSystem();
}

void Stage::updatePs1(float t)
{
	m_ps1->startParticleSystem();
}

void Stage::updatePs1Once(float t)
{
	schedule(schedule_selector(Stage::updatePs1), 1.f);
}

