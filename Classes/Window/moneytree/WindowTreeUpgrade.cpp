#include "WindowTreeUpgrade.h"

WindowTreeUpgrade::WindowTreeUpgrade()
{
}

WindowTreeUpgrade::~WindowTreeUpgrade()
{
}

void WindowTreeUpgrade::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZMoneyTreeUPgrade.csb");
	CCASSERT(pMainNode, "WindowTreeUpgrade::setContent() layout/JZMoneyTreeUPgrade.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		/* -----------关闭 按钮------------- */
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_buy", [this](Ref* sender)
	{
		HallGameScene::getInstance()->openWindow(EWT_Shop);
		HallGameScene::getInstance()->updateWindow(EWT_Shop);
	});
}

void WindowTreeUpgrade::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}