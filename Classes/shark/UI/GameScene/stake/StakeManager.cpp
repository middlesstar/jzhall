#include "StakeManager.h"
#include "shark/NetWork/SharkNetworkKernel.h"
#include "HallGameScene.h"
#include "shark/Module/SharkNotice.h"
#include "LanguageConfig.h"
USING_NS_CC;

#define     RECHARGEOUT         "recharge_out"
#define     Shark_player_out    "playergoAway"
StakeManager::StakeManager()
:pMainNode(nullptr)
, m_auto(nullptr)
, m_cancel(nullptr)
, m_cancelAuto(nullptr)
, m_continue(nullptr)
, m_stake(nullptr)
,m_betText(nullptr)
, m_bIsAI(false)
, m_lastValidAll(_ZERO)
, m_lastGameAll(_ZERO)
, m_totleStake(nullptr)
, m_shieldLayer(nullptr)
, m_bIsenable(true)
, m_caijin(nullptr)

{
	memset(m_lastValidRecode, NULL, sizeof(m_lastValidRecode));
	HALL_NOTIFICATION_REG(Shark_player_out, StakeManager::playerOut, nullptr);
	//memset(m_rate, NULL, sizeof(m_rate));
}
StakeManager::~StakeManager()
{
	HALL_NOTIFICATION_UNREG(Shark_player_out);
	//playerOut(nullptr);
}

void StakeManager::setContent()
{
	m_auto = dynamic_cast<Button*>(_seekNodeByName(m_pNode,AUTOBTN));
	CC_ASSERT(m_auto != nullptr);
	m_auto->addTouchEventListener(this, toucheventselector(StakeManager::AIEvent));

	m_cancel = dynamic_cast<Button*>(_seekNodeByName(m_pNode, CANCELBTN));
	CC_ASSERT(m_cancel != nullptr);
	m_cancel->addTouchEventListener(this, toucheventselector(StakeManager::CancelJetEvent));

	m_cancelAuto = dynamic_cast<Button*>(_seekNodeByName(m_pNode, CANCELAUTO));
	CC_ASSERT(m_cancelAuto != nullptr);
	m_cancelAuto->addTouchEventListener(this, toucheventselector(StakeManager::AICancelEvent));

	m_continue = dynamic_cast<Button*>(_seekNodeByName(m_pNode, CONTINUEBTN));
	CC_ASSERT(m_continue != nullptr);
	m_continue->addTouchEventListener(this, toucheventselector(StakeManager::ContinuJetEvent));

	m_stake = dynamic_cast<Button*>(_seekNodeByName(m_pNode, STAKEBTN));
	CC_ASSERT(m_stake != nullptr);
	m_stake->addTouchEventListener(this, toucheventselector(StakeManager::JetAddEvent));

	m_betText = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, STALELABEL));
	CC_ASSERT(m_betText != nullptr);

	m_totleStake = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, TOTALSCORE));
	CC_ASSERT(m_totleStake != nullptr);

	m_caijin = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode, CAIJINSCORE));
	CC_ASSERT(m_caijin != nullptr);


	//��ɫ��
	m_shieldLayer = CCLayerColor::create(Color4B(_ZERO, _ZERO, _ZERO, 100), 800, 320);
	m_shieldLayer->setPosition(Vec2(_ZERO, _ZERO));
	m_shieldLayer->setTouchEnabled(true);
	m_shieldLayer->setVisible(false);
	m_pNode->addChild(m_shieldLayer);



	//��ʼ��һ��ѹ�ְ�ť
	SetJetNum(OneOfStake::SetAddJet(1));

	char str[20];
	for (int i = 0; i < Stake_Btn_Max; i++)
	{
		sprintf(str, "btn%d", i+1);
		Button* btn = dynamic_cast<Button*>(_seekNodeByName(m_pNode, str));
		m_stakeArray[i].dependNode(btn);
		if (i == 0)
		{
			m_stakeArray[i].SetTag(1000);
		}
		else if (i == 3)
		{
			m_stakeArray[i].SetTag(2000);
		}
		else
		{
			m_stakeArray[i].SetTag(i);
		}		
		btn->addTouchEventListener(&m_stakeArray[i], toucheventselector(OneOfStake::BtEvent));
	}


	//Ѻע��ʾ��ǩ
	auto jetallroot = m_pNode->getChildByName(STAKEALL);
	std::string strnode[] = STAKE_ALL_ARRAY;
	for (size_t i = _ZERO; i < Stake_Btn_Max; i++)
	{
		m_oneofjet[i] = SharkGoldHeap::create(28);
		m_oneofjet[i]->setPosition(Vec2(28, 50));
		jetallroot->getChildByName(strnode[i])->addChild(m_oneofjet[i]);
		//m_oneofjet[i]->StartPlay((int)i, i * 100);
	}

	//��ʼ���½�Ҷ�
	schedule(schedule_selector(StakeManager::updateGoldheap), 0.5f);

	//��ؽ������ݱ仯
	schedule(schedule_selector(StakeManager::updateMyStake), 0.05f);
	//��ʼ���ܷ�
	setMyTotelStake(_ZERO);

}

