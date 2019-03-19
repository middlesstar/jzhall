#ifndef __Stage_H__
#define __Stage_H__

#include "slwh/Base/ModelBase.h"
#include "slwh/Main-Script/SpeRes/SpeD3Y.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
#include "Sphere.h"


#ifndef STAGE_SPHERE_MAX
#define	STAGE_SPHERE_MAX			5
#endif
class Stage :
	public ModelBase
{
protected:

#ifndef _SpeD3Y
#else friend class SpeD3Y;
#endif


	enum enumPointAtState
	{
		ptNormal = 0x00000000,                                      //指针静止状态
		ptSpeedAdd = 0x00000004,                                    //指针加速的时间
		ptSpeedMax = 0x00000006,                                    //指针最大速度时间
		ptSpeedSub = 0x00000007,                                    //指针减速的时间
		ptSpeedMin = 0x0000000c,                                    //指针小减速度运行时间
	};

	enumPointAtState		m_ptState = ptNormal;

	float					m_speed;								//当前速度
	float					m_runTime;								//指针运转时间
	bool					m_bIsLerp;								//最后2秒精确控制指针
	float					m_targetAngle;							//目标位置角度
	Sphere*					m_sphere;								//球

	cocos2d::Animate3D*		m_open;									//动画1
	cocos2d::Animate3D*		m_close;								//动画2

	Sprite3D*				m_spr[STAGE_SPHERE_MAX];
	Sprite3D*				m_stage;
	Sprite3D*				m_point;

	PUParticleSystem3D*		m_ps;
	PUParticleSystem3D*		m_ps1;
public:
	Stage();
	~Stage();

	float GetRotate();
	bool DetalSpeed(float t);
	void StartPlay(int index);
	void StartPlay(float angle);
	float GetPtTarget(){ return m_targetAngle; };
	void AddSphere(Sphere* const &s);
	void OpenSphere(bool b);
	bool AttachSprite3D(
		std::string const &stage,
		std::string const &point,
		std::string const &sphere);

protected:

	void Start();
	void update(float delta);
	void updatePs(float t);
	void updatePs1(float t);
	void updatePs1Once(float t);
};

#endif

