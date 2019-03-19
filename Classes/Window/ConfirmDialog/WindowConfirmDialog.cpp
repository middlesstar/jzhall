#include "WindowConfirmDialog.h"
#include "ConfirmDialogDelegate.h"

WindowConfirmDialog::WindowConfirmDialog()
	: m_pNodeOk(nullptr)
	, m_pNodeOkCancel(nullptr)
	, m_pNodeVip(nullptr)
	, m_pDelegate(nullptr)
{

}

WindowConfirmDialog::~WindowConfirmDialog()
{

}

WindowConfirmDialog* WindowConfirmDialog::create(const std::string& strText)
{
	WindowConfirmDialog* pRet = new (std::nothrow) WindowConfirmDialog;
	if (pRet && pRet->init(strText))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool WindowConfirmDialog::init(const std::string& strText)
{
	if (!Layer::init())
	{
		return false;
	}

	m_pNode = Node::create();
	this->addChild(m_pNode);
	Size size = Director::getInstance()->getWinSize();
	m_pNode->setPosition(Vec2(size.width / 2, size.height / 2));

	setContent(strText);

	return true;
}

void WindowConfirmDialog::setContent(const std::string& strText)
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZDialog.csb");
	CCASSERT(pMainNode, "WindowConfirmDialog::setContent() layout/JZDialog.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_pNodeOk = _seekNodeByName(pMainNode, "node_1btn");
	m_pNodeOkCancel = _seekNodeByName(pMainNode, "node_2btn");
	m_pNodeVip = _seekNodeByName(pMainNode, "node_3btn");
	Text* pText = dynamic_cast<Text*>(_seekNodeByName(pMainNode, "label_tip"));
	pText->setString(strText);

	//设置按钮事件
	_addButtonClickEvent(m_pNodeOk, "btn_ok", [this](Ref* sender)
	{
		this->close();

		if (m_pDelegate)
		{
			m_pDelegate->onOkClick(this->getTag(), this->getUserData());
		}
	});

	_addButtonClickEvent(m_pNodeOkCancel, "btn_ok", [this](Ref* sender)
	{
		this->close();

		if (m_pDelegate)
		{
			m_pDelegate->onOkClick(this->getTag(), this->getUserData());
		}
	});

	_addButtonClickEvent(m_pNodeOkCancel, "btn_cancel", [this](Ref* sender)
	{
		this->close();

		if (m_pDelegate)
		{
			m_pDelegate->onCancelClick(this->getTag(), this->getUserData());
		}
	});

	_addButtonClickEvent(m_pNodeVip, "btn_ok", [this](Ref* sender)
	{
		this->close();

		if (m_pDelegate)
		{
			m_pDelegate->onCancelClick(this->getTag(), this->getUserData());
		}
	});

	_addButtonClickEvent(m_pNodeVip, "btn_vip", [this](Ref* sender)
	{
		//跳转到VIP界面
		HallGameScene::getInstance()->openWindow(EWT_Shop);
		HallGameScene::getInstance()->updateWindow(EWT_Shop);

		this->close();
	});


}

void WindowConfirmDialog::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowConfirmDialog::setDialogType(int nType)
{
	switch (nType)
	{
	case EWDT_OK:
		m_pNodeOk->setVisible(true);
		m_pNodeOkCancel->setVisible(false);
		m_pNodeVip->setVisible(false);
		break;
	case EWDT_OK_CANCEL:
		m_pNodeOk->setVisible(false);
		m_pNodeOkCancel->setVisible(true);
		m_pNodeVip->setVisible(false);
		break;
	case EWDT_OK_VIP:
		m_pNodeOk->setVisible(false);
		m_pNodeOkCancel->setVisible(false);
		m_pNodeVip->setVisible(true);
		break;
	default:
		m_pNodeOk->setVisible(false);
		m_pNodeOkCancel->setVisible(true);
		break;
	}
}