#ifndef SHZ_SERVER_LIST_DATA_HEAD_FILE
#define SHZ_SERVER_LIST_DATA_HEAD_FILE

#pragma once

#include "df/DF.h"
#include "types.h"
#include "Struct.h"
#include <vector>
#include <map>
//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��������
enum SHZ_enItemGenre
{
	ItemGenre_Type,					//��Ϸ����
	ItemGenre_Kind,					//��Ϸ����
	ItemGenre_Node,					//��Ϸ�ڵ�
	ItemGenre_Page,					//��Ϸҳ��
	ItemGenre_Server,				//��Ϸ����
	ItemGenre_Inside,				//��Ϸ�ڲ�
};

//����״̬
enum SHZ_enServerStatus
{
	ServerStatus_Normal,			//����״̬
	ServerStatus_Entrance,			//����ʹ��
	ServerStatus_EverEntrance,		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class SHZCGameListItem;
class SHZCGameTypeItem;
class SHZCGameKindItem;
class SHZCGameNodeItem;
class SHZCGamePageItem;
class HallGameServerItem;
class SHZCGameInsideItem;

//����˵��
typedef std::vector<SHZCGameListItem *> CGameListItemArray_SHZ;

//����˵��


typedef std::map<uint16, SHZCGameTypeItem* > CGameTypeItemMap_SHZ;
typedef std::map<uint16, SHZCGameKindItem* > CGameKindItemMap_SHZ;
typedef std::map<uint16, SHZCGameNodeItem* > CGameNodeItemMap_SHZ;
typedef std::map<uint16, SHZCGamePageItem* > CGamePageItemMap_SHZ;
typedef std::map<uint16, HallGameServerItem* > CGameServerItemMap_SHZ;



//////////////////////////////////////////////////////////////////////////////////

//�б�ӿ�
struct SHZIServerListDataSink
{
	//״̬֪ͨ
public:
	//���֪ͨ
	virtual void OnGameItemFinish() = 0;
	//���֪ͨ
	virtual void OnGameKindFinish(uint16 wKindID) = 0;
	//����֪ͨ
	virtual void OnGameItemUpdateFinish() = 0;

	//����֪ͨ
public:
	//����֪ͨ
	virtual void OnGameItemInsert(SHZCGameListItem * pGameListItem) = 0;
	//����֪ͨ
	virtual void OnGameItemUpdate(SHZCGameListItem * pGameListItem) = 0;
	//ɾ��֪ͨ
	virtual void OnGameItemDelete(SHZCGameListItem * pGameListItem) = 0;
};

//////////////////////////////////////////////////////////////////////////////////

//�б�����
class SHZCGameListItem
{
	//��Ԫ����
	friend class HallServerListData;

	//��������
protected:
	SHZ_enItemGenre						m_ItemGenre;						//��������
	SHZCGameListItem *					m_pParentListItem;					//��������

	//��������
protected:
	//���캯��
	SHZCGameListItem(SHZ_enItemGenre ItemGenre);
	//��������
	virtual ~SHZCGameListItem();

	//���ܺ���
public:
	//��ȡ����
	SHZ_enItemGenre GetItemGenre() { return m_ItemGenre; }
	//��ȡ����
	SHZCGameListItem * GetParentListItem() { return m_pParentListItem; }

	//���غ���
public:
	//��������
	virtual uint16 GetSortID() = 0;
};

//////////////////////////////////////////////////////////////////////////////////

//����ṹ
class SHZCGameTypeItem : public SHZCGameListItem
{
	//��������
public:
	tagGameType						m_GameType;							//������Ϣ

	//��������
public:
	//���캯��
	SHZCGameTypeItem();
	//��������
	virtual ~SHZCGameTypeItem();

