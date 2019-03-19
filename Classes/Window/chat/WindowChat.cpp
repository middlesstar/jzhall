#include "WindowChat.h"
#include "LogicChat.h"
#include "Custom/DragLayer.h"
#include "WindowChatPacketResult.h"

WindowChat::WindowChat()
	: m_pDragLayer(nullptr)
	, m_pDialogBg(nullptr)
	, m_pClickLayout(nullptr)
	, m_pTextChatContent(nullptr)
	, m_pImgWorld(nullptr)
	, m_pImgPersonal(nullptr)
	, m_pChatItem(nullptr)

	, m_pScrollWorldChat(nullptr)
	, m_pScrollPersonalChat(nullptr)

	, m_fBottomYPosWorld(0)
	, m_fBottomYPosChannel(0)
{
	m_bSwallow = false;
}

WindowChat::~WindowChat()
{
	CC_SAFE_RELEASE_NULL(m_pChatItem);
}

void WindowChat::setContent()
{
	_initChatAssistantNode();
	_initChatDialogNode();

	_setUIStatus(ECS_ASSISTANT);
}

void WindowChat::updateWindow(int nCode, void* pData)
{
	if (nCode == ECS_GETPACKET)
	{
		this->_openPacketResultWindow((CMD_ST_ChatGetHongBaoRsp*)pData);
	}
	else
	{
		CMD_ST_ChatMsgRsp rsp = *((CMD_ST_ChatMsgRsp*)pData);
		_addAChatRecord(&rsp);
	}
	
}

void WindowChat::onExit()
{
	LogicChat::getInstance()->setLastPos(m_pDragLayer->getPosition());
	LogicChat::getInstance()->setChatWindow(nullptr);
	WindowBase::onExit();
}

void WindowChat::_initChatAssistantNode()
{
	ImageView* m_pImgChat = ImageView::create("images/hall/game_hall/gh_chat.png");
	m_pImgChat->setTouchEnabled(true);
	m_pImgChat->setSwallowTouches(false);
	m_pImgChat->addClickEventListener(CC_CALLBACK_1(WindowChat::_onChatAssistantClick, this));
	m_pImgChat->addTouchEventListener(CC_CALLBACK_2(WindowChat::_onChatAssistantTouch, this));

	m_pDragLayer = DragLayer::create();
	m_pDragLayer->setContentSize(m_pImgChat->getContentSize());
	//m_pDragLayer->openDebug();
	m_pDragLayer->setPosition(LogicChat::getInstance()->getLastPos());
	m_pDragLayer->addChild(m_pImgChat);

	this->addChild(m_pDragLayer);
}

void WindowChat::_initChatDialogNode()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZChat.csb");
	CCASSERT(pMainNode, "WindowBank::setContent() layout/JZChat.json load nullptr!");

	m_pNode->addChild(pMainNode);

	m_pDialogBg = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode,"img_chat_bg"));
	m_pClickLayout = dynamic_cast<Layout*>(_seekNodeByName(pMainNode, "panel_click"));
	m_posDialog = m_pDialogBg->getPosition();
	m_pClickLayout->addClickEventListener(CC_CALLBACK_1(WindowChat::_onChatPanelClick,this));

	m_pTextChatContent = dynamic_cast<TextField*>(_seekNodeByName(pMainNode,"input_user"));

	_addButtonClickEvent(pMainNode, "btn_redpacket", CC_CALLBACK_1(WindowChat::_onSendRedPacketClick, this));
	_addButtonClickEvent(pMainNode, "btn_send", CC_CALLBACK_1(WindowChat::_onSendSayClick, this));
	_addButtonClickEvent(pMainNode, "btn_close_message", CC_CALLBACK_1(WindowChat::_onCloseMessageTipClick, this));
	_addButtonClickEvent(pMainNode, "btn_close_frame", [this](Ref* pRef){_setUIStatus(ECS_ASSISTANT); });

	m_pImgWorld = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode,"img_channel_world"));
	m_pImgPersonal = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, "img_channel_personal"));
	m_pImgWorld->addClickEventListener(CC_CALLBACK_1(WindowChat::_onChannelClick,this));
	m_pImgPersonal->addClickEventListener(CC_CALLBACK_1(WindowChat::_onChannelClick, this));
	m_pImgWorld->setTag(ECS_WORLD);
	m_pImgPersonal->setTag(ECS_PERSONAL);

	m_pScrollWorldChat = dynamic_cast<ui::ScrollView*>(_seekNodeByName(pMainNode, "scroll_chat"));
	m_pChatItem = dynamic_cast<Layout*>(m_pScrollWorldChat->getChildByName("panel_item"));
	m_pChatItem->removeFromParent();
	m_pChatItem->retain();
	m_fBottomYPosWorld = m_pScrollWorldChat->getContentSize().height;

	m_pScrollPersonalChat = dynamic_cast<ui::ScrollView*>(m_pScrollWorldChat->clone());
	m_fBottomYPosChannel = m_fBottomYPosWorld;
	m_pScrollWorldChat->getParent()->addChild(m_pScrollPersonalChat);

