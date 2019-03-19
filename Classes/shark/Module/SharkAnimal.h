#ifndef __SharkAnimal_H__
#define __SharkAnimal_H__


#include "shark/Base/AnimalBase.h"
USING_NS_CC;

/*
ÿ������Ĺ�����
*/

class SharkAnimal : public AnimalBase
{  

private:
	cocos2d::Animate3D*				m_act;											//����1


public:
	//���Ŷ����ӿ�
	void PlayAct(int posIndex, int animalIndex, std::string const &path);

	//ֹͣ�����ӿ�
	void StopAct();

	SharkAnimal();
	~SharkAnimal();


};

#endif

