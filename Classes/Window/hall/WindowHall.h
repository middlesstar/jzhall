#ifndef  _WindowHall_H_
#define  _WindowHall_H_

#include "Window/WindowBase.h"
#include "CycleScroll.h"

enum eHallUpdateCode
{
	EHUC_ICON				=	0,				//ˢ��icon
	ENUM_FUDAI_START		=	1,				//���������ʼ
	ENUM_FUDAI_FAILURE		=	2,				//�������ʧ��
	ENUM_COMMENT_SUCCESS	=	3				//�����������۳ɹ�
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
	void _setUserInfo();				//���ø�����Ϣ	
	void _updateUserIcon();				//ˢ��icon

	void _playSelectEffect();			//ĳ������Ϸ�������м���Ч��
	void _stopSelectEffect();

	void StartTimeCount(int t);			//�����������ʱ
	void updateTime(float t);				//ʱ�䵹��ʱ
	void getInfo(int coinScore, float yuanbaoScore);//�����ص�
	void fdSetTime(int time);				//���ø�������ʱʱ��

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
	TextAtlas*	m_pLeftTime;				//����������ʱ
	CycleScroll* m_pCycleScroll;
	std::vector<sUnitInfo> m_vecHallData;
};

#endif // _WindowHall_H_

