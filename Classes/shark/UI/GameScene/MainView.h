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
//Ѻ��
#define  STAKERIGHT          "bet_right"
//�û��б�
#define		BET_LEFT       	 "bet_left"
#define     WAITNODE         "wait_node"
//��¼��
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
	//�رս���ʱ�ĶϿ����ӵĴ���
	void netDisconnect();
public:
	//�Ӻ�̨����ǰ��ʱ����
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

	//�ȴ��������
	WaitNext               waitNextManager;

	//��¼�����
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

	bool					touchAble;					//�Ƿ����Ѻע
};

#endif // _MainView_H_
