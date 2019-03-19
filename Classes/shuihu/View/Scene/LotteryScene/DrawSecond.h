#ifndef __DrawSecond_LAYER_H__
#define __DrawSecond_LAYER_H__

#include "cocos2d.h"
class Card;
class LotteryMission;
class DrawSecond : public cocos2d::Layer
{
public:
	DrawSecond();
	~DrawSecond();
public:
    bool init(int type);
	static DrawSecond* createLayer(int type);
public:
	//创建纸牌
	void createCard();
	//倒计时1
	void timeChange1(float f);
	//倒计时2
	void timeChange2(float f);
	//翻牌
	void shut();
	//控制洗牌速度
	void control();
	//控制动作的先后
	void privious();
	//卡牌发光
	void cardLight();
	//触摸监听
	void touchlayer();
	//显示所有的卡牌
	void showAll();
	//移除所有的层 
	void removeLayer();
public:
	void setTouchSwallow(bool bSwallow = true);
private:
	cocos2d::EventListenerTouchOneByOne* m_pEventListener;
private:
	cocos2d::EventListenerTouchOneByOne* listener;
	//触摸点
	cocos2d::Point		      m_ptPos;
	cocos2d::EventDispatcher* dispatcher;
private:
	cocos2d::Vector<cocos2d::LabelTTF* >          scoreLabel;
	//记录当前抽奖的等级
	int     degree;
	//记录洗牌的次数
	int      num;
	//倒计时
	int      t;
	//倒计时
	char     timeShow[10];
	//倒计时标签1
	cocos2d::LabelAtlas*    time1;
	//屏幕大小
	cocos2d::Size      s;
	//显示奖励的卡片
	cocos2d::Vector<Card* > cards;
	//开奖显示的光圈
	cocos2d::Vector<cocos2d::Sprite* > light;
	//创建触摸屏蔽层
	cocos2d::Layer*   contentLayer;
	//创建数据接收对象
	LotteryMission* mission;
};

#endif // __DrawSecond_LAYER_H__
