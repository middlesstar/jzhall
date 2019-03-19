#ifndef  _WindowChat_H_
#define  _WindowChat_H_

#include "Window/WindowBase.h"

class DragLayer;
struct CMD_ST_ChatMsgRsp;
struct CMD_ST_ChatGetHongBaoRsp;

enum eChatStatus
{
	ECS_WORLD = 0,			//世界频道
	ECS_PERSONAL = 1,		//当前频道


	ECS_ASSISTANT	=	10,		//助手状态
	ECS_DIALOG		=	11,		//聊天框状态

	ECS_GETPACKET	=	20,		//领取红包返回结果
};

class  WindowChat : public WindowBase
{
public:
	WindowChat();
	virtual ~WindowChat();

	CREATE_FUNC(WindowChat);
	virtual void setContent();				                //设置UI布局
	virtual void updateWindow(int nCode, void* pData) override;

	virtual void onExit() override;
protected:
	void _initChatAssistantNode();
	void _initChatDialogNode();
	void _setUIStatus(int nStatus);
	void _setChatChannel(int nChannel);

	void _addAChatRecord(const CMD_ST_ChatMsgRsp* pData);							//添加一个聊天气泡(包含聊天和红包)
	void _AddChatItem(int nChannel, Layout* pItem);

	Layout* _createAChatItem(const CMD_ST_ChatMsgRsp* pData);
	Layout* _createARedPacketItem(const CMD_ST_ChatMsgRsp* pData);
	void _setPlayerCommonInfo(Layout* pItem,const CMD_ST_ChatMsgRsp* pData);

	void _showAssistant();
	void _dismissAssistant();
	void _showDialog();
	void _dismissDialog();

	void _openPacketResultWindow(const CMD_ST_ChatGetHongBaoRsp* pData);

	void _onChatAssistantClick(Ref* pRef);
	void _onChatAssistantTouch(Ref* pRef, Widget::TouchEventType tEvent);
	void _onChatPanelClick(Ref* pRef);

	void _onSendRedPacketClick(Ref* pRef);
	void _onSendSayClick(Ref* pRef);
	void _onCloseMessageTipClick(Ref* pRef);
	void _onChannelClick(Ref* pRef);				//切换聊天频道

	void _onRedPacketClick(Ref* pRef);				//抢红包
protected:
	DragLayer* m_pDragLayer;

	ImageView* m_pDialogBg;
	Layout* m_pClickLayout;
	Vec2 m_posDialog;

	TextField* m_pTextChatContent;
	ImageView* m_pImgWorld;
	ImageView* m_pImgPersonal;

	ui::ScrollView* m_pScrollWorldChat;			//世界聊天
	ui::ScrollView* m_pScrollPersonalChat;		//当前频道聊天
	Layout* m_pChatItem;

	float m_fBottomYPos;
	float m_fBottomYPosWorld;
	float m_fBottomYPosChannel;
};

#endif // _WindowChat_H_

