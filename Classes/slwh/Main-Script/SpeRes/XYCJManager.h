#ifndef __XYCJManager_H__
#define __XYCJManager_H__

#include "slwh/Base/UIBase.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

#define ZP_MAX		16

//��࿪����
#define RES_MAX		4

typedef void(Node::*XYCJ_CallFunc)();

#define CallBack_XYCJ(x)	(XYCJ_CallFunc)(&x)	

class XYCJManager :
	public UIBase
{

protected:
	ImageView*						m_resMid;
	ImageView*						m_resLeft;
	ImageView*						m_resRight;
	ImageView*						m_title;

	ImageView*						m_list[ZP_MAX];
	ImageView*						m_goldlist[ZP_MAX];
	std::map<int, std::string>		m_normalMap;
	std::map<int, std::string>		m_goldMap;

	PUParticleSystem3D*				m_ps;
	Node*							m_pListen;
	XYCJ_CallFunc					m_pfnSelectior;

	//��ǰλ��
	int								m_index;
	int								m_res[RES_MAX];
	int								m_resBufRun[RES_MAX];
	int								m_resNowIndex;
	//�˶�����
	float							m_speed;

	int								m_scoreBl;		//�÷ֱ���
	float							m_yuanbaobl;	//�õ�Ԫ������
	float							m_cpBl;			//��Ʊ����

	static float					m_beilv[ZP_MAX];	//XYCJת�̱���
public:
	XYCJManager();
	~XYCJManager();
	//A�������Ʒ �� A���䱶�� 4�����
	void StartPlayEff();														//������Ч
	void StopPlayEff();															//ֹͣ��Ч
	void StartPlay(int Atype, float Avalue, int* x);											//��ʼXYCJ���߼�
	void StopPlay();

	void SetCallback(Node* n, XYCJ_CallFunc callback);							//�����ص�����
	bool onTouchBegan(Touch* touch, Event* event);
	int getScoreBl(){ return m_scoreBl; };
	float getyuanbaoBl(){ return m_yuanbaobl; };
	float getcpBl(){ return m_cpBl; };
protected:
	void Start();

	void oneStep();
	void Running(float t);
	void update(float t);
	void Res(float t);

	void CallBack();

};

#endif

