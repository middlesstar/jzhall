#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "ClientKernelSink_SHZ.h"
#include "TimestampManager_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"
#include "Network/CommandDef.h"
#include "shuihu/View/Scene/LotteryScene/LotteryMission_SHZ.h"
USING_NS_CC;
SHZClientKernelSink gClientKernelSink_SHZ;
//////////////////////////////////////////////////////////////////////////
//���ƽӿ�
//������Ϸ����
bool SHZClientKernelSink::SetupGameClient()
{
	
	PLAZZ_PRINTF(("flow->ClientKernelSink::SetupGameClient\n"));
	mIsNetworkPrepared = false;
	m_GameClientView   = SHZCGameClientView::create();
	PLAZZ_PRINTF(("flow->ClientKernelSink::SetupGameClient1\n"));
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,m_GameClientView->>);
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f,m_GameClientView));
	
	//return true ���ͽ��볡����Ϣ
	//����, �Լ�����IClientKernel->SendGameOption();

	//m_btishi=false;

	return false;
}

//������Ϸ
void SHZClientKernelSink::ResetGameClient()
{
	
	PLAZZ_PRINTF(("flow->ClientKernelSink::ResetGameClient\n"));
	mIsNetworkPrepared = false;


}

//�ر���Ϸ
void SHZClientKernelSink::CloseGameClient()
{
	mIsNetworkPrepared = false;
	m_GameClientView= 0;
	if (DF::shared()->GetGameType() == 0)
	{
		if (!platformIsConnectToInternet_SHZ()){
			if (Director::getInstance()->getRunningScene())
			{
				Director::getInstance()->popScene();
			}

			return;
		}
	}


	if (Director::getInstance()->getRunningScene())
	{
		Director::getInstance()->popScene();
	}

}

//////////////////////////////////////////////////////////////////////////
//����¼�

//ϵͳ������Ϣ
bool SHZClientKernelSink::OnGFTableMessage(const char* szMessage)
{
	PLAZZ_PRINTF(("flow->ClientKernelSink::OnGFTableMessage %s\n"), szMessage);
	return true;
}

//�ȴ���ʾ
bool SHZClientKernelSink::OnGFWaitTips(bool bWait)
{
	PLAZZ_PRINTF(("flow->ClientKernelSink::OnGFWaitTips %s\n"), bWait ? a_u8("��Ϸ�ȴ�...") : a_u8("�ȴ�����"));
	return true;
}

//������Ϣ
bool SHZClientKernelSink::OnGFMatchInfo(tagMatchInfo* pMatchInfo)
{
	PLAZZ_PRINTF(("flow->ClientKernelSink::OnGFMatchInfo\n"));
	return true;
}

//�����ȴ���ʾ
bool SHZClientKernelSink::OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip)
{
	PLAZZ_PRINTF(("flow->ClientKernelSink::OnGFMatchWaitTips\n"));
	return true;
}

//�������
bool SHZClientKernelSink::OnGFMatchResult(tagMatchResult* pMatchResult)
{
	PLAZZ_PRINTF(("flow->ClientKernelSink::OnGFMatchResult\n"));
	return true;
}


//////////////////////////////////////////////////////////////////////////
//�Թ���Ϣ
bool SHZClientKernelSink::OnEventLookonMode(void * pData, int wDataSize)
{
	return true;
}

