#ifndef __SLWHGameScene_H__
#define __SLWHGameScene_H__

#include "cocos2d.h"
#include "UILayerMain.h"
#include "Model3DLayer.h"
#include "SpeRes/SpeD3Y.h"
#include "SpeRes/SpeD4X.h"
#include "SpeRes/SpeCJ.h"
#include "SpeRes/XYCJManager.h"
#include "SpeRes/SpeDMG2.h"
USING_NS_CC;

class SLWHGameScene : public cocos2d::Scene
{

public:
	//time_t t;
	static Model3DLayer*	m_model3DLayer;
	static UILayerMain*		m_uiLayer;
	static SLWHGameScene*	This;
protected:
	int                     timeDelay;              //��������ʱ���ӳ�
	int						m_nowIndex;				//��ǰ��Ϸ����
	enumServerState			m_nowState;				//��ǰ��Ϸ״̬
	bool					m_bIslive;				//���������Ƿ������ϼ��

	Node*                   chat;                   //�������Ľڵ�
	Node*                   cj_root;                //�齱�ڵ�
	Node*                   xycj_root;              //���˳齱���ڵ�

	time_t                  outTime;                //�����̨ʱ��ʱ���
public:
	SLWHGameScene();

	virtual ~SLWHGameScene();

	virtual bool init();
	//��ʼ��Ϸ
	void StartGames(int leftTime);
	//�ȴ���Ϸ
	void WaitGames(int lefttime);
	//����
	void OpenGames();

	virtual void onEnter() override;

	virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;

	//��������Ӧ
	virtual void OnHreatPak();

	CREATE_FUNC(SLWHGameScene);
protected:
	//�رս���ʱ�ĶϿ����ӵĴ���
	void netDisconnect();

	/*--------------�Ӻ�̨����ǰ�˵Ĵ���------------*/
	void comeFromBack(Ref* ref);               //�Ӻ�̨����ǰ��
	void comeToBack(Ref* ref);                 //��ǰ�˽����̨
	void startTimer(float time);               //������ʱ��

	void goMainScene(float t);                 //���������棬������
	void loadroomData();                       //���ط���������Ϣ
protected:
	//��Ϸ����������
	void GameRunning(float t);
	//��ʱ����������
	void Bridge(float t);
	//���ſ�������
	void PlayAnimalMusic(enumSpecialRs eRs, enumAnimalType eType, enumAnimalColor eColor);
	//���״̬
	void waitGameUpdate(float t);
	//�ӳٵ���Ѻע
	void delaypopJet(float t);
	//������
	void CheckLive(float t);
	//��������鶨ʱ��
	void checkLiveupdate(float t);
	//����������
	void updateSendHreat(float t);
protected:
	//���⿪��
	//D3Y
	SpeD3Y					m_speD3Y;
	//D4X
	SpeD4X					m_speD4X;
	//CJ
	SpeCJ					m_speCJ;
	//XYCJ
	XYCJManager				m_speXYCJ;
	//DMG
	SpeDMG2					m_speDmg;


protected:
	bool AddGold(int g);

	bool AddYuanbao(int g);					//����Ԫ��
		
	bool AddTicket(int g);					//���Ӵ�Ʊ

	void initSpe();							//��ʼ�����⿪��
	//�����߼�
	void Normal(int index);					//��ͨ����

	void D3Y(int index);					//����Ԫ

	void D4X(int index);					//����ϲ

	void CJ(int index);						//�ʽ�

	void XYCJ(int index);					//���˳齱

	void DMG(int index);					//������

	void _4LF(int index);					//4����

	//void a(Node* n);
};

#endif

