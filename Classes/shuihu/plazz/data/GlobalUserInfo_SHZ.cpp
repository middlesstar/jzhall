#include "GlobalUserInfo_SHZ.h"

SHZCGlobalUserInfo	__gGlobalUserInfo_SHZ;	//用户信息

//////////////////////////////////////////////////////////////////////////////////

//静态变量
SHZCGlobalUserInfo * SHZCGlobalUserInfo::m_pSHZGlobalUserInfo=0;				//用户信息

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCGlobalUserInfo::SHZCGlobalUserInfo()
{
	//设置对象
	ASSERT(m_pSHZGlobalUserInfo==0);
	if (m_pSHZGlobalUserInfo==0) m_pSHZGlobalUserInfo=this;

	//设置变量
	zeromemory(&m_GlobalUserData,sizeof(m_GlobalUserData));
	zeromemory(&m_IndividualUserData,sizeof(m_IndividualUserData));

	return;
}

//析构函数
SHZCGlobalUserInfo::~SHZCGlobalUserInfo()
{
	//释放对象
	ASSERT(m_pSHZGlobalUserInfo==this);
	if (m_pSHZGlobalUserInfo==this) m_pSHZGlobalUserInfo=0;

	return;
}

//重置资料
void SHZCGlobalUserInfo::ResetUserInfoData()
{
	//设置变量
	zeromemory(&m_GlobalUserData,sizeof(m_GlobalUserData));
	zeromemory(&m_IndividualUserData,sizeof(m_IndividualUserData));

	return;
}

//////////////////////////////////////////////////////////////////////////////////
