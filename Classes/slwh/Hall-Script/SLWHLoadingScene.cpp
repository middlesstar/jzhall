#include "SLWHLoadingScene.h"
#include "slwh/ResourcePath.h"
#include "cocostudio/CocoStudio.h"
#include "slwh/Hall-Script/UILayerHall.h"

#define TEX_PNG_PLIST_PATH				"SLWHTex.plist"
#define MODEL_PLIST_PATH				"SLWHModel.plist"
#define SLWH_MAX_PATH					256


//SLWHGameScene* SLWHLoadingScene::m_pMainGameScene = nullptr;
SLWHLoadingScene::SLWHLoadingScene():
m_loadingBar(nullptr),
m_AllCount(_ZERO),
m_texIndex(_ZERO),
m_texCount(_ZERO),
m_modelIndex(_ZERO),
m_modelCount(_ZERO)
{
	m_vecTex = FileUtils::getInstance()->getValueVectorFromFile(TEX_PNG_PLIST_PATH);
	m_texCount = (int)m_vecTex.size();
	m_AllCount += m_texCount;

	m_vecModel = FileUtils::getInstance()->getValueVectorFromFile(MODEL_PLIST_PATH);
	m_modelCount = (int)m_vecModel.size();
	m_AllCount += m_modelCount;

}

SLWHLoadingScene::~SLWHLoadingScene()
{
}

void SLWHLoadingScene::onEnter()
{
	Node::onEnter();
}

void SLWHLoadingScene::onExit()
{
	Node::onExit();
}

bool SLWHLoadingScene::init()
{
	do
	{
		CC_BREAK_IF(!Scene::init());

		//获取COCOS_STUDIO主场景UI根节点
		auto rootNode = CSLoader::createNode(COCOS_STUDIO_PROGRESSSCENE);
		CC_BREAK_IF(rootNode == nullptr);
		addChild(rootNode);

		m_loadingBar = dynamic_cast<LoadingBar*>(rootNode->getChildByTag(1321)->getChildByTag(1322));
		CC_BREAK_IF(m_loadingBar == nullptr);

		SetProgressBar(_ZERO);

		return true;
	} while (_ZERO);

	return false;
}

void SLWHLoadingScene::onEnterTransitionDidFinish()
{

	//先加载图片
	loadTex();

}

