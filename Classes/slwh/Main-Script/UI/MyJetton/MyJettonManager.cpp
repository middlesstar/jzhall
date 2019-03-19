#include "MyJettonManager.h"
#include "slwh/Network/NetworkKernel.h"
#include "slwh/Module/SLWHNotice.h"
#include "LanguageConfig.h"
#include "HallGameScene.h"

using namespace ui;

//jetton 节点下逻辑标签
#define _Node				"node"

//Node Tag
#define Jet_Munu			"Jet_Menu"
#define	Jet_Bet				"Jet_BT"
#define	Jet_All_One			"Jet_All"


#define Jet_Add_Bt			"bet"
#define Jet_Now_Add_Num		"Num"

//押注按钮逻辑标签数组 顺序： (红)动物1 ~ 4   (绿)~    (黄)~    Z H X
#define JET_BET_ARRAY		{"Jet_1", "Jet_4", "Jet_7", "Jet_10", "Jet_0", "Jet_3", "Jet_6", "Jet_9", "Jet", "Jet_2", "Jet_5", "Jet_8", "Jet_11", "Jet_12", "Jet_13" }

//Jet_Munu  tag
#define CONTINU_BT			"contiun"
#define	CANCEL_BT			"cancel"
#define	AI_BT				"AI"
#define	CANCEL_AI_BT		"cancelAI"  


//彩金和总压分,发现用tag在整合的时候 改标签很蛋疼，改用名字
#define _TOTALSCORE			"totalscore"
#define _CAIJIN				"caijin"

#define Jet_All				"Jet_All"
//押注按钮逻辑标签数组 顺序： (红)动物1 ~ 4   (绿)~    (黄)~    Z H X
#define Jet_All_Array		{"AnimalImage_1","AnimalImage_4","AnimalImage_7","AnimalImage_10","AnimalImage_0","AnimalImage_3","AnimalImage_6","AnimalImage_9","AnimalImage","AnimalImage_2","AnimalImage_5","AnimalImage_8","AnimalImage_13","AnimalImage_12","AnimalImage_11"}

//压分界面结果记录
#define Jet_Record			{"0","1","2","3","4","5","6","7","8","9","10","11"}

MyJettonManager::MyJettonManager() :
m_bIsAI(false),
m_lastGameAll(_ZERO),
m_lastValidAll(_ZERO),
m_betAdd(nullptr),
m_betText(nullptr),
m_moveNode(nullptr),
m_ai(nullptr),
m_continu(nullptr),
m_cancel(nullptr),
m_cancelAI(nullptr),
m_bIsenable(true),
m_layerColor(nullptr),
m_totleJet(nullptr),
m_caijin(nullptr)
{
	memset(m_lastValidRecode, NULL, sizeof(m_lastValidRecode));
	memset(m_oneofjet, NULL, sizeof(m_oneofjet));
	memset(m_jetrecord, NULL, sizeof(m_jetrecord));
}

MyJettonManager::~MyJettonManager()
{
	m_layerColor->removeFromParent();
	m_layerColor = nullptr;

}

