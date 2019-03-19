#ifndef __ResultManager_H__
#define __ResultManager_H__

#include "slwh/Base/UIBase.h"
#include "ResOneJet.h"
#include "ResAnimal.h"
#include "ResZHX.h"
#include "XYCJRes.h"

/*
�������������
*/

#define RES_ANINAL_NUM				12


class ResultManager :
	public UIBase
{
protected:
	//LayerColor*				m_layerColor;
	Node*						m_speCJ;
	TextAtlas*					m_cjnum;
	ResAnimal					m_resAnimal;
	ResZHX						m_resZHX;
	ResOneJet					m_resArray[RES_ANINAL_NUM];
	XYCJRes						m_xycjres;
	TextAtlas*					m_getNum;
	ImageView*					m_speTitle;
	Vec2						m_visible;
	float						m_speed;
	Node*						m_res;
	Sprite*						m_betGold;
	SCORE						m_getnum;
	int							m_getYuanbao;
	int							m_getTicket;
public:
	ResultManager();
	~ResultManager();
	//void attachBetgold(Sprite* n);
	int StartPlay(AnimalResult e, int* jetbeilv, int* jetNum, AnimalResult* e1 = nullptr);	//���������������飬ѹ�����飬4LF���(Ĭ�Ͽ�) ���ص÷�
	int StartPlay(AnimalResult e, int* jetbeilv, int* jetNum,SCORE cj);				//���������������飬ѹ�����飬�ʽ�÷�
	int StartPlay(AnimalResult e, int* jetbeilv, int* jetNum,int aType, int *xycjarr, float scorebl,float yuanbaobl, float cpbl);	//���������������飬ѹ������
	void StartPlay();
	void StopPlay();
	int getyuanbao(){ return m_getYuanbao; }
	int getticket(){ return m_getTicket; }
	//void update(float t);
protected:
	int SetRes(AnimalResult e, int* beilv, int* jetNum, AnimalResult* e1 = nullptr, SCORE cj = 0, int aType = _ZERO,int* xycjarr = nullptr, float scorebl = _ZERO, float yuanbao = _ZERO, float cp = _ZERO);
	
	void Start();

	bool onTouchBegan(Touch* touch, Event* event);
	//�����̻�
	void playYanhua(Vec2 v2);
	//���Ÿ���Ч��
	void PlayGaizhang();
	//����Ч�������ص�
	void gaizhangCallfunc();
	//�𶯽����ص�
	//void zdCallFunc(Node* p);
	//���Ž����Ȫ�����ֹ���
	void goldandnumrool(float t);
	//���ֹ���Ч��
	void numroll(float t);
	//����GetNum��������string
	void setGetnum(SCORE l);
	//���Ž�ҷ��ж���
	void playGoldFly(Vec2 startPos, Vec2 endPos);

};

#endif

 