// 	m_pScrollWorldChat->getInnerContainer()->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
// 	m_pScrollWorldChat->getInnerContainer()->setBackGroundColor(Color3B(255, 0, 0));
// 
// 	m_pScrollPersonalChat->getInnerContainer()->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
// 	m_pScrollPersonalChat->getInnerContainer()->setBackGroundColor(Color3B(0, 0, 255));

	_setChatChannel(ECS_WORLD);
}

void WindowChat::_setUIStatus(int nStatus)
{
	if (nStatus == ECS_ASSISTANT)
	{
		_showAssistant();
		_dismissDialog();
	}
	else if (nStatus == ECS_DIALOG)
	{
		_dismissAssistant();
		_showDialog();
	}
}

void WindowChat::_setChatChannel(int nChannel)
{
	int nWorldPngIndex = nChannel == ECS_WORLD ? 0 : 1;
	int nPersonalPngIndex = nChannel == ECS_PERSONAL ? 0 : 1;

	m_pImgWorld->loadTexture(StringUtils::format("images/hall/chatting/chatting_world%d.png", nWorldPngIndex), TextureResType::PLIST);
	m_pImgPersonal->loadTexture(StringUtils::format("images/hall/chatting/chatting_current%d.png", nPersonalPngIndex), TextureResType::PLIST);

	m_pImgWorld->setTouchEnabled(nChannel != ECS_WORLD);
	m_pImgPersonal->setTouchEnabled(nChannel != ECS_PERSONAL);

	m_pScrollWorldChat->setVisible(nChannel == ECS_WORLD);
	m_pScrollPersonalChat->setVisible(nChannel == ECS_PERSONAL);

	LogicChat* pLogic = LogicChat::getInstance();
	pLogic->setCurChannel(pLogic->getChatChannelType(nChannel == ECS_WORLD));
	m_pTextChatContent->setString("");
}

void WindowChat::_addAChatRecord(const CMD_ST_ChatMsgRsp* pData)
{
	if (!pData)
	{
		return;
	}

	Layout* pItem = pData->ntype == 1 ? _createAChatItem(pData) : _createARedPacketItem(pData);
	_AddChatItem(pData->msgtype, pItem);
}

void WindowChat::_AddChatItem(int nChannel, Layout* pItem)
{
	if (!pItem)
	{
		return;
	}

	ui::ScrollView* pScroll = nChannel == ECC_WORLD ? m_pScrollWorldChat : m_pScrollPersonalChat;
	float* pBottomYPos = nChannel == ECC_WORLD ? &m_fBottomYPosWorld : &m_fBottomYPosChannel;

	float fHeight = pItem->getContentSize().height;
	float fBottomYPos = *pBottomYPos - fHeight;
	if (fBottomYPos >= 0)
	{
		for (auto& child : pScroll->getChildren())
		{
			child->setPositionY(child->getPositionY() - fHeight);
		}

		*pBottomYPos -= fHeight;

		pItem->setPositionY(pScroll->getContentSize().height - fHeight);
	}
	else    //超出了innerContent的height
	{
		Size innersize = pScroll->getInnerContainerSize();
		pScroll->setInnerContainerSize(Size(innersize.width, innersize.height - fBottomYPos));
		if (fabs(fBottomYPos) < fHeight)
		{
			for (auto& child : pScroll->getChildren())
			{
				child->setPositionY(child->getPositionY() - *pBottomYPos);
			}

			pItem->setPositionY(pScroll->getContentSize().height - *pBottomYPos);
			*pBottomYPos = 0;
		}
		else
		{
			pItem->setPositionY(innersize.height - fBottomYPos - fHeight);
		}
	}

	pScroll->addChild(pItem);
	pScroll->scrollToTop(0.3f, false);
	pItem->setScale(0);
	pItem->runAction(ScaleTo::create(0.1f, 1.0f));
}

