#ifndef __Animal_H__
#define __Animal_H__

#include "slwh/Base/ModelBase.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
/*
√ø∏ˆ∂ØŒÔµƒπ‹¿Ì¿‡
*/

class Animal : public ModelBase
{

protected:
	//enum enumAnimalChild
	//{
	//	Invalidate = -1,
	//	Close2Center,
	//	FarawayCenter,
	//};

	//enumAnimalChild					m_childAnimal;									//∂ØŒÔœÚ÷–“∆∂Øø™πÿ
	//float							m_centerTime;									//∂ØŒÔ÷––ƒ“∆∂Ø ±º‰
	//Vec3							m_animalOldpt;                                  //∂ØŒÔ“∆∂Ø«∞µƒŒª÷√

	//static Quaternion				m_rotate2Camera;                                //∂ØŒÔ√Ê∂‘œ‡ª˙µƒ–˝◊™Ω«∂»

	//static float					m_animalRotateSpeedSub;							//∂ØŒÔ–˝◊™πÈŒ™ÀŸ∂»
	//static Quaternion				m_animalOldV3;									//∂ØŒÔ≥ı ºΩ«∂»    


	//const static float				m_animalRotateSpeed;							//∂ØŒÔ–˝◊™ÀŸ∂»
	//const static Vec3				m_centerPt;										//∂ØŒÔ“∆∂Ø÷––ƒ◊¯±Í
	//const static float				m_animalMoveSpeed;								//∂ØŒÔ“∆∂ØÀŸ∂»
	//const static float				m_animalMoveSpeedF;								//∂ØŒÔ“∆∂Ø≈◊ŒÔ‘À∂Ø∑÷ÀŸ∂»
	//const static float				Max_CenterTime;									//√ø∏ˆµ•∏ˆ‘À∂Ø ±º‰Œ™1.5√Î      

	bool							m_bIsAcRun;										//∂Øª≠ «∑Ò
protected:
	cocos2d::Animate3D*		m_ac1;											//∂Øª≠1
	cocos2d::Animate3D*		m_ac2;											//∂Øª≠2
	cocos2d::Animate3D*		m_ac3;											//∂Øª≠3
	//PUParticleSystem3D*		m_ps;

	void updateAc(float t);
	void Bridge(float t);
	void PlayNormalAc();
	
public:
	//¥Úø™∆’Õ®∂Øª≠
	void OpenNormalAc(bool b);
	//º”‘ÿ∂ØŒÔ2∏ˆ∂Øª≠
	bool LoadAction(Animate3D* const &one, Animate3D* const &two, Animate3D* const &three);

	//≤•∑≈∂Øª≠Ω”ø⁄
	void PlayHappyAc();

	Animal();
	~Animal();


};

#endif

