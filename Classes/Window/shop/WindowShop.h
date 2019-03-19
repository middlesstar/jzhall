#ifndef  _WindowShop_H_
#define  _WindowShop_H_

#include "Window/WindowBase.h"

/* 充值界面 */
enum eWindowShopState
{
	EWSS_NORMAL,		//充值界面
	EWSS_VIP,			//VIP说明界面
	EWSS_FIRST			//首冲界面
};

class  WindowShop : public WindowBase
{
public:
	WindowShop();
	virtual ~WindowShop();

	CREATE_FUNC(WindowShop);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;

	virtual void updateWindow(int nCode) override;	                //刷新窗口
protected:
	void _setUIState(int nState);
	void _initVipInfo();
	void _initShopItems();
	void _initVipItems();

	void _playButtonAction();

	void _ItemClickCallback(Ref* pRef);
	void refreshVipInfo(Ref* ref);
protected:
	Sprite* m_pBg;
	Sprite* m_pInfoSpr;
	Sprite* m_pTopLevelSpr;
	Node* m_pNodeRecharge;
	Node* m_pNodeVip;
	Node* m_pNodeFirst;

	ui::ScrollView* m_pVipScroll;
	ImageView* m_pVipItem;
	Button* m_pBtnClose;
};

#endif // _WindowShop_H_

