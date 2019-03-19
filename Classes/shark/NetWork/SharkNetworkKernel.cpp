#include "SharkNetworkKernel.h"
#include "shark/UI/GameScene/SharkGameScene.h"
#include "shark/Module/SharkNotice.h"
#include "LanguageConfig.h"
SharkNetWorkKernel* SharkNetWorkKernel::m_instance = nullptr;
SharkNetWorkKernel::SharkNetWorkKernel() : CSocketMission()
{
}

SharkNetWorkKernel::SharkNetWorkKernel(const std::string& url, int port) : CSocketMission(url, port)
{
	
}

SharkNetWorkKernel::~SharkNetWorkKernel()
{

}

void SharkNetWorkKernel::relese()
{
	if (nullptr == m_instance) return;

	stop();
	delete m_instance;
	m_instance = nullptr;

}

SharkNetWorkKernel* SharkNetWorkKernel::getInstance()
{
	if (m_instance == nullptr)
	{
		//m_instance = new NetworkKernel("120.26.6.159", 9501);
		m_instance = new SharkNetWorkKernel();
	}
	return m_instance;
}

//连接上服务器
void SharkNetWorkKernel::onEventTCPSocketLink()
{
}

//结束了服务
void SharkNetWorkKernel::onEventTCPSocketShut()
{
}

//连接出错
void SharkNetWorkKernel::onEventTCPSocketError(int errorCode)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//消息总汇
bool SharkNetWorkKernel::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	log("--------NetworkKernel:Main:%d,Sub:%d,dataSize:%d--------", main, sub, dataSize);
	switch (main)
	{
		//登陆命令1
	case MDM_GR_LOGON_SHARK: return onSubCmdLogon(sub, data, dataSize);
		//配置命令2
	case MDM_GR_CONFIG_SHARK: return onSubCmdConfig(sub, data, dataSize);
		//用户信息3
	case MDM_GR_USER_SHARK: return onSunCmdUserInfo(sub, data, dataSize);
		//游戏命令200
	case MDM_GF_GAME_SHARK: return onSubCmdGame(sub, data, dataSize);
		//状态命令4
	case MDM_GR_STATUS_SHARK: return onSubCmdState(sub, data, dataSize);
		//系统命令
	case MDM_CM_SYSTEM_SHARK: return onSubCmdSystem(sub, data, dataSize);
	default:
		break;
	}

	return true;
}


//---------------------------登陆相关----------------------------//

//发送登录
bool SharkNetWorkKernel::SendLogonPacket()
{

	auto pInfo = Player::getInstance();

	PACKET_AIDE_SIZE(512);
	packet.write4Byte(DF::shared()->GetPlazaVersion());
	packet.write4Byte(DF::shared()->GetFrameVersion());
	packet.write4Byte(DF::shared()->GetGameVersion());
	packet.write4Byte(pInfo->getLogonData().dwUserID);
	packet.writeUTF16(u8_2(""), LEN_PASSWORD);									//登陆游戏服务器 密码 传 空字符串
	packet.writeUTF16(u8_2(DF::shared()->GetMachineID()), LEN_MACHINE_ID);
	packet.write2Byte(DF::shared()->GetGameKindID());

	//发送数据
	SharkNetWorkKernel::getInstance()->send(MDM_GR_LOGON_SHARK, SUB_GR_LOGON_USERID_SHARK, packet.getBuffer(), packet.getPosition());

	return true;
}

//登陆命令网络读取
bool SharkNetWorkKernel::onSubCmdLogon(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//登录成功
	case SUB_GR_LOGON_SUCCESS_SHARK:	return onSocketSubLogonSuccess(data, dataSize);
		//登录失败
	case SUB_GR_LOGON_FAILURE_SHARK:	return onSocketSubLogonFailure(data, dataSize);
		//登录完成
	case SUB_GR_LOGON_FINISH_SHARK:	return onSocketSubLogonFinish(data, dataSize);
		//更新提示
	case SUB_GR_UPDATE_NOTIFY_SHARK:	return onSocketSubUpdateNotify(data, dataSize);
	}

	return true;
}

