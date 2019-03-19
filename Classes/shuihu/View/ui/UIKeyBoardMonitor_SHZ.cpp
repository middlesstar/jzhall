#include "UIKeyBoardMonitor_SHZ.h"
#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"

#include "HallGameScene.h"

USING_NS_CC;



bool SHZUIKeyBoardMonitor::init()
{
	this->setKeypadEnabled(true);
	return true;
}

void SHZUIKeyBoardMonitor::setTitleAndContent(const char *title, const char *content)
{
	this->title = title;
	this->content = content;
}

void SHZUIKeyBoardMonitor::setSceneName(char *scene)
{
	strcpy(sceneName, scene);
}


void SHZUIKeyBoardMonitor::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:

		popup_SHZ(title, content, DLG_MB_OK | DLG_MB_CANCEL, 0, this, callfuncN_selector(SHZUIKeyBoardMonitor::onReturnCallback));
		break;
	default:
		break;
	}
}

void SHZUIKeyBoardMonitor::onReturnCallback(cocos2d::CCNode *pNode)
{
	switch (pNode->getTag())
	{
	case DLG_MB_OK:
		{
			if (0 == strcmp("GameTypeScene", sceneName))
			{
				HallGameScene::switchScene(EHST_HALL);
			}

			if (0 == strcmp("exit", sceneName))
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
				CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
				CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				exit(0);
#endif
#endif
			}
		}
		break;
	}
}