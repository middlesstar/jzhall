#include "WindowFMDole.h"
#include "LogicFreeMoney.h"

WindowFMDole::WindowFMDole()
{

}

WindowFMDole::~WindowFMDole()
{
}

void WindowFMDole::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZFMDole.csb");
	CCASSERT(pMainNode, "WindowFMDole::setContent() layout/JZFMDole.csb load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_get", [this](Ref* sender)
	{
		LogicFreeMoney::getInstance()->requestDole();
	});
	
}

void WindowFMDole::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}