#include "CommandStructDef.h"
#include "libs/PacketAide.h"

void CMD_GP_OperateSuccess::fromPack(const PacketAide& packData)
{
	lResultCode = packData.rlong();
	packData.rchars(szDescribeString, 128);
}

void CMD_GP_OperateFailure::fromPack(const PacketAide& packData)
{
	lResultCode = packData.rlong();
	packData.rchars(szDescribeString, 128);
}

void tagHallGPVisitorLogon::toPack(PacketAide& packData)
{
	packData.wbyte(cbDeviceType);
	packData.wword(wModuleID);
	packData.wdword(dwPlazaVersion);
	packData.wchars(szMachineID, countarray(szMachineID));
	packData.wbyte(cbValidateFlags);
	packData.wchars(szPassword, countarray(szPassword));
}

void tagHallGPVisitorLogon::fromPack(const PacketAide& packData)
{
	cbDeviceType = packData.rbyte();
	wModuleID = packData.rword();
	dwPlazaVersion = packData.rword();
	packData.rchars(szMachineID, countarray(szMachineID));
	cbValidateFlags = packData.rbyte();
	packData.rchars(szPassword, countarray(szPassword));
}

void CMD_GP_RegisterAccounts::toPack(PacketAide& packData)
{
	packData.wbyte(cbDeviceType);
	packData.wword(wModuleID);
	packData.wdword(dwPlazaVersion);
	packData.wchars(szMachineID, LEN_MACHINE_ID);
	packData.wchars(szMobilePhone, LEN_MOBILE_PHONE);
	packData.wchars(szLogonPass, LEN_MD5);
	packData.wchars(szInsurePass, LEN_MD5);

	packData.wword(wFaceID);
	packData.wbyte(cbGender);

	packData.wchars(szAccounts, LEN_ACCOUNTS);
	packData.wchars(szNickName, LEN_NICKNAME);

	packData.wchars(szSpreader, LEN_ACCOUNTS);
	packData.wchars(szPassPortID, LEN_PASS_PORT_ID);
	packData.wchars(szCompellation, LEN_COMPELLATION);

	packData.wbyte(cbValidateFlags);
}

void CMD_GP_RegisterAccounts::fromPack(const PacketAide& packData)
{
	cbDeviceType = packData.rbyte();
	wModuleID = packData.rword();
	dwPlazaVersion = packData.rdword();
	packData.rchars(szMachineID, LEN_MACHINE_ID);
	packData.rchars(szMobilePhone, LEN_MOBILE_PHONE);

	packData.rchars(szLogonPass, LEN_MD5);
	packData.rchars(szInsurePass, LEN_MD5);

	wFaceID = packData.rword();
	cbGender = packData.rbyte();

	packData.rchars(szAccounts, LEN_ACCOUNTS);
	packData.rchars(szNickName, LEN_NICKNAME);

	packData.rchars(szSpreader, LEN_ACCOUNTS);
	packData.rchars(szPassPortID, LEN_PASS_PORT_ID);
	packData.rchars(szCompellation, LEN_COMPELLATION);

	cbValidateFlags = packData.rbyte();
}

void CMD_GP_LogonSuccess::toPack(PacketAide& packData)
{
	packData.wword(wFaceID);
	packData.wdword(dwUserID);
	packData.wdword(dwGameID);
	packData.wdword(dwGroupID);
	packData.wdword(dwCustomID);
	packData.wdword(dwUserMedal);
	packData.wdword(dwExperience);
	packData.wdword(dwLoveLiness);

	packData.wSCORE(lUserScore);
	packData.wSCORE(lUserInsure);

	packData.wbyte(cbGender);
	packData.wbyte(cbMoorMachine);

	packData.wchars(szAccounts, LEN_ACCOUNTS);
	packData.wchars(szNickName, LEN_ACCOUNTS);
	packData.wchars(szGroupName, LEN_GROUP_NAME);

	packData.wbyte(cbShowServerStatus);
}

