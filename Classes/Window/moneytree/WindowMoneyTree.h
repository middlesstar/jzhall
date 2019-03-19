#ifndef  _WindowMoneyTree_H_
#define  _WindowMoneyTree_H_

#include "Window/WindowBase.h"

#define TREE_COIN_MAX_NODE			36			//cocostudio����ƴ��36���ڵ�

enum eMoneyTreeState
{
	EMTS_Idle,			//����״̬
	EMTS_Shake,			//ҡ��״̬
	EMTS_Get			//��ȡ���˽�ҵ�״̬
};

class  WindowMoneyTree : public WindowBase
{
public:
	WindowMoneyTree();
	virtual ~WindowMoneyTree();

	CREATE_FUNC(WindowMoneyTree);
	virtual void setContent() override;				                //����UI����

	virtual void updateWindow(int nCode) override;	                //ˢ�´���
	virtual void onEnter() override;
	virtual void onExit() override;
protected:
	void _setAcceleration();		//����������Ӧ
	void _onMoneyTreeAcceleration(Acceleration* acc, Event* unused_event);
	
	void _shakeTree();				//ҡ����
	void _showTreeCoin();			//���ϳ����
	void _addCoinToNode(Node* pParent);
	void _runCoinDropAction();		//���Ž�ҵ��䶯��

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
	//ͷ�� 
	Text* m_pGetTimeCD;
	Text* m_pTreeSayText;

	Sprite* m_pSpriteVipInfo;
	Sprite* m_pSpriteVipTop;

	int m_nScheduleTimer;
	int m_nShakeCount;

	Node* m_paCoinNode[TREE_COIN_MAX_NODE];
};

#endif // _WindowMoneyTree_H_

