#ifndef __SharkAnimal_H__
#define __SharkAnimal_H__


#include "shark/Base/AnimalBase.h"
USING_NS_CC;

/*
每个动物的管理类
*/

class SharkAnimal : public AnimalBase
{  

private:
	cocos2d::Animate3D*				m_act;											//动画1


public:
	//播放动画接口
	void PlayAct(int posIndex, int animalIndex, std::string const &path);

	//停止动画接口
	void StopAct();

	SharkAnimal();
	~SharkAnimal();


};

#endif

