#ifndef  _WindowMoneyTree_H_
#define  _WindowMoneyTree_H_

#include "Window/WindowBase.h"

#define TREE_COIN_MAX_NODE			36			//cocostudio里面拼了36个节点

enum eMoneyTreeState
{
	EMTS_Idle,			//空闲状态
	EMTS_Shake,			//摇动状态
	EMTS_Get			//获取到了金币的状态
};

class  WindowMoneyTree : public WindowBase
{
public:
	WindowMoneyTree();
	virtual ~WindowMoneyTree();

	CREATE_FUNC(WindowMoneyTree);
	virtual void setContent() override;				                //设置UI布局

	virtual void updateWindow(int nCode) override;	                //刷新窗口
	virtual void onEnter() override;
	virtual void onExit() override;
protected:
	void _setAcceleration();		//设置重力感应
	void _onMoneyTreeAcceleration(Acceleration* acc, Event* unused_event);
	
	void _shakeTree();				//摇动树
	void _showTreeCoin();			//树上长金币
	void _addCoinToNode(Node* pParent);
	void _runCoinDropAction();		//播放金币掉落动画

	void _setPlayerInfo();
	void _setMoneyStaticInfo();
	void _setMoneyDynamicInfo();
	void _moneyTreeSay(int nTreeState,bool bForceCover);

	void _setVipInfo();
	void _refreshVipInfo(Ref* pRef);
protected:
	Sprite* m_pTreeSaySprite;
	Sprite* m_pCoinSprite;
	ImageView* m_pImgSharkTip;
	//头像 
	Text* m_pGetTimeCD;
	Text* m_pTreeSayText;

	Sprite* m_pSpriteVipInfo;
	Sprite* m_pSpriteVipTop;

	int m_nScheduleTimer;
	int m_nShakeCount;

	Node* m_paCoinNode[TREE_COIN_MAX_NODE];
};

#endif // _WindowMoneyTree_H_