//登录成功
bool SharkNetWorkKernel::onSocketSubLogonSuccess(void* data, int dataSize)
{

	//PACKET_AIDE_DATA(data);

	//玩家属性
	//mUserAttribute.dwUserRight = packet.read4Byte();
	//mUserAttribute.dwMasterRight = packet.read4Byte();

	//if (SHZCUserRight::IsGameCheatUser(mUserAttribute.dwUserRight))
	//{

	//	PLAZZ_PRINTF("%s=%d", a_u8("用户权限"), mUserAttribute.dwUserRight);
	//}
	//else
	//{
	//	PLAZZ_PRINTF("%s=%d", a_u8("用户没有权限"), mUserAttribute.dwUserRight);
	//}

	//设置状态
	//SetServiceStatus(SHZ_ServiceStatus_RecvInfo);

	return true;

}

//登录失败
bool SharkNetWorkKernel::onSocketSubLogonFailure(void* data, int dataSize)
{
	//效验数据
	ASSERT(dataSize >= 4);
	if (dataSize < 4) return false;

	PACKET_AIDE_DATA(data);
	int lErrorCode = packet.read4Byte();
	int len = (dataSize - 4) / 2;
	u2string str;
	str.resize(len + 1, '\0');
	packet.readUTF16(&str[0], len);
	std::string sString = u2_8(&str[0]);

	log("%s:%s",a_u8("OnSocketSubLogonFailure 登录失败:"), sString.c_str());

	SharkNotice::openNotice(sString.c_str());
	//关闭连接
	SharkNetWorkKernel::getInstance()->stop();

	return true;
}

//登录完成
bool SharkNetWorkKernel::onSocketSubLogonFinish(void* data, int dataSize)
{
	//发送快速坐下
	SendSitDownPacket();

	//发送框架命令
	SendGameOption();

	//同步房间信息
	SendGameRoomInfo();

	//设置状态
	//SetServiceStatus(SHZ_ServiceStatus_ServiceIng);

	//mUserAttribute.dwUserID = mMeUserItem->GetUserID();
	//mUserAttribute.wChairID = INVALID_CHAIR;
	//mUserAttribute.wTableID = INVALID_TABLE;

	////规则判断
	//if (SHZCServerRule::IsForfendGameRule(mServerAttribute.dwServerRule) == false)
	//{
	//	//发送规则
	//	SendUserRulePacket();
	//}

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRLogonFinish();
	return true;
}

//版本更新提示
bool SharkNetWorkKernel::onSocketSubUpdateNotify(void* data, int dataSize)
{
	//PACKET_AIDE_DATA(data);
	////升级标志
	//byte cbMustUpdatePlaza = packet.readByte();
	//byte cbMustUpdateClient = packet.readByte();
	//byte cbAdviceUpdateClient = packet.readByte();

	////当前版本
	//dword dwCurrentPlazaVersion = packet.read4Byte();
	//dword dwCurrentFrameVersion = packet.read4Byte();
	//dword dwCurrentClientVersion = packet.read4Byte();

	////大厅更新
	//if (cbMustUpdatePlaza == TRUE)
	//{
	//	IntermitConnect(true);

	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGRUpdateNotify(1, a_u8("游戏广场已更新"));
	//	return true;
	//}

	////游戏更新
	//if (cbMustUpdateClient == TRUE)
	//{
	//	IntermitConnect(true);

	//	if (mIServerItemSink)
	//		mIServerItemSink->OnGRUpdateNotify(1, a_u8("游戏已更新"));
	//	return true;
	//}

	////构造提示
	//if (cbAdviceUpdateClient != TRUE)
	//	return true;

	////构造提示
	//char szDescribe[512] = { 0 };
	//sprintf(szDescribe, a_u8("“%s”已经更新了，我们强烈建议您进行更新，现在进行更新吗？"), mGameKind.szKindName);

	//IntermitConnect(true);

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRUpdateNotify(0, szDescribe);

	return true;

}


//---------------------------配置消息相关----------------------------//

