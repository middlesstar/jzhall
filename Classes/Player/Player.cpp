#include "Player.h"
#include "df/DF.h"
#include "df/ServerListData.h"
#include "HallGameScene.h"
#include "Window/CustomMessage.h"
#include "Window/hall/WindowHall.h"
#include "Window/login/LoginDataFieldDef.h"
#include "Util/GameUtil.h"
#include "Custom/GoldFallen.h"

#include "shuihu/plazz/data/GlobalUserInfo_SHZ.h"

#define URL_VERSION_INFO		"http://admin.92dwc.com/version.aspx"

//累计充值表
int nSumRechargeTable[] = { 0, 30, 100, 300, 800, 2000, 5000, 10000, 20000, 50000, 100000 };

Player::Player()
	: m_nSendTicketCounts(0)
	, m_nVersion(0)
	, m_nIosFlag(0)
{
	_registerMessageHandler();
}

Player::~Player()
{

}

void Player::_registerMessageHandler()
{
	NetManager* pNetManager = NetManager::getInstance();

	//注册交互消息
	pNetManager->registerPakcetHandler(HALL_MDM_GP_LOGON, HALL_SUB_GP_LOGON_SUCCESS, bind(&Player::onLogonSuccess, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_LOGON, HALL_SUB_GP_HALL_INFO, bind(&Player::onLogonHallInfo, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVER_LIST, HALL_SUB_GP_LIST_TYPE, bind(&Player::onGameType, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVER_LIST, HALL_SUB_GP_LIST_KIND, bind(&Player::onGameKind, this, placeholders::_1, placeholders::_2));
	//pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVER_LIST, HALL_SUB_GP_LIST_PAGE, bind(&Player::onGamePage, this, placeholders::_1, placeholders::_2));
	//pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVER_LIST, HALL_SUB_GP_LIST_NODE, bind(&Player::onGameNode, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVER_LIST, HALL_SUB_GP_LIST_SERVER, bind(&Player::onGameServer, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_LOGON, HALL_SUB_GP_LOGON_FINISH, bind(&Player::onLogonFinish, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_LOGON, HALL_SUB_GP_LOGON_FAILURE, bind(&Player::onLogonFailure, this, placeholders::_1, placeholders::_2));

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_USER_FACE_INFO, bind(&Player::onModifyIconResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_MODIFLYUSER_RSP, bind(&Player::onModifyNicknameResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GR_USER_INSURE_SUCCESS, bind(&Player::onBankOperateSuccess, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_USER_INSURE_FAILURE, bind(&Player::onBankOperateFailure, this, placeholders::_1, placeholders::_2));

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GIVECHUANPIAO_RSP, bind(&Player::onSendTicketResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_BINDACCOUNT_RSP, bind(&Player::onModifyAccountResp, this, placeholders::_1, placeholders::_2));

	pNetManager->registerPakcetHandler(HALL_MDM_GP_SERVICE, HALL_SUB_NETFLUSHUSER_RSP, bind(&Player::onFlushServerInfoResp, this, placeholders::_1, placeholders::_2));
	pNetManager->registerPakcetHandler(HALL_MDM_GP_LOGON, SUB_GP_UPDATE_NOTIFY, bind(&Player::onUpdateNotify, this, placeholders::_1, placeholders::_2));
}

void Player::requestVersionInfo()
{
	cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
	request->setUrl(URL_VERSION_INFO);
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(Player::onVersionInfoResp, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void Player::onVersionInfoResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	if (!response)
	{
		log("[Player]onVersionInfoResp response error");
		return;
	}

	log("Http request is isSucceed = %d,code = %d", response->isSucceed(), response->getResponseCode());

	if (!response->isSucceed())
	{
		log("Http request is failed for : %s,code = %d", response->getErrorBuffer(), response->getResponseCode());
		return;
	}


	std::vector<char> *buffer = response->getResponseData();
	std::string responseString(buffer->begin(), buffer->end());

	log("------------------onVersionInfoResp[responseString = %s]--------------------", responseString.c_str());

	rapidjson::Document d;
	d.Parse<0>(responseString.c_str());

    if (d.HasParseError())
    {
        log("---------------onVersionInfoResp format error-----------------");
        return;
    }
    
	if (d.HasMember("ver") && d["ver"].IsInt())
	{
		m_nVersion = d["ver"].GetInt();
	}

	if (d.HasMember("isios") && d["isios"].IsInt())
	{
		m_nIosFlag = d["isios"].GetInt();
	}
}

void Player::requestVisitorLogin()
{
	//PACKET_AIDE_SIZE(1 + 1 + LEN_MOBILE_PHONE * 2 + LEN_MACHINE_ID * 2);

	//packet.writeString(DF::shared()->GetMachineID(), LEN_MACHINE_ID);
	//packet.writeString(DF::shared()->GetMobilePhone(), LEN_MOBILE_PHONE);
	//packet.writeByte(rand() % 12);
	//packet.writeByte(MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS);

	tagHallGPVisitorLogon req;
	req.cbDeviceType = DF::shared()->GetDeviceType();
	req.wModuleID = 0;
	req.dwPlazaVersion = DF::shared()->GetPlazaVersion();
	req.setString(req.szMachineID, DF::shared()->GetMachineID());
	req.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
	req.setString(req.szPassword, DF::MD5Encrypt(DF::shared()->GetMachineID()));

	m_strPassword = DF::MD5Encrypt(DF::shared()->GetMachineID());			//保存密码

    log("--------Player::requestVisitorLogin[req.szMachineID = %s]-------",req.szMachineID);
    
	PACKET_AIDE_SIZE_NEW(256);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_LOGON, HALL_SUB_GR_LOGON_QUICK, packet);
}

void Player::requestRegister(const string& strAcc, const string& strPsw)
{
	m_strAccount = strAcc;
	m_strPassword = strPsw;

	CMD_GP_RegisterAccounts registerData;
	registerData.cbDeviceType = DF::shared()->GetDeviceType();
	registerData.wModuleID = 0;
	registerData.dwPlazaVersion = DF::shared()->GetPlazaVersion();
	registerData.setString(registerData.szMachineID, DF::shared()->GetMachineID());
	registerData.setString(registerData.szMobilePhone, DF::shared()->GetMobilePhone());

	registerData.setString(registerData.szLogonPass, DF::MD5Encrypt(strPsw.c_str()));			//登录密码
	registerData.setString(registerData.szInsurePass, DF::MD5Encrypt(strPsw.c_str()));		//银行密码

	registerData.wFaceID = 1;
	registerData.cbGender = 2;

	registerData.setString(registerData.szAccounts, strAcc.c_str());			//用户名
	registerData.setString(registerData.szNickName, strAcc.c_str());

	registerData.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;

	PACKET_AIDE_SIZE_NEW(1024);
	log("---------------PACKET_AIDE_SIZE_NEW[new = %d]---------------", 10);
	registerData.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_LOGON, HALL_SUB_GP_REGISTER_ACCOUNTS, packet);
}

void Player::requestLogin(const string& strAcc, const string& strPsw)
{
	m_strAccount = strAcc;
	m_strPassword = strPsw;

	CMD_GP_LogonAccounts logonData;
	logonData.cbDeviceType = DF::shared()->GetDeviceType();
	logonData.wModuleID = 0;
	logonData.dwPlazaVersion = DF::shared()->GetPlazaVersion();
	logonData.setString(logonData.szMachineID, DF::shared()->GetMachineID());
	logonData.setString(logonData.szMobilePhone, DF::shared()->GetMobilePhone());
	logonData.setString(logonData.szPassword, strPsw.c_str());			//登录密码
	logonData.setString(logonData.szAccounts, strAcc.c_str());			//用户名
	logonData.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
	logonData.wPlatform = 0;

	PACKET_AIDE_SIZE_NEW(1024);
	logonData.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_LOGON, HALL_DBR_GP_LOGON_ACCOUNTS, packet);
}

void Player::requsetServerList()
{
	PACKET_AIDE_SIZE_NEW(1);
	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVER_LIST, HALL_SUB_GP_SERVERINFO, packet);		//请求房间信息
}

void Player::requsetFlushServerInfo()
{
 	CMD_ST_NetFlushServerReq req;
 	req.userid = m_objLogonData.dwUserID;
 	req.roomid = HallGameScene::getCurRoomid();
 
 	PACKET_AIDE_SIZE_NEW(16);
 	req.toPack(packet);
 
 	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_NETFLUSHUSER_REQ, packet);
}

void Player::onFlushServerInfoResp(const PacketAide& packet, int nLen)
{
	CMD_ST_NetFlushUserRsp resp;
	resp.fromPack(packet);

	log("---------------onFlushServerInfoResp(gold = %d,ticket = %d)---------------", resp.money, resp.cpvalue);

	NetManager::getInstance()->onReconnectWithServer();
}

void Player::onLogonSuccess(const PacketAide& packet, int nLen)
{
	log("---------------onLogonSuccess---------------");

	m_objLogonData.fromPack(packet);

	m_strAccount = m_objLogonData.szAccounts;
	//_setSubGameGlobalData_SHZ(m_objLogonData);
}

void Player::onLogonHallInfo(const PacketAide& packet, int nLen)
{
	log("---------------onLogonHallInfo---------------");

	m_objHallData.fromPack(packet);
}

void Player::onGameType(const PacketAide& packet, int nLen)
{
	//先清除房间信息
	HallServerListData::shared()->clearServerInfo();
	log("---------------onGameType---------------");

	int itemSize = 2 + 2 + 2 + LEN_TYPE * 2;

	if (nLen % itemSize == 0)
	{
		//变量定义
		int iItemCount = nLen / itemSize;

		//获取对象
		ASSERT(HallServerListData::shared() != 0);
		HallServerListData * pServerListData = HallServerListData::shared();

		//更新数据
		for (int i = 0; i < iItemCount; i++)
		{
			tagHallGameType GameType;
			GameType.fromPack(packet);

			pServerListData->InsertGameType((tagGameType*)&GameType);
		}
	}
}

void Player::onGameKind(const PacketAide& packet, int nLen)
{
	log("---------------onGameKind---------------");

	int itemSize = 2 + 2 + 2 + 2 + 2 + 4 + 4 + LEN_KIND * 2 + LEN_PROCESS * 2;

	if (nLen % itemSize == 0)
	{
		//变量定义
		int iItemCount = nLen / itemSize;

		//获取对象
		ASSERT(HallServerListData::shared() != 0);
		HallServerListData * pServerListData = HallServerListData::shared();

		//更新数据
		for (int i = 0; i < iItemCount; i++)
		{
			tagHallGameKind GameKind;
			GameKind.fromPack(packet);

			pServerListData->InsertGameKind((tagGameKind*)&GameKind);
		}
	}
}

//void Player::onGamePage(const PacketAide& packet, int nLen)
//{
//	log("---------------onGamePage---------------");
//
//	m_objGamePage.fromPack(packet);
//}
//
//void Player::onGameNode(const PacketAide& packet, int nLen)
//{
//	log("---------------onGameNode---------------");
//
//	m_objGameNode.fromPack(packet);
//}

void Player::onGameServer(const PacketAide& packet, int nLen)
{
	//服务器传入的包里面是房间列表
	log("---------------onGameServer---------------");

	//m_objGameServer.fromPack(packet);

	int itemSize = 2 + 2 + 2 + 2 + 2 + 4 + 4 + LEN_SERVERADDR * 2 + LEN_SERVER * 2;
	//效验参数

	if (nLen%itemSize == 0)
	{
		//变量定义
		int iItemCount = nLen / itemSize;

		//获取对象
		ASSERT(HallServerListData::shared() != 0);
		HallServerListData * pServerListData = HallServerListData::shared();

		//u2string str;

		//更新数据
		for (int i = 0; i < iItemCount; i++)
		{
			tagHallGameServer GameServer;
			GameServer.fromPack(packet);

			pServerListData->InsertGameServer((tagSHZ_GameServer*)&GameServer);
		}
	}

}

void Player::onLogonFinish(const PacketAide& packet, int nLen)
{
	log("---------------onLogonFinish---------------");

	m_objLogonFinish.fromPack(packet);

	HallGameScene::getInstance()->openWindow(EWT_Hall);
	HallGameScene::getInstance()->updateWindow(EWT_Login);			//保存账号
	HallGameScene::getInstance()->closeWindow(EWT_LoginAccount);
	HallGameScene::getInstance()->closeWindow(EWT_Login);
}

void Player::onLogonFailure(const PacketAide& packet, int nLen)
{
	m_objLogonFailure.fromPack(packet);

	HallGameScene::getInstance()->openMessageTip(m_objLogonFailure.szDescribeString);
	log("---------------onLogonFailure[lResultCode = %d,desc = %s]---------------", m_objLogonFailure.lResultCode, m_objLogonFailure.szDescribeString);
}

void Player::requestModifyIcon(int nFaceId)
{
	CMD_GP_SystemFaceInfo req;

	req.wFaceID = nFaceId;
	req.dwUserID = m_objLogonData.dwUserID;
	string strPwd = Player::getInstance()->getPassword();
	req.setString(req.szPassword, (strPwd.size() < 32) ? DF::MD5Encrypt(strPwd.c_str()) : strPwd.c_str());
	req.setString(req.szMachineID, DF::shared()->GetMachineID());

	PACKET_AIDE_SIZE_NEW(256);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_SYSTEM_FACE_INFO, packet);
}

