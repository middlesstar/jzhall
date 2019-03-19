#include "ResOneAnimal.h"



//节点下的名称
#define _BEILV			"num"


std::map<int, std::string>* ResOneAnimal::m_map = nullptr;
ResOneAnimal::ResOneAnimal():
m_animal(NULL),
m_beilv(NULL)
{
}


ResOneAnimal::~ResOneAnimal()
{
	if (NULL != m_map)
	{
		delete m_map;
		m_map = NULL;
	}

}


void ResOneAnimal::Start()
{
	m_animal = dynamic_cast<ImageView*>(m_node);
	CC_ASSERT(NULL != m_animal);

	m_beilv = dynamic_cast<TextAtlas*>(m_node->getChildByName(_BEILV));
	CC_ASSERT(NULL != m_beilv);

	if (m_map == nullptr)
	{
		m_map = new std::map<int, std::string>();
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal0)] = Path_SMALL_Red_Animal0;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal1)] = Path_SMALL_Red_Animal1;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal2)] = Path_SMALL_Red_Animal2;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal3)] = Path_SMALL_Red_Animal3;

		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal0)] = Path_SMALL_Green_Animal0;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal1)] = Path_SMALL_Green_Animal1;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal2)] = Path_SMALL_Green_Animal2;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal3)] = Path_SMALL_Green_Animal3;

		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal0)] = Path_SMALL_Yellow_Animal0;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal1)] = Path_SMALL_Yellow_Animal1;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal2)] = Path_SMALL_Yellow_Animal2;
		(*m_map)[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal3)] = Path_SMALL_Yellow_Animal3;

		(*m_map)[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal0)] = Path_SMALL_ALL_Animal0;
		(*m_map)[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal1)] = Path_SMALL_ALL_Animal1;
		(*m_map)[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal2)] = Path_SMALL_ALL_Animal2;
		(*m_map)[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal3)] = Path_SMALL_ALL_Animal3;

		(*m_map)[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Red, enumAnimalType::AnimalMax)] = Path_SMALL_Red_AnimalAll;
		(*m_map)[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Green, enumAnimalType::AnimalMax)] = Path_SMALL_Green_AnimalAll;
		(*m_map)[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Yellow, enumAnimalType::AnimalMax)] = Path_SMALL_Yellow_AnimalAll;

		(*m_map)[_RES_KEY(enumSpecialRs::DMG, enumAnimalColor::ColorMax, enumAnimalType::AnimalMax)] = Path_SMALL_All_AnimalAll;

	}
	
}


void ResOneAnimal::SetAnimal(AnimalResult e)
{
	if (NULL == m_node)
	{
		return;
	}
	if (e.e1 == _4LF || e.e1 == CJ || e.e1 == XYCJ)
	{
		e.e1 = Normal;
	}
	else if (e.e1 == D3Y)
	{
		e.e2 = ColorMax;
	}
	else if (e.e1 == D4X)
	{
		e.e3 = AnimalMax;
	}
	else if (e.e1 == DMG)
	{
		e.e2 = ColorMax;
		e.e3 = AnimalMax;
	}

	m_animal->loadTexture((*m_map)[_RES_KEY(e.e1, e.e2, e.e3)], TextureResType::PLIST);
}


void ResOneAnimal::SetBeilv(int u)
{
	if (NULL == m_node)
	{
		return;
	}

	if (u == -1)
	{
		m_beilv->setVisible(false);
	}
	else
	{
		m_beilv->setVisible(true);
		std::stringstream str;
		str << u;
		m_beilv->setString(str.str());
	}
	
}


