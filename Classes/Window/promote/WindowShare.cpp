#include "WindowShare.h"
#include "UMShareSDK.h"
#include "LanguageConfig.h"

WindowShare::WindowShare()
{

}

WindowShare::~WindowShare()
{

}

void WindowShare::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZShare.csb");
	CCASSERT(pMainNode, "WindowShare::setContent() layout/JZShare.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();
	
	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_weixin", CC_CALLBACK_1(WindowShare::_shareWeiXin,this));
	_addButtonClickEvent(pMainNode, "btn_other", CC_CALLBACK_1(WindowShare::_shareWeiXinFriendsCicle, this));
	_addButtonClickEvent(pMainNode, "btn_qqzone", CC_CALLBACK_1(WindowShare::_shareQQZone, this));

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});
}

void WindowShare::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowShare::_shareWeiXin(Ref* sender)
{
	UMShareSDK::getInstance()->directShare(EU_WEIXIN, GETSTRING(14));
}

void WindowShare::_shareWeiXinFriendsCicle(Ref* sender)
{
	UMShareSDK::getInstance()->directShare(EU_WEIXIN_CIRCLE, GETSTRING(14));
}

void WindowShare::_shareQQZone(Ref* sender)
{
	UMShareSDK::getInstance()->directShare(EU_QZONE, GETSTRING(14));
}
