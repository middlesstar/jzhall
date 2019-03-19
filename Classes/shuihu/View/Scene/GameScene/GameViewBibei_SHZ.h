#ifndef SHZ_GameViewBiBei_H__
#define SHZ_GameViewBiBei_H__

#include "cocos2d.h"
#include "shuihu/GameHeader_SHZ.h"
#include "cocos-ext.h"
#include "GameStruct_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "TouchLayer_SHZ.h"
class SHZCGameViewBibei : public cocos2d::CCNode, public SHZITouchSink
{

	//资源
public:

	//背景图片
	cocos2d::CCSprite*				m_BgImgeTexture;

	cocos2d::CCNode*				m_NodeBoss;				//boss

	//cocos2d::CCNode*				m_NodeLeft;				//左边

	//cocos2d::CCNode*				m_NodeRight;			//右边

	cocos2d::CCSprite*				m_TaiSaiViewTexture;	//大小背景

	cocos2d::CCSprite*				m_sai1Texture;			//骰子1
	cocos2d::CCSprite*				m_sai2Texture;			//骰子2

	cocos2d::CCSprite*				m_yuanbao1Texture;		//元宝1
	cocos2d::CCSprite*				m_yuanbao2Texture;		//元宝2

	//记录
	cocos2d::CCSpriteBatchNode*		m_RecordTexture;		//记录


	cocos2d::CCSprite*				m_winscoreViewTexture;				//赢分标志
	cocos2d::CCSprite*				m_bibeiViewTexture;					//比倍
	cocos2d::CCSprite*				m_defenViewTexture;					//得分
	//变量
public:

	bool							m_bStartAnim;		//开始动画

	bool							m_bwinLostAnim;		//输赢动画

	//塞子点数,塞子一
	int								 m_S1;
	//
	int								 m_S2;

	//比倍分数
	int								 m_bibeiScore;
	//比倍获得分数
	int								 m_bibeiWinScore;
	//
	int								 m_bibeiWinTempScore;

	//玩家上的分
	int								 m_CreditScore;

	//是否可以下分
	bool							 m_CanHaveMoney;

	int								 m_ixiazhuPos;		//下注位置

	int								 m_TempDwonscore;

	int								 m_TempCCscore;
	//
	int								 m_TempTTscore;

	int								 m_iDoubleCount;	//加倍次数

	bool							m_bxiazhu;


	cocos2d::CCRect					m_rcdaxiao[3];			//压大小区域

	//接口
public:

	SHZCGameSceneInterface*			m_GameSceneInterface;

	//按钮
public:

	cocos2d::extension::ControlButton*					m_btBig;				//买大
	cocos2d::extension::ControlButton*					m_btGetScore;			//得分
	cocos2d::extension::ControlButton*					m_btSmall;				//加注
	//cocos2d::extension::ControlButton*					m_btbibei;				//比倍
	cocos2d::extension::ControlButton*					m_bttie;				//买和

	cocos2d::extension::ControlButton*					m_btaddbei;				//加倍
	cocos2d::extension::ControlButton*					m_btcurbei;				//减倍

	//字体
public:

	//cocos2d::CCLabelBMFont					*m_lbScore1;						//界面分数显示
	//cocos2d::CCLabelBMFont					*m_lbScore2;						//界面分数显示
	cocos2d::CCLabelTTF						*m_lbScore3;						//界面分数显示

	cocos2d::CCLabelBMFont					*m_lbwinScore;						//赢分

public:

	SHZTouchLayer*						touch_layer_;						//鼠标接口

public:
	SHZCGameViewBibei(void);
	~SHZCGameViewBibei(void);

	bool init();

	static SHZCGameViewBibei* create();

	//创建按钮
	void  CreatedGameBt();

	//粒子特效
	void playEff();

	//设置接口
	void  SetGameSceneInterface(SHZCGameSceneInterface* face){ m_GameSceneInterface = face; }

	//动画函数
public:

	//开始动画
	void ShowStartAnim(cocos2d::CCNode* node, void* data);


	//庄家开奖动画
	void ShowLotteryAnim(cocos2d::CCNode* node, void* data);

	//庄家和赌徒开奖后动画
	void ShowWinLostAnim(cocos2d::CCNode* node, void* data);


	//继承函数
private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);


	//功能函数
public:
	//触摸
	void touchEvent();
	//得分
	void GetWinScore(int gettype);

	//获取赢分
	void GetWinScore(int winscore, int gametype);

	//比倍
	virtual bool BiBei(int bibeitype, int bibeimax);

	//比倍结果
	void SetBiBei(int winscore, int S1, int S2);

	//开始动作
	void StartAnim(int iCreditScore, int ibibeiscore);

	//比倍数组
	void CopyBiBeiData(int ibibeidata, int ipos);

	//加倍
	void SetAddbei(int iaddScore);

	//比倍赢分
	void ShowBibeiwin(bool bshow);

	/*-------------todosl---------------*/
	//创建金币
	void createCoin();
	//金币特效
	void createCoinFly(Node* sp, int m);
	//调用winscore
	void getWinScore();

	//移除金币
	void removeCoinFly(Node* obj);

	//金币放大效果
	void coinChange(Node* obj);

	/*-------------------------------*/

	//动画
public:

	//创建动作(循环播放动画)
	void ForeverAction(CCNode* node, const char *strName, int iCount, cocos2d::CCPoint ptImage, int iFirst = 0);

	//创建动作(播放动画一次)
	void SequenceAction(const char *strImage, cocos2d::SEL_CallFuncND selector, int iCount, cocos2d::CCPoint ptImage, int itag);


	//定时器
public:

	//比倍得分
	void OnbibeiordefenTimer(float dt);

	//获取赢分
	void OnDwonScoreTimer(float dt);

	//显示塞子
	void OnShowSaizi(float dt);

private:
	bool mbGetScore;
	cocos2d::EventListenerTouchOneByOne* listener;//鼠标接口
};


#endif