Layout* WindowChat::_createAChatItem(const CMD_ST_ChatMsgRsp* pData)
{
	if (!pData)
	{
		return nullptr;
	}

	Layout* pItem = dynamic_cast<Layout*>(m_pChatItem->clone());

	_setPlayerCommonInfo(pItem, pData);

 	Text* pText = dynamic_cast<Text*>(_seekNodeByName(pItem, "text_chat"));
	float fHeight = pText->getContentSize().height;

	Label* pRender = dynamic_cast<Label*>(pText->getVirtualRenderer());
	pRender->setDimensions(pText->getContentSize().width, 0);
	pText->setString(pData->content);

	ImageView* pImgMid = dynamic_cast<ImageView*>(_seekNodeByName(pItem, "img_chat_mid"));

	Size textSize = pText->getContentSize();
	Size imgSize = pImgMid->getContentSize();

	float fOverstep = textSize.height - fHeight;
	if (fOverstep > 0)
	{
		pImgMid->setContentSize(Size(imgSize.width, imgSize.height + fOverstep));
		Size layoutSize = pItem->getContentSize();
		pItem->setContentSize(Size(layoutSize.width, layoutSize.height + fOverstep));

		for (auto& child : pItem->getChildren())
		{
			child->setPositionY(child->getPositionY() + fOverstep);
		}
	}

	pItem->removeChildByName("panel_redpacket");

	return pItem;
}

Layout* WindowChat::_createARedPacketItem(const CMD_ST_ChatMsgRsp* pData)
{
	Layout* pItem = dynamic_cast<Layout*>(m_pChatItem->clone());

	if (pItem)
	{
		Layout* pPanelRedPacket = dynamic_cast<Layout*>(pItem->getChildByName("panel_redpacket"));
		ImageView* pImgRedpacket = dynamic_cast<ImageView*>(pPanelRedPacket->getChildByName("img_redpacket"));
		pPanelRedPacket->setVisible(true);
		pImgRedpacket->addClickEventListener(CC_CALLBACK_1(WindowChat::_onRedPacketClick,this));
		pImgRedpacket->setTag(pData->hid);
		pImgRedpacket->setName(pData->uname);

		_setPlayerCommonInfo(pItem,pData);

		Text* pText = dynamic_cast<Text*>(_seekNodeByName(pItem, "text_chat"));
		float fHeight = pText->getContentSize().height;

		pItem->removeChildByName("panel_chat");

		float fOverstep = pImgRedpacket->getContentSize().height - fHeight;
		if (fOverstep > 0)
		{
			Size layoutSize = pItem->getContentSize();
			pItem->setContentSize(Size(layoutSize.width, layoutSize.height + fOverstep));

			for (auto& child : pItem->getChildren())
			{
				child->setPositionY(child->getPositionY() + fOverstep);
			}
		}
	}

	return pItem;
}