//��Ϸ��Ϣ
bool SHZClientKernelSink::OnEventGameMessage(int wSubCmdID, void * pData, int wDataSize)
{

	SHZIClientKernel *kernel = SHZIClientKernel::get();

	switch (wSubCmdID)
	{
		//���·�
	case SUB_S_UPORDOWNESCORE_SHZ:
		{

			ASSERT(wDataSize==sizeof(CMD_S_UpOrDownScore_SHZ));
			if (wDataSize!=sizeof(CMD_S_UpOrDownScore_SHZ)) {return false;}
			//����ת��
			CMD_S_UpOrDownScore_SHZ *pBuyBullet=(CMD_S_UpOrDownScore_SHZ*)pData;
			//�����Լ����·�
			m_GameClientView->SetCreditScoreScore(pBuyBullet->m_UserScore);
			//�����Լ�����
			//const tagUserInfo * pUserData =kernel->GetMeUserItem()->GetUserInfo();
			//m_GameClientView->SetBaseScore(pUserData->lScore);
			return true;
		}

		//��ע��Ϣ
	case SUB_S_ADDSCORE_SHZ: 
		{
			PLAZZ_PRINTF("%s",a_u8("��ע��Ϣ"));
			ASSERT(wDataSize==sizeof(CMD_S_AddScore_SHZ));
			if (wDataSize!=sizeof(CMD_S_AddScore_SHZ)) {return false;}
			PLAZZ_PRINTF("%s",a_u8("��ע��Ϣ2"));
			//����ת��
			CMD_S_AddScore_SHZ *pMyData=(CMD_S_AddScore_SHZ*)pData;
			//�����Լ����·�
			m_GameClientView->SetXiaZhu(pMyData->m_AddScore);
			return true;
		}
		//������Ϣ
	case SUB_S_TYPESCROLL_SHZ:
		{
			//PLAZZ_PRINTF("wDataSize=%d,%s=%d",a_u8("������Ϣ"),wDataSize,sizeof(CMD_S_TypeScroll));
			ASSERT(wDataSize==sizeof(CMD_S_TypeScroll_SHZ));
			if (wDataSize!=sizeof(CMD_S_TypeScroll_SHZ)) {return false;}
			//����ת��
			CMD_S_TypeScroll_SHZ *pMyData=(CMD_S_TypeScroll_SHZ*)pData;
			//�����Լ����·�
			m_GameClientView->SetCreditScoreScore(pMyData->m_UserScore);
			//�����û��ĳ齱���־���
			if (!pMyData->bquanpan)
			{
				if (0 !=pMyData->m_WinScore)
				   m_GameClientView->setLotteryExp(pMyData->m_WinScore);
			}
			else
			{
			}

			//
			for (int i = 0; i < 15; i++)
			{
				if (pMyData->m_TypeImgid[i] == 9)
				{
					log("---------pMyData->m_TypeImgid[i] = %d-------------", pMyData->m_TypeImgid[i]);
				}
				
				m_GameClientView->ScrollInformation(pMyData->m_WinScore, i, pMyData->m_TypeImgid[i], pMyData->bquanpan);
				
			}
				

			return true;
		} 
		//
	case SUB_S_BIBEIDATAM_SHZ:  //�ȱ�����
		{
			ASSERT(wDataSize==sizeof(CMD_S_BiBeiData_SHZ));
			if (wDataSize!=sizeof(CMD_S_BiBeiData_SHZ)) {return false;}
			//����ת��
			CMD_S_BiBeiData_SHZ *pMyData=(CMD_S_BiBeiData_SHZ*)pData;
			//CString str;
			// str.Format(L"%d",pMyData->m_BiBeiData[0]);
			// AfxMessageBox(str);
			//��������
			for (int i=0;i<10;i++)
			{
				m_GameClientView->CopyBiBeiData(pMyData->m_BiBeiData[i],i);
				
			}
			

			return true;

		}
		//��ȡӮ����Ϣ
	case SUB_S_GETSCORE_SHZ:
		{
			//AfxMessageBox(L"1d");
			ASSERT(wDataSize==sizeof(CMD_S_WinScore_SHZ));
			if (wDataSize!=sizeof(CMD_S_WinScore_SHZ)) {return false;}
			//����ת��
			CMD_S_WinScore_SHZ *pMyData=(CMD_S_WinScore_SHZ*)pData;

			//�����Լ����·�
			m_GameClientView->GetWinScore(pMyData->m_WinScore,pMyData->m_GameState);
			//


			return true;
		}
		//�ȱ���Ϣ
	case SUB_S_BIBEISTART_SHZ:
		{

			ASSERT(wDataSize==sizeof(CMD_S_BiBeiRelult_SHZ));
			if (wDataSize!=sizeof(CMD_S_BiBeiRelult_SHZ)) {return false;}
			//����ת��
			CMD_S_BiBeiRelult_SHZ *pMyData=(CMD_S_BiBeiRelult_SHZ*)pData;
			
			//�����Լ����·�
			m_GameClientView->SetBiBei(pMyData->m_WinScore,pMyData->m_S1,pMyData->m_S2);
			

			return true;
		}
		//С������Ϣ
	case SUB_S_MALISTART_SHZ:
		{
			PLAZZ_PRINTF("CMD_S_MaLi=%d",sizeof(CMD_S_MaLi_SHZ));
			if (wDataSize!=sizeof(CMD_S_MaLi_SHZ)) {return false;}
			//����ת��
			CMD_S_MaLi_SHZ *pMyData=(CMD_S_MaLi_SHZ*)pData;
			for(int i=0;i<4;i++)
				m_GameClientView->SetMaLiInformation(pMyData->m_AddScore,i,pMyData->m_ThreeTypeId[i],pMyData->m_GameCount,pMyData->m_TypeIndex,pMyData->m_NowBegin);

			return true;
		}
	case SUB_S_ADDBEI_SHZ:  //�ӱ�
		{
			if (wDataSize!=sizeof(CMD_S_Addbei_SHZ)) {return false;}
			//����ת��
			CMD_S_Addbei_SHZ *pAddbei=(CMD_S_Addbei_SHZ*)pData;

			m_GameClientView->m_NodeGameView1->SetAddbei(pAddbei->iaddCount);

			return true;
		}
	case SUB_S_GAMEMSG_SHZ:
		{
			

			std::string str;

			if (DF::shared()->GetGameType() == 0){
				PACKET_AIDE_DATA(pData);
				u2string sMsg;
				sMsg.resize(wDataSize + 1, '\0');
				packet.readUTF16(&sMsg[0], wDataSize / 2);
				str = u2_8(&sMsg[0]);
			}
			else{
				char* cData = (char*)pData;
				str = a_u8(cData);
			}
			
			PLAZZ_PRINTF("%s",str.c_str());

			//m_GameClientView->SetGameNotice(str.c_str());

			return true;
		}
	case SUB_S_LEVEL_UP_SHZ:
	    {
		   //m_GameClientView->StartLevelUpAnim();  //������������
		   return true;
	    }
	case SUB_S_DISABLE_GAME_SHZ:
	   {
		   m_GameClientView->DisableGamePopUp();  //������Ϸ
		   return true; 
	   }
	case SUB_S_YA_LINE_SHZ:
	  {
		  CMD_C_Line_SHZ* cLine = (CMD_C_Line_SHZ*)pData;
		  m_GameClientView->SetYaLine(cLine->iCurLine);
		  return true;
	  }
	case SUB_S_REWARD_SHZ:{
			return true;
	}
		//ˮ䰴�����齱����
	case SUB_S_LOTTERYEXP_SHZ:
	{
			LotteryMission::getInstance()->recLottery(pData);
			
			return true;
	}
  }

	//�������
	//ASSERT(FALSE);

	return true;
}


