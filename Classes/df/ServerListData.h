#ifndef SHZ_SERVER_LIST_DATA_HEAD_FILE
#define SHZ_SERVER_LIST_DATA_HEAD_FILE

#pragma once

#include "df/DF.h"
#include "types.h"
#include "Struct.h"
#include <vector>
#include <map>
//////////////////////////////////////////////////////////////////////////////////
//枚举定义

//子项类型
enum SHZ_enItemGenre
{
	ItemGenre_Type,					//游戏种类
	ItemGenre_Kind,					//游戏类型
	ItemGenre_Node,					//游戏节点
	ItemGenre_Page,					//游戏页面
	ItemGenre_Server,				//游戏房间
	ItemGenre_Inside,				//游戏内部
};

//房间状态
enum SHZ_enServerStatus
{
	ServerStatus_Normal,			//正常状态
	ServerStatus_Entrance,			//正在使用
	ServerStatus_EverEntrance,		//曾经进入
};

//////////////////////////////////////////////////////////////////////////////////

//类说明
class SHZCGameListItem;
class SHZCGameTypeItem;
class SHZCGameKindItem;
class SHZCGameNodeItem;
class SHZCGamePageItem;
class HallGameServerItem;
class SHZCGameInsideItem;

//数组说明
typedef std::vector<SHZCGameListItem *> CGameListItemArray_SHZ;

//索引说明


typedef std::map<uint16, SHZCGameTypeItem* > CGameTypeItemMap_SHZ;
typedef std::map<uint16, SHZCGameKindItem* > CGameKindItemMap_SHZ;
typedef std::map<uint16, SHZCGameNodeItem* > CGameNodeItemMap_SHZ;
typedef std::map<uint16, SHZCGamePageItem* > CGamePageItemMap_SHZ;
typedef std::map<uint16, HallGameServerItem* > CGameServerItemMap_SHZ;



//////////////////////////////////////////////////////////////////////////////////

//列表接口
struct SHZIServerListDataSink
{
	//状态通知
public:
	//完成通知
	virtual void OnGameItemFinish() = 0;
	//完成通知
	virtual void OnGameKindFinish(uint16 wKindID) = 0;
	//更新通知
	virtual void OnGameItemUpdateFinish() = 0;

	//更新通知
public:
	//插入通知
	virtual void OnGameItemInsert(SHZCGameListItem * pGameListItem) = 0;
	//更新通知
	virtual void OnGameItemUpdate(SHZCGameListItem * pGameListItem) = 0;
	//删除通知
	virtual void OnGameItemDelete(SHZCGameListItem * pGameListItem) = 0;
};

//////////////////////////////////////////////////////////////////////////////////

//列表子项
class SHZCGameListItem
{
	//友元定义
	friend class HallServerListData;

	//属性数据
protected:
	SHZ_enItemGenre						m_ItemGenre;						//子项类型
	SHZCGameListItem *					m_pParentListItem;					//父项子项

	//函数定义
protected:
	//构造函数
	SHZCGameListItem(SHZ_enItemGenre ItemGenre);
	//析构函数
	virtual ~SHZCGameListItem();

	//功能函数
public:
	//获取类型
	SHZ_enItemGenre GetItemGenre() { return m_ItemGenre; }
	//获取父项
	SHZCGameListItem * GetParentListItem() { return m_pParentListItem; }

	//重载函数
public:
	//排列索引
	virtual uint16 GetSortID() = 0;
};

//////////////////////////////////////////////////////////////////////////////////

//种类结构
class SHZCGameTypeItem : public SHZCGameListItem
{
	//属性数据
public:
	tagGameType						m_GameType;							//种类信息

	//函数定义
public:
	//构造函数
	SHZCGameTypeItem();
	//析构函数
	virtual ~SHZCGameTypeItem();

