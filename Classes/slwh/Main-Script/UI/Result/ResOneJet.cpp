#include "ResOneJet.h"

//节点下逻辑名称
#define _JET			"jet"
#define _LIGHT			"light"
#define _BEILV			"beilv"
#define _BK				"bk"

#define PATH_PNG_BK_0	"game/game_results/gr_bet_fram0.png"
#define PATH_PNG_BK_1	"game/game_results/gr_bet_fram1.png"

ResOneJet::ResOneJet():
m_beilv(nullptr),
m_jet(nullptr),
m_light(nullptr),
m_bk(nullptr)
{
}

ResOneJet::~ResOneJet()
{
}

void ResOneJet::Start()
{

	m_beilv = dynamic_cast<TextAtlas*>(m_node->getChildByName(_BEILV));
	CC_ASSERT(nullptr != m_beilv);

	m_jet = dynamic_cast<TextAtlas*>(m_node->getChildByName(_JET));
	CC_ASSERT(nullptr != m_jet);

	m_light = dynamic_cast<ImageView*>(m_node->getChildByName(_LIGHT));
	CC_ASSERT(nullptr != m_light);
	m_light->setVisible(false);

	m_bk = dynamic_cast<ImageView*>(m_node->getChildByName(_BK));
	CC_ASSERT(nullptr != m_bk);
	m_bk->setVisible(false);

}

void ResOneJet::SetBeiLv(int t)
{
	if (nullptr == m_node)
	{
		return;
	}
	std::stringstream str;
	str << t;
	m_beilv->setString(str.str());

	//策划说 大于20倍就 是掉渣天的彩色框
	t >= 20 ? m_bk->loadTexture(PATH_PNG_BK_1, TextureResType::PLIST) : m_bk->loadTexture(PATH_PNG_BK_0, TextureResType::PLIST);

}

void ResOneJet::SetJet(int t)
{
	if (nullptr == m_node)
	{
		return;
	}

	std::stringstream str;
	str << t;
	m_jet->setString(str.str());
}

//中了动物要开灯
void ResOneJet::OpenLight(bool b)
{
	if (nullptr == m_node)
	{
		return;
	}

	if (b)
	{
		m_light->setScale(1.5f);
		m_bk->setScale(1.5f);
		auto ac0 = DelayTime::create(1.5f);
		auto ac1 = EaseExponentialIn::create(ScaleTo::create(0.5, 1.0f));			//盖章效果
		auto ac2 = Blink::create(8.f, 30);											//闪瞎眼
		auto fun = CallFuncN::create([this](Node* n){n->setVisible(true); });
		auto seq = Sequence::create(ac0, fun, ac1, ac2, nullptr);
		auto seq1 = Sequence::create(ac0->clone(), fun->clone(), ac1->clone(), nullptr);
		
		m_light->runAction(seq);
		m_bk->runAction(seq1);
	}
	else
	{
		m_light->setVisible(false);
		m_bk->setVisible(false);
	}
}
