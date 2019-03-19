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
	//����ֽ��
	void createCard();
	//����ʱ1
	void timeChange1(float f);
	//����ʱ2
	void timeChange2(float f);
	//����
	void shut();
	//����ϴ���ٶ�
	void control();
	//���ƶ������Ⱥ�
	void privious();
	//���Ʒ���
	void cardLight();
	//��������
	void touchlayer();
	//��ʾ���еĿ���
	void showAll();
	//�Ƴ����еĲ� 
	void removeLayer();
public:
	void setTouchSwallow(bool bSwallow = true);
private:
	cocos2d::EventListenerTouchOneByOne* m_pEventListener;
private:
	cocos2d::EventListenerTouchOneByOne* listener;
	//������
	cocos2d::Point		      m_ptPos;
	cocos2d::EventDispatcher* dispatcher;
private:
	cocos2d::Vector<cocos2d::LabelTTF* >          scoreLabel;
	//��¼��ǰ�齱�ĵȼ�
	int     degree;
	//��¼ϴ�ƵĴ���
	int      num;
	//����ʱ
	int      t;
	//����ʱ
	char     timeShow[10];
	//����ʱ��ǩ1
	cocos2d::LabelAtlas*    time1;
	//��Ļ��С
	cocos2d::Size      s;
	//��ʾ�����Ŀ�Ƭ
	cocos2d::Vector<Card* > cards;
	//������ʾ�Ĺ�Ȧ
	cocos2d::Vector<cocos2d::Sprite* > light;
	//�����������β�
	cocos2d::Layer*   contentLayer;
	//�������ݽ��ն���
	LotteryMission* mission;
};

#endif // __DrawSecond_LAYER_H__
