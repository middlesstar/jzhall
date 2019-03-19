#ifndef SHZ__QUAN_PAN_H_H
#define SHZ__QUAN_PAN_H_H
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ShieldLayer_SHZ.h"
#include "shuihu/plazz/plazz/CGPQuanPanMission_SHZ.h"
#include "shuihu/View/Scene/ServerListScene/IOpenServerItem_SHZ.h"




class SHZUIQuanPan:
	public cocos2d::CCLayer,
	public SHZICGPQuanPanMissionSink
{

public:
	CREATE_FUNC(SHZUIQuanPan);
	virtual bool init();  //初始化界面

//构造与析构函数
public:
	SHZUIQuanPan();
	virtual void onEnterTransitionDidFinish();
	~SHZUIQuanPan();

private:
	bool SetUpView();         //构建场景
	bool ReadQuanPanInfo();   //读取全盘信息
	inline void ToBin(int __v, int __bin[]){

		int i = 0;

		while (i <= 7)
		{
			__bin[i++] = __v % 2;
			__v = __v / 2;
		}

	}




////////////////////////////////////////////////////////////////////////////
// ICGPQuanPanMissionSink
public:
	virtual void onGPQuanPanStatue(uint8 statue);

//按钮的事件响应
public:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);  //打开全盘奖


//设置开启房间接口
public:
	inline void SetOpenServerItem(SHZIOpenServerItem* __iOpenServerItem){ mOpenServer = __iOpenServerItem; }

private:
	int mQuanPanTag[8];         //标示当前全盘的状态
	SHZShieldLayer* mShieldLayer;  //屏蔽层
	uint8 mStatue;
	SHZCGPQuanPanMission mQuanPanMission;

public:
	SHZIOpenServerItem* mOpenServer;
};

#endif