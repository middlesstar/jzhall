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

//Æô¶¯ÉÁË¸
void AnimalBgChange::startPlay()
{
	animalBg->setVisible(true);
}
//¹Ø±ÕÉÁË¸
void AnimalBgChange::stopPlay()
{
	animalBg->setVisible(false);
}


