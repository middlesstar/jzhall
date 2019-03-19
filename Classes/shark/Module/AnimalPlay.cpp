#include "AnimalPlay.h"


AnimalPlay::AnimalPlay() 
{

}
AnimalPlay::~AnimalPlay()
{
	//this->removeAllChildrenWithCleanup(true);
}


bool AnimalPlay::AttachSprite3D
(std::string const &Ani0Path,
std::string const &Ani1Path,
std::string const &Ani2Path,
std::string const &Ani3Path,
std::string const &Ani4Path,
std::string const &Ani5Path,
std::string const &Ani6Path,
std::string const &Ani7Path,
std::string const &Ani8Path)
{
	Sprite3D* spr = nullptr;	
	for (size_t i = _ZERO; i < Animal_MAx; i++)
	{
		m_animal[i] = new SharkAnimal();

		switch (i)
		{
		case ANIMAL_GOLD -1:
			spr = Sprite3D::create(Ani0Path);
			spr->setTexture(IMAGE_COLD);
			if (nullptr == spr)
				return false;
			break;
		case ANIMAL_SILVER-1:
			spr = Sprite3D::create(Ani0Path);
			spr->setTexture(IMAGE_SILVER);
			if (nullptr == spr)
				return false;
			break;
		case ANIMAL_SWALLOW-1:
			spr = Sprite3D::create(Ani1Path);
			if (nullptr == spr)
				return false;
			break;
		case ANIMAL_DOVE-1:
			spr = Sprite3D::create(Ani2Path);
			if (nullptr == spr)
				return false;
			break;
		case ANIMAL_PEACOCK-1:
			spr = Sprite3D::create(Ani3Path);
			if (nullptr == spr)
				return false;
			break;
		case ANIMAL_HAWK-1:
			spr = Sprite3D::create(Ani4Path);
			if (nullptr == spr)
				return false;
			break;
		case ANIMAL_LION-1:
			spr = Sprite3D::create(Ani5Path);
			if (nullptr == spr)
				return false;			
			break;
		case ANIMAL_PANDA-1:
			spr = Sprite3D::create(Ani6Path);
			if (nullptr == spr)
				return false;
			break;
		case ANIMAL_MONKEY-1:
			spr = Sprite3D::create(Ani7Path);
			if (nullptr == spr)
				return false;
			break;
		case ANIMAL_RABBIT-1:
			spr = Sprite3D::create(Ani8Path);
			if (nullptr == spr)
				return false;
			break;
		default:
			break;
		}
		//m_animal[i]->setGlobalZOrder(100);
		m_animal[i]->AttachSprite3D(spr);
		m_animal[i]->setRotation3D(Vec3(_ZERO, _ZERO, _ZERO));
		m_animal[i]->setPosition3D(Vec3(_ZERO, _ZERO, _ZERO));
		m_animal[i]->setCameraMask((int)CameraFlag::USER1);

		addChild(m_animal[i]);
		m_animal[i]->setVisible(false); 
	}

	return true;
}



//开始动物跳舞
void AnimalPlay::startAnimal(int posIndex,int animalIndex, std::string const &path)
{
	//需要让animalIndex-1
	m_animal[animalIndex-1]->PlayAct(posIndex,animalIndex,path);
	//m_animal[ANIMAL_PANDA- 1]->PlayAct(1, ANIMAL_PANDA, PATH_PANDA);
}

//停止动物跳舞
void AnimalPlay::stopAnimal(int index)
{
	//需要让animalIndex-1
	m_animal[index-1]->StopAct();

}

//移除所有的动物
//void AnimalPlay::removeAnimal()
//{
//	for (size_t i = _ZERO; i < Animal_MAx; i++)
//	{
//		m_animal[i]->removeAni();
//	}
//}