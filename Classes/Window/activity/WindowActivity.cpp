#include "WindowActivity.h"
#include "LogicActivity.h"
#include "LanguageConfig.h"
#include "Player/Player.h"
#include "Util/GameUtil.h"
#include "Window/activity/GrabRed/GrabRed.h"


WindowActivity::WindowActivity()
{
	
}

WindowActivity::~WindowActivity()
{
	 
	
}

void WindowActivity::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZactivity.csb");
	CCASSERT(pMainNode, "WindowActivity::setContent() layout/JZactivity.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	//设置按钮关闭事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_begin", [this](Ref* sender){
		//发送红包请求

		this->close();
	});
}


void WindowActivity::updateTime(float t)
{
}

void WindowActivity::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowActivity::updateWindow(int nCode, void* pData)
{
}

void WindowActivity::updateWindow(int nCode)
{
	if (nCode == 0)
	{
		//开始抢红包
		
	}
	else
	{
		//不能抢福袋（时间未到）
		log("----------%s----------", a_u8("不能抢福袋"));
	}
}

void WindowActivity::StartTimeCount(int t)
{

}