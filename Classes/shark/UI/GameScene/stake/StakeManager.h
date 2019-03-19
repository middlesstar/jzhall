#ifndef _StakeManager_H_
#define _StakeManager_H_

#include "cocos2d.h"
#include "OneOfStake.h"
#include "shark/Module/SharkGoldHeap.h"


#define  AUTOBTN          "btn_auto"
#define  CANCELAUTO       "btn_cancelAuto"
#define  CONTINUEBTN      "btn_continue"
#define  CANCELBTN        "btn_cancel"               //ȡ��Ѻ��
#define  STAKEBTN         "btn_subsection"           //Ѻ�ְ�ť
#define  STALELABEL       "stakeLabel"               //Ѻ�ֵ���ʾ��ǩ
#define  TOTALSCORE       "totalscore"                         //��Ѻ�ֱ�ǩ
#define  CAIJINSCORE      "winscore"                         //�ʽ��ǩ

#define  STAKEALL         "topPanel"



#define STAKE_ALL_ARRAY		{"bet_bird","bet_goldShark" , "bet_silverShark", "bet_beast", "bet_swallow", "bet_dove", "bet_peacock", "bet_eagle", "bet_lion", "bet_panda", "bet_monkey", "bet_rabbit" }

#ifndef Stake_Btn_Max
#define Stake_Btn_Max	12
#endif

class StakeManager : public ViewBase
{
public:

	StakeManager();
	~StakeManager();
	CREATE_FUNC(StakeManager);

	virtual void setContent();
	
protected:
	Node*    pMainNode;
	Button*  m_stake;
	Button*  m_cancel;
	Button*  m_continue;
	Button*  m_auto;
	Button*  m_cancelAuto;
	//��ǰѹ�ֵ�
	TextAtlas*	m_betText;

	//��ɫ��
	LayerColor*    m_shieldLayer;
	//�Ƿ����
	bool			m_bIsenable;

	//��ť��ѹ���б� ��Ҷ�
	SharkGoldHeap*	m_oneofjet[Stake_Btn_Max];

public:
	void SaveLastGame();
	void SetAllStake(int* g);
	int* GetRecord();
	//���ñ���
	void SetRate(int* a);
	void ClearJetton();
	void SetAI(bool b);
	bool GetAI();
	int  GetLastStakeAll();
	bool ContinuStake();
	bool CancelStake();

	//����뿪ʱ�����б��ϵ���Ϣ
	void playerOut(Ref* ref);

protected:
	
	void ContinuJetEvent(Ref *pSender, TouchEventType type);
	void CancelJetEvent(Ref *pSender, TouchEventType type);
	void AIEvent(Ref *pSender, TouchEventType type);
	void AICancelEvent(Ref *pSender, TouchEventType type);
protected:
	//�����ҵ���ѹ��
	void setMyTotelStake(SCORE score);
	void SetJetNum(int i);
	void JetAddEvent(Ref *pSender, TouchEventType type);

public:	
	//��˸��ť�ϵĹ�Ȧ
	void  showCircle();
	//��ʾ��ת��Ȧ
	void showRing();

	//���òʽ�
	void setCaijin(SCORE score);
	//�����ҵ�ѹ��
	void updateMyStake(float t);

	//���ô�������
	void setTouch(bool b);

	bool onTouchBegan(Touch* touch, Event* event);

	//����ѹ�ֳɹ�
	void successStake(SCORE gold, SCORE score, dword animalID);
	//����ȡ��ѹ�ֳɹ�
	void successCancelStake();

	//���½�ҶѶ�ʱ����Ѻע�׶θ��£�
	void updateGoldheap(float t);
	//��Ҷ�����
	void hideGoldheap();
	//������ҶѶ�ʱ��
	void startGoldheap();
private:
	cocos2d::EventListenerTouchOneByOne* eventListener;
private:
	cocos2d::EventListenerTouchOneByOne* listener;


protected:
	//Stake_Btn_Max��ѹ����尴ť  
	OneOfStake		m_stakeArray[Stake_Btn_Max];
	//�Ƿ����� �Զ�
	bool			m_bIsAI;
	//��һ����Чѹ��
	int				m_lastValidRecode[Stake_Btn_Max];
	//��һ����Чѹ������
	int				m_lastValidAll;
	//��һ��ѹ�ּ�¼ 
	int				m_lastRecode[Stake_Btn_Max];
	//��һ��ѹ������
	int				m_lastGameAll;

	//��¼���ֵı���
	//int             m_rate[Stake_Btn_Max];


	//��ѹ�� 
	TextAtlas*	m_totleStake;
	//�ʽ�
	TextAtlas*	m_caijin;

};

#endif // _StakeManager_H_
