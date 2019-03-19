#include "WindowExchange.h"
#include "LogicExchange.h"
#include "LanguageConfig.h"
#include "Player/Player.h"
#include "Util/GameUtil.h"
#include "Window/UIHallHelper.h"

struct sGoodsInfo
{
	int nNameStringId;		//文字中的id
	int nPrice;				//价格，单位：元宝
	int nType;				//1-金币类型货物，直接兑换，2-话费类型货物，填写电话和qq，3-实物类型，还需填写姓名和地址
};

//兑换的12个物品配置
sGoodsInfo sExchangeConfig[] = 
{
	{ 1200, 1000,  1 },
	{ 1201, 3000,  1 },
	{ 1202, 5000,  2 },
	{ 1203, 10000, 2 },
	{ 1204, 20000, 2 },
	{ 1205, 50000, 3 },
	{ 1206, 80000, 3 },
	{ 1207, 100000, 3 },
	{ 1208, 160000, 3 },
	{ 1209, 220000, 3 },
	{ 1210, 600000, 3 },
	{ 1211, 800000, 3 }
};

WindowExchange::WindowExchange()
	: m_pInfoSpr(nullptr)
	, m_pNodeExchange(nullptr)
	, m_pNodeRecord(nullptr)
	, m_pNodeGoods(nullptr)
	, m_pNodeTel(nullptr)
	, m_pBtnRecord(nullptr)
	, m_pBtnExchange(nullptr)
	, m_pRecordScroll(nullptr)
	, m_pRecordItem(nullptr)
	, m_nSelectIndex(0)
	, m_bRefreshRecord(true)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Gold, WindowExchange::refreshGold, nullptr);
}

WindowExchange::~WindowExchange()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Gold);

	CC_SAFE_RELEASE_NULL(m_pRecordItem);
}

void WindowExchange::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZExchange.csb");
	CCASSERT(pMainNode, "WindowExchange::setContent() layout/JZExchange.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_pInfoSpr = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode,"sprite_info"));
	m_pNodeExchange = _seekNodeByName(pMainNode, "node_exchange");
	m_pNodeRecord = _seekNodeByName(pMainNode, "node_record");
	m_pNodeGoods = _seekNodeByName(pMainNode, "node_apply_goods");
	m_pNodeTel = _seekNodeByName(pMainNode, "node_apply_tel");

	m_pBtnRecord = dynamic_cast<Button*>(_seekNodeByName(m_pInfoSpr, "btn_record"));
	m_pBtnExchange = dynamic_cast<Button*>(_seekNodeByName(m_pInfoSpr, "btn_exchange"));

	m_pRecordScroll = dynamic_cast<ui::ScrollView*>(_seekNodeByName(m_pNodeRecord, "scroll_record"));
	m_pRecordItem = dynamic_cast<ImageView*>(_seekNodeByName(m_pRecordScroll, "item"));
	m_pRecordItem->removeFromParent();
	m_pRecordItem->retain();

	Text* pTextName = dynamic_cast<Text*>(_seekNodeByName(pMainNode,"label_username"));
	Text* pTextGold = dynamic_cast<Text*>(_seekNodeByName(pMainNode, "label_goldnum"));
	pTextName->setString(Player::getInstance()->getLogonData().szNickName);
	pTextGold->setString(StringUtils::format("%d", Player::getInstance()->getHallData().llYuanBao));

	_initPhoneAndQQInput();
	_setUIState(EWES_NORMAL);
	_setScrollContent();

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender){this->close();});
	_addButtonClickEvent(m_pNodeTel, "btn_tel_apply", CC_CALLBACK_1(WindowExchange::_ItemExchange_Callback,this));
	_addButtonClickEvent(m_pNodeGoods, "btn_goods_apply", CC_CALLBACK_1(WindowExchange::_GoodsExchange_Callback, this));

	m_pBtnRecord->addClickEventListener([this](Ref* sender)
	{
		this->_setUIState(EWES_RECORD);

		if (m_bRefreshRecord)
		{
			LogicExchange::getInstance()->requestRecords();
		}

		playButtonClickSound();
	});

	m_pBtnExchange->addClickEventListener([this](Ref* sender)
	{
		this->_setUIState(EWES_NORMAL);
		playButtonClickSound();
	});
	
}

void WindowExchange::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowExchange::updateWindow(int nCode, void* pData)
{
	if (nCode == EWES_RECORD)
	{
		_setUIState(EWES_RECORD);
		_setExchangeRecord((CMD_ST_DxRewardList_rsp*)pData);
		m_bRefreshRecord = false;
	}
}