//配置命令网络读取
bool SharkNetWorkKernel::onSubCmdConfig(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//列表配置
	case SUB_GR_CONFIG_COLUMN_SHARK:	return onSocketSubConfigColumu(data, dataSize);
		//房间配置
	case SUB_GR_CONFIG_SERVER_SHARK:	return onSocketSubConfigRoom(data, dataSize);
		//道具配置
	case SUB_GR_CONFIG_PROPERTY_SHARK: return onSocketSubConfigProperty(data, dataSize);
		//配置完成
	case SUB_GR_CONFIG_FINISH_SHARK:	return onSocketSubConfigFinish(data, dataSize);
		//玩家权限
	case SUB_GR_CONFIG_USER_RIGHT_SHARK:	return onSocketSubConfigUserRight(data, dataSize);
	default:
		break;
	}
	return true;
}

//列表配置
bool SharkNetWorkKernel::onSocketSubConfigColumu(void* data, int dataSize)
{
	//变量定义
	PACKET_AIDE_DATA(data);
	byte cbColumnCount = packet.readByte();
	return true;
}

//房间配置
bool SharkNetWorkKernel::onSocketSubConfigRoom(void* data, int dataSize)
{
	//效验数据
	ASSERT(dataSize == 10);
	if (dataSize < 10) return false;

	PACKET_AIDE_DATA(data);

	auto gameInfo = GameInfo::GetInstance();
	SharkTableInfo info = gameInfo->gettableInfor();

	//房间属性
	info.wTableCount = packet.read2Byte();
	info.wChairCount = packet.read2Byte();
	info.wServerType = packet.read2Byte();
	info.dwServerRule = packet.read4Byte();

	gameInfo->settableInfor(info);

	//m_UserListControl.SetServerRule(m_dwServerRule);

	/*if (!mTableViewFrame.ConfigTableFrame(
		mServerAttribute.wTableCount,
		mServerAttribute.wChairCount,
		mServerAttribute.dwServerRule,
		mServerAttribute.wServerType,
		mServerAttribute.wServerID))
		{
		IntermitConnect(false);
		return false;
		}*/

	return true;
}

//道具配置
bool SharkNetWorkKernel::onSocketSubConfigProperty(void* data, int dataSize)
{
	return true;
}

//配置完成
bool SharkNetWorkKernel::onSocketSubConfigFinish(void* data, int dataSize)
{
	return true;
}

//玩家权限
bool SharkNetWorkKernel::onSocketSubConfigUserRight(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	//玩家权限
	dword dwUserRight = packet.read4Byte();
	return true;
}


//---------------------------用户信息相关3----------------------------//


//发送快速坐下
bool SharkNetWorkKernel::SendSitDownPacket()
{
	return send(MDM_GR_USER_SHARK, SUB_GR_USER_CHAIR_REQ_SHARK);
}

//发送起立
bool SharkNetWorkKernel::SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave)
{
	PACKET_AIDE_SIZE(10);
	packet.write2Byte(wTableID);
	packet.write2Byte(wChairID);
	packet.writeByte(cbForceLeave);					//1 强制离开

	//发送数据
	send(MDM_GR_USER_SHARK, SUB_GR_USER_STANDUP_SHARK, packet.getBuffer(), packet.getPosition());

	return true;
}

//用户 信息网络读取
bool SharkNetWorkKernel::onSunCmdUserInfo(int sub, void* data, int dataSize)
{
	switch (sub)
	{
	case SUB_GR_USER_ENTER_SHARK: return onSocketSubUserEnter(data, dataSize);
	case SUB_GR_USER_STATUS_SHARK: return onSocketSubUserStatus(data, dataSize);
	case SUB_GR_USER_ERROR_CODE: return onSocketSubErrorCode(data, dataSize);
	default:
		break;
	}
	return true;
}

//用户状态
bool SharkNetWorkKernel::onSocketSubUserStatus(void* data, int dataSize)
{


	SharkUserInfo info = GameInfo::GetInstance()->getuserInfor();

	PACKET_AIDE_DATA(data);
	dword dwUserID = packet.read4Byte();
	if (info.dwUserID != dwUserID) return true;

	info.wTableID = packet.read2Byte();
	info.wChairID = packet.read2Byte();
	info.cbUserStatus = packet.readByte();

	GameInfo::GetInstance()->setuserInfor(info);

	return true;
}

