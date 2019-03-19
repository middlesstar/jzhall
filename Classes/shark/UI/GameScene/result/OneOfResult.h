#ifndef _OneOfResult_H_
#define _OneOfResult_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class OneOfResult : public ViewBase
{
public:
	OneOfResult();
	~OneOfResult();

	CREATE_FUNC(OneOfResult);

	//按钮响应事件
	void BtEvent(Ref *pSender, TouchEventType type);
	
protected:
	void setContent();
public:
	//设置倍率
	void SetBeiLv(int g);

	//显示总押分
	void showStake(int score);

	//显示赢分
	void showWin(int score);

	//显示彩色光圈
	void showColor();

	//光圈旋转
	void runCircle();

	//停止所有的动作
	void stopAll();

	//设置本局开奖的按钮
	void showWinBtn(bool flag,int score);
   //显示正常的图片
	void showNormal();

	//播放光圈缩小后的音效
	void playCircleSong();

protected:
	TextAtlas*			m_beilvNum;											//倍率艺术数字
	TextAtlas*			m_mystakeNum;											//我的押注艺术数字
	Text*               winNum;                                             //赢的分数
	Sprite*             blackLabel;                                        //黑色背景
	Sprite*             whiteLabel;                                        //白色背景
	ImageView*          blinkCircle;                                       //光圈

	Button*             btn;
};

#endif 