//预加载音乐
bool SLWHLoadingScene::loadMusic()
{
	//背景
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(RABBIT_CALL);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(PANDA_CALL);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(LION_CALL);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MONKEY_CALL);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(HI_TIME);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(_DJKS);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(_KJJS);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(JET_WAIT);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(ZP_RUN);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(ZP_START);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(XYCJ_HI_TIME2);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(XYCJ_LUCK_BG);

	//特效
	SimpleAudioEngine::getInstance()->preloadEffect(GREEN_RABBIT);
	SimpleAudioEngine::getInstance()->preloadEffect(GREEN_LION);
	SimpleAudioEngine::getInstance()->preloadEffect(GREEN_MONKEY);
	SimpleAudioEngine::getInstance()->preloadEffect(GREEN_PANDA);

	SimpleAudioEngine::getInstance()->preloadEffect(RED_LION);
	SimpleAudioEngine::getInstance()->preloadEffect(RED_MONKEY);
	SimpleAudioEngine::getInstance()->preloadEffect(RED_PANDA);
	SimpleAudioEngine::getInstance()->preloadEffect(RED_RABBIT);

	SimpleAudioEngine::getInstance()->preloadEffect(YELLOW_LION);
	SimpleAudioEngine::getInstance()->preloadEffect(YELLOW_MONKEY);
	SimpleAudioEngine::getInstance()->preloadEffect(YELLOW_PANDA);
	SimpleAudioEngine::getInstance()->preloadEffect(YELLOW_RABBIT);

	SimpleAudioEngine::getInstance()->preloadEffect(LOTTERYTYPE_GREEN);
	SimpleAudioEngine::getInstance()->preloadEffect(LOTTERYTYPE_RED);
	SimpleAudioEngine::getInstance()->preloadEffect(LOTTERYTYPE_YELLOW);
	SimpleAudioEngine::getInstance()->preloadEffect(LOTTERYTYPE_LION);
	SimpleAudioEngine::getInstance()->preloadEffect(LOTTERYTYPE_MONKEY);
	SimpleAudioEngine::getInstance()->preloadEffect(LOTTERYTYPE_PANDA);
	SimpleAudioEngine::getInstance()->preloadEffect(LOTTERYTYPE_RABBIT);

	SimpleAudioEngine::getInstance()->preloadEffect(_BETCOIN);
	SimpleAudioEngine::getInstance()->preloadEffect(_XIAN);
	SimpleAudioEngine::getInstance()->preloadEffect(_HE);
	SimpleAudioEngine::getInstance()->preloadEffect(_ZHUANG);
	SimpleAudioEngine::getInstance()->preloadEffect(_CLICK);
	SimpleAudioEngine::getInstance()->preloadEffect(_TIME0);
	SimpleAudioEngine::getInstance()->preloadEffect(_TIME1);
	SimpleAudioEngine::getInstance()->preloadEffect(_JETTON);
	SimpleAudioEngine::getInstance()->preloadEffect(RES_ADD);
	SimpleAudioEngine::getInstance()->preloadEffect(_JETTON);
	SimpleAudioEngine::getInstance()->preloadEffect(_KZ);
	SimpleAudioEngine::getInstance()->preloadEffect(XYCJ_RUN);
	SimpleAudioEngine::getInstance()->preloadEffect(XYCJ_STOP);


	return true;
}

void SLWHLoadingScene::SetProgressBar(float per)
{
	if (per < _ZERO) per = _ZERO;
	if (per > 1.f) per = 1.f;

	m_loadingBar->setPercent(per * 100);
}

bool SLWHLoadingScene::loadTex()
{
	char strPath[SLWH_MAX_PATH] = { _ZERO };
	std::string name = m_vecTex[m_texIndex].asString();
	sprintf(strPath, "%s.png", name.c_str());
	Director::getInstance()->getTextureCache()->addImageAsync(strPath, CC_CALLBACK_1(SLWHLoadingScene::onAyncTexCallback, this));

	return true;
}

void SLWHLoadingScene::onAyncTexCallback(Texture2D* ref)
{

	//std::string name = m_vecRes[m_count].asString() + ".plist";
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile(name, ref);
	m_texIndex++;
	SetProgressBar((float)m_texIndex / (float)m_AllCount);

	if (m_texIndex >= m_texCount)
	{
		//Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, UILayerHall::createScene()));
		m_vecTex.clear();
		m_texIndex = _ZERO;

		//加载音乐
		loadMusic();

		//创建主游戏场景
		//m_pMainGameScene = SLWHGameScene::create();
		//m_pMainGameScene->retain();
		//加载模型
		loadModel();
		return;
	}
	else loadTex();

}

bool SLWHLoadingScene::loadModel()
{
	char strPath[SLWH_MAX_PATH] = { _ZERO };
	std::string name = m_vecModel[m_modelIndex].asString();
	sprintf(strPath, "%s.c3b", name.c_str());
	Sprite3D::createAsync(strPath, CC_CALLBACK_2(SLWHLoadingScene::onAsyncModelCallback, this), nullptr);
	return true;
}

void SLWHLoadingScene::onAsyncModelCallback(Sprite3D* sprite, void* param)
{
	m_modelIndex++;
	SetProgressBar((float)(m_texCount + m_modelIndex) / (float)m_AllCount);

	if (m_texCount + m_modelIndex >= m_AllCount)
	{
		m_vecModel.clear();
		m_modelIndex = _ZERO;
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, UILayerHall::createScene()));
		return;
	}
	else loadModel();
}