void Player::onModifyIconResp(const PacketAide& packet, int nLen)
{
	CMD_GP_UserFaceInfo resp;
	resp.fromPack(packet);

	m_objLogonData.wFaceID = resp.wFaceID;
	m_objLogonData.dwCustomID = resp.dwCustomID;

	HallGameScene::getInstance()->updateWindow(EWT_Hall, EHUC_ICON);
}

void Player::requestModifyNickname(const string& strName)
{
	CMD_ST_modiflyuser_req req;

	m_strModifyName = strName.substr(0, countarray(req.nickName));
	req.userId = m_objLogonData.dwUserID;
	req.setString(req.nickName, m_strModifyName.c_str());

	PACKET_AIDE_SIZE_NEW(128);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_MODIFLYUSER_REQ, packet);
}

void Player::onModifyNicknameResp(const PacketAide& packet, int nLen)
{
	CMD_ST_modiflyuser_rsp resp;
	resp.fromPack(packet);

	if (resp.ret == 0)		//修改成功
	{
		if (!m_strModifyName.empty())
		{
			memset(m_objLogonData.szNickName, 0, countarray(m_objLogonData.szNickName));
			m_objLogonData.setString(m_objLogonData.szNickName, m_strModifyName.c_str());

			//修改缓存中的信息
			string strNickName = StringUtils::format("%s_%d", LoginDataFieldNickName, m_objLogonData.dwUserID);
			GameUtil::setData(strNickName.c_str(), m_strModifyName.c_str());

			m_strModifyName.clear();
		}

		HALL_NOTIFICATION(Hall_Message_Refresh_NickName, nullptr);
	}
}