void MyJettonManager::Start()
{

	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal0)] = Path_SMALL_Red_Animal0;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal1)] = Path_SMALL_Red_Animal1;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal2)] = Path_SMALL_Red_Animal2;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Red, enumAnimalType::Animal3)] = Path_SMALL_Red_Animal3;

	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal0)] = Path_SMALL_Green_Animal0;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal1)] = Path_SMALL_Green_Animal1;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal2)] = Path_SMALL_Green_Animal2;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Green, enumAnimalType::Animal3)] = Path_SMALL_Green_Animal3;

	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal0)] = Path_SMALL_Yellow_Animal0;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal1)] = Path_SMALL_Yellow_Animal1;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal2)] = Path_SMALL_Yellow_Animal2;
	m_map[_RES_KEY(enumSpecialRs::Normal, enumAnimalColor::Yellow, enumAnimalType::Animal3)] = Path_SMALL_Yellow_Animal3;

	m_map[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal0)] = Path_SMALL_ALL_Animal0;
	m_map[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal1)] = Path_SMALL_ALL_Animal1;
	m_map[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal2)] = Path_SMALL_ALL_Animal2;
	m_map[_RES_KEY(enumSpecialRs::D3Y, enumAnimalColor::ColorMax, enumAnimalType::Animal3)] = Path_SMALL_ALL_Animal3;

	m_map[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Red, enumAnimalType::AnimalMax)] = Path_SMALL_Red_AnimalAll;
	m_map[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Green, enumAnimalType::AnimalMax)] = Path_SMALL_Green_AnimalAll;
	m_map[_RES_KEY(enumSpecialRs::D4X, enumAnimalColor::Yellow, enumAnimalType::AnimalMax)] = Path_SMALL_Yellow_AnimalAll;

	m_map[_RES_KEY(enumSpecialRs::DMG, enumAnimalColor::ColorMax, enumAnimalType::AnimalMax)] = Path_SMALL_All_AnimalAll;

	
	auto node = m_node->getChildByName(_Node);
	CC_ASSERT(nullptr != node);

	//得到菜单根节点
	auto menuroot = node->getChildByName(Jet_Munu);
	CC_ASSERT(menuroot != nullptr);
	//菜单下的节点
	m_betAdd = dynamic_cast<Button*>(menuroot->getChildByName(Jet_Add_Bt));
	CC_ASSERT(nullptr != m_betAdd);
	m_betAdd->addTouchEventListener(this, toucheventselector(MyJettonManager::JetAddEvent));
	//压分切换的数字在压分切换按钮下
	m_betText = dynamic_cast<TextAtlas*>(m_betAdd->getChildByName(Jet_Now_Add_Num));
	CC_ASSERT(nullptr != m_betText);

	m_ai = dynamic_cast<Button*>(menuroot->getChildByName(AI_BT));
	CC_ASSERT(nullptr != m_ai);
	m_ai->addTouchEventListener(this, toucheventselector(MyJettonManager::AIEvent));

	m_continu = dynamic_cast<Button*>(menuroot->getChildByName(CONTINU_BT));
	CC_ASSERT(nullptr != m_continu);
	m_continu->addTouchEventListener(this, toucheventselector(MyJettonManager::ContinuJetEvent));

	m_cancel = dynamic_cast<Button*>(menuroot->getChildByName(CANCEL_BT));
	CC_ASSERT(nullptr != m_cancel);
	m_cancel->addTouchEventListener(this, toucheventselector(MyJettonManager::CancelJetEvent));

	m_cancelAI = dynamic_cast<Button*>(menuroot->getChildByName(CANCEL_AI_BT));
	CC_ASSERT(nullptr != m_cancelAI);
	m_cancelAI->addTouchEventListener(this, toucheventselector(MyJettonManager::AICancelEvent));


	m_totleJet = dynamic_cast<TextAtlas*>(node->getChildByName(_TOTALSCORE));
	CC_ASSERT(nullptr != m_totleJet);

	m_caijin = dynamic_cast<TextAtlas*>(node->getChildByName(_CAIJIN));
	CC_ASSERT(nullptr != m_caijin);

	//颜色层
	m_layerColor = CCLayerColor::create(Color4B(_ZERO, _ZERO, _ZERO, 100),800,320);
	m_layerColor->setPosition(Vec2(_ZERO,_ZERO));
	m_layerColor->setTouchEnabled(true);
	m_layerColor->setVisible(false);
	m_node->addChild(m_layerColor);

	
	Node* n = node->getChildByName(Jet_Bet);
	std::string arr[] = JET_BET_ARRAY;
	for (size_t i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		Button* x = dynamic_cast<Button*>(n->getChildByName(arr[i]));
		m_jetArray[i].AttachNode(x);
		m_jetArray[i].SetTag(i);
		x->addTouchEventListener(&m_jetArray[i] , toucheventselector(OneOfJetton::BtEvent));
	}

	//押注显示标签
	auto jetallroot = node->getChildByName(Jet_All_One);
	std::string strnode[] =  Jet_All_Array;
	for (size_t i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		m_oneofjet[i] = SLWH_GoldHeap::create(30);
		m_oneofjet[i]->setPosition(Vec2(28,50));
		jetallroot->getChildByName(strnode[i])->addChild(m_oneofjet[i]);
		//m_oneofjet[i]->StartPlay((int)i, i * 100);
	}


	//关联节点 结果记录面板
	m_record = node->getChildByName("Res_Record");
	CC_ASSERT(m_record != nullptr);
	std::string a[] = Jet_Record;
	for (size_t i = _ZERO; i < RECORDMAX; i++)
	{
		Node* n = m_record->getChildByName(a[i]);
		m_jetrecord[i].animal = dynamic_cast<ImageView*>(n);
		CC_ASSERT(nullptr != m_jetrecord[i].animal);
		m_jetrecord[i].zhx = dynamic_cast<ImageView*>(n->getChildByName("ZHX_Image"));
		CC_ASSERT(nullptr != m_jetrecord[i].zhx);
	}
	m_record->setOpacity(_ZERO);

	//指向队尾
	m_listRear = RECORDMAX - 1;

	//完全透明
	//m_node->setOpacity(_ZERO);
	//开始更新金币堆
	schedule(schedule_selector(MyJettonManager::updategoldheap), 0.5f);
	//监控界面数据变化
	scheduleUpdate();
	//初始化总分
	setMyTotelJet(_ZERO);
	//
	m_cancelAI->setVisible(false);

}

