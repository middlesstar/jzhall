#pragma once

#include "slwh/Base/UIBase.h"





class JetAndResManager :
	public UIBase
{
protected:
	//enum enumState
	//{
	//	Up = 20,
	//	Down = 0
	//};

	enum TimeCountTitle
	{
		enumInvalideta = -1,

		enumPleaseJet,
		enumCountJet,
		enumGameScene,
		enumJetScene,
		enumOpen,								//���ڿ���

		enumMax
	};



	//bool m_bIsRun;										//��������
	bool m_bIsOpen;										//�Ƿ��Ѿ������
	bool m_bIsEnble;									//�Ƿ���ã�������壩
	//��������
	//float m_speed;
	//int m_index;										//��ǰ����֡
	//enumState m_targetIndex;							//Ŀ��λ��

	int	m_nowTime;										//ѹ��ʣ��ʱ��
	int m_countjettime;									//ͳ�Ʒ���3��
	TimeCountTitle m_nowTitle;							//��ǰ����
protected:
	Node*			m_TimeCountNode;					//��ʱ���ڵ�
	Button*			m_jetimg;
	Node*			m_moveNode;							//�ƶ��ڵ�
	ImageView*		m_title;							//�±�С����
	TextAtlas*		m_timeCount;						//ʱ�䵹��ʱ
	Sprite*			m_dot;								//ʡ�Ժ�
	ActionInterval*	m_dotAc;							//ʡ�ԺŶ���
public:
	JetAndResManager();
	~JetAndResManager();
	//0���� 1ǿ�ƴ� 2ǿ������
	void MyJettonOut(int code = 0);						
	//�����������û��б�ѹ���������
	void Attach3Nodes(Node* Open, Node* Jet, Node* Userlist);
	//��ť��Ӧ�¼�
	void BtEvent(Ref* pSender, TouchEventType type);
	//�Ƿ�ɼ�
	void setVisible(bool b);							//�Ƿ�����
	//��������ʱ
	void StartPlay(int time);
	//�õ���ǰʣ��ʱ��
	int GetleftTime(){ return m_nowTime; }
	//��ʼ������λ��  ��Ҫ���׸������ʼ��λ��  ����ֵ���BUG
	void initposition();
	//��ֹͼƬ��ʧ,���Ƴ�ʱ����
	void inhideImg();
protected:
	virtual void onEnter() override;

	virtual void onExit() override;

	//����ʱ��UI
	void setTimeUI(int t);

	void Start();
	//��������
	void Running(float t);

	bool GetIsOpen();
	//ʱ��͵�ļ�ʱ��
	void updateDot_Time(float t);
	//2��λ�õ��л�
	void moveTimeCountTojet(bool b);					//�Ƿ��ƶ��ڵ㵽ѹ�ֽ���
	//���ñ���
	void setTitle(TimeCountTitle e);
	//�򿪻����
	void show(bool b);

};


