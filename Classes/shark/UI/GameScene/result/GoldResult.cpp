#include "goldResult.h"

#define      GOLDSHARK      "winGoldShark"
#define      YELLOELIGHT    "yellow_light"
#define      BLAST          "blast"
GoldResult::GoldResult()
:pMainNode(nullptr)
,light(nullptr)
, blastLight(nullptr)
{

}

GoldResult:: ~GoldResult()
{
	
}

void GoldResult::setContent()
{
	pMainNode = dynamic_cast<Node*>(_seekNodeByName(m_pNode, GOLDSHARK));
	CC_ASSERT(pMainNode != nullptr);
	pMainNode->setVisible(false);

	light = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, YELLOELIGHT));
	CC_ASSERT(light != nullptr);
	light->setVisible(false);

	blastLight = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, BLAST));
	CC_ASSERT(blastLight != nullptr);
	blastLight->setVisible(false);
	
}

void GoldResult::startPlay()
{
	pMainNode->setVisible(true);
	light->setVisible(true);

	//������Ч
	//boastLight();
	auto in = FadeOut::create(0.5f);
	auto seq = Sequence::create(in, in->reverse(), NULL);
	light->runAction(RepeatForever::create(seq));
}
void GoldResult::stopPlay()
{
	pMainNode->setVisible(false);
	light->stopAllActions();
	light->setVisible(false);
}

//��ʾ��������Ч
void GoldResult::boastLight()
{
	auto particle = ParticleSystemQuad::create(PATH_BLAST);
	particle->setPosition(Vec2(117, 44));
	particle->setAutoRemoveOnFinish(true);                          // ��ɺ��ƶ��Ƴ�  
	particle->setDuration(8.0f);									// ��������ϵͳ�ĳ���ʱ����  
	blastLight->addChild(particle);
}