void MyJettonManager::update(float t)
{
	if (OneGameInfo::GetInstance()->getServerState() == _Jet)
	{
		setMyTotelJet(OneOfJetton::MyAllTotel);
	}
}

//初始化数据中 个位为ZHX  算法为ID = 类型*1000 + 颜色*100 + 动物 *10 + ZHX
void MyJettonManager::InitResList(int* a)
{
	for (size_t i = _ZERO; i < RECORDMAX; i++)
	{

		//记录进行转换
		AnimalResult e(a[i]);

		switch (e.e1)
		{
		case D3Y:
			e.e2 = enumAnimalColor::ColorMax;
			break;
		case D4X:
			e.e3 = enumAnimalType::AnimalMax;
			break;
		case DMG:
			e.e2 = enumAnimalColor::ColorMax;
			e.e3 = enumAnimalType::AnimalMax;
			break;
		case _4LF:
		case CJ:
		case XYCJ:
		default:
			e.e1 = Normal;
			break;
		}

		m_jetrecord[RECORDMAX - 1 - i].animal->loadTexture(m_map[_RES_KEY(e.e1, e.e2, e.e3)], TextureResType::PLIST);
		SetZHXTex(RECORDMAX - 1 - i, e.e4);
	}
}

//添加结果
void MyJettonManager::Add(enumSpecialRs e1, enumAnimalColor e2, enumAnimalType e3, enumGameZXH e4)
{
	//_PLAY_MUSCI_EFF(RES_ADD);

	Vec2 pos = m_jetrecord[RECORDMAX - 1].animal->getPosition();
	m_jetrecord[m_listRear].animal->setOpacity((GLubyte)(TRANSPARENT_MAX * 0.8f));
	
	for (size_t i = _ZERO; i < RECORDMAX; i++)
	{
		if (i != RECORDMAX - 1)
		{
			m_jetrecord[RECORDMAX - 1 - i].animal->setPosition(m_jetrecord[RECORDMAX - 2 - i].animal->getPosition());
		}
		else
		{
			m_jetrecord[_ZERO].animal->setPosition(pos);
		}
	}


	if (++m_listRear >= RECORDMAX)
		m_listRear = _ZERO;

	switch (e1)
	{
	case D3Y:
		e2 = enumAnimalColor::ColorMax;
		break;
	case D4X:
		e3 = enumAnimalType::AnimalMax;
		break;
	case DMG:
		e2 = enumAnimalColor::ColorMax;
		e3 = enumAnimalType::AnimalMax;
		break;
	case _4LF:
	case CJ:
	case XYCJ:
	default:
		e1 = Normal;
		break;
	}

	m_jetrecord[m_listRear].animal->loadTexture(m_map[_RES_KEY(e1, e2, e3)], TextureResType::PLIST);
	SetZHXTex(m_listRear, e4);
	m_jetrecord[m_listRear].animal->setOpacity(TRANSPARENT_MAX);
}

