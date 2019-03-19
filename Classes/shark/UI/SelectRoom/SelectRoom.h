#ifndef _SelectRoom_H_
#define _SelectRoom_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "shark/resourceDef.h"

USING_NS_CC;
using namespace ui;

class SelectRoom : public Layer
{
public:

	SelectRoom();
	~SelectRoom();

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SelectRoom);

	
protected:

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
	//���뷿��
	void goRoom(int roomIndex);
	void BtEvent(Ref *pSender, TouchEventType type);

	void setTime(float t);

	void setTime();

	void goHall();                     //���ش���

	void connectGame(Ref* ref);                //��½��Ϸ������

	//��ֵ���䶯��
	void moveRecharge();
public:
	//���ý������Ԫ����
	void setScore();

	//ˢ�½��
	void updateSharkCoin(Ref* ref);


protected:
	Node*           pMainNode;
	Button*         m_roomSwallow;
	Button*         m_roomLion;
	Button*         m_roomHawk;

	Button*         recharge;
	Button*         rule;
	Button*         setting;
	Button*         back;

	TextAtlas*      goldLabel;
	TextAtlas*      ingotLabel;

	ImageView*      rechargeLight;
	bool            selectFlag;                 //��������־λ

	word							serverPort;						//����˿�
	char							serverName[LEN_SERVER];			//��������


};

#endif // _SelectRoom_H_
