#ifndef _PlayerListManager_H_
#define _PlayerListManager_H_

#include "StakeItem.h"
#include "PlayerListOne.h"
#include "shark/Base/ViewBase.h"

/*
һ��ѹ�ְ�ť�Ľű�
*/


class PlayerListManager 
	: public ViewBase
{
protected:
	Node*                           playerInfo;
	Node*                           stakeInfo;
	Sprite*							m_head;
	ImageView*						m_headBk;
	Text*							m_name;
	ImageView*						m_vip0;
	ImageView*						m_vip;
	TextAtlas*						m_vipNum;
	Text*							m_scoreNum;
	Text*							m_onlineNum;						//��������
	Text*							m_totelJetNum;						//��ѹ��
	ImageView*						m_winBk;							//Ӯ������ı�����
	Text*							m_winNum;							//Ӯ������ı�����λK��
	Sprite*							m_betgold;							//���ͼƬ

	ListView*						m_playerListView;
	std::vector<PlayerListOne*>		m_playerlistinfo;
	//Ѻ����Ϣ����
	Text*					     	totalStake;
	ListView*                       stakeListView;
	std::vector<StakeItem*>         stakeListInfo;

	SCORE							m_getnum;							//�õ��Ľ��
	SCORE							m_targetGold;						//������Ŀ����
	int                             countStake;                         //��¼�м���Ѻ�ְ�ť
protected:
	//��ʼ��
	void setContent();
	void setOnlineNum(int i);									//����������� 
	void setTotelNum(SCORE score);								//������ѹ��
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//ͷ�񱳾�����VIP���

	//������Ѻ����
	void setTotalStake(int num);


	//�ٷ���ҹ����� ���ô˺���
	void goldRollPlay(SCORE targetnum);
	//����������ʼ���� �� �����Ȫ���� 
	void goldRolldelay(float  t);
	void goldRollfunc(float  t);


public:
	//�����ҽ��
	void addPlayerElement(const UserInfo_Shark &info);
	//��������б�
	void updatePlayerList(std::vector<UserInfo_Shark> vec);
	//��ʼ������б�
	void InitPlayerList();
	//ɾ�����
	void deleteUserElement(int userid);
	//�����ѹ��
	void otherJet(int userid, long jetadd, SCORE nowscore, dword animalindex);
	//�����ȡ��ѹ��
	void othercancelJet(int userid, SCORE jetadd);
	//�����Լ���Ϣ
	void updateInfo(int code = 0);

	//������Ӯ������ѹ��
	void setWinScore(SCORE score, gameState type);

	void cancelTotleNum();                                //�����һ�ֵ�������Ѻ������Ϣ
	
	PlayerListManager();

	~PlayerListManager();
public:
	//ˢ�½�����ʾ
	void refreshPlayerCoin(Ref* ref);
	//ˢ�½���vip��ʾ
	void refreshPlayerVIP(Ref* ref);
public:
	//��ʾѺ����Ϣ
	void showStakeInfo();

	//ɾ���Լ���Ѻ���б���Ϣ
	void deleteOwnStake();

public:
	//ת���б���ͼ 
	void changeView(int index);            //1.����б�2.Ѻע�б�


};


#endif 