void WindowExchange::onOkClick(int nData, void* pData)
{
	CC_ASSERT(nData >= 0 && nData < sizeof(sExchangeConfig) / sizeof(sExchangeConfig[0]));

	LogicExchange::getInstance()->setCostGold(sExchangeConfig[nData].nPrice);
	LogicExchange::getInstance()->requestExchange(nData);
	m_bRefreshRecord = true;
}

void WindowExchange::_setUIState(int nState)
{
	switch (nState)
	{
	case EWES_NORMAL:
		m_pInfoSpr->setVisible(true);
		m_pBtnRecord->setVisible(true);
		m_pBtnExchange->setVisible(false);
		m_pNodeExchange->setVisible(true);
		m_pNodeRecord->setVisible(false);
		m_pNodeGoods->setVisible(false);
		m_pNodeTel->setVisible(false);
		break;
	case EWES_RECORD:
		m_pInfoSpr->setVisible(true);
		m_pBtnRecord->setVisible(false);
		m_pBtnExchange->setVisible(true);
		m_pNodeExchange->setVisible(false);
		m_pNodeRecord->setVisible(true);
		m_pNodeGoods->setVisible(false);
		m_pNodeTel->setVisible(false);
		break;
	case EWES_GOODS:
		m_pInfoSpr->setVisible(false);
		m_pNodeExchange->setVisible(false);
		m_pNodeRecord->setVisible(false);
		m_pNodeGoods->setVisible(true);
		m_pNodeTel->setVisible(false);
		_setExchangeGoodsName();
		break;
	case EWES_TEL:
		m_pInfoSpr->setVisible(false);
		m_pNodeExchange->setVisible(false);
		m_pNodeRecord->setVisible(false);
		m_pNodeGoods->setVisible(false);
		m_pNodeTel->setVisible(true);
		_setExchangeTelName();
		break;
	default:
		break;
	}
}

void WindowExchange::_setScrollContent()
{
	ui::ScrollView* pScrollView = dynamic_cast<ui::ScrollView*>(_seekNodeByName(m_pNodeExchange, "scroll"));

	Button* pBtn = nullptr;
	for (int i = 0; i < 12; i++)
	{
		pBtn = dynamic_cast<Button*>(_seekNodeByName(pScrollView,StringUtils::format("itembtn%d",i + 1)));
		pBtn->setTag(i);
		pBtn->addClickEventListener(CC_CALLBACK_1(WindowExchange::_ItemClick_Callback,this));
	}
}

void WindowExchange::_setExchangeRecord(CMD_ST_DxRewardList_rsp* pData)
{
	if (!pData)
	{
		return;
	}
	m_pRecordScroll->removeAllChildren();

	ImageView* pItem = nullptr;
	float posy = m_pRecordItem->getPositionY();
	float fHeight = m_pRecordItem->getContentSize().height;
	for (int i = 0; i < countarray(pData->aGoodsList); i++)
	{
		if (!pData->aGoodsList[i].isEmpty())
		{
			pItem = dynamic_cast<ImageView*>(m_pRecordItem->clone());
			pItem->setPositionY(posy);
			m_pRecordScroll->addChild(pItem);
			posy -= fHeight;

			_setRecordItem(pItem, &(pData->aGoodsList[i]));
		}
		
	}

	_adjustScrollHeight(m_pRecordScroll, -posy - fHeight / 2);
}

void WindowExchange::_setRecordItem(ImageView* pItem, ItemGoodsDx* pRecordData)
{
	if (!pItem || !pRecordData || pRecordData->isEmpty())
	{
		return;
	}

	Text* pTextData = dynamic_cast<Text*>(pItem->getChildByName("label_date"));
	Text* pTextName = dynamic_cast<Text*>(pItem->getChildByName("label_name"));
	Text* pTextId = dynamic_cast<Text*>(pItem->getChildByName("label_id"));
	Text* pTextState = dynamic_cast<Text*>(pItem->getChildByName("label_state"));

	pTextData->setString(pRecordData->dtime);
	pTextName->setString(pRecordData->itemName);
	pTextId->setString(pRecordData->orderNo);
	pTextState->setString(pRecordData->dstate == 0 ? GETSTRING(28) : GETSTRING(29));
}

void WindowExchange::_adjustScrollHeight(ui::ScrollView* pRecordScroll, float fIncreaseHeight)
{
	if (!pRecordScroll || fIncreaseHeight < 0)
	{
		return;
	}
	
	Size size = pRecordScroll->getContentSize();
	pRecordScroll->setInnerContainerSize(Size(size.width, size.height + fIncreaseHeight));

	for (auto item : pRecordScroll->getChildren())
	{
		item->setPositionY(item->getPositionY() + fIncreaseHeight);
	}
}