void MyJettonManager::SetZHXTex(int index, enumGameZXH type)
{
	if (index < _ZERO || index >= RECORDMAX)
	{
		return;
	}

	switch (type)
	{
	case enumGameZXH::Zhuang:
		//庄
		m_jetrecord[index].zhx->loadTexture(Path_PNG_ZHUANG, TextureResType::PLIST);
		break;
	case enumGameZXH::He:
		//和
		m_jetrecord[index].zhx->loadTexture(Path_PNG_HE, TextureResType::PLIST);
		break;
	case enumGameZXH::Xian:
		//闲
		m_jetrecord[index].zhx->loadTexture(Path_PNG_XIAN, TextureResType::PLIST);
		break;
	default:
		CCLOG("SLWH: error ! InitResList\n");
		break;
	}
}

void MyJettonManager::Add(AnimalResult e)
{
	Add(e.e1, e.e2, e.e3, e.e4);
}

void MyJettonManager::OpenRecord()
{
	m_record->runAction(FadeIn::create(0.5f));
}

int MyJettonManager::GetLastJetAll()
{
	return m_lastGameAll;
}

//此方法应在 转盘开始转动的时候调用
void MyJettonManager::SaveLastGame()
{
	m_lastGameAll = _ZERO;

	for (int i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		m_lastGameAll += m_jetArray[i].GetJet();
		m_lastRecode[i] = m_jetArray[i].GetJet();
	}

	//总分 大于0  则为有效压分
	if (_ZERO < m_lastGameAll)
	{
		m_lastValidAll = m_lastGameAll;
		memcpy(m_lastValidRecode, m_lastRecode, sizeof(m_lastRecode));
	/*	for (int i = _ZERO; i < Jetton_Btn_Max; i++)
			m_lastValidRecode[i] = m_jetArray[i].GetJet();*/
	}
}

void MyJettonManager::SetRate(int* a)
{
	for (int i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		m_jetArray[i].SetBeiLv(a[i]);
	}
}

void MyJettonManager::ClearJetton()
{
	for (int i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		m_jetArray[i].SetMyJetton(_ZERO);

	}
	OneOfJetton::MyAllTotel = _ZERO;
}

bool MyJettonManager::ContinuJet()
{

	//判断上局是否有压分
	if (m_lastValidAll <= _ZERO)
	{
		return false;
	}
	
	//进行校验是否重复点击
	bool b = false;						//是否可以续押 默认是不可以的
	for (size_t i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		if (m_lastValidRecode[i] != m_jetArray[i].GetJet())
		{
			//出现了一次不一样的
			b = true;			//可以续压生效
			break;
		}
	}
	if (b == false) return false;

	//进行续压逻辑
	auto  info = Player::getInstance();
	//如果有压分 先取消压分
	if (OneOfJetton::MyAllTotel > _ZERO)
	{
		CancelJet();
	}

	if (info->getLogonData().lUserScore >= m_lastValidAll)
	{
		for (int i = _ZERO; i < Jetton_Btn_Max; i++)
		{
			//向服务端发送压分信息
			if (m_lastValidRecode[i] == _ZERO)
				continue;

			NetworkKernel::getInstance()->SendJetInfo(i, m_lastValidRecode[i]);
		}
	}
	else
	{
		SLWHNotice::openNotice(GETSTRING(1409), [this](Ref*)
		{
			auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
			rechargeNode->setPosition(Vec2(0, 0));
			Director::getInstance()->getRunningScene()->addChild(rechargeNode, 100);

			//HALL_NOTIFICATION("recharge_out", nullptr);
		});
	}

	return true;

}