//用户进入
bool SharkNetWorkKernel::onSocketSubUserEnter(void* data, int dataSize)
{
	//已经读取到自己的信息了，不再读取其他玩家进入的信息（网狐框架）

	log("---------GameInfo = %d----------", GameInfo::GetInstance()->getuserInfor().dwUserID);
	if (GameInfo::GetInstance()->getuserInfor().dwUserID != _ZERO) return true;
	//变量定义
	SharkUserInfo UserInfo;
	zeromemory(&UserInfo, sizeof(UserInfo));

	PACKET_AIDE_DATA(data);
	//用户属性
	UserInfo.dwGameID = packet.read4Byte();
	UserInfo.dwUserID = packet.read4Byte();
	UserInfo.dwGroupID = packet.read4Byte();
	UserInfo.wFaceID = packet.read2Byte();
	UserInfo.dwCustomID = packet.read4Byte();

	//用户属性
	UserInfo.cbGender = packet.readByte();
	UserInfo.cbMemberOrder = packet.readByte();
	UserInfo.cbMasterOrder = packet.readByte();

	//用户状态
	UserInfo.wTableID = packet.read2Byte();
	UserInfo.wChairID = packet.read2Byte();
	UserInfo.cbUserStatus = packet.readByte();

	//用户积分
	UserInfo.lScore = packet.read8Byte();
	packet.rSCORE();			//元宝信息
	packet.rSCORE();			//水浒经验
	UserInfo.lGrade = packet.read8Byte();
	UserInfo.lInsure = packet.read8Byte();

	UserInfo.dwWinCount = packet.read4Byte();
	UserInfo.dwLostCount = packet.read4Byte();
	UserInfo.dwDrawCount = packet.read4Byte();
	UserInfo.dwFleeCount = packet.read4Byte();
	UserInfo.dwUserMedal = packet.read4Byte();
	UserInfo.dwExperience = packet.read4Byte();
	UserInfo.lLoveLiness = packet.read4Byte();

	GameInfo::GetInstance()->setuserInfor(UserInfo);

	return true;
}

//错误信息
bool SharkNetWorkKernel::onSocketSubErrorCode(void* data, int dataSize)
{

	PACKET_AIDE_DATA(data);
	int lErrorCode = packet.read4Byte();

	int len = (dataSize - 4) / 2;
	u2string str;
	str.resize(len + 1, '\0');
	packet.readUTF16(&str[0], len);
	std::string sString = u2_8(&str[0]);

	log("--------------error code:,%d,error info:%s--------------", lErrorCode, sString.c_str());

	return true;
}

//---------------------------系统命令相关1000----------------------------//

//系统命令网络读取
bool SharkNetWorkKernel::onSubCmdSystem(int sub, void* data, int dataSize)
{
	switch (sub)
	{

	case SUB_CM_SYSTEM_MESSAGE_SHARK: return onSocketSubSystemMessage(data, dataSize);
	default:
		break;
	}

	return true;
}


bool SharkNetWorkKernel::onSocketSubSystemMessage(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	word wType = packet.read2Byte();
	word wLength = packet.read2Byte();

	int len = (dataSize - 4) / 2;
	u2string str;
	str.resize(len + 1, '\0');
	packet.readUTF16(&str[0], len);

	const std::string sString = u2_8(&str[0]);

	log("--------------%s:%s--------------",a_u8("NetworkKernel::NetworkKernel\n"), sString.c_str());

	//关闭游戏
	if (wType & (SMT_CLOSE_ROOM_SHARK | SMT_CLOSE_LINK_SHARK | SMT_CLOSE_GAME_SHARK))
	{
		log("%s",a_u8("----------------关闭游戏----------------"));

		SharkNotice::openNotice(GETSTRING(1400), [](Ref*)
		{
			SharkGameScene::m_mainView->menuManager.goBack(1);
		});
	}

	return true;
}


//---------------------------状态命令相关----------------------------//

