#include "CommonConfirmNode.h"
#include "Player/Player.h"
#include "HallGameScene.h"

CommonConfirmNode::CommonConfirmNode()
{

}

CommonConfirmNode::~CommonConfirmNode()
{

}

bool CommonConfirmNode::init()
{
	return Node::init();
}

void CommonConfirmNode::onOkClick(int nData, void* pData)
{
	switch (nData)
	{
	case ECCNT_DISCONNECT:
		{
			HallGameScene::getInstance()->backToLoginWindow();
		}
		break;
	case ECCNT_EXIT:
		{
			HallGameScene::getInstance()->exitGame();
		}
		break;
	case ECCNT_BACKTOLOGIN:
	   {
		   HallGameScene::getInstance()->backToLoginWindow();
	   }
		break;
	default:
		break;
	}
}

void CommonConfirmNode::onCancelClick(int nData, void* pData)
{
	switch (nData)
	{
	case ECCNT_DISCONNECT:
		break;
	case ECCNT_EXIT:
		break;
	case ECCNT_BACKTOLOGIN:
		break;
	default:
		break;
	}
}