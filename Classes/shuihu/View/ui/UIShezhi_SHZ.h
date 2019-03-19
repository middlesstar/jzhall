#ifndef SHZ__UIShezhi_H_
#define SHZ__UIShezhi_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class SHZShieldLayer;

class SHZUIShezhi
	: public cocos2d::CCLayer
{
public:
	CREATE_FUNC(SHZUIShezhi);

public:
	SHZUIShezhi();
	~SHZUIShezhi();
	bool init();

	virtual void onExit();
private:
	void musicChanged(Ref* sender, Control::EventType ev);
	void soundChanged(Ref* sender, Control::EventType ev);
	void onCloseClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);

private:
	SHZShieldLayer* mShieldLayer;
};

#endif // _UIShezhi_H_