//发送进入场景
bool SharkNetWorkKernel::SendGameOption()
{

	//变量定义
	PACKET_AIDE_SIZE(512);
	packet.writeByte(FALSE);									//是否允许旁观 (FALSE为不允许)
	packet.write4Byte(DF::shared()->GetFrameVersion());
	packet.write4Byte(DF::shared()->GetGameVersion());

	//发送数据
	return send(MDM_GF_FRAME_SHARK, SUB_GF_GAME_OPTION_SHARK, packet.getBuffer(), packet.getPosition());
}

//状态命令网络读取
bool SharkNetWorkKernel::onSubCmdState(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//桌子信息
	case SUB_GR_TABLE_INFO_SHARK: return onSocketSubStatusTableInfo(data, dataSize);
		//桌子状态
	case SUB_GR_TABLE_STATUS_SHARK: return onSocketSubStatusTableStatus(data, dataSize);
	default:
		break;
	}

	return true;
}

//桌子信息（会发送所有玩家的信息过来）
bool SharkNetWorkKernel::onSocketSubStatusTableInfo(void* data, int dataSize)
{

	//变量定义
	PACKET_AIDE_DATA(data);
	word wTableCount = packet.read2Byte();

	//for (int i = 0; i < wTableCount; i++)
	//{
	//	byte cbTableLock = packet.readByte();
	//	byte cbPlayStatus = packet.readByte();
	//	//mTableViewFrame.SetTableStatus(i, (cbPlayStatus == TRUE), (cbTableLock == TRUE));
	//}

	return true;
}

//桌子状态
bool SharkNetWorkKernel::onSocketSubStatusTableStatus(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	SharkTableInfo info;
	info.wTableID = packet.read2Byte();
	info.bTableLock = packet.readByte();
	info.bPlayStatus = packet.readByte();
	GameInfo::GetInstance()->settableInfor(info);

	return true;

}

//---------------------------游戏逻辑相关----------------------------//

//发送心跳包
bool SharkNetWorkKernel::SendHeartPak()
{
	send(MDM_GF_GAME_SHARK, SUB_G_GAMETICKTIME);//定义
	return true;
}

//接收心跳包
bool SharkNetWorkKernel::onSocketSubHeartPak(void* data, int dataSize)
{
	if (SharkGameScene::This != nullptr)
	{
		SharkGameScene::This->OnHreatPak();
	}
	return true;
}
//请求房间信息
bool SharkNetWorkKernel::SendGameRoomInfo()
{
	send(MDM_GF_GAME_SHARK, SUB_G_INTOGAMEINGREQ);
	return true;
}

//发送自己的压分信息
bool SharkNetWorkKernel::SendJetInfo(word index, SCORE jet)
{
	//校验参数
	//CC_ASSERT(index >= _ZERO && index < 12);
	//if (index < _ZERO || index >= 12) return false;

	
	CMD_GP_PUTNOTESREQU jetInfo;
	jetInfo.AmimalIndex = index;
	jetInfo.AmimalGold = jet;



	send(MDM_GF_GAME_SHARK, SUB_G_OPERATIONNOTES, &jetInfo, sizeof(CMD_GP_PUTNOTESREQU));
	return true;
}

//请求所有玩家总压分数
bool SharkNetWorkKernel::SendAllJetInfo()
{
	return true;
}

//游戏命令网络读取
bool SharkNetWorkKernel::onSubCmdGame(int sub, void* data, int dataSize)
{

	switch (sub)
	{
	case SUB_G_INTOGAMEINGRES: return onSocketSubTableInfo(data, dataSize);
	case SUB_G_GAMESENLINSTART: return onSocketSubOpenRlt(data, dataSize);
	case SUB_G_GAMESNEXTSTART: return  onSocketSubStartGame(data, dataSize);
	case SUB_G_GAMECAIJINGRES: return onSocketSubOpenCJRlt(data, dataSize);
	case SUB_G_GAMEUSERCHANGE: return onSocketSubJoinOrExit(data, dataSize);
	case SUB_G_GAMESENLINSTARTBORD: return onSocketSubOtherUserJet(data, dataSize);
	case SUB_G_GAMESTARTGAMELIST: return onSocketSubUpdateUserList(data, dataSize);
	case SUB_G_GAMECHEARNOTEALL: return onSocketSubOtherCancelJet(data, dataSize);
	case SUB_G_GAMETICKTIMERSP: return onSocketSubHeartPak(data, dataSize);
	default:
		break;
	}
	return true;
}

