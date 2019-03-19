#include "WindowChatPacketResult.h"
#include "LogicChat.h"
#include "Custom/DragLayer.h"
#include "LanguageConfig.h"
#include "Player/Player.h"

WindowChatPacketResult::WindowChatPacketResult()
{
}

WindowChatPacketResult::~WindowChatPacketResult()
{
}

void WindowChatPacketResult::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZChatPacketResult.csb");
	CCASSERT(pMainNode, "WindowHall::setContent() layout/JZChatPacketResult.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* pRef){this->close(); });
	_addButtonClickEvent(pMainNode, "btn_ok", [this](Ref* pRef){this->close(); });
}

void WindowChatPacketResult::onEnter()
{
	WindowBase::onEnter();

	setKeyboardEnabled(true);
	HallGameScene::setHandleKeyboard(false);

	ScaleIn();
}

void WindowChatPacketResult::onExit()
{
	setKeyboardEnabled(false);
	HallGameScene::setHandleKeyboard(true);

	WindowBase::onExit();
}

void WindowChatPacketResult::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		this->close();
	}
}

void WindowChatPacketResult::setData(const CMD_ST_ChatGetHongBaoRsp* pData)
{
	if (!pData)
	{
		return;
	}

	//设置自己的图标和名字
	ImageView* pImgIcon = dynamic_cast<ImageView*>(_seekNodeByName(m_pNode,"icon"));
	pImgIcon->loadTexture(StringUtils::format("images/shak/head/head_%d.png", Player::getInstance()->getLogonData().wFaceID), TextureResType::PLIST);

	ImageView* pImgIconFrame = dynamic_cast<ImageView*>(_seekNodeByName(m_pNode, "icon_frame"));
	pImgIconFrame->loadTexture(StringUtils::format("images/shak/vip/frame%d.png", Player::getInstance()->getHallData().dwVipLevel), TextureResType::PLIST);

	Text* pTextMyName = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "text_self_name"));
	pTextMyName->setString(Player::getInstance()->getLogonData().szNickName);

	//设置自己抢到多少分
	if (pData->ret == 3)
	{
		Text* pTextScore = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "text_score_tip"));
		Text* pTextTip = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "text_over_tip"));

		pTextScore->setVisible(false);
		pTextTip->setVisible(true);
	}
	else
	{
		Text* pTextScore = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "text_score_tip"));
		Text* pTextScoreTip = dynamic_cast<Text*>(_seekNodeByName(pTextScore, "text_tip"));
		RichText* pText = RichText::create();
		RichElement* pElem1 = RichElementText::create(0, Color3B(0,255,0), 255, StringUtils::format("%d", pData->gold), pTextScore->getFontName(), pTextScore->getFontSize());
		RichElement* pElem2 = RichElementText::create(0, Color3B(0, 255, 0), 255, pTextScoreTip->getString(), pTextScoreTip->getFontName(), pTextScoreTip->getFontSize());
		pText->insertElement(pElem1, 0);
		pText->insertElement(pElem2, 1);
		pText->setAnchorPoint(pTextScore->getAnchorPoint());
		pText->setPosition(pTextScore->getPosition());

		pTextScore->getParent()->addChild(pText);
		pTextScore->removeFromParent();
	}

	Text* pRedName = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "text_red_name"));
	RichElement* pOwnerElem1 = RichElementText::create(0, Color3B(0, 255, 0), 255, LogicChat::getInstance()->getRedPacketName(), pRedName->getFontName(), pRedName->getFontSize());
	RichElement* pOwnerElem2 = RichElementText::create(0, Color3B(255, 255, 255), 255, pRedName->getString(), pRedName->getFontName(), pRedName->getFontSize());
	RichText* pRichRedName = RichText::create();
	pRichRedName->insertElement(pOwnerElem1, 0);
	pRichRedName->insertElement(pOwnerElem2, 1);
	pRichRedName->setAnchorPoint(pRedName->getAnchorPoint());
	pRichRedName->setPosition(pRedName->getPosition());
	pRedName->getParent()->addChild(pRichRedName);
	pRedName->removeFromParent();
	

	//设置8个抢红包的玩家信息
	Text* pText = nullptr;
	int nTextId = 1;

	Layout* pLayout = dynamic_cast<Layout*>(_seekNodeByName(m_pNode,"panel_record"));
	Layout* pItem = pLayout;
	for (int i = 0; i < countarray(pData->hlist) && nTextId < 8; i++)
	{
		if (pData->hlist[i].userid > 0 && pData->hlist[i].gold > 0)
		{
			if (i > 0)
			{
				pItem = dynamic_cast<Layout*>(pLayout->clone());
				pLayout->getParent()->addChild(pItem);
			}

			_setReocedData(pItem, &pData->hlist[i],i);
		}
	}
}

void WindowChatPacketResult::_setReocedData(Layout* pLayout, const StHong* pData, int nIndex)
{
	if (!pLayout || !pData)
	{
		return;
	}

	Text* pTextNum = dynamic_cast<Text*>(_seekNodeByName(pLayout,"text_num"));
	pTextNum->setString(StringUtils::format("%d", nIndex + 1));

	//设置VIP
	ImageView* pImgVip = dynamic_cast<ImageView*>(_seekNodeByName(pLayout, "img_vip"));
	TextAtlas* pTextVip = dynamic_cast<TextAtlas*>(_seekNodeByName(pLayout, "text_vip"));
	ImageView* pImgNotVip = dynamic_cast<ImageView*>(_seekNodeByName(pLayout, "img_novip"));

	pImgNotVip->setVisible(pData->viplevel == 0);
	pImgVip->setVisible(pData->viplevel > 0);
	pTextVip->setString(StringUtils::format("%d", pData->viplevel));

	Text* pTextName = dynamic_cast<Text*>(_seekNodeByName(pLayout, "text_name"));
	pTextName->setString(pData->uname);

	Text* pTextCoin = dynamic_cast<Text*>(_seekNodeByName(pLayout, "text_coin"));
	pTextCoin->setString(StringUtils::format("%d", pData->gold));
	
	//设置位置
	const float fOffsetX = 350.0f;
	const float fOffsetY = 40.0f;
	const int nRowNum = 4;
	if (nIndex > 0)
	{
		pLayout->setPositionX(pLayout->getPositionX() + nIndex / nRowNum * fOffsetX);
		pLayout->setPositionY(pLayout->getPositionY() - nIndex % nRowNum * fOffsetY);
	}
}