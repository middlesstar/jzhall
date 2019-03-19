#ifndef __ResAnimal_H__
#define __ResAnimal_H__

#include "slwh/Base/UIBase.h"
#include "ResOneAnimal.h"

/*
动物结果管理类
*/


class ResAnimal :
	public UIBase
{
protected:

	ResOneAnimal			m_normal;
	ResOneAnimal			m_4LF[4];

	std::map<int, std::string> m_map;
public:
public:
	ResAnimal();
	~ResAnimal();

	void SetAnimal(AnimalResult e, int beilv);
	void SetAnimal(AnimalResult* e, int* beilv);
protected:
	void Start();

};

#endif

