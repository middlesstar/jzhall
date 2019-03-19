#ifndef __Card_H__
#define __Card_H__

#include "cocos2d.h"
class Card : public cocos2d::Sprite
{
public:
	/**
	*   ����һ��PokerSprite
	*   front   �Ƶ�����
	*   back    �Ƶı���
	*/
	static Card* create(cocos2d::Sprite* front, cocos2d::Sprite* back);
	/* ����
	* duration  ����ʱ�����룩
	*/
	void close(float duration = 0.5);
	//�ƶ�����
	void move(float duration,float x,int num );
	//��
	void open(float duration);
protected:
	bool initWithSprite(cocos2d::Sprite* front, cocos2d::Sprite* back);
private:
	//��¼�����ƶ�
	bool flag;
	cocos2d::Sprite* front;
	cocos2d::Sprite* back;
};


#endif // __Card_H__
