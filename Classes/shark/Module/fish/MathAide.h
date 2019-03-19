#ifndef _MathAide_H_
#define _MathAide_H_

#include "cocos2d.h"

typedef struct __MOVE_POINT
{
	cocos2d::Vec2 position;
	float direction;
}MOVE_POINT;

typedef std::vector<MOVE_POINT> MovePoints;

class MathAide
{
public:
	static void BuildCircle(float centerX, float centerY, float radius, cocos2d::Vec2* FishPos, int FishCount);
	//beginRadian ��ʼ���� beginAngle ��ʼ�Ƕ�
	static void BuildCircle(float centerX, float centerY, float radius, MovePoints &FishPos, int FishCount, float beginRadian = 0.f, float beginAngle = 0.f);
	static void BuildLinear(float initX[], float initY[], int initCount, MovePoints& TraceVector, float fDistance);
	//������ε���ײ���
	static bool pointHitTest(cocos2d::Vec2 posVal, cocos2d::Size collideSize, float angle, float warnRadius = 0, float deathRadius = 0);
	//Բ����ε���ײ���
	static bool circleHitTest(cocos2d::Vec2 centerVal, float r, cocos2d::Size collideSize, float angle, float warnRadius = 0, float deathRadius = 0);

	static bool rectHitTest(cocos2d::Vec2 posA, cocos2d::Size sizeA, float angleA, cocos2d::Vec2 posB, cocos2d::Size sizeB, float angleB);
};

#endif