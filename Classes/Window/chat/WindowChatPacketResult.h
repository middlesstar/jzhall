#ifndef  _WindowChatPacketResult_H_
#define  _WindowChatPacketResult_H_

#include "Window/WindowBase.h"

struct CMD_ST_ChatGetHongBaoRsp;
struct StHong;

class  WindowChatPacketResult : public WindowBase
{
public:
	WindowChatPacketResult();
	virtual ~WindowChatPacketResult();

	CREATE_FUNC(WindowChatPacketResult);
	virtual void setContent();				                //…Ë÷√UI≤ºæ÷

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;

	void setData(const CMD_ST_ChatGetHongBaoRsp* pData);

protected:
	void _setReocedData(Layout* pLayout,const StHong* pData,int nIndex);
};

#endif // _WindowChatPacketResult_H_

