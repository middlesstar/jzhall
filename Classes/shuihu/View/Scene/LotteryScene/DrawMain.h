#ifndef __DrawMain_LAYER_H__
#define __DrawMain_LAYER_H__

#include "cocos2d.h"
#include "shuihu/ViewHeader_SHZ.h"

class DrawMain
	: public cocos2d::Layer
{
public:
	DrawMain();
	~DrawMain();
public:
	bool init(int type, int exp,int index);
	static DrawMain* createLayer(int type,int exp,int index);
	//屏蔽触摸
public:
	void setTouchSwallow(cocos2d::Object* psender, bool bSwallow = true);
private:
	cocos2d::EventListenerTouchOneByOne* m_pEventListener;
public:
	//确定取消按钮事件
	void  onBtnClick(Ref* ref, cocos2d::extension::Control::EventType evenType);
	//关闭按钮事件
	void onBtnClose(cocos2d::Object* psender);
	//创建抽奖卡片
	void createCards();
	//创建抽奖等级
	void createDegree(int type, int exp);
	//抽奖等级回调
	void menuItemCallback(cocos2d::Object* psender);
    //开始抽奖
	void startPlay(cocos2d::Object* psender);
	//界面等级显示
	void degreeUI(cocos2d::Object* psender);

	//创建领奖进度条
	void createProgress(int type, int exp);
	//创建进度条改变
	void changeProgress();
	//抽奖限制积分改变
	void changeScoreLimit();
	//弹出抽奖层
	void lotteryLayer();
	//显示抽奖等级
	void showDegree();
	
private:
	cocos2d::Size  s;
	cocos2d::Vector<cocos2d::MenuItemToggle* >    menuToggle;
	cocos2d::Vector<cocos2d::LabelTTF* >          scoreLabel;
private:
	//抽奖等级显示 
	cocos2d::Sprite*           degreeIcon;
	//经验获取提示
	cocos2d::LabelTTF*         expCue;
	//当前积分
	cocos2d::LabelAtlas*         curScore;
	//抽奖限制分数
	cocos2d::LabelAtlas*         limitScore;
	//显示进度条百分比
	cocos2d::LabelAtlas*         perLabel;
	//经验进度条
	cocos2d::ProgressTimer*    d_progressBar;
	//记录进入房间等级
	int houseIndex;
	//存放卡片上的数字
	int num[6][6];
	//记录抽奖等级；
	int     degree;
	//记录刚进入抽奖界面时的抽奖等级
	int     originDegree;
	//记录抽奖的经验值
	int     lotteryExp;
	//记录距离下一等级的分数
	int     disScore;
	//抽奖提示层
	LayerColor*  showLayer;
	//创建进度条背景
	Sprite*  progressBg;

	//提示能进行更高等级的抽奖
	 Sprite*                 tipText;
	 Menu*                   bt_start;
	 //不能点击抽奖
	 Sprite*                 notDraw;
	 //Label*         tipText;
};

#endif // __DrawMain_LAYER_H__
