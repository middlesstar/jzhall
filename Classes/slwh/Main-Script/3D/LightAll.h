#ifndef __LightAll_H__
#define __LightAll_H__

#include "slwh/Base/ModelBase.h"


/*
场景大灯
*/


class LightAll : public ModelBase
{
private:
	std::map<int, std::string> m_lightTex;
	//std::string  m_lightTex[7];
	enumAnimalColor m_color;
	bool m_bIsRunning;
public:
	//打开场景大灯闪光
	void OpenLight(enumAnimalColor color);

	//关闭场景大灯闪光
	void CloseLight();

	LightAll();
	~LightAll();

protected:

	void Start();
private:
	void ChangeLight(int index);
	void Lightting(float t);

};

#endif