void CMD_GP_LogonSuccess::fromPack(const PacketAide& packData)
{
	wFaceID = packData.rword();
	dwUserID = packData.rdword();
	dwGameID = packData.rdword();
	dwGroupID = packData.rdword();
	dwCustomID = packData.rdword();
	dwUserMedal = packData.rdword();
	dwExperience = packData.rdword();
	dwLoveLiness = packData.rdword();

	lUserScore = packData.rSCORE();
	lUserInsure = packData.rSCORE();

	cbGender = packData.rbyte();
	cbMoorMachine = packData.rbyte();

	packData.rchars(szAccounts, LEN_ACCOUNTS);
	packData.rchars(szNickName, LEN_ACCOUNTS);
	packData.rchars(szGroupName, LEN_GROUP_NAME);

	cbShowServerStatus = packData.rbyte();
}

void CMD_GP_LogonSuccess_Hall::fromPack(const PacketAide& packData)
{
	userid = packData.rdword();
	llYuanBao = packData.rSCORE();
	dwVipLevel = packData.rdword();
	dwExpLevel = packData.rdword();
	
	dwTicket = packData.rdword();
	fudaitime = packData.rdword();

	for (int i = 0; i < 6; i++)
	{
		ritems[i] = packData.rdword();
	}

	sumRecharge = packData.rdword();
	istrourist = packData.rdword();
	iscomment = packData.rdword();
}

void CMD_GP_LogonFailure::toPack(PacketAide& packData)
{
	packData.wlong(lResultCode);
	packData.wchars(szDescribeString, 128);
}

void CMD_GP_LogonFailure::fromPack(const PacketAide& packData)
{
	lResultCode = packData.rlong();
	packData.rchars(szDescribeString, 128);
}

void CMD_GP_LogonFinish::toPack(PacketAide& packData)
{
	packData.wword(wIntermitTime);
	packData.wword(wOnLineCountTime);
}

void CMD_GP_LogonFinish::fromPack(const PacketAide& packData)
{
	wIntermitTime = packData.rword();
	wOnLineCountTime = packData.rword();
}

void tagHallGameType::toPack(PacketAide& packData)
{
	packData.wword(wJoinID);
	packData.wword(wSortID);
	packData.wword(wTypeID);
	packData.wchars(szTypeName, LEN_TYPE);
}

void tagHallGameType::fromPack(const PacketAide& packData)
{
	wJoinID = packData.rword();
	wSortID = packData.rword();
	wTypeID = packData.rword();
	packData.rchars(szTypeName, LEN_TYPE);
}

void tagHallGameKind::toPack(PacketAide& packData)
{
	packData.wword(wTypeID);
	packData.wword(wJoinID);
	packData.wword(wSortID);
	packData.wword(wKindID);
	packData.wword(wGameID);
	packData.wdword(dwOnLineCount);
	packData.wdword(dwFullCount);

	packData.wchars(szKindName, LEN_KIND);
	packData.wchars(szProcessName, LEN_PROCESS);
}

void tagHallGameKind::fromPack(const PacketAide& packData)
{
	wTypeID = packData.rword();
	wJoinID = packData.rword();
	wSortID = packData.rword();
	wKindID = packData.rword();
	wGameID = packData.rword();
	dwOnLineCount = packData.rdword();
	dwFullCount = packData.rdword();

	packData.rchars(szKindName, LEN_KIND);
	packData.rchars(szProcessName, LEN_PROCESS);
}

void tagHallGameNode::toPack(PacketAide& packData)
{
	packData.wword(wKindID);
	packData.wword(wJoinID);
	packData.wword(wSortID);
	packData.wword(wNodeID);

	packData.wchars(szNodeName, LEN_NODE);
}

void tagHallGameNode::fromPack(const PacketAide& packData)
{
	wKindID = packData.rword();
	wJoinID = packData.rword();
	wSortID = packData.rword();
	wNodeID = packData.rword();

	packData.rchars(szNodeName, LEN_NODE);
}

void tagHallGamePage::toPack(PacketAide& packData)
{
	packData.wword(wPageID);
	packData.wword(wKindID);
	packData.wword(wNodeID);
	packData.wword(wSortID);
	packData.wword(wOperateType);

	packData.wchars(szDisplayName, LEN_PAGE);
}

