#ifndef SHZ_UI_COMMENT_H_H
#define SHZ_UI_COMMENT_H_H
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ShieldLayer_SHZ.h"

class SHZUIComment
	:public cocos2d::CCLayer
{
public:
	CREATE_FUNC(SHZUIComment);

private:
	SHZUIComment();
	~SHZUIComment();


public:
	virtual bool init();

private:
	bool setUpView();

public:
	void btnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);

private:
	SHZShieldLayer* mShieldLayer;  //ÆÁ±Î²ã
};
#endif