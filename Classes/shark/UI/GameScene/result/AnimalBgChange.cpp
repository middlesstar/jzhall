#include "AnimalBgChange.h"





AnimalBgChange::AnimalBgChange() :animalBg(nullptr)

{

}
AnimalBgChange::~AnimalBgChange()
{

}
void AnimalBgChange::setContent()
{
	animalBg = dynamic_cast<ImageView*>(_seekNodeByName(m_pNode, "animalBg"));
	CC_ASSERT(animalBg!=nullptr);
	animalBg->setVisible(false);
}

//������˸
void AnimalBgChange::startPlay()
{
	animalBg->setVisible(true);
}
//�ر���˸
void AnimalBgChange::stopPlay()
{
	animalBg->setVisible(false);
}


