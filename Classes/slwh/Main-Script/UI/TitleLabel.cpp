#include "TitleLabel.h"

//在Title节点下的逻辑标签
#define _TITLE_BG		"Titlebg"
#define	_TITLE			"T"



const float TitleLabel::UPDATE_TIME = 3.f;
TitleLabel* TitleLabel::m_instance = nullptr;
TitleLabel::TitleLabel() :
m_titleBg(nullptr),
m_title(nullptr),
m_nowTyoe(Invalid)
{
}

TitleLabel::~TitleLabel()
{	
}

void TitleLabel::relese()
{
	if (m_instance)
	{
		m_instance->removeFromParent();
		m_instance = nullptr;
	}
	
}

TitleLabel* TitleLabel::GetInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = new TitleLabel();

	}
	return m_instance;
}

void TitleLabel::Start()
{
	m_title = dynamic_cast<ImageView*>(m_node->getChildByName(_TITLE));
	CC_ASSERT(nullptr != m_title);
	m_nowTyoe = Normal1;
	m_title->loadTexture(Path_PNG_Title_NORMAL1, TextureResType::PLIST);


	m_titleBg = dynamic_cast<ImageView*>(m_node->getChildByName(_TITLE_BG));
	CC_ASSERT(nullptr != m_titleBg);
	m_titleBg->setVisible(false);

	m_node->setOpacity(_ZERO);
}

//打开标题提醒
void TitleLabel::StartOpen()
{
	if (m_nowTyoe != Normal1) PlayStarEff();

	m_node->setOpacity(_ZERO);

	auto ac1 = CCFadeIn::create(0.5f);
	auto ac2 = DelayTime::create(2.f);
	auto ac3 = CCFadeOut::create(0.5f);
	auto seq = Sequence::create(ac1, ac2, ac3, nullptr);
	m_node->runAction(seq);
}

void TitleLabel::SetTitle(enumTitleType t)
{

	TitleLabel * This = TitleLabel::GetInstance();
	
	//没有管理资源
	if (nullptr == This->m_node)
		return;

	This->m_node->stopAllActions();

	do
	{
		if (This->m_nowTyoe == t) break;

		This->m_nowTyoe = t;
		This->m_titleBg->setVisible(true);
		
		if (This->m_title != nullptr)
		{
			
			This->m_title->removeFromParent();
			This->m_title = nullptr;
		}
	
		switch (This->m_nowTyoe)
		{
		case Invalid:
			//打个酱油
			break;
		case Normal1:
			This->m_titleBg->setVisible(false);
			This->m_title = ImageView::create(Path_PNG_Title_NORMAL1, TextureResType::PLIST);
			This->m_node->addChild(This->m_title,200);
			break;
		/*case Normal2:
			This->m_title = ImageView::create(Path_PNG_Title_NORMAL2);
			This->m_node->addChild(This->m_title,200);
			This->m_titleBg->setVisible(false);
			break;
		case Normal3:
			This->m_title= ImageView::create(Path_PNG_Title_NORMAL3);
			This->m_node->addChild(This->m_title,200);
			This->m_titleBg->setVisible(false);
			break;*/
		case D3Y:
			This->m_title = ImageView::create(Path_PNG_Title_D3Y);
			This->m_node->addChild(This->m_title);
			break;
		case D4X:
			This->m_title = ImageView::create(Path_PNG_Title_D4X);
			This->m_node->addChild(This->m_title);
			break;
		case _4lian:
			This->m_title = ImageView::create(Path_PNG_Title_4LF);
			This->m_node->addChild(This->m_title);
			break;
		case DMG:
			This->m_title = ImageView::create(Path_PNG_Title_DMG);
			This->m_node->addChild(This->m_title);
			break;
		case CJ:
			This->m_title = ImageView::create(Path_PNG_Title_CJ);
			This->m_node->addChild(This->m_title);
			break;
		case XYCJ:
			This->m_title = ImageView::create(Path_PNG_Title_XYCJ);
			This->m_node->addChild(This->m_title);
			break;
		case TitleMax:
			//打个酱油
			break;
		default:
			break;
		}
	} while (_ZERO);

	This->StartOpen();
}

void TitleLabel::PlayStarEff()
{
	auto particle = ParticleSystemQuad::create("images/common/Eff/card_2.plist");
	particle->setPosition(Vec2(_ZERO,_ZERO));
	particle->setAutoRemoveOnFinish(true);                          // 完成后制动移除  
	particle->setDuration(2.5f);									// 设置粒子系统的持续时间秒  
	m_node->addChild(particle);
}

