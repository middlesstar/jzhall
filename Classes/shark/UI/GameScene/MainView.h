#ifndef _MainView_H_
#define _MainView_H_

#include "cocos2d.h"
#include "battle/Battle.h"
#include "result/AnimalBgChange.h"
#include "result/GoldResult.h"
#include "result/SilverResult.h"
#include "result/ColorResult.h"
#include "result/ResultBoardManager.h"
#include "stake/StakeManager.h"
#include "shark/UI/SharkTimeCount.h"
#include "shark/UI/SharkMenu.h"
#include "PlayerList/PlayerListManager.h"
#include "result/RateShow.h"
#include "result/SharkShow.h"
#include "result/SilverShow.h"
#include "shark/Module/fish/FishLayer.h"

#include "shark/Module/AnimalPlay.h"
#include "shark/Module/AnimalCamera.h"

#include "shark/UI/WaitNext.h"

#include "shark/UI/GameScene/record/RecordManager.h"

#define   BATTLERIGHT        "battle_right"
#define   GENELRALBATTLE     "genelBattle"
#define   GOLDSHARK          "winGoldShark"
#define   SILVERSHARK        "winSilverShark"
#define   COLORSHARK         "colorGold"
//押分
#define  STAKERIGHT          "bet_right"
//用户列表
#define		BET_LEFT       	 "bet_left"
#define     WAITNODE         "wait_node"
//记录板
#define     RECORDBOARD      "resultShow"


class MainView : public cocos2d::Layer
{
public:

	MainView();
	~MainView();

	virtual bool init();
	CREATE_FUNC(MainView);

protected:
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
public:
	void showFishLayer();
	void clearFishLayer();
public:
	//关闭进程时的断开连接的处理
	void netDisconnect();
public:
	//从后台进入前端时调用
	void comeBack(Ref* ref);

public: 
	Battle                  battleManager;
	AnimalBgChange          animalBgManager;
	GoldResult              goldManager;
	SilverResult            silverManager;
	ColorResult             colorManager;
	ResultBoardManager      resultBoardManager;
	StakeManager            stakeManager;
	SharkTimeCount          sharkTimeCount;
	SharkMenu               menuManager;
	PlayerListManager       m_playerListManager;
	RateShow                rateManager;
	SharkShow               sharkRotaManager;
	SilverShow              silverRotaManager;

	AnimalPlay              animalManager;
	AnimalCamera            cameraManager;

	//等待界面管理
	WaitNext               waitNextManager;

	//记录板管理
	RecordManager          recordManager;

public:
	Node*       battle_rootNode;
	Node*       stake_rootNode;
	Node*       leftBoard_rootNode;
	Node*       result_rootNode;
	Node*       menu_rootNode;
	Node*       wait_rootNode;
	Node*       fishManager;

	FishLayer*      fish;

	//cocostudio::timeline::ActionTimeline* uiAction;

	bool					touchAble;					//是否可以押注
};

#endif // _MainView_H_
