#pragma once
#include "UserListOne.h"
#include "slwh/Base/UIBase.h"

/*
һ��ѹ�ְ�ť�Ľű�
*/


class UserListManager 
	: public UIBase
{
protected:
	Sprite*							m_head;
	ImageView*						m_headBk;
	Text*							m_name;
	ImageView*						m_vip0;
	ImageView*						m_vip;
	TextAtlas*						m_vipNum;
	TextAtlas*						m_scoreNum;
	Text*							m_onlineNum;						//��������
	Text*							m_totelJetNum;						//��ѹ��
	ImageView*						m_winBk;							//Ӯ������ı�����
	Text*							m_winNum;							//Ӯ������ı�����λK��
	Sprite*							m_betgold;							//���ͼƬ

	ListView*						m_userListView;
	std::vector<UserListOne*>		m_userlistinfo;

	float							m_actiontime;						//����ʱ��
	SCORE							m_getnum;							//�õ��Ľ��
	SCORE							m_targetGold;						//������Ŀ����
protected:
	//��ʼ��
	void Start();
	void setOnlineNum(int i);									//����������� 
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//ͷ�񱳾�����VIP���

	//��ٷ��޽�ҹ����� ���ô˺���
	void goldRollPlay(SCORE targetnum);
	//����������ʼ���� �� �����Ȫ���� 
	void goldRolldelay(float  t);
	void goldRollfunc(float  t);
public:
	//ˢ�½�����ʾ
	void refreshPlayerCoin(Ref* ref);
	//ˢ�½���vip��ʾ
	void refreshPlayerVIP(Ref* ref);
public:
	//ǣ�ߴ��� ���׳�m_betgoldָ��
	//Sprite* GetBetGold(){ return m_betgold; }
	void setTotelNum(SCORE score);								//������ѹ��
	//�����ҽ��
	void addUserElement(const slwhPlayerInfo &info);
	//��������б�
	void updateUserList(std::vector<slwhPlayerInfo> vec);
	//��ʼ������б�
	void InitUserList();
	//ɾ�����
	void deleteUserElement(int userid);
	//�����ѹ��
	void otherJet(int userid, long jetadd, SCORE nowscore, dword animalindex);
	//�����ȡ��ѹ��
	void othercancelJet(int userid, SCORE jetadd);
	//�����Լ���Ϣ    //����Ҫ�ӽ�ҹ���Ч����ʵ���ǵ��ۣ�����ı�ԭ�еĽṹ���ڴ˺����Ӹ����� �Դ���Ϊ�������Ҫ��λ�ù���
	void updateInfo(int code = 0);
	//������Ӯ������ѹ��
	void setWinScore(SCORE score, bool biscount);

	UserListManager();

	~UserListManager();
	//����
	void update(float t);
};



	