//������Ϣ
bool SHZClientKernelSink::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void * pData, int wDataSize)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SetGameStatus(GAME_SCENE_FREE_SHZ);

	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE_SHZ:   //����״̬
	case GAME_SCENE_PLAY_SHZ:	//��Ϸ״̬
		{
			CMD_S_GameScene_SHZ *MyData  = (CMD_S_GameScene_SHZ *)pData;
			

			SHZCGlobalUserInfo   * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
			tagSHZUserInsureInfo * pUserInsureInfo=pGlobalUserInfo->GetUserInsureInfo();
			tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			m_GameClientView->m_iDuihuan = MyData->m_CellScore;
			m_GameClientView->SetCreditScoreScore(pUserInsureInfo->lUserScore);//MyData->m_UserScore);


			SHZIClientKernel * kernel = SHZIClientKernel::get();
			CMD_S_UpOrDownScore_SHZ  UserDownSocre;
			UserDownSocre.m_UserScore = pUserInsureInfo->lUserScore / m_GameClientView->m_iDuihuan;
			kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_UPORDOWNESCORE_SHZ,&UserDownSocre,sizeof(CMD_S_UpOrDownScore_SHZ));
			return true;
		}
	}

	return false;
}

bool  SHZClientKernelSink::GetWinScore(int gettype)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	CMD_C_GetScore_SHZ getscore;
	getscore.m_GetType = gettype;
	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_GETSCORE_SHZ,&getscore,sizeof(CMD_C_GetScore_SHZ));
	return true;
}
//��ʼ����
bool SHZClientKernelSink::StartGetType()
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SendSocketData(MDM_GF_GAME_SHZ,SUB_C_TYPESCROLL_SHZ);
	return true;
}

//���·�
bool SHZClientKernelSink::AddOrRemoveScore(bool addorremove)
{
	CMD_C_UpOrDownScore_SHZ  UporDown;
	UporDown.m_bUpOrDown = addorremove;
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_UPORDOWNESCORE_SHZ,&UporDown,sizeof(CMD_C_UpOrDownScore_SHZ));
	return true;

}
bool  SHZClientKernelSink::GetBiBeiData()
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_REQUESTBEIBEI_SHZ);
	return true;
}
//
bool SHZClientKernelSink::MaLiSendMessage()
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_MALISTART_SHZ);
	return true;
}
//������ע
bool SHZClientKernelSink::ChangeCellScore()
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	kernel->SendSocketData(MDM_GF_GAME_SHZ, SUB_C_ADDSCORE_SHZ);
	return true;
}
int SHZClientKernelSink::SwitchChairID(int m_Currchairid)
{
	SHZIClientKernel * kernel = SHZIClientKernel::get();
	int changChairID = m_Currchairid;
	switch(kernel->GetMeChairID())
	{
	case 0:
	case 1:
	case 2:
		{
			switch(m_Currchairid)
			{
			case 0:return 0;
			case 1:return 1;
			case 2:return 2;
			case 3:return 3;
			case 4:return 4;
			case 5:return 5;
			}

		}
	case 3:
	case 4:
	case 5:
		{
			switch(m_Currchairid)
			{
			case 0:return 3;
			case 1:return 4;
			case 2:return 5;
			case 3:return 0;
			case 4:return 1;
			case 5:return 2;
			}

		}
	}

	return changChairID;  
}
