#include "WindowPersonal.h"
#include "Player/Player.h"
#include "LanguageConfig.h"

WindowPersonal::WindowPersonal()
	: m_pSelectSprite(nullptr)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_NickName, WindowPersonal::refreshNickName, nullptr);
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Ticket, WindowPersonal::refreshTicket, nullptr);
}

WindowPersonal::~WindowPersonal()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_NickName);
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Ticket);
}

void WindowPersonal::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZPersonal.csb");
	CCASSERT(pMainNode, "WindowModifyIcon::setContent() layout/JZPersonal.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_pNodePersonal = _seekNodeByName(pMainNode, "node_personal");
	m_pNodeModify = _seekNodeByName(pMainNode, "node_modify");
	m_pNodeBind = _seekNodeByName(pMainNode, "node_bind");

	m_pSpritePersonal = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "img_title_personal"));
	m_pSpriteModify = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "img_title_modify"));
	m_pSelectSprite = dynamic_cast<Sprite*>(_seekNodeByName(m_pNodeModify, "img_select"));

	ImageView* pImgIcon = dynamic_cast<ImageView*>(_seekNodeByName(m_pNodePersonal, "img_icon"));
	pImgIcon->loadTexture(StringUtils::format("images/common/head/head_%d.png", Player::getInstance()->getLogonData().wFaceID), TextureResType::PLIST);

	_setPersonalInfo();
	_setUIState(EWPS_PERSONAL);

	_setPersonalNode();
	_setModifyNode();
	_setBindNode();

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});
}

void WindowPersonal::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowPersonal::refreshNickName(Ref* ref)
{
	Text* pNickName = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_nickname"));
	pNickName->setString(Player::getInstance()->getLogonData().szNickName);
}

void WindowPersonal::refreshTicket(Ref* ref)
{
	Text* pTicket = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_ticket"));
	pTicket->setString(StringUtils::format("%d", Player::getInstance()->getHallData().dwTicket));
}

void WindowPersonal::_setPersonalInfo()
{
	Text* pId = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal,"text_id"));
	Text* pGold = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_gold"));
	Text* pNickName = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_nickname"));
	Text* pTitle = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_title"));
	Text* pCoin = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_coin"));
	Text* pLevel = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_vip"));
	Text* pTicket = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_ticket"));
	Text* pExp = dynamic_cast<Text*>(_seekNodeByName(m_pNodePersonal, "text_exp"));

	Player* pPlayer = Player::getInstance();

	pId->setString(StringUtils::format("%d", pPlayer->getLogonData().dwGameID));
	pGold->setString(StringUtils::format("%d", pPlayer->getHallData().llYuanBao));
	pNickName->setString(pPlayer->getLogonData().szNickName);
	pTitle->setString(GETSTRING(pPlayer->getPlayerLevel() / 10 + 70));
	pCoin->setString(StringUtils::format("%d", pPlayer->getLogonData().lUserScore));
	pLevel->setString(StringUtils::format("%d", pPlayer->getPlayerLevel()));
	pTicket->setString(StringUtils::format("%d", pPlayer->getHallData().dwTicket));
	pExp->setString(StringUtils::format("%d", pPlayer->getHallData().dwVipLevel));

// 	TextField* pInoutName = dynamic_cast<TextField*>(_seekNodeByName(m_pNodePersonal,"input_nickname"));
// 	pInoutName->setPlaceHolder(pPlayer->getLogonData().szNickName);

	Button* pGiveTicket = dynamic_cast<Button*>(_seekNodeByName(m_pNodePersonal, "btn_send"));
	pGiveTicket->addClickEventListener([this](Ref* pRef)
	{
		HallGameScene::getInstance()->openWindow(EWT_TicketSend);
		playButtonClickSound();
	});

	_addButtonClickEvent(m_pNodePersonal, "btn_setting", [this](Ref* pRef)
	{
		_setUIState(EWPS_BIND);
	});

	_addButtonClickEvent(m_pNodePersonal, "btn_modifypwd", [this](Ref* pRef)
	{
		_setUIState(EWPS_BIND);
	});

	_addButtonClickEvent(m_pNodePersonal, "btn_vip", [this](Ref* pRef)
	{
		HallGameScene::getInstance()->openWindow(EWT_Shop);
		HallGameScene::getInstance()->updateWindow(EWT_Shop);
	});

	Node* pNodeModifyAccount = _seekNodeByName(m_pNodePersonal, "node_modifyAccount");
	Node* pNodeModifyPwd = _seekNodeByName(m_pNodePersonal, "node_modifyPwd");
	pNodeModifyAccount->setVisible(pPlayer->isVisitor());
	pNodeModifyPwd->setVisible(!pPlayer->isVisitor());
}