//返回所有玩家压分信息
bool SharkNetWorkKernel::onSocketSubAllJetInfoRlt(void* data, int dataSize)
{
	return true;
}

//SHARK游戏进入数据同步
bool SharkNetWorkKernel::onSocketSubTableInfo(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);
	//设置当前房间信息

	
	auto gameInfo = GameInfo::GetInstance();
	RoomInfo_Shark* roominfo = gameInfo->getroomInfor();

	//服务端返回12条记录
	int res[12];
	for (size_t i = 0; i <12; i++)
	{
		int animalNumber = packet.read4Byte();
		int animalIndex = packet.read4Byte();
		res[i] = animalNumber;
		
	}
	gameInfo->setResRecord(res);
	roominfo->JsRate = packet.read4Byte();              //金鲨倍率
	gameInfo->setserverState((gameState)packet.read4Byte());		//游戏状态

	log("-------------%s:%d-----------", a_u8("同步状态:"), gameInfo->getserverState());

	//设置第一局游戏信息
	gameInfo->setLeftTime(packet.read4Byte());	

	
	roominfo->dwRateID = packet.read4Byte();          //倍率索引

	//读取彩金
	roominfo->lCJ = packet.read8Byte();
	log("-------CJ:%d--------------", roominfo->lCJ);

	//读取在线玩家人数
	roominfo->dwOnline = packet.read4Byte();
	log("-------online:%d--------------", roominfo->dwOnline);

	//读取在线玩家信息
	for (size_t i = 0; i < roominfo->dwOnline; i++)
	{
		UserInfo_Shark info;
		info.userid = packet.read4Byte();


		u2string str;
		str.resize(LEN_NICKNAME + 1, '\0');
		packet.readUTF16(&str[0], LEN_NICKNAME);
		memcpy(info.nickname, u2_8(&str[0]), LEN_NICKNAME);

		info.viplevel = packet.read4Byte();
		info.gold = packet.read4Byte();
		info.headimgid = packet.read4Byte();
		info.winscore = packet.read8Byte();
		//info.jet = packet.read8Byte();
		//info.jet = _ZERO;
		roominfo->vec.push_back(info);

		log("-------ID:%d ,name:%s, gold:%d ,vip:%d , head:%d, win:%ld---------------", info.userid, info.nickname, info.gold, info.viplevel, info.headimgid, info.winscore);

		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{
			int coinScore = gameInfo->getchangeScore();
			//int yuanBaoScore = gameinfo->getchangeYuanBao();
			coinScore = info.gold;
			gameInfo->setchangeScore(coinScore);
		}
	}	


	//游戏准备完毕
	GameInfo::GetInstance()->setstartGameFinish(true);
	return true;
	
}

//其他玩家压分
bool SharkNetWorkKernel::onSocketSubOtherUserJet(void* data, int dataSize)
{
	CC_ASSERT(sizeof(CMD_GP_PUTNOTESREC) == dataSize);
	if (sizeof(CMD_GP_PUTNOTESREC) != dataSize) return false;

	CMD_GP_PUTNOTESREC* p = (CMD_GP_PUTNOTESREC*)data;
	if (SharkGameScene::m_mainView != nullptr)
	{
		log("-----------------%s----------------", a_u8("返回其他玩家信息"));
		log("-------------totalCoin = %d------------------",p->gold);
		log("-------------playerCoin = %ld------------------", p->currgold);
		log("-------------playerID = %d-----------",p->userid);
		auto gameInfo = GameInfo::GetInstance();
		RoomInfo_Shark* roominfo = gameInfo->getroomInfor();
		int index = p->AnimalIndex;
		if (index == 1000)
		{
			index = 0;
		}
		else if (index == 2000)
		{
			index = 3;
		}
		roominfo->alljetinfo.lTotelBet[index] += p->gold;
		roominfo->alljetinfo.lAllBet += p->gold;

		if (Player::getInstance()->getLogonData().dwUserID == p->userid)
		{
			log("----------index = %d---------------", index);
			SharkGameScene::m_mainView->stakeManager.successStake(p->gold, p->currgold, index);
		}

		SharkGameScene::m_mainView->m_playerListManager.otherJet(p->userid, p->gold, p->currgold, index);
	}
	return true;

}

