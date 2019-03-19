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

	//设置服务器列表
	//void setDatas();
	bool CreateKernel(int tag);   //根据房间初始化客户端内核

public:
	virtual void onEnterTransitionDidFinish();

	virtual void onEnter();

	//按钮回调
public:
	//房间1点击
	void onServerClick(cocos2d::CCObject* pSender, cocos2d::extension::Control::EventType e);

	virtual bool onOpenServerItem(int index = 0);

private:
	//bool addServer(HallGameServerItem* pGameServerItem);
	//bool removeServer(HallGameServerItem* pGameServerItem);
	//void updateServer(HallGameServerItem* pGameServerItem);

	//////////////////////////////////////////////////////////////////////////

	// IServerListDataSink
public:
	//状态通知
public:
	//完成通知
	virtual void OnGameItemFinish();
	//完成通知
	virtual void OnGameKindFinish(uint16 wKindID);
	//更新通知
	virtual void OnGameItemUpdateFinish();

	//更新通知
public:
	//插入通知
	virtual void OnGameItemInsert(SHZCGameListItem * pGameListItem);
	//更新通知
	virtual void OnGameItemUpdate(SHZCGameListItem * pGameListItem);
	//删除通知
	virtual void OnGameItemDelete(SHZCGameListItem * pGameListItem);

private:
	typedef std::vector<HallGameServerItem*>	SERVER_LIST;
	int	mMode;
	enum {SERVERS=4};
	cocos2d::extension::ControlButton*	mServers[SERVERS];
	
	//SERVER_LIST								mServerList[SERVERS];

	/*----------修改bysl-----------*/
	SHZUIQuanPan*                               mUIQuanPan;

	int										m_ibtIndx;				//按钮标识

	//SHZCRoomGameNotice							*m_RoomGameNotice;		//公告



	int  miRefCount;  //界面的参考计数
};

#endif // _ServerListLayer_H_