void WindowChat::_setPlayerCommonInfo(Layout* pItem, const CMD_ST_ChatMsgRsp* pData)
{
	if (!pItem || !pData)
	{
		return;
	}

	//设置头像
	ImageView* pIcon = dynamic_cast<ImageView*>(_seekNodeByName(pItem,"icon"));
	ImageView* pIconFrame = dynamic_cast<ImageView*>(_seekNodeByName(pItem, "icon_frame"));
	pIcon->loadTexture(StringUtils::format("images/shak/head/head_%d.png", pData->faceid), TextureResType::PLIST);
	pIconFrame->loadTexture(StringUtils::format("images/shak/vip/frame%d.png", pData->viplevel), TextureResType::PLIST);

	//设置VIP
	ImageView* pImgVip = dynamic_cast<ImageView*>(_seekNodeByName(pItem, "img_vip"));
	TextAtlas* pTextVip = dynamic_cast<TextAtlas*>(_seekNodeByName(pImgVip, "text_vip"));
	ImageView* pImgNotVip = dynamic_cast<ImageView*>(_seekNodeByName(pItem, "img_novip"));

	pImgNotVip->setVisible(pData->viplevel == 0);
	pImgVip->setVisible(pData->viplevel > 0);
	pTextVip->setString(StringUtils::format("%d", pData->viplevel));

	//设置名字
	Text* pName = dynamic_cast<Text*>(_seekNodeByName(pItem, "text_name"));
	pName->setString(pData->uname);

	//设置时间
	Text* pTime = dynamic_cast<Text*>(_seekNodeByName(pItem, "text_time"));
	char buf[32] = { 0 };
	struct tm *l = localtime((time_t*)&(pData->sendtime));
	snprintf(buf, sizeof(buf), "%02d:%02d:%02d",l->tm_hour, l->tm_min, l->tm_sec);
	pTime->setString(buf);
}

void WindowChat::_showAssistant()
{
	m_pDragLayer->setVisible(true);
}

void WindowChat::_dismissAssistant()
{
	m_pDragLayer->setVisible(false);
}

void WindowChat::_showDialog()
{
	m_pDialogBg->stopAllActions();
	m_pDialogBg->runAction(MoveTo::create(0.2f, m_posDialog + Vec2(m_pDialogBg->getContentSize().width, 0)));
	m_pClickLayout->setVisible(true);
}

void WindowChat::_dismissDialog()
{
	m_pDialogBg->stopAllActions();
	m_pDialogBg->runAction(MoveTo::create(0.2f, m_posDialog));
	m_pClickLayout->setVisible(false);
}

void WindowChat::_openPacketResultWindow(const CMD_ST_ChatGetHongBaoRsp* pData)
{
	WindowChatPacketResult* pWindow = dynamic_cast<WindowChatPacketResult*>(HallGameScene::createWindow(EWT_PacketResult));

	if (pWindow)
	{
		pWindow->setData(pData);
		this->addChild(pWindow);
	}
}

void WindowChat::_onChatAssistantClick(Ref* pRef)
{
	Widget* pWidget = dynamic_cast<Widget*>(pRef);
	if (pWidget->getTag() == 1)
	{
		pWidget->setTag(0);
		return;
	}

	//log("---------------_onChatAssistantClick----------------");
	_setUIStatus(ECS_DIALOG);
}

void WindowChat::_onChatAssistantTouch(Ref* pRef, Widget::TouchEventType tEvent)
{
	switch (tEvent)
	{
	case Widget::TouchEventType::MOVED:
		{
			Widget* pWidget = dynamic_cast<Widget*>(pRef);
			Vec2 pos = pWidget->getTouchMovePosition() - pWidget->getTouchBeganPosition();
			if (pos.length() > 5.0f)
			{
				pWidget->setTag(1);
			}
		}
		break;
	default:
		break;
	}
}

void WindowChat::_onChatPanelClick(Ref* pRef)
{
	_setUIStatus(ECS_ASSISTANT);
}

void WindowChat::_onSendRedPacketClick(Ref* pRef)
{
	//log("----------_onSendRedPacketClick-----------");

	this->addChild(HallGameScene::createWindow(EWT_SendRedPacket));
}

void WindowChat::_onSendSayClick(Ref* pRef)
{
	//log("----------_onSendSayClick-----------");
	if (!m_pTextChatContent->getString().empty())
	{
		LogicChat::getInstance()->requestSendChatMessage(m_pTextChatContent->getString());
		m_pTextChatContent->setString("");
	}
}

void WindowChat::_onCloseMessageTipClick(Ref* pRef)
{

}

void WindowChat::_onChannelClick(Ref* pRef)
{
	Widget* pWidget = dynamic_cast<Widget*>(pRef);

	if (pWidget)
	{
		_setChatChannel(pWidget->getTag());
	}
}

void WindowChat::_onRedPacketClick(Ref* pRef)
{
	ImageView* pImg = dynamic_cast<ImageView*>(pRef);
	
	if (pImg)
	{
		LogicChat::getInstance()->setRedPacketName(pImg->getName());
		LogicChat::getInstance()->requestGetRedPacket(pImg->getTag());
	}
}