void Player::requestSaveCoinToBank(SCORE lScore)
{
	CMD_GP_UserSaveScore req;
	req.dwUserID = m_objLogonData.dwUserID;
	req.lSaveScore = lScore;
	req.setString(req.szMachineID, DF::shared()->GetMachineID());

	PACKET_AIDE_SIZE_NEW(128);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_USER_SAVE_SCORE, packet);
}

void Player::requestTakeCoinFromBank(SCORE lScore)
{
	CMD_GP_UserTakeScore req;
	req.dwUserID = m_objLogonData.dwUserID;
	req.lTakeScore = lScore;
	string strPwd = Player::getInstance()->getPassword();
	req.setString(req.szPassword, (strPwd.size() < 32) ? DF::MD5Encrypt(strPwd.c_str()) : strPwd.c_str());			//登录密码
	req.setString(req.szMachineID, DF::shared()->GetMachineID());

	PACKET_AIDE_SIZE_NEW(256);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_USER_TAKE_SCORE, packet);
}

void Player::onBankOperateSuccess(const PacketAide& packet, int nLen)
{
	CMD_GR_S_UserInsureSuccess resp;
	resp.fromPack(packet);

	m_objLogonData.lUserScore = resp.lUserScore;
	m_objLogonData.lUserInsure = resp.lUserInsure;

	//刷新界面
	HALL_NOTIFICATION(Hall_Message_Refresh_Coin, nullptr);
}

