#ifndef SHZ_GameView_H__
#define SHZ_GameView_H__

#include "cocos2d.h"
#include "shuihu/GameHeader_SHZ.h"
#include "cocos-ext.h"
#include "GameStruct_SHZ.h"
#include "df/LocalStore.h"
#include "GoldDownLayer_SHZ.h"

class DrawMain;
//主界面
class SHZCGameView0 : public cocos2d::CCNode
{
public:

	//类型图片
	cocos2d::CCNode*				m_TypeImgeTexture;

	//框框
	cocos2d::CCNode*				m_BoxNode;

	//背景图片
	cocos2d::CCSprite*				m_BgImgeTexture;
	//界面边框
	cocos2d::CCSprite*				m_BorderImgeTexture;


	// 时间进度条
	cocos2d::CCProgressTimer*		m_progressBar;

	//类型图中奖边框
	cocos2d::CCSprite*				m_TypeBorderImgeTexture;

	//滚动胶卷
	cocos2d::CCSpriteBatchNode*		m_GunDongImgeTexture;

	//线
	cocos2d::CCSpriteBatchNode*		m_LineTexture;

	//灯
	cocos2d::CCSpriteBatchNode*     m_LightTexture;


	cocos2d::CCSprite*				m_winscoreViewTexture;				//赢分标志
	cocos2d::CCSprite*				m_bibeiViewTexture;					//比倍
	cocos2d::CCSprite*				m_defenViewTexture;					//得分

	cocos2d::CCSprite*				m_tishiTexture;						//自动提示

	cocos2d::Sprite*                    chatLabel;                      //创建输入框背景
	cocos2d::Sprite*                    showBoard;                      //创建显示栏
	cocos2d::Sprite*                    spreadBoard;                    //创建显示栏展开

	//接口
public:

	SHZCGameSceneInterface*			m_GameSceneInterface;

	//分数
public:
	/*-------修改bysl--------*/
	
	//存储抽奖经验数据
	int                            m_ExpScore;
	/*--------------------------*/

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

	//变量
public:

	bool					m_bTwinkle;						//连线闪烁

	bool					m_bPlayTypeAnim;				//动画播放

	//类型图结构
	SHZTypeImgStruct           m_typeimgstruct[15];

	//类型牌
	int                     m_typeid[15];

	bool                    m_LineType[45];

	int						m_iCurLine;						//当前压线

	int						m_iAddCount;					//加注次数

	//是否自动进行游戏
	bool                    m_IsZidong;

	//是否进入游戏
	bool                    m_InitScreen;

	//是否处于滚动状态
	bool                    m_ScrollState;

	bool					m_IsYanshi;

	bool                    m_havequan[15];

	//是否可以下分
	bool                    m_CanHaveMoney;

	int                     m_ThreeMaLiCount;

	int						m_iGameViewState;			//界面 0主界面 1比大小 2小马利

	bool					m_bQuanpan;					//全盘奖


	bool                   m_bGetScore;                 //是否已经得分

	bool                   m_bAllStop;
	//TODLSL
	int                     houseIndex;                     //房间索引

	//位置
public:

	cocos2d::CCPoint		m_ptline[9];				//线位置

	cocos2d::CCPoint		m_ptLight[9];				//灯位置


	//定时器标识
public:

	int						m_iTypeTimeTag;					//类型时间标识

	int						m_iTypeScrollTag;				//滚动定时标识

	int						m_iDwonScoreTag;				//赢分定时标识

	//按钮
public:
	cocos2d::extension::ControlButton*					m_btZidong;				//自动
	cocos2d::extension::ControlButton*					m_btGetScore;			//得分
	cocos2d::extension::ControlButton*					m_btAddScore;			//加注
	cocos2d::extension::ControlButton*					m_btbibei;				//比倍
	cocos2d::extension::ControlButton*					m_btStart;				//开始
	cocos2d::extension::ControlButton*					m_btStop;				//全停
	cocos2d::extension::ControlButton*					m_btshoudong;			//手动

	cocos2d::extension::ControlButton*					m_btSend;			    //发送聊天
	cocos2d::extension::ControlButton*					m_btUp;			        //发送聊天
	cocos2d::extension::ControlButton*					m_btDown;			    //发送聊天


	//字体
public:


	cocos2d::LabelAtlas					*m_lbScore2;						//界面分数显示
	cocos2d::LabelAtlas					*m_lbScore5;						//界面分数显示

	cocos2d::CCLabelBMFont				*m_lbwinScore;						//赢分

public:
	/*---------------修改bysl--------------*/
	//抽奖进度条背景
	cocos2d::Sprite                     *bg;
	//提示可以抽奖
	cocos2d::Sprite                     *tiShi;

