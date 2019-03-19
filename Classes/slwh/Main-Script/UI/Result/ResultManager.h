#ifndef __ResultManager_H__
#define __ResultManager_H__

#include "slwh/Base/UIBase.h"
#include "ResOneJet.h"
#include "ResAnimal.h"
#include "ResZHX.h"
#include "XYCJRes.h"

/*
开奖结果管理类
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
	int StartPlay(AnimalResult e, int* jetbeilv, int* jetNum, AnimalResult* e1 = nullptr);	//动物结果，倍率数组，压分数组，4LF结果(默认空) 返回得分
	int StartPlay(AnimalResult e, int* jetbeilv, int* jetNum,SCORE cj);				//动物结果，倍率数组，压分数组，彩金得分
	int StartPlay(AnimalResult e, int* jetbeilv, int* jetNum,int aType, int *xycjarr, float scorebl,float yuanbaobl, float cpbl);	//动物结果，倍率数组，压分数组
	void StartPlay();
	void StopPlay();
	int getyuanbao(){ return m_getYuanbao; }
	int getticket(){ return m_getTicket; }
	//void update(float t);
protected:
	int SetRes(AnimalResult e, int* beilv, int* jetNum, AnimalResult* e1 = nullptr, SCORE cj = 0, int aType = _ZERO,int* xycjarr = nullptr, float scorebl = _ZERO, float yuanbao = _ZERO, float cp = _ZERO);
	
	void Start();

	bool onTouchBegan(Touch* touch, Event* event);
	//播放烟花
	void playYanhua(Vec2 v2);
	//播放盖章效果
	void PlayGaizhang();
	//盖章效果结束回调
	void gaizhangCallfunc();
	//震动结束回掉
	//void zdCallFunc(Node* p);
	//播放金币喷泉和数字滚动
	void goldandnumrool(float t);
	//数字滚动效果
	void numroll(float t);
	//设置GetNum艺术数字string
	void setGetnum(SCORE l);
	//播放金币飞行动画
	void playGoldFly(Vec2 startPos, Vec2 endPos);

};

#endif

 