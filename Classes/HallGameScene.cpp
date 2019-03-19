#include "HallGameScene.h"
#include "MessageTipNode.h"
#include "LanguageConfig.h"
#include "Window/WindowManager.h"
#include "Window/WindowBase.h"
#include "Window/ConfirmDialog/WindowConfirmDialog.h"
#include "Window/chat/LogicChat.h"
#include "Player/Player.h"
#include "Sound/SoundManager.h"

// #include "buyu/view/scene/logon/LogonScene.h"
// #include "buyu/plazz/msg/CMD_Fish.h"
#include "df/DF.h"
#include "df/JZCore.h"
#include "df/StringData.h"
#include "libs/crypt/coAes.h"

#include "df/EncryptKey.h"
#include "shuihu/gPlatform_SHZ.h"
#include "shuihu/Utils_SHZ.h"
#include "shuihu/View/Scene/LoadScene/LoadingLayer_SHZ.h"

#include "slwh/Hall-Script/SLWHLoadingScene.h"
#include "slwh/slwhDefine.h"

//金鲨银鲨
//#include "shark/UI/SelectRoom/SelectRoom.h"
#include "shark/UI/LoadingScene/LoadingShark.h"
#include "shark/sharkDefine.h"

#define HEARTBEAT_TIMER_NAME	"SendHeartBeat"

static HallGameScene* pGlobelHallGameScene = NULL;
int HallGameScene::m_gCurSceneId = 0;
std::vector<std::string> HallGameScene::m_gSearchPathVec;

HallGameScene::HallGameScene()
{
}

HallGameScene::~HallGameScene()
{
	pGlobelHallGameScene = nullptr;
}

Scene* HallGameScene::createScene()
{
	Scene* pScene = Scene::create();
	pScene->addChild(HallGameScene::getInstance());
	m_gCurSceneId = EHST_HALL;

	return pScene;
}

HallGameScene* HallGameScene::getInstance()
{
	if (!pGlobelHallGameScene)
	{
		pGlobelHallGameScene = HallGameScene::create();
	}

	return pGlobelHallGameScene;
}

WindowBase* HallGameScene::createWindow(int nWid)
{
	return WindowManager::getInstance()->getWindowFactory(nWid)->create();
}

void HallGameScene::setHandleKeyboard(bool bHandle)
{
	if (m_gCurSceneId == EHST_HALL && HallGameScene::getInstance()->isKeyboardEnabled() != bHandle)
	{
		HallGameScene::getInstance()->setKeyboardEnabled(bHandle);
	}
}

int HallGameScene::getCurRoomid()
{
	int nServerSceneId = ECC_HALL;			//房间ID 0-大厅 1-水浒传 2-森林舞会 3-金沙银沙 4-捕鱼 

	switch (m_gCurSceneId)
	{
	case EHST_HALL:
		nServerSceneId = ECC_HALL;
		break;
	case EHST_SLWH:
		nServerSceneId = ECC_SLWH;
		break;
	case EHST_SHARK:
		nServerSceneId = ECC_SHARK;
		break;
	case EHST_SHUIHU:
		nServerSceneId = ECC_SHUIHU;
		break;
	default:
		break;
	}

	return nServerSceneId;
}

void HallGameScene::openSceneMessageTip(const char* pText)
{
	MessageTipNode* pMessageNode = MessageTipNode::createWithMessage(pText);
	Size size = Director::getInstance()->getWinSize();
	pMessageNode->setPosition(size.width / 2, size.height / 2);

	Director::getInstance()->getRunningScene()->addChild(pMessageNode);
}

void HallGameScene::openChatSprite(Node* pParentNode)
{
	if (!pParentNode)
	{
		return;
	}

	WindowBase* pWindow = HallGameScene::createWindow(EWT_Chat);
	LogicChat::getInstance()->setChatWindow(pWindow);
	pParentNode->addChild(pWindow);
}

bool HallGameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_pWindowParentNode = Node::create();
	this->addChild(m_pWindowParentNode);

	m_pDialogParentNode = Node::create();
	this->addChild(m_pDialogParentNode,10000);

	m_pCommonConfirmNode = CommonConfirmNode::create();
	this->addChild(m_pCommonConfirmNode);

	openWindow(m_gCurSceneId == 0 ? EWT_Login : EWT_Hall);

	setKeyboardEnabled(true);

	return true;
}

void HallGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	//目前只处理返回键
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		if (m_pWindowParentNode->getChildrenCount() == 0)
		{
			return;
		}

		if (m_pWindowParentNode->getChildrenCount() > 1 || m_pWindowParentNode->getChildren().back()->getTag() == EWT_Hall)
		{
			dynamic_cast<WindowBase*>(m_pWindowParentNode->getChildren().back())->onBackKeyPressed();
		}
		else
		{
			openOkCancelDialog(GETSTRING(3), nullptr, ECCNT_EXIT);
		}
	}
}

void HallGameScene::onEnter()
{
	Layer::onEnter();

	SoundManager* pSound = SoundManager::shared();
	pSound->playMusicNormal(pSound->getHallMusicFileFullPath("timego").c_str(), true);

	SimpleAudioEngine::sharedEngine()->preloadEffect(pSound->getHallMusicFileFullPath("ui").c_str());

	//开始定时发送心跳包
	this->schedule([](float dt) 
	{
		NetManager::getInstance()->sendHeartBeat();
	}, 15.0f, HEARTBEAT_TIMER_NAME);
}

void HallGameScene::onExit()
{
	this->unschedule(HEARTBEAT_TIMER_NAME);

	//SoundManager::shared()->stopMusic();
	SimpleAudioEngine::sharedEngine()->unloadEffect(SoundManager::shared()->getHallMusicFileFullPath("ui").c_str());

	Layer::onExit();
}

void HallGameScene::switchScene(int nSceneId)
{
	if (m_gCurSceneId == nSceneId)
	{
		return;
	}

	initSubGame(nSceneId);			//先初始化子游戏的一些东西

	Scene* pScene = nullptr;

	//先移除旧场景中的聊天节点
	if (LogicChat::getInstance()->getChatWindow())
	{
		LogicChat::getInstance()->getChatWindow()->removeFromParent();
	}

	switch (nSceneId)
	{
	case EHST_HALL:
		pScene = HallGameScene::createScene();
		break;
	case EHST_SLWH:
		pScene = SLWHLoadingScene::create();
		break;
	case EHST_SHARK:
		//pScene = LogonScene::create();
		//pScene = SelectRoom::createScene();
		pScene = LoadingShark::create();
		break;
	case EHST_SHUIHU:
		pScene = SHZLoadingLayer::scene();
		break;
	default:
		break;
	}

	if (pScene != nullptr)
	{
		m_gCurSceneId = nSceneId;
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, pScene));

		AnimationCache::destroyInstance();
		SpriteFrameCache::getInstance()->destroyInstance();
		TextureCache::getInstance()->removeAllTextures();

		LogicChat::getInstance()->requestChangeRoom(getCurRoomid());
	}
}

void HallGameScene::initSubGame(int nSceneId)
{
	FileUtils::getInstance()->setSearchPaths(m_gSearchPathVec);
	StringData::shared()->clear();

	//Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
	//Texture2D::PVRImagesHavePremultipliedAlpha(true);

	switch (nSceneId)
	{
	case EHST_HALL:
		break;
	case EHST_SLWH:
		{
			FileUtils::getInstance()->addSearchPath("slwh/");

			DF::shared()->init(KIND_ID_SLWH, GAME_PLAYER_SLWH, VERSION_CLIENT_SLWH, SSTRING("appname").c_str());
		}

		break;
	case EHST_SHARK:
	{
		   FileUtils::getInstance()->addSearchPath("shark/Music");
	     //FileUtils::getInstance()->addSearchPath("images/shak/");
// 		 	//添加资源搜索目录
// 		 	FileUtils::getInstance()->addSearchPath("buyu/");
// 		 	FileUtils::getInstance()->addSe		archPath("buyu/ui/hall_scene");
// 		 	FileUtils::getInstance()->addSearchPath("buyu/ui/hall_scene/mail");
// 		 	FileUtils::getInstance()->addSearchPath("buyu/ui/common");
// 		 	FileUtils::getInstance()->addSearchPath("buyu/ui/game_scene");
// 		 	FileUtils::getInstance()->addSearchPath("buyu/fonts/");
// 		 	FileUtils::getInstance()->addSearchPath("buyu/bmfonts");
// 		 	FileUtils::getInstance()->addSearchPath("buyu/animations");
// 		 
// 			StringData::shared()->init("buyu/strings.plist");
// 		 	coAesSetKey("11111111222222223333333344444444");
// 		 	DF::shared()->init(KIND_ID, GAME_PLAYER, VERSION_CLIENT, SSTRING("appname").c_str());
// 		 
// 		 	/// 初始化平台相关的信息
// 		 	JZCore::getInstance()->initPlatform();
// 
// 			SoundManager::shared()->initBuYu();
		 DF::shared()->init(KIND_ID_SHARK, GAME_PLAYER_SHARK, VERSION_CLIENT_SLWH, SSTRING("appname").c_str());
		}
		break;
	case EHST_SHUIHU:
		{
			FileUtils::getInstance()->addSearchPath("shuihu/");
			FileUtils::getInstance()->addSearchPath("shuihu/selectroom/");

			StringData::shared()->init("shuihu/strings.plist");

			srand(time(0));
			Player::getInstance()->pushDataToSHZ();
			SoundManager::shared()->initShuiHu();
		}
		break;
	default:
		break;
	}
}

