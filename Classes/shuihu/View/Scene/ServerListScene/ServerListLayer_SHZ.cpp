#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"
#include "ServerListLayer_SHZ.h"
#include "df/Timer.h"
#include "shuihu/View/Scene/GameScene/ClientKernelSink_SHZ.h"
#include "shuihu/View/ui/UIComment_SHZ.h"
#include "shuihu/View/Scene/GetHouseIndex_SHZ.h"
#include "shuihu/View/ShuiHuNotice.h"
#include "LanguageConfig.h"
#include "HallGameScene.h"
#include "Player/Player.h"
#include "Window/shop/WindowShop.h"
USING_NS_CC;
USING_NS_CC_EXT;
//////////////////////////////////////////////////////////////////////////
#define PLIST_PATH_SHZ				"selectroom/serverlistscene.plist"
#define TEXTURE_PATH_SHZ			"selectroom/serverlistscene.png"

#define IMG_LISTSCENE_SERVER_0	"server_0.png"
#define IMG_LISTSCENE_SERVER_1	"server_1.png"
#define IMG_LISTSCENE_SERVER_2	"server_2.png"
#define IMG_LISTSCENE_SERVER_3	"server_3.png"


const int  gLocalScore_SHZ[] = {1000,100000,1000000};

//////////////////////////////////////////////////////////////////////////
//��������
bool ServerSort_SHZ(HallGameServerItem* pGameServerItem,HallGameServerItem* pGameServerItem2)
{
	if (pGameServerItem2->m_GameServer.wSortID<pGameServerItem->m_GameServer.wSortID)
	{
		return false;
	}

	return true;
}
void split_SHZ(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}



//////////////////////////////////////////////////////////////////////////
SHZServerListLayer* SHZServerListLayer::create(int mode)
{
	
	SHZServerListLayer* layer = new SHZServerListLayer();
	if (layer && layer->init(mode))
	{
		layer->autorelease();
		return layer;
	}

	delete layer;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
SHZServerListLayer::SHZServerListLayer()
{
}

SHZServerListLayer::~SHZServerListLayer()
{
	HallServerListData::shared()->SetServerListDataSink(0);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(PLIST_PATH_SHZ);
	Director::getInstance()->getTextureCache()->removeTextureForKey(TEXTURE_PATH_SHZ);
}

bool SHZServerListLayer::init(int mode)
{
	miRefCount = 0;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH_SHZ);

		m_ibtIndx=0;
		
		mMode = mode;
		float cx = kRevolutionWidth_SHZ / 2;
		float cy = kRevolutionHeight_SHZ / 2;
		
		float detaSpace = 10.0f;
		float width = (kRevolutionWidth_SHZ - detaSpace * 5) / 4;
		float detaLabelX = width * 0.38;
		float detaLabelY = 68.0f;
		float xScale = 253 / width;
		if (mode==1)
		{
			mServers[0] = createButton_SHZ("server_0.png", 0, 0, this, cccontrol_selector(SHZServerListLayer::onServerClick));
			mServers[0]->setTag(0);
			mServers[0]->setPosition(Vec2(width / 2 + detaSpace, 370));
			mServers[0]->setScaleX(xScale);
			addChild(mServers[0]);  
			/*----------- �޸�bysl----------*/

			mServers[1] = createButton_SHZ("server_1.png", 0, 0, this, cccontrol_selector(SHZServerListLayer::onServerClick));
			mServers[1]->setTag(1);
			mServers[1]->setPosition(Vec2(width * 1.5 + detaSpace * 2, 370)); 
			mServers[1]->setScaleX(xScale);
			addChild(mServers[1]);  


			mServers[2] = createButton_SHZ("server_2.png", 0, 0, this, cccontrol_selector(SHZServerListLayer::onServerClick));
			mServers[2]->setTag(2);
			mServers[2]->setPosition(Vec2(width * 2.5 + detaSpace * 3, 370)); 
			mServers[2]->setScaleX(xScale);
			addChild(mServers[2]); 
			

			mServers[3] = createButton_SHZ("server_3.png", 0, 0, this, cccontrol_selector(SHZServerListLayer::onServerClick));
			mServers[3]->setTag(3);
			mServers[3]->setPosition(Vec2(width * 3.5 + detaSpace * 4, 370)); 
			mServers[3]->setScaleX(xScale);
			addChild(mServers[3]);
			
		}   

		
		//m_RoomGameNotice = SHZCRoomGameNotice::create();

		//addChild(m_RoomGameNotice);

		return true;

	} while (0);

	//��������
	

	return false;
}

void SHZServerListLayer::onEnterTransitionDidFinish(){
	CCLayer::onEnterTransitionDidFinish();
	
}

