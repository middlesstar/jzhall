#ifndef SHZ_GLOBAL_USER_INFO_HEAD_FILE
#define SHZ_GLOBAL_USER_INFO_HEAD_FILE

#pragma once

#include "shuihu/plazz/Plazz_SHZ.h"

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//用户信息
struct tag_SHZGlobalUserData
{
	//基本资料
	dword							dwUserID;							//用户 I D
	dword							dwGameID;							//游戏 I D
	dword							dwUserMedal;						//用户奖牌
	dword							dwExperience;						//用户经验
	dword							dwLoveLiness;						//用户魅力
	char							szAccounts[LEN_ACCOUNTS];			//登录帐号
	char							szNickName[LEN_NICKNAME];			//用户昵称
	char							szPassword[LEN_PASSWORD];			//登录密码

	//扩展资料
	byte							cbGender;							//用户性别
	byte							cbMoorMachine;						//锁定机器
	char							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//社团资料
	dword							dwGroupID;							//社团索引
	char							szGroupName[LEN_GROUP_NAME];		//社团名字

	//会员资料
	byte							cbMemberOrder;						//会员等级
	SHZsystemtime						MemberOverDate;						//到期时间

	//头像信息
	word							wFaceID;							//头像索引
	dword							dwCustomID;							//自定标识
	tagCustomFaceInfo				CustomFaceInfo;						//自定头像
};

//扩展资料
struct tagIndividualUserData_SHZ
{
	//用户信息
	char							szUserNote[LEN_USER_NOTE];			//用户说明
	char							szCompellation[LEN_COMPELLATION];	//真实名字

	//电话号码
	char							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	char							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	char							szQQ[LEN_QQ];						//Q Q 号码
	char							szEMail[LEN_EMAIL];					//电子邮件
	char							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//银行信息
struct tagSHZUserInsureInfo
{
	word							wRevenueTake;						//税收比例
	word							wRevenueTransfer;					//税收比例
	word							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转账条件
};


//////////////////////////////////////////////////////////////////////////////////

//用户信息
class SHZCGlobalUserInfo
{
	//用户信息
protected:
	tag_SHZGlobalUserData				m_GlobalUserData;						//用户资料
	tagIndividualUserData_SHZ			m_IndividualUserData;					//扩展资料
	tagSHZUserInsureInfo				m_UserInsureInfo;						//银行资料

	//静态变量
protected:
	static SHZCGlobalUserInfo *		m_pSHZGlobalUserInfo;						//用户信息

	//函数定义
public:
	//构造函数
	SHZCGlobalUserInfo();
	//析构函数
	virtual ~SHZCGlobalUserInfo();

	//功能函数
public:
	//重置资料
	void ResetUserInfoData();
	//用户资料
	tag_SHZGlobalUserData * GetGlobalUserData() { return &m_GlobalUserData; }
	//扩展资料
	tagIndividualUserData_SHZ * GetIndividualUserData() { return &m_IndividualUserData; }
	//银行资料
	tagSHZUserInsureInfo * GetUserInsureInfo() { return &m_UserInsureInfo; }

	//静态函数
public:
	//获取对象
	static SHZCGlobalUserInfo * GetInstance() { return m_pSHZGlobalUserInfo; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif
