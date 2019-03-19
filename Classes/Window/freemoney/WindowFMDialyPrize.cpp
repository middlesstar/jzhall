#include "WindowFMDialyPrize.h"
#include "LogicFreeMoney.h"
#include "LanguageConfig.h"
#include "Player/Player.h"

WindowFMDialyPrize::WindowFMDialyPrize()
	: m_pTurntable(nullptr)
	, m_fStartRorate(0.f)
{

}

WindowFMDialyPrize::~WindowFMDialyPrize()
{
}

void WindowFMDialyPrize::setContent()
{ 
	Node *pMainNode = _loadCSBLayoutFile("layout/JZFMDailyPrize.csb");
	CCASSERT(pMainNode, "WindowFMDialyPrize::setContent() layout/JZFMDailyPrize.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_pTurntable = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode,"img_turntable"));
	m_fStartRorate = m_pTurntable->getRotation();

	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_begin", [this](Ref* sender)
	{
		if (m_pTurntable->numberOfRunningActions() == 0)
		{
			LogicFreeMoney::getInstance()->requestDialyPrize();
		}
	});
}

void WindowFMDialyPrize::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();

	SimpleAudioEngine::sharedEngine()->preloadEffect(SoundManager::shared()->getHallMusicFileFullPath("turntable").c_str());
}

void WindowFMDialyPrize::onExit()
{
	SimpleAudioEngine::sharedEngine()->unloadEffect(SoundManager::shared()->getHallMusicFileFullPath("turntable").c_str());

	WindowBase::onExit();
}

void WindowFMDialyPrize::updateWindow(int nCode)
{
	if (nCode >= 0)
	{
		_startTurntable(nCode, std::bind(&WindowFMDialyPrize::_stopCallback, this));
	}
}

void WindowFMDialyPrize::_startTurntable(int nStopPos, const std::function<void()>& onTurntableStop)
{
	const float OnePieceAngle = 30.f;
	const int MainAngle = 360 * 36;
	const float fRange = 8.f;

	auto actionDeceleration = RotateBy::create(7.6f, MainAngle - OnePieceAngle * (nStopPos - 1) + CCRANDOM_MINUS1_1() * fRange);

	auto sequenceAction = Sequence::create(
		EaseOut::create(actionDeceleration, 8.2f),
		DelayTime::create(0.5f),
		CallFunc::create(onTurntableStop),
		NULL
		);

	m_pTurntable->setRotation(m_fStartRorate);
	m_pTurntable->runAction(sequenceAction);

	SoundManager::shared()->playEffectNormal(SoundManager::shared()->getHallMusicFileFullPath("turntable").c_str());
}

void WindowFMDialyPrize::_stopCallback()
{
	log("----------_stopCallback----------");
	SoundManager::shared()->stopEffect();

	LogicFreeMoney* pLogic = LogicFreeMoney::getInstance();

	int nTipId = pLogic->getDailyPrize().wType == 0 ? 11 : 12;
	int nGetNum = pLogic->getDailyPrize().wType == 0 ? pLogic->getDailyPrize().dwGetGold : pLogic->getDailyPrize().dwGetYuanbao;
	HallGameScene::getInstance()->openMessageTip(StringUtils::format(GETSTRING(nTipId).c_str(), nGetNum).c_str());

	if (pLogic->getDailyPrize().wType == 0)
	{
		Player::getInstance()->addCoin(pLogic->getDailyPrize().dwGetGold);
		HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
	}
	else
	{
		Player::getInstance()->addGold(pLogic->getDailyPrize().dwGetYuanbao);
		HALL_NOTIFICATION(Hall_Message_Refresh_Gold, nullptr);
	}
}