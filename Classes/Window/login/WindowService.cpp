#include "WindowService.h"

WindowService::WindowService()
{

}

WindowService::~WindowService()
{

}

void WindowService::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZService.csb");
	CCASSERT(pMainNode, "WindowService::setContent() layout/JZService.csb load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});
}