#ifndef __UIBase_H__
#define __UIBase_H__

#include "cocos2d.h"
#include "slwh/ResourcePath.h"
USING_NS_CC;


/*
���е�UI�����������

Ϊ��
1.ͬһ�ṹ
2.��ӹ�������
.
.
.
������ UI���������� �Ĵ���
*/


class UIBase :public Node
{
protected:
	Node*							m_node;

public:
	UIBase();

	virtual ~UIBase();
	//����һ����Դ ���г�ʼ�� ���ش˺�������������Start()����
	virtual bool AttachNode(Node* const &spr);

	virtual void StartPlay(){};
	virtual void StopPlay(){};

	virtual void setVisible(const bool &b);
protected:

	//����AttachSprite3D�ͻ�����Start
	virtual void Start(){};
	void Move(Vec2 v2);
	void Move(Node* node, Vec2 v2);
};

#endif

