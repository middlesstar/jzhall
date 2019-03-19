#include "MessageTipNode.h"

MessageTipNode::MessageTipNode(void)
{

}

MessageTipNode::~MessageTipNode(void)
{

}

//����
MessageTipNode* MessageTipNode::createWithMessage(const char* cMessage)
{
	MessageTipNode* mPMessage = MessageTipNode::create();
	mPMessage->label_massage->setString(cMessage);
	mPMessage->adjustBg();
	mPMessage->addBeginAnim();	//���ö���

	return mPMessage;
}

bool MessageTipNode::init()
{
	Sprite::init();

	/* ���� */
	m_pLeftBg = Sprite::create("msgbg_edge.png");
	m_pMidBg = Sprite::create("msgbg_mid.png");
	m_pRightBg = Sprite::create("msgbg_edge.png");

	m_pRightBg->setScaleX(-1);
	m_pLeftBg->setAnchorPoint(Vec2(1.0, 0.5));
	m_pRightBg->setAnchorPoint(Vec2(1.0, 0.5));

	/* ����͸����͸���Ա����ʱ���� */
	setCascadeOpacityEnabled(true);

	this->addChild(m_pLeftBg);
	this->addChild(m_pMidBg);
	this->addChild(m_pRightBg);

	//��Ϣ
	this->label_massage = LabelTTF::create();
	label_massage->setFontSize(30);
	//int nColor = 0xffeabc;
	int nColor = 0xffffff;
	this->label_massage->setColor(ccc3((nColor >> 16) & 0xff, (nColor >> 8) & 0xff, nColor & 0xff));
	this->addChild(label_massage, 1);

	return true;
}

void MessageTipNode::addBeginAnim()
{
	this->setScale(0);
	//���Ŷ���
	this->runAction(Sequence::create(EaseSineIn::create(ScaleTo::create(0.2f, 1)),
		DelayTime::create(0.9f),
		FadeOut::create(0.6f),
		RemoveSelf::create(),
		NULL
		));
}

void MessageTipNode::adjustBg()
{
	Size size = label_massage->getContentSize();

	m_pMidBg->setScaleX(size.width + 30);		//���м�Ĳ��ֺ�������
	m_pLeftBg->setPositionX(-size.width / 2 - 15);
	m_pRightBg->setPositionX(size.width / 2 + 15);
}
