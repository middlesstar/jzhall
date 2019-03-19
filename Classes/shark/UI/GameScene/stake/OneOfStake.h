#ifndef _OneOfStake_H_
#define _OneOfStake_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"

class OneOfStake : public  ViewBase
{
public:

	OneOfStake();
	~OneOfStake();

	CREATE_FUNC(OneOfStake);

	virtual void setContent();

	//按钮响应事件
	void BtEvent(Ref *pSender, TouchEventType type);
public:
	//设置压分档
	static int SetAddJet(int code = 0);
	//设置按钮索引
	void SetTag(int x){ m_tag = x; }
	//得到按钮索引
	int GetTag(){ return m_tag; }
	//设置倍率
	void SetBeiLv(int g);
	//设置我的压分
	void SetMyStake(SCORE g);
	//设置总压分
	//void SetAllJetton(int g);
	//增加压分
	void AddStake();
	//得到当前压分档
	SCORE GetStake(){ return m_stake; }
	//服务器返回压分成功 刷新压分
	void UpdateStake(SCORE score);

	//放大数字
	void enlargeNum();

	//显示旋转的光圈
	void showRing();
	//显示按钮上的光圈
	void showCircle();

	//按钮特效
	void btnEffect();
	//飞往金币堆
	void GoldFly();
	//回调销毁
	void CallBack(Node* node);
	//重置押分按钮标志位
	void resetBtn();
protected:
	Node*     pMainNode;

protected:

	static Vec2			m_targetPos[12];										//金币飞行12个目标位置
	SCORE			    m_stake;												//当前押注数额
	TextAtlas*			m_beilvNum;											   //倍率艺术数字
	TextAtlas*			m_mystakeNum;											//我的押注艺术数字
	Sprite*             m_ring;                                               //旋转光圈
	Sprite*             m_circle;                                             //押分显示的光圈
	int					m_tag;												//按照索引是第几个按钮		

	bool                stakeFlag;                                             //判断押分是否成功

	static int			stakeAdd;												//压分增加
public:
	static SCORE		MyAllTotel;											//我的总压分




};

#endif // _OneOfStake_H_
