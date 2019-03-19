#include "WindowPromote.h"
#include "LogicPromote.h"

WindowPromote::WindowPromote()
	: m_pNodePromote(nullptr)
	, m_pNodePrize(nullptr)
	, m_pNodeHow(nullptr)
	, m_pNodeRank(nullptr)
{

}

WindowPromote::~WindowPromote()
{
	CC_SAFE_RELEASE_NULL(m_pItem);
}

void WindowPromote::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZPromote.csb");
	CCASSERT(pMainNode, "WindowPromote::setContent() layout/JZPromote.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();
	
	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	m_pNodePromote = _seekNodeByName(pMainNode, "node_promote");
	m_pNodePrize = _seekNodeByName(pMainNode, "node_prize");
	m_pNodeHow = _seekNodeByName(pMainNode, "node_how");
	m_pNodeRank = _seekNodeByName(pMainNode, "node_rank");

	m_pScroll = dynamic_cast<ui::ScrollView*>(_seekNodeByName(m_pNodeRank, "scroll"));
	m_pItem = dynamic_cast<ImageView*>(m_pScroll->getChildByName("item"));
	m_pItem->removeFromParent();
	m_pItem->retain();

	Sprite* pBtnSpriteBg = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "sprite_bg"));

	m_pImgPromote = dynamic_cast<ImageView*>(_seekNodeByName(pBtnSpriteBg, "img_promote"));
	m_pImgPrize = dynamic_cast<ImageView*>(_seekNodeByName(pBtnSpriteBg, "img_prize"));
	m_pImgGetPacket = dynamic_cast<ImageView*>(_seekNodeByName(pBtnSpriteBg, "img_how"));
	m_pImgRank = dynamic_cast<ImageView*>(_seekNodeByName(pBtnSpriteBg, "img_rank"));

	m_pImgSelect = m_pImgPromote;

	m_pImgPromote->setTag(EWPS_PROMOTE - 1);
	m_pImgPrize->setTag(EWPS_PRIZE - 1);
	m_pImgGetPacket->setTag(EWPS_HOW - 1);
	m_pImgRank->setTag(EWPS_RANK - 1);

	m_pImgPromote->addClickEventListener(CC_CALLBACK_1(WindowPromote::_tabClickCallback, this));
	m_pImgPrize->addClickEventListener(CC_CALLBACK_1(WindowPromote::_tabClickCallback, this));
	m_pImgGetPacket->addClickEventListener(CC_CALLBACK_1(WindowPromote::_tabClickCallback, this));
	m_pImgRank->addClickEventListener(CC_CALLBACK_1(WindowPromote::_tabClickCallback, this));

	ImageView* pImg = nullptr;
	for (int i = 1; i <= 4; i++)
	{
		pImg = dynamic_cast<ImageView*>(_seekNodeByName(m_pNodePromote, StringUtils::format("img_box%d", i)));
		pImg->setTag(i);
		pImg->addClickEventListener(CC_CALLBACK_1(WindowPromote::_boxClickCallback, this));
	}

	_addButtonClickEvent(m_pNodePromote, "btn_look", [this](Ref* sender)
	{
		//查看奖励,打开红包收益界面
		_tabClickCallback(m_pImgPrize);
	});

	_addButtonClickEvent(m_pNodePrize, "btn_getprize", [this](Ref* sender)
	{
		//领取自己的红包收益
		LogicPromote* pLogic = LogicPromote::getInstance();
		if (pLogic->getGetCoinNum() == 0 && pLogic->getGoldNum() == 0)
		{
			HallGameScene::getInstance()->openMessageTip(46);
			return;
		}

		pLogic->requestShareInfo(2);
	});

	_addButtonClickEvent(m_pNodePromote, "btn_share", [this](Ref* sender){HallGameScene::getInstance()->openWindow(EWT_Share); });
	_addButtonClickEvent(m_pNodeHow, "Button_5", CC_CALLBACK_1(WindowPromote::_getSharePrizeCallback,this));

	_setUIState(EWPS_PROMOTE);
	_setShareDayInfo();
}

void WindowPromote::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowPromote::updateWindow(int nCode, void* pData)
{
	switch (nCode)
	{
	case EWPS_SHAREDAYS:
		_setShareDayInfo();
		break;
	case EWPS_PRIZE:
		_setPromotePrize((CMD_ST_ShouYiRsp*)pData);
		break;
	case EWPS_RANK:
		_setPromoteRank((CMD_ST_ShouRankYiRsp*)pData);
		break;
	default:
		break;
	}
}