	//重载函数
public:
	//排列索引
	virtual uint16 GetSortID() { return m_GameType.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//类型结构
class SHZCGameKindItem : public SHZCGameListItem
{
	//属性数据
public:
	tagGameKind						m_GameKind;							//类型信息

	//更新变量
public:
	bool							m_bUpdateItem;						//更新标志
	uint32							m_dwUpdateTime;						//更新时间

	//扩展数据
public:
	uint32							m_dwProcessVersion;					//游戏版本

	//函数定义
public:
	//构造函数
	SHZCGameKindItem();
	//析构函数
	virtual ~SHZCGameKindItem();

	//重载函数
public:
	//排列索引
	virtual uint16 GetSortID() { return m_GameKind.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//节点结构
class SHZCGameNodeItem : public SHZCGameListItem
{
	//属性数据
public:
	tagGameNode						m_GameNode;							//节点信息

	//函数定义
public:
	//构造函数
	SHZCGameNodeItem();
	//析构函数
	virtual ~SHZCGameNodeItem();

	//重载函数
public:
	//排列索引
	virtual uint16 GetSortID() { return m_GameNode.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//房间结构
class HallGameServerItem : public SHZCGameListItem
{
	//属性数据
public:
	tagSHZ_GameServer					m_GameServer;						//房间信息

	//扩展数据
public:
	SHZ_enServerStatus					m_ServerStatus;						//房间状态
	tagSLWH_GameServer					m_GameRoomInfo_SLWH;				//SLWH扩展信息
	tagShark_GameServer                 m_GameRoomInfo_SHARK;               //Shark扩展信息
	//房间限制
public:

	int								m_iScoreLimit;						//进入限制
	int                             m_vipLimit;                         //vip限制

	//辅助变量
public:
	SHZCGameKindItem *					m_pGameKindItem;					//游戏类型

	//函数定义
public:
	//构造函数
	HallGameServerItem();
	//析构函数
	virtual ~HallGameServerItem();

	//重载函数
public:
	//排列索引
	virtual uint16 GetSortID() { return m_GameServer.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//定制结构
class SHZCGamePageItem : public SHZCGameListItem
{
	//属性数据
public:
	tagGamePage						m_GamePage;							//定制信息

	//函数定义
public:
	//构造函数
	SHZCGamePageItem();
	//析构函数
	virtual ~SHZCGamePageItem();

	//重载函数
public:
	//排列索引
	virtual uint16 GetSortID() { return m_GamePage.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//内部结构
class SHZCGameInsideItem : public SHZCGameListItem
{
	//属性数据
public:
	uint32							m_dwInsideID;						//内部 ID

	//函数定义
public:
	//构造函数
	SHZCGameInsideItem();
	//析构函数
	virtual ~SHZCGameInsideItem();

	//重载函数
public:
	//排列索引
	virtual uint16 GetSortID() { return 0L; }
};

//////////////////////////////////////////////////////////////////////////////////

//列表服务
class HallServerListData
{
	//静态函数
public:
	//获取对象
	static HallServerListData* shared();
	static void purge();

	//索引变量
protected:
	CGameTypeItemMap_SHZ				m_GameTypeItemMap;					//种类索引
	CGameKindItemMap_SHZ				m_GameKindItemMap;					//类型索引
	CGameNodeItemMap_SHZ				m_GameNodeItemMap;					//节点索引
	CGamePageItemMap_SHZ				m_GamePageItemMap;					//页面索引
	CGameServerItemMap_SHZ				m_GameServerItemMap;				//房间索引
	
	//内核变量
protected:
	CGameListItemArray_SHZ				m_GameListItemWait;					//等待子项
	SHZIServerListDataSink *			m_pIServerListDataSink;				//回调接口

public:
	uint32                           m_dwAllOnLineCount;                 //总在线人数

	//函数定义
private:
	//构造函数
	HallServerListData();
	//析构函数
	virtual ~HallServerListData();

	//配置函数
public:
	//设置接口
	void SetServerListDataSink(SHZIServerListDataSink * pIServerListDataSink);

	//状态通知
public:
	//完成通知
	void OnEventListFinish();
	//完成通知
	void OnEventKindFinish(uint16 wKindID);
	//下载通知
	void OnEventDownLoadFinish(uint16 wKindID);

	//人数函数
public:
	//设置人数
	void SetKindOnLineCount(uint16 wKindID, uint32 dwOnLineCount);
	//设置人数
	void SetServerOnLineCount(uint16 wServerID, uint32 dwOnLineCount);
	//设置人数
	void SetServerOnLineFinish();

	//插入函数
public:
	//插入种类
	bool InsertGameType(tagGameType * pGameType);
	//插入类型
	bool InsertGameKind(tagGameKind * pGameKind);
	//插入节点
	bool InsertGameNode(tagGameNode * pGameNode);
	//插入定制
	bool InsertGamePage(tagGamePage * pGamePage);
	//插入房间
	bool InsertGameServer(tagSHZ_GameServer * pGameServer);

	//删除函数
public:
	//删除种类
	bool DeleteGameType(uint16 wTypeID);
	//删除类型
	bool DeleteGameKind(uint16 wKindID);
	//删除节点
	bool DeleteGameNode(uint16 wNodeID);
	//删除定制
	bool DeleteGamePage(uint16 wPageID);
	//删除房间
	bool DeleteGameServer(uint16 wServerID);

	//枚举函数
public:
	//枚举种类
	SHZCGameTypeItem * EmunGameTypeItem(CGameTypeItemMap_SHZ::iterator &it);
	//枚举类型
	SHZCGameKindItem * EmunGameKindItem(CGameKindItemMap_SHZ::iterator &it);
	//枚举节点
	SHZCGameNodeItem * EmunGameNodeItem(CGameNodeItemMap_SHZ::iterator &it);
	//枚举定制
	SHZCGamePageItem * EmunGamePageItem(CGamePageItemMap_SHZ::iterator &it);
	//枚举房间
	CGameServerItemMap_SHZ::iterator GetServerItemMapBegin() { return m_GameServerItemMap.begin(); }
	HallGameServerItem * EmunGameServerItem(CGameServerItemMap_SHZ::iterator &it);

	//查找函数
public:
	//查找种类
	SHZCGameTypeItem * SearchGameType(uint16 wTypeID);
	//查找类型
	SHZCGameKindItem * SearchGameKind(uint16 wKindID);
	//查找节点
	SHZCGameNodeItem * SearchGameNode(uint16 wNodeID);
	//查找定制
	SHZCGamePageItem * SearchGamePage(uint16 wPageID);
	//查找房间
	HallGameServerItem * SearchGameServer(uint16 wServerID);

	//数目函数
public:
	//种类数目
	uint32 GetGameTypeCount() { return (uint32)m_GameTypeItemMap.size(); }
	//类型数目
	uint32 GetGameKindCount() { return (uint32)m_GameKindItemMap.size(); }
	//节点数目
	uint32 GetGameNodeCount() { return (uint32)m_GameNodeItemMap.size(); }
	//定制数目
	uint32 GetGamePageCount() { return (uint32)m_GamePageItemMap.size(); }
	//房间数目
	uint32 GetGameServerCount() { return (uint32)m_GameServerItemMap.size(); }


	//获取总在线人数
	uint32 GetAllOnLineCount();//{return m_dwAllOnLineCount;}

	//新加方法
public:
	HallGameServerItem* getGameServerInfo(int nKindID,int nIndex);

	void clearServerInfo();			//清除列表信息
	void clearGameServer();			//清除房间信息
};

//////////////////////////////////////////////////////////////////////////////////

#endif