void tagHallGamePage::fromPack(const PacketAide& packData)
{
	wPageID = packData.rword();
	wKindID = packData.rword();
	wNodeID = packData.rword();
	wSortID = packData.rword();
	wOperateType = packData.rword();

	packData.rchars(szDisplayName, LEN_PAGE);
}

void tagHallGameServer::toPack(PacketAide& packData)
{
	packData.wword(wKindID);
	packData.wword(wNodeID);
	packData.wword(wSortID);
	packData.wword(wServerID);
	packData.wword(wServerPort);

	packData.wdword(dwOnLineCount);
	packData.wdword(dwFullCount);

	packData.wchars(szServerAddr, LEN_SERVERADDR);
	packData.wchars(szServerName, LEN_SERVER);
}

void tagHallGameServer::fromPack(const PacketAide& packData)
{
	wKindID = packData.rword();
	wNodeID = packData.rword();
	wSortID = packData.rword();
	wServerID = packData.rword();
	wServerPort = packData.rword();

	dwOnLineCount = packData.rdword();
	dwFullCount = packData.rdword();

	packData.rchars(szServerAddr, LEN_SERVERADDR);
	packData.rchars(szServerName, LEN_SERVER);
}

void CMD_GP_LogonGameID::toPack(PacketAide& packData)
{
	packData.wbyte(cbDeviceType);
	packData.wword(wModuleID);
	packData.wdword(dwPlazaVersion);

	packData.wchars(szMachineID, LEN_MACHINE_ID);
	packData.wchars(szMobilePhone, LEN_MOBILE_PHONE);


	packData.wdword(dwGameID);
	packData.wchars(szPassword, LEN_MD5);
	packData.wbyte(cbValidateFlags);
}

void CMD_GP_LogonGameID::fromPack(const PacketAide& packData)
{
	cbDeviceType = packData.rbyte();
	wModuleID = packData.rword();
	dwPlazaVersion = packData.rdword();

	packData.rchars(szMachineID, LEN_MACHINE_ID);
	packData.rchars(szMobilePhone, LEN_MOBILE_PHONE);


	dwGameID = packData.rdword();
	packData.rchars(szPassword, LEN_MD5);
	cbValidateFlags = packData.rbyte();
}


void CMD_GP_LogonAccounts::toPack(PacketAide& packData)
{
	packData.wbyte(cbDeviceType);
	packData.wword(wModuleID);
	packData.wdword(dwPlazaVersion);
	packData.wchars(szMachineID, LEN_MACHINE_ID);
	packData.wchars(szMobilePhone, LEN_MOBILE_PHONE);

	packData.wchars(szPassword, LEN_MD5);
	packData.wchars(szAccounts, LEN_ACCOUNTS);

	packData.wbyte(cbValidateFlags);
	packData.wword(wPlatform);
}

void CMD_GP_LogonAccounts::fromPack(const PacketAide& packData)
{
	cbDeviceType = packData.rbyte();
	wModuleID = packData.rword();
	dwPlazaVersion = packData.rdword();
	packData.rchars(szMachineID, LEN_MACHINE_ID);
	packData.rchars(szMobilePhone, LEN_MOBILE_PHONE);

	packData.rchars(szPassword, LEN_MD5);
	packData.rchars(szAccounts, LEN_ACCOUNTS);

	cbValidateFlags = packData.rbyte();
	wPlatform = packData.rword();
}

void CMD_GP_YaoQianShu_USERINFO::toPack(PacketAide& packData)
{
	packData.wword(nType);
	packData.wdword(dwGameID);
}

void CMD_GP_YaoQianShu_USERINFO::fromPack(const PacketAide& packData)
{
	nType = packData.rword();
	dwGameID = packData.rdword();
}

void CMD_GP_YaoQianShu_Get::toPack(PacketAide& packData)
{
	packData.wword(nType);
	packData.wdword(nTime);
	packData.wdword(GetGold);
	packData.wdword(TotelGold);
}

void CMD_GP_YaoQianShu_Get::fromPack(const PacketAide& packData)
{
	nType = packData.rword();
	nTime = packData.rdword();
	GetGold = packData.rdword();
	TotelGold = packData.rdword();
}


