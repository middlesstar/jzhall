#include "WindowShop.h"
#include "LogicShop.h"
#include "Player/Player.h"

//服务器的商品id
const int nProductIds[] = { 1001, 1002, 1003, 1004, 1005, 1006};

WindowShop::WindowShop()
	: m_pBg(nullptr)
	, m_pInfoSpr(nullptr)
	, m_pTopLevelSpr(nullptr)
	, m_pNodeRecharge(nullptr)
	, m_pNodeVip(nullptr)
	, m_pNodeFirst(nullptr)
	, m_pVipScroll(nullptr)
	, m_pVipItem(nullptr)
	, m_pBtnClose(nullptr)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_VIP, WindowShop::refreshVipInfo, nullptr);
}

WindowShop::~WindowShop()
{
	CC_SAFE_RELEASE_NULL(m_pVipItem);
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_VIP);
}

void WindowShop::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZShop.csb");
	CCASSERT(pMainNode, "WindowShop::setContent() layout/JZShop.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();
	
	//设置按钮事件
	m_pBtnClose = dynamic_cast<Button*>(_seekNodeByName(pMainNode, "btn_close"));
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	m_pBg = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "notice_bg"));
	m_pInfoSpr = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode,"recharge_level"));
	m_pTopLevelSpr = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "recharge_level_top"));
	m_pNodeRecharge = _seekNodeByName(pMainNode, "node_normal");
	m_pNodeVip = _seekNodeByName(pMainNode, "node_vip");
	m_pNodeFirst = _seekNodeByName(pMainNode, "node_first");

	m_pVipScroll = dynamic_cast<ui::ScrollView*>(_seekNodeByName(m_pNodeVip, "scroll"));
	m_pVipItem = dynamic_cast<ImageView*>(_seekNodeByName(m_pVipScroll, "img_item"));
	m_pVipItem->retain();
	m_pVipItem->removeFromParent();

	_addButtonClickEvent(m_pInfoSpr, "btn_first", [this](Ref* sender){this->_setUIState(EWSS_FIRST);});
	_addButtonClickEvent(m_pNodeRecharge, "btn_explain", [this](Ref* sender){this->_setUIState(EWSS_VIP);});
	_addButtonClickEvent(m_pNodeVip, "btn_recharge", [this](Ref* sender){this->_setUIState(EWSS_NORMAL);});
	_addButtonClickEvent(m_pNodeFirst, "btn_buy", [this](Ref* sender){this->_setUIState(EWSS_NORMAL);});
	_addButtonClickEvent(m_pNodeFirst, "btn_shop", [this](Ref* sender){this->_setUIState(EWSS_NORMAL); });

	_initVipInfo();
	_initShopItems();
	_initVipItems();
	_playButtonAction();

	if (Player::getInstance()->getHallData().sumRecharge > 0)
	{
		_setUIState(EWSS_NORMAL);
	}
	else
	{
		_setUIState(EWSS_FIRST);
	}
}

void WindowShop::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowShop::updateWindow(int nCode)
{
	if (nCode == 0)
	{
		_setUIState(EWSS_VIP);
	}
	else
	{
		_initShopItems();
	}
	
}

void WindowShop::_setUIState(int nState)
{
	switch (nState)
	{
	case EWSS_NORMAL:
		m_pBg->setVisible(true);
		m_pInfoSpr->setVisible(Player::getInstance()->getHallData().dwVipLevel < 10);
		m_pTopLevelSpr->setVisible(!m_pInfoSpr->isVisible());
		m_pNodeRecharge->setVisible(true);
		m_pNodeVip->setVisible(false);
		m_pNodeFirst->setVisible(false);
		m_pBtnClose->setVisible(true);
		break;
	case EWSS_VIP:
		m_pBg->setVisible(true);
		m_pInfoSpr->setVisible(Player::getInstance()->getHallData().dwVipLevel < 10);
		m_pTopLevelSpr->setVisible(!m_pInfoSpr->isVisible());
		m_pNodeRecharge->setVisible(false);
		m_pNodeVip->setVisible(true);
		m_pNodeFirst->setVisible(false);
		m_pBtnClose->setVisible(true);
		break;
	case EWSS_FIRST:
		m_pBg->setVisible(false);
		m_pInfoSpr->setVisible(false);
		m_pTopLevelSpr->setVisible(false);
		m_pNodeRecharge->setVisible(false);
		m_pNodeVip->setVisible(false);
		m_pNodeFirst->setVisible(true);
		m_pBtnClose->setVisible(false);
		break;
	default:
		break;
	}
}

void WindowShop::_initVipInfo()
{
	Player* pPlayer = Player::getInstance();

	if (pPlayer->getHallData().dwVipLevel < 10)
	{
		m_pInfoSpr->setVisible(true);
		m_pTopLevelSpr->setVisible(false);

		TextAtlas* pTextCurVip = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pInfoSpr, "label_curvip"));
		TextAtlas* pTextNextVip = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pInfoSpr, "label_nextvip"));
		TextAtlas* pTextNeedRecharge = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pInfoSpr, "label_needrecharge"));
		LoadingBar* pLb = dynamic_cast<LoadingBar*>(_seekNodeByName(m_pInfoSpr, "vipexp_pb"));

		pTextCurVip->setString(StringUtils::format("%d", pPlayer->getHallData().dwVipLevel));
		pTextNextVip->setString(StringUtils::format("%d", pPlayer->getHallData().dwVipLevel + 1));
		pTextNeedRecharge->setString(StringUtils::format("%d", pPlayer->getNextVipNeedRecharge()));
		pLb->setPercent(pPlayer->getCurVipRechargePercent());
	}
	else
	{
		m_pInfoSpr->setVisible(false);
		m_pTopLevelSpr->setVisible(true);
	}
}

