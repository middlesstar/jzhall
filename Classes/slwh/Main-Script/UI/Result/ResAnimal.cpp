#include "ResAnimal.h"

//节点下逻辑标签
#define NORMAL_ANINAL		"animal"
#define _4LF_ANIMAL_0		"animal_0"
#define _4LF_ANIMAL_1		"animal_1"
#define _4LF_ANIMAL_2		"animal_2"
#define _4LF_ANIMAL_3		"animal_3"

ResAnimal::ResAnimal()
{
}

ResAnimal::~ResAnimal()
{
}

void ResAnimal::Start()
{

	m_normal.AttachNode(m_node->getChildByName(NORMAL_ANINAL));

	m_4LF[0].AttachNode(m_node->getChildByName(_4LF_ANIMAL_0));
	m_4LF[1].AttachNode(m_node->getChildByName(_4LF_ANIMAL_1));
	m_4LF[2].AttachNode(m_node->getChildByName(_4LF_ANIMAL_2));
	m_4LF[3].AttachNode(m_node->getChildByName(_4LF_ANIMAL_3));

}

void ResAnimal::SetAnimal(AnimalResult e, int beilv)
{	

	for (size_t i = _ZERO; i < 4; i++)
	{
		m_4LF[i].setVisible(false);
	}
	m_normal.setVisible(true);
	m_normal.SetAnimal(e);
	m_normal.SetBeilv(beilv);

}

void ResAnimal::SetAnimal(AnimalResult* e, int* beilv)
{

	m_normal.setVisible(false);
	for (size_t i = _ZERO; i < 4; i++)
	{
		m_4LF[i].setVisible(true);
		m_4LF[i].SetAnimal(e[i]);
		m_4LF[i].SetBeilv(beilv[i]);
	}
}

