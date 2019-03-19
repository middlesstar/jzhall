#ifndef SHZ_GameClientView_H__
#define SHZ_GameClientView_H__

#include "cocos2d.h"
#include "shuihu/GameHeader_SHZ.h"
#include "TouchLayer_SHZ.h"
#include "df/Timer.h"
#include "cocos-ext.h"
#include "GameStruct_SHZ.h"
#include "GameView0_SHZ.h"
#include "GameViewBibei_SHZ.h"
#include "GameViewMali_SHZ.h"


//筹码定义
#define JETTON_COUNT_SHZ				7									//筹码数目
#define JETTON_RADII_SHZ				55									//筹码半径

//消息定义
#define IDM_PLACE_JETTON_SHZ			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER_SHZ			WM_USER+201							//申请信息
#define IDM_CONTINUE_CARD_SHZ			WM_USER+202							//继续发牌
#define IDM_AUTO_OPEN_CARD_SHZ			WM_USER+203							//按钮标识
#define IDM_OPEN_CARD_SHZ				WM_USER+204		

//索引定义
#define INDEX_PLAYER_SHZ				0									//闲家索引
#define INDEX_BANKER_SHZ				1									//庄家索引

#define MAX_APPLY_DISPLAY_SHZ			6									//最大申请显示人数

//历史记录
#define MAX_SCORE_HISTORY_SHZ			50									//历史个数
#define MAX_FALG_COUNT_SHZ				32									//标识个数

//筹码数组
//typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;
class SHZGameTest : public cocos2d::CCLayer
{
public:

	CREATE_FUNC(SHZGameTest);

	virtual bool init();

	//增加在Android平台下的返回键监听
	//virtual void keyBackClicked();
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	inline void setViewRef(SHZCGameView0* pGameView0Ref, SHZCGameViewMali* pGameViewMaliRef) {
		m_pGameView0Ref = pGameView0Ref;
		m_pGameViewMaliRef = pGameViewMaliRef;
	}

	void onCloseCallback(cocos2d::CCNode *pNode);

	//充值按钮的暂时处理方案
	void comeToHall();
private:
	SHZCGameView0* m_pGameView0Ref;
	SHZCGameViewMali* m_pGameViewMaliRef;
};


class SHZCGameClientView : public cocos2d::CCScene
	, public SHZCGameSceneInterface
	, public SHZITouchSink
{

public:
	//是否自动进行游戏
	bool                           m_IsZidong;

	//游戏模式,为0时是15线，1时是比倍，2时是滚动机
	int                            m_GameState;

	//上的单元积分
	int                     m_CellScore;

	int						m_iDuihuan;						//兑换比例

	//玩家上的分
	int                     m_CreditScore;

	bool					m_bPlayTypeAnim;				//动画播放


	int						m_iGetScoreType;				//得分类型

	int						m_iCount;						//公告数量



private:

	dword				    m_Time;							//全局时间

	//资源TEXTURE
private:

	cocos2d::CCSprite*				m_winscoreViewTexture;				//赢分标志
	cocos2d::CCSprite*				m_bibeiViewTexture;					//比倍
	cocos2d::CCSprite*				m_defenViewTexture;					//得分

	cocos2d::CCSprite*				m_GameNotcieTexture;				//游戏公告

	bool						    m_IsMos;

	int								m_iImageCount;

	//界面
public:

	SHZCGameView0*						m_NodeGameView0;	//主界面
	SHZCGameViewBibei*					m_NodeGameView1;	//比大小
	SHZCGameViewMali*					m_NodeGameView2;	//小马利

	SHZGameTest*						m_GameTest;

	CCProgressTimer *				mProgress;

	SHZTailorView*						m_TailorView;

	//字体
public:

	//信息
	cocos2d::LabelAtlas				*m_LbUserScore;						//用户分数

	cocos2d::LabelAtlas				*m_LbLv;							//等级

	cocos2d::CCLabelBMFont			*m_lbwinScore;						//赢分

	//按钮
public:

	//ControlButton*				m_btshare;								//分享
	ControlButton*				m_btGameBack;							//返回
	ControlButton*				m_btshezhi;								//设置
	ControlButton*				m_btHelp;								//帮助
	ControlButton*				m_btJeton;								//筹码界面
	ControlButton*                m_btRecharge;                           //充值按钮
public:

	//TouchLayer*						touch_layer_;						//鼠标接口

	//函数定义
public:
	//构造函数
	SHZCGameClientView();
	//析构函数
	virtual ~SHZCGameClientView();

	//创建方法
	CREATE_FUNC(SHZCGameClientView);

	//继承函数
private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);
public:

	void onReward(cocos2d::CCObject* obj);

	//充值之后刷新水浒传界面
	void refreshCoin(Ref* ref);

	//弹出提示退出的对话框
	void closeOutGame(Node* node);
	
