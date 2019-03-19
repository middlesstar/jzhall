#include "WindowTicketSend.h"
#include "Player/Player.h"
#include "Util/GameUtil.h"

#define LEAST_SEND_NUM		24
#define MOST_SEND_NUM		(24 * 10)

WindowTicketSend::WindowTicketSend()
	: m_pTextSendNum(nullptr)
	, m_pIDInput(nullptr)
	, m_nSendNum(LEAST_SEND_NUM)
{
}

WindowTicketSend::~WindowTicketSend()
{
}

void WindowTicketSend::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZTicketSend.csb");
	CCASSERT(pMainNode, "WindowModifyIcon::setContent() layout/JZTicketSend.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_pTextSendNum = dynamic_cast<Text*>(_seekNodeByName(pMainNode,"text_num"));
	m_pTextSendNum->setString(StringUtils::format("%d", m_nSendNum));
	m_pIDInput = dynamic_cast<TextField*>(_seekNodeByName(pMainNode, "input_user"));

	Node* pNode = pMainNode->getChildByName("node_parent");
	_addButtonClickEvent(pNode, "btn_minus", [this](Ref* sender)
	{
		this->_subOneUnit();
	});

	_addButtonClickEvent(pNode, "btn_plus", [this](Ref* sender)
	{
		this->_addOneUnit();
	});

	_addButtonClickEvent(pNode, "btn_ok", [this](Ref* sender)
	{
		//请求赠送
		if (!GameUtil::isNum(m_pIDInput->getString()))
		{
			HallGameScene::getInstance()->openMessageTip(19);
			m_pIDInput->setString("");
		}
		else
		{
			Player::getInstance()->requestSendTicket(atoi(m_pIDInput->getString().c_str()), m_nSendNum);
			this->close();
		}
	});

	//设置按钮事件
	_addButtonClickEvent(pNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});
}

void WindowTicketSend::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowTicketSend::_addOneUnit()
{
	m_nSendNum += LEAST_SEND_NUM;
	m_nSendNum = MIN(m_nSendNum, MOST_SEND_NUM);
	m_pTextSendNum->setString(StringUtils::format("%d", m_nSendNum));
}

void WindowTicketSend::_subOneUnit()
{
	m_nSendNum -= LEAST_SEND_NUM;
	m_nSendNum = MAX(m_nSendNum, 0);
	m_pTextSendNum->setString(StringUtils::format("%d", m_nSendNum));
}