//����뿪ʱ�����б��ϵ���Ϣ
void StakeManager::playerOut(Ref* ref)
{
	//�������ѹ�ֽ׶Σ�������ѹ��
	auto gameInfo = GameInfo::GetInstance();
	if (gameInfo->getserverState() == STAKE)
	{		
		//�������ȡ��ѹ��
		CancelStake();
		//successCancelStake();
	}
	//����Լ���ť��Ѻ����Ϣ
		//ClearJetton();
		//�����ť��Ѻ����Ϣ
		auto roominfo = gameInfo->getroomInfor();
		roominfo->alljetinfo.lAllBet = 0;
		for (size_t i = _ZERO; i < Stake_Btn_Max; i++)
		{
			//roominfo->alljetinfo.lTotelBet[i] -= m_stakeArray[i].GetStake();
			//roominfo->alljetinfo.lAllBet -= m_stakeArray[i].GetStake();
			roominfo->alljetinfo.lTotelBet[i] = 0;
		}

}

//��˸��ť�ϵĹ�Ȧ
void  StakeManager::showCircle()
{
	for (int i = 0; i < Stake_Btn_Max; i++)
	{
		m_stakeArray[i].showCircle();
	}
}


//��ʾ��ת��Ȧ
void StakeManager::showRing()
{
	auto gameInfo = GameInfo::GetInstance();
	for (int i = 0; i < Stake_Btn_Max; i++)
	{
		int score = gameInfo->getownInfo().ownStake[i];
		if (score>0)
		{
			m_stakeArray[i].showRing();
		}
	}
}

//���ô�������
void StakeManager::setTouch(bool b)
{
	if (m_bIsenable == b)
	{
		return;
	}
	m_bIsenable = b;
	if (b)
	{
		m_shieldLayer->setVisible(false);
		getEventDispatcher()->removeEventListener(eventListener);
		eventListener = nullptr;
	}
	else
	{
		m_shieldLayer->setVisible(true);
		eventListener = EventListenerTouchOneByOne::create();
		eventListener->setSwallowTouches(true);

		eventListener->onTouchBegan = CC_CALLBACK_2(StakeManager::onTouchBegan, this);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, m_shieldLayer);
	}
}

bool StakeManager::onTouchBegan(Touch* touch, Event* event)
{
	if (m_bIsenable)
	{
		return false;
	}

	Vec2 pBeginPos = touch->getLocation();
	Size contentSize = m_shieldLayer->getContentSize();
	Vec2 pos = m_shieldLayer->getParent()->convertToWorldSpace(m_shieldLayer->getPosition());

	Rect rect = Rect(pos.x, pos.y, pos.x + contentSize.width, pos.y + contentSize.height);
	return rect.containsPoint(pBeginPos);
}
//���òʽ�
void StakeManager::setCaijin(SCORE score)
{
	std::stringstream str;
	str << score;
	m_caijin->setString(str.str());
}

//�����ҵ�Ѻ��
void StakeManager::updateMyStake(float t)
{
	if (GameInfo::GetInstance()->getserverState() == STAKE)
	{
		setMyTotelStake(OneOfStake::MyAllTotel);
	}
}

//�����ҵ���ѹ��
void StakeManager::setMyTotelStake(SCORE score)
{
	std::stringstream str;
	str << score;
	m_totleStake->setString(str.str());

}