void CMD_GP_SystemFaceInfo::toPack(PacketAide& packData)
{
	packData.wword(wFaceID);
	packData.wdword(dwUserID);
	packData.wchars(szPassword, LEN_PASSWORD);
	packData.wchars(szMachineID, LEN_MACHINE_ID);
}

void CMD_GP_SystemFaceInfo::fromPack(const PacketAide& packData)
{
	wFaceID = packData.rword();
	dwUserID = packData.rdword();
	packData.rchars(szPassword, LEN_PASSWORD);
	packData.rchars(szMachineID, LEN_MACHINE_ID);
}

void CMD_GP_UserFaceInfo::toPack(PacketAide& packData)
{
	packData.wword(wFaceID);
	packData.wdword(dwCustomID);
}

void CMD_GP_UserFaceInfo::fromPack(const PacketAide& packData)
{
	wFaceID = packData.rword();
	dwCustomID = packData.rdword();
}

void CMD_ST_modiflyuser_req::toPack(PacketAide& packData)
{
	packData.wdword(userId);
	packData.wchars(nickName, countarray(nickName));
}

void CMD_ST_modiflyuser_req::fromPack(const PacketAide& packData)
{
	userId = packData.rdword();
	packData.rchars(nickName, countarray(nickName));
}

void CMD_ST_modiflyuser_rsp::toPack(PacketAide& packData)
{
	packData.wdword(ret);
}

void CMD_ST_modiflyuser_rsp::fromPack(const PacketAide& packData)
{
	ret = packData.rdword();
}

void CMD_GP_UserSaveScore::toPack(PacketAide& packData)
{
	packData.wdword(dwUserID);
	packData.wSCORE(lSaveScore);
	packData.wchars(szMachineID, countarray(szMachineID));
}

void CMD_GP_UserSaveScore::fromPack(const PacketAide& packData)
{
	dwUserID = packData.rdword();
	lSaveScore = packData.rSCORE();
	packData.rchars(szMachineID, countarray(szMachineID));
}

void CMD_GP_UserTakeScore::toPack(PacketAide& packData)
{
	packData.wdword(dwUserID);
	packData.wSCORE(lTakeScore);
	packData.wchars(szPassword, countarray(szPassword));
	packData.wchars(szMachineID, countarray(szMachineID));
}

void CMD_GP_UserTakeScore::fromPack(const PacketAide& packData)
{
	dwUserID = packData.rdword();
	lTakeScore = packData.rSCORE();
	packData.rchars(szPassword, countarray(szPassword));
	packData.rchars(szMachineID, countarray(szMachineID));
}

void CMD_GR_S_UserInsureSuccess::fromPack(const PacketAide& packData)
{
	dwUserID = packData.rdword();
	lUserScore = packData.rSCORE();
	lUserInsure = packData.rSCORE();
	packData.rchars(szDescribeString, countarray(szDescribeString));
}

void CMD_GP_UserInsureFailure::fromPack(const PacketAide& packData)
{
	lResultCode = packData.rlong();
	packData.rchars(szDescribeString, 128);
}

void CMD_ST_JIUJIJIN_req::toPack(PacketAide& packData)
{
	packData.wdword(userId);
}

void CMD_ST_JIUJIJIN_rsp::fromPack(const PacketAide& packData)
{
	gold = packData.rdword();
	code = packData.rdword();
}

void CMD_GP_DailyLottery_USERINFO::toPack(PacketAide& packData)
{
	packData.wdword(dwGameID);
}

void CMD_GP_DailyLottery_Get::fromPack(const PacketAide& packData)
{
	dwIndex = packData.rdword();
	wType = packData.rword();
	dwGetGold = packData.rdword();
	dwGetYuanbao = packData.rdword();
}

void CMD_ST_SignDay_req::toPack(PacketAide& packData)
{
	packData.wdword(userId);
	packData.wdword(opType);
}
void CMD_ST_SignDay_rsp::fromPack(const PacketAide& packData)
{
	gold = packData.rdword();
	opType = packData.rdword();
	ret = packData.rdword();
	signindex = packData.rdword();
	lxcount = packData.rdword();
}

