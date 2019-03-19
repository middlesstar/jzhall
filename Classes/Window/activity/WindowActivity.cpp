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

	//���ð�ť�ر��¼�
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_begin", [this](Ref* sender){
		//���ͺ������

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
		//��ʼ�����
		
	}
	else
	{
		//������������ʱ��δ����
		log("----------%s----------", a_u8("����������"));
	}
}

void WindowActivity::StartTimeCount(int t)
{

}