bool MyJettonManager::CancelJet()
{
	if (OneOfJetton::MyAllTotel > _ZERO)
	{
		//发送取消压分消息
		NetworkKernel::getInstance()->send(MDM_GF_GAME_SLWH, SUB_G_GAMECHEARNOTES);
	}
	return true;
}

//更新金币堆
void MyJettonManager::updategoldheap(float t)
{
	auto gameinfo = OneGameInfo::GetInstance();
	enumServerState state = gameinfo->getServerState();
	if (state != _Jet) return;
	
	SCORE *jet = gameinfo->getroomInfo()->alljetinfo.lTotelBet;
	SCORE standard = _ZERO;
	//选出最大值，以此为标准
	for (size_t i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		if (jet[i] > standard)
			standard = jet[i];
	}
	//没人压分
	if (standard == _ZERO)
	{
		for (size_t i = _ZERO; i < Jetton_Btn_Max; i++)
		{
			m_oneofjet[i]->StartPlay(_ZERO, _ZERO);
		}
	}
	else
	{
		for (size_t i = _ZERO; i < Jetton_Btn_Max; i++)
		{
			m_oneofjet[i]->StartPlay((float)jet[i] / standard, jet[i]);
		}
	}
}

//设置彩金
void MyJettonManager::setCaijin(SCORE score)
{
	std::stringstream str;
	str << score;
	m_caijin->setString(str.str());
}

//设置我的总压分
void MyJettonManager::setMyTotelJet(SCORE score)
{
	std::stringstream str;
	str << score;
	m_totleJet->setString(str.str());
}

void MyJettonManager::ContinuJetEvent(Ref *pSender, TouchEventType type)
{
	
	auto bt = dynamic_cast<Node*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(_GRAY, _GRAY, _GRAY));
		bt->runAction(ScaleTo::create(0.1f, 1.2f));
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		ContinuJet();
		_PLAY_MUSCI_EFF(_CLICK);
		bt->runAction(ScaleTo::create(0.1f, 1.0f));
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		
		break;
	case TOUCH_EVENT_CANCELED:
		bt->runAction(ScaleTo::create(0.1f, 1.0f));
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));

		break;
	default:
		break;
	}
	//m_music.Play(enumMusic.Click);

}

void MyJettonManager::CancelJetEvent(Ref *pSender, TouchEventType type)
{
	
	auto bt = dynamic_cast<Node*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(_GRAY, _GRAY, _GRAY));
		bt->runAction(ScaleTo::create(0.1f, 1.2f));
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		CancelJet();
		_PLAY_MUSCI_EFF(_CLICK);
		bt->runAction(ScaleTo::create(0.1f, 1.0f));
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		break;
	case TOUCH_EVENT_CANCELED:
		bt->runAction(ScaleTo::create(0.1f, 1.0f));
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));

		break;
	default:
		break;
	}

}

void MyJettonManager::AIEvent(Ref *pSender, TouchEventType type)
{
	//点击了自动 召唤按钮
	//m_music.Play(enumMusic.Click);
	auto bt = dynamic_cast<Node*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(_GRAY, _GRAY, _GRAY));
		bt->runAction(ScaleTo::create(0.1f,1.2f));
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		SetAI(true);
		bt->runAction(ScaleTo::create(0.1f, 1.0f));
		_PLAY_MUSCI_EFF(_CLICK);
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		break;
	case TOUCH_EVENT_CANCELED:
		bt->runAction(ScaleTo::create(0.1f, 1.0f));
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));

		break;
	default:
		break;
	}

}