//开奖刷新玩家列表
bool SharkNetWorkKernel::onSocketSubUpdateUserList(void* data, int datasize)
{
 	if (SharkGameScene::m_mainView == nullptr) return true;
	PACKET_AIDE_DATA(data);
	dword online = packet.read4Byte();

	auto gameinfo = GameInfo::GetInstance();
	auto roomInfo = gameinfo->getroomInfor();
	vector<UserInfo_Shark> vec;
	for (size_t i = 0; i < online; i++)
	{
		UserInfo_Shark info;
		info.userid = packet.read4Byte();

		u2string str;
		str.resize(LEN_NICKNAME + 1, '\0');
		packet.readUTF16(&str[0], LEN_NICKNAME);
		memcpy(info.nickname, u2_8(&str[0]), LEN_NICKNAME);

		info.viplevel = packet.read4Byte();
		info.gold = packet.read4Byte();
		info.headimgid = packet.read4Byte();
		info.winscore = packet.read8Byte();
		info.jet = _ZERO;
		vec.push_back(info);
		
		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{
			int coinScore = gameinfo->getchangeScore();

			coinScore = info.gold;
			gameinfo->setchangeScore(coinScore);
		}
	}

	roomInfo->vec = vec;
	return true;

}


//玩家加入和离开
bool SharkNetWorkKernel::onSocketSubJoinOrExit(void* data, int dataSize)
{

	PACKET_AIDE_DATA(data);

	auto gameInfo = GameInfo::GetInstance();

	UserInfo_Shark info;
	info.userid = packet.read4Byte();

	u2string str;
	str.resize(LEN_NICKNAME + 1, '\0');
	packet.readUTF16(&str[0], LEN_NICKNAME);
	memcpy(info.nickname, u2_8(&str[0]), LEN_NICKNAME);

	info.viplevel = packet.read4Byte();
	info.gold = packet.read4Byte();
	info.headimgid = packet.read4Byte();
	info.winscore = packet.read8Byte();

	int state = packet.read4Byte();

	RoomInfo_Shark *roomInfo = gameInfo->getroomInfor();
	if (state == 0)
	{
		//玩家离开
		roomInfo->dwOnline--;

		auto roominfo = gameInfo->getroomInfor();
		
		if (SharkGameScene::m_mainView != nullptr)
		{
			SharkGameScene::m_mainView->m_playerListManager.deleteUserElement(info.userid);
		}
		for (vector<UserInfo_Shark>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
		{
			if ((*itor).userid == info.userid)
			{
				roominfo->vec.erase(itor);
				break;
			}
		}

		
	}
	else if (state == 1)
	{
		//玩家进入
		if (info.userid != GameInfo::GetInstance()->getuserInfor().dwUserID)
		{
			//第一次进入游戏会触发
			roomInfo->dwOnline++;
		}
		if (SharkGameScene::m_mainView != nullptr)
		{
			gameInfo->getroomInfor()->vec.push_back(info);
			//if (GameInfo::GetInstance()->getserverState() == STAKE)
				SharkGameScene::m_mainView->m_playerListManager.InitPlayerList();
			//else
				//SharkGameScene::m_mainView->m_playerListManager.addPlayerElement(info);
		}

	}

	return true;

}

//返回开奖结果
bool SharkNetWorkKernel::onSocketSubOpenRlt(void* data, int dataSize)
{

	CC_ASSERT(dataSize == sizeof(CMD_GP_StartGameRandRsp));
	if (dataSize != sizeof(CMD_GP_StartGameRandRsp)) return false;

	CMD_GP_StartGameRandRsp* gameRlt = (CMD_GP_StartGameRandRsp*)data;
	auto gameInfo = GameInfo::GetInstance();

	OpenResult result;
	for (int i = 0; i < 3; i++)
	{
		result.resultIndex[i] = gameRlt->AnimalIndex[i];
	}
	result.aType = gameRlt->Atype;
	result.aValue = gameRlt->Avalue;

	gameInfo->setopenRelt(result);	
	
	gameInfo->setserverState(gameState::BATTLE);
	return true;
}

//开始新一轮游戏
//开始游戏(压分)
bool SharkNetWorkKernel::onSocketSubStartGame(void* data, int dataSize)
{

	CC_ASSERT(dataSize == sizeof(CMD_GP_StartNextGame));
	if (dataSize != sizeof(CMD_GP_StartNextGame)) return false;
	CMD_GP_StartNextGame* info = (CMD_GP_StartNextGame*)data;	


	auto gameInfo = GameInfo::GetInstance();
	RoomInfo_Shark* roominfo = gameInfo->getroomInfor();
	roominfo->JsRate = info->JsRate;
	roominfo->dwRateID = info->RateID;
	roominfo->lCJ = info->CaiJinValue;
	//设置状态为开始游戏
	gameInfo->setserverState(gameState::STAKE);

	
	//把上一局的押分总信息归零
	roominfo->alljetinfo.lAllBet = 0;
	for (int i = 0; i < 12; i++)
	{
		roominfo->alljetinfo.lTotelBet[i] = 0;
	}



	return true;
}

//有玩家取消了压分
bool SharkNetWorkKernel::onSocketSubOtherCancelJet(void* data, int dataSize)
{
	CC_ASSERT(dataSize == sizeof(CMD_GP_CharNotesInfor));
	if (dataSize != sizeof(CMD_GP_CharNotesInfor)) return false;

	CMD_GP_CharNotesInfor* info = (CMD_GP_CharNotesInfor*)data;
	//更新总总压分
	if (SharkGameScene::m_mainView != nullptr)
	{
		auto gameInfo = GameInfo::GetInstance();
		RoomInfo_Shark* roominfo = gameInfo->getroomInfor();
		SCORE sum = _ZERO;
		for (size_t i = _ZERO; i < info->size; i++)
		{
			int index = info->notes[i].noteid;
			if (index == 1000)
			{
				index = 0;
			}
			else if(index == 2000)
			{
				index = 3;
			}
			roominfo->alljetinfo.lTotelBet[index] -= info->notes[i].gold;
			sum += info->notes[i].gold;
			log("-------------------cancel jet :%d----------------------", info->notes[i].noteid);
		}

		roominfo->alljetinfo.lAllBet -= sum;

		//自己取消压分成功 通知刷新UI
		if (info->userid == Player::getInstance()->getLogonData().dwUserID)
		{
			SharkGameScene::m_mainView->stakeManager.successCancelStake();
		}

		SharkGameScene::m_mainView->m_playerListManager.othercancelJet(info->userid, sum);
	}
	return true;

}

//CJ彩金开奖消息
bool SharkNetWorkKernel::onSocketSubOpenCJRlt(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	auto gameInfo = GameInfo::GetInstance();
	SharkCJ sharkCaiJin = gameInfo->getsharkCj();

	//有几个玩家中奖
	sharkCaiJin.num = packet.read4Byte();
	//读取3个玩家信息
	for (int i = 0; i < sharkCaiJin.num; i++)
	{
		sharkCaiJin.userid[i] = packet.read4Byte();

		u2string str;
		str.resize(LEN_NICKNAME + 1, '\0');
		packet.readUTF16(&str[0], LEN_NICKNAME);
		memcpy(sharkCaiJin.name[i], u2_8(&str[0]), LEN_NICKNAME);

		int	viplevel = packet.read4Byte();
		sharkCaiJin.score[i] = packet.read4Byte();
		int	headimgid = packet.read4Byte();
		SCORE winscore = packet.read8Byte();

		log("--------CJ:---name:%s , gold:%d--------", sharkCaiJin.name[i], sharkCaiJin.score[i]);
	}

	 gameInfo->setsharkCj(sharkCaiJin);

		//设置状态为开奖
	gameInfo->setserverState(gameState::BATTLE);


	return true;
}

