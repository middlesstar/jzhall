#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "ServerListLayer_SHZ.h"
#include "ServerListScene_SHZ.h"
#include "shuihu/View/Scene/UserInfoLayer_SHZ.h"
#include "shuihu/View/ui/UIKeyBoardMonitor_SHZ.h"
#include "HallGameScene.h"
#include "shuihu/Resource_SHZ.h"
#include "shuihu/View/Scene/getHouseIndex_SHZ.h"
USING_NS_CC;


SHZServerListScene* SHZServerListScene::create(int mode)
{
	SHZServerListScene* scene = new SHZServerListScene();
	if (scene && scene->init(mode))
	{
		scene->autorelease();
		return scene;
	}

	delete scene;
	return 0;
}

SHZServerListScene::SHZServerListScene()
{

}

SHZServerListScene::~SHZServerListScene()
{

}

//初始化方法
bool SHZServerListScene::init(int mode)
{
	do
	{
		CC_BREAK_IF(!CCScene::init());

		mMode = mode;

		SHZUIKeyBoardMonitor *monitor = SHZUIKeyBoardMonitor::create();
		monitor->setTitleAndContent(SSTRING("back_to_login").c_str(), SSTRING("back_to_login_content").c_str());
		monitor->setSceneName("GameTypeScene");
		addChild(monitor);

		SHZUserInfoLayer* info = SHZUserInfoLayer::create();
		info->setCloseDialogInfo(this, callfuncN_selector(SHZServerListScene::closeCallback), SSTRING("back_to_login"), SSTRING("back_to_login_content"));
		addChild(info, 0);

		int __gType = DF::shared()->GetGameType();

		if (DF::shared()->GetGameType() == 0)
		{
			SHZServerListLayer* server_list = SHZServerListLayer::create(mMode);
			addChild(server_list, 0);

			HallGameScene::openChatSprite(this);
		}

		return true;
	} while (0);

	return false;
}
//判断是否弹出充值框
void SHZServerListScene::judgeRecharge()
{
	if (SHZGetHouseIndex::getInstance()->getRechargeFlag())
	{
		auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
		rechargeNode->setPosition(Vec2(0, 0));
		this->addChild(rechargeNode, 2000);

		SHZGetHouseIndex::getInstance()->setRechargeFlag(false);
	}

}
void SHZServerListScene::onEnterTransitionDidFinish(){

	CCScene::onEnterTransitionDidFinish();
	judgeRecharge();
	//SHZSoundManager::shared()->stopMusic();
	_PLAY_MUSCI_SHZ(BACKGROUNDROOM3, true);
}

void SHZServerListScene::closeCallback(cocos2d::CCNode *pNode)
{
	switch (pNode->getTag())
	{
	case DLG_MB_OK:
		{
			//返回大厅
			HallGameScene::switchScene(EHST_HALL);
		}
		break;
	}
}