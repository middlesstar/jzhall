#ifndef __MyJettonManager_H__
#define __MyJettonManager_H__

#include "OneOfJetton.h"
#include "slwh/Module/SLWHGoldHeap.h"

/*
ѹ����������
*/

#ifndef Jetton_Btn_Max
#define Jetton_Btn_Max	15
#endif

#define RECORDMAX 12

class MyJettonManager :
	public UIBase
{
protected:

	//����ĸĶ� ������Ϸ���������¼�ظ����߼�  ֱ�Ӹ��ƹ����ģ� ���û����ƺ�
	struct Record
	{
		ImageView* animal;
		ImageView* zhx;
	};
	std::map<int, std::string> m_map;

	int m_listRear;									//ѭ������β
	Record m_jetrecord[RECORDMAX];
	//Vec2 m_ptEnd;									//�յ�����
protected:
	//Jetton_Btn_Max��ѹ����尴ť  ˳�� (��)����1 ~ 4   (��)~    (��)~    Z H X
	OneOfJetton		m_jetArray[Jetton_Btn_Max];
	//�Ƿ����� AI
	bool			m_bIsAI;
	//��һ����Чѹ��
	int				m_lastValidRecode[Jetton_Btn_Max];
	//��һ����Чѹ������
	int				m_lastValidAll;
	//��һ��ѹ�ּ�¼ 
	int				m_lastRecode[Jetton_Btn_Max];
	//��һ��ѹ������
	int				m_lastGameAll;
	//�Ƿ����
	bool			m_bIsenable;
	//��ť��ѹ���б� ��Ҷ�
	SLWH_GoldHeap*	m_oneofjet[Jetton_Btn_Max];
protected:
	void SetZHXTex(int index, enumGameZXH type);
	void Start();
	void ContinuJetEvent(Ref *pSender, TouchEventType type);
	void CancelJetEvent(Ref *pSender, TouchEventType type);
	void AIEvent(Ref *pSender, TouchEventType type);
	void AICancelEvent(Ref *pSender, TouchEventType type);

public:
	MyJettonManager();
	~MyJettonManager();

public:
	void SaveLastGame();
	void SetRate(int* a);
	void ClearJetton();
	void SetAllJetton(int* g);
	void SetAI(bool b);
	int* GetRecord();
	bool GetAI();
	int  GetLastJetAll();
	bool ContinuJet();
	bool CancelJet();
	//��ʼ����¼��Ϣ�ӿ�  ��λ��Ϊׯ�к�  
	void InitResList(int* a);
	//*****************************************************************************************
protected:

	Node*		m_moveNode;
	//Ѻע��ť����m_node����
	//Button*		m_Jet;
	//ѹ��ѡ��ť
	Button*		m_betAdd;
	//��ǰѹ�ֵ�
	TextAtlas*	m_betText;
	//�Զ���ť
	Button*		m_ai;
	//��ѹ��ť
	Button*		m_continu;
	//ȡ��ѹ�ְ�ť
	Button*		m_cancel;
	//ȡ���Զ�
	Button*		m_cancelAI;
	//��ɫ��
	LayerColor* m_layerColor;
	//��ѹ�� 
	TextAtlas*	m_totleJet;
	//�ʽ�
	TextAtlas*	m_caijin;
	//��¼���
	Node*		m_record;
protected:
	//�����ҵ���ѹ��
	void setMyTotelJet(SCORE score);
	//����ѹ�ֵ��¼�
	void JetAddEvent(Ref *pSender, TouchEventType type);
	
public:
	//����ѹ�ֵ�
	void SetJetNum(int i);

	void update(float t);

	bool onTouchBegan(Touch* touch, Event* event);
	//�Ƿ���ѹ��
	void enable(bool b);
	//��ʾ
	virtual void StartPlay();
	//����
	virtual void StopPlay();
	//���½�ҶѶ�ʱ����Ѻע�׶θ��£�
	void updategoldheap(float t);
	//���òʽ�
	void setCaijin(SCORE score);

	//��ӵ�����ڵ�
	void Add(enumSpecialRs e1, enumAnimalColor e2, enumAnimalType e3, enumGameZXH e4);

	void Add(AnimalResult e);
	//��ʼ��Ϸ�ٻ���������
	void OpenRecord();
	//����ѹ�ֳɹ�
	void successJet(SCORE gold, SCORE score, dword animalID);
	//����ȡ��ѹ�ֳɹ�
	void successCancelJet();
};

#endif

