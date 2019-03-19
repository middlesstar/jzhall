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
	//进入房间
	void goRoom(int roomIndex);
	void BtEvent(Ref *pSender, TouchEventType type);

	void setTime(float t);

	void setTime();

	void goHall();                     //返回大厅

	void connectGame(Ref* ref);                //登陆游戏服务器

	//充值宝箱动画
	void moveRecharge();
public:
	//设置金币数和元宝数
	void setScore();

	//刷新金币
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
	bool            selectFlag;                 //房间点击标志位

	word							serverPort;						//房间端口
	char							serverName[LEN_SERVER];			//房间名称


};

#endif // _SelectRoom_H_
