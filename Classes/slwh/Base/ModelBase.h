#ifndef __ModelBase_H__
#define __ModelBase_H__

#include "cocos2d.h"
#include "slwh/ResourcePath.h"
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


class ModelBase :public Node
{
protected:
	cocos2d::Sprite3D* m_sprite3d;

public:
	ModelBase();
	virtual ~ModelBase();

	//关联一个资源 进行初始化 重载此函数请在最后调用Start()方法
	virtual bool AttachSprite3D(Sprite3D* const &spr);
	virtual void StartPlay(){};
	virtual void StopPlay(){};
protected:
	//朝forward方向（Vec3::）  旋转angle角度
	virtual void Rotate(const Vec3 &forward, const float &angle);
	virtual void Rotate(const Vec3 &eulerAngles);
	virtual void Rotate(const float &xAngle, const float &yAngle, const float &zAngle);


	//调用AttachSprite3D就会启动Start
	virtual void Start(){};

public:
	//将一个大的角度 转换为 0~360 范围内
	static float Angle0To360(float angle);

};

#endif

