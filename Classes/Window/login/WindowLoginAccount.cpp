#include "WindowLoginAccount.h"
#include "Player/Player.h"
#include "LoginDataFieldDef.h"
#include "Util/GameUtil.h"
#include "df/DF.h"
#include "LanguageConfig.h"

WindowLoginAccount::WindowLoginAccount()
	: m_pNodeLogin(nullptr)
	, m_pNodeNew(nullptr)
	, m_pNodeChange(nullptr)
	, m_pScroll(nullptr)
	, m_pItem(nullptr)
	, m_pImgSelect(nullptr)
	, m_pSelectItem(nullptr)
{

}

WindowLoginAccount::~WindowLoginAccount()
{
	CC_SAFE_RELEASE_NULL(m_pItem);
}

void WindowLoginAccount::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZLoginAccount.csb");
	CCASSERT(pMainNode, "WindowLoginAccount::setContent() layout/JZLoginAccount.csb load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	m_pNodeLogin = _seekNodeByName(pMainNode, "node_account");
	m_pNodeNew = _seekNodeByName(pMainNode, "node_new");
	m_pNodeChange = _seekNodeByName(pMainNode, "node_select");

	_initLoginNode();
	_initNewNode();
	_initChangeNode();
}

void WindowLoginAccount::updateWindow(int nCode)
{
	if (nCode != EWLS_LOGIN && nCode != EWSS_NEW && nCode != EWSS_CHANGE)
	{
		return;
	}

	_setUIState(nCode);
}

void WindowLoginAccount::onOkClick(int nData, void* pData)
{
	if (nData > 0)
	{
		_deleteAccountCache(nData);
	}
}

void WindowLoginAccount::_setUIState(int nState)
{
	if (!m_pNodeLogin || !m_pNodeNew || !m_pNodeChange)
	{
		return;
	}

	m_pNodeLogin->setVisible(nState == EWLS_LOGIN);
	m_pNodeNew->setVisible(nState == EWSS_NEW);
	m_pNodeChange->setVisible(nState == EWSS_CHANGE);
}

void WindowLoginAccount::_initLoginNode()
{
	if (!m_pNodeLogin)
	{
		return;
	}

	_addButtonClickEvent(m_pNodeLogin, "btn_close", [this](Ref* sender){this->close(); });
	_addButtonClickEvent(m_pNodeLogin, "btn_new", [this](Ref* sender){this->_setUIState(EWSS_NEW); });
	_addButtonClickEvent(m_pNodeLogin, "btn_ok", [this](Ref* sender)
	{
		//请求登录
		TextField* pAccount = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeLogin,"text_account"));
		TextField* pPwd = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeLogin, "text_password"));

		if (pAccount && pPwd && !pAccount->getString().empty() && !pPwd->getString().empty())
		{
			Player::getInstance()->requestLogin(pAccount->getString(), DF::MD5Encrypt(pPwd->getString().c_str()));
		}
	});
}

void WindowLoginAccount::_initNewNode()
{
	if (!m_pNodeNew)
	{
		return;
	}

	_addButtonClickEvent(m_pNodeNew, "btn_close", [this](Ref* sender){this->close(); });
	_addButtonClickEvent(m_pNodeNew, "btn_ok", [this](Ref* sender)
	{
		//请求新建账号

		TextField* pAccount = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeNew, "text_account"));
		TextField* pPwd = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeNew, "text_password"));
		TextField* pPwdConfirm = dynamic_cast<TextField*>(_seekNodeByName(m_pNodeNew, "text_password_confirm"));

		if (pAccount && pPwd && pPwdConfirm)
		{
			if (pAccount->getString().empty() || pPwd->getString().empty() || pPwdConfirm->getString().empty())
			{
				HallGameScene::getInstance()->openMessageTip(37);
				return;
			}

			if (pAccount->getString().length() < 6 || pAccount->getString().length() > 15)
			{
				HallGameScene::getInstance()->openMessageTip(41);
				return;
			}

			if (pPwd->getString() != pPwdConfirm->getString())
			{
				HallGameScene::getInstance()->openMessageTip(36);
				return;
			}

			if (pPwd->getString().length() < 6 || pPwd->getString().length() > 15)
			{
				HallGameScene::getInstance()->openMessageTip(42);
				return;
			}

			Player::getInstance()->requestRegister(pAccount->getString(), pPwd->getString());
		}
	});
}

void WindowLoginAccount::_initChangeNode()
{
	if (!m_pNodeChange)
	{
		return;
	}

	_initChangeScroll();

	_addButtonClickEvent(m_pNodeChange, "btn_close", [this](Ref* sender){this->close(); });
	_addButtonClickEvent(m_pNodeChange, "btn_new", [this](Ref* sender){this->_setUIState(EWSS_NEW); });
	_addButtonClickEvent(m_pNodeChange, "btn_visitor", [this](Ref* sender)
	{
		HallGameScene::getInstance()->updateWindow(EWT_Login, 2);
		Player::getInstance()->setAccount("");
		Player::getInstance()->setPassword("");
		this->close();
	});

	_addButtonClickEvent(m_pNodeChange, "btn_ok", [this](Ref* sender)
	{
		if (m_pSelectItem)
		{
			Player::getInstance()->setAccount(dynamic_cast<Text*>(m_pSelectItem->getChildByName("text_name"))->getString());
			Player::getInstance()->setPassword(m_pSelectItem->getName());

			HallGameScene::getInstance()->updateWindow(EWT_Login, 1);
			this->close();
		}
		
	});
}

