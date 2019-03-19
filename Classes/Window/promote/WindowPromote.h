#ifndef  _WindowPromote_H_
#define  _WindowPromote_H_

#include "Window/WindowBase.h"

struct CMD_ST_ShouYiRsp;
struct CMD_ST_ShouRankYiRsp;
struct SyRank;

/* 兑换界面 */
enum eWindowPromoteState
{
	EWPS_SHAREDAYS,		//推广天数
	EWPS_PROMOTE,		//我要推广	
	EWPS_PRIZE,			//推广奖励
	EWPS_HOW,			//如何推广
	EWPS_RANK			//推广排行
};

class  WindowPromote : public WindowBase
{
public:
	WindowPromote();
	virtual ~WindowPromote();

	CREATE_FUNC(WindowPromote);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;

	virtual void updateWindow(int nCode, void* pData) override;	    //刷新窗口,附带参数
protected:
	void _setUIState(int nState);

	int _getShareDayLevel(int nShareDay);
	float _getShareDayProgress(int nShareDay);

	void _setShareDayInfo();
	void _setPromotePrize(CMD_ST_ShouYiRsp* pData);

	void _setPromoteRank(CMD_ST_ShouRankYiRsp* pData);
	void _setRankItem(ImageView* pItem, SyRank* pData, int nIndex);

	void _playShakeTipActiom(Widget* pWidget);
	void _stopShakeTipActiom(Widget* pWidget);

	void _getSharePrizeCallback(Ref* pRef);
	void _tabClickCallback(Ref* pRef);
	void _boxClickCallback(Ref* pRef);
protected:
	Node* m_pNodePromote;
	Node* m_pNodePrize;
	Node* m_pNodeHow;
	Node* m_pNodeRank;

	ImageView* m_pImgPromote;
	ImageView* m_pImgPrize;
	ImageView* m_pImgGetPacket;
	ImageView* m_pImgRank;
	ImageView* m_pImgSelect;

	ui::ScrollView* m_pScroll;
	ImageView* m_pItem;
};

#endif // _WindowPromote_H_

