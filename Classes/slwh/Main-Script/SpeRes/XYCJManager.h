#ifndef __XYCJManager_H__
#define __XYCJManager_H__

#include "slwh/Base/UIBase.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

#define ZP_MAX		16

//最多开奖数
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

	//当前位置
	int								m_index;
	int								m_res[RES_MAX];
	int								m_resBufRun[RES_MAX];
	int								m_resNowIndex;
	//运动属性
	float							m_speed;

	int								m_scoreBl;		//得分倍率
	float							m_yuanbaobl;	//得到元宝倍率
	float							m_cpBl;			//船票倍率

	static float					m_beilv[ZP_MAX];	//XYCJ转盘倍率
public:
	XYCJManager();
	~XYCJManager();
	//A区间的物品 和 A区间倍率 4个结果
	void StartPlayEff();														//播放特效
	void StopPlayEff();															//停止特效
	void StartPlay(int Atype, float Avalue, int* x);											//开始XYCJ主逻辑
	void StopPlay();

	void SetCallback(Node* n, XYCJ_CallFunc callback);							//结束回调函数
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

