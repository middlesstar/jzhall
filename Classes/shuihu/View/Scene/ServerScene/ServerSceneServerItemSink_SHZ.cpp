#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "ServerScene_SHZ.h"
#include "shuihu/View/Scene/GameScene/ClientKernelSink_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
//登陆信息

//请求失败
void SHZServerScene::onGRRequestFailure(const std::string& szDescribeString)
{
	PLAZZ_PRINTF("onGRRequestFailure %s", szDescribeString.c_str());
	popup_SHZ(SSTRING("gr_request_failure").c_str(), szDescribeString.c_str());
}

//登陆成功
void SHZServerScene::OnGRLogonSuccess()
{
	PLAZZ_PRINTF("%s",SSTRING("gr_login_success").c_str());
}

//登陆失败
void SHZServerScene::OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString)
{
	G_NOTIFICATION_SHZ("BACK_TO_SERVER_LIST", SHZMTData::create(0, 0, 0, SSTRING("gr_login_failed"), sDescribeString));
}

//登陆完成
void SHZServerScene::OnGRLogonFinish()
{

	dword dwServerRule = mServerItem->GetServerAttribute().dwServerRule;

	//规则判断
	if (SHZCServerRule::IsForfendGameRule(dwServerRule)==false)
	{

	}

	//锁桌判断
	if (SHZCServerRule::IsForfendLockTable(dwServerRule)==false)
	{

	}

	//查找按钮
	if (SHZCServerRule::IsAllowAvertCheatMode(dwServerRule)==false)
	{
	}

	if (SHZCServerRule::IsAllowQuickMode(dwServerRule))
	{
		// 动画时间
		this->scheduleOnce(schedule_selector(SHZServerScene::onQuickSitdown), 0.5);
	}
}

//更新通知
void SHZServerScene::OnGRUpdateNotify(byte cbMustUpdate, const std::string& szDescribeString)
{
	//返回房间或退出房间
	if (cbMustUpdate)
	{
		popup_SHZ(SSTRING("gr_update_notify").c_str(), szDescribeString.c_str(), DLG_MB_OK, 0, this, callfuncN_selector(SHZServerScene::OnUpdateRequest));
		return;
	}

	popup_SHZ(SSTRING("gr_update_notify").c_str(), szDescribeString.c_str(), DLG_MB_OK | DLG_MB_CANCEL, 0, this, callfuncN_selector(SHZServerScene::OnUpdateRequest));
}

void SHZServerScene::OnUpdateRequest(CCNode* node)
{
	switch (node->getTag())
	{
	case DLG_MB_OK:
		{
			//退出游戏
			if (SHZIServerItem::get())
				SHZIServerItem::get()->IntermitConnect(true);

			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//配置信息

//列表配置
void SHZServerScene::OnGRConfigColumn()
{

}
//房间配置
void SHZServerScene::OnGRConfigServer() 
{


}
//道具配置
void SHZServerScene::OnGRConfigProperty()
{

}
//玩家权限配置
void SHZServerScene::OnGRConfigUserRight() 
{

}

//配置完成
void SHZServerScene::OnGRConfigFinish()
{

}

//////////////////////////////////////////////////////////////////////////
//用户信息

//用户进入
void SHZServerScene::OnGRUserEnter(SHZIClientUserItem* pIClientUserItem)
{

}
//用户更新
void SHZServerScene::OnGRUserUpdate(SHZIClientUserItem* pIClientUserItem)
{

	if (pIClientUserItem == SHZIServerItem::get()->GetMeUserItem())
	{
		G_NOTIFICATION_SHZ("USER_SCORE", 0);

		if (pIClientUserItem->GetUserStatus()==US_FREE)
		{
			
		}
	}
		

}
//用户删除
void SHZServerScene::OnGRUserDelete(SHZIClientUserItem* pIClientUserItem)
{

}

//////////////////////////////////////////////////////////////////////////
//框架消息

//用户邀请
void SHZServerScene::OnGFUserInvite(const std::string& szMessage)
{
	popup_SHZ(SSTRING("gr_user_invite").c_str(), szMessage.c_str());
}
//用户邀请失败
void SHZServerScene::OnGFUserInviteFailure(const std::string& szMessage)
{
	popup_SHZ(SSTRING("gr_user_invite_failed").c_str(), szMessage.c_str());
}

//房间退出
void SHZServerScene::OnGFServerClose(const std::string& szMessage)
{	
	mMessageTemp = szMessage;
	
	PLAZZ_PRINTF("ServerScene::OnGFServerClose %s", mMessageTemp.c_str());

	//延迟退出
	this->scheduleOnce(schedule_selector(SHZServerScene::func_server_close), 0.3f);

}

//创建游戏内核
bool SHZServerScene::CreateKernel()
{
	SHZIClientKernel* kernel = SHZIClientKernel::create();

	if (kernel == 0)
		return false;
	//kernel->SetChatSink();
	//kernel->SetStringMessageSink();
	kernel->SetClientKernelSink(&gClientKernelSink_SHZ);


	if (kernel->Init()){
		return true;
	}

	SHZIClientKernel::destory();
	return false;
}




void SHZServerScene::func_server_close(float dt)
{


	if (!mMessageTemp.empty())
	{
		G_NOTIFICATION_SHZ("BACK_TO_SERVER_LIST", SHZMTData::create(0, 0, 0, SSTRING("gr_exit_room"), mMessageTemp));
	}
	else
	{
		G_NOTIFICATION_SHZ("BACK_TO_SERVER_LIST", 0);
	}
}