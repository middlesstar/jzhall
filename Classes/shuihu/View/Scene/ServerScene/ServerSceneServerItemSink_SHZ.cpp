#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "ServerScene_SHZ.h"
#include "shuihu/View/Scene/GameScene/ClientKernelSink_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
//��½��Ϣ

//����ʧ��
void SHZServerScene::onGRRequestFailure(const std::string& szDescribeString)
{
	PLAZZ_PRINTF("onGRRequestFailure %s", szDescribeString.c_str());
	popup_SHZ(SSTRING("gr_request_failure").c_str(), szDescribeString.c_str());
}

//��½�ɹ�
void SHZServerScene::OnGRLogonSuccess()
{
	PLAZZ_PRINTF("%s",SSTRING("gr_login_success").c_str());
}

//��½ʧ��
void SHZServerScene::OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString)
{
	G_NOTIFICATION_SHZ("BACK_TO_SERVER_LIST", SHZMTData::create(0, 0, 0, SSTRING("gr_login_failed"), sDescribeString));
}

//��½���
void SHZServerScene::OnGRLogonFinish()
{

	dword dwServerRule = mServerItem->GetServerAttribute().dwServerRule;

	//�����ж�
	if (SHZCServerRule::IsForfendGameRule(dwServerRule)==false)
	{

	}

	//�����ж�
	if (SHZCServerRule::IsForfendLockTable(dwServerRule)==false)
	{

	}

	//���Ұ�ť
	if (SHZCServerRule::IsAllowAvertCheatMode(dwServerRule)==false)
	{
	}

	if (SHZCServerRule::IsAllowQuickMode(dwServerRule))
	{
		// ����ʱ��
		this->scheduleOnce(schedule_selector(SHZServerScene::onQuickSitdown), 0.5);
	}
}

//����֪ͨ
void SHZServerScene::OnGRUpdateNotify(byte cbMustUpdate, const std::string& szDescribeString)
{
	//���ط�����˳�����
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
			//�˳���Ϸ
			if (SHZIServerItem::get())
				SHZIServerItem::get()->IntermitConnect(true);

			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//������Ϣ

//�б�����
void SHZServerScene::OnGRConfigColumn()
{

}
//��������
void SHZServerScene::OnGRConfigServer() 
{


}
//��������
void SHZServerScene::OnGRConfigProperty()
{

}
//���Ȩ������
void SHZServerScene::OnGRConfigUserRight() 
{

}

//�������
void SHZServerScene::OnGRConfigFinish()
{

}

//////////////////////////////////////////////////////////////////////////
//�û���Ϣ

//�û�����
void SHZServerScene::OnGRUserEnter(SHZIClientUserItem* pIClientUserItem)
{

}
//�û�����
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
//�û�ɾ��
void SHZServerScene::OnGRUserDelete(SHZIClientUserItem* pIClientUserItem)
{

}

//////////////////////////////////////////////////////////////////////////
//�����Ϣ

//�û�����
void SHZServerScene::OnGFUserInvite(const std::string& szMessage)
{
	popup_SHZ(SSTRING("gr_user_invite").c_str(), szMessage.c_str());
}
//�û�����ʧ��
void SHZServerScene::OnGFUserInviteFailure(const std::string& szMessage)
{
	popup_SHZ(SSTRING("gr_user_invite_failed").c_str(), szMessage.c_str());
}

//�����˳�
void SHZServerScene::OnGFServerClose(const std::string& szMessage)
{	
	mMessageTemp = szMessage;
	
	PLAZZ_PRINTF("ServerScene::OnGFServerClose %s", mMessageTemp.c_str());

	//�ӳ��˳�
	this->scheduleOnce(schedule_selector(SHZServerScene::func_server_close), 0.3f);

}

//������Ϸ�ں�
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