#ifndef SHZ_GameViewMail_H__
#define SHZ_GameViewMail_H__

#include "cocos2d.h"
#include "shuihu/GameHeader_SHZ.h"
#include "cocos-ext.h"
#include "GameStruct_SHZ.h"
#include "GoldDownLayer_SHZ.h"

class SHZCGameViewMali :
	public cocos2d::CCNode
{

	//资源
public:

	//滚动胶卷
	cocos2d::CCSpriteBatchNode*		m_GunDongImgeTexture;

	//背景图片
	cocos2d::CCSprite*				m_BgImgeTexture;

	cocos2d::CCSprite*				m_TypeImageTexture;		//

	cocos2d::CCNode*				m_TypeNode;

	cocos2d::CCSprite*				m_winscoreViewTexture;				//赢分标志
	cocos2d::CCSprite*				m_bibeiViewTexture;					//比倍
	cocos2d::CCSprite*				m_defenViewTexture;					//得分

	bool							m_bNext;				//

	bool							m_bRoll;				//滚动

	//接口
public:

	SHZCGameSceneInterface*			m_GameSceneInterface;

	//变量
public:

	int                     m_ThreeMaLiCount;
	//
	int                     m_ServerPai;
	//
	int                     m_ThreeInt[4];
	//
	SHZTypeImgStruct           m_typeimgstructthree[4];

	int						m_typeid[4];			//类型图片


	int						m_izhuanQuanID;			//转圈标识

	int                     m_CurrThreePai;

	int                     m_CurrQuanCount;
	//
	bool                    m_ThreeShan;

	int                     m_ThisScore;

	//每次开出牌后的获得分数
	int                     ThisScore;

	//单元积分
	int                     m_CellScore;

	//玩家上的分
	int                     m_CreditScore;

	int                     m_TempDwonscore;

	int                     m_TempCCscore;
	//
	int                     m_TempTTscore;

	int                     m_MaLiSquenceCount;

	bool					m_bzidong;

	cocos2d::CCPoint		m_ptTypeImage[24];

	//按钮
public:

	cocos2d::extension::ControlButton*					m_btGetScore;			//得分
	cocos2d::extension::ControlButton*					m_btbibei;				//比倍
	//cocos2d::extension::ControlButton*					m_btshare;				//分享

	//字体
public:

	cocos2d::CCLabelTTF						*m_lbScore1;						//界面分数显示
	cocos2d::CCLabelTTF						*m_lbScore2;						//界面分数显示
	cocos2d::CCLabelTTF						*m_lbScore3;						//界面分数显示
	cocos2d::CCLabelTTF						*m_lbScore4;						//界面分数显示

	cocos2d::CCLabelBMFont					*m_lbwinScore;						//赢分

public:
	SHZCGameViewMali(void);
	~SHZCGameViewMali(void);


	bool init();

	static SHZCGameViewMali* create();

	//创建按钮
	void  CreatedGameBt();

	void SetGameSceneInterface(SHZCGameSceneInterface* scene){ m_GameSceneInterface = scene; };

	//继承函数
private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);

public:



	//功能函数
public:

	//得分
	void GetWinScore(int gettype);

	//获取赢分
	void GetWinScore(int winscore, int gametype);

	//小玛丽信息
	void SetMaLiInformation(int winscore, int index, int imgtypeid, int gamecount, int tyepindex, bool nowbegin);

	//开始小玛丽
	void StartMali(int icreditscore, int ibibeiScore, int icellscore, bool bzidong);

	//发送玛丽消息
	void MaLiSendMessage();

	//设置滚动
	void SetScrollState(bool bScroll);

	//设置4个类型图
	void SetTypeImg(int iPos);

	//类型动画播放
	void ShowTypeAnim(cocos2d::CCNode* node, void* data);

	//类型缩放
	void TypescalingAnim(cocos2d::CCNode* node, void* data);

	//设置得分动画显示
	void SetGetScore(int iScore, int idefen);

	//得分动画完成
	void GetScoreFinsh();

	//动画
public:

	//创建动作(播放动画一次)
	void SequenceAction(const char *strImage, cocos2d::SEL_CallFuncND selector, int iCount, cocos2d::CCPoint ptImage);

	//定时器
public:

	void OnZhuanQuanTimer(float dt);

	//获取赢分
	void OnDwonScoreTimer(float dt);

	//比倍得分
	void OnbibeiordefenTimer(float dt);


public:
	SHZGoldDownLayer* m_goldDownLayer;

private:
	void ShowGoldDownEffect();
	void TouchStateCheck(float dt);
};

#endif

