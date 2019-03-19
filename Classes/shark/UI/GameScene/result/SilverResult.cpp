#include "SilverResult.h"



#define   SILVERSHARK     "winSilverShark"
#define   YELLOWLIGHT     "yellow_light"
#define   LUCKTIME        "luckytime"

SilverResult::SilverResult()
:pMainNode(nullptr)
,light(nullptr)
, luckLight(nullptr)
{

}
SilverResult::~SilverResult()
{
	
}

void SilverResult::setContent()
{
	pMainNode = dynamic_cast<Node*>(_seekNodeByName(m_pNode,SILVERSHARK));
	CC_ASSERT(pMainNode != nullptr);
	pMainNode->setVisible(false);

	light = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, YELLOWLIGHT));
	CC_ASSERT(light != nullptr);
	light->setVisible(false);

	luckLight = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, LUCKTIME));
	CC_ASSERT(luckLight != nullptr);
	luckLight->setVisible(false);
}

//������˸
void SilverResult::startPlay()
{
	pMainNode->setVisible(true);
	light->setVisible(true);

	//����ʱ�̵�����Ч
	//luckTime();

	auto in = FadeOut::create(0.5f);
	auto seq = Sequence::create(in, in->reverse(), NULL);
	light->runAction(RepeatForever::create(seq));
}
//�ر���˸
void SilverResult::stopPlay()
{
	pMainNode->setVisible(false);
	light->stopAllActions();
	light->setVisible(false);
}

//����ʱ�̵�����Ч
void SilverResult::luckTime()
{	
	auto particle = ParticleSystemQuad::create(PATH_BLAST);
	particle->setPosition(Vec2(151, 42));
	particle->setAutoRemoveOnFinish(true);                          // ��ɺ��ƶ��Ƴ�  
	particle->setDuration(8.0f);									// ��������ϵͳ�ĳ���ʱ����  
	luckLight->addChild(particle);
}