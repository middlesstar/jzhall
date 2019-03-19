#ifndef SHZ_UserInfoLayer_H_
#define SHZ_UserInfoLayer_H_

#include "cocos2d.h"
#include "cocos-ext.h"


//用户信息Layer
class SHZUserInfoLayer
	: public cocos2d::CCLayer
{
public:
	CREATE_FUNC(SHZUserInfoLayer);

public:
	SHZUserInfoLayer();
	~SHZUserInfoLayer();
	bool init();
	
	//设置用户的数据
	void setDatas();



	void setCloseDialogInfo(cocos2d::CCObject* target, cocos2d::SEL_CallFuncN callfun, const std::string& title, const std::string& content);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);


	//先于onEnterDidFinished()
	virtual void onEnter();

	//通知消息方法
private:
	void onUserScore(cocos2d::CCObject* obj);
	void onReward(cocos2d::CCObject* obj);


public:
	void ForeverAction(const char *strName,int iCount,cocos2d::CCPoint ptImage,int iFirst=0);

	//刷新界面显示
	void refreshPlayerCoin(Ref* ref);
	//////////////////////////////////////////////////////////////////////////
	// IGPInsureMissionSink
private:
	virtual void onInsureInfo();
	virtual void onShareSucceed(int score);
	virtual void onShareFailed();

private:
	// 网路版
	cocos2d::CCLabelTTF* mLbNickname;
	cocos2d::LabelAtlas* mLbScore;	// 分数
	cocos2d::LabelAtlas* m_LbLv;	// 元宝数

	cocos2d::CCObject*		mTarget;
	cocos2d::SEL_CallFuncN	mCallback;
	std::string				mTitle;
	std::string				mContent;


};

#endif // _UserInfoLayer_H_
