#ifndef  _GrabRed_H_
#define  _GrabRed_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ActiveFish.h"
#include "ActiveInfo.h"
#include "RedPacket.h"
#include "ActiveConfig.h"

USING_NS_CC;
using namespace ui;

typedef void(Node::*SEL_CallFun)(int x, float y);

//*******************************************************************************
//RedPacket.h


//抢红包管理类
class GrabRed :
	public Layer
{
protected:
	Sprite*						m_backImg;
	Sprite*						m_topImg;
	Sprite*						m_wace0;
	Sprite*						m_wace1;
	Text*						m_text;


	RedPacket					m_redPacket;
	ActiveFish					m_fish;
	ActiveInfo					m_info;

	bool						m_bIsActivity;

	Node*						m_pListen;
	// 回调函数指针  
	SEL_CallFun					m_pfnSelectior;


protected:
	virtual bool init();

	bool onTouchBegan(Touch* touch, Event* event);
public:	


	void SetCallback(Node* n, SEL_CallFun callback);


	//void update(float delta);
	GrabRed();
	~GrabRed();

	void StartPlay();
	void StopPlay();

	float GetTime();
	float GetHF();						//此接口改为元宝(策划需求)
	int GetGold();

	

	CREATE_FUNC(GrabRed);
protected:
	void GameOver();

	void update(float t);


	//测试
	//void BtEvent(Ref* pSender, TouchEventType type);
};

#endif // _GrabRed_H_