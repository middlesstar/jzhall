#ifndef __ZHXControl_H__
#define __ZHXControl_H__

#include "slwh/Base/UIBase.h"

class ZHXControl :
	public UIBase
{
protected:
	//庄和闲的控制
	enum enumZHXState
	{
		ZHXNormal = 0x00000000,										//静止状态
		ZHXSpeedAdd = 0x00000003,                                    //加速的时间
		ZHXSpeedMax = 0x00000004,                                    //最大速度时间
		ZHXSpeedSub = 0x00000005,                                    //减速的时间
		ZHXSpeedMin = 0x0FFFFFFF,                                    //小减速度运行时间 直到确定结果才停止
	};
	//庄和闲控制状态变量
	enumZHXState m_enumState = ZHXNormal;

	//此命名规则是初始状态
	ImageView*				m_zhxUp;
	ImageView*				m_zhxDown;


	//记录初始位置
	Vec2					m_downPt;
	Vec2					m_upPt;
	//动画变量
	float PixelH;										//移动的距离
	enumGameZXH				m_nowZHX;					//当前显示的ZHX
	static const float		Speed_Add;					//加速度
	static const float		Speed_Sub;					//减速度
	static const float		Speed_MinSub;				//小减速度
	float					m_speed;					//当前速度
	float					m_time;						//当前时间


	//结果变量
	enumGameZXH m_rsZHX = InvalidZHX;


	//是否转动
	bool m_bIsRunning = false;
	std::map<enumGameZXH, enumGameZXH>	m_ZHXMap;
	std::map<enumGameZXH, std::string>	m_ZHXTex;
public:
	ZHXControl();
	~ZHXControl();

	//开始转动接口
	void StartPlay(enumGameZXH e);
	void StartPlay(int index);
protected:
	void Start();
	void update(float t);

	//滚动一次动画
	void RoundOne(float t);
	//初始化一局游戏
	void OnInit();

};

#endif