void CMD_GP_Hall_Goods_USERINFO::toPack(PacketAide& packData)
{
	packData.wdword(userId);
	packData.wword(wIndex);
	packData.wchars(tName, countarray(tName));
	packData.wSCORE(llTelNum);
	packData.wchars(tAddress, countarray(tAddress));
	packData.wSCORE(uqq);
}

void CMD_GP_Hall_Goods_Get::fromPack(const PacketAide& packData)
{
	wgold = packData.rdword();
	dwErrorCode = packData.rdword();
}

void ItemGoodsDx::fromPack(const PacketAide& packData)
{
	packData.rchars(dtime, countarray(dtime));
	packData.rchars(itemName, countarray(itemName));
	packData.rchars(orderNo, countarray(orderNo));
	dstate = packData.rdword();
}

void CMD_ST_DxRewardList_req::toPack(PacketAide& packData)
{
	packData.wdword(userId);
}

void CMD_ST_DxRewardList_rsp::fromPack(const PacketAide& packData)
{
	for (int i = 0; i < countarray(aGoodsList);  i++)
	{
		aGoodsList[i].fromPack(packData);
	}
}

void EmailInfoSt::fromPack(const PacketAide& packData)
{
	eid = packData.rdword();
	packData.rchars(sendusername, countarray(sendusername));
	packData.rchars(emailtitle, countarray(emailtitle));
	packData.rchars(emialcontent, countarray(emialcontent));
	packData.rchars(sendtime, countarray(sendtime));
	isread = packData.rdword();
	attachtype = packData.rdword();
	attachnums = packData.rdword();
	attachtype2 = packData.rdword();
	attachnums2 = packData.rdword();
}

void CMD_ST_GetEmailList_req::toPack(PacketAide& packData)
{
	packData.wdword(userId);
}

void CMD_ST_GetEmailList_rsp::fromPack(const PacketAide& packData)
{
	for (int i = 0; i < countarray(emaillist); i++)
	{
		emaillist[i].fromPack(packData);
	}
}

void CMD_ST_GiveChuanPiaoReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(gameid);
	packData.wdword(nums);
}

void CMD_ST_GiveChuanPiaoRsp::fromPack(const PacketAide& packData)
{
	ret = packData.rdword();
}

void CMD_ST_GetLiPinReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wchars(cardnum, countarray(cardnum));
}

void CMD_ST_GetLiPinRsp::fromPack(const PacketAide& packData)
{
	ret = packData.rdword();
	gold = packData.rdword();
	money = packData.rdword();
}

void CMD_ST_ShareSucessReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(sharetype);
}

void CMD_ST_ShareSucessRsp::fromPack(const PacketAide& packData)
{
	lGold = packData.rlong();
	dwCount = packData.rdword();
	dwSharetype = packData.rdword();
}

void CMD_ST_GetShareReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wchars(cardnum, countarray(cardnum));
}

void CMD_ST_GetShareRsp::fromPack(const PacketAide& packData)
{
	ret = packData.rdword();
	money = packData.rdword();
	gold = packData.rdword();
}


void CMD_ST_FuDaiReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(optype);
	packData.wdword(gold);
	packData.wdword(money);
}

void CMD_ST_FuDaiRsp::fromPack(const PacketAide& packData)
{
	optype = packData.rdword();
	ret = packData.rdword();
	lefttime = packData.rlong();
}

void CMD_Web_RechargeRsp::fromPack(const PacketAide& packData)
{
	userid = packData.rdword();
	productid = packData.rdword();
	currgold = packData.rdword();
	currvip = packData.rdword();
	leftexp = packData.rdword();
	sumrecharge = packData.rdword();
}

void CMD_ST_ShouYiReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(optype);
}

void CMD_ST_ShouYiRsp::fromPack(const PacketAide& packData)
{
	optype = packData.rdword();
	hongbao = packData.rdword();
	gold = packData.rdword();
	money = packData.rdword();
}

