#ifndef  _WindowExpedition_H_
#define  _WindowExpedition_H_

#include "Window/WindowBase.h"

class Card;
/* ���ڻ��� */
class WindowExpedition : public WindowBase
{
public:
	WindowExpedition();
	virtual ~WindowExpedition();

	CREATE_FUNC(WindowExpedition);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;
	virtual void updateWindow(int nCode, void* pData) override;	
protected:
	void _btnOpen_CallBack(Ref* pRef);
	void _btnChallenge_CallBack(Ref* pRef);

	//��������
	void creareCard();
	//����ʱ
	void timeChange(float f);
	//�Զ���ת����
	void autoTurn();
	//��Ӵ���
	void touchlayer();
	//�����ֻζ�
	void handShake();
	//��תǰ��������
	void turnTwo();
	//��ʾ�������
	void showResult();
	//�Ƴ������������ʾ
	void removeShow(float t);
	
private:
	cocos2d::EventListenerTouchOneByOne* listener;
	//������
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

	//��ʾ�����Ŀ�Ƭ
	Vector<Card* > cards;

	//����ʱ
	int      time;
	//����������
	Layer*   contentLayer;


};

#endif // _WindowExpedition_H_

