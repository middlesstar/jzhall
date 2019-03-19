#ifndef __spriteFly_H__
#define __spriteFly_H__

#include "cocos2d.h"
/**  使用方法
*   1.创建一个spriteFly对象
*   2.将该对象add到当前layer   
*   3.调用setPos方法设置元宝飞出的起始和终止位置（传入参数1：产生金币的起始位置，传入参数2：元宝飞行的终止位置）
*   4.调用本类中的startAni方法即可开始播放动画（传入参数1：播放金币效果，传入参数2：播放元宝效果）
*   5.如需调用加分效果，只需用对象调用addscore方法（传入参数1：加分效果的label，传入参数2：当前用户的总分数，传入参数3：玩家获取的分数）
*/


class spriteFly : public cocos2d::Node
{

protected:

	//创建元宝
	void createYuanbao();

	//创建元宝飞
	void createYuanbaoFly(Node* sp, int m);

	//创建元宝放大效果
	void yuanBaoChange(Node* obj);

	//创建金币
	void createCoin();
	//金币特效
	void createCoinFly(Node* sp, int m);

	//移除金币
	void removeSpriteFly(Node* obj);

	//金币放大效果
	void coinChange(Node* obj);	

	//加分效果
	void scoreTimer(float t);	

public:
	spriteFly();
	~spriteFly();
	static spriteFly* create();
	virtual bool init();
	//开始播放动画
	void startAni(unsigned int n);
	//加分效果传入参数
	void addscore(cocos2d::LabelAtlas* label, int totaleNum,int num);
	//设置金币或元宝飞出的起始和结束位置
	void setPos(cocos2d::Vec2 pos1,cocos2d::Vec2 pos2);

	//飞出位置
	cocos2d::Vec2                 beginPos;
	//结束位置
	cocos2d::Vec2                 endPos;

protected:
	//动画的播放类型
	unsigned int		         type;
	//当前分数
	int                          score;
	//获取的分数
	int                          getScore;
	//分数变化区间
	int                           disScore;
	//记录分数的标签       
	cocos2d::LabelAtlas*          scoreLabel;
	
    
};

#endif // __spriteFly_H__