void WindowLoginAccount::_initChangeScroll(bool bFirst/* = true*/)
{
	if (bFirst)
	{
		m_pScroll = dynamic_cast<ui::ScrollView*>(_seekNodeByName(m_pNodeChange, "scroll"));
		m_pItem = dynamic_cast<Layout*>(m_pScroll->getChildByName("item"));
		m_pImgSelect = dynamic_cast<ImageView*>(m_pScroll->getChildByName("img_select"));

		m_pItem->retain();
		m_pItem->removeFromParent();
		m_pImgSelect->setVisible(false);
	}
	else
	{
		m_pImgSelect->retain();
		m_pScroll->removeAllChildren();
		m_pScroll->addChild(m_pImgSelect);
		m_pImgSelect->release();
		m_pScroll->setInnerContainerSize(m_pScroll->getContentSize());
	}
	

	string strGameId = GameUtil::getData(LoginDataGameID, "");

	vector<int> vecGameId;
	GameUtil::getSplitData(strGameId, vecGameId, '|');

	Size itemSize = m_pItem->getContentSize();
	float fOffsetY = 0;
	if (vecGameId.empty())
	{

	}
	else
	{
		string strNickName;
		string strPassword;
		Layout* pItem = nullptr;
		int nItemCount = 0;
		for (auto& it : vecGameId)
		{
			strNickName = GameUtil::getData(StringUtils::format("%s_%d", LoginDataFieldNickName, it).c_str(),"");
			strPassword = GameUtil::getData(StringUtils::format("%s_%d", LoginDataFieldPassword, it).c_str(),"");

			if (!strNickName.empty() && !strPassword.empty())
			{
				pItem = dynamic_cast<Layout*>(m_pItem->clone());
				_setAccountItem(pItem, strNickName, strPassword,it);
				pItem->setPositionY(m_pItem->getPositionY() - fOffsetY);
				fOffsetY += itemSize.height;

				m_pScroll->addChild(pItem);
				nItemCount++;
			}
		}

		//调整位置
		Size innerSize = m_pScroll->getInnerContainerSize();
		if (nItemCount * itemSize.height > innerSize.height)
		{
			float fItemOffsetY = nItemCount * itemSize.height - innerSize.height;
			m_pScroll->setInnerContainerSize(Size(innerSize.width, nItemCount * itemSize.height));

			for (auto & child : m_pScroll->getChildren())
			{
				child->setPositionY(child->getPositionY() + fItemOffsetY);
			}
		}
	}

	if (m_pScroll->getChildrenCount() > 1)
	{
		m_pSelectItem = nullptr;
		_setItemSelect(dynamic_cast<Layout*>(m_pScroll->getChildren().at(1)));		//第0个child是m_pImgSelect
	}
}

void WindowLoginAccount::_deleteAccountCache(int nUserid)
{
	//先删除缓存中的记录
	string strNickName = StringUtils::format("%s_%d", LoginDataFieldNickName, nUserid);
	string strPassword = StringUtils::format("%s_%d", LoginDataFieldPassword, nUserid);
	UserDefault::getInstance()->deleteValueForKey(strNickName.c_str());
	UserDefault::getInstance()->deleteValueForKey(strPassword.c_str());

	string strGameId = GameUtil::getData(LoginDataGameID, "");

	vector<int> vecGameId;
	GameUtil::getSplitData(strGameId, vecGameId, '|');

	for (vector<int>::iterator it = vecGameId.begin(); it != vecGameId.end(); it++)
	{
		if (*it == nUserid)
		{
			vecGameId.erase(it);
			break;
		}
	}

	strGameId.clear();
	for (auto& it : vecGameId)
	{
		if (strGameId.empty())
		{
			strGameId = StringUtils::format("%d", it);
		}
		else
		{
			strGameId = StringUtils::format("%s|%d", strGameId.c_str(), it);
		}
	}
	GameUtil::setData(LoginDataGameID, strGameId);

	_initChangeScroll(false);
}

void WindowLoginAccount::_setAccountItem(Layout* pItem, const std::string& strAccount, const std::string& strPassword, int nUserID)
{
	if (!pItem)
	{
		return;
	}

	Text* pTextName = dynamic_cast<Text*>(pItem->getChildByName("text_name"));
	Button* pDelete = dynamic_cast<Button*>(pItem->getChildByName("btn_delete"));

	pTextName->setText(strAccount);
	pItem->setName(strPassword);
	pItem->setTag(nUserID);

	pDelete->addClickEventListener(CC_CALLBACK_1(WindowLoginAccount::_ItemDeleteCallback,this));
	pDelete->setTag(nUserID);
	pDelete->setVisible(false);

	pItem->addClickEventListener(CC_CALLBACK_1(WindowLoginAccount::_ItemClickCallback, this));
}

void WindowLoginAccount::_setItemSelect(Layout* pItem)
{
	if (!pItem)
	{
		return;
	}

	if (m_pSelectItem)
	{
		m_pSelectItem->getChildByName("btn_delete")->setVisible(false);
	}

	m_pSelectItem = pItem;
	m_pImgSelect->setVisible(true);
	Size itemSize = pItem->getContentSize();
	m_pImgSelect->setPosition(Vec2(pItem->getPositionX() + itemSize.width / 2, pItem->getPositionY() + itemSize.height / 2));

	if (m_pScroll && m_pScroll->getChildrenCount() > 2)
	{
		pItem->getChildByName("btn_delete")->setVisible(true);
	}
}

void WindowLoginAccount::_ItemClickCallback(Ref* pRef)
{
	_setItemSelect(dynamic_cast<Layout*>(pRef));
}

void WindowLoginAccount::_ItemDeleteCallback(Ref* pRef)
{
	Button* pButton = dynamic_cast<Button*>(pRef);

	if (!pButton)
	{
		return;
	}

	HallGameScene::getInstance()->openOkCancelDialog(GETSTRING(40), this, pButton->getTag());
}