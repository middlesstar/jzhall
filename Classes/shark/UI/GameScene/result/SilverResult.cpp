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

//开启闪烁
void SilverResult::startPlay()
{
	pMainNode->setVisible(true);
	light->setVisible(true);

	//幸运时刻到来特效
	//luckTime();

	auto in = FadeOut::create(0.5f);
	auto seq = Sequence::create(in, in->reverse(), NULL);
	light->runAction(RepeatForever::create(seq));
}
//关闭闪烁
void SilverResult::stopPlay()
{
	pMainNode->setVisible(false);
	light->stopAllActions();
	light->setVisible(false);
}

//幸运时刻到来特效
void SilverResult::luckTime()
{	
	auto particle = ParticleSystemQuad::create(PATH_BLAST);
	particle->setPosition(Vec2(151, 42));
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(8.0f);									// 设置粒子系统的持续时间秒  
	luckLight->addChild(particle);
}