void HallGameScene::openWindow(int nWid)
{
	m_pWindowParentNode->removeChildByTag(nWid);		//先删除
	WindowBase* pWindow = WindowManager::getInstance()->getWindowFactory(nWid)->create();

	if (pWindow)
	{
		m_pWindowParentNode->addChild(pWindow, 0, nWid);
	}

	return;
}

void HallGameScene::closeWindow(int nWid)
{
	m_pWindowParentNode->removeChildByTag(nWid);		//删除
}

void HallGameScene::updateWindow(int nWid,int nCode/* = 0*/)
{
	WindowBase* pWindow = dynamic_cast<WindowBase*>(m_pWindowParentNode->getChildByTag(nWid));
	if (pWindow)
	{
		pWindow->updateWindow(nCode);
	}
	else
	{
		this->openWindow(nWid);
	}
}

void HallGameScene::updateWindow(int nWid, int nCode, void * pData)
{
	WindowBase* pWindow = dynamic_cast<WindowBase*>(m_pWindowParentNode->getChildByTag(nWid));
	if (pWindow)
	{
		pWindow->updateWindow(nCode, pData);
	}
	else
	{
		this->openWindow(nWid);
		this->updateWindow(nWid, nCode, pData);
	}
}

void HallGameScene::exitGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HallGameScene::openMessageTip(const char* pText)
{
	m_pDialogParentNode->removeAllChildren();
	MessageTipNode* pMessageNode = MessageTipNode::createWithMessage(pText);
	Size size = Director::getInstance()->getWinSize();
	pMessageNode->setPosition(size.width/2,size.height/2);
	m_pDialogParentNode->addChild(pMessageNode);
}

void HallGameScene::openMessageTip(int nStrId)
{
    openMessageTip(LanguageConfig::getInstance()->getString(nStrId).c_str());
}

void HallGameScene::openOkDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate/* = nullptr*/, int nData/* = 0*/, void* pData/* = nullptr*/)
{
	WindowConfirmDialog* pDialog = WindowConfirmDialog::create(strText);
	pDialog->setDialogType(EWDT_OK);
	pDialog->setDelegate(pDelegate == nullptr ? m_pCommonConfirmNode : pDelegate);
	pDialog->setUserData(pData);
	m_pWindowParentNode->addChild(pDialog, 10000, nData);
}

void HallGameScene::openOkCancelDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate/* = nullptr*/, int nData/* = 0*/, void* pData/* = nullptr*/)
{
	WindowConfirmDialog* pDialog = WindowConfirmDialog::create(strText);
	pDialog->setDialogType(EWDT_OK_CANCEL);
	pDialog->setDelegate(pDelegate == nullptr ? m_pCommonConfirmNode : pDelegate);
	pDialog->setUserData(pData);
	m_pWindowParentNode->addChild(pDialog, 10000, nData);
}


void HallGameScene::openOkVipDialog(const std::string& strText, ConfirmDialogDelegate* pDelegate /*= nullptr*/, int nData /*= 0*/, void* pData/* = nullptr*/)
{
	WindowConfirmDialog* pDialog = WindowConfirmDialog::create(strText);
	pDialog->setDialogType(EWDT_OK_VIP);
	pDialog->setDelegate(pDelegate == nullptr ? m_pCommonConfirmNode : pDelegate);
	pDialog->setUserData(pData);
	m_pWindowParentNode->addChild(pDialog, 10000, nData);
}

void HallGameScene::backToLoginWindow()
{
	if (m_gCurSceneId != EHST_HALL)
	{
		switchScene(EHST_HALL);
	}

	//打开界面
	m_pWindowParentNode->removeAllChildren();
	this->openWindow(EWT_Login);

	//断开登录服务器
	NetManager::getInstance()->close();
}