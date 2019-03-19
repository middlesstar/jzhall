#include "Model3DLayer.h"


/*
	���ڷ�רҵ3D��Ϸ���� ģ�Ͱ�������˴�С
	*/

Model3DLayer::Model3DLayer()
{
}

Model3DLayer::~Model3DLayer()
{
}

bool Model3DLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	FileUtils::getInstance()->addSearchPath("slwh/ps3d/materials");

	m_visibleSize = Director::getInstance()->getWinSize();
	log("----------dais: Model3DLayer::init-------------");
	//����ģ��
	//��ʱ����
	cocos2d::Sprite3D* x;
	//���ص��汳��
	x = Sprite3D::create(Path_Terrian);
	CC_ASSERT(x != nullptr);
	x->setScale(25.f);
	x->setRotation3D(Vec3(-90, _ZERO, _ZERO));
	x->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
	x->setCameraMask((int)CameraFlag::USER1);
	//x->setTexture(Path_Terrian_DDS);
	//m_terrain.AttachSprite3D(x);
	this->addChild(x);



	//���س������
	x = Sprite3D::create(Path_LightAll);
	CC_ASSERT(x != nullptr);
	x->setScale(25.f);
	x->setRotation3D(Vec3(180, _ZERO, _ZERO));
	x->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
	x->setCameraMask((int)CameraFlag::USER1);
	m_lightAll.AttachSprite3D(x);
	this->addChild(&m_lightAll);



	//������
	x = Sprite3D::create(Path_Line);
	CC_ASSERT(x != nullptr);
	x->setScale(25.f);
	x->setRotation3D(Vec3(180, _ZERO, _ZERO));
	x->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
	x->setCameraMask((int)CameraFlag::USER1);
	//x->runAction(RotateBy::create(5.f,Vec3(180,0,0)));
	m_line.AttachSprite3D(x);
	this->addChild(&m_line);


	//ת��
	if (m_stage.AttachSprite3D(Path_Stage, Path_Point, Path_S))
	{
		this->addChild(&m_stage);
	}

	//ָ���е�ָʾ��
	x = Sprite3D::create(Path_Sphere);
	CC_ASSERT(x != nullptr);
	x->setScale(22.f);
	x->setRotation3D(Vec3(180, _ZERO, _ZERO));
	x->setPosition3D(Vec3(_ZERO, 8, _ZERO));
	//x->setTexture(Path_Sphere_DDS);
	x->setCameraMask((int)CameraFlag::USER1);
	m_sphere.AttachSprite3D(x);
	//��ӵ�ת�̽ڵ� �ﵽ�˶�ͬ��Ч��
	m_stage.AddSphere(&m_sphere);
	m_stage.addChild(&m_sphere);


	//������Դ·�� �ɹ����ഴ������
	if (m_DownTo.AttachSprite3D(Path_DownTo))
	{
		this->addChild(&m_DownTo);
	}

	//����������Դ·�� �ɹ����ഴ������
	if (m_animal.AttachSprite3D(Path_Rabbit, Path_Monkey, Path_Panda, Path_Lion, Path_Step))
	{
		this->addChild(&m_animal);
	}

	//�ڷ������
	auto ca = Camera::createPerspective(60, (GLfloat)m_visibleSize.width / m_visibleSize.height, 1, 1000);
	CC_ASSERT(ca != nullptr);
	m_camera3D.AttachCamera(ca);
	addChild(&m_camera3D);


	log("----------dais: Model3DLayer::init over-------------");
	//auto dirLight = DirectionLight::create(Vec3(_ZERO, -1, -0.2), Color3B(255, 255, 255));
	//if (nullptr != dirLight)
	//{
	//	dirLight->setPosition3D(Vec3(_ZERO, 500, _ZERO));
	//	dirLight->setIntensity(1.8f);
	//	addChild(dirLight);
	//	//dirLight->setCameraMask((int)CameraFlag::USER1);
	//}
	//else
	//{
	//	log("%s",a_u8("���������ʧ��"));
	//}

	//auto _spotLight = SpotLight::create(Vec3(_ZERO, -1.0f, _ZERO), Vec3(0.0f, 200.0f, 0.0f), Color3B(200, 200, 200), 0.0, 1.5, 200.0f);
	////_spotLight->retain();
	//_spotLight->setIntensity(3.0f);
	//_spotLight->setEnabled(true);
	//addChild(_spotLight);
	//_spotLight->setCameraMask((int)CameraFlag::USER1);

	//auto Light = PointLight::create(Vec3(0,150, 0), Color3B(255, 255, 255),500);
	//if (nullptr != Light)
	//{
	//	Light->setintensity(0.5f);
	//	addChild(Light);
	//	//dirLight->setCameraMask((int)CameraFlag::USER1);
	//}
	//else
	//{
	//	CCLOG("������ʧ��");
	//}

	return true;
}

