#include "CycleUnit.h"
#include "HallGameScene.h"
#include "cocos2d.h"
#include "Sound/SoundManager.h"

USING_NS_CC;

CycleUnit::CycleUnit()
	: m_pEffectNode(nullptr)
{

}

CycleUnit::~CycleUnit()
{

}

CycleUnit* CycleUnit::create(const sUnitInfo& info)
{
	CycleUnit* pUnit = new CycleUnit;

	if (pUnit && pUnit->init(info))
	{
		pUnit->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pUnit);
	}

	return pUnit;
}

bool CycleUnit::init(const sUnitInfo& info)
{
	if (!ImageView::init())
	{
		return false;
	}
	this->load(info);

	m_pEffectNode = Node::create();
	m_pEffectNode->setPosition(Vec2(_contentSize.width / 2, 0));
	this->addChild(m_pEffectNode,1000);

	return true;
}

void CycleUnit::load(const sUnitInfo& info)
{
	this->loadTexture(info.strImgPath, TextureResType::PLIST);
	this->setUserData((void*)&info);
}

void CycleUnit::playEffect(bool bPlaySoundEffect)
{
	sUnitInfo* pData = (sUnitInfo*)this->getUserData();
	if (pData)
	{
		auto _emitter = ParticleSystemQuad::create(pData->strEffectPath);
		auto _emitter1 = ParticleSystemQuad::create(pData->strEffectPath);
		switch (pData->m_nSceneId)
		{
		case EHST_SHARK:
			m_pEffectNode->addChild(_emitter1);
			_emitter->setPosition(Vec2(-_contentSize.width * 0.2f, _contentSize.height * 0.2f));
			_emitter1->setPosition(Vec2(_contentSize.width * 0.2f, _contentSize.height * 0.2f));
			if (bPlaySoundEffect)
			{
				SoundManager::shared()->stopEffect();
				SoundManager::shared()->playEffectNormal(SoundManager::shared()->getHallMusicFileFullPath("jinsha").c_str());
			}
			break;
		case EHST_SLWH:
			m_pEffectNode->addChild(_emitter1);
			_emitter->setPosition(Vec2(_contentSize.width * 0.15f, _contentSize.height * 0.5f));
			_emitter1->setPosition(Vec2(-_contentSize.width * 0.2f, _contentSize.height * 0.3f));
			if (bPlaySoundEffect)
			{
				SoundManager::shared()->stopEffect();
				SoundManager::shared()->playEffectNormal(SoundManager::shared()->getHallMusicFileFullPath("senlin").c_str());
			}
			break;
		case EHST_SHUIHU:
			_emitter->setScale(0.25f);
			_emitter->setPosition(Vec2(0,30));
			if (bPlaySoundEffect)
			{
				SoundManager::shared()->stopEffect();
				SoundManager::shared()->playEffectNormal(SoundManager::shared()->getHallMusicFileFullPath("shuihu").c_str());
			}
			break;
		default:
			break;
		}
		
		m_pEffectNode->addChild(_emitter);
	}
	
}

void CycleUnit::stopEffect()
{
	m_pEffectNode->removeAllChildren();
}