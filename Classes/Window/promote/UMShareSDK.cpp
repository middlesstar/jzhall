#include "UMShareSDK.h"
#include "df/IPConfig.h"
#include "HallGameScene.h"
#include "LogicPromote.h"
#include "Player/Player.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "Cocos2dx/ShareButton/UMShareButton.h"
USING_NS_UM_SOCIAL;		// 使用友盟命令空间

static CCUMSocialSDK* gpUMSDK = nullptr;
#endif

/*
* 分享回调, 该回调不是某个类的成员函数， 而是一个普通的函数, 具体使用参考HelloWorldScene的例子
* @param platform 要分享到的目标平台
* @param stCode 返回码, 200代表分享成功, 100代表开始分享
* @param errorMsg 分享失败时的错误信息,android平台没有错误信息
*/
void directShareCallback(int platform, int stCode, const std::string& errorMsg)
{
	log("---------------UMShareSDK::_directShareCallback[platform = %d,stCode = %d,errorMsg = %s]--------------", platform, stCode, errorMsg.c_str());
	

	if (stCode == 100)
	{
		//log("#### HelloWorld 开始分享");
	}
	else if (stCode == 200)
	{
		HallGameScene::getInstance()->openMessageTip(15);

		LogicPromote::getInstance()->requestShareSucess(UMShareSDK::getInstance()->transformToServerPlatform(platform));
	}
	else
	{
		HallGameScene::getInstance()->openMessageTip(errorMsg.c_str());
	}
}

UMShareSDK::UMShareSDK()
{
	_initSDKParam();
	_initSDK();
}

UMShareSDK::~UMShareSDK()
{

}

void UMShareSDK::directShare(int platform, const std::string& strText)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (!gpUMSDK || strShareImg.empty())
	{
		return;
	}

	gpUMSDK->directShare(platform,strText.c_str(),strShareImg.c_str(),share_selector(directShareCallback));
#else
	directShareCallback(platform,200,"Window share!");
#endif
}

int UMShareSDK::transformToServerPlatform(int nUMPlatform)
{
	int nRet = 0;
	switch (nUMPlatform)
	{
	case EU_WEIXIN:
		nRet = 3;
		break;
	case EU_WEIXIN_CIRCLE:
		nRet = 1;
		break;
	case EU_QZONE:
		nRet = 2;
		break;
	default:
		break;
}

	return nRet;
}

void UMShareSDK::_initSDKParam()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	strShareImg = "hall_icon.png";
	strUMAppkey = "56efe42167e58ebb6a001b97";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	strShareImg = "assets/hall_icon.png";
	strUMAppkey = "56efe511e0f55a01940007c8";
#endif

    strWeiXinAppId = "wx563619c3d583d89f";
    strWeiXinAppKey = "1e57fa2a3ac55a57075dc6ba180ae92a";
	strQQAppId = "1105205909";
	strQQAppkey = "nWjLxuKy6vKFf3in";
}

void UMShareSDK::_initSDK()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	gpUMSDK = CCUMSocialSDK::create(strUMAppkey.c_str());			// 获取一个CCUMSocialSDK实例
	gpUMSDK->setQQAppIdAndAppKey(strQQAppId.c_str(), strQQAppkey.c_str());
	gpUMSDK->setWeiXinAppInfo(strWeiXinAppId.c_str(),strWeiXinAppKey.c_str());
	gpUMSDK->setLogEnable(true) ;
    gpUMSDK->setMessageTypeWeb();
	gpUMSDK->setTargetUrl(StringUtils::format("http://admin.92dwc.com/weixin/sharepage.aspx?userid=%d",Player::getInstance()->getLogonData().dwUserID).c_str());

	vector<int>* platforms = new vector<int>();
	platforms->push_back(WEIXIN);
	platforms->push_back(WEIXIN_CIRCLE);
	platforms->push_back(QZONE);
	gpUMSDK->setPlatforms(platforms);
#endif
}