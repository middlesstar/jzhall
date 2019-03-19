#include "WindowMail.h"
#include "LogicMail.h"
#include "LanguageConfig.h"

WindowMail::WindowMail()
	: m_pImgItem(nullptr)
	, m_pScroll(nullptr)
	, m_pNodeContent(nullptr)
{

}

WindowMail::~WindowMail()
{
	CC_SAFE_RELEASE_NULL(m_pImgItem);
}

void WindowMail::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZMail.csb");
	CCASSERT(pMainNode, "WindowMail::setContent() layout/JZMail.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();
	
	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});

	m_pScroll = dynamic_cast<ui::ScrollView*>(_seekNodeByName(pMainNode,"scroll"));
	m_pImgItem = dynamic_cast<ImageView*>(m_pScroll->getChildByName("item"));
	m_pImgItem->removeFromParent();
	m_pImgItem->retain();

	m_pNodeContent = _seekNodeByName(pMainNode, "node_content");
	_addButtonClickEvent(m_pNodeContent, "btn_ok", [this](Ref* sender){this->_setUIState(EWMS_List); });
	_addButtonClickEvent(m_pNodeContent, "btn_get", CC_CALLBACK_1(WindowMail::_getAttachGoodsClick,this));

	_setUIState(EWMS_List);
}

void WindowMail::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}

void WindowMail::updateWindow(int nCode, void* pData)
{
	if (nCode == 0)
	{
		_setMailList((CMD_ST_GetEmailList_rsp*)pData);
	}
	else
	{
		_setUIState(EWMS_List);
	}
}

void WindowMail::_setUIState(int nState)
{
	switch (nState)
	{
	case EWMS_List:
		m_pScroll->setVisible(true);
		m_pNodeContent->setVisible(false);
		break;
	case EWMS_Content:
		m_pScroll->setVisible(false);
		m_pNodeContent->setVisible(true);
		break;
	default:
		break;
	}
}

void WindowMail::_setMailList(CMD_ST_GetEmailList_rsp* pMailData)
{
	if (! pMailData)
	{
		return;
	}
	m_pScroll->removeAllChildren();

	ImageView* pItem = nullptr;
	float posy = m_pImgItem->getPositionY();
	float fHeight = m_pImgItem->getContentSize().height;
	for (int i = 0; i < countarray(pMailData->emaillist); i++)
	{
		if (!pMailData->emaillist[i].isEmpty())
		{
			pItem = dynamic_cast<ImageView*>(m_pImgItem->clone());
			pItem->setPositionY(posy);
			m_pScroll->addChild(pItem);
			posy -= fHeight;

			_setMailItem(pItem, &(pMailData->emaillist[i]));
		}

	}

	_adjustScrollHeight(m_pScroll, -posy - fHeight / 2);
}

void WindowMail::_setMailItem(ImageView* pItem, EmailInfoSt* pMail)
{
	if (!pItem || !pMail)
	{
		return;
	}

	Text* pTextSender = dynamic_cast<Text*>(pItem->getChildByName("label_sender"));
	Text* pTextTitle = dynamic_cast<Text*>(pItem->getChildByName("label_title"));
	Text* pTextTime = dynamic_cast<Text*>(pItem->getChildByName("label_time"));

	pTextSender->setString(pMail->sendusername);
	pTextTitle->setString(pMail->emailtitle);
	pTextTime->setString(pMail->sendtime);

	pItem->addClickEventListener(CC_CALLBACK_1(WindowMail::_MailItemClick,this));
	pItem->setUserData(pMail);
}

void WindowMail::_adjustScrollHeight(ui::ScrollView* pScroll, float fIncreaseHeight)
{
	if (!pScroll || fIncreaseHeight < 0)
	{
		return;
	}

	Size size = pScroll->getContentSize();
	pScroll->setInnerContainerSize(Size(size.width, size.height + fIncreaseHeight));

	for (auto item : pScroll->getChildren())
	{
		item->setPositionY(item->getPositionY() + fIncreaseHeight);
	}
}

