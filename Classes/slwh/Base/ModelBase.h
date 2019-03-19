#ifndef __ModelBase_H__
#define __ModelBase_H__

#include "cocos2d.h"
#include "slwh/ResourcePath.h"
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


class ModelBase :public Node
{
protected:
	cocos2d::Sprite3D* m_sprite3d;

public:
	ModelBase();
	virtual ~ModelBase();

	//����һ����Դ ���г�ʼ�� ���ش˺�������������Start()����
	virtual bool AttachSprite3D(Sprite3D* const &spr);
	virtual void StartPlay(){};
	virtual void StopPlay(){};
protected:
	//��forward����Vec3::��  ��תangle�Ƕ�
	virtual void Rotate(const Vec3 &forward, const float &angle);
	virtual void Rotate(const Vec3 &eulerAngles);
	virtual void Rotate(const float &xAngle, const float &yAngle, const float &zAngle);


	//����AttachSprite3D�ͻ�����Start
	virtual void Start(){};

public:
	//��һ����ĽǶ� ת��Ϊ 0~360 ��Χ��
	static float Angle0To360(float angle);

};

#endif