//�˷���Ӧ�� ת�̿�ʼת����ʱ�����
void StakeManager::SaveLastGame()
{
	m_lastGameAll = _ZERO;

	for (int i = _ZERO; i < Stake_Btn_Max; i++)
	{
		m_lastGameAll += m_stakeArray[i].GetStake();
		m_lastRecode[i] = m_stakeArray[i].GetStake();
	}

	//�ܷ� ����0  ��Ϊ��Чѹ��
	if (_ZERO < m_lastGameAll)
	{

		m_lastValidAll = m_lastGameAll;
		memcpy(m_lastValidRecode, m_lastRecode, sizeof(m_lastRecode));		
		/*	for (int i = _ZERO; i < Jetton_Btn_Max; i++)
		m_lastValidRecode[i] = m_jetArray[i].GetJet();*/
	}

	//�ѱ��ֵ�Ѻ�������¼����
	ownStakeInfo info;
	memcpy(info.ownStake, m_lastRecode, sizeof(m_lastRecode));
	//memcpy(info.radeIndex, m_rate, sizeof(m_rate));
	info.ownTotal = m_lastGameAll;
	GameInfo::GetInstance()->setownInfo(info);
}

void StakeManager::SetRate(int* a)
{
	for (int i = _ZERO; i < Stake_Btn_Max; i++)
	{
		m_stakeArray[i].SetBeiLv(a[i]);
		//m_rate[i] = a[i];
	}
}
void StakeManager::ClearJetton()
{
	for (int i = _ZERO; i < Stake_Btn_Max; i++)
	{
		m_stakeArray[i].SetMyStake(_ZERO);
		m_stakeArray[i].resetBtn();
	}
	OneOfStake::MyAllTotel = _ZERO;
}

void StakeManager::SetAI(bool b)
{
	if (b)
	{
		m_cancelAuto->setVisible(true);
		m_bIsAI = true;
	}
	else
	{
		m_cancelAuto->setVisible(false);
		m_bIsAI = false;
	}
}
bool StakeManager::GetAI()
{
	return m_bIsAI;
}

int  StakeManager::GetLastStakeAll()
{
	return m_lastGameAll;
}
bool StakeManager::ContinuStake()
{
	//�ж��Ͼ��Ƿ���ѹ��
	if (m_lastValidAll <= _ZERO)
	{
		return false;
	}

	//����У���Ƿ��ظ����
	bool b = false;						//�Ƿ������Ѻ Ĭ���ǲ����Ե�
	for (size_t i = _ZERO; i < Stake_Btn_Max; i++)
	{
		if (m_lastValidRecode[i] != m_stakeArray[i].GetStake())
		{
			//������һ�β�һ����
			b = true;			//������ѹ��Ч
			break;
		}
	}
	if (b == false) return false;

	auto  info = Player::getInstance();

	if (OneOfStake::MyAllTotel > _ZERO)
	{
		CancelStake();
	}
	if (info->getLogonData().lUserScore >= m_lastGameAll)
	{

		for (int i = _ZERO; i < Stake_Btn_Max; i++)
		{
			//�����˷���ѹ����Ϣ
			if (m_lastValidRecode[i] == _ZERO)
				continue;
			int tag = m_stakeArray[i].GetTag();
			SharkNetWorkKernel::getInstance()->SendJetInfo(tag, m_lastValidRecode[i]);
			log("----------------m_lastValidRecode[%d] = %d------------------------", i, m_lastValidRecode[i]);
		}
	}
	else
	{
		if (m_bIsAI)
		{
			SetAI(false);
		}
		SharkNotice::openNotice(GETSTRING(1409), [this](Ref*)
		{			
			HALL_NOTIFICATION(RECHARGEOUT,nullptr);
		});
		CCLOG("Lack of gold! Please recharge! \n");
	}

	return true;
}
bool StakeManager::CancelStake()
{
	//����ȡ��ѹ����Ϣ
	SharkNetWorkKernel::getInstance()->send(MDM_GF_GAME_SHARK, SUB_G_GAMECHEARNOTES);
	return true;
}

void StakeManager::SetJetNum(int i)
{
	if (m_betText == nullptr) return;
	std::stringstream str;
	str << i;
	m_betText->setString(str.str());
}

