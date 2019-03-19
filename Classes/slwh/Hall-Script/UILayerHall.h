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
	Button*				m_roomRabbit;	//�÷�
	TextAtlas*			m_limitRabbit;	//���ӷ���
	Button*				m_roomPanda;	//è��
	TextAtlas*			m_limitPanda;	//è����
	Button*				m_roomLion;		//ʨ��
	TextAtlas*			m_limitLion;	//ʨ����
	Button*				m_recharge;		//��ֵ��ť
	Button*				m_setting;		//���ð�ť
	Button*				m_regular;		//����ť
	Button*				m_exit;			//�˳�
	TextAtlas*			m_goldNum;		//���
	TextAtlas*			m_yuanbaoNum;	//Ԫ��


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
	//���뷿��
	void goRoom(int roomIndex);
	//ˢ����
	void refreshNum(TextAtlas* text,SCORE num);
	//exit
	void ExitToHall();
public:
	//Scene* mainScene;
	//ˢ�½��
	void updateSlwhCoin(Ref* ref);

};


#endif


