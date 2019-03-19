#ifndef __GlobalTimer_H__
#define __GlobalTimer_H__

#include "cocos2d.h"
USING_NS_CC;


class GlobalTimer : public Node
{
public:
	// ����ȫ�ֶ�ʱ��(ÿһ֡��ִ��)
	void StarPlay();
	// ֹͣ
	void StopPlay();
	// ��ͣ
	void Pause();
	// �ָ�  
	void Resume();

protected:
	GlobalTimer();

	~GlobalTimer();
	// ���߼�ʵ��  
	void GlobalUpdate(float t);
protected:
	static GlobalTimer* m_instance;
public:
	// �õ�ȫ�ֶ�ʱ������
	static GlobalTimer* Getinstance();
};

#endif // __GlobalTimer_H__                                                                                                                                                                                                                                                                                                                                              