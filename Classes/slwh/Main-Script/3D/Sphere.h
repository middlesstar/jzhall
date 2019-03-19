#ifndef __Sphere_H__
#define __Sphere_H__



#include "slwh/Base/ModelBase.h"
class Sphere :
	public ModelBase
{
private:
	bool					m_bIsRunning;
	enumAnimalColor			m_color;
	int					    m_colorIndex[Animal_Max];


public:
	Sphere();
	~Sphere();

	void SetColorIndex(int* const &x);				//24������  ���鳤��Ϊ24
	void ChangeColor(enumAnimalColor color);
	void ChangeColor(int index);
	void OpenLight(enumAnimalColor color);
	enumAnimalColor OpenLight();
	void CloseLight();

protected:
	void Lightting(float t);
	void Start();

};

#endif