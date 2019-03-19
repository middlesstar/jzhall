#include "LogicShop.h"
#include "Player/Player.h"
#include "Device/Device.h"
#include "json/document.h"
//#include "json/stringbuffer.h"
//#include "json/writer.h"


#define RECHARGE_ORDERID_URL		"http://admin.92dwc.com/jz_recharge.aspx"						//请求订单url
#define RECHARGE_PAYFINISH_URL		"http://admin.92dwc.com/jz_appstore_validateV2.aspx"			//支付成功后，向服务器请求验证

LogicShop::LogicShop()
{
	_registerMessageHandler();
}

LogicShop::~LogicShop()
{

}

void LogicShop::_registerMessageHandler()
{
	NetManager* pNetManager = NetManager::getInstance();

	pNetManager->registerPakcetHandler(HALL_MDM_GP_WEBOP, HALL_SUB_WEB_RECHARGERSP, bind(&LogicShop::onBuySuccessResp, this, placeholders::_1, placeholders::_2));
}

void LogicShop::requestOrderId(int productId)
{
	int channelId = kChannel_iOS;
#if(CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	channelId = kChannel_android;
#endif

	cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
	request->setUrl(RECHARGE_ORDERID_URL);
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LogicShop::onHttpRequestCompleted, this));
	char requestContent[256] = { 0 };
	sprintf(requestContent, "{ \"userId\" : %d, \"productId\" : %d, \"channelId\" : %d }", Player::getInstance()->getLogonData().dwUserID, productId, channelId);
	log("%s", requestContent);
	std::vector<std::string> headers;
	headers.push_back("Content-Type: application/json; charset=utf-8");
	request->setHeaders(headers);
	request->setRequestData(requestContent, strlen(requestContent));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->setTag(StringUtils::format("%d", productId).c_str());
	request->release();
}


void LogicShop::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	if (!response)
	{
		log("[RechargeMission]onHttpRequestCompleted response error");
		return;
	}

	log("Http request is isSucceed = %d,code = %d", response->isSucceed(), response->getResponseCode());

	if (!response->isSucceed())
	{
		log("Http request is failed for : %s,code = %d", response->getErrorBuffer(), response->getResponseCode());
		return;
	}

	int nProductID = 0;
	sscanf(response->getHttpRequest()->getTag(), "%d", &nProductID);
    nProductID -= 1001;

	std::vector<char> *buffer = response->getResponseData();
	std::string responseString(buffer->begin(), buffer->end());

	if (responseString == "") {
		//G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
		return;
	}

	log("------------------onHttpRequestCompleted[responseString = %s]--------------------", responseString.c_str());

	bool isParseSuccess = true;

	rapidjson::Document d;
	d.Parse<0>(responseString.c_str());


	if (d.HasMember("ret"))
	{
		const rapidjson::Value &ret = d["ret"];
		if (ret.IsInt())
		{
			log("ret : %d", ret.GetInt());
			if (ret.GetInt() != 0)
			{
				isParseSuccess = false;
			}
		}
		else
		{
			isParseSuccess = false;
		}
	}

	std::string orderIdStr;
	if (d.HasMember("orderId"))
	{
		const rapidjson::Value &orderId = d["orderId"];
		if (orderId.IsString())
		{
			orderIdStr = orderId.GetString();
			log("orderId : %s", orderId.GetString());
		}
	}
	else
	{
		isParseSuccess = false;
	}

	std::string transIdStr;
	if (d.HasMember("transId"))
	{
		const rapidjson::Value &transId = d["transId"];
		if (transId.IsString())
		{
			transIdStr = transId.GetString();
			log("transIdStr : %s", transId.GetString());
		}

		if (atoi(transIdStr.c_str()) == 0)
		{
			isParseSuccess = false;
		}

	}
	else
	{
		isParseSuccess = false;

	}

	if (isParseSuccess)
	{
		//tagGlobalUserData* userData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		//start();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		JZ::Device::pay(nProductID, toString(Player::getInstance()->getLogonData().dwUserID), orderIdStr);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		//android充值部分
		JZ::Device::pay(transIdStr);
#endif
	}
	else
	{
		//G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
	}
}

void LogicShop::onBuySuccessResp(const PacketAide& packet, int nLen)
{
	CMD_Web_RechargeRsp resp;
	resp.fromPack(packet);

	log("----------------LogicShop::onBuySuccessResp[productid = %d,num = %d,sumrecharge = %d,currvip = %d]-----------------", resp.productid, resp.currgold,resp.sumrecharge,resp.currgold);

	//充值成功返回
	HallGameScene::getInstance()->openMessageTip(45);

	Player* pPlayer = Player::getInstance();
	pPlayer->setTotalRecharge(resp.sumrecharge);
	pPlayer->setVipLevel(resp.currvip);
	pPlayer->setCoin(resp.currgold);

	//首冲，赠送4张船票
	if (!pPlayer->getShopItemRecharged(resp.productid - 1001))
	{
		pPlayer->addTicket(4);
		HALL_NOTIFICATION(Hall_Message_Refresh_Ticket, nullptr);
	}
	pPlayer->setShopItemRecharged(resp.productid - 1001);

	HallGameScene::getInstance()->updateWindow(EWT_Shop,1);

	HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
	HALL_NOTIFICATION(Hall_Message_Refresh_VIP, nullptr);
	
}