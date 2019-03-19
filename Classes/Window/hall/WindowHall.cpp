#include "WindowHall.h"
#include "Window/moneytree/LogicMoneyTree.h"
#include "Window/promote/LogicPromote.h"
#include "LanguageConfig.h"
#include "Player/Player.h"
#include "Window/mail/LogicMail.h"
#include "Window/activity/LogicActivity.h"
#include "Window/activity/GrabRed/GrabRed.h"
#include "Util/GameUtil.h"

#define ROTATE_ANGLE		20.0f
#define ROTATE_TIME			1.5f

WindowHall::WindowHall()
	: m_pHeadIcon(nullptr)
	, m_pCycleScroll(nullptr)
	, m_pLeftLight(nullptr)
	, m_pSelectLight(nullptr)
	, m_pRightLight(nullptr)
	, m_pSelectEffect(nullptr)
	, m_pBgLight(nullptr)
	, m_pRechargeLight(nullptr)
	, m_pRecharge(nullptr)
	, m_pLeftTime(nullptr)
	, m_grabRed(nullptr)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_NickName, WindowHall::refreshNickName, nullptr);
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin, WindowHall::refreshCoin, nullptr);
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Gold, WindowHall::refreshGold, nullptr);
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_VIP, WindowHall::refreshVip, nullptr);

	//m_vecHallData.push_back(sUnitInfo("images/hall/game_hall/gh_forest_dance.png", "effect/slwh.plist", EHST_SLWH));
	//m_vecHallData.push_back(sUnitInfo("images/hall/game_hall/gh_fishing.png", "effect/buyu.plist", EHST_SHARK));
	//m_vecHallData.push_back(sUnitInfo("images/hall/game_hall/gh_water_margin.png", "effect/shuihu.plist", EHST_SHUIHU));
	
	m_vecHallData.push_back(sUnitInfo("images/hall/game_hall/gh_water_margin.png", "effect/shuihu.plist", EHST_SHUIHU));
	m_vecHallData.push_back(sUnitInfo("images/hall/game_hall/gh_forest_dance.png", "effect/slwh.plist", EHST_SLWH));
	m_vecHallData.push_back(sUnitInfo("images/hall/game_hall/gh_fishing.png", "effect/buyu.plist", EHST_SHARK));
}

WindowHall::~WindowHall()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_NickName);
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Gold);
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_VIP);
}