	cocos2d::Sprite                     *boxLight;
	cocos2d::extension::ControlButton*					m_btPool;			    //抽奖
	cocos2d::LabelAtlas                   *m_lbaward;                         //奖池积分label
	cocos2d::LabelTTF                  *m_lbshow;                         //奖池积分百分比显示

	//领奖进度条
	cocos2d::CCProgressTimer*       s_progressBar;


	//抽奖层
	DrawMain* lotteryLayer;

	//判断宝箱是否在闪动
	bool flag;
	/*------------------------------------*/

	


public:
	SHZCGameView0(void);
	~SHZCGameView0(void);

	virtual bool init();
	void IsDisableView();
	static SHZCGameView0* create();

	//龙飞舞的动画
	void dragonRun();

	//粒子特效
	void playEff();

	//创建按钮
	void  CreatedGameBt();

	/*-----------修改bysl-----------*/
	//判断抽奖等级
	int getDegreeType();
	//记录玩家获取的经验
	void setLotteryExp(int myExp);

	//记录玩家更新的经验
	void updateLotteryExp();

	//判断玩家经验值是否可以抽奖
	void arriveLottery();

	//创建聊天室
	void CreateChatRoom();
	//创建经验条
	void createProgress();

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
	void SetGameSceneInterface(SHZCGameSceneInterface* scene){ m_GameSceneInterface = scene; };

	//筹码不足时的充值提示
	void rechargeNotice(Node* node);

	//继承函数
private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);


	// 移除播放完的动画回调
	void removeCallback(cocos2d::CCNode *obj);
	//功能函数
public:

	void ChecKLine();  //检查连线，是否得分

	void TypeFlash();

	//更新自己上分
	void SetCreditScoreScore(int myscore);

	//更新滚动后获取的分数以及类型数据
	void ScrollInformation(int winscore, int index, int imgtypeid, bool bquanpan);

	//设置下注
	void SetXiaZhu(int allcellscore);


	//设置压线
	void SetYaLine(int cline);

	//设置滚动
	void SetScrollState(bool bScroll);
	

	//设置15个类型图
	void SetTypeImg(int iPos);

	//设置15个类型图黑
	void SetTypeImgBack(int iPos);

	//设置15个类型图赢
	void SetTypeImgWin(int iPos);


	//类型动画播放
	void ShowTypeAnim1(cocos2d::CCNode* node, void* data);

	//类型动画播放
	void ShowTypeAnim2(cocos2d::CCNode* node, void* data);

	//设置动画播放
	void SetTypeAnim(int typeIndex);

	//上下分
	void AddOrRemoveScore(bool addorremove);

	//得分
	void GetWinScore(int gettype);

	//获取赢分
	void GetWinScore(int winscore, int gametype);

	//比倍
	void GetBiBeiData();

	//加注
	void ChangeCellScore();

	//切换场景动画
	void ChangeSceneAnim();

	//场景动作结束
	void ChangeScenAnimFinish(cocos2d::CCNode* node, void* data);

	//显示界面
	void ShowView(int iCreditScore);

	//显示线
	void ShowLine();

	//显示灯
	void ShowLight();

	//显示赢分线
	void SetWinLine();

	//不能游戏对话框
	void DiableGamePopup();



	//定时器
public:

	//定时器消息
	void OnTimer(int nIDEvent);

	//闪烁
	void OnTwinkleTimer(float dt);

	//显示全停按钮
	void OnShowStopBt(float dt);

	//定时器
	void On1000Timer(float dt);
	void On1001Timer(float dt);
	void On1002Timer(float dt);

	//比倍得分
	void OnbibeiordefenTimer(float dt);

	//类型滚动
	void OnTypeScrollStopTimer(float dt);

	//获取赢分
	void OnDwonScoreTimer(float dt);

	//纤细结束后的动画闪烁
	void OnTypeImgStructTimer(float dt);

	//动画
public:

	//创建动作(循环播放动画)
	void ForeverAction(cocos2d::CCNode* pNode, const char *strName, int iCount, cocos2d::CCPoint pt);

	//创建动作(循环播放动画)
	void ForeverAction(cocos2d::CCSprite* pSprite, const char *strName, int iCount, int iFirst, int ilayer);

	//创建动作(播放动画一次)
	void SequenceAction(const char *strImage, cocos2d::SEL_CallFuncND selector, int iCount, cocos2d::CCPoint ptImage, int iTag);
	///////////////////////////////////////////////////////////////////////
	// UIUpgradeSink回调
public:
	virtual void setCreditScore(int iCreditScore);

public:
	SHZGoldDownLayer* m_goldDownLayer;
private:
	void ShowGoldDownEffect();
};


#endif
