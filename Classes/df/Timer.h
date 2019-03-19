#ifndef _Timer_H_
#define _Timer_H_

class CoTimer
{
public:
	/**
	* 获取当前系统时间(单位:毫秒)
	* @return int
	*/
	static unsigned long getCurrentTime();

public:
	/**
	* 构造函数
	* @param delay:int 毫秒数
	*/
	CoTimer(int delay = 0);

	/**
	* 设置延迟时间
	* @param delay:int 毫秒数
	*/
	void init(int delay);

	/**
	* 是否时间到了
	* @return bool
	*/
	bool isTimeUp() const;

	/**
	* 获取流逝时间
	*/
	unsigned long getElapsed() const;
protected:
	unsigned long mStart;
	unsigned long mDelay;
};

#endif // _Timer_H_