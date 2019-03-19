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

	//���ð�ť�¼�
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		/* -----------�ر� ��ť------------- */
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_promote", [this](Ref* sender)
	{
		/* -----------������� ��ť------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_Promote);
	});

	_addButtonClickEvent(pMainNode, "btn_sign", [this](Ref* sender)
	{
		/* -----------ÿ��ǩ�� ��ť------------- */
		this->close();
		LogicFreeMoney::getInstance()->requestDailySin(3);
		//HallGameScene::getInstance()->openWindow(EWT_Sign);
	});

	_addButtonClickEvent(pMainNode, "btn_dialyprize", [this](Ref* sender)
	{
		/* -----------ÿ�ճ齱 ��ť------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_DialyPrize);
	});

	_addButtonClickEvent(pMainNode, "btn_moneytree", [this](Ref* sender)
	{
		/* -----------ҡǮ�� ��ť------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_MoneyTree);
	});

	_addButtonClickEvent(pMainNode, "btn_jiujijin", [this](Ref* sender)
	{
		/* -----------��ȡ�ȼý� ��ť------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_Dole);
	});

	_addButtonClickEvent(pMainNode, "btn_weixin", [this](Ref* sender)
	{
		/* -----------΢�Ź��ں� ��ť------------- */
		this->close();
		HallGameScene::getInstance()->openWindow(EWT_WeiXin);
	});
}

void WindowFreeMoney::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}