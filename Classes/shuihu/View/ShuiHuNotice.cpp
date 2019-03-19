#include "ShuiHuNotice.h"



#ifndef _ZERO
#define _ZERO 0
#endif



ShuiHuNotice::ShuiHuNotice() :
m_rootNode(nullptr),
m_okbt(nullptr),
m_callback(nullptr),
m_text(nullptr)
{
}

ShuiHuNotice::~ShuiHuNotice()
{
}


void ShuiHuNotice::openNotice(const std::string &text/* = ""*/, std::function<void(Ref*)> btnCallback /*= nullptr*/)
{

	static bool bOpen = false;

	//�Ѿ��жԻ����
	if (bOpen) return;

	//����֪ͨ��
	auto This = ShuiHuNotice::create();
	if (This == nullptr) return;
	This->m_callback = btnCallback;
	This->setAnchorPoint(Vec2(0.5f, 0.5f));
	Vec2 v2 = Director::getInstance()->getWinSize();
	This->setPosition(v2 / 2);

	//��ǰ�����
	auto runscene = Director::getInstance()->getRunningScene();
	runscene->addChild(This);

	//��ӽڵ㵽�����
	This->m_rootNode = CSLoader::createNode(COCOS_STUDIO_SHARKNOTICE);
	CC_ASSERT(This->m_rootNode != nullptr);
	This->m_rootNode->setAnchorPoint(Vec2(0.5, 0.5));
	This->addChild(This->m_rootNode, 500);

	//�õ�text�ı�����������
	This->m_text = dynamic_cast<Text*>(This->m_rootNode->getChildByName("text"));
	CC_ASSERT(This->m_text != nullptr);
	This->m_text->setString(text);

	//�õ�OK��ť
	This->m_okbt = dynamic_cast<Button*>(This->m_rootNode->getChildByName("ok"));
	CC_ASSERT(This->m_okbt != nullptr);

	//��������
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ShuiHuNotice::onTouchBegan, This);
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
			CCCallFuncN::create(This, callfuncN_selector(ShuiHuNotice::destory)),
			NULL
			));

		bOpen = false;
		//�ص�
		if (This->m_callback != nullptr) This->m_callback(This);
	});

	//������ʼ��
	This->setScale(_ZERO);
	//�볡

	This->runAction(EaseBackOut::create(ScaleTo::create(0.3f, 1.f)));
	bOpen = true;
}

//����
void ShuiHuNotice::destory(Node* n)
{
	if (n)
	{
		n->removeFromParent();
	}
}