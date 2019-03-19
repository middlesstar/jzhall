#ifndef __AnimalManager_H__
#define __AnimalManager_H__

#include "slwh/Base/ModelBase.h"
#include "Animal.h"

//������Ⱥ��̤�弯

#ifndef Animal_Max
#define Animal_Max 24
#endif

//����˳��  ����0 ����1 ��è2 ʨ��3
//    1,0,1,3,
//    0,1,0,2,
//    0,2,1,3,
//    2,0,1,0,
//    3,0,1,0,
//    2,0,1,3,


/*
���ж���Ĺ����� ���ű���
*/

class AnimalManager :
	public ModelBase
{
protected:

	//����ת������
	enum enumAnimalState
	{
		AnimalNormal = 0x00000000,									//��ֹ״̬
		AnimalptSpeedAdd = 0x00000004,								//���ٵ�ʱ��
		AnimalSpeedMax = 0x00000005,								//����ٶ�ʱ��
		AnimalSpeedSub = 0x00000008,								//���ٵ�ʱ��
		AnimalSpeedMin = 0x0000000e,								//С���ٶ�����ʱ��
	};

	const float			Speed_Add = 50.0f;								//���ٶ�
	const float			Speed_Sub = 20.0f;								//���ٶ�
	//const float Speed_Min	= 1.5f;									    //��С�ٶ�

	float				m_speed;										//��ǰ�ٶ�
	float				m_runTime;										//��תʱ��
	float				m_targetAngle;									//Ŀ��λ��
	bool				m_bIsLerp;										//�Ƿ�����ֵ���о�ȷ����λ��

	enumAnimalState		m_animalState;									//����״̬


	Vec3				m_oldPos;
	Vec3				m_oldRot;
	int					m_centerAnimal;									//�������ƶ��Ķ���
	int					m_animalType;
	//bool				m_bIsCenter;									//�Ƿ��ƶ�������

protected:
	//̤��
	Sprite3D* m_step[Animal_Max];

	//����
	Animal m_animal[Animal_Max];

public:
	AnimalManager();
	~AnimalManager();

	//4�����Ｐ̤��·��
	bool AttachSprite3D(
		std::string const &Ani0Path,
		std::string const &Ani1Path,
		std::string const &Ani2Path,
		std::string const &Ani3Path,
		std::string const &stepPath);

	//����������ͨ����
	void OpenAniNorAc(bool b = true);

	//����ת��ת�� ����ͣ������Ϊindex��λ�� 0 ~ 23
	void StartPlay(int index,int colorindex);
	//�����ƶ� �� ����
	void ResPlay(int ptIndex, enumAnimalColor e1);
	void ResPlay(int ptIndex, int color);
	void ResStop();
	//����ֱ������
	void ResDance(int ptIndex);
protected:

	//void AnimalToCenterUpdate(float t);
	void Start();

	void update(float t);

	bool DetalSpeed(float t);

	//���ض���
	bool LoadAnimal(
		std::string const &Ani0Path,
		std::string const &Ani1Path,
		std::string const &Ani2Path,
		std::string const &Ani3Path
		);
	//���ض���̤��
	bool LoadStep(std::string const &stepPath);
	std::string getStepTex(int color);


	void CallBackMoveToCenterEnd();
	//void CallBackMoveToOld();


};

#endif

