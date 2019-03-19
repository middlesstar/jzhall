#ifndef __AnimalBase_H__
#define __AnimalBase_H__

#include "cocos2d.h"
#include "shark/resourceDef.h"
USING_NS_CC;


/*
所有的3DMODEL管理类的祖先

为了
1.同一结构
2.添加公共方法
.
.
.
便有了 3D资源管理基类 的存在
*/


class AnimalBase :public Node
{
protected:
	cocos2d::Sprite3D* m_sprite3d;

public:

	//关联一个资源 进行初始化 重载此函数请在最后调用Start()方法
	virtual bool AttachSprite3D(Sprite3D* const &spr);
	AnimalBase();
	virtual ~AnimalBase();

};

#endif

