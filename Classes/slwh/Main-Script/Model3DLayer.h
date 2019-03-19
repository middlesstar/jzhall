#ifndef __Model3DLayer_H__
#define __Model3DLayer_H__

#include "cocos2d.h"

#include "slwh/ResourcePath.h"

#include "3D/Animal.h"
#include "3D/Line.h"
#include "3D/LightAll.h"
#include "3D/Stage.h"
#include "3D/Sphere.h"
#include "3D/DownTo.h"
#include "3D/AnimalManager.h"
#include "3D/MyCamera.h"

USING_NS_CC;

#ifndef _ZERO
#define _ZERO 0
#endif // !_ZERO


/*	3D 游戏场景层 创建每个3D物体的管理对象
*/

class Model3DLayer : public cocos2d::Layer
{
private:
	Size				m_visibleSize;

public:
	Line				m_line;
	LightAll			m_lightAll;
	Stage				m_stage;
	Sphere				m_sphere;
	DownTo				m_DownTo;
	AnimalManager		m_animal;
	MyCamera			m_camera3D;

protected:
	//void CreateObj();

public:
	Model3DLayer();
	~Model3DLayer();


	virtual bool init();
	//void update(float delta);
	//void StartGames();
	CREATE_FUNC(Model3DLayer);

};

#endif

