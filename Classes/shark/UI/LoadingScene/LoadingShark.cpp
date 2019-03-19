#include "LoadingShark.h"
#include "shark/resourceDef.h"
#include "cocostudio/CocoStudio.h"
#include "shark/UI/SelectRoom/SelectRoom.h"



#define TEX_PNG_PLIST_PATH				"images/shak/SharkRes.plist"
#define MODEL_PLIST_PATH				"images/shak/SharkAnimal.plist"
#define SHARK_MAX_PATH					260


LoadingShark::LoadingShark() :
m_loadingBar(nullptr),
m_AllCount(_ZERO),
m_texIndex(_ZERO),
m_texCount(_ZERO),
m_modelIndex(_ZERO),
m_modelCount(_ZERO),
m_state(_ZERO)
{
	m_vecTex = FileUtils::getInstance()->getValueVectorFromFile(TEX_PNG_PLIST_PATH);
	m_texCount = (int)m_vecTex.size();
	m_AllCount += m_texCount;

	m_vecModel = FileUtils::getInstance()->getValueVectorFromFile(MODEL_PLIST_PATH);
	m_modelCount = (int)m_vecModel.size();
	m_AllCount += m_modelCount;

}

LoadingShark::~LoadingShark()
{
}

void LoadingShark::onEnter()
{
	Node::onEnter();
}

void LoadingShark::onExit()
{
	Node::onExit();
}


bool LoadingShark::init()
{
	do
	{
		CC_BREAK_IF(!Scene::init());

		//获取COCOS_STUDIO主场景UI根节点
		auto rootNode = CSLoader::createNode(COCOS_STUDIO_LOADING);
		CC_BREAK_IF(rootNode == nullptr);
		addChild(rootNode);

		m_loadingBar = dynamic_cast<LoadingBar*>(rootNode->getChildByName("progressBar")->getChildByName("LoadingBar_1"));
		CC_BREAK_IF(m_loadingBar == nullptr);

		SetProgressBar(_ZERO);

		return true;
	} while (_ZERO);

	return false;
}

void LoadingShark::onEnterTransitionDidFinish()
{
	switch (m_state)
	{
	case 0:
		//先加载图片
		loadTex();
		break;
	case 1:

		break;
	default:
		break;
	}
}


void LoadingShark::SetProgressBar(float per)
{

	if (per < _ZERO) per = _ZERO;
	if (per > 1.f) per = 1.f;

	m_loadingBar->setPercent(per * 100);
}

bool LoadingShark::loadTex()
{
	char strPath[SHARK_MAX_PATH] = { _ZERO };
	std::string name = m_vecTex[m_texIndex].asString();
	sprintf(strPath, "%s.png", name.c_str());
	Director::getInstance()->getTextureCache()->addImageAsync(strPath, CC_CALLBACK_1(LoadingShark::onAyncTexCallback, this));

	return true;
}


void LoadingShark::onAyncTexCallback(Texture2D* ref)
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

		//加载模型
		loadModel();
		return;
	}
	else loadTex();

}



bool LoadingShark::loadModel()
{
	char strPath[SHARK_MAX_PATH] = { _ZERO };
	std::string name = m_vecModel[m_modelIndex].asString();
	sprintf(strPath, "%s.c3b", name.c_str());
	Sprite3D::createAsync(strPath, CC_CALLBACK_2(LoadingShark::onAsyncModelCallback, this), nullptr);
	return true;
}


void LoadingShark::onAsyncModelCallback(Sprite3D* sprite, void* param)
{
	m_modelIndex++;
	SetProgressBar((float)(m_texCount + m_modelIndex) / (float)m_AllCount);

	if (m_texCount + m_modelIndex >= m_AllCount)
	{
		m_vecModel.clear();
		m_modelIndex = _ZERO;
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, SelectRoom::createScene()));
		return;
	}
	else loadModel();
}


