#include "cocos2d.h"
#include "gPlatform_SHZ.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <Jni.h>
#include "platform/android/jni/JniHelper.h"

#define JNI_PACKAGE "org.cocos2dx.cpp/JniCaller"

bool getJniMethod_SHZ(jobject& jobj, JniMethodInfo &minfo, const char* sFunction, const char* sSignature)
{
	if (!JniHelper::getStaticMethodInfo(minfo, JNI_PACKAGE, "shared", "()Ljava/lang/Object;"))
		return false;

	jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	return JniHelper::getMethodInfo(minfo, JNI_PACKAGE, sFunction, sSignature);
}

bool jniCjIsConnectToInternet()
{
	JniMethodInfo minfo;
	jobject jobj;
	int result = 0;
	if (getJniMethod_SHZ(jobj, minfo, "jniCjIsConnectToInternet", "()I"))
	{
		result = minfo.env->CallIntMethod(jobj, minfo.methodID);
	}
	cocos2d::log("result:%d", result);
	return result == 1;
}
#endif

bool platformIsConnectToInternet_SHZ()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return jniCjIsConnectToInternet();
#else
	return true;
#endif
}