void WindowPersonal::_setUIState(int nState)
{
	m_pNodePersonal->setVisible(nState == EWPS_PERSONAL);
	m_pNodeModify->setVisible(nState == EWPS_MODIFY);
	m_pNodeBind->setVisible(nState == EWPS_BIND);
	
	if (nState == EWPS_MODIFY)
	{
		_setModifyNode();
	}
}
void WindowPersonal::_setPersonalNode()
{
	_addButtonClickEvent(m_pNodePersonal, "btn_modify", [this](Ref* sender)
	{
		this->_setUIState(EWPS_MODIFY);
	});

	_addButtonClickEvent(m_pNodePersonal, "btn_modify_nickname", [this](Ref* sender)
	{
		//修改昵称请求
		TextField* pInput = dynamic_cast<TextField*>(_seekNodeByName(m_pNodePersonal,"input_nickname"));
		if (pInput)
		{
			Player::getInstance()->requestModifyNickname(pInput->getString());
		}
	});

	_addButtonClickEvent(m_pNodePersonal, "btn_bind", [this](Ref* sender)
	{
		this->_setUIState(EWPS_BIND);
	});
}

void WindowPersonal::_setModifyNode()
{
	//设置修改头像部分
	Button* pBtn = nullptr;
	int nFaceId = Player::getInstance()->getLogonData().wFaceID;
	for (int i = 1; i <= 10; i++)
	{
		pBtn = dynamic_cast<Button*>(_seekNodeByName(m_pNodeModify, __String::createWithFormat("icon%d", i)->getCString()));
		if (pBtn && m_pSelectSprite)
		{
			if (pBtn->getTag() - 1 == nFaceId)
			{
				m_pSelectSprite->setPosition(pBtn->getPosition());
			}

			pBtn->addClickEventListener([this](Ref* sender)
			{
				Button* pBtn = dynamic_cast<Button*>(sender);
				if (pBtn && m_pSelectSprite)
				{
					m_pSelectSprite->setPosition(pBtn->getPosition());
					m_pSelectSprite->setTag(pBtn->getTag() - 1);
				}
				playButtonClickSound();
			});
		}
	}

	_addButtonClickEvent(m_pNodeModify, "btn_cancel", [this](Ref* sender)
	{
		this->_setUIState(EWPS_PERSONAL);
	});

	_addButtonClickEvent(m_pNodeModify, "btn_ok", [this](Ref* sender)
	{
		Player::getInstance()->requestModifyIcon(m_pSelectSprite->getTag());
		this->close();
	});
}

void WindowPersonal::_setBindNode()
{
	Text* pTextNickName = dynamic_cast<Text*>(_seekNodeByName(m_pNodeBind,"text_nickname"));
	ImageView* pImgTip = dynamic_cast<ImageView*>(_seekNodeByName(m_pNodeBind, "img_tip"));
	Sprite* pSpriteModifyName = dynamic_cast<Sprite*>(_seekNodeByName(m_pNodeBind, "sprite_input_bg_name"));

	Player* pPlayer = Player::getInstance();

	pTextNickName->setVisible(!pPlayer->isVisitor());
	pSpriteModifyName->setVisible(pPlayer->isVisitor());
	pTextNickName->setString(pPlayer->getLogonData().szNickName);
	pPlayer->isVisitor() ? pImgTip->loadTexture("images/hall/personal/personal_text.png", TextureResType::PLIST)
						 : pImgTip->loadTexture("images/hall/personal/personal_text1.png", TextureResType::PLIST);
	
	m_pTextAccount = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeBind, "input_name"));
	m_pTextPassword = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeBind, "input_pwd"));
	m_pTextPasswordConfirm = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeBind, "input_pwd_1"));

	_addButtonClickEvent(m_pNodeBind, "btn_ok", [this](Ref* sender)
	{
		//确定修改昵称或者密码
		if (Player::getInstance()->isVisitor() && m_pTextAccount->getString().empty())
		{
			HallGameScene::getInstance()->openMessageTip(37);
			return;
		}

		if (m_pTextAccount->getString().length() < 6 || m_pTextAccount->getString().length() > 15)
		{
			HallGameScene::getInstance()->openMessageTip(41);
			return;
		}

		if (m_pTextPassword->getString().empty())
		{
			HallGameScene::getInstance()->openMessageTip(39);
			return;
		}

		if (m_pTextPassword->getString().length() < 6 || m_pTextPassword->getString().length() > 15)
		{
			HallGameScene::getInstance()->openMessageTip(42);
			return;
		}

		if (m_pTextPassword->getString() != m_pTextPasswordConfirm->getString())
		{
			HallGameScene::getInstance()->openMessageTip(36);
			return;
		}

		Player::getInstance()->requestModifyAccount(m_pTextAccount->getString(), m_pTextPassword->getString());
	});
}
