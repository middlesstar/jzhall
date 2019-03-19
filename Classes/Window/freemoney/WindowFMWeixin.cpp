#include "WindowFMWeixin.h"
#include "LogicFreeMoney.h"

WindowFMWeixin::WindowFMWeixin()
{

}

WindowFMWeixin::~WindowFMWeixin()
{
}

void WindowFMWeixin::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZFMWeiXin.csb");
	CCASSERT(pMainNode, "WindowFMWeixin::setContent() layout/JZFMWeiXin.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_pInput = dynamic_cast<TextField*>(_seekNodeByName(pMainNode,"input_num"));

	_addButtonClickEvent(pMainNode, "btn_get", [this](Ref* sender)
	{
		LogicFreeMoney::getInstance()->requestWechatPresent(m_pInput->getString());
	});

	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});
}

void WindowFMWeixin::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}