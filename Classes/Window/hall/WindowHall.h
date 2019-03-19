#ifndef  _WindowHall_H_
#define  _WindowHall_H_

#include "Window/WindowBase.h"
#include "CycleScroll.h"

enum eHallUpdateCode
{
	EHUC_ICON				=	0,				//刷新icon
	ENUM_FUDAI_START		=	1,				//抢福袋活动开始
	ENUM_FUDAI_FAILURE		=	2,				//抢福袋活动失败
	ENUM_COMMENT_SUCCESS	=	3				//好评有礼评论成功
};

class  WindowHall : public WindowBase, public CycleUnitSelectDelegate
{
public:
	WindowHall();
	virtual ~WindowHall();

	CREATE_FUNC(WindowHall);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void setContent() override;
	virtual void updateWindow(int nCode) override;

	virtual void onBackKeyPressed() override;

	virtual void onUnitSelect(const sUnitInfo* pSourceData) override;
	virtual void onUnitMoveBegin() override;
	virtual void onUnitMoveEnd() override;

	void refreshNickName(Ref* ref);
	void refreshCoin(Ref* ref);
	void refreshGold(Ref* ref);
	void refreshVip(Ref* ref);
protected:
	void _setUserInfo();				//设置个人信息	
	void _updateUserIcon();				//刷新icon

	void _playSelectEffect();			//某个子游戏滑动到中间后的效果
	void _stopSelectEffect();

	void StartTimeCount(int t);			//开启红包倒计时
	void updateTime(float t);				//时间倒计时
	void getInfo(int coinScore, float yuanbaoScore);//福袋回调
	void fdSetTime(int time);				//设置福袋倒计时时间

	void _playShopButtonAction();
	void _dismissComment();

	void _initPlatform();
protected:
	ImageView* m_pHeadIcon;

	ImageView* m_pLeftLight;
	ImageView* m_pSelectLight;
	ImageView* m_pRightLight;
	ImageView* m_pSelectEffect;
	ImageView* m_pRechargeLight;
	Sprite* m_pBgLight;
	Button* m_pRecharge;
	ImageView*	m_grabRed;
	TextAtlas*	m_pLeftTime;				//抢福袋倒计时
	CycleScroll* m_pCycleScroll;
	std::vector<sUnitInfo> m_vecHallData;
};

#endif // _WindowHall_H_

