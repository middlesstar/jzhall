#include "SharkNotice.h"



#ifndef _ZERO
#define _ZERO 0
#endif



SharkNotice::SharkNotice() :
m_rootNode(nullptr),
m_okbt(nullptr),
m_callback(nullptr),
m_text(nullptr)
{
}

SharkNotice::~SharkNotice()
{
}


void SharkNotice::openNotice(const std::string &text/* = ""*/, std::function<void(Ref*)> btnCallback /*= nullptr*/)
{

	static bool bOpen = false;

	//已经有对话框打开
	if (bOpen) return;

	//创建通知框
	auto This = SharkNotice::create();
	if (This == nullptr) return;
	This->m_callback = btnCallback;
	


	This->setAnchorPoint(Vec2(0.5f, 0.5f));
	Vec2 v2 = Director::getInstance()->getWinSize();
	This->setPosition(v2 / 2);
	////当前活动场景
	//auto runscene = Director::getInstance()->getRunningScene();
	//runscene->addChild(This);

	//当前活动场景
	auto runscene = Director::getInstance()->getRunningScene();

	auto m_visibleSize = Director::getInstance()->getWinSize();
	//摆放摄像机(3d动物层)
	auto ca = Camera::createPerspective(60, (GLfloat)m_visibleSize.width / m_visibleSize.height, 1, 1000);
	ca->setDepth(102);


	ca->setCameraFlag(CameraFlag::USER4);
	ca->setPosition3D(Vec3(0, 0, 0));
	ca->lookAt(Vec3(0, 0, -1));
	runscene->addChild(ca, 1000);


	//添加节点到活动场景
	This->m_rootNode = CSLoader::createNode(COCOS_STUDIO_SHARKNOTICE);
	CC_ASSERT(This->m_rootNode != nullptr);
	This->m_rootNode->setAnchorPoint(Vec2(0.5, 0.5));
	This->addChild(This->m_rootNode, 500);


	This->setCameraMask((int)CameraFlag::USER4);
	This->setPosition3D(Vec3(0, 0, -640));
	This->setRotation3D(Vec3(0, 0, 0));
	runscene->addChild(This);



	//得到text文本并设置文字
	This->m_text = dynamic_cast<Text*>(This->m_rootNode->getChildByName("text"));
	CC_ASSERT(This->m_text != nullptr);
	This->m_text->setString(text);

	//得到OK按钮
	This->m_okbt = dynamic_cast<Button*>(This->m_rootNode->getChildByName("ok"));
	CC_ASSERT(This->m_okbt != nullptr);

	//触摸屏蔽
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SharkNotice::onTouchBegan, This);
	listener->setSwallowTouches(true);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, This);

	//给按钮添加消息响应
	This->m_okbt->addClickEventListener([This](Ref* sender){
		//按钮不再可用
		This->m_okbt->setEnabled(false);
		//出场
		This->runAction(Sequence::create(
			//EaseBackIn::create(ScaleTo::create(0.3f,_ZERO)),
			DelayTime::create(0.1),
			CCCallFuncN::create(This, callfuncN_selector(SharkNotice::destory)),
			NULL
			));

		bOpen = false;
		//回调
		if (This->m_callback != nullptr) This->m_callback(This);
	});

	//动画初始化
	This->setScale(_ZERO);
	//入场
	This->runAction(EaseBackOut::create(ScaleTo::create(0.3f, 1.f)));

	bOpen = true;
}

//销毁
void SharkNotice::destory(Node* n)
{
	if (n)
	{
		n->removeFromParent();
	}
}