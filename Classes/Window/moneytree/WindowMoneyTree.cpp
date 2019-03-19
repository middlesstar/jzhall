#include "WindowMoneyTree.h"
#include "LogicMoneyTree.h"
#include "Player/Player.h"
#include "Util/GameUtil.h"
#include "LanguageConfig.h"

#define TREE_SAY_CD		20			//摇钱树说话cd

const string strMoneyCDSchedule = "__MoneyTree_Schedule";
const string strTreeSaySchedule = "__TreeSay_Schedule";
const string strCoinName = "_Coin";

WindowMoneyTree::WindowMoneyTree()
	: m_pTreeSaySprite(nullptr)
	, m_pCoinSprite(nullptr)
	, m_pImgSharkTip(nullptr)
	, m_pGetTimeCD(nullptr)
	, m_pTreeSayText(nullptr)
	, m_pSpriteVipInfo(nullptr)
	, m_pSpriteVipTop(nullptr)
	, m_nScheduleTimer(0)
	, m_nShakeCount(0)
{
	memset(m_paCoinNode, 0, sizeof(m_paCoinNode));

	HALL_NOTIFICATION_REG(Hall_Message_Refresh_VIP, WindowMoneyTree::_refreshVipInfo, nullptr);
}

WindowMoneyTree::~WindowMoneyTree()
{
	unschedule(strMoneyCDSchedule);

	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_VIP);
}

void WindowMoneyTree::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZMoneyTree.csb");
	CCASSERT(pMainNode, "WindowMoneyTree::setContent() layout/JZMoneyTree.json load nullptr!");

	m_pNode->addChild(pMainNode);

	m_pTreeSaySprite = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode,"tree_dialog"));
	m_pCoinSprite = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "img_tree_coin"));
	m_pGetTimeCD = dynamic_cast<Text*>(_seekNodeByName(pMainNode, "label_cd"));
	m_pTreeSayText = dynamic_cast<Text*>(_seekNodeByName(m_pTreeSaySprite, "text_tip"));

	m_pSpriteVipInfo = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "tree_level"));
	m_pSpriteVipTop = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "tree_level_top"));

	m_pImgSharkTip = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode,"img_shark_tip"));
	m_pImgSharkTip->setOpacity(0);

	m_pTreeSaySprite->setOpacity(0);

	Node* pTreeParentNode = _seekNodeByName(pMainNode, "tree");
	for (int i = 0; i < TREE_COIN_MAX_NODE; i++)
	{
		m_paCoinNode[i] = _seekNodeByName(pTreeParentNode, StringUtils::format("money_node%d",i));
	}

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		/* -----------关闭 按钮------------- */
		this->close();
	});

	_addButtonClickEvent(pMainNode, "btn_recharge", [this](Ref* sender)
	{
		/* -----------立即充值 按钮------------- */
		//_shakeTree();
		HallGameScene::getInstance()->openWindow(EWT_Shop);
	});

	_addButtonClickEvent(pMainNode, "btn_tree_upgrade", [this](Ref* sender)
	{
		HallGameScene::getInstance()->openWindow(EWT_TreeUpgrade);
	});

	_showTreeCoin();
	_setPlayerInfo();
	_setMoneyStaticInfo();
	_setMoneyDynamicInfo();

	schedule([this](float dt)
	{
		_setMoneyDynamicInfo();

		m_nScheduleTimer = ++m_nScheduleTimer % TREE_SAY_CD;
		if (m_nScheduleTimer == 0)
		{
			_moneyTreeSay(EMTS_Idle, false);
		}

	}, 1.0f, strMoneyCDSchedule);
}

void WindowMoneyTree::updateWindow(int nCode)
{
	if (nCode == EMTT_REQUEST_MONEY)
	{
		_moneyTreeSay(EMTS_Get,true);
		_runCoinDropAction();
	}
	else
	{
		_setPlayerInfo();
		_setMoneyStaticInfo();
		_setMoneyDynamicInfo();
	}
}

void WindowMoneyTree::onEnter()
{
	WindowBase::onEnter();

	Device::setAccelerometerEnabled(true);
	_setAcceleration();
}

void WindowMoneyTree::onExit()
{
	Device::setAccelerometerEnabled(false);

	WindowBase::onExit();
}

