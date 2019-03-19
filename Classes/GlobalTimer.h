#ifndef __GlobalTimer_H__
#define __GlobalTimer_H__

#include "cocos2d.h"
USING_NS_CC;


class GlobalTimer : public Node
{
public:
	// 启动全局定时器(每一帧都执行)
	void StarPlay();
	// 停止
	void StopPlay();
	// 暂停
	void Pause();
	// 恢复  
	void Resume();

protected:
	GlobalTimer();

	~GlobalTimer();
	// 主逻辑实现  
	void GlobalUpdate(float t);
protected:
	static GlobalTimer* m_instance;
public:
	// 得到全局定时器对象
	static GlobalTimer* Getinstance();
};

#endif // __GlobalTimer_H__                                                                                                                                                                                                                                                                                                                                              