#ifndef _GameEntiy_H_
#define _GameEntiy_H_

#include "cocos2d.h"

class GameEntity
{
public:
	GameEntity();
	virtual ~GameEntity();
	//����ʵ��
	virtual	void OnUpdate(float dt){};

	//����ʵ��
	void    AddSpeed(int rate){ m_fSpeed *= rate; }
private:
	//ʵ���ʶID
	CC_SYNTHESIZE(int, m_wID, ID);
	//ʵ�����ͱ�ʶID
	CC_SYNTHESIZE(int, m_wTypeID, TypeID);
	//ʵ�崴��ʱ��
	CC_SYNTHESIZE(int, m_wCreateTime, CreateTime);
	//ʵ���С
	CC_SYNTHESIZE(cocos2d::Size, m_Size, Size);
	//ʵ��λ��
	CC_SYNTHESIZE(cocos2d::Vec2, m_Position, Position);
	//ʵ�巽��
	CC_SYNTHESIZE(float, m_fDirection, Direction);
	//��ȡʵ���ٶ�
	CC_SYNTHESIZE(float, m_fSpeed, Speed);
	//��ͣʵ�嶯��
	CC_SYNTHESIZE(bool, m_bPause, Pause);
	//ʵ������
	CC_SYNTHESIZE(int, m_wData, Data);

};

#endif