void WindowPromote::_setUIState(int nState)
{
	switch (nState)
	{
	case EWPS_PROMOTE:
		m_pNodePromote->setVisible(true);
		m_pNodePrize->setVisible(false);
		m_pNodeHow->setVisible(false);
		m_pNodeRank->setVisible(false);
		break;
	case EWPS_PRIZE:
		m_pNodePromote->setVisible(false);
		m_pNodePrize->setVisible(true);
		m_pNodeHow->setVisible(false);
		m_pNodeRank->setVisible(false);
		break;
	case EWPS_HOW:
		m_pNodePromote->setVisible(false);
		m_pNodePrize->setVisible(false);
		m_pNodeHow->setVisible(true);
		m_pNodeRank->setVisible(false);
		break;
	case EWPS_RANK:
		m_pNodePromote->setVisible(false);
		m_pNodePrize->setVisible(false);
		m_pNodeHow->setVisible(false);
		m_pNodeRank->setVisible(true);
		break;
	default:
		break;
	}
}

int WindowPromote::_getShareDayLevel(int nShareDay)
{
	int nShareDaysArray[] = {1, 5, 10, 30 };
	int i = 0;

	for (; i < countarray(nShareDaysArray); i++)
	{
		if (nShareDay < nShareDaysArray[i])
		{
			break;
		}
	}

	return i;
}

float WindowPromote::_getShareDayProgress(int nShareDay)
{
	float fProgress = 0.f;
	int nShareDaysArray[] = { 1, 5, 10, 30 };
	int nProgressArray[] = {14, 44, 74 };

	int nLevel = _getShareDayLevel(nShareDay);
	if (nLevel == 0)
	{
		fProgress = 0.f;
	}
	else if (nLevel >= countarray(nShareDaysArray))
	{
		fProgress = 100.0f;
	}
	else
	{
		fProgress = (nShareDay - nShareDaysArray[nLevel - 1]) * 10.0f / (nShareDaysArray[nLevel] - nShareDaysArray[nLevel - 1]) + nProgressArray[nLevel - 1];
	}

	return fProgress;
}

void WindowPromote::_setShareDayInfo()
{
	int nShareDaysArray[] = {1,5,10,30};

	LogicPromote* pLogic = LogicPromote::getInstance();
	Text* pTextShareDays = dynamic_cast<Text*>(_seekNodeByName(m_pNodePromote, "label_share_times"));
	pTextShareDays->setString(StringUtils::format("%d", pLogic->getShareDay()));

	LoadingBar* pShareProgress = dynamic_cast<LoadingBar*>(_seekNodeByName(m_pNodePromote, "pg_share"));
	pShareProgress->setPercent(_getShareDayProgress(pLogic->getShareDay()));

	//设置宝箱
	ImageView* pImg = nullptr;
	int nLevel = _getShareDayLevel(pLogic->getShareDay());
	for (int i = 1; i <= 4; i++)
	{
		pImg = dynamic_cast<ImageView*>(_seekNodeByName(m_pNodePromote, StringUtils::format("img_box%d",i)));

		if (nLevel >= i)
		{
			pImg->setTouchEnabled(i > pLogic->getGetIndex());
			if (i > pLogic->getGetIndex())
			{
				pImg->loadTexture("images/hall/promote/promote_box_1.png", TextureResType::PLIST);
				_playShakeTipActiom(pImg);
			}
			else
			{
				pImg->loadTexture("images/hall/promote/promote_box_2.png", TextureResType::PLIST);
				_stopShakeTipActiom(pImg);
			}
		}
	}
}

void WindowPromote::_setPromotePrize(CMD_ST_ShouYiRsp* pData)
{
	if (!pData)
	{
		return;
	}

	Text* pTextPromoteNum = dynamic_cast<Text*>(_seekNodeByName(m_pNodePrize, "label_promotenum"));
	pTextPromoteNum->setString(StringUtils::format("%d", pData->hongbao));

	Text* pTextCoinNum = dynamic_cast<Text*>(_seekNodeByName(m_pNodePrize, "Text_1"));
	pTextCoinNum->setString(StringUtils::format("%d", pData->gold));
	
	Text* pTextGoldNum = dynamic_cast<Text*>(_seekNodeByName(m_pNodePrize, "Text_3"));
	pTextGoldNum->setString(StringUtils::format("%d", pData->money));
}

