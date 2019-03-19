#ifndef _OneOfStake_H_
#define _OneOfStake_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"

class OneOfStake : public  ViewBase
{
public:

	OneOfStake();
	~OneOfStake();

	CREATE_FUNC(OneOfStake);

	virtual void setContent();

	//��ť��Ӧ�¼�
	void BtEvent(Ref *pSender, TouchEventType type);
public:
	//����ѹ�ֵ�
	static int SetAddJet(int code = 0);
	//���ð�ť����
	void SetTag(int x){ m_tag = x; }
	//�õ���ť����
	int GetTag(){ return m_tag; }
	//���ñ���
	void SetBeiLv(int g);
	//�����ҵ�ѹ��
	void SetMyStake(SCORE g);
	//������ѹ��
	//void SetAllJetton(int g);
	//����ѹ��
	void AddStake();
	//�õ���ǰѹ�ֵ�
	SCORE GetStake(){ return m_stake; }
	//����������ѹ�ֳɹ� ˢ��ѹ��
	void UpdateStake(SCORE score);

	//�Ŵ�����
	void enlargeNum();

	//��ʾ��ת�Ĺ�Ȧ
	void showRing();
	//��ʾ��ť�ϵĹ�Ȧ
	void showCircle();

	//��ť��Ч
	void btnEffect();
	//������Ҷ�
	void GoldFly();
	//�ص�����
	void CallBack(Node* node);
	//����Ѻ�ְ�ť��־λ
	void resetBtn();
protected:
	Node*     pMainNode;

protected:

	static Vec2			m_targetPos[12];										//��ҷ���12��Ŀ��λ��
	SCORE			    m_stake;												//��ǰѺע����
	TextAtlas*			m_beilvNum;											   //������������
	TextAtlas*			m_mystakeNum;											//�ҵ�Ѻע��������
	Sprite*             m_ring;                                               //��ת��Ȧ
	Sprite*             m_circle;                                             //Ѻ����ʾ�Ĺ�Ȧ
	int					m_tag;												//���������ǵڼ�����ť		

	bool                stakeFlag;                                             //�ж�Ѻ���Ƿ�ɹ�

	static int			stakeAdd;												//ѹ������
public:
	static SCORE		MyAllTotel;											//�ҵ���ѹ��




};

#endif // _OneOfStake_H_
