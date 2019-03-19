#ifndef SHZ_ServerListLayer_H_
#define SHZ_ServerListLayer_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "shuihu/View/ui/UIQuanPan_SHZ.h"
#include "IOpenServerItem_SHZ.h"
#include "RoomGameNotice_SHZ.h"






class SHZServerListLayer
	: public cocos2d::CCLayer
	, SHZIServerListDataSink, SHZIOpenServerItem
{
public:
	static SHZServerListLayer* create(int mode);

private:
	SHZServerListLayer();
	~SHZServerListLayer();
	bool init(int mode);

	//���÷������б�
	//void setDatas();
	bool CreateKernel(int tag);   //���ݷ����ʼ���ͻ����ں�

public:
	virtual void onEnterTransitionDidFinish();

	virtual void onEnter();

	//��ť�ص�
public:
	//����1���
	void onServerClick(cocos2d::CCObject* pSender, cocos2d::extension::Control::EventType e);

	virtual bool onOpenServerItem(int index = 0);

private:
	//bool addServer(HallGameServerItem* pGameServerItem);
	//bool removeServer(HallGameServerItem* pGameServerItem);
	//void updateServer(HallGameServerItem* pGameServerItem);

	//////////////////////////////////////////////////////////////////////////

	// IServerListDataSink
public:
	//״̬֪ͨ
public:
	//���֪ͨ
	virtual void OnGameItemFinish();
	//���֪ͨ
	virtual void OnGameKindFinish(uint16 wKindID);
	//����֪ͨ
	virtual void OnGameItemUpdateFinish();

	//����֪ͨ
public:
	//����֪ͨ
	virtual void OnGameItemInsert(SHZCGameListItem * pGameListItem);
	//����֪ͨ
	virtual void OnGameItemUpdate(SHZCGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual void OnGameItemDelete(SHZCGameListItem * pGameListItem);

private:
	typedef std::vector<HallGameServerItem*>	SERVER_LIST;
	int	mMode;
	enum {SERVERS=4};
	cocos2d::extension::ControlButton*	mServers[SERVERS];
	
	//SERVER_LIST								mServerList[SERVERS];

	/*----------�޸�bysl-----------*/
	SHZUIQuanPan*                               mUIQuanPan;

	int										m_ibtIndx;				//��ť��ʶ

	//SHZCRoomGameNotice							*m_RoomGameNotice;		//����



	int  miRefCount;  //����Ĳο�����
};

#endif // _ServerListLayer_H_