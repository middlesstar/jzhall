#include "GameUpdate.h"
#include "Player/Player.h"

GameUpdate::GameUpdate()
{

}

GameUpdate::~GameUpdate()
{
}

void GameUpdate::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZUpdate.csb");
	CCASSERT(pMainNode, "WindowService::setContent() layout/JZUpdate.csb load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	_addButtonClickEvent(pMainNode, "btn_ok", [this](Ref* sender)
	{
		std::string updateAdd = Player::getInstance()->getUpdateNotify().szUpdateUrl;
		log("--------------------%s-------------", updateAdd.c_str());
		Application::getInstance()->openURL(updateAdd.c_str());
		this->close();
	});
}