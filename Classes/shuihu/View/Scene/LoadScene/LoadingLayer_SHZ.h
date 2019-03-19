#ifndef SHZ_LoadingLayer_H_
#define SHZ_LoadingLayer_H_

#include "cocos2d.h"

class SHZLoadingLayer
	: public cocos2d::CCLayer
{
public:
	//创建场景
	static cocos2d::CCScene* scene();
	//创建方法
	CREATE_FUNC(SHZLoadingLayer);

public:
	//构造函数
	SHZLoadingLayer();
	~SHZLoadingLayer();
	//初始化方法
	virtual bool init(); 
	//进入完成
	virtual void onEnterTransitionDidFinish();
	//更新
	virtual void update(float delta);
private:
	//初始化页面的基本纹理
	bool setUpdateView();
	//获取每日提醒
	std::string getTip();


private:

	// 已加载数量
	int		mLoadedNum;
	// 加载数量
	int		mTotalNum;
	
	float	mOriginW;   //保存进度条的原始大小
	float	mCurW;
	float	mTargetW;
	cocos2d::CCSprite	*mBar;
	cocos2d::__Array      *mResources;


	//cocos2d::Vector<String *> m_VecStringList;
	cocos2d::CCLabelTTF *mTips;
};
#endif // _LoadingLayer_H_