void Player::onBankOperateFailure(const PacketAide& packet, int nLen)
{
	CMD_GP_UserInsureFailure resp;
	resp.fromPack(packet);

	HallGameScene::getInstance()->openMessageTip(resp.szDescribeString);
}

void Player::requestSendTicket(dword gameid, dword tickets)
{
	CMD_ST_GiveChuanPiaoReq req;
	req.userid = m_objLogonData.dwUserID;
	req.gameid = gameid;
	req.nums = tickets;

	m_nSendTicketCounts = tickets;

	PACKET_AIDE_SIZE_NEW(32);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_GIVECHUANPIAO_REQ, packet);
}

void Player::onSendTicketResp(const PacketAide& packet, int nLen)
{
	CMD_ST_GiveChuanPiaoRsp resp;
	resp.fromPack(packet);

	switch (resp.ret)
	{
	case 0:			//成功
		this->addTicket(-m_nSendTicketCounts);
		m_nSendTicketCounts = 0;
		HallGameScene::getInstance()->openMessageTip(16);
		HALL_NOTIFICATION(Hall_Message_Refresh_Ticket, nullptr);
		break;
	case 1:		//玩家不存在
		HallGameScene::getInstance()->openMessageTip(20);
		break;
	case 2:		//船票不足
		HallGameScene::getInstance()->openMessageTip(17);
		break;
	case 3:		//赠送的对象是自己
		HallGameScene::getInstance()->openMessageTip(18);
		break;
	default:
		break;
	}
}

