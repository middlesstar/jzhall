#include "ServerListData.h"
#include <string>
#include "df/types.h"
#include "Util/GameUtil.h"

//构造函数
SHZCGameListItem::SHZCGameListItem(SHZ_enItemGenre ItemGenre)
{
	//属性数据
	m_ItemGenre = ItemGenre;
	m_pParentListItem = 0;

	return;
}

//析构函数
SHZCGameListItem::~SHZCGameListItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCGameTypeItem::SHZCGameTypeItem() : SHZCGameListItem(ItemGenre_Type)
{
	//属性数据
	zeromemory(&m_GameType, sizeof(m_GameType));
}

//析构函数
SHZCGameTypeItem::~SHZCGameTypeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCGameKindItem::SHZCGameKindItem() : SHZCGameListItem(ItemGenre_Kind)
{
	//更新变量
	m_dwUpdateTime = 0;
	m_bUpdateItem = false;

	//扩展数据
	m_dwProcessVersion = 0L;

	//属性数据
	zeromemory(&m_GameKind, sizeof(m_GameKind));

	return;
}

//析构函数
SHZCGameKindItem::~SHZCGameKindItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCGameNodeItem::SHZCGameNodeItem() : SHZCGameListItem(ItemGenre_Node)
{
	//属性数据
	zeromemory(&m_GameNode, sizeof(m_GameNode));
}

//析构函数
SHZCGameNodeItem::~SHZCGameNodeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
HallGameServerItem::HallGameServerItem() : SHZCGameListItem(ItemGenre_Server)
{
	//辅助变量
	m_pGameKindItem = 0;

	//扩展数据
	m_ServerStatus = ServerStatus_Normal;

	//属性数据
	zeromemory(&m_GameServer, sizeof(m_GameServer));
}

//析构函数
HallGameServerItem::~HallGameServerItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCGamePageItem::SHZCGamePageItem() : SHZCGameListItem(ItemGenre_Page)
{
	//属性数据
	zeromemory(&m_GamePage, sizeof(m_GamePage));
}

//析构函数
SHZCGamePageItem::~SHZCGamePageItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCGameInsideItem::SHZCGameInsideItem() : SHZCGameListItem(ItemGenre_Inside)
{
}

//析构函数
SHZCGameInsideItem::~SHZCGameInsideItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//静态变量

HallServerListData * __gServerListData_SHZ = 0;				//列表数据


HallServerListData* HallServerListData::shared()
{
	if (__gServerListData_SHZ == 0)
		__gServerListData_SHZ = new HallServerListData();
	return __gServerListData_SHZ;
}

void HallServerListData::purge()
{
	if (__gServerListData_SHZ)
		delete __gServerListData_SHZ;
	__gServerListData_SHZ = 0;
}

//////////////////////////////////////////////////////////////////////////////////
//构造函数
HallServerListData::HallServerListData()
{
	//接口变量
	m_pIServerListDataSink = 0;

	m_dwAllOnLineCount = 0;

	return;
}

//析构函数
HallServerListData::~HallServerListData()
{
	//变量定义
	uint16 wKey = 0;

	clearServerInfo();

	return;
}

//设置接口
void HallServerListData::SetServerListDataSink(SHZIServerListDataSink * pIServerListDataSink)
{
	//设置变量
	m_pIServerListDataSink = pIServerListDataSink;

	return;
}

//完成通知
void HallServerListData::OnEventListFinish()
{

	if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemFinish();

	return;
}

//完成通知
void HallServerListData::OnEventKindFinish(uint16 wKindID)
{

	if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameKindFinish(wKindID);

	return;
}

