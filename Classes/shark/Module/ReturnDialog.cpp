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

//Ψһ�ĵ��ýӿ�,�ص��ǵ��ȷ�ϰ�ť��ִ�У������ָ����this
static void openReturnNotice(const std::string &text = "", std::function<void(Ref*)> btnCallback = nullptr)
{

}


void ReturnDialog::openReturnNotice(const std::string &textLabel/* = ""*/, std::function<void(Ref*)> btnCallback /*= nullptr*/)
{	
	//�Ѿ��жԻ����
	if (m_instance) return;

	//����֪ͨ��
	auto This = ReturnDialog::create();
	m_instance = This;
	if (This == nullptr) return;
	This->m_callback = btnCallback;


	This->setAnchorPoint(Vec2(0.5f, 0.5f));
	Vec2 v2 = Director::getInstance()->getWinSize();
	This->setPosition(v2 / 2);
	//��ǰ�����
	auto runscene = Director::getInstance()->getRunningScene();

	auto m_visibleSize = Director::getInstance()->getWinSize();
	//�ڷ������(3d�����)
	auto ca = Camera::createPerspective(60, (GLfloat)m_visibleSize.width / m_visibleSize.height, 1, 1000);
	ca->setDepth(102);

	ca->setCameraFlag(CameraFlag::USER3);
	ca->setPosition3D(Vec3(0, 0, 0));
	ca->lookAt(Vec3(0, 0, -1));
	runscene->addChild(ca, 1000);


	//��ӽڵ㵽�����
	This->rootNode = CSLoader::createNode(COCOS_STUDIO_RETURNNOTICE);
	CC_ASSERT(This->rootNode != nullptr);
	This->rootNode->setAnchorPoint(Vec2(0.5, 0.5));
	This->addChild(This->rootNode, 500);

	This->setCameraMask((int)CameraFlag::USER3);
	This->setPosition3D(Vec3(0, 0, -640));
	This->setRotation3D(Vec3(0, 0, 0));
	runscene->addChild(This);

	

	//�õ�text�ı�����������
	This->text = dynamic_cast<Text*>(This->rootNode->getChildByName("noticeBg")->getChildByName("notice"));
	CC_ASSERT(This->text != nullptr);
	This->text->setString(textLabel);

	//�õ�OK��ť
	This->m_okbt = dynamic_cast<Button*>(This->rootNode->getChildByName("noticeBg")->getChildByName("okBtn"));
	CC_ASSERT(This->m_okbt != nullptr);
	

	//�õ�cancel��ť
	This->m_cancelbt = dynamic_cast<Button*>(This->rootNode->getChildByName("noticeBg")->getChildByName("cancelBtn"));
	CC_ASSERT(This->m_cancelbt!=nullptr);

	//��������
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ReturnDialog::onTouchBegan, This);
	listener->setSwallowTouches(true);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, This);


	//����ť�����Ϣ��Ӧ
	This->m_okbt->addClickEventListener([This](Ref* sender){
		//��ť���ٿ���
		This->m_okbt->setEnabled(false);
		//����
		This->runAction(Sequence::create(
			//EaseBackIn::create(ScaleTo::create(0.3f,_ZERO)),
			//DelayTime::create(0.1),
			CCCallFuncN::create(This, callfuncN_selector(ReturnDialog::destory)),
			NULL
			));
		m_instance = nullptr;
		//�ص�
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

	//������ʼ��
	This->setScale(_ZERO);
	//�볡
	This->runAction(EaseBackOut::create(ScaleTo::create(0.3f, 1.f)));
}


void ReturnDialog::cancelDialog()
{
	//������˿��
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

//����
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