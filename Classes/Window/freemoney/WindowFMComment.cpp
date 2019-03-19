#include "WindowFMComment.h"
#include "LogicFreeMoney.h"
#include "Device/Device.h"

WindowFMComment::WindowFMComment()
{

}

WindowFMComment::~WindowFMComment()
{
}

void WindowFMComment::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZFMComment.csb");
	CCASSERT(pMainNode, "WindowFMComment::setContent() layout/JZFMComment.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_cancel", [this](Ref* sender)
	{
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_ok", [this](Ref* sender)
	{
		LogicFreeMoney::getInstance()->requestComment();

		//跳转到ios商城
        JZ::Device::openCommentUrl();

		this->close();
	});
}

void WindowFMComment::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}