void SHZServerListLayer::onEnter(){
	CCLayer::onEnter();
	//setDatas();

miRefCount++;

//ֻ���IOSƽ̨���ֺ����Ľ���
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    int iCState = CCUserDefault::sharedUserDefault()->getIntegerForKey(ACCOUNT_NETWORK_COMMENT, 0);
//	if(miRefCount > 1 && iCState == 0)
//	{
//		SHZUIComment* uiComment = SHZUIComment::create();
//		addChild(uiComment);
//	}
//#endif
}


// void SHZServerListLayer::setDatas(){
// 
// 
// 	int __gType = DF::shared()->GetGameType();
// 
// 
// 	if (0 == __gType % 2){  //�����
// 	
// 		CGameServerItemMap_SHZ::iterator it = HallServerListData::shared()->GetServerItemMapBegin();
// 		HallGameServerItem* pGameServerItem = 0;
// 
// 		while (pGameServerItem = HallServerListData::shared()->EmunGameServerItem(it)){
// 			addServer(pGameServerItem);
// 		}
// 
// 	}
// }
// 
// bool SHZServerListLayer::addServer(HallGameServerItem* pGameServerItem)
// {
// 	if (DF::shared()->GetGameKindID() != 0 && DF::shared()->GetGameKindID() != pGameServerItem->m_pGameKindItem->m_GameKind.wKindID)
// 	{
// 		return false;
// 	}
// 
// 	std::vector<std::string> elems;
// 	split_SHZ(pGameServerItem->m_GameServer.szServerName, '|', elems);
// 	if (elems.size() < 4){
// 		return false;
// 	}
// 
// 	int iType		= 0;
// 	int iMin		= 0;
// 	int isort		= 0;
// 	sscanf(elems[1].c_str(), "%d", &iType);
// 	sscanf(elems[2].c_str(), "%d", &iMin);
// 	sscanf(elems[3].c_str(), "%d", &isort);
// 
// 	if (iType != 0 && isort != 3)
// 	{	
// 		pGameServerItem->m_iScoreLimit = iMin;
// 		pGameServerItem->m_GameServer.wSortID = isort;
// 		mServerList[isort].push_back(pGameServerItem);
// 	}
// 
// 	return true;
// }
// 
// bool SHZServerListLayer::removeServer(HallGameServerItem* pGameServerItem)
// {
// 	return false;
// }
// 
// void SHZServerListLayer::updateServer(HallGameServerItem* pGameServerItem)
// {
// 
// }

//////////////////////////////////////////////////////////////////////////
// ��ť�ص�

//����1���
void SHZServerListLayer::onServerClick(cocos2d::CCObject* pSender, cocos2d::extension::Control::EventType e)
{
	Control* ctr = (Control*)pSender;
	int index = ctr->getTag();   //�õ����������

	//������ȫ�̽��Ľ���ͽ����ж�

	if (3 == index)
	{
		mUIQuanPan = SHZUIQuanPan::create();
		mUIQuanPan->SetOpenServerItem(this);
		mUIQuanPan->setPosition(kRevolutionWidth_SHZ / 2, kRevolutionHeight_SHZ / 2);
		
		this->addChild(mUIQuanPan,100);

		return;  //ֱ�ӷ���
	}

	if (!platformIsConnectToInternet_SHZ()){
		popup_SHZ(a_u8("�������"), a_u8("�޷����ӵ���Ϸ������!"));
		return;
	}

	bool __state = onOpenServerItem(index);
}

//������������Ϸ�ں�
bool SHZServerListLayer::CreateKernel(int tag){

	SHZIClientKernel* kernel = SHZIClientKernel::create();

	kernel->SetClientKernelSink(&gClientKernelSink_SHZ);

	gClientKernelSink_SHZ.SetupGameClient();

	return true;
}


