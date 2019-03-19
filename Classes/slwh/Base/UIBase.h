#ifndef __UIBase_H__
#define __UIBase_H__

#include "cocos2d.h"
#include "slwh/ResourcePath.h"
USING_NS_CC;


/*
所有的UI管理类的祖先

为了
1.同一结构
2.添加公共方法
.
.
.
便有了 UI界面管理基类 的存在
*/


class UIBase :public Node
{
protected:
	Node*							m_node;

public:
	UIBase();

	virtual ~UIBase();
	//关联一个资源 进行初始化 重载此函数请在最后调用Start()方法
	virtual bool AttachNode(Node* const &spr);

	virtual void StartPlay(){};
	virtual void StopPlay(){};

	virtual void setVisible(const bool &b);
protected:

	//调用AttachSprite3D就会启动Start
	virtual void Start(){};
	void Move(Vec2 v2);
	void Move(Node* node, Vec2 v2);
};

#endif

