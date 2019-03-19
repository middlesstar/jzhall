#ifndef __AnimalManager_H__
#define __AnimalManager_H__

#include "slwh/Base/ModelBase.h"
#include "Animal.h"

//管理动物群及踏板集

#ifndef Animal_Max
#define Animal_Max 24
#endif

//动物顺序  兔子0 猴子1 熊猫2 狮子3
//    1,0,1,3,
//    0,1,0,2,
//    0,2,1,3,
//    2,0,1,0,
//    3,0,1,0,
//    2,0,1,3,


/*
所有动物的管理类 （脚本）
*/

class AnimalManager :
	public ModelBase
{
protected:

	//动物转盘属性
	enum enumAnimalState
	{
		AnimalNormal = 0x00000000,									//静止状态
		AnimalptSpeedAdd = 0x00000004,								//加速的时间
		AnimalSpeedMax = 0x00000005,								//最大速度时间
		AnimalSpeedSub = 0x00000008,								//减速的时间
		AnimalSpeedMin = 0x0000000e,								//小减速度运行时间
	};

	const float			Speed_Add = 50.0f;								//加速度
	const float			Speed_Sub = 20.0f;								//减速度
	//const float Speed_Min	= 1.5f;									    //最小速度

	float				m_speed;										//当前速度
	float				m_runTime;										//运转时间
	float				m_targetAngle;									//目标位置
	bool				m_bIsLerp;										//是否开启插值进行精确控制位置

	enumAnimalState		m_animalState;									//动物状态


	Vec3				m_oldPos;
	Vec3				m_oldRot;
	int					m_centerAnimal;									//向中心移动的动物
	int					m_animalType;
	//bool				m_bIsCenter;									//是否移动到中心

protected:
	//踏板
	Sprite3D* m_step[Animal_Max];

	//动物
	Animal m_animal[Animal_Max];

public:
	AnimalManager();
	~AnimalManager();

	//4个动物及踏板路径
	bool AttachSprite3D(
		std::string const &Ani0Path,
		std::string const &Ani1Path,
		std::string const &Ani2Path,
		std::string const &Ani3Path,
		std::string const &stepPath);

	//开启动物普通动作
	void OpenAniNorAc(bool b = true);

	//开启转盘转动 最终停在索引为index的位置 0 ~ 23
	void StartPlay(int index,int colorindex);
	//动物移动 和 跳舞
	void ResPlay(int ptIndex, enumAnimalColor e1);
	void ResPlay(int ptIndex, int color);
	void ResStop();
	//动物直接跳舞
	void ResDance(int ptIndex);
protected:

	//void AnimalToCenterUpdate(float t);
	void Start();

	void update(float t);

	bool DetalSpeed(float t);

	//加载动物
	bool LoadAnimal(
		std::string const &Ani0Path,
		std::string const &Ani1Path,
		std::string const &Ani2Path,
		std::string const &Ani3Path
		);
	//加载动物踏板
	bool LoadStep(std::string const &stepPath);
	std::string getStepTex(int color);


	void CallBackMoveToCenterEnd();
	//void CallBackMoveToOld();


};

#endif