void MyJettonManager::SetAI(bool b)
{
	if (b)
	{
		m_cancelAI->setVisible(true);
		m_bIsAI = true;
	}
	else
	{
		m_cancelAI->setVisible(false);
		m_bIsAI = false;
	}
}

void MyJettonManager::AICancelEvent(Ref *pSender, TouchEventType type)
{
	//m_music.Play(enumMusic.Click);
	//隐藏按钮 关闭了自动 

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		((Node*)pSender)->setColor(Color3B(_GRAY, _GRAY, _GRAY));

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		_PLAY_MUSCI_EFF(_CLICK);
		SetAI(false);
		((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		break;
	case TOUCH_EVENT_CANCELED:
		((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));

		break;
	default:
		break;
	}

}

int* MyJettonManager::GetRecord()
{
	return m_lastRecode;
}

bool MyJettonManager::GetAI()
{
	return m_bIsAI;
}

void MyJettonManager::enable(bool b)
{
	if (m_bIsenable == b)
	{
		return;
	}

	m_bIsenable = b;
	if (m_bIsenable)
	{
		
		m_layerColor->setVisible(false);
		EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
		eventDispatcher->removeEventListenersForTarget(m_layerColor);
	}
	else
	{
		m_layerColor->setVisible(true);
		EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(MyJettonManager::onTouchBegan, this);
		listener->setSwallowTouches(true);
		eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_layerColor);
	}

}

bool MyJettonManager::onTouchBegan(Touch* touch, Event* event)
{
	if (m_bIsenable)
	{
		return false;
	}

	Vec2 pBeginPos = touch->getLocation();
	Size contentSize = m_layerColor->getContentSize();
	Vec2 pos = m_layerColor->getParent()->convertToWorldSpace(m_layerColor->getPosition());

	Rect rect = Rect(pos.x, pos.y, pos.x + contentSize.width, pos.y +  contentSize.height);
	return rect.containsPoint(pBeginPos);
}

void MyJettonManager::SetJetNum(int i)
{
	if (m_betText == nullptr) return;
	std::stringstream str;
	str << i;
	m_betText->setString(str.str());
}

void MyJettonManager::JetAddEvent(Ref *pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		((Node*)pSender)->setColor(Color3B(_GRAY, _GRAY, _GRAY));

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		_PLAY_MUSCI_EFF(_CLICK);
		SetJetNum(OneOfJetton::SetAddJet());
		((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		break;
	case TOUCH_EVENT_CANCELED:
		((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));

		break;
	default:
		break;
	}

}

void MyJettonManager::StartPlay()
{ 
	m_node->runAction(FadeIn::create(0.5f)); 
	for (int i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		m_oneofjet[i]->StartPlay(_ZERO,_ZERO);
	}

};

void MyJettonManager::StopPlay()
{ 
	m_node->runAction(FadeOut::create(0.5f));
	for (int i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		m_oneofjet[i]->StopPlay();
	}
};

void MyJettonManager::successJet(SCORE gold, SCORE score, dword animalID)
{
	m_jetArray[animalID].UpdateJet(gold);
	//收到了数据包
	m_jetArray[animalID].BIsCanJet = true;
}

void MyJettonManager::successCancelJet()
{
	auto info = Player::getInstance();
	//如果玩家已压分,返回玩家压分金币
	for (int i = _ZERO; i < Jetton_Btn_Max; i++)
	{
		m_jetArray[i].SetMyJetton(_ZERO);
	}
	//金币加上
	info->addCoin(OneOfJetton::MyAllTotel,false);
	OneOfJetton::MyAllTotel = _ZERO;
	
}

 