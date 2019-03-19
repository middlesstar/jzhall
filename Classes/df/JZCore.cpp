#include "JZCore.h"
#include "cocos2d.h"
#include "LanguageConfig.h"
USING_NS_CC;

//// android JNI
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <Jni.h>

#define JNI_PACKAGE_NAME "org.cocos2dx.cpp/JniCaller"

bool getJniMethod(jobject& jobj, JniMethodInfo &minfo, const char* sFunction, const char* sSignature)
{
	if (!JniHelper::getStaticMethodInfo(minfo,JNI_PACKAGE_NAME,"shared","()Ljava/lang/Object;"))
		return false;  
	jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	return JniHelper::getMethodInfo(minfo, JNI_PACKAGE_NAME, sFunction, sSignature);
}

/// 得到设备的唯一标示
void getDeviceIDForAndroid()
{
	JniMethodInfo minfo;
	jobject jobj;

	if(getJniMethod(jobj,minfo,"getDeviceID","()Ljava/lang/String;"))
	{
		jstring result = (jstring)minfo.env->CallObjectMethod(jobj,minfo.methodID);
		const char* str = minfo.env->GetStringUTFChars(result,0);
		JZCore::getInstance()->setDeviceID(str);
		minfo.env->DeleteLocalRef(result);
	}
}

void getDeviceNameForAndroid()
{
	JniMethodInfo minfo;
	jobject jobj;

	if(getJniMethod(jobj,minfo,"getDeviceName","()Ljava/lang/String;"))
	{
		jstring result = (jstring)minfo.env->CallObjectMethod(jobj, minfo.methodID);
		const char* str = minfo.env->GetStringUTFChars(result, 0);
		JZCore::getInstance()->setDeviceName(str);
		minfo.env->DeleteLocalRef(result);
	}
}

#endif

JZCore* JZCore::m_pCore = nullptr;

JZCore* JZCore::getInstance()
{
	if (nullptr == m_pCore)
	{
		m_pCore = new JZCore;
	}
	return m_pCore;
}

void JZCore::initPlatform()
{
	/// 得到平台的唯一标示
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	getDeviceIDForAndroid();
	getDeviceNameForAndroid();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	JZCore::getInstance()->setDeviceName("window");
	JZCore::getInstance()->setDeviceID(GETSTRING(999995).c_str());
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    JZCore::getInstance()->setDeviceName("ios");
//    JZCore::getInstance()->setDeviceID(GlobelDef::UUID_STR);
#else
    

#endif
}

/// 得到设备的唯一标示
std::string& JZCore::getDeviceID()
{
	return mszDeviceID;
}

/// 得到设备的名称
std::string& JZCore::getDeviceName()
{
	return mszDeviceName;
}

/// 手机平台显示消息
void JZCore::Toast(std::string& message)
{
	cocos2d::MessageBox(message.c_str(), "");
}
