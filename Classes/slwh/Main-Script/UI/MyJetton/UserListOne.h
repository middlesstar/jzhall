#pragma once

#include "slwh/ResourcePath.h"

/*
	����б�Ԫ
*/


class UserListOne 
{
protected:
	ImageView*			m_rootNode;								//���ڵ�(ImageVirw��������ListViewռλ)
	ImageView*			m_head;									//ͷ��
	ImageView*			m_headBk;								//ͷ��߿�
	ImageView*			m_vip0;									//vip0
	ImageView*			m_vip;									//vip��ɫ
	TextAtlas*			m_vipNum;								//vip����
	Text*				m_name;									//�ǳ�
	ImageView*			m_winBk;								//Ӯ������ı�����
	Text*				m_winNum;								//Ӯ������ı�����λK��
	TextAtlas*			m_scoreNum;								//��������
	

	static UserListOne*	m_templateThis;							//ģ��


	PlayerInfo			m_userInfo;								//�����Ϣ

protected:
	UserListOne();
	
	void setWinScore(SCORE score, int type);					//type = 0 ѹ�ֽ׶� type = 1��Ӯ����
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//ͷ�񱳾�����VIP���
public:
	~UserListOne();
	PlayerInfo getuserinfo()const { return m_userInfo; }
	void setuserinfo(const PlayerInfo & info);
	
	ImageView* getRootNdoe(){ return m_rootNode; }

	


	static void initTemplate(ImageView* node);					//��ʼ��ģ����ܴ������(ģ����cocos studio��ƴ��)
	static UserListOne* create();
};