void WindowMoneyTree::_setAcceleration()
{
	auto acceListener = EventListenerAcceleration::create(CC_CALLBACK_2(WindowMoneyTree::_onMoneyTreeAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(acceListener, this);
}

void WindowMoneyTree::_onMoneyTreeAcceleration(Acceleration* acc, Event* unused_event)
{
	static Acceleration lastAcc = *acc;
	const double dCurTimeStamp = acc->timestamp;
	const double dLentthSq = Vec3(acc->x - lastAcc.x, acc->y - lastAcc.y, acc->z - lastAcc.z).length();
	if (dLentthSq > 1.0f)
	{
		unsigned int nowMS = GameUtil::getMillisecond();
		static unsigned int lLastTime = 0;
		if (nowMS - lLastTime > 700)
		{
			log("move %lf , len %lf", dCurTimeStamp, dLentthSq);
			lLastTime = nowMS;
			++m_nShakeCount;

			if (m_nShakeCount >= 2)
			{
				_shakeTree();
				m_nShakeCount = 0;
			}
		}

	}

	lastAcc = *acc;
}

void WindowMoneyTree::_shakeTree()
{
	LogicMoneyTree* pLogic = LogicMoneyTree::getInstance();

	if (pLogic->getMoneyGetTime() > GameUtil::getMillisecond() / 1000)
	{
		_moneyTreeSay(EMTS_Shake,true);
	}
	else
	{
		pLogic->requestMoneyTree(EMTT_REQUEST_MONEY);
	}

	SoundManager::shared()->playEffectNormal(SoundManager::shared()->getHallMusicFileFullPath("shake").c_str());
}

void WindowMoneyTree::_showTreeCoin()
{
	for (int i = 0; i < TREE_COIN_MAX_NODE; i++)
	{
		m_paCoinNode[i]->removeAllChildrenWithCleanup(true);
	}

	//根据当前能领的金币，计算出应该显示出的金币,总数是36个
	LogicMoneyTree* pLogic = LogicMoneyTree::getInstance();

	float fTreeProgress = (float)pLogic->getMoneyTreeData().GetGold / pLogic->getMoneyTreeData().TotelGold;
	int nTreeCoinNum = fTreeProgress * TREE_COIN_MAX_NODE;

	vector<int>& coinVec = pLogic->getCoinVection();

	if (nTreeCoinNum > coinVec.size())
	{
		GameUtil::getMuiltRandomNum(0, TREE_COIN_MAX_NODE - 1, nTreeCoinNum, coinVec);
	}

	for (int i = 0; i < coinVec.size(); i++)
	{
		if (coinVec.at(i) >= 0 && coinVec.at(i) < TREE_COIN_MAX_NODE)
		{
			_addCoinToNode(m_paCoinNode[coinVec.at(i)]);
		}
	}
}

void WindowMoneyTree::_addCoinToNode(Node* pParent)
{
	if (!pParent)
	{
		return;
	}

	ImageView* pImgCoin = ImageView::create("images/hall/tree/tree_coin.png");
	pParent->addChild(pImgCoin, 10000, strCoinName);
}

void WindowMoneyTree::_runCoinDropAction()
{
	Node* pNode = nullptr;
	int nRandomDistance = 0;
	float yPos = 0;
	for (int i = 0; i < TREE_COIN_MAX_NODE; i++)
	{
		pNode = m_paCoinNode[i]->getChildByName(strCoinName);
		yPos = m_paCoinNode[i]->getPositionY();
		nRandomDistance = GameUtil::getRandomNum(0, 100);
		if (pNode)
		{
			pNode->runAction(Sequence::create(EaseOut::create(MoveBy::create(CCRANDOM_0_1() * 0.8f + 0.8f, Vec2(0, -MAX(0, yPos - nRandomDistance))),0.25f), RemoveSelf::create(true), NULL));
		}
	}
}

void WindowMoneyTree::_setPlayerInfo()
{
	Text* pName = dynamic_cast<Text*>(_seekNodeByName(m_pNode,"text_name"));
	Text* pCoin = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "text_money"));
	Sprite* pSpriteIcon = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode,"img_icon"));

	Player* pPlayer = Player::getInstance();
	pName->setString(StringUtils::format("%s", pPlayer->getLogonData().szNickName));
	pCoin->setString(StringUtils::format("%d", pPlayer->getLogonData().lUserScore));
	pSpriteIcon->setTexture(StringUtils::format("images/common/head/head_%d.png", pPlayer->getLogonData().wFaceID));
}

void WindowMoneyTree::_setMoneyStaticInfo()
{
	//可领取金币，每天最大金币数，VIP等级，树上的金币图片
	Text* pTextCoin = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "label_coin"));
	Text* pTextMaxCoin = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "label_maxconin"));
	Text* pTextVip = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "label_lv"));

	LogicMoneyTree* pLogic = LogicMoneyTree::getInstance();
	int nVip = Player::getInstance()->getHallData().dwVipLevel;
	pTextCoin->setString(StringUtils::format("%d", pLogic->getMoneyTreeData().GetGold));
	pTextMaxCoin->setString(StringUtils::format("%d", pLogic->getMoneyTreeData().TotelGold));
	pTextVip->setString(StringUtils::format("%d", nVip));

	_setVipInfo();
}

