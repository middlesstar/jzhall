#ifndef __OneOfJetton_H__
#define __OneOfJetton_H__

#include "slwh/Base/UIBase.h"


/*
һ��ѹ�ְ�ť�Ľű�
*/



class OneOfJetton : public UIBase
{
protected:
	static Vec2			m_sTarPos[15];										//��ҷ���15��Ŀ��λ��
	int					m_jet;												//��ǰѺע����
	TextAtlas*			m_beilvNum;											//������������
	TextAtlas*			m_myJetNum;											//�ҵ�Ѻע��������
	int					m_tag;												//���������ǵڼ�����ť		12=ׯ 13=�� 14=��			

	
	static int			JetAdd;												//ѹ������
public:
	static SCORE		MyAllTotel;											//�ҵ���ѹ��
    bool			BIsCanJet;										//�Ƿ����ѹ��	�������ݰ����ɹ����ձ�ɼ���ѹ��			
public:
	OneOfJetton();
	~OneOfJetton();

	//��ť��Ӧ�¼�
	void BtEvent(Ref *pSender, TouchEventType type);

protected:
	//��������������Ч
	void PopStarPlay();

	void GoldFlyStartPlay();

	void Start();

public:
	//����ѹ�ֵ� 0 = Ĭ�� ,1 = ��ʼ��
	static int SetAddJet(int code = 0); 
	//���ð�ť����
	void SetTag(int x){ m_tag = x; }
	//�õ���ť����
	int GetTag(){ return m_tag; }
	//���ñ���
	void SetBeiLv(int g);
	//�����ҵ�ѹ��
	void SetMyJetton(SCORE g);
	//������ѹ��
	//void SetAllJetton(int g);
	//����ѹ��
	void AddJet();
	//����������ѹ�ֳɹ� ˢ��ѹ��
	void UpdateJet(SCORE score);
	//�õ���ǰѹ��
	int GetJet(){ return m_jet; }
	
};

#endif

