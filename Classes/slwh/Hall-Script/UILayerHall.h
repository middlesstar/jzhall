#ifndef __UILayerHall_H__
#define __UILayerHall_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "slwh/ResourcePath.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class UILayerHall :
	public Layer
{
private:
	Button*				m_roomRabbit;	//兔房
	TextAtlas*			m_limitRabbit;	//兔子分数
	Button*				m_roomPanda;	//猫房
	TextAtlas*			m_limitPanda;	//猫分数
	Button*				m_roomLion;		//狮房
	TextAtlas*			m_limitLion;	//狮分数
	Button*				m_recharge;		//充值按钮
	Button*				m_setting;		//设置按钮
	Button*				m_regular;		//规则按钮
	Button*				m_exit;			//退出
	TextAtlas*			m_goldNum;		//金币
	TextAtlas*			m_yuanbaoNum;	//元宝


public:
	void SetGold(SCORE g);

	void SetYunBao(SCORE y);

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(UILayerHall);

	virtual void onEnter() override;

	virtual void onExit() override;

	~UILayerHall();
protected:
	UILayerHall();

	void BtEvent(Ref *pSender, TouchEventType type);

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;

	virtual void onEnterTransitionDidFinish() override;

	void setTime(float t);
	//进入房间
	void goRoom(int roomIndex);
	//刷数字
	void refreshNum(TextAtlas* text,SCORE num);
	//exit
	void ExitToHall();
public:
	//Scene* mainScene;
	//刷新金币
	void updateSlwhCoin(Ref* ref);

};


#endif


