#ifndef SHZ_GoldLayer_H_
#define SHZ_GoldLayer_H_

#include "cocos2d.h"

class SHZGoldDownLayer : public cocos2d::CCLayer
{
public:
	virtual bool init();
	
	inline bool isTouched() { return  m_isTouched; }

	CREATE_FUNC(SHZGoldDownLayer);

	~SHZGoldDownLayer();

public:
	//virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	//virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//virtual void registerWithTouchDispatcher();
	virtual void onEnter();
	virtual void onExit();


	void setTouchSwallow(bool bSwallow );             //设置吞噬触摸
private:
	void addGold(float dt);
	void goldMoveFinish(cocos2d::CCNode* pSender);

private:
	SHZGoldDownLayer() { m_isTouched = false; }

private:
	float			m_minXPos;	// 金币出现的最小X位置
	float			m_maxXPos;	// 金币出现的最大X位置
	static float	m_yPos;		// 金币出现的Y位置
	static float	m_speed;	// 金币下落速度
	static float	m_time;		// 金币下落时间

	bool			m_isTouched;// 是否触摸

	cocos2d::EventListenerTouchOneByOne* m_pEventListener;
};

#endif