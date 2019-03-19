//
//  AppStore_ios.cpp
//  buyu_final
//
//  Created by XiaoQuan Sha on 12/17/15.
//
//

#include "Device/Device.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

#define  CLASS_NAME "org/cocos2dx/cpp/AppActivity"

namespace JZ {

	void Device::pay(const std::string &transId)
	{
		log("---------------------jnimethod1----------------");
		JniMethodInfo minfo;
		jobject jobj;
		bool isHave = JniHelper::getStaticMethodInfo(minfo, CLASS_NAME, "getInstance", "()Ljava/lang/Object;");
		if (isHave)
		{
			log("---------------------jnimethod2----------------");
			jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		}

		isHave = JniHelper::getMethodInfo(minfo, CLASS_NAME, "startIAPPay", "(Ljava/lang/String;)V");
		if (isHave)
		{
//			jstring appuserid_jstr = minfo.env->NewStringUTF(appuserid.c_str());
//			jstring cporderid_jstr = minfo.env->NewStringUTF(cporderid.c_str());
//			jstring notifyurl_jstr = minfo.env->NewStringUTF(notifyurl.c_str());
//			minfo.env->CallVoidMethod(jobj, minfo.methodID, appuserid_jstr, wareid, iprice, cporderid_jstr, notifyurl_jstr);
//			minfo.env->DeleteLocalRef(appuserid_jstr);
//			minfo.env->DeleteLocalRef(cporderid_jstr);
//			minfo.env->DeleteLocalRef(notifyurl_jstr);
           log("---------------------jnimethod3----------------");
            jstring apptransId_jstr = minfo.env->NewStringUTF(transId.c_str());
			minfo.env->CallVoidMethod(jobj, minfo.methodID, apptransId_jstr);
			minfo.env->DeleteLocalRef(apptransId_jstr);
		}
		minfo.env->DeleteLocalRef(minfo.classID);
	}

}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_OnPayProcessFailed(JNIEnv *env, jobject obj)
{
//	G_NOTIFY(HIDE_SHIELD_NOTIFY, 0);
}
