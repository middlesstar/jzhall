#ifndef  _WindowExpedition_H_
#define  _WindowExpedition_H_

#include "Window/WindowBase.h"

class Card;
/* 窗口基类 */
class WindowExpedition : public WindowBase
{
public:
	WindowExpedition();
	virtual ~WindowExpedition();

	CREATE_FUNC(WindowExpedition);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;
	virtual void updateWindow(int nCode, void* pData) override;	
protected:
	void _btnOpen_CallBack(Ref* pRef);
	void _btnChallenge_CallBack(Ref* pRef);

	//创建卡牌
	void creareCard();
	//倒计时
	void timeChange(float f);
	//自动翻转卡牌
	void autoTurn();
	//添加触摸
	void touchlayer();
	//出现手晃动
	void handShake();
	//翻转前两个卡牌
	void turnTwo();
	//显示开奖结果
	void showResult();
	//移除开奖结果的显示
	void removeShow(float t);
	
private:
	cocos2d::EventListenerTouchOneByOne* listener;
	//触摸点
	cocos2d::Point		      m_ptPos;
	cocos2d::EventDispatcher* dispatcher;

protected:
	Sprite* m_bg1;
	Sprite* m_bg2;
	Sprite* ticket;
	Sprite* tips;
	Sprite* clock;
	Sprite* hand;
	Sprite* show;

	Button* m_pBtnOpen;
	Button* m_pBtnChallenge;

	LabelAtlas* timer;
	LabelAtlas* text;

	//显示奖励的卡片
	Vector<Card* > cards;

	//倒计时
	int      time;
	//创建容器层
	Layer*   contentLayer;


};

#endif // _WindowExpedition_H_

