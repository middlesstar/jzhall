#ifndef _SharkGameScene_H_
#define _SharkGameScene_H_

#include "cocos2d.h"
#include "MainView.h"

class SharkGameScene : public cocos2d::Scene
{
public:
	virtual bool init();
	//开始游戏
	void gameStart(int leftTime);
	//开奖
	void openPrize(int leftTime);
	//等待下一局游戏
	void waitNextGame(int leftTime);


	virtual void onEnter() override;

	virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;

	CREATE_FUNC(SharkGameScene);
	//心跳包响应
	virtual void OnHreatPak();
protected:
	/*-------------------游戏中-------------------------*/
	void gameRunning(float t);

	void bridge(float t);


	//void PlayAnimalMusic(enumSpecialRs eRs, enumAnimalType eType, enumAnimalColor eColor);

	void waitUpdate(float t);

	//网络检测
	void CheckNet(float t);
	void checkNetupdate(float t);
	//发送心跳包
	void updateSendHreat(float t);

protected:
	/*---------------------开奖-------------------------*/
	//普通开奖
	void generalResult(int index);
	//金鲨鱼开奖
	void sharkResult(int index);
	//银鲨鱼开奖
	void silverResult(int index);

	//彩鲨鱼开奖
	void colorShark(int index);
	bool AddGold(int g);

	//判断是哪一个开奖
	void judgeResult(int index, bool colorsharFlag);
	//停止开奖
	void stopResult(int index, bool colorsharFlag);
	//显示开奖总结界面
	void showResult();
	//显示获取的倍率
	void showRate();
	//隐藏倍率显示的界面
	void hideRate();
	//判断动物
	void judgeAnimal(int index);
	
	//计算获得的元宝或者船票的个数
	int countNum(int index);

   //判断开奖界面显示哪一个动物
	void  showAnimal();

	//消除开奖界面的动物
	void stopResAnimal();

	//开奖界面结束切换到押分界面
	void changeView();

	//进入游戏主界面
	void goMainScene(float t);

	//开奖界面最后一个环节
	void battleResult();
protected:
	void comeFromBack(Ref* ref);               //从后台返回前端
	void comeToBack(Ref* ref);                 //从前端进入后台
	void startTimer(float time);                         //开启定时器
	
protected:
	int						m_nowIndex;				//当前游戏环节
	int                     m_nowState;             //当前游戏状态
	bool					m_bIslive;				//网络连接是否连接上检测
	int                     timeDelay;              //进入界面的时间延迟

	time_t                  outTime;                //进入后台时的时间戳

	int                     resultIndex;            //开奖类型索引

public:
	static  MainView*      m_mainView;
	static SharkGameScene*	This;
	Node*                   chat;



};

#endif // _SharkGameScene_H_