void Player::requestModifyAccount(const string& strAccount, const string& strPassword)
{
	m_strModifyName = strAccount;
	m_strModifyPassword = DF::MD5Encrypt(strPassword.c_str());

	CMD_ST_BindAccountReq req;
	req.userid = m_objLogonData.dwUserID;
	req.setString(req.szAccounts, strAccount.c_str());
	req.setString(req.szPassword, DF::MD5Encrypt(strPassword.c_str()));
	req.setString(req.szMachineID, DF::shared()->GetMachineID());

	PACKET_AIDE_SIZE_NEW(256);
	req.toPack(packet);

	NetManager::getInstance()->sendMsg(HALL_MDM_GP_SERVICE, HALL_SUB_GP_HALL_BINDACCOUNT_REQ, packet);
}

void Player::onModifyAccountResp(const PacketAide& packet, int nLen)
{
	CMD_ST_BindAccountRsp resp;
	resp.fromPack(packet);

	//账号密码修改成功 1 --游客, 2 - 不是这个机器, 3 - 账号重复

	if (resp.ret == 0)
	{
		if (!m_strModifyName.empty())
		{
			memset(m_objLogonData.szNickName, 0, countarray(m_objLogonData.szNickName));
			m_objLogonData.setString(m_objLogonData.szNickName, m_strModifyName.c_str());

			//修改缓存中的信息
			string strNickName = StringUtils::format("%s_%d", LoginDataFieldNickName, m_objLogonData.dwUserID);
			GameUtil::setData(strNickName.c_str(), m_strModifyName.c_str());

			m_objHallData.istrourist = 1;			//修改以后就是非游客了
			m_strModifyName.clear();
			HALL_NOTIFICATION(Hall_Message_Refresh_NickName, nullptr);
		}

		if (!m_strModifyPassword.empty())
		{
			m_strPassword = m_strModifyPassword;
			//修改缓存中的信息
			string strPassword = StringUtils::format("%s_%d", LoginDataFieldPassword, m_objLogonData.dwUserID);
			GameUtil::setData(strPassword.c_str(), m_strPassword.c_str());

			m_strModifyPassword.clear();
		}

		HallGameScene::getInstance()->closeWindow(EWT_Personal);
	}
	else if (resp.ret == 3)
	{
		HallGameScene::getInstance()->openMessageTip(42);
	}
	else
	{
		HallGameScene::getInstance()->openMessageTip(5);
	}
}

void Player::setCoin(int nCoinNum)
{
	if (nCoinNum > m_objLogonData.lUserScore)
	{
		GOLD_FALL_ANIMATION();
	}

	m_objLogonData.lUserScore = nCoinNum;
}

void Player::setGold(int nGoldNum)
{
	m_objHallData.llYuanBao = nGoldNum;
}

void Player::setComment(bool bComment)
{
	m_objHallData.iscomment = bComment ? 1 : 0;
}

void Player::setVipLevel(int VipLevel)
{
	m_objHallData.dwVipLevel = VipLevel;
}

void Player::setTotalRecharge(int nRechargeNum)
{
	m_objHallData.sumRecharge = nRechargeNum;
}