void WindowPromote::_setPromoteRank(CMD_ST_ShouRankYiRsp* pData)
{
	if (!pData)
	{
		return;
	}

	m_pScroll->removeAllChildren();

	ImageView* pItem = nullptr;
	float fYPos = m_pItem->getPositionY();
	int nRankId = 1;
	for (int i = 0; i < countarray(pData->ranks); i++)
	{
		if (pData->ranks[i].userid > 0)
		{
			pItem = dynamic_cast<ImageView*>(m_pItem->clone());
			_setRankItem(pItem, &pData->ranks[i], nRankId);
			
			pItem->setPositionY(fYPos);
			fYPos = fYPos - 38.f;
			nRankId++;
			m_pScroll->addChild(pItem);
		}
	}
}

void WindowPromote::_setRankItem(ImageView* pItem, SyRank* pData, int nIndex)
{
	if (!pItem || !pData)
	{
		return;
	}

	Text* pTextRank = dynamic_cast<Text*>(pItem->getChildByName("text_rank"));
	pTextRank->setString(StringUtils::format("%d", nIndex));

	Text* pTextAccount = dynamic_cast<Text*>(pItem->getChildByName("label_account"));
	pTextAccount->setString(StringUtils::format("%s", pData->account));

	Text* pTextPromoteCount = dynamic_cast<Text*>(pItem->getChildByName("label_players"));
	pTextPromoteCount->setString(StringUtils::format("%d", pData->nums));

	Text* pTextGetGold = dynamic_cast<Text*>(pItem->getChildByName("label_payment"));
	pTextGetGold->setString(StringUtils::format("%d", pData->moneys));
}

void WindowPromote::_playShakeTipActiom(Widget* pWidget)
{
	if (!pWidget)
	{
		return;
	}

	pWidget->stopAllActions();

	auto rotateAction = Sequence::create(RotateTo::create(0.06f, 20.0f), RotateTo::create(0.12f, -20.0f),
		RotateTo::create(0.09f, 10.0f), RotateTo::create(0.06f, -10.0f), RotateTo::create(0.03, 0), NULL);
	auto scaleAction = Sequence::create(ScaleTo::create(0.6f, 1.2f), ScaleTo::create(0.6f, 1.0f), NULL);
	pWidget->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.0f), EaseSineInOut::create(rotateAction), scaleAction, NULL)));
}

void WindowPromote::_stopShakeTipActiom(Widget* pWidget)
{
	if (!pWidget)
	{
		return;
	}

	pWidget->stopAllActions();
	pWidget->setScale(1.0f);
	pWidget->setRotation(0);
}

void WindowPromote::_getSharePrizeCallback(Ref* pRef)
{
	TextField* pInput = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeHow,"TextField_2"));

	if (pInput && pInput->getString().length() <= 8)		//分享码最长8位
	{
		LogicPromote::getInstance()->requestGetSharePrize(pInput->getString());
	}
}

void WindowPromote::_tabClickCallback(Ref* pRef)
{
	playButtonClickSound();

	ImageView* pImg = dynamic_cast<ImageView*>(pRef);

	if (!pImg || pImg == m_pImgSelect)
	{
		return;
	}

	m_pImgSelect->loadTexture(StringUtils::format("images/hall/promote/promote_left_%d.png", m_pImgSelect->getTag() + 100), TextureResType::PLIST);
	pImg->loadTexture(StringUtils::format("images/hall/promote/promote_left_%d.png", pImg->getTag()), TextureResType::PLIST);
	m_pImgSelect = pImg;

	switch ((pImg->getTag() + 1))
	{
	case EWPS_PROMOTE:
		this->_setUIState(EWPS_PROMOTE);
		break;
	case EWPS_PRIZE:
		//打开红包收益界面
		this->_setUIState(EWPS_PRIZE);
		LogicPromote::getInstance()->requestShareInfo(1);
		break;
	case EWPS_HOW:
		this->_setUIState(EWPS_HOW);
		break;
	case EWPS_RANK:
		//打开收益排行界面
		this->_setUIState(EWPS_RANK);
		LogicPromote::getInstance()->requestShareInfo(3);
		break;
	default:
		break;
	}
}

void WindowPromote::_boxClickCallback(Ref* pRef)
{
	LogicPromote::getInstance()->requestMySharePrize();
}