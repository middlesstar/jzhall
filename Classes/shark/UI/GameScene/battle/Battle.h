#ifndef _Battle_H_
#define _Battle_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"



class Battle : public ViewBase
{
public:    

	Battle();
	virtual ~Battle();

	CREATE_FUNC(Battle);
	virtual void setContent();				                //设置UI布局

	//启动旋转
	void startPlay();
	//关闭旋转
	void stopPlay();
	//回转光圈接口
	void rotation();
	//设置光圈开始和停止的位置
	void setCirclePos(int beginPos, int endPos);
	//控制光圈回转停止的位置
	void setRotationPos(int index);
protected:

	//充值宝箱动画
	void moveRecharge();
	
	//最后一个显示烟花爆炸的效果
	void lightBoast(int index);
	//点亮光圈
	void lightCircle(float t);
	//控制光圈的亮暗
	void controlCircle(int repeat);
	//移动光圈
	void moveCircle(float t);
	//减速光圈
	void reduceCircle(float t);
	//再减速光圈
	void stopCircle(float t);
	//匀速停止光圈
	void meanCircle(float t);
	/*------------中奖-----------------------*/
	//回退
	void back(float);

	//充值按钮
	void Recharge(Ref *pSender, TouchEventType type);

public:
	//更换界面需要更换的元素
	void changeImg(int index);
	//还原更换的图片
	void reChangeImg();
	//设置彩金的数值
	void setColorMoney(int money);
private:

	//存储旋转的光圈
	cocos2d::Vector<Sprite* > circle;
	//记录点亮光圈的个数
	int  num;
	//控制点亮光圈的个数
	int limitNum;
	//记录当前光圈的位置
	int pos;
	//控制光圈跑的圈数
	int moveNum;
	//控制光圈减速的速度
	float limitSpeed;
	//记录光圈匀速移动的个数；
	int closeNum;
	//控制停止的位置
	int stopPos;
	//光圈移动一圈的标志位
	int circleFlag;
	//光圈回转停止的位置
	int rotationPos;

	//确定用的那一套随机概率
	int lvIndex;
protected:
	Node*        pMainNode;

	ImageView*      topColor_shark;
	ImageView*      downColor_shark;

	ImageView*     leftTop_shark;
	ImageView*     rightTop_shark;
	ImageView*     rightDown_shark;
	ImageView*     leftDown_shark;
	ImageView*     leftMiddle_shark;
	ImageView*     rightMiddle_shark;

	Sprite*      screen;
	TextAtlas*   colorMoneyNum;

	Button*       btn_recharge;
	ImageView*    rechargeLight;

};

#endif // _Battle_H_