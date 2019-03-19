#ifndef  _ResultBoardManager_H_
#define  _ResultBoardManager_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"
#include "shark/UI/GameScene/result/OneOfResult.h"

class ResultBoardManager : public ViewBase
{
public:
	ResultBoardManager();
	virtual ~ResultBoardManager();
	CREATE_FUNC(ResultBoardManager);

	virtual void setContent();

	//开启闪烁
	void startPlay();
	//关闭闪烁
	void stopPlay();

protected:

	//设置显示还原
	void setRestore();
	//设置超级大赢家显示
	void setBigWin();
	//设置赢分显示
	void setWin();
	//计算本局游戏的得分
	void summaryNum();

	//结算游戏得分
	void summaryScore();

	//显示特效
	void playEff(float t);


	//中普通动物
	void generalSummary(int index);
	//中金鲨
	void sharkSummary(int index);

	//中彩金鲨
	void colorSummary(int index);

	//彩金鲨具体实现
	void colorShark(int index);

	//设置赢取分数，元宝数，船票数
	void setWinScore(int score, int yuanbao, int ticket);

	//清空本局得分记录
	void clearWinScore();

public:
	//设置倍率
	void SetRate(int index);
	//设置显示本局游戏中的按钮
	void showWinBtn(int index,bool flag,int score);

	//播放金币喷泉和数字滚动
	void goldandnumroll(float t);
	//数字滚动效果
	void numroll(float t);
	//设置GetNum艺术数字string
	void setGetnum(SCORE l);
	//播放金币飞行动画
	void playGoldFly(Vec2 startPos, Vec2 endPos);
protected:
	//根节点
	Node*              pMainNode;
	Node*              downNode;
	Node*              commandShow;
	Node*              winNode;
	Node*              stakeButton;
	//Node*              lostNode;
	TextAtlas*         scoreLabel;
	TextAtlas*         ticketLabel;
	TextAtlas*         yuanBaoLabel;

	Sprite*            winLogo;
	Sprite*            bigWinLogo;

	//存储所有的光圈
	cocos2d::Vector<Sprite* > ringCircle;
	//存储所有的显示label
	cocos2d::Vector<TextAtlas*>textScore;

	//开奖结果计算
	int            ticketNum;
	int            yuanbaoNum;
	int            scoreNum;

	//Stake_Btn_Max个压分面板按钮  
	OneOfResult		m_stakeArray[12];


};

#endif // _SharkResultManger_H_