void Player::setShopItemRecharged(int nIndex)
{
	if (nIndex >= 0 && nIndex < 6)
	{
		m_objHallData.ritems[nIndex] = 1;
	}
}

bool Player::getShopItemRecharged(int nIndex)
{
	bool bRecharge = true;

	if (nIndex >= 0 && nIndex < 6)
	{
		bRecharge = m_objHallData.ritems[nIndex] != 0;
	}

	return bRecharge;
}

int Player::getNextVipNeedRecharge()
{
	int nNeedRecharge = 0;
	
	if (m_objHallData.dwVipLevel >= 0 && m_objHallData.dwVipLevel < countarray(nSumRechargeTable) - 1)
	{
		nNeedRecharge = nSumRechargeTable[m_objHallData.dwVipLevel + 1] - m_objHallData.sumRecharge;
	}

	return nNeedRecharge;
}

void Player::addCoin(int nCoinNum, bool bPlayEffect/* = true*/)
{
	m_objLogonData.lUserScore += nCoinNum;

	if (bPlayEffect && nCoinNum > 0)
	{
		GOLD_FALL_ANIMATION();
	}
}

void Player::addGold(int nGoldNum)
{
	m_objHallData.llYuanBao += nGoldNum;
}

void Player::addTicket(int nTicketNum)
{
	m_objHallData.dwTicket += nTicketNum;
}

float Player::getCurVipRechargePercent()
{
	float fPercent = 0.f;

	int i = 1;
	for (; i < countarray(nSumRechargeTable); i++)
	{
		if (m_objHallData.sumRecharge < nSumRechargeTable[i])
		{
			break;
		}
	}

	if (i == countarray(nSumRechargeTable) || m_objHallData.dwVipLevel == countarray(nSumRechargeTable) - 1)
	{
		fPercent = 100.0f;
	}
	else
	{
		fPercent = (m_objHallData.sumRecharge - nSumRechargeTable[i - 1]) * 100.f / (nSumRechargeTable[i] - nSumRechargeTable[i - 1]);
	}

	return fPercent;
}

int Player::getPlayerLevel()
{
	return MIN((int)pow(m_objLogonData.dwExperience / 15.f, 1.0f / 3) + 1, 99);
}

string Player::getEncryptPassword()
{
	string strRet = m_strPassword;
	if (!m_strPassword.empty() && m_strPassword.length() < 32)
	{
		strRet = DF::MD5Encrypt(m_strPassword.c_str());
	}

	return strRet;
}

bool Player::isVisitor()
{
	return m_objHallData.istrourist == 0;
}

void Player::pushDataToSHZ()
{
	_setSubGameGlobalData_SHZ(m_objLogonData);
}
void Player::_setSubGameGlobalData_SHZ(const CMD_GP_LogonSuccess& logonData)
{
 	SHZCGlobalUserInfo * pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
	tag_SHZGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
	tagSHZUserInsureInfo * pUserInsureInfo = pGlobalUserInfo->GetUserInsureInfo();

	//保存信息
	pGlobalUserData->wFaceID = logonData.wFaceID;
	pGlobalUserData->dwUserID = logonData.dwUserID;
	pGlobalUserData->dwGameID = logonData.dwGameID;
	pGlobalUserData->dwGroupID = logonData.dwGroupID;
	pGlobalUserData->dwCustomID = logonData.dwCustomID;
	pGlobalUserData->dwUserMedal = logonData.dwUserMedal;
	pGlobalUserData->dwExperience = logonData.dwExperience;
	pGlobalUserData->dwLoveLiness = logonData.dwLoveLiness;

	//用户成绩
	pUserInsureInfo->lUserScore = logonData.lUserScore;
	pUserInsureInfo->lUserInsure = logonData.lUserInsure;

	pGlobalUserData->cbGender = logonData.cbGender;
	pGlobalUserData->cbMoorMachine = logonData.cbMoorMachine;

	//帐号信息
	strncpy(pGlobalUserData->szAccounts, logonData.szAccounts, countarray(pGlobalUserData->szAccounts));
	strncpy(pGlobalUserData->szNickName, logonData.szNickName, countarray(pGlobalUserData->szNickName));
}

void  Player::onUpdateNotify(const PacketAide& packet, int nLen)
{
	m_objUpdateNotify.fromPack(packet);

	HallGameScene::getInstance()->openWindow(EWT_Update);
}