void WindowShop::_initShopItems()
{
	ImageView* pImgItem = nullptr;
	ImageView* pImgPresent = nullptr;
	ImageView* pImgPercent = nullptr;

	CMD_GP_LogonSuccess_Hall hallData = Player::getInstance()->getHallData();
	for (int i = 0; i < 6; i++)
	{
		pImgItem = dynamic_cast<ImageView*>(_seekNodeByName(m_pNodeRecharge, StringUtils::format("img_item%d", i + 1)));
		pImgItem->addClickEventListener(CC_CALLBACK_1(WindowShop::_ItemClickCallback, this));

		pImgItem->setTag(nProductIds[i]);
		pImgPresent = dynamic_cast<ImageView*>(pImgItem->getChildByName("img_present"));
		pImgPercent = dynamic_cast<ImageView*>(pImgItem->getChildByName("img_percent"));

		pImgPresent->loadTexture(StringUtils::format("images/hall/recharge/recharge_first%d.png", hallData.ritems[i] == 0 ? (i + 1) : (i + 101)), TextureResType::PLIST);
		pImgPercent->loadTexture(StringUtils::format("images/hall/recharge/recharge_give%d.png", hallData.ritems[i] == 0 ? 100 : (i + 1)), TextureResType::PLIST);
	}
}

void WindowShop::_initVipItems()
{
	m_pVipScroll->removeAllChildren();

	const int nMaxVipLevel = 10;
	int nRechargeCount[nMaxVipLevel] = { 30, 100, 300, 800, 2000, 5000, 10000, 20000, 50000, 100000 };					//累计充值
	int nPresentCount[nMaxVipLevel] = { 10000, 20000, 30000, 60000, 100000, 200000, 300000, 500000, 800000, 1000000 };	//每日赠送

	ImageView* pItem = nullptr;
	TextAtlas* pText = nullptr;
	ImageView* pImgIntro = nullptr;
	for (int i = 0; i < nMaxVipLevel; i++)
	{
		pItem = dynamic_cast<ImageView*>(m_pVipItem->clone());

 		//设置VIP等级
 		pText = dynamic_cast<TextAtlas*>(pItem->getChildByName("text_vip"));
 		pText->setString(StringUtils::format("%d",i + 1));
 
 		//累计充值
 		pText = dynamic_cast<TextAtlas*>(pItem->getChildByName("text_rechargenum"));
 		pText->setString(StringUtils::format("%d", nRechargeCount[i]));
 
 		//每日赠送
 		pText = dynamic_cast<TextAtlas*>(pItem->getChildByName("text_sendnum"));
 		pText->setString(StringUtils::format("%d", nPresentCount[i]));
 
 		//特权
 		pImgIntro = dynamic_cast<ImageView*>(pItem->getChildByName("intro"));
		pImgIntro->loadTexture(StringUtils::format("images/hall/recharge/recharge_words%d.png", i + 1), TextureResType::PLIST);

		pItem->setPositionX(m_pVipItem->getPositionX() + m_pVipItem->getContentSize().width * 1.08f * i);

		m_pVipScroll->addChild(pItem);
	}

	if (pItem)
	{
		Size size = m_pVipScroll->getContentSize();
		m_pVipScroll->setInnerContainerSize(Size(pItem->getPositionX() + (1 - pItem->getAnchorPoint().x) * pItem->getContentSize().width , size.height));
	}
}

void WindowShop::_playButtonAction()
{
	Button* pExplain = dynamic_cast<Button*>(_seekNodeByName(m_pNodeRecharge, "btn_explain"));
	Button* pVip = dynamic_cast<Button*>(_seekNodeByName(m_pNodeVip, "btn_recharge"));

	if (pExplain && pVip)
	{
		pExplain->stopAllActions();
		pVip->stopAllActions();

		auto rotateAction = Sequence::create(RotateTo::create(0.06f, 20.0f), RotateTo::create(0.12f, -20.0f),
			RotateTo::create(0.09f, 10.0f), RotateTo::create(0.06f, -10.0f), RotateTo::create(0.03, 0), NULL);
		auto scaleAction = Sequence::create(ScaleTo::create(0.6f, 1.2f), ScaleTo::create(0.6f, 1.0f), NULL);

		Action* pAction = RepeatForever::create(Sequence::create(DelayTime::create(2.0f), EaseSineInOut::create(rotateAction), scaleAction, NULL));
		pExplain->runAction(pAction->clone());
		pVip->runAction(pAction->clone());
	}
}

void WindowShop::_ItemClickCallback(Ref* pRef)
{
	ImageView* pItem = dynamic_cast<ImageView*>(pRef);
	LogicShop* pLogic = LogicShop::getInstance();

	pLogic->requestOrderId(pItem->getTag());
}

void WindowShop::refreshVipInfo(Ref* ref)
{
	_initVipInfo();
}