#include "ReturnDialog.h"



#ifndef _ZERO
#define _ZERO 0
#endif

ReturnDialog* ReturnDialog::m_instance = nullptr;
ReturnDialog::ReturnDialog() :
rootNode(nullptr),
m_okbt(nullptr),
m_callback(nullptr),
text(nullptr),
m_cancelbt(nullptr)
{
}

ReturnDialog::~ReturnDialog()
{
	
}

static bool GetBOpen()
{

}

//唯一的调用接口,回掉是点击确认按钮后执行，传入的指针是this
static void openReturnNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr)
{

}


void ReturnDialog::openReturnNotice(const std::string &textLabel/* = ""*/, std::function<void(Ref*)> btnCallback /*= nullptr*/)
{	
	//已经有对话框打开
	if (m_instance) return;

	//创建通知框
	auto This = ReturnDialog::create();
	m_instance = This;
	if (This == nullptr) return;
	This->m_callback = btnCallback;


	This->setAnchorPoint(Vec2(0.5f, 0.5f));
	Vec2 v2 = Director::getInstance()->getWinSize();
	This->setPosition(v2 / 2);
	//当前活动场景
	auto runscene = Director::getInstance()->getRunningScene();

	auto m_visibleSize = Director::getInstance()->getWinSize();
	//摆放摄像机(3d动物层)
	auto ca = Camera::createPerspective(60, (GLfloat)m_visibleSize.width / m_visibleSize.height, 1, 1000);
	ca->setDepth(102);

	ca->setCameraFlag(CameraFlag::USER3);
	ca->setPosition3D(Vec3(0, 0, 0));
	ca->lookAt(Vec3(0, 0, -1));
	runscene->addChild(ca, 1000);


	//添加节点到活动场景
	This->rootNode = CSLoader::createNode(COCOS_STUDIO_RETURNNOTICE);
	CC_ASSERT(This->rootNode != nullptr);
	This->rootNode->setAnchorPoint(Vec2(0.5, 0.5));
	This->addChild(This->rootNode, 500);

	This->setCameraMask((int)CameraFlag::USER3);
	This->setPosition3D(Vec3(0, 0, -640));
	This->setRotation3D(Vec3(0, 0, 0));
	runscene->addChild(This);

	

	//得到text文本并设置文字
	This->text = dynamic_cast<Text*>(This->rootNode->getChildByName("noticeBg")->getChildByName("notice"));
	CC_ASSERT(This->text != nullptr);
	This->text->setString(textLabel);

	//得到OK按钮
	This->m_okbt = dynamic_cast<Button*>(This->rootNode->getChildByName("noticeBg")->getChildByName("okBtn"));
	CC_ASSERT(This->m_okbt != nullptr);
	

	//得到cancel按钮
	This->m_cancelbt = dynamic_cast<Button*>(This->rootNode->getChildByName("noticeBg")->getChildByName("cancelBtn"));
	CC_ASSERT(This->m_cancelbt!=nullptr);

	//触摸屏蔽
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ReturnDialog::onTouchBegan, This);
	listener->setSwallowTouches(true);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, This);


	//给按钮添加消息响应
	This->m_okbt->addClickEventListener([This](Ref* sender){
		//按钮不再可用
		This->m_okbt->setEnabled(false);
		//出场
		This->runAction(Sequence::create(
			//EaseBackIn::create(ScaleTo::create(0.3f,_ZERO)),
			//DelayTime::create(0.1),
			CCCallFuncN::create(This, callfuncN_selector(ReturnDialog::destory)),
			NULL
			));
		m_instance = nullptr;
		//回调
		if (This->m_callback != nullptr) This->m_callback(This);
	});

	This->m_cancelbt->addClickEventListener([This](Ref* sender){
		This->m_cancelbt->setEnabled(false);

		This->runAction(Sequence::create(
			//EaseBackIn::create(ScaleTo::create(0.3f,_ZERO)),
			//DelayTime::create(0.1),
			CCCallFuncN::create(This, callfuncN_selector(ReturnDialog::destory)),
			NULL
			));
		m_instance = nullptr;

	});

	//动画初始化
	This->setScale(_ZERO);
	//入场
	This->runAction(EaseBackOut::create(ScaleTo::create(0.3f, 1.f)));
}


void ReturnDialog::cancelDialog()
{
	//如果打开了框框
	if (m_instance)
	{
		m_instance->runAction(Sequence::create(
			//EaseBackIn::create(ScaleTo::create(0.3f,_ZERO)),
			//DelayTime::create(0.1),
			CCCallFuncN::create(m_instance, callfuncN_selector(ReturnDialog::destory)),
			NULL
			));
		m_instance = nullptr;
	}
}

bool ReturnDialog::GetBOpen()
{
	return m_instance != nullptr;
}

//销毁
void ReturnDialog::destory(Node* n)
{
	if (n)
	{
		n->removeFromParent();
	}
}

void ReturnDialog::onEnter()
{
	Node::onEnter();
}
void ReturnDialog::onExit()
{
	Node::onExit();
	m_instance = nullptr;
}