void StakeManager::ContinuJetEvent(Ref *pSender, TouchEventType type)
{

	auto bt = dynamic_cast<Node*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(180, 180, 180));
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		ContinuStake();
		_PLAY_SOUND_SHARK(PRESS);
		bt->setColor(Color3B(255, 255, 255));

		break;
	case TOUCH_EVENT_CANCELED:
		bt->setColor(Color3B(180, 180, 180));

		break;
	default:
		break;
	}

}
void StakeManager::CancelJetEvent(Ref *pSender, TouchEventType type)
{

	auto bt = dynamic_cast<Node*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(180, 180, 180));

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		CancelStake();
		_PLAY_SOUND_SHARK(PRESS);

		bt->setColor(Color3B(255, 255, 255));
		break;
	case TOUCH_EVENT_CANCELED:

		bt->setColor(Color3B(255, 255, 255));

		break;
	default:
		break;
	}

}
void StakeManager::AIEvent(Ref *pSender, TouchEventType type)
{
	//������Զ� �ٻ���ť
	auto bt = dynamic_cast<Node*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(180, 180, 180));
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		SetAI(true);
		_PLAY_SOUND_SHARK(PRESS);
		bt->setColor(Color3B(255, 255, 255));
		break;
	case TOUCH_EVENT_CANCELED:
		bt->runAction(ScaleTo::create(0.1f, 1.0f));
		bt->setColor(Color3B(255, 255, 255));

		break;
	default:
		break;
	}

}

void StakeManager::AICancelEvent(Ref *pSender, TouchEventType type)
{
	//���ذ�ť �ر����Զ� 

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		((Node*)pSender)->setColor(Color3B(180, 180, 180));

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		_PLAY_SOUND_SHARK(PRESS);
		SetAI(false);
		((Node*)pSender)->setColor(Color3B(255, 255, 255));
		break;
	case TOUCH_EVENT_CANCELED:
		((Node*)pSender)->setColor(Color3B(255, 255, 255));

		break;
	default:
		break;
	}

}

void StakeManager::JetAddEvent(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		((Node*)pSender)->setColor(Color3B(180, 180, 180));

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		_PLAY_SOUND_SHARK(PRESS);
		SetJetNum(OneOfStake::SetAddJet());
		((Node*)pSender)->setColor(Color3B(255, 255, 255));
		break;
	case TOUCH_EVENT_CANCELED:
		((Node*)pSender)->setColor(Color3B(255, 255, 255));

		break;
	default:
		break;
	}

}

//����ѹ�ֳɹ�
void StakeManager::successStake(SCORE gold, SCORE score, dword animalIndex)
{
	m_stakeArray[animalIndex].UpdateStake(gold);
	
}
//����ȡ��ѹ�ֳɹ�
void StakeManager::successCancelStake()
{
	auto info = Player::getInstance();
	int oldG = _ZERO;
	//��������ѹ��,�������ѹ�ֽ��
	for (int i = _ZERO; i < Stake_Btn_Max; i++)
	{
		oldG += m_stakeArray[i].GetStake();
		m_stakeArray[i].SetMyStake(_ZERO);
	}

	if (oldG == _ZERO) return;

	OneOfStake::MyAllTotel = _ZERO;
	info->addCoin(oldG,false);
}

//���½�ҶѶ�ʱ����Ѻע�׶θ��£�
void StakeManager::updateGoldheap(float t)
{
	auto gameInfo = GameInfo::GetInstance();
	gameState state = gameInfo->getserverState();
	if (state != STAKE) return;

	SCORE jet[12];
	SCORE standard = _ZERO;
	
	//ѡ�����ֵ���Դ�Ϊ��׼
	for (size_t i = _ZERO; i < Stake_Btn_Max; i++)
	{
		jet[i] = gameInfo->getroomInfor()->alljetinfo.lTotelBet[i];
		if (jet[i] > standard)
			standard = jet[i];
	}
	//û��ѹ��
	if (standard == _ZERO)
	{
		for (size_t i = _ZERO; i < Stake_Btn_Max; i++)
		{
			m_oneofjet[i]->StartPlay(_ZERO, _ZERO);
		}
	}
	else
	{
		for (size_t i = _ZERO; i < Stake_Btn_Max; i++)
		{
			m_oneofjet[i]->StartPlay((float)jet[i] / standard, jet[i]);
		}
	}
}


//��Ҷ�����
void StakeManager::hideGoldheap()
{
	for (size_t i = _ZERO; i < Stake_Btn_Max; i++)
	{
		m_oneofjet[i]->StopPlay();
		//m_oneofjet[i]->StartPlay(_ZERO, _ZERO);
	}
	//this->unschedule(schedule_selector(StakeManager::updateGoldheap));
	////�رռ���ҵ�Ѻ��
	//this->unschedule(schedule_selector(StakeManager::updateMyStake));
}

//������ҶѶ�ʱ��
void StakeManager::startGoldheap()
{
	for (int i = _ZERO; i < Stake_Btn_Max; i++)
	{
		m_oneofjet[i]->StartPlay(_ZERO, _ZERO);
	}
}