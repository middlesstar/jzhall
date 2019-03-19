#ifndef __ResOneAnimal_H__
#define __ResOneAnimal_H__

#include "slwh/Base/UIBase.h"
class ResOneAnimal :
	public UIBase
{
protected:
	ImageView*			m_animal;
	TextAtlas*			m_beilv;

	static std::map<int, std::string>* m_map;
public:
	ResOneAnimal();
	~ResOneAnimal();

	void SetAnimal(AnimalResult e);
	void SetBeilv(int u);

protected:
	void Start();
};

#endif

