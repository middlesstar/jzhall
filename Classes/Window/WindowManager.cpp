#include "WindowManager.h"
#include "WindowDefine.h"

#include "login/WindowLogin.h"
#include "login/WindowLoginAccount.h"
#include "login/WindowService.h"
#include "hall/WindowHall.h"
#include "hall/WindowPersonal.h"
#include "hall/WindowTicketSend.h"
#include "setting/WindowSetting.h"
#include "moneytree/WindowMoneyTree.h"
#include "moneytree/WindowTreeUpgrade.h"
#include "bank/WindowBank.h"
#include "freemoney/WindowFreeMoney.h"
#include "freemoney/WindowFMSign.h"
#include "freemoney/WindowFMDialyPrize.h"
#include "freemoney/WindowFMWeixin.h"
#include "freemoney/WindowFMComment.h"
#include "freemoney/WindowFMDole.h"
#include "mail/WindowMail.h"
#include "shop/WindowShop.h"
#include "vipgift/WindowVipGift.h"
#include "exchange/WindowExchange.h"
#include "promote/WindowPromote.h"
#include "promote/WindowShare.h"
#include "expedition/WindowExpedition.h"
#include "activity/WindowActivity.h"
#include "chat/WindowChat.h"
#include "chat/WindowSendRedPacket.h"
#include "chat/WindowChatPacketResult.h"
#include "activity/ConcludeNotice.h"
#include "update/GameUpdate.h"

WindowManager* WindowManager::m_gpWindowManager = nullptr;

WindowManager* WindowManager::getInstance()
{
	if (!m_gpWindowManager)
	{
		m_gpWindowManager = new WindowManager;
	}

	return m_gpWindowManager;
}

WindowManager::WindowManager()
{
	//init();
}

WindowManager::~WindowManager()
{

}

bool WindowManager::init()
{
	/* ---------新加的窗口文件在此注册-------- */

 	REGISTER_WINDOW(EWT_Login,		WindowLogin);
	REGISTER_WINDOW(EWT_LoginAccount,WindowLoginAccount);
	REGISTER_WINDOW(EWT_Service,	WindowService);
 	REGISTER_WINDOW(EWT_Hall,		WindowHall);
 	REGISTER_WINDOW(EWT_Shop,		WindowShop);
 	REGISTER_WINDOW(EWT_MoneyTree,	WindowMoneyTree);
 	REGISTER_WINDOW(EWT_FreeMoney,	WindowFreeMoney);
 	//REGISTER_WINDOW(EWT_Rank,		WindowRank);
 	REGISTER_WINDOW(EWT_Promote,	WindowPromote);
 	REGISTER_WINDOW(EWT_Exchange,	WindowExchange);
 	REGISTER_WINDOW(EWT_VipGift,	WindowVipGift);
 	REGISTER_WINDOW(EWT_Bank,		WindowBank);
 	REGISTER_WINDOW(EWT_Mail,		WindowMail);
 	REGISTER_WINDOW(EWT_Setting,	WindowSetting);
 	REGISTER_WINDOW(EWT_Personal,	WindowPersonal);
	REGISTER_WINDOW(EWT_TicketSend, WindowTicketSend);
 	REGISTER_WINDOW(EWT_Sign,		WindowFMSign);
 	REGISTER_WINDOW(EWT_DialyPrize, WindowFMDialyPrize);
 	REGISTER_WINDOW(EWT_WeiXin,		WindowFMWeixin);
 	REGISTER_WINDOW(EWT_Comment,	WindowFMComment);
	REGISTER_WINDOW(EWT_Dole,		WindowFMDole);
	REGISTER_WINDOW(EWT_Expedition, WindowExpedition);
	REGISTER_WINDOW(EWT_Activity,	ConcludeNotice);
	REGISTER_WINDOW(EWT_Share,		WindowShare);
	REGISTER_WINDOW(EWT_TreeUpgrade,WindowTreeUpgrade);
	REGISTER_WINDOW(EWT_Chat,		WindowChat);
	REGISTER_WINDOW(EWT_SendRedPacket, WindowSendRedPacket);
	REGISTER_WINDOW(EWT_PacketResult, WindowChatPacketResult);

	//tosl
	REGISTER_WINDOW(EWT_Update, GameUpdate);
	return true;
}

void WindowManager::addWindowFactory(int winid, WindowFactory* pWindowFactory)
{
	std::map<int, WindowFactory*>::iterator it = m_mapWindowFactory.find(winid);
	if (it != m_mapWindowFactory.end())
	{
		delete it->second;
		it->second = nullptr;

		m_mapWindowFactory.erase(it);
	}

	m_mapWindowFactory[winid] = pWindowFactory;
}

WindowFactory* WindowManager::getWindowFactory(int winid)
{
	std::map<int, WindowFactory*>::iterator it = m_mapWindowFactory.find(winid);

	if (it != m_mapWindowFactory.end())
	{
		return it->second;
	}

	return nullptr;
}