//下载通知
void HallServerListData::OnEventDownLoadFinish(uint16 wKindID)
{
	CGameKindItemMap_SHZ::iterator it = m_GameKindItemMap.find(wKindID);

	if (it == m_GameKindItemMap.end())
		return;

	//变量定义
	SHZCGameKindItem * pGameKindItem = it->second;

	//更新信息
	if (pGameKindItem != 0)
	{
		//获取版本
		//tchar szProcessPath[MAX_PATH]={0};
		//GetProcessPathByProcessFullName(pGameKindItem->m_GameKind.szProcessName, szProcessPath, MAX_PATH);
		//CWHService::GetModuleVersion(szProcessPath,pGameKindItem->m_dwProcessVersion);

		//更新子项
		//ASSERT(m_pIServerListDataSink!=0);
		if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//设置人数
void HallServerListData::SetKindOnLineCount(uint16 wKindID, uint32 dwOnLineCount)
{
	//搜索类型
	SHZCGameKindItem * pGameKindItem = SearchGameKind(wKindID);

	//设置人数
	if (pGameKindItem != 0)
	{
		//设置变量
		pGameKindItem->m_GameKind.dwOnLineCount = dwOnLineCount;

		//事件通知
		//ASSERT(m_pIServerListDataSink!=0);
		if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//设置人数
void HallServerListData::SetServerOnLineCount(uint16 wServerID, uint32 dwOnLineCount)
{
	//搜索房间
	HallGameServerItem * pGameServerItem = SearchGameServer(wServerID);

	//设置人数
	if (pGameServerItem != 0)
	{
		//设置变量
		m_dwAllOnLineCount -= pGameServerItem->m_GameServer.dwOnLineCount;
		m_dwAllOnLineCount += dwOnLineCount;

		//设置变量
		pGameServerItem->m_GameServer.dwOnLineCount = dwOnLineCount;

		//事件通知
		//ASSERT(m_pIServerListDataSink!=0);
		if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);

		//查找类型
		SHZCGameKindItem * pGameKindItem = SearchGameKind(pGameServerItem->m_GameServer.wKindID);
		if (pGameKindItem)
		{
			//变量定义
			uint32 dwGameKindOnline = 0;
			CGameServerItemMap_SHZ::iterator it = m_GameServerItemMap.end();
			do
			{
				//获取房间
				HallGameServerItem * pGameServerItem2 = EmunGameServerItem(it);

				//设置房间
				if ((pGameServerItem2 != 0) && (pGameServerItem2->m_GameServer.wKindID == pGameServerItem->m_GameServer.wKindID))
				{
					dwGameKindOnline += pGameServerItem2->m_GameServer.dwOnLineCount;
				}

			} while (it != m_GameServerItemMap.end());

			//设置变量
			pGameKindItem->m_GameKind.dwOnLineCount = dwGameKindOnline;

			//事件通知
			//ASSERT(m_pIServerListDataSink!=0);
			if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
		}
	}

	return;
}

//设置人数
void HallServerListData::SetServerOnLineFinish()
{
	//事件通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemUpdateFinish();
}

//插入种类
bool HallServerListData::InsertGameType(tagGameType * pGameType)
{
	//效验参数
	//ASSERT(pGameType!=0);
	if (pGameType == 0) return false;


	//变量定义
	SHZCGameTypeItem * pGameTypeItem = 0;
	bool bInsert = false;
	CGameTypeItemMap_SHZ::iterator it = m_GameTypeItemMap.find(pGameType->wTypeID);

	if (it == m_GameTypeItemMap.end())
	{
		pGameTypeItem = new SHZCGameTypeItem;
		bInsert = true;
	}
	else
	{
		pGameTypeItem = it->second;
	}

	if (pGameTypeItem == 0) return false;

	//设置数据
	memcpy(&pGameTypeItem->m_GameType, pGameType, sizeof(tagGameType));

	//寻找父项
	if (pGameType->wJoinID != 0)
	{
		//寻找父项
		pGameTypeItem->m_pParentListItem = SearchGameType(pGameType->wJoinID);

		//待决判断
		if (pGameTypeItem->m_pParentListItem == 0)
		{
			m_GameListItemWait.push_back(pGameTypeItem);
			return true;
		}
	}

	//插入处理
	if (bInsert == true)
	{
		//设置索引
		m_GameTypeItemMap[pGameType->wTypeID] = pGameTypeItem;

		//界面更新
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemInsert(pGameTypeItem);
	}
	else
	{
		//界面更新
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemUpdate(pGameTypeItem);
	}

	return true;
}

//插入类型
bool HallServerListData::InsertGameKind(tagGameKind * pGameKind)
{
	//效验参数
	//ASSERT(pGameKind!=0);
	if (pGameKind == 0) return false;

	//变量定义
	SHZCGameKindItem * pGameKindItem = 0;
	bool bInsert = false;
	CGameKindItemMap_SHZ::iterator it = m_GameKindItemMap.find(pGameKind->wKindID);

	if (it == m_GameKindItemMap.end())
	{
		pGameKindItem = new SHZCGameKindItem;
		bInsert = true;
	}
	else
	{
		pGameKindItem = it->second;
	}

	if (pGameKindItem == 0) return false;

	//获取版本
	if (bInsert == true)
	{
		//tchar szProcessPath[MAX_PATH]={0};
		//GetProcessPathByProcessFullName(pGameKind->szProcessName, szProcessPath, MAX_PATH);
		//CWHService::GetModuleVersion(szProcessPath,pGameKindItem->m_dwProcessVersion);
	}

	//设置数据
	memcpy(&pGameKindItem->m_GameKind, pGameKind, sizeof(tagGameKind));

	//寻找父项
	if (pGameKind->wJoinID != 0)
	{
		pGameKindItem->m_pParentListItem = SearchGameKind(pGameKind->wJoinID);
	}
	else
	{
		pGameKindItem->m_pParentListItem = SearchGameType(pGameKind->wTypeID);
	}

	//待决判断
	if (pGameKindItem->m_pParentListItem == 0)
	{
		m_GameListItemWait.push_back(pGameKindItem);
		return true;
	}

	//插入处理
	if (bInsert == true)
	{
		//设置索引
		m_GameKindItemMap[pGameKind->wKindID] = pGameKindItem;

		//插入子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemInsert(pGameKindItem);
	}
	else
	{
		//更新子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return true;
}

//插入节点
bool HallServerListData::InsertGameNode(tagGameNode * pGameNode)
{
	//效验参数
	//ASSERT(pGameNode!=0);
	if (pGameNode == 0) return false;

	//变量定义
	SHZCGameNodeItem * pGameNodeItem = 0;
	bool bInsert = false;
	CGameNodeItemMap_SHZ::iterator it = m_GameNodeItemMap.find(pGameNode->wNodeID);

	if (it == m_GameNodeItemMap.end())
	{
		pGameNodeItem = new SHZCGameNodeItem;
		bInsert = true;
	}
	else
	{
		pGameNodeItem = it->second;
	}

	if (pGameNodeItem == 0) return false;

	//设置数据
	memcpy(&pGameNodeItem->m_GameNode, pGameNode, sizeof(tagGameNode));

	//寻找父项
	if (pGameNode->wJoinID != 0)
	{
		pGameNodeItem->m_pParentListItem = SearchGameNode(pGameNode->wJoinID);
	}
	else
	{
		pGameNodeItem->m_pParentListItem = SearchGameKind(pGameNode->wKindID);
	}

	//待决判断
	if (pGameNodeItem->m_pParentListItem == 0)
	{
		m_GameListItemWait.push_back(pGameNodeItem);
		return true;
	}

	//插入处理
	if (bInsert == true)
	{
		//设置索引
		m_GameNodeItemMap[pGameNode->wNodeID] = pGameNodeItem;

		//插入子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemInsert(pGameNodeItem);
	}
	else
	{
		//更新子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemUpdate(pGameNodeItem);
	}

	return true;
}

//插入定制
bool HallServerListData::InsertGamePage(tagGamePage * pGamePage)
{
	//效验参数
	//ASSERT(pGamePage!=0);
	if (pGamePage == 0) return false;

	//变量定义
	SHZCGamePageItem * pGamePageItem = 0;
	bool bInsert = false;
	CGamePageItemMap_SHZ::iterator it = m_GamePageItemMap.find(pGamePage->wPageID);

	if (it == m_GamePageItemMap.end())
	{
		pGamePageItem = new SHZCGamePageItem;
		bInsert = true;
	}
	else
	{
		pGamePageItem = it->second;
	}

	if (pGamePageItem == 0) return false;


	//设置数据
	memcpy(&pGamePageItem->m_GamePage, pGamePage, sizeof(tagGamePage));

	//寻找父项
	if ((pGamePage->wNodeID != 0) || (pGamePage->wKindID != 0))
	{
		//寻找父项
		if (pGamePage->wNodeID != 0) pGamePageItem->m_pParentListItem = SearchGameNode(pGamePage->wNodeID);
		else if (pGamePage->wKindID != 0) pGamePageItem->m_pParentListItem = SearchGameKind(pGamePage->wKindID);

		//待决判断
		if (pGamePageItem->m_pParentListItem == 0)
		{
			m_GameListItemWait.push_back(pGamePageItem);
			return true;
		}
	}

	//插入处理
	if (bInsert == true)
	{
		//设置索引
		m_GamePageItemMap[pGamePage->wPageID] = pGamePageItem;

		//插入子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemInsert(pGamePageItem);
	}
	else
	{
		//更新子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemUpdate(pGamePageItem);
	}

	return true;
}

//插入房间
bool HallServerListData::InsertGameServer(tagSHZ_GameServer * pGameServer)
{
	//效验参数
	//ASSERT(pGameServer!=0);
	if (pGameServer == 0) return false;

	//变量定义
	HallGameServerItem * pGameServerItem = 0;
	bool bInsert = false;
	CGameServerItemMap_SHZ::iterator it = m_GameServerItemMap.find(pGameServer->wServerID);

	if (it == m_GameServerItemMap.end())
	{
		pGameServerItem = new HallGameServerItem;
		bInsert = true;
	}
	else
	{
		pGameServerItem = it->second;
	}

	if (pGameServerItem == 0) return false;

	//设置数据
	memcpy(&pGameServerItem->m_GameServer, pGameServer, sizeof(tagSHZ_GameServer));
	m_dwAllOnLineCount += pGameServer->dwOnLineCount;

	//寻找父项
	if (pGameServer->wNodeID != 0)
	{
		pGameServerItem->m_pGameKindItem = SearchGameKind(pGameServer->wKindID);
		pGameServerItem->m_pParentListItem = SearchGameNode(pGameServer->wNodeID);
	}
	else
	{
		pGameServerItem->m_pGameKindItem = SearchGameKind(pGameServer->wKindID);
		pGameServerItem->m_pParentListItem = SearchGameKind(pGameServer->wKindID);
	}

	//查找类型
	if (bInsert == true && pGameServerItem->m_pGameKindItem)
	{
		//变量定义
		uint32 dwGameKindOnline = 0;
		uint32 dwGameKindFullCount = 0;
		CGameServerItemMap_SHZ::iterator it = m_GameServerItemMap.end();
		do
		{
			//获取房间
			HallGameServerItem * pGameServerItem2 = EmunGameServerItem(it);

			//设置房间
			if ((pGameServerItem2 != 0) && (pGameServerItem2->m_GameServer.wKindID == pGameServerItem->m_GameServer.wKindID))
			{
				dwGameKindOnline += pGameServerItem2->m_GameServer.dwOnLineCount;
				dwGameKindFullCount += pGameServerItem2->m_GameServer.dwFullCount;
			}

		} while (it != m_GameServerItemMap.end());

		//设置变量
		pGameServerItem->m_pGameKindItem->m_GameKind.dwOnLineCount = dwGameKindOnline;
		pGameServerItem->m_pGameKindItem->m_GameKind.dwFullCount = dwGameKindFullCount;

		//事件通知
		//ASSERT(m_pIServerListDataSink!=0);
		if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem->m_pGameKindItem);
	}

	////待决判断
	//if (pGameServerItem->m_pParentListItem==0) 
	//{
	//	m_GameListItemWait.push_back(pGameServerItem);
	//	return true;
	//}

	//插入处理
	if (bInsert == true)
	{
		//设置索引
		m_GameServerItemMap[pGameServer->wServerID] = pGameServerItem;

		//插入子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemInsert(pGameServerItem);
	}
	else
	{
		//更新子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return true;
}

//删除种类
bool HallServerListData::DeleteGameType(uint16 wTypeID)
{
	//查找种类
	CGameTypeItemMap_SHZ::iterator it = m_GameTypeItemMap.find(wTypeID);
	if (it == m_GameTypeItemMap.end())
	{
		return false;
	}

	SHZCGameTypeItem * pGameTypeItem = it->second;

	//删除通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemDelete(pGameTypeItem);

	//重置数据
	SafeDelete(pGameTypeItem);

	//删除数据
	m_GameTypeItemMap.erase(it);

	return true;
}

//删除类型
bool HallServerListData::DeleteGameKind(uint16 wKindID)
{
	//查找类型
	CGameKindItemMap_SHZ::iterator it = m_GameKindItemMap.find(wKindID);
	if (it == m_GameKindItemMap.end())
	{
		return false;
	}

	SHZCGameKindItem * pGameKindItem = it->second;

	//删除通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemDelete(pGameKindItem);

	//删除数据
	SafeDelete(pGameKindItem);

	//删除数据
	m_GameKindItemMap.erase(it);

	return true;
}

//删除节点
bool HallServerListData::DeleteGameNode(uint16 wNodeID)
{
	//查找节点
	CGameNodeItemMap_SHZ::iterator it = m_GameNodeItemMap.find(wNodeID);
	if (it == m_GameNodeItemMap.end())
	{
		return false;
	}

	SHZCGameNodeItem * pGameNodeItem = it->second;

	//删除通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemDelete(pGameNodeItem);

	//删除数据
	SafeDelete(pGameNodeItem);

	//删除数据
	m_GameNodeItemMap.erase(it);

	return false;
}

//删除房间
bool HallServerListData::DeleteGameServer(uint16 wServerID)
{
	//查找房间
	CGameServerItemMap_SHZ::iterator it = m_GameServerItemMap.find(wServerID);
	if (it == m_GameServerItemMap.end())
	{
		return false;
	}

	HallGameServerItem * pGameServerItem = it->second;

	//设置数据
	m_dwAllOnLineCount -= pGameServerItem->m_GameServer.dwOnLineCount;

	//删除通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemDelete(pGameServerItem);

	//删除数据
	SafeDelete(pGameServerItem);

	//删除数据
	m_GameServerItemMap.erase(it);

	return false;
}

//删除定制
bool HallServerListData::DeleteGamePage(uint16 wPageID)
{
	//查找类型
	CGamePageItemMap_SHZ::iterator it = m_GamePageItemMap.find(wPageID);
	if (it == m_GamePageItemMap.end())
	{
		return false;
	}

	SHZCGamePageItem * pGamePageItem = it->second;

	//删除通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink != 0) m_pIServerListDataSink->OnGameItemDelete(pGamePageItem);

	//删除数据
	SafeDelete(pGamePageItem);

	//删除数据
	m_GamePageItemMap.erase(it);

	return true;
}

//枚举种类
SHZCGameTypeItem * HallServerListData::EmunGameTypeItem(CGameTypeItemMap_SHZ::iterator &it)
{
	//调整参数
	if (it == m_GameTypeItemMap.end()) it = m_GameTypeItemMap.begin();
	if (it == m_GameTypeItemMap.end()) return 0;

	//查找房间
	SHZCGameTypeItem * pGameTypeItem = it->second;
	it++;
	return pGameTypeItem;
}

//枚举类型
SHZCGameKindItem * HallServerListData::EmunGameKindItem(CGameKindItemMap_SHZ::iterator &it)
{
	//调整参数
	if (it == m_GameKindItemMap.end()) it = m_GameKindItemMap.begin();
	if (it == m_GameKindItemMap.end()) return 0;

	//查找房间
	SHZCGameKindItem * pGameKindItem = it->second;
	it++;
	return pGameKindItem;
}

//枚举节点
SHZCGameNodeItem * HallServerListData::EmunGameNodeItem(CGameNodeItemMap_SHZ::iterator &it)
{
	//调整参数
	if (it == m_GameNodeItemMap.end()) it = m_GameNodeItemMap.begin();
	if (it == m_GameNodeItemMap.end()) return 0;

	//查找节点
	SHZCGameNodeItem * pGameNodeItem = it->second;
	it++;
	return pGameNodeItem;
}

//枚举定制
SHZCGamePageItem * HallServerListData::EmunGamePageItem(CGamePageItemMap_SHZ::iterator &it)
{
	//调整参数
	if (it == m_GamePageItemMap.end()) return 0;

	//查找房间
	SHZCGamePageItem * pGamePageItem = it->second;
	it++;
	return pGamePageItem;
}

//枚举房间
HallGameServerItem * HallServerListData::EmunGameServerItem(CGameServerItemMap_SHZ::iterator &it)
{
	//调整参数
	if (it == m_GameServerItemMap.end()) return 0;

	//查找房间
	HallGameServerItem * pGameServerItem = it->second;
	it++;
	return pGameServerItem;
}

//查找种类
SHZCGameTypeItem * HallServerListData::SearchGameType(uint16 wTypeID)
{
	CGameTypeItemMap_SHZ::iterator it = m_GameTypeItemMap.find(wTypeID);
	if (it == m_GameTypeItemMap.end())
		return 0;

	return it->second;
}

//查找类型
SHZCGameKindItem * HallServerListData::SearchGameKind(uint16 wKindID)
{
	CGameKindItemMap_SHZ::iterator it = m_GameKindItemMap.find(wKindID);
	if (it == m_GameKindItemMap.end())
		return 0;

	return it->second;
}

//查找节点
SHZCGameNodeItem * HallServerListData::SearchGameNode(uint16 wNodeID)
{
	CGameNodeItemMap_SHZ::iterator it = m_GameNodeItemMap.find(wNodeID);
	if (it == m_GameNodeItemMap.end())
		return 0;

	return it->second;
}

//查找房间
HallGameServerItem * HallServerListData::SearchGameServer(uint16 wServerID)
{
	CGameServerItemMap_SHZ::iterator it = m_GameServerItemMap.find(wServerID);
	if (it == m_GameServerItemMap.end())
		return 0;

	return it->second;
}

//查找定制
SHZCGamePageItem * HallServerListData::SearchGamePage(uint16 wPageID)
{
	CGamePageItemMap_SHZ::iterator it = m_GamePageItemMap.find(wPageID);
	if (it == m_GamePageItemMap.end())
		return 0;

	return it->second;
}

//获取总在线人数
uint32 HallServerListData::GetAllOnLineCount()
{
	//定义变量
	uint32 dwAllOnLineCount = 0;
	CGameKindItemMap_SHZ::iterator it = m_GameKindItemMap.end();

	//遍历
	do
	{
		SHZCGameKindItem *pGameKindItem = EmunGameKindItem(it);
		if (pGameKindItem)
		{
			dwAllOnLineCount += pGameKindItem->m_GameKind.dwOnLineCount;
		}
	} while (it != m_GameKindItemMap.end());

	return dwAllOnLineCount;
}

HallGameServerItem* HallServerListData::getGameServerInfo(int nKindID, int nIndex)
{
	HallGameServerItem* pRet = nullptr;

	if (nKindID != 0)
	{
		CGameServerItemMap_SHZ::iterator it = m_GameServerItemMap.begin();

		std::vector<int> elems;
		while (it != m_GameServerItemMap.end())
		{
			if (it->second->m_GameServer.wKindID == nKindID)
			{
				elems.clear();
				GameUtil::getSplitData(it->second->m_GameServer.szServerName, elems, '|');
				if (elems.size() < 5){
					continue;
				}

				if (elems[1] != 0 && elems[3] == nIndex && elems[3] != 3)
				{	
					//房间分数限制
					it->second->m_GameRoomInfo_SHARK.roomScoreLimit = it->second->m_GameRoomInfo_SLWH.dwScoreLimit = it->second->m_iScoreLimit = elems[2];

					it->second->m_GameServer.wSortID = elems[3];
					//VIP限制
					it->second->m_GameRoomInfo_SHARK.roomVip = it->second->m_GameRoomInfo_SLWH.dwVip = it->second->m_vipLimit = elems[4];

					pRet = it->second;
					break;
				}
			}

			++it;
		}
	}
	
	return pRet;
}

void HallServerListData::clearServerInfo()
{
	//删除种类
	CGameTypeItemMap_SHZ::iterator itTypeItem = m_GameTypeItemMap.begin();
	for (; itTypeItem != m_GameTypeItemMap.end(); ++itTypeItem)
	{
		SafeDelete(itTypeItem->second);
	}
	m_GameTypeItemMap.clear();

	//删除类型
	CGameKindItemMap_SHZ::iterator itKindItem = m_GameKindItemMap.begin();
	for (; itKindItem != m_GameKindItemMap.end(); ++itKindItem)
	{
		SafeDelete(itKindItem->second);
	}
	m_GameKindItemMap.clear();

	//删除节点
	CGameNodeItemMap_SHZ::iterator itNodeItem = m_GameNodeItemMap.begin();
	for (; itNodeItem != m_GameNodeItemMap.end(); ++itNodeItem)
	{
		SafeDelete(itNodeItem->second);
	}
	m_GameNodeItemMap.clear();

	//删除房间
	CGameServerItemMap_SHZ::iterator itServerItem = m_GameServerItemMap.begin();
	for (; itServerItem != m_GameServerItemMap.end(); ++itServerItem)
	{
		SafeDelete(itServerItem->second);
	}
	m_GameServerItemMap.clear();

	//删除定制
	CGamePageItemMap_SHZ::iterator itPageItem = m_GamePageItemMap.begin();
	for (; itPageItem != m_GamePageItemMap.end(); ++itPageItem)
	{
		SafeDelete(itPageItem->second);
	}
	m_GamePageItemMap.clear();

	//删除定制
	CGameListItemArray_SHZ::iterator itListItem = m_GameListItemWait.begin();
	for (; itListItem != m_GameListItemWait.end(); ++itListItem)
	{
		SafeDelete(*itListItem);
	}
	m_GameListItemWait.clear();
}

void HallServerListData::clearGameServer()
{
	//删除房间
	CGameServerItemMap_SHZ::iterator itServerItem = m_GameServerItemMap.begin();
	for (; itServerItem != m_GameServerItemMap.end(); ++itServerItem)
	{
		SafeDelete(itServerItem->second);
	}
	m_GameServerItemMap.clear();
}

//////////////////////////////////////////////////////////////////////////////////