protected:

	/** 初始化 */
	bool init();
	/** 释放 */
	void OnDeinit();
	/** 恢复 */
	void OnRestore();
	/** 更新 */
	void update(float dt);
	/** 渲染 */
	void OnDrawView();

	//创建按钮
	void  CreatedGameBt();

	//场景动画完成
	virtual void onEnterTransitionDidFinish();

	virtual void onEnter();  //场景进入

	//显示顶上的横幅
	void ShowTopView(bool bshow);

	//重载函数
public:
	//重置界面
	virtual void ResetGameView();
	//调整控件
	virtual void RectifyControl();

	//切换比倍界面
	virtual void OnChangeBibeiView();

	//切换主界面
	virtual void OnChangeGameView(int iCreditScore);

	//切换玛丽界面
	virtual void OnChangeMlView();

	

public:
	
	//更新抽奖后的金币数和元宝数和经验值
	void  updateLotteryResult(Ref* obj);

	//记录玩家获取的经验
	void setLotteryExp(int myExp);
	//更新自己上分
	void SetCreditScoreScore(int myscore);
	//更新滚动后获取的分数以及类型数据
	void ScrollInformation(int winscore, int index, int imgtypeid, bool bquanpan);
	//设置玩家上下注
	void SetXiaZhu(int allcellscore);
	//获取积分
	void GetWinScore(int winscore, int gametype);
	//比倍结果
	void SetBiBei(int winscore, int S1, int S2);
	//小玛利信息
	void SetMaLiInformation(int winscore, int index, int imgtypeid, int gamecount, int tyepindex, bool nowbegin);
	//清理所有数据
	void CleanAllMemory();
	//任务文字提示
	void SetFontString(char WString);
	//比倍数组
	void CopyBiBeiData(int ibibeidata, int ipos);

	//时间执行
	bool TypeImgVoid(int timeID, int typeIndex, int abindex);

	//抽奖完毕飞金币
	void jinbiFly(Ref* ref);
	//抽奖完毕后飞元宝
	void yuanbaoFly(Ref* ref);

   //一分一分加金币的效果
	void addCoinScore(float t);
	//一分一分加元宝的效果
	void addBaoScore(float t);


	//控件消息
	void ControlsMessage();

	//绘制按钮
	void DrawBt();

	//功能函数
public:


	//设置15个类型图
	void SetTypeImg(int iPos);

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

	//比倍
	void GetBiBeiData();

	//加注
	void ChangeCellScore();

	//游戏公告
	void SetGameNotice(const char* strMsg);

	void RollText(cocos2d::CCNode* node, void* data);

	void DisableGamePopUp();  //显示无法游戏

	//设置压线
	void SetYaLine(int line);


	//定时器
public:

	//比倍得分
	void OnbibeiordefenTimer(float dt);

	//获取赢分
	void OnDwonScoreTimer(float dt);

	//粒子特效
	void playEff();

	

private:

	//创建动作(播放动画)
	void ForeverAction(cocos2d::CCSprite* pSprite, int32 iWidth, int32 iHeight, int ilayer = 8);

	//创建动作(循环播放动画)
	void ForeverAction(cocos2d::CCSprite* pSprite, char *strName, int iCount, int iFirst = 0, int ilayer = 8);

	//创建动作(播放动画一次)
	void SequenceAction(const char *strImage, cocos2d::SEL_CallFuncND selector, int iCount, cocos2d::CCPoint ptImage, int iTag);

private:
	int vipColorIndex(const char* strMsg);

private:
	void recordScore(long addscore);
	/*----------抽奖之后刷新金币--------------*/
	int  originScore;
	int  disScore;
	int  getScore;
};

#endif