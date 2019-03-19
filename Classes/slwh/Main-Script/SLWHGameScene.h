#ifndef __SLWHGameScene_H__
#define __SLWHGameScene_H__

#include "cocos2d.h"
#include "UILayerMain.h"
#include "Model3DLayer.h"
#include "SpeRes/SpeD3Y.h"
#include "SpeRes/SpeD4X.h"
#include "SpeRes/SpeCJ.h"
#include "SpeRes/XYCJManager.h"
#include "SpeRes/SpeDMG2.h"
USING_NS_CC;

class SLWHGameScene : public cocos2d::Scene
{

public:
	//time_t t;
	static Model3DLayer*	m_model3DLayer;
	static UILayerMain*		m_uiLayer;
	static SLWHGameScene*	This;
protected:
	int                     timeDelay;              //进入界面的时间延迟
	int						m_nowIndex;				//当前游戏环节
	enumServerState			m_nowState;				//当前游戏状态
	bool					m_bIslive;				//网络连接是否连接上检测

	Node*                   chat;                   //放聊天框的节点
	Node*                   cj_root;                //抽奖节点
	Node*                   xycj_root;              //幸运抽奖根节点

	time_t                  outTime;                //进入后台时的时间戳
public:
	SLWHGameScene();

	virtual ~SLWHGameScene();

	virtual bool init();
	//开始游戏
	void StartGames(int leftTime);
	//等待游戏
	void WaitGames(int lefttime);
	//开奖
	void OpenGames();

	virtual void onEnter() override;

	virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;

	//心跳包响应
	virtual void OnHreatPak();

	CREATE_FUNC(SLWHGameScene);
protected:
	//关闭进程时的断开连接的处理
	void netDisconnect();

	/*--------------从后台进入前端的处理------------*/
	void comeFromBack(Ref* ref);               //从后台返回前端
	void comeToBack(Ref* ref);                 //从前端进入后台
	void startTimer(float time);               //开启定时器

	void goMainScene(float t);                 //进入主界面，创建层
	void loadroomData();                       //加载房间配置信息
protected:
	//游戏流程跑起来
	void GameRunning(float t);
	//定时器过渡桥梁
	void Bridge(float t);
	//播放开奖音乐
	void PlayAnimalMusic(enumSpecialRs eRs, enumAnimalType eType, enumAnimalColor eColor);
	//检查状态
	void waitGameUpdate(float t);
	//延迟弹出押注
	void delaypopJet(float t);
	//网络检测
	void CheckLive(float t);
	//心跳包检查定时器
	void checkLiveupdate(float t);
	//发送心跳包
	void updateSendHreat(float t);
protected:
	//特殊开奖
	//D3Y
	SpeD3Y					m_speD3Y;
	//D4X
	SpeD4X					m_speD4X;
	//CJ
	SpeCJ					m_speCJ;
	//XYCJ
	XYCJManager				m_speXYCJ;
	//DMG
	SpeDMG2					m_speDmg;


protected:
	bool AddGold(int g);

	bool AddYuanbao(int g);					//增加元宝
		
	bool AddTicket(int g);					//增加船票

	void initSpe();							//初始化特殊开奖
	//开奖逻辑
	void Normal(int index);					//普通开奖

	void D3Y(int index);					//大三元

	void D4X(int index);					//大四喜

	void CJ(int index);						//彩金

	void XYCJ(int index);					//幸运抽奖

	void DMG(int index);					//大满贯

	void _4LF(int index);					//4连发

	//void a(Node* n);
};

#endif

