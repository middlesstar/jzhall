#ifndef __Card_H__
#define __Card_H__

#include "cocos2d.h"
class Card : public cocos2d::Sprite
{
public:
	/**
	*   创建一个PokerSprite
	*   front   牌的正面
	*   back    牌的背面
	*/
	static Card* create(cocos2d::Sprite* front, cocos2d::Sprite* back);
	/* 翻牌
	* duration  动作时长（秒）
	*/
	void close(float duration = 0.5);
	//移动卡牌
	void move(float duration,float x,int num );
	//打开
	void open(float duration);
protected:
	bool initWithSprite(cocos2d::Sprite* front, cocos2d::Sprite* back);
private:
	//记录卡牌移动
	bool flag;
	cocos2d::Sprite* front;
	cocos2d::Sprite* back;
};


#endif // __Card_H__
