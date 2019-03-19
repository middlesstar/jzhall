#ifndef SHZ__ShieldLayer_H_
#define SHZ__ShieldLayer_H_

#include "cocos2d.h"
USING_NS_CC;
//////////////////////////////////////////////////////////////////////////
//ÆÁ±Î²ã
class SHZShieldLayer: public cocos2d::CCLayer
{
public:
	static SHZShieldLayer* create(int prority)
	{
		SHZShieldLayer* layer = new SHZShieldLayer();
		if (layer && layer->init(prority))
		{
			layer->autorelease();
			return layer;
		}

		delete layer;
		return 0;
	}

private:
	SHZShieldLayer():mPrority(-128)
	{
 
	}

	~SHZShieldLayer()
	{
		setTouchSwallow(false);
	}

	bool init(int prority)
	{
		do 
		{
			mPrority = prority;
			CC_BREAK_IF(!Layer::init());
			setTouchSwallow(true);
			return true;
		} while (0);
		return false;
	}


public:

	void setTouchSwallow(bool bSwallow )
	{
		if (bSwallow)
		{
			/*if (m_pEventListener)
			{
			return;
			}*/
			m_pEventListener = EventListenerTouchOneByOne::create();
			m_pEventListener->setSwallowTouches(true);

			m_pEventListener->onTouchBegan = [this](Touch* touch, Event* event)->bool{return true; };
			getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEventListener, this);
			
		}
		else
		{
			getEventDispatcher()->removeEventListener(m_pEventListener);
			m_pEventListener = nullptr;
		}
	}

private:
	int mPrority;

	cocos2d::EventListenerTouchOneByOne* m_pEventListener;
};

#endif // _ShieldLayer_H_
