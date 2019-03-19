#include "Model3DLayer.h"


/*
	由于非专业3D游戏美工 模型按需调整了大小
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
	//加载模型
	//临时变量
	cocos2d::Sprite3D* x;
	//加载地面背景
	x = Sprite3D::create(Path_Terrian);
	CC_ASSERT(x != nullptr);
	x->setScale(25.f);
	x->setRotation3D(Vec3(-90, _ZERO, _ZERO));
	x->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
	x->setCameraMask((int)CameraFlag::USER1);
	//x->setTexture(Path_Terrian_DDS);
	//m_terrain.AttachSprite3D(x);
	this->addChild(x);



	//加载场景大灯
	x = Sprite3D::create(Path_LightAll);
	CC_ASSERT(x != nullptr);
	x->setScale(25.f);
	x->setRotation3D(Vec3(180, _ZERO, _ZERO));
	x->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
	x->setCameraMask((int)CameraFlag::USER1);
	m_lightAll.AttachSprite3D(x);
	this->addChild(&m_lightAll);



	//呼吸灯
	x = Sprite3D::create(Path_Line);
	CC_ASSERT(x != nullptr);
	x->setScale(25.f);
	x->setRotation3D(Vec3(180, _ZERO, _ZERO));
	x->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
	x->setCameraMask((int)CameraFlag::USER1);
	//x->runAction(RotateBy::create(5.f,Vec3(180,0,0)));
	m_line.AttachSprite3D(x);
	this->addChild(&m_line);


	//转盘
	if (m_stage.AttachSprite3D(Path_Stage, Path_Point, Path_S))
	{
		this->addChild(&m_stage);
	}

	//指针中的指示灯
	x = Sprite3D::create(Path_Sphere);
	CC_ASSERT(x != nullptr);
	x->setScale(22.f);
	x->setRotation3D(Vec3(180, _ZERO, _ZERO));
	x->setPosition3D(Vec3(_ZERO, 8, _ZERO));
	//x->setTexture(Path_Sphere_DDS);
	x->setCameraMask((int)CameraFlag::USER1);
	m_sphere.AttachSprite3D(x);
	//添加到转盘节点 达到运动同步效果
	m_stage.AddSphere(&m_sphere);
	m_stage.addChild(&m_sphere);


	//关联资源路径 由管理类创建集合
	if (m_DownTo.AttachSprite3D(Path_DownTo))
	{
		this->addChild(&m_DownTo);
	}

	//关联动物资源路径 由管理类创建集合
	if (m_animal.AttachSprite3D(Path_Rabbit, Path_Monkey, Path_Panda, Path_Lion, Path_Step))
	{
		this->addChild(&m_animal);
	}

	//摆放摄像机
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
	//	log("%s",a_u8("创建方向光失败"));
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
	//	CCLOG("创建光失败");
	//}

	return true;
}