//��������
bool SHZServerListLayer::onOpenServerItem(int index)
{
	//�˳���Ϸ
	if (SHZIServerItem::get())
		SHZIServerItem::get()->IntermitConnect(true);

	HallGameServerItem* pGameServerItem = HallServerListData::shared()->getGameServerInfo(KIND_ID_SHZ, index);
	if (!pGameServerItem)
	{
		//�˴���ӷ�������ά��
		ShuiHuNotice::openNotice(GETSTRING(1401));
		return false;
	}
	//�˴��жϽ��뷿��ĸ������� 


	switch (index)
	{
	case 0:
	{
			  if (Player::getInstance()->getLogonData().lUserScore < pGameServerItem->m_iScoreLimit)
			  {
				  //log("-----------------%s-----------------", a_u8("����̫����ȥ��ֵ"));
				  ShuiHuNotice::openNotice(StringUtils::format(GETSTRING(1407).c_str(), pGameServerItem->m_iScoreLimit), [this](Ref*)
				  {
					  auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
					  rechargeNode->setPosition(Vec2(0, 0));
					  this->addChild(rechargeNode, 100);

					  //selectFlag = true;
				  });
				  return false;
			  }
			  else
			  {
				  SHZGetHouseIndex::getInstance()->setIndex(pGameServerItem->GetSortID());
			  }
			  break;
	}

	case 1:
	{
			  if (Player::getInstance()->getLogonData().lUserScore < pGameServerItem->m_iScoreLimit)
			  {
				  ShuiHuNotice::openNotice(StringUtils::format(GETSTRING(1407).c_str(), pGameServerItem->m_iScoreLimit), [this](Ref*)
				  {
					  auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
					  rechargeNode->setPosition(Vec2(0, 0));
					  this->addChild(rechargeNode, 100);

					  //selectFlag = true;
				  });
				  return false;
			  }
			  else
			  {
				  SHZGetHouseIndex::getInstance()->setIndex(pGameServerItem->GetSortID());
			  }
			  break;
	}

	case 2:
	{
			  if (Player::getInstance()->getHallData().dwVipLevel < pGameServerItem->m_vipLimit)
			  {
				  log("-----------------%s-----------------", a_u8("������VIP"));
				  ShuiHuNotice::openNotice(GETSTRING(1408), [this](Ref*)
				  {
					  //�˴��赯����ʾ����ת���˽�vip����
					  auto pWindow = (Node*)HallGameScene::createWindow(EWT_Shop);
					  pWindow->setPosition(Vec2(0, 0));
					  this->addChild(pWindow, 100);
					  auto pShop = (WindowShop*)pWindow;
					  pShop->updateWindow(0);

					  //selectFlag = true;
				  });
				  return false;
			  }
			  else
			  {
				  if (Player::getInstance()->getLogonData().lUserScore < pGameServerItem->m_iScoreLimit)
				  {
					  log("-----------------%s-----------------", a_u8("��������"));
					  ShuiHuNotice::openNotice(StringUtils::format(GETSTRING(1407).c_str(), pGameServerItem->m_iScoreLimit), [this](Ref*)
					  {
						  auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
						  rechargeNode->setPosition(Vec2(0, 0));
						  this->addChild(rechargeNode, 100);

						  //selectFlag = true;
					  });
					  return false;
				  }
				  else
				  {
					  SHZGetHouseIndex::getInstance()->setIndex(pGameServerItem->GetSortID());
				  }

			  }
			  break;
	}

	default:
		break;
	}


	
	G_NOTIFICATION_SHZ("CONNECT_SERVER", SHZMTData::create((unsigned int*)pGameServerItem)); // MTNotifcation.cpp��ע��

	return true;
}




//////////////////////////////////////////////////////////////////////////
// IServerListDataSink
//���֪ͨ
void SHZServerListLayer::OnGameItemFinish()
{
	//setDatas();
} 

//���֪ͨ
void SHZServerListLayer::OnGameKindFinish(uint16 wKindID)
{

}

//����֪ͨ
void SHZServerListLayer::OnGameItemUpdateFinish()
{
}

//����֪ͨ
void SHZServerListLayer::OnGameItemInsert(SHZCGameListItem * pGameListItem)
{
	//��������
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:		//��Ϸ����
		{
			break;
		}
	case ItemGenre_Kind:		//��Ϸ����
		{
			break;
		}
	case ItemGenre_Node:		//��Ϸ�ڵ�
		{
			break;
		}
	case ItemGenre_Server:		//��Ϸ����
		{
			//if (addServer((HallGameServerItem*)pGameListItem))
			//{
			//}
			break;
		}
	case ItemGenre_Page:	//��������
		{
			break;
		}
	}
}

//����֪ͨ
void SHZServerListLayer::OnGameItemUpdate(SHZCGameListItem * pGameListItem)
{
	//��������
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:		//��Ϸ����
		{
			break;
		}
	case ItemGenre_Kind:		//��Ϸ����
		{
			break;
		}
	case ItemGenre_Node:		//��Ϸ�ڵ�
		{
			break;
		}
	case ItemGenre_Server:		//��Ϸ����
		{
			//updateServer((HallGameServerItem*)pGameListItem);
			//������ͼ
			break;
		}
	case ItemGenre_Page:	//��������
		{
			break;
		}
	}
}

//ɾ��֪ͨ
void SHZServerListLayer::OnGameItemDelete(SHZCGameListItem * pGameListItem)
{
	//��������
	switch (pGameListItem->GetItemGenre())
	{
	case ItemGenre_Type:		//��Ϸ����
		{
			break;
		}
	case ItemGenre_Kind:		//��Ϸ����
		{
			break;
		}
	case ItemGenre_Node:		//��Ϸ�ڵ�
		{
			break;
		}
	case ItemGenre_Server:		//��Ϸ����
		{
// 			if (removeServer((HallGameServerItem*)pGameListItem))
// 			{
// 			}
			break;
		}
	case ItemGenre_Page:	//��������
		{
			break;
		}
	}
}
