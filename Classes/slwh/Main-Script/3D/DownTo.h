#ifndef __DownTo_H__
#define __DownTo_H__

#include "slwh/Base/ModelBase.h"

/*
指针指向的板子
*/

class DownTo :
	public ModelBase
{
private:
	int  m_index;
	bool m_bIsRunning;
	//存储24个板子的颜色
	enumAnimalColor m_color[Animal_Max];
	//存储24个板子
	Sprite3D* m_downTo[Animal_Max];

public:
	bool LoadModel(std::string const &path);
	//通过路径加载子资源
	bool AttachSprite3D(std::string const &path);
	void ChangeColor(enumAnimalColor* const &arr);
	void ChangeColor(int* const &arr);
	DownTo();
	~DownTo();

protected:
	//改变板子颜色动画过程函数
	void ChgeColorRun(float t);
	void Start();

	//void update(float t);
};

#endif