void WindowExchange::_initPhoneAndQQInput()
{
	TextField* pInputTel = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeTel, "tel_input"));
	TextField* pInputQQ = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeTel, "qq_input"));

	m_pTelPhoneNumInput = UIHallHelper::createAccountEditBox(pInputTel, 11, "images/hall/exchange/exchange_input_0.png");
	m_pTelQQInput = UIHallHelper::createAccountEditBox(pInputQQ, 15, "images/hall/exchange/exchange_input_0.png");

	TextField* pGoodsInputTel = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeGoods, "input_tel"));
	TextField* pGoodsInputQQ = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeGoods, "input_qq"));
	m_pGoodsPhoneNumInput = UIHallHelper::createAccountEditBox(pGoodsInputTel, 11, "images/hall/exchange/exchange_input_0.png");
	m_pGoodsQQInput = UIHallHelper::createAccountEditBox(pGoodsInputQQ, 15, "images/hall/exchange/exchange_input_0.png");
}

void WindowExchange::_setExchangeTelName()
{
	CC_ASSERT(m_nSelectIndex >= 0 && countarray(sExchangeConfig));

	Text* pName = dynamic_cast<Text*>(_seekNodeByName(m_pNodeTel, "label_tel_name"));
	pName->setString(GETSTRING(sExchangeConfig[m_nSelectIndex].nNameStringId).c_str());
}

void WindowExchange::_setExchangeGoodsName()
{
	CC_ASSERT(m_nSelectIndex >= 0 && countarray(sExchangeConfig));

	Text* pName = dynamic_cast<Text*>(_seekNodeByName(m_pNodeGoods, "label_name"));
	pName->setString(GETSTRING(sExchangeConfig[m_nSelectIndex].nNameStringId).c_str());
}

void WindowExchange::_ItemClick_Callback(Ref* pRef)
{
	playButtonClickSound();

	m_nSelectIndex = dynamic_cast<Button*>(pRef)->getTag();
	CC_ASSERT(m_nSelectIndex >= 0 && m_nSelectIndex < sizeof(sExchangeConfig) / sizeof(sExchangeConfig[0]));

	if (Player::getInstance()->getHallData().llYuanBao < sExchangeConfig[m_nSelectIndex].nPrice)
	{
		HallGameScene::getInstance()->openMessageTip(33);
		return;
	}

	switch (sExchangeConfig[m_nSelectIndex].nType)
	{
	case 1:		//打开提示框
		HallGameScene::getInstance()->openOkCancelDialog(
			StringUtils::format(GETSTRING(13).c_str(), sExchangeConfig[m_nSelectIndex].nPrice, GETSTRING(sExchangeConfig[m_nSelectIndex].nNameStringId).c_str()),
			this, m_nSelectIndex);
		
		break;
	case 2:
		_setUIState(EWES_TEL);
		break;
	case 3:
		_setUIState(EWES_GOODS);
		break;
	default:
		break;
	}
}

void WindowExchange::_ItemExchange_Callback(Ref* pRef)
{
	//先判断输入是否合法
	if (!GameUtil::isNum(m_pTelPhoneNumInput->getText()) || !GameUtil::isNum(m_pTelQQInput->getText()))
	{
		HallGameScene::getInstance()->openMessageTip(34);
		return;
	}

	//发送请求
	LogicExchange::getInstance()->setCostGold(sExchangeConfig[m_nSelectIndex].nPrice);
	LogicExchange::getInstance()->requestExchange(m_nSelectIndex, GameUtil::toLongLong(m_pTelPhoneNumInput->getText()), GameUtil::toLongLong(m_pTelQQInput->getText()));
	m_bRefreshRecord = true;
}

void WindowExchange::_GoodsExchange_Callback(Ref* pRef)
{
	TextField* pInputName = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeGoods, "input_name"));
	TextField* pInputAdress = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeGoods, "input_adress"));

	//先判断输入是否合法
	if (!GameUtil::isNum(m_pGoodsPhoneNumInput->getText()) || !GameUtil::isNum(m_pGoodsQQInput->getText()))
	{
		HallGameScene::getInstance()->openMessageTip(34);
		return;
	}

	//发送请求
	LogicExchange::getInstance()->setCostGold(sExchangeConfig[m_nSelectIndex].nPrice);
	LogicExchange::getInstance()->requestExchange(m_nSelectIndex, GameUtil::toLongLong(m_pGoodsPhoneNumInput->getText()), GameUtil::toLongLong(m_pGoodsQQInput->getText()),
		pInputName->getString(), pInputAdress->getString());
	m_bRefreshRecord = true;
}

void WindowExchange::refreshGold(Ref* ref)
{
	Text* pTextGold = dynamic_cast<Text*>(_seekNodeByName(m_pNode, "label_goldnum"));
	pTextGold->setString(StringUtils::format("%d", Player::getInstance()->getHallData().llYuanBao));
}