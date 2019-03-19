#ifndef SHZ__UIHelp_H_
#define SHZ__UIHelp_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "shuihu/View/Scene/GameScene/TouchLayer_SHZ.h"
class SHZShieldLayer;

class SHZUIHelp
	: public cocos2d::Node
	, public SHZITouchSink
{
public:
	CREATE_FUNC(SHZUIHelp);

public:
	SHZUIHelp();
	~SHZUIHelp(); 
	bool init();
public:
	virtual void onExit();
	
private:
	void onCloseClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);

public:
	/*virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);*/
	void touchEvent();

public:

	void SetLeftAnim();

	void SetRightAnim();

	void RightHelpMove(cocos2d::CCNode* node, void* data);

	void LeftHelpMove(cocos2d::CCNode* node, void* data);

private:

	SHZShieldLayer* mShieldLayer;

	cocos2d::CCPoint		m_ptPos;

	cocos2d::CCSprite*		m_spHelp[5];

	int						m_iCount;

	bool					m_bMove;		//移动中

public:

	//TouchLayer*						touch_layer_;	
	cocos2d::EventListenerTouchOneByOne* listener;//鼠标接口
};

#endif // _UIHelp_H_