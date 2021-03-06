#include "SharkShow.h"

SharkShow::SharkShow()
:mainNode(nullptr)
, circle1(nullptr)
, circle2(nullptr)
, circle3(nullptr)
{

}
SharkShow::~SharkShow()
{
	
}
void SharkShow::setContent()
{
	mainNode = dynamic_cast<Node*>(_seekNodeByName(m_pNode, "sharkRotate"));
	CC_ASSERT(mainNode != nullptr);
	mainNode->setVisible(false);

	circle1 = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, "circle1"));
	CC_ASSERT(circle1 != nullptr);

	circle2 = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, "circle2"));
	CC_ASSERT(circle2 != nullptr);


	circle3 = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, "circle3"));
	CC_ASSERT(circle3 != nullptr);


}


void SharkShow::runRotate(Node* node)
{
	auto sp = (Sprite*)node;

	auto rota = RotateBy::create(1.0f, 360);
	auto rep = RepeatForever::create(rota);
	sp->runAction(rep);
}
//������˸
void SharkShow::startPlay()
{
	mainNode->setVisible(true);
	
	this->runRotate(circle1);
	this->runRotate(circle2);
	this->runRotate(circle3);

	_PLAY_MUSCI_SHARK(SHARKTIME_MUSIC, true);
}
//�ر���˸
void SharkShow::stopPlay()
{
	circle1->stopAllActions();
	circle2->stopAllActions();
	circle3->stopAllActions();
	mainNode->setVisible(false);
}

