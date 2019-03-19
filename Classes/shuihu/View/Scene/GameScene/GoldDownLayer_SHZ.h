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


	void setTouchSwallow(bool bSwallow );             //�������ɴ���
private:
	void addGold(float dt);
	void goldMoveFinish(cocos2d::CCNode* pSender);

private:
	SHZGoldDownLayer() { m_isTouched = false; }

private:
	float			m_minXPos;	// ��ҳ��ֵ���СXλ��
	float			m_maxXPos;	// ��ҳ��ֵ����Xλ��
	static float	m_yPos;		// ��ҳ��ֵ�Yλ��
	static float	m_speed;	// ��������ٶ�
	static float	m_time;		// �������ʱ��

	bool			m_isTouched;// �Ƿ���

	cocos2d::EventListenerTouchOneByOne* m_pEventListener;
};

#endif