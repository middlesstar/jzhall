#include "RuleShow.h"


#define  COCOS_STUDIO_RULE  "shark/JZRuleLayer.csb"
RuleShow::RuleShow() :closeBtn(nullptr)
{

}
RuleShow::~RuleShow()
{
	
}
void RuleShow::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile(COCOS_STUDIO_RULE);
	CC_ASSERT(pMainNode!=nullptr);


	m_pNode->addChild(pMainNode);
	this->addBlackLayer();
	//设置按钮事件
	_addButtonClickEvent(pMainNode, "closebtn", [this](Ref* sender)
	{
		this->close();
	});
}


void RuleShow::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}


