#ifndef  _WindowSendRedPacket_H_
#define  _WindowSendRedPacket_H_

#include "Window/WindowBase.h"

class  WindowSendRedPacket : public WindowBase
{
public:
	WindowSendRedPacket();
	virtual ~WindowSendRedPacket();

	CREATE_FUNC(WindowSendRedPacket);
	virtual void setContent();				                //…Ë÷√UI≤ºæ÷

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;

protected:
	void redPacketItem_Callback(Ref* pRef);
	void sendBtn_Callback(Ref* pRef);
protected:
	ImageView* m_pImgSelect;
};

#endif // _WindowSendRedPacket_H_