void WindowMail::_setMailContent(EmailInfoSt* pMail)
{
	if (!pMail)
	{
		return;
	}

	Text* pTextTitle = dynamic_cast<Text*>(_seekNodeByName(m_pNodeContent,"Text_title"));
	Text* pTextTime = dynamic_cast<Text*>(_seekNodeByName(m_pNodeContent, "Text_time"));
	TextField* pTextContent = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeContent, "Text_content"));

	pTextTitle->setString(pMail->emailtitle);
	pTextTime->setString(StringUtils::format(GETSTRING(50).c_str(), pMail->sendtime));
	pTextContent->setString(pMail->emialcontent);

	Node* pNodeMsg = _seekNodeByName(m_pNodeContent, "node_msg");
	Node* pNodeGoods = _seekNodeByName(m_pNodeContent, "node_goods");

	if (pMail->attachtype == 0 && pMail->attachtype2 == 0)		//无附件
	{
		pNodeMsg->setVisible(true);
		pNodeGoods->setVisible(false);
	}
	else    //有附件
	{
		pNodeMsg->setVisible(false);
		pNodeGoods->setVisible(true);

		//2个附件
		ImageView* pImgIcon = dynamic_cast<ImageView*>(_seekNodeByName(pNodeGoods, "img_goods"));
		if (pMail->attachtype != 0 && pMail->attachtype2 != 0)
		{
			ImageView* pImgAtt1 = dynamic_cast<ImageView*>(pImgIcon->clone());
			ImageView* pImgAtt2 = dynamic_cast<ImageView*>(pImgIcon->clone());
			Text* pText1 = dynamic_cast<Text*>(_seekNodeByName(pImgAtt1, "text_count"));
			Text* pText2 = dynamic_cast<Text*>(_seekNodeByName(pImgAtt2, "text_count"));

			pImgAtt1->setPositionX(pImgIcon->getPositionX() - pImgIcon->getContentSize().width * 2 / 3);
			pImgAtt2->setPositionX(pImgIcon->getPositionX() + pImgIcon->getContentSize().width * 2 / 3);

			pImgAtt1->loadTexture(_getAttachGoodsImgPath(pMail->attachtype), TextureResType::PLIST);
			pImgAtt2->loadTexture(_getAttachGoodsImgPath(pMail->attachtype2), TextureResType::PLIST);
			pText1->setString(StringUtils::format("%d", pMail->attachnums));
			pText2->setString(StringUtils::format("%d", pMail->attachnums2));

			pImgIcon->getParent()->addChild(pImgAtt1);
			pImgIcon->getParent()->addChild(pImgAtt2);

			pImgIcon->removeFromParent();
		}
		else
		{
			Text* pText = dynamic_cast<Text*>(_seekNodeByName(pImgIcon, "text_count"));

			pImgIcon->loadTexture(_getAttachGoodsImgPath(pMail->attachtype == 0 ? pMail->attachtype2 : pMail->attachtype), TextureResType::PLIST);
			pText->setString(StringUtils::format("%d", pMail->attachtype == 0 ? pMail->attachnums2 : pMail->attachnums));
		}
		
	}
}

void WindowMail::_MailItemClick(Ref* pRef)
{
	playButtonClickSound();

	ImageView* pItem = dynamic_cast<ImageView*>(pRef);

	if (!pRef || !pItem->getUserData())
	{
		return;
	}
	
	EmailInfoSt* pMailData = (EmailInfoSt*)pItem->getUserData();
	_setMailContent(pMailData);
	_setUIState(EWMS_Content);

	LogicMail* pLogic = LogicMail::getInstance();
	pLogic->setCurClickMail(*pMailData);

	if (!pMailData->hasAttach())
	{
		pLogic->requestMailsOperate(EEOT_DELETE, pMailData->eid);

		pLogic->deleteEmailByEid(pMailData->eid);
		_setMailList(pLogic->getEmailListData());
	}
}

void WindowMail::_getAttachGoodsClick(Ref* pRef)
{
	LogicMail* pLogic = LogicMail::getInstance();

	if (!pLogic->getCurClickMail().isEmpty())
	{
		pLogic->requestMailsOperate(EEOT_GET_ATTACH,pLogic->getCurClickMail().eid);

		pLogic->deleteEmailByEid(pLogic->getCurClickMail().eid);
		_setMailList(pLogic->getEmailListData());
	}
}

std::string WindowMail::_getAttachGoodsImgPath(int nType)
{
	string strRetPath;

	switch (nType)
	{
	case 1:
		strRetPath = "images/hall/mail/mail_gold.png";
		break;
	case 2:
		strRetPath = "images/hall/mail/mail_diamond.png";
		break;
	case 3:
		strRetPath = "images/hall/mail/mail_ticket.png";
		break;
	default:
		break;
	}

	return strRetPath;
}