#ifndef  _UMShareSDK_H_
#define  _UMShareSDK_H_

#include "cocos2d.h"
#include "MacroDefineCommon.h"

USING_NS_CC;

enum eUMPlatformCode
{
	EU_WEIXIN			=	1,
	EU_WEIXIN_CIRCLE	=	2,
	EU_QZONE			=	4,

	EU_MAX
};

class  UMShareSDK
{
protected:
	UMShareSDK();

public:
	SINGLE_INSTANCE_FUNC(UMShareSDK);
    ~UMShareSDK();
	
	void directShare(int platform, const std::string& strText);

	int transformToServerPlatform(int nUMPlatform);		//将友盟平台转换成服务器平台id,1-微信朋友圈 2-QQ朋友圈 3-微信好友
protected:
	void _initSDKParam();
	void _initSDK();

protected:
	std::string strShareImg;
	std::string strUMAppkey;
	std::string strWeiXinAppId;
	std::string strWeiXinAppKey;
	std::string strQQAppId;
	std::string strQQAppkey;
};

#endif // _UMShareSDK_H_

