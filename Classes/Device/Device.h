#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "cocos2d.h"
//#include "consts/UmengEventKey.h"

#define UMENG_PROFILE_SIGNIN(uid) JZ::Device::UmengProfileSignIn(uid)
#define UMENG_EVENT(EVENT_ID) JZ::Device::UmengEvent(EVENT_ID)

namespace JZ {
	class Device
	{
	public:
		static void updateGame(const char *url);
		static bool isThisVersionOnline();

		static void openCommentUrl();

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        static void pay(int rechargeType, const std::string &userid, const std::string &orderid);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		static std::string getPackageName();
		static void pay(const std::string &transId);
#endif

		//=========================================== umeng anylyse =======================================================
		static void UmengInit();
		static void UmengDidEnterBackground();
		static void UmengWillEnterForground();
		static void UmengEnd();
		static void UmengProfileSignIn(const char *uid);
		static void UmengEvent(const char *eventId, const char *label = nullptr);
		static void UmengEvent(const char *eventId, const char *keys[], const char *values[], size_t arraySize);
		//=========================================== umeng anylyse end =======================================================
	};
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_OnPayProcessFailed(JNIEnv *env, jobject obj);
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_OnUpdateDownloadProgress(JNIEnv *env, jobject obj, jfloat percent);
}
#endif

#endif // !__DEVICE_H__