	//���غ���
public:
	//��������
	virtual uint16 GetSortID() { return m_GameType.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//���ͽṹ
class SHZCGameKindItem : public SHZCGameListItem
{
	//��������
public:
	tagGameKind						m_GameKind;							//������Ϣ

	//���±���
public:
	bool							m_bUpdateItem;						//���±�־
	uint32							m_dwUpdateTime;						//����ʱ��

	//��չ����
public:
	uint32							m_dwProcessVersion;					//��Ϸ�汾

	//��������
public:
	//���캯��
	SHZCGameKindItem();
	//��������
	virtual ~SHZCGameKindItem();

	//���غ���
public:
	//��������
	virtual uint16 GetSortID() { return m_GameKind.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//�ڵ�ṹ
class SHZCGameNodeItem : public SHZCGameListItem
{
	//��������
public:
	tagGameNode						m_GameNode;							//�ڵ���Ϣ

	//��������
public:
	//���캯��
	SHZCGameNodeItem();
	//��������
	virtual ~SHZCGameNodeItem();

	//���غ���
public:
	//��������
	virtual uint16 GetSortID() { return m_GameNode.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//����ṹ
class HallGameServerItem : public SHZCGameListItem
{
	//��������
public:
	tagSHZ_GameServer					m_GameServer;						//������Ϣ

	//��չ����
public:
	SHZ_enServerStatus					m_ServerStatus;						//����״̬
	tagSLWH_GameServer					m_GameRoomInfo_SLWH;				//SLWH��չ��Ϣ
	tagShark_GameServer                 m_GameRoomInfo_SHARK;               //Shark��չ��Ϣ
	//��������
public:

	int								m_iScoreLimit;						//��������
	int                             m_vipLimit;                         //vip����

	//��������
public:
	SHZCGameKindItem *					m_pGameKindItem;					//��Ϸ����

	//��������
public:
	//���캯��
	HallGameServerItem();
	//��������
	virtual ~HallGameServerItem();

	//���غ���
public:
	//��������
	virtual uint16 GetSortID() { return m_GameServer.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//���ƽṹ
class SHZCGamePageItem : public SHZCGameListItem
{
	//��������
public:
	tagGamePage						m_GamePage;							//������Ϣ

	//��������
public:
	//���캯��
	SHZCGamePageItem();
	//��������
	virtual ~SHZCGamePageItem();

	//���غ���
public:
	//��������
	virtual uint16 GetSortID() { return m_GamePage.wSortID; }
};

//////////////////////////////////////////////////////////////////////////////////

//�ڲ��ṹ
class SHZCGameInsideItem : public SHZCGameListItem
{
	//��������
public:
	uint32							m_dwInsideID;						//�ڲ� ID

	//��������
public:
	//���캯��
	SHZCGameInsideItem();
	//��������
	virtual ~SHZCGameInsideItem();

	//���غ���
public:
	//��������
	virtual uint16 GetSortID() { return 0L; }
};

//////////////////////////////////////////////////////////////////////////////////

//�б����
class HallServerListData
{
	//��̬����
public:
	//��ȡ����
	static HallServerListData* shared();
	static void purge();

	//��������
protected:
	CGameTypeItemMap_SHZ				m_GameTypeItemMap;					//��������
	CGameKindItemMap_SHZ				m_GameKindItemMap;					//��������
	CGameNodeItemMap_SHZ				m_GameNodeItemMap;					//�ڵ�����
	CGamePageItemMap_SHZ				m_GamePageItemMap;					//ҳ������
	CGameServerItemMap_SHZ				m_GameServerItemMap;				//��������
	
	//�ں˱���
protected:
	CGameListItemArray_SHZ				m_GameListItemWait;					//�ȴ�����
	SHZIServerListDataSink *			m_pIServerListDataSink;				//�ص��ӿ�

public:
	uint32                           m_dwAllOnLineCount;                 //����������

	//��������
private:
	//���캯��
	HallServerListData();
	//��������
	virtual ~HallServerListData();

	//���ú���
public:
	//���ýӿ�
	void SetServerListDataSink(SHZIServerListDataSink * pIServerListDataSink);

	//״̬֪ͨ
public:
	//���֪ͨ
	void OnEventListFinish();
	//���֪ͨ
	void OnEventKindFinish(uint16 wKindID);
	//����֪ͨ
	void OnEventDownLoadFinish(uint16 wKindID);

	//��������
public:
	//��������
	void SetKindOnLineCount(uint16 wKindID, uint32 dwOnLineCount);
	//��������
	void SetServerOnLineCount(uint16 wServerID, uint32 dwOnLineCount);
	//��������
	void SetServerOnLineFinish();

	//���뺯��
public:
	//��������
	bool InsertGameType(tagGameType * pGameType);
	//��������
	bool InsertGameKind(tagGameKind * pGameKind);
	//����ڵ�
	bool InsertGameNode(tagGameNode * pGameNode);
	//���붨��
	bool InsertGamePage(tagGamePage * pGamePage);
	//���뷿��
	bool InsertGameServer(tagSHZ_GameServer * pGameServer);

	//ɾ������
public:
	//ɾ������
	bool DeleteGameType(uint16 wTypeID);
	//ɾ������
	bool DeleteGameKind(uint16 wKindID);
	//ɾ���ڵ�
	bool DeleteGameNode(uint16 wNodeID);
	//ɾ������
	bool DeleteGamePage(uint16 wPageID);
	//ɾ������
	bool DeleteGameServer(uint16 wServerID);

	//ö�ٺ���
public:
	//ö������
	SHZCGameTypeItem * EmunGameTypeItem(CGameTypeItemMap_SHZ::iterator &it);
	//ö������
	SHZCGameKindItem * EmunGameKindItem(CGameKindItemMap_SHZ::iterator &it);
	//ö�ٽڵ�
	SHZCGameNodeItem * EmunGameNodeItem(CGameNodeItemMap_SHZ::iterator &it);
	//ö�ٶ���
	SHZCGamePageItem * EmunGamePageItem(CGamePageItemMap_SHZ::iterator &it);
	//ö�ٷ���
	CGameServerItemMap_SHZ::iterator GetServerItemMapBegin() { return m_GameServerItemMap.begin(); }
	HallGameServerItem * EmunGameServerItem(CGameServerItemMap_SHZ::iterator &it);

	//���Һ���
public:
	//��������
	SHZCGameTypeItem * SearchGameType(uint16 wTypeID);
	//��������
	SHZCGameKindItem * SearchGameKind(uint16 wKindID);
	//���ҽڵ�
	SHZCGameNodeItem * SearchGameNode(uint16 wNodeID);
	//���Ҷ���
	SHZCGamePageItem * SearchGamePage(uint16 wPageID);
	//���ҷ���
	HallGameServerItem * SearchGameServer(uint16 wServerID);

	//��Ŀ����
public:
	//������Ŀ
	uint32 GetGameTypeCount() { return (uint32)m_GameTypeItemMap.size(); }
	//������Ŀ
	uint32 GetGameKindCount() { return (uint32)m_GameKindItemMap.size(); }
	//�ڵ���Ŀ
	uint32 GetGameNodeCount() { return (uint32)m_GameNodeItemMap.size(); }
	//������Ŀ
	uint32 GetGamePageCount() { return (uint32)m_GamePageItemMap.size(); }
	//������Ŀ
	uint32 GetGameServerCount() { return (uint32)m_GameServerItemMap.size(); }


	//��ȡ����������
	uint32 GetAllOnLineCount();//{return m_dwAllOnLineCount;}

	//�¼ӷ���
public:
	HallGameServerItem* getGameServerInfo(int nKindID,int nIndex);

	void clearServerInfo();			//����б���Ϣ
	void clearGameServer();			//���������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

#endif