void WindowMoneyTree::_setMoneyDynamicInfo()
{
	if (m_pGetTimeCD)
	{
		LogicMoneyTree* pLogic = LogicMoneyTree::getInstance();

		if (pLogic->getMoneyGetTime() == 0xffffffff)
		{
			m_pGetTimeCD->setString("");
			return;
		}

		unsigned int uNowTime = GameUtil::getMillisecond() / 1000;
		if (pLogic->getMoneyGetTime() > uNowTime)
		{
			unsigned int nCoolDown = pLogic->getMoneyGetTime() - uNowTime;
			m_pGetTimeCD->setString(StringUtils::format("%02d:%02d:%02d", nCoolDown / 3600, (nCoolDown % 3600) / 60, nCoolDown % 60));

			m_pImgSharkTip->stopAllActions();
			m_pImgSharkTip->setOpacity(0);
		}
		else
		{
			m_pGetTimeCD->setString(GETSTRING(10));

			if (m_pImgSharkTip->numberOfRunningActions() == 0)
			{
				m_pImgSharkTip->runAction(RepeatForever::create(Sequence::create(FadeIn::create(2.0f), FadeOut::create(2.0f), NULL)));
			}
		}
	}
}

void WindowMoneyTree::_moneyTreeSay(int nTreeState, bool bForceCover)
{
	if (!m_pTreeSaySprite || !m_pTreeSayText)
	{
		return;
	}

	if (m_pTreeSaySprite->getNumberOfRunningActions() > 0 && !bForceCover)
	{
		return;
	}

	//摇钱 1100-1105，空闲 1106-1124，获得金币1125-1127
	int nSayId = 100;
	switch (nTreeState)
	{
	case EMTS_Idle:
		nSayId = GameUtil::getRandomNum(1106, 1124);
		break;
	case EMTS_Shake:
		nSayId = GameUtil::getRandomNum(1100, 1105);
		break;
	case EMTS_Get:
		nSayId = GameUtil::getRandomNum(1125, 1127);
		break;
	default:
		break;
	}

	m_pTreeSayText->setString(GETSTRING(nSayId));
	m_pTreeSaySprite->setOpacity(0);
	m_pTreeSaySprite->stopAllActions();
	m_pTreeSaySprite->runAction(Sequence::create(FadeIn::create(0.8f), DelayTime::create(2.0f),FadeOut::create(3.0f),NULL));
}

void WindowMoneyTree::_setVipInfo()
{
	int nVip = Player::getInstance()->getHallData().dwVipLevel;

	if (nVip < 10)
	{
		m_pSpriteVipInfo->setVisible(true);
		m_pSpriteVipTop->setVisible(false);

		TextAtlas* pTextCurVip = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "label_curvip"));
		TextAtlas* pTextNextVip = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "label_nextvip"));
		TextAtlas* pTextNeedRecharge = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "label_recharge"));

		LoadingBar* pRechargeProgress = dynamic_cast<LoadingBar*>(_seekNodeByName(m_pNode, "vip_pb"));


		pTextCurVip->setString(StringUtils::format("%d", nVip));
		pTextNextVip->setString(StringUtils::format("%d", nVip + 1));
		pTextNeedRecharge->setString(StringUtils::format("%d", Player::getInstance()->getNextVipNeedRecharge()));
		pRechargeProgress->setPercent(Player::getInstance()->getCurVipRechargePercent());
	}
	else
	{
		m_pSpriteVipInfo->setVisible(false);
		m_pSpriteVipTop->setVisible(true);
	}
	

	//设置VIP每天可领取数额
	Text* pTextNextVipGet = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "text_nextvip"));
	Text* pTextGetCoin = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "text_getcoin"));

	int nPresentCount[] = {0, 10000, 20000, 30000, 60000, 100000, 200000, 300000, 500000, 800000, 1000000 };	//每日赠送
	if (nVip + 1 < countarray(nPresentCount))
	{
		pTextNextVipGet->setString(StringUtils::format("%d", nVip + 1));
		pTextGetCoin->setString(StringUtils::format("%d", nPresentCount[nVip + 1]));
	}
}

void WindowMoneyTree::_refreshVipInfo(Ref* pRef)
{
	_setVipInfo();
}