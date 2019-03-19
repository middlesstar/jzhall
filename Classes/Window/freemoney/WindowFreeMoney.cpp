#include "WindowFreeMoney.h"
#include "LogicFreeMoney.h"

WindowFreeMoney::WindowFreeMoney()
{

}

WindowFreeMoney::~WindowFreeMoney()
{

}

void WindowFreeMoney::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZFreeMoney.csb");
	CCASSERT(pMainNode, "WindowFreeMoney::setContent() layout/JZFreeMoney.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		/* -----------关闭 按钮------------- */
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_promote", [this](Ref* sender)
	{
		/* -----------分享好礼 按钮------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_Promote);
	});

	_addButtonClickEvent(pMainNode, "btn_sign", [this](Ref* sender)
	{
		/* -----------每日签到 按钮------------- */
		this->close();
		LogicFreeMoney::getInstance()->requestDailySin(3);
		//HallGameScene::getInstance()->openWindow(EWT_Sign);
	});

	_addButtonClickEvent(pMainNode, "btn_dialyprize", [this](Ref* sender)
	{
		/* -----------每日抽奖 按钮------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_DialyPrize);
	});

	_addButtonClickEvent(pMainNode, "btn_moneytree", [this](Ref* sender)
	{
		/* -----------摇钱树 按钮------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_MoneyTree);
	});

	_addButtonClickEvent(pMainNode, "btn_jiujijin", [this](Ref* sender)
	{
		/* -----------领取救济金 按钮------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_Dole);
	});

	_addButtonClickEvent(pMainNode, "btn_weixin", [this](Ref* sender)
	{
		/* -----------微信公众号 按钮------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_WeiXin);
	});
}

void WindowFreeMoney::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}