void WindowHall::onEnter()
{
	WindowBase::onEnter();

	SoundManager* pSound = SoundManager::shared();
	SimpleAudioEngine::sharedEngine()->preloadEffect(pSound->getHallMusicFileFullPath("shuihu").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(pSound->getHallMusicFileFullPath("senlin").c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(pSound->getHallMusicFileFullPath("jinsha").c_str());
}

void WindowHall::onExit()
{
	SoundManager* pSound = SoundManager::shared();
	SimpleAudioEngine::sharedEngine()->unloadEffect(pSound->getHallMusicFileFullPath("shuihu").c_str());
	SimpleAudioEngine::sharedEngine()->unloadEffect(pSound->getHallMusicFileFullPath("senlin").c_str());
	SimpleAudioEngine::sharedEngine()->unloadEffect(pSound->getHallMusicFileFullPath("jinsha").c_str());

	WindowBase::onExit();
}

void WindowHall::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZHall.csb");
	CCASSERT(pMainNode, "WindowHall::setContent() layout/JZHall.json load nullptr!");

	m_pNode->addChild(pMainNode);

	m_pHeadIcon = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode,"head"));
 	m_pHeadIcon->addClickEventListener([this](Ref* sender)
 	{
 		HallGameScene::getInstance()->openWindow(EWT_Personal);
 		playButtonClickSound();
 	});

	m_pLeftLight = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode,"img_light_left"));
	m_pSelectLight = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, "img_select_light"));
	m_pRightLight = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, "img_light_right"));
	m_pSelectEffect = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, "img_select_effect"));
	m_pRechargeLight = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, "gh_recharge_light"));
	m_pBgLight = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "img_light"));
	m_pLeftTime = dynamic_cast<TextAtlas*>(_seekNodeByName(pMainNode,"gh_timecount"));
	m_grabRed = dynamic_cast<ImageView*>(_seekNodeByName(pMainNode, "gh_activity"));


	m_pLeftLight->runAction(RepeatForever::create(Sequence::create(RotateTo::create(ROTATE_TIME, -ROTATE_ANGLE), RotateTo::create(ROTATE_TIME, ROTATE_ANGLE), NULL)));
	m_pRightLight->runAction(RepeatForever::create(Sequence::create(RotateTo::create(ROTATE_TIME, ROTATE_ANGLE), RotateTo::create(ROTATE_TIME, -ROTATE_ANGLE), NULL)));

	m_pSelectLight->setOpacity(0);
	m_pSelectEffect->setOpacity(0);

	m_pRecharge = dynamic_cast<Button*>(_seekNodeByName(pMainNode, "gh_recharge"));
	m_pRecharge->addClickEventListener([this](Ref* sender)
	{
		/* -----------充值 按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_Shop);
		playButtonClickSound();
	});

	m_grabRed->addClickEventListener([this](Ref* sender)
	{
		/* -----------活动------------- */
		LogicActivity::getInstance()->requestGrabRed();
		playButtonClickSound();
	});

	_updateUserIcon();
	_setUserInfo();
	
	_addButtonClickEvent(pMainNode, "btn_back", [](Ref* sender)
	{
		/* -----------返回 按钮------------- */
		HallGameScene::getInstance()->openOkCancelDialog(GETSTRING(6), nullptr, ECCNT_BACKTOLOGIN);
	});

	_addButtonClickEvent(pMainNode, "gh_tree", [](Ref* sender)
	{
		/* -----------摇钱树 按钮------------- */
		//HallGameScene::getInstance()->openWindow(EWT_MoneyTree);
		LogicMoneyTree::getInstance()->requestMoneyTree(EMTT_GET_DATA);
	});

	_addButtonClickEvent(pMainNode, "gh_benefits", [](Ref* sender)
	{
		/* -----------免费金币 按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_FreeMoney);
	});
	
	_addButtonClickEvent(pMainNode, "gh_ranking", [this](Ref* sender)
	{
		/* -----------排行榜 按钮------------- */
		//Player::getInstance()->requsetServerList();
		HallGameScene::getInstance()->openMessageTip(4);
	});

	_addButtonClickEvent(pMainNode, "gh_promote", [](Ref* sender)
	{
		/* -----------红包 按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_Promote);
		LogicPromote::getInstance()->requestShareDays();
	});

	_addButtonClickEvent(pMainNode, "gh_exchange", [](Ref* sender)
	{
		/* -----------兑奖 按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_Exchange);
	});

	_addButtonClickEvent(pMainNode, "gh_expedition", [](Ref* sender)
	{
		/* -----------远征 按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_Expedition);
		//HallGameScene::getInstance()->openMessageTip(4);
	});

	_addButtonClickEvent(pMainNode, "gh_vipgift", [](Ref* sender)
	{
		/* -----------VIP礼包 按钮------------- */
		if (Player::getInstance()->getHallData().ritems[1] == 0)
		{
			HallGameScene::getInstance()->openWindow(EWT_VipGift);
		}
		else
		{
			HallGameScene::getInstance()->openWindow(EWT_Shop);
			HallGameScene::getInstance()->updateWindow(EWT_Shop);
		}
	});

	//if (Player::getInstance()->getHallData().iscomment != 0)
	if (Player::getInstance()->getHallData().iscomment != 0 && GameUtil::getData("UserComment", 0) == 1)
	{
		_dismissComment();
	}
	else
	{
		_addButtonClickEvent(pMainNode, "gh_comment", [](Ref* sender)
		{
			/* -----------好评有礼 按钮------------- */
			HallGameScene::getInstance()->openWindow(EWT_Comment);
			//HallGameScene::getInstance()->openMessageTip(4);
		});
	}

	

	_addButtonClickEvent(pMainNode, "gh_bank", [](Ref* sender)
	{
		/* -----------银行 按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_Bank);
	});

	_addButtonClickEvent(pMainNode, "gh_mail", [](Ref* sender)
	{
		/* -----------邮件 按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_Mail);
		LogicMail::getInstance()->requestMails();
	});

	_addButtonClickEvent(pMainNode, "gh_settings", [this](Ref* sender)
	{
		/* -----------设置 按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_Setting);
	});

	Node* pNodeScroll = _seekNodeByName(pMainNode, "node_scroll");
	m_pCycleScroll = CycleScroll::create(m_vecHallData);
	m_pCycleScroll->setSelectDelegate(this);
	pNodeScroll->addChild(m_pCycleScroll);

	_playSelectEffect();
	_playShopButtonAction();
	_initPlatform();

	//开启福袋倒计时
	dword time = Player::getInstance()->getHallData().fudaitime;
	fdSetTime(time);
	StartTimeCount(time);

	HallGameScene::openChatSprite(this);
}

void WindowHall::StartTimeCount(int t)
{	
	auto halldata = Player::getInstance()->getHallData();
	halldata.fudaitime = t;
	Player::getInstance()->setHallData(halldata);
	if (t == 0)
	{
		m_pLeftTime->setVisible(false);
		m_grabRed->loadTexture("images/hall/game_hall/gh_countdown2.png", TextureResType::PLIST);
	}
	else
	{
		m_pLeftTime->setVisible(true);
		m_grabRed->loadTexture("images/hall/game_hall/gh_countdown.png", TextureResType::PLIST);
	}

	schedule(schedule_selector(WindowHall::updateTime), 1.f);
}

void WindowHall::updateTime(float t)
{
	dword time = Player::getInstance()->getHallData().fudaitime;
	if ( time )
	{
		fdSetTime(time);
	}
	else
	{
		unschedule(schedule_selector(WindowHall::updateTime));
		m_pLeftTime->setVisible(false);
		m_grabRed->loadTexture("images/hall/game_hall/gh_countdown2.png", TextureResType::PLIST);
	}
	
		
}

void WindowHall::fdSetTime(int time)
{
	int h = time / 3600;
	int m = time % 3600 / 60;
	int s = time % 60;
	char buf[256];
	sprintf(buf, "%02d/%02d/%02d", h, m, s);
	m_pLeftTime->setString(buf);
}

void WindowHall::updateWindow(int nCode)
{
	switch (nCode)
	{
	case EHUC_ICON:
		_updateUserIcon();
		break;
	case ENUM_FUDAI_START:
		{
			_RUN_FISH_ACTIVETY(GrabRed, WindowHall::getInfo, this);
		}
		break;
	case ENUM_FUDAI_FAILURE:
		HallGameScene::getInstance()->openOkVipDialog(GETSTRING(1305));
	case ENUM_COMMENT_SUCCESS:
		_dismissComment();
		break;
	default:
		break;
	}
}

void WindowHall::onBackKeyPressed()
{
	HallGameScene::getInstance()->openOkCancelDialog(GETSTRING(6), nullptr, ECCNT_BACKTOLOGIN);
}

//抢福袋回调
void WindowHall::getInfo(int coinScore, float yuanbaoScore)
{
	log("------------coinScore = %d-----------------", coinScore);
	log("------------yuanBaoScore = %f--------------", yuanbaoScore);
	//抢福袋： 呼出结束对话框
	HallGameScene::getInstance()->openWindow(EWT_Activity);
	//懒得定义结构体了，不和谐的传输数据的方式，比较蛋疼
	int data = coinScore * 100 + (int)yuanbaoScore;
	HallGameScene::getInstance()->updateWindow(EWT_Activity, 0 ,(void*)(&data));
	//重置时间为4小时
	StartTimeCount(3600 * 4);
}

void WindowHall::onUnitSelect(const sUnitInfo* pSourceData)
{
	if (pSourceData)
	{
		HallGameScene::switchScene(pSourceData->m_nSceneId);
	}
}

void WindowHall::onUnitMoveBegin()
{
	_stopSelectEffect();
}

void WindowHall::onUnitMoveEnd()
{
	_playSelectEffect();
}

void WindowHall::refreshNickName(Ref* ref)
{
	Text* pName = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "gh_name"));
	pName->setString(Player::getInstance()->getLogonData().szNickName);
}

void WindowHall::refreshCoin(Ref* ref)
{
	TextAtlas* pMoney = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "label_money"));
	pMoney->setString(StringUtils::format("%d", Player::getInstance()->getLogonData().lUserScore));
}

void WindowHall::refreshGold(Ref* ref)
{
	TextAtlas* pGold = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "label_gold"));
	pGold->setString(StringUtils::format("%d", Player::getInstance()->getHallData().llYuanBao));
}

void WindowHall::refreshVip(Ref* ref)
{
	TextAtlas* pVipLevel = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "vip_level"));
	pVipLevel->setString(StringUtils::format("%d", Player::getInstance()->getHallData().dwVipLevel));
}

void WindowHall::_setUserInfo()
{
	Text* pName = dynamic_cast<Text*>(_seekNodeByName(m_pNode,"gh_name"));
	Text* pID = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "gh_id"));
	Text* pTitle = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "gh_title"));

	TextAtlas* pLevel = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "label_lv"));
	Sprite* pVip = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, "gh_vip"));
	TextAtlas* pVipLevel = dynamic_cast<TextAtlas*>(_seekNodeByName(pVip, "vip_level"));

	TextAtlas* pMoney = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "label_money"));
	TextAtlas* pGold = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, "label_gold"));

	Player* pPlayer = Player::getInstance();

	pName->setString(pPlayer->getLogonData().szNickName);
	pID->setString(StringUtils::format("ID:%d", pPlayer->getLogonData().dwGameID));
	pTitle->setString(GETSTRING(pPlayer->getPlayerLevel() / 10 + 70));

	pLevel->setString(StringUtils::format("%d", pPlayer->getPlayerLevel()));
	pVipLevel->setString(StringUtils::format("%d", pPlayer->getHallData().dwVipLevel));

	pMoney->setString(StringUtils::format("%d", pPlayer->getLogonData().lUserScore));
	pGold->setString(StringUtils::format("%d", pPlayer->getHallData().llYuanBao));

	log("------------luserScore1 = %lld-------------", pPlayer->getLogonData().lUserScore);

}

void WindowHall::_updateUserIcon()
{
	if (m_pHeadIcon)
	{
		m_pHeadIcon->loadTexture(StringUtils::format("images/common/head/head_%d.png", Player::getInstance()->getLogonData().wFaceID), TextureResType::PLIST);
	}
}

void WindowHall::_playSelectEffect()
{
	if (!m_pSelectLight || !m_pBgLight)
	{
		return;
	}

	m_pSelectLight->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.8f), FadeOut::create(0.8f), NULL)));
	m_pSelectEffect->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.8f), FadeOut::create(0.8f), NULL)));

	m_pBgLight->removeAllChildren();
	//随机加一些星星
	Size size = m_pBgLight->getContentSize();
	auto _emitter = ParticleSystemQuad::create("effect/xingxing.plist");
	_emitter->setPosition(Vec2(size.width/2,size.height/2));
	m_pBgLight->addChild(_emitter);
	_emitter->setScale(0.5f);
}

void WindowHall::_stopSelectEffect()
{
	m_pBgLight->removeAllChildren();
	m_pSelectLight->stopAllActions();
	m_pSelectEffect->stopAllActions();

	m_pSelectLight->setOpacity(0);
	m_pSelectEffect->setOpacity(0);
}

void WindowHall::_playShopButtonAction()
{
	if (!m_pRecharge || !m_pRechargeLight)
	{
		return;
	}
	m_pRecharge->stopAllActions();
	m_pRechargeLight->stopAllActions();

	auto rotateAction = Sequence::create(RotateTo::create(0.06f, 20.0f), RotateTo::create(0.12f, -20.0f),
		RotateTo::create(0.09f, 10.0f), RotateTo::create(0.06f, -10.0f), RotateTo::create(0.03, 0), NULL);
	auto scaleAction = Sequence::create(ScaleTo::create(0.6f, 1.2f), ScaleTo::create(0.6f, 1.0f), NULL);
	m_pRecharge->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.0f), EaseSineInOut::create(rotateAction), scaleAction,NULL)));

	//m_pRechargeLight->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.5f), FadeIn::create(1.5f), NULL)));
	m_pRechargeLight->runAction(RepeatForever::create(RotateBy::create(0.1f,1.f)));
}

void WindowHall::_dismissComment()
{
	Node* pBtn = _seekNodeByName(m_pNode, "gh_comment");
	pBtn->setVisible(false);
}

void WindowHall::_initPlatform()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (Player::getInstance()->getIosFlag() == 0)	//不开启
	{
		Button* pTree = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_tree"));
		Button* pBenefits = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_benefits"));
		Button* pRank = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_ranking"));
		Button* pExchange = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_exchange"));
		Button* pExpedition = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_expedition"));
		Button* pComment = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_comment"));
		Button* pPromote = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_promote"));
		Button* pVip = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_vipgift"));

		pTree->setVisible(false);
		pBenefits->setVisible(false);
		pRank->setVisible(false);
		pExpedition->setVisible(false);
		pExchange->setVisible(false);
		pComment->setVisible(false);
		pPromote->setVisible(false);
		pVip->setVisible(false);

		m_pHeadIcon->setTouchEnabled(false);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Button* pComment = dynamic_cast<Button*>(_seekNodeByName(m_pNode, "gh_comment"));
	pComment->setVisible(false);
#endif
	
}