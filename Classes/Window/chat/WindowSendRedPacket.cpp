#include "WindowSendRedPacket.h"
#include "LogicChat.h"
#include "Custom/DragLayer.h"

WindowSendRedPacket::WindowSendRedPacket()
{
}

WindowSendRedPacket::~WindowSendRedPacket()
{
}

void WindowSendRedPacket::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZChatSendRedPacket.csb");
	CCASSERT(pMainNode, "WindowHall::setContent() layout/JZChatSendRedPacket.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_pImgSelect = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode,"img_select"));

	ImageView* pImg = nullptr;
	for (int i = 1; i <= 4; i++)
	{
		pImg = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, StringUtils::format("img_red%d",i)));
		pImg->addClickEventListener(CC_CALLBACK_1(WindowSendRedPacket::redPacketItem_Callback,this));
	}

	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* pRef){this->close(); });
	_addButtonClickEvent(pMainNode, "btn_send", CC_CALLBACK_1(WindowSendRedPacket::sendBtn_Callback, this));
}

void WindowSendRedPacket::onEnter()
{
	WindowBase::onEnter();
	
	setKeyboardEnabled(true);
	HallGameScene::setHandleKeyboard(false);

	ScaleIn();
}

void WindowSendRedPacket::onExit()
{
	setKeyboardEnabled(false);
	HallGameScene::setHandleKeyboard(true);

	WindowBase::onExit();
}


void WindowSendRedPacket::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		this->close();
	}
}

void WindowSendRedPacket::redPacketItem_Callback(Ref* pRef)
{
	ImageView* pImgItem = dynamic_cast<ImageView*>(pRef);

	if (pImgItem && m_pImgSelect)
	{
		m_pImgSelect->setPosition(pImgItem->getPosition());
		m_pImgSelect->setTag(pImgItem->getTag());
	}

	playButtonClickSound();
}

void WindowSendRedPacket::sendBtn_Callback(Ref* pRef)
{
	if (m_pImgSelect)
	{
		LogicChat::getInstance()->requestSendRedPacket(m_pImgSelect->getTag());
		this->close();
	}
}