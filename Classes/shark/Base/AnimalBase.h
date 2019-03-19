#ifndef __AnimalBase_H__
#define __AnimalBase_H__

#include "cocos2d.h"
#include "shark/resourceDef.h"
USING_NS_CC;


/*
���е�3DMODEL�����������

Ϊ��
1.ͬһ�ṹ
2.��ӹ�������
.
.
.
������ 3D��Դ������� �Ĵ���
*/


class AnimalBase :public Node
{
protected:
	cocos2d::Sprite3D* m_sprite3d;

public:

	//����һ����Դ ���г�ʼ�� ���ش˺�������������Start()����
	virtual bool AttachSprite3D(Sprite3D* const &spr);
	AnimalBase();
	virtual ~AnimalBase();

};

#endif

