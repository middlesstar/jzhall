#include "WindowBank.h"
#include "Player/Player.h"
#include "Window/UIHallHelper.h"

WindowBank::WindowBank()
	: m_pSaveInput(nullptr)
	, m_pDrawInput(nullptr)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin, WindowBank::refreshCoin, nullptr);
}

WindowBank::~WindowBank()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);
}

void WindowBank::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZBank.csb");
	CCASSERT(pMainNode, "WindowBank::setContent() layout/JZBank.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	TextField* pSave = dynamic_cast<TextField*>(_seekNodeByName(pMainNode, "input_save"));
	TextField* pDraw = dynamic_cast<TextField*>(_seekNodeByName(pMainNode, "input_draw"));

	m_pSaveInput = UIHallHelper::createAccountEditBox(pSave, 16, "images/hall/bank/bank_input.png");
	m_pDrawInput = UIHallHelper::createAccountEditBox(pDraw, 16, "images/hall/bank/bank_input.png");

	_setData();
	_setEvent();
}

void WindowBank::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowBank::refreshCoin(Ref* ref)
{
	_setData();
}

void WindowBank::_setData()
{
	Text* pTextBank = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "card_num"));
	Text* pTextWallet = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "coin_num"));
	pTextBank->setString(StringUtils::format("%d", Player::getInstance()->getLogonData().lUserInsure));
	pTextWallet->setString(StringUtils::format("%d", Player::getInstance()->getLogonData().lUserScore));
}

void WindowBank::_setEvent()
{
	//设置按钮事件
	_addButtonClickEvent(m_pNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	_addButtonClickEvent(m_pNode, "btn_save", [this](Ref* sender)
	{
		if (m_pSaveInput)
		{
			Player::getInstance()->requestSaveCoinToBank(atoll(m_pSaveInput->getText()));
		}
	});

	_addButtonClickEvent(m_pNode, "btn_draw", [this](Ref* sender)
	{
		if (m_pDrawInput)
		{
			Player::getInstance()->requestTakeCoinFromBank(atoll(m_pDrawInput->getText()));
		}
	});
}