#ifndef  _AnimalPlay_H_
#define  _AnimalPlay_H_

#include "cocos2d.h"

#include "shark/resourceDef.h"
#include "shark/Base/AnimalBase.h"
#include "Shark/Module/SharkAnimal.h"
USING_NS_CC;


class  AnimalPlay : public AnimalBase
{
public:
	AnimalPlay();
	virtual ~AnimalPlay();


public:
	bool AttachSprite3D(
		std::string const &Ani0Path,
		std::string const &Ani1Path,
		std::string const &Ani2Path,
		std::string const &Ani3Path,
		std::string const &Ani4Path,
		std::string const &Ani5Path,
		std::string const &Ani6Path,
		std::string const &Ani7Path,
		std::string const &Ani8Path);

	//开始动物跳舞
	void startAnimal(int posIndex, int animalIndex, std::string const &path);

	//停止动物跳舞
	void stopAnimal(int index);

	//移除所有的动物
	void removeAnimal();

	//动物
	SharkAnimal* m_animal[10];

};

#endif // _AnimalPlay_H_

