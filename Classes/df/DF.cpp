//#include "../Plazz.h"
#include "DF.h"
#include "JZCore.h"
#include "libs/crypt/coMD5.h"
#include "cocos2d.h"
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////

DF* DF::shared() 
{
	static DF sInstance;
	return &sInstance;
}

const char* DF::MD5Encrypt(const char* pszSourceData)
{
	static std::string sPassword;
	sPassword = coMD5(pszSourceData);
	return sPassword.c_str();
} 
//////////////////////////////////////////////////////////////////////////

DF::DF() :mGameType(0)
, mErrorMessage("")
{
	 
}

DF::~DF()
{
}

void DF::init(int iKindID, int iPlayers, int iClientVersion, const char* sGameName)
{
	mKindID = iKindID;
	mGamePlayers = iPlayers;
	mClientVersion = iClientVersion;
	mGameName = sGameName;
}

void DF::init(int iKindID, int iPlayers, int iClientVersion, const char* sGameName, int iPlatformID){

	mKindID        = iKindID;
	mGamePlayers   = iPlayers;
	mClientVersion = iClientVersion;
	mGameName      = sGameName;
	mPlatformID    = iPlatformID;
    
}

void DF::SetGameType(int __type){
	mGameType = __type;
	mEnableSingle = false;
	mbGame = false;
}

byte DF::GetDeviceType()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return DEVICE_TYPE_IPHONE;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return DEVICE_TYPE_ANDROID;
#else
	return DEVICE_TYPE_PC;
#endif
}

const char* DF::GetMachineID()
{
	static bool bFirst = true;
	static char szMachineID[LEN_MACHINE_ID]={0};

	if (bFirst)
	{
		bFirst=false;
        sprintf(szMachineID, "%s", JZCore::getInstance()->getDeviceID().c_str());
        log("===================== szMachineID : %s", szMachineID);

	}

	return szMachineID;
}

const char* DF::GetMobilePhone()
{
	static bool bFirst = true;
	static char szPhone[LEN_MACHINE_ID]={0};

	if (bFirst)
	{
		bFirst=false;
        sprintf(szPhone,"%s", JZCore::getInstance()->getDeviceName().c_str());
        log("===================== szPhone : %s", szPhone);
	}

	return szPhone;
}


uint32 DF::GetPlazaVersion()
{
	switch (GetDeviceType())
	{
	case DEVICE_TYPE_ANDROID:
		return VERSION_MOBILE_ANDROID;
	case DEVICE_TYPE_IPAD:
	case DEVICE_TYPE_IPHONE:
	case DEVICE_TYPE_ITOUCH:
		return VERSION_MOBILE_IOS;
	}

	return VERSION_PLAZA;
}

uint32 DF::GetFrameVersion()
{
	return VERSION_FRAME;
}

uint16 DF::GetPlazzKindID()
{
	//if (GetDeviceType() == DEVICE_TYPE_PC)
	//	return INVALID_WORD;

	return GetGameKindID();
}


uint16 DF::GetPlatformID()
{
	return mPlatformID;
}

uint32 DF::GetGameVersion()
{
	return mClientVersion;
	//switch (GetDeviceType())
	//{
	//case DEVICE_TYPE_ANDROID:
	//	return PROCESS_VERSION(6,0,3);
	//case DEVICE_TYPE_IPAD:
	//case DEVICE_TYPE_IPHONE:
	//case DEVICE_TYPE_ITOUCH:
	//	return PROCESS_VERSION(6,0,3);
	//}

	//return PROCESS_VERSION(6,0,3);
}

uint16 DF::GetGameKindID()
{
	return mKindID;
}
uint32 DF::GetGamePlayer()
{
	return mGamePlayers;
}

const char* DF::GetGameName(char szGameName[LEN_KIND])
{
	const char* sGameName = mGameName.c_str();
	strncpy(szGameName, sGameName, LEN_KIND);
	return sGameName;
}

const char* DF::GetGameName()
{
	return mGameName.c_str();
} 