void CMD_ST_ShouRankYiRsp::fromPack(const PacketAide& packData)
{
	for (int i = 0; i < countarray(ranks); i++)
	{
		ranks[i].userid = packData.rdword();
		packData.rchars(ranks[i].account, countarray(ranks[i].account));
		ranks[i].nums = packData.rdword();
		ranks[i].moneys = packData.rdword();
	}
}

void CMD_ST_BindAccountReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wchars(szAccounts, countarray(szAccounts));
	packData.wchars(szPassword, countarray(szPassword));
	packData.wchars(szMachineID, countarray(szMachineID));
}

void CMD_ST_BindAccountRsp::fromPack(const PacketAide& packData)
{
	ret = packData.rdword();
}

void CMD_ST_HaoPingReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
}

void CMD_ST_HaoPingRsp::fromPack(const PacketAide& packData)
{
	ret = packData.rdword();
	gold = packData.rdword();
}

void CMD_ST_ShareCountReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(optype);
}

void CMD_ST_ShareCountRsp::fromPack(const PacketAide& packData)
{
	count = packData.rdword();
	optype = packData.rdword();
	getcount = packData.rdword();
}

void CMD_ST_GetEmailAttReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(euid);
	packData.wdword(optye);
}

void CMD_ST_GetEmailAttRsp::fromPack(const PacketAide& packData)
{
	optye = packData.rdword();
	ret = packData.rdword();
}

void CMD_ST_ChatMsgReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(msgtype);
	packData.wchars(content, countarray(content));
}

void CMD_ST_ChatMsgRsp::fromPack(const PacketAide& packData)
{
	ntype = packData.rdword();
	hid = packData.rdword();
	ret = packData.rdword();
	msgtype = packData.rdword();
	userid = packData.rdword();
	viplevel = packData.rdword();
	faceid = packData.rdword();
	sendtime = packData.rSCORE();

	packData.rchars(uname, countarray(uname));
	packData.rchars(content, countarray(content));
}

void CMD_ST_ChatHongBaoReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(hongbaoid);
	packData.wdword(msgtype);
}

void CMD_ST_ChatGetHongBaoReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(hongbaoid);
}

void StHong::fromPack(const PacketAide& packData)
{
	userid = packData.rdword();
	gold = packData.rdword();
	viplevel = packData.rdword();
	packData.rchars(uname, countarray(uname));
}

void CMD_ST_ChatGetHongBaoRsp::fromPack(const PacketAide& packData)
{
	ret = packData.rdword();
	gold = packData.rdword();

	for (int i = 0; i < countarray(hlist); i++)
	{
		hlist[i].fromPack(packData);
	}
}

void CMD_ST_ReplaceReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(roomid);
}

void CMD_ST_NetFlushServerReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
	packData.wdword(roomid);
}

void CMD_ST_NetFlushUserRsp::fromPack(const PacketAide& packData)
{
	money = packData.rdword();
	cpvalue = packData.rdword();
}

void CMD_ST_HeartReq::toPack(PacketAide& packData)
{
	packData.wdword(userid);
}

void CMD_GP_GET_RANK::toPack(PacketAide& packData)
{
	packData.wdword(dwUserID);
	packData.wbyte(dwType);
}

void RankInfoSt::fromPack(const PacketAide& packData)
{
	dwRank = packData.rdword();
	dwUserID = packData.rdword();
	packData.rchars(szTitle, countarray(szTitle));
	lScore = packData.rSCORE();
	packData.rchars(szUserName, countarray(szUserName));
	dwVip = packData.rSCORE();
}

void CMD_GP_GET_RANK_RESULT::fromPack(const PacketAide& packData)
{
	for (int i = 0; i < countarray(ranklist); ++i)
	{
		ranklist[i].fromPack(packData);
	}
}

void CMD_GP_UpdateNotify::fromPack(const PacketAide& packData)
{
	cbMustUpdate = packData.rbyte();
	cbAdviceUpdate = packData.rbyte();
	dwCurrentVersion = packData.rdword();
	packData.rchars(szUpdateUrl, countarray(szUpdateUrl));
}