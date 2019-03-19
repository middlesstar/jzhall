#include "WindowLogin.h"
#include "Player/Player.h"
#include "WindowLoginAccount.h"
#include "LoginDataFieldDef.h"
#include "Util/GameUtil.h"
#include "LanguageConfig.h"
#include "df/DF.h"

WindowLogin::WindowLogin()
{

}

WindowLogin::~WindowLogin()
{

}

void WindowLogin::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZLoginLayer.csb");
	CCASSERT(pMainNode, "WindowLogin::setContent() layout/JZLoginLayer.json load nullptr!");

	m_pNode->addChild(pMainNode);


	_addButtonClickEvent(pMainNode, "btn_account", [this](Ref* sender)
	{
		/* -----------昵称登入按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_LoginAccount);
		HallGameScene::getInstance()->updateWindow(EWT_LoginAccount, EWLS_LOGIN);
	});

	_addButtonClickEvent(pMainNode, "btn_service", [this](Ref* sender)
	{
		/* -----------客服按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_Service);
	});

	_addButtonClickEvent(pMainNode, "btn_back", [this](Ref* sender)
	{
		/* -----------返回按钮------------- */
		HallGameScene::getInstance()->openOkCancelDialog(GETSTRING(3), nullptr, ECCNT_EXIT);
	});

	_addButtonClickEvent(pMainNode, "btn_change", [this](Ref* sender)
	{
		/* -----------切换账号按钮------------- */
		HallGameScene::getInstance()->openWindow(EWT_LoginAccount);
		HallGameScene::getInstance()->updateWindow(EWT_LoginAccount, EWSS_CHANGE);
	});

	_addButtonClickEvent(pMainNode, "btn_start", [this](Ref* sender)
	{
		/* -----------开始游戏按钮------------- */
		Player* pPlayer = Player::getInstance();

		if (!pPlayer->getAccount().empty() && !pPlayer->getPassword().empty())
		{
			pPlayer->requestLogin(pPlayer->getAccount(), pPlayer->getPassword());
		}
		else
		{
			pPlayer->requestVisitorLogin();			//请求游客登录
		}
	});

	_addButtonLight();
	_initLoginAccount();
}

void WindowLogin::updateWindow(int nCode)
{
	if (nCode == 0)
	{
		_saveLoginAccount();
	}
	else if (nCode == 1)
	{
		_setLoginAccount(Player::getInstance()->getAccount());
	}
	else
	{
		_setLoginAccount(GETSTRING(38));
	}
}

void WindowLogin::_initLoginAccount()
{
	string strGameId = GameUtil::getData(LoginDataGameID, "");

	vector<int> vecGameId;
	GameUtil::getSplitData(strGameId, vecGameId, '|');

	if (vecGameId.empty())
	{
		_setLoginAccount(GETSTRING(38));
	}
	else
	{
		string strNickName = GameUtil::getData(StringUtils::format("%s_%d", LoginDataFieldNickName, vecGameId.at(0)).c_str(), "");
		string strPassword = GameUtil::getData(StringUtils::format("%s_%d", LoginDataFieldPassword, vecGameId.at(0)).c_str(), "");
		
		Player::getInstance()->setAccount(strNickName);
		Player::getInstance()->setPassword(strPassword);
		
		_setLoginAccount(strNickName);
	}
}

void WindowLogin::_setLoginAccount(const std::string& strNickName)
{
	Text* pAccountName = dynamic_cast<Text*>(_seekNodeByName(m_pNode,"text_account"));
	if (pAccountName)
	{
		pAccountName->setString(strNickName);
	}
}

void WindowLogin::_saveLoginAccount()
{
	//此处保存登录账号
	string strGameId = GameUtil::getData(LoginDataGameID, "");

	vector<int> vecGameId;
	GameUtil::getSplitData(strGameId, vecGameId, '|');

	Player* pPlayer = Player::getInstance();
	int nLoginGameId = pPlayer->getLogonData().dwUserID;

	if (!vecGameId.empty())
	{
		for (vector<int>::iterator it = vecGameId.begin(); it != vecGameId.end(); it++)
		{
			if (*it == nLoginGameId)
			{
				vecGameId.erase(it);
				break;
			}
		}
	}

	vecGameId.insert(vecGameId.begin(), nLoginGameId);
	strGameId.clear();
	for (auto& it : vecGameId)
	{
		if (strGameId.empty())
		{
			strGameId = StringUtils::format("%d",it);
		}
		else
		{
			strGameId = StringUtils::format("%s|%d", strGameId.c_str(), it);
		}
	}
	GameUtil::setData(LoginDataGameID, strGameId);

	string strNickName = StringUtils::format("%s_%d", LoginDataFieldNickName, nLoginGameId);
	string strPassword = StringUtils::format("%s_%d", LoginDataFieldPassword, nLoginGameId);
	GameUtil::setData(strNickName.c_str(), pPlayer->getLogonData().szNickName);
	
	//密码是明码时加密
	if (!pPlayer->getPassword().empty())
	{
		if (pPlayer->getPassword().length() < 32)
		{
			GameUtil::setData(strPassword.c_str(), DF::MD5Encrypt(pPlayer->getPassword().c_str()));
		}
		else
		{
			GameUtil::setData(strPassword.c_str(), pPlayer->getPassword().c_str());
		}
	}
	
}

void WindowLogin::_addButtonLight()
{
	//开始按钮上的光效
	Node* pNodeLight = _seekNodeByName(m_pNode, "node_light");

	Animation* pAnimation = Animation::create();
	for (int i = 0; i < 5; i++)
	{
		pAnimation->addSpriteFrameWithFile(StringUtils::format("effect/animation/light/bright_%d.png", i));
	}
	pAnimation->setDelayPerUnit(0.1f);
	Animate* pAnimate = Animate::create(pAnimation);
	Sprite* pSprite = Sprite::create();
	pSprite->runAction(RepeatForever::create(pAnimate));
	pNodeLight->addChild(pSprite);
}