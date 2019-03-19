#include "WindowVipGift.h"
#include "Window/shop/LogicShop.h"

WindowVipGift::WindowVipGift()
{

}

WindowVipGift::~WindowVipGift()
{

}

void WindowVipGift::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZVipGift.csb");
	CCASSERT(pMainNode, "WindowVipGift::setContent() layout/JZVipGift.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();
	
	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_buy", [this](Ref* sender)
	{
		LogicShop::getInstance()->requestOrderId(1002);			//直接购买30元商品，productid为1